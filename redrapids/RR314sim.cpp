#include "RR314sim.h"
#include <iostream>

using namespace RedRapids;

//////////////////////////////////////////////////////////////////////
void*
RR314sim::simThread(void* threadArgs) {

  RR314sim* pRR314sim = (RR314sim*)threadArgs;
  pRR314sim->simulate();

}

//////////////////////////////////////////////////////////////////////

RR314sim::RR314sim(RR314* pRR314):
  _pRR314(pRR314)
{
  // establish the ABP data size
  _abp.resize(1000);
  // establish the IQ data size
  _iq.resize(200);

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
void
RR314sim::start() {

  pthread_t thread;

  // spawn the thread that will deliver the bogus data
  pthread_create(&thread, NULL, simThread, (void*) this);

}

//////////////////////////////////////////////////////////////////////
void
RR314sim::simulate() {
  
  while(1) {
    for (int i = 0; i < _iqChans.size(); i++) {
      _pRR314->newData(&_iq[0], _iqChans[i], _iq.size());
    }
    for (int i = 0; i < _abpChans.size(); i++) {
      _pRR314->newData(&_abp[0], _abpChans[i], _abp.size());
    }
    usleep(10000);
  }
}

//////////////////////////////////////////////////////////////////////

