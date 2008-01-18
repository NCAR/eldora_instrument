#include "RR314sim.h"
#include <iostream>

using namespace RedRapids;

//////////////////////////////////////////////////////////////////////
void* RR314sim::simThread(void* threadArgs) {

    RR314sim* pRR314sim = (RR314sim*)threadArgs;
    pRR314sim->simulate();
    return ((void*)1);
}

//////////////////////////////////////////////////////////////////////

RR314sim::RR314sim(RR314* pRR314, int gates, int startiq, int numiq, int nci) :
    _pRR314(pRR314), _gates(gates), _startiq(startiq), _numiq(numiq),
            _nci(nci), _pulseNum(0), _beamNum(0) {

    // configure the dmaChan numbers which
    // will receive the IQ and ABP data
    _iqChans.push_back(0);
    _abpChans.push_back(1);
    _iqChans.push_back(2);
    _abpChans.push_back(3);
    _iqChans.push_back(4);
    _abpChans.push_back(5);
    _iqChans.push_back(6);
    _abpChans.push_back(7);
}

//////////////////////////////////////////////////////////////////////

RR314sim::~RR314sim() {
}

//////////////////////////////////////////////////////////////////////
void RR314sim::start() {

    pthread_t thread;

    // spawn the thread that will deliver the bogus data
    pthread_create(&thread, NULL, simThread, (void*) this);

}

//////////////////////////////////////////////////////////////////////
void RR314sim::simulate() {
    // establish the ABP data size. Each apb is three signed ints,
    // plus the two identifier words.
    _abp.resize(3*_gates+3);

    // establish the IQ data size. numiq sets the number of iq pairs.
    // each iq is 2 bytes, but the interface to RR314 expects signed ints,
    // so resize it for the _numiq plus the 2 identifier words.
    _iq.resize(2*_numiq+6);

    while (1) {
        for (int p = 0; p < _nci; p++) {
            for (unsigned int i = 0; i < _iqChans.size(); i++) {
                // The firmware assigns channel numbers 1-8, 
                // corresponding to the receiver channels
                _iq[0] = 2*i + 1;
                _iq[1] = 0;
                // prt id
                _iq[2] = 0;
                _iq[3] = 0;
                // pulse number
                _iq[4] = (_pulseNum >> 16) & 0xffff;
                _iq[5] = _pulseNum & 0xffff;
                for (unsigned int k = 6; k < _iq.size(); k += 2) {
                    // create Is and Qs
                    _iq[k  ] = (rand() - RAND_MAX/2);
                    _iq[k] /= k*1.0;
                    _iq[k+1] = (rand() - RAND_MAX/2);
                    _iq[k+1] /= k*1.0;
                }
                 _pRR314->newIQData(&_iq[0], _iqChans[i], _iq.size());
            }
            _pulseNum++;
        }
        for (unsigned int i = 0; i < _abpChans.size(); i++) {
             /// The firmware assigns channel numbers 1-4, 
            /// corresponding to the receiver channels
            _abp[0] = i + 1;
            _abp[1] = 0;
            _abp[2] = _beamNum;
             for (unsigned int a = 3; a < _abp.size(); a += 3) {
                _abp[a+0] = rand() - RAND_MAX/2;
                _abp[a+1] = rand() - RAND_MAX/2;
                _abp[a+2] = rand();
            }
            _pRR314->newABPData(&_abp[0], _abpChans[i], _abp.size());
        }
        usleep(30000);
        _beamNum++;
    }
}

//////////////////////////////////////////////////////////////////////

