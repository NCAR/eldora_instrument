#include "ABPCollector.h"
#include <iostream>
///////////////////////////////////////////////////////
ABPCollector::ABPCollector(int radarId):
_radarId(radarId), _discards(0)
{
}
///////////////////////////////////////////////////////
ABPCollector::~ABPCollector() {
    
}
///////////////////////////////////////////////////////
void
ABPCollector::addPulse(EldoraDDS::Pulse* p) {
    if (p->radarId != _radarId)
        return;
    
    if (this->size() == 0) {
        this->push_back(p);
        return;
    }
    
    if ((p->timestamp != (*this)[0]->timestamp) || this->size() == 4) {
        // mismatch timestamp or already have 4, so 
        // count the current collection as discards, 
        // and remove them from the collator.
        _discards += this->size();
        flush();
    }
    
    // save this one
    this->push_back(p);
    return;
}

///////////////////////////////////////////////////////
void
ABPCollector::flush(){
    for (unsigned int i = 0; i < this->size(); i++) {
        _finishedPulses.push_back((*this)[i]);
    }
    this->clear();
}
///////////////////////////////////////////////////////
EldoraDDS::Pulse* ABPCollector::finishedPulse() {
    EldoraDDS::Pulse* p = 0;
    if (_finishedPulses.size()){
        p = _finishedPulses[0];
        _finishedPulses.erase(_finishedPulses.begin());
    }
    return p;
}
///////////////////////////////////////////////////////
int
ABPCollector::discards() {
    int n = _discards;
    _discards = 0;
    return n;
}
