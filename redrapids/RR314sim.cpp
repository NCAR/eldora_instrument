#include "RR314sim.h"
#include <iostream>

using namespace RedRapids;

//////////////////////////////////////////////////////////////////////
void* RR314sim::simThread(void* threadArgs) {

	RR314sim* pRR314sim = (RR314sim*)threadArgs;
	pRR314sim->simulate();

}

//////////////////////////////////////////////////////////////////////

RR314sim::RR314sim(RR314* pRR314, int gates, int startiq, int numiq, int nci) :
	_pRR314(pRR314), 
	_gates(gates), 
	_startiq(startiq),
	_numiq(numiq),		
	_nci(nci), 
	_pulseNum(0),
	_beamNum(0)
{

	// configure the channel numbers which
	// will receive the IQ and ABP data
	_iqChans.push_back(0);
	_abpChans.push_back(1);
	_abpChans.push_back(2);
	_abpChans.push_back(3);
	_iqChans.push_back(4);
	_abpChans.push_back(5);
	_abpChans.push_back(6);
	_abpChans.push_back(7);
	_iqChans.push_back(8);
	_abpChans.push_back(9);
	_abpChans.push_back(10);
	_abpChans.push_back(11);
	_iqChans.push_back(12);
	_abpChans.push_back(13);
	_abpChans.push_back(14);
	_abpChans.push_back(15);
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
	// establish the ABP data size
	_abp.resize(_gates+2);
	// establish the IQ data size
	_iq.resize(_numiq+2);
	for (int k = 2; k < _iq.size(); k++) {
		// create Is and Qs
		short I = -1;
		short Q = 1;
		_iq[k] = (I << 16) | (Q);
	}

	while (1) {
		for (int p = 0; p < _nci; p++) {
			for (int i = 0; i < _iqChans.size(); i++) {
				_iq[0] = _pulseNum;
				_iq[1] = _iqChans[i];
				_pRR314->newData(&_iq[0], _iqChans[i], _iq.size());
			}
			_pulseNum++;
		}
		for (int i = 0; i < _abpChans.size(); i++) {
			_abp[0] = _beamNum;
			_abp[1] = _abpChans[i];
			_pRR314->newData(&_abp[0], _abpChans[i], _abp.size());
		}
		usleep(50000);
		_beamNum++;
	}
}

//////////////////////////////////////////////////////////////////////

