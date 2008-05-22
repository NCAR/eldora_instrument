#include "RayCollator.h"
#include <iostream>
#include <assert.h>

///////////////////////////////////////////////////////
RayCollator::RayCollator(int radarId,
                         int numPrtIds) :
    _radarId(radarId), _discards(0), _numPrtIds(numPrtIds)
{
}
///////////////////////////////////////////////////////
RayCollator::~RayCollator()
{

}
///////////////////////////////////////////////////////
bool RayCollator::addRay(EldoraDDS::Ray* r)
{

    // put the ray int the queue. Flush the queue if this is
    // the fourth ray for it's rayNum, and there are
    // incompletely filled rayNums earlier than this one.

    // but first of all, if this belongs to a different radar,
    // don't consider it.
    if (r->radarId != _radarId)
        return false;

    int chan = r->hskp.chan;
    // sanity check on the channel
    assert(chan > 0 && chan < 5);

    long rayNum = r->hskp.rayNum;
    int prtId = r->prtId;

    // if this is a new rayNum, create new entries in _channelCount and _rays.
    if (_channelCount.find(rayNum) == _channelCount.end()) {
        // new rayNum, so add to time stamp list and
        // intialize the _rays entry.
        _channelCount[rayNum] = 0;
        _rays[rayNum].resize(_numPrtIds);
        for (int p = 0; p < _numPrtIds; p++) {
            // resize the vector for each prtId to hold 4 rays.
            _rays[rayNum][p].resize(4);
            // initialize each ray pointer to 0.
            for (unsigned int i = 0; i < 4; i++) {
                _rays[rayNum][p][i] = 0;
            }
        }
    }

    // bump the count of rays for this time stamp.
    _channelCount[rayNum] = _channelCount[rayNum] + 1;
    
    // insure that we haven't already gotten this ray.
    /// @todo This assert needs to be replaced with something less drastic
    assert(_rays[rayNum][prtId][chan-1] == 0);
    
    // Finally, save the ray
    _rays[rayNum][prtId][chan-1] = r;

    // see if we have a complete set of rays, i.e. 
    // 4 channels for each prtId. If so, check and remove 
    // incomplete ray sets for earlier timetags.
    if (_channelCount[rayNum] == 4*_numPrtIds) {
        // We have now collected 4 rays for this rayNum.
        // If there are rayNums before this one
        // without 4 rays, then we have dropped a ray
        // or gotten then out of order. Discard those.
        while (_channelCount.size()) {
            if (_channelCount.begin()->second == 4*_numPrtIds) {
                // found a set of rays that are completed and waiting to 
                // be used by the client.
                break;
            }
            // an incomplete set is at the head of the list. 
            _discards += _channelCount.begin()->second;
            // transfer them to the unused ray list.
            // flush will remove _channelCount.begin()->first
            // from _channelCount
            flush(_channelCount.begin()->first);
        }
    }

    return true;
}

///////////////////////////////////////////////////////
void RayCollator::flush(long rayNum)
{
    for (int p = 0; p < _numPrtIds; p++) {
        for (unsigned int i = 0; i < 4; i++) {
            // _rays[p][c] will be non-zero if used.
            if (_rays[rayNum][p][i]) {
                _unusedRays.push_back(_rays[rayNum][p][i]);
            }
        }
    }
    _rays.erase(_rays.find(rayNum));
    _channelCount.erase(_channelCount.find(rayNum));
}

///////////////////////////////////////////////////////
std::vector< std::vector< EldoraDDS::Ray*> > RayCollator::raysReady()
{
    // if there are four rays at the head of the queue,
    // return them. Otherwise, return an empty vector.

   std::vector< std::vector< EldoraDDS::Ray*> > r;

    if (_channelCount.size() > 0) {
        if (_channelCount.begin()->second == 4*_numPrtIds) {
            // return the rays at the head of the queue.
            r = _rays.begin()->second;
            // remove the rays from the queue
            _rays.erase(_rays.begin());
            // remove the channelCount entry
            _channelCount.erase(_channelCount.begin());
        }
    }

    return r;
}
///////////////////////////////////////////////////////
EldoraDDS::Ray* RayCollator::unusedRay()
{
    EldoraDDS::Ray* p = 0;
    if (_unusedRays.size()) {
        p = _unusedRays[0];
        _unusedRays.erase(_unusedRays.begin());
    }
    return p;
}
///////////////////////////////////////////////////////
int RayCollator::discards()
{
    int n = _discards;
    _discards = 0;
    return n;
}

///////////////////////////////////////////////////////
void
RayCollator::dump(std::string id) {
    std::cout << "******** " << id << " ********\n";
    std::cout << "   radarId:" << _radarId << "\n";
    std::map<long, int>::iterator pC;
    for (pC = _channelCount.begin(); pC != _channelCount.end(); pC++) {
        std::cout << "   _channelCount: " << pC->first << ":" << pC->second << "\n";
    }
    std::cout << "-------- " << id << " --------\n";
}
