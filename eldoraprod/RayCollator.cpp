#include "RayCollator.h"
#include <iostream>
#include <assert.h>

///////////////////////////////////////////////////////
RayCollator::RayCollator(int radarId) :
	_radarId(radarId), _discards(0) {
}
///////////////////////////////////////////////////////
RayCollator::~RayCollator() {

}
///////////////////////////////////////////////////////
bool RayCollator::addRay(EldoraDDS::Ray* r) {
	
	// put the ray int the queue. Flush the queue if this is
	// the fourth ray for it's rayNum, and there are
	// incompletely filled rayNums earlier than this one.
	
	// but first of all, if this belongs to a different radar,
	// don't consider it.
	if (r->radarId != _radarId)
		return false;

	int chan = r->hskp.chan;
	// sanity check on the channel
	assert (chan > 0 && chan < 5);

	long rayNum = r->hskp.rayNum;

	// if this is a new rayNum, initialize the 
	// _channelCount and _rays entries.
	if (_channelCount.find(rayNum) == _channelCount.end()) {
		// new rayNum, so add to time stamp list and
		// intialize _rays entry.
		_channelCount[rayNum] = 0;
		_rays[rayNum].resize(4);
		for (unsigned int i = 0; i < 4; i++)
			_rays[rayNum][i] = 0;
	}

	// bump the count of rays for this time stamp.
	_channelCount[rayNum]++;
	
	// save the ray
	assert(_rays[rayNum][chan-1] == 0);
	_rays[rayNum][chan-1] = r;

	// see if we have a complete set of rays, i.e. 
	// 4 channels. If so, check and remove incomplete 
	// ray sets for earlier timetags.
	if (_channelCount[rayNum] == 4) {
		// We have now collected 4 rays for this rayNum.
		// If there are rayNums ahead of this one
		// without 4 rays, then we have dropped a ray
		// or gotten then out of order. Discard those.
		while (_channelCount.size()) {
			if (_channelCount.begin()->second == 4)
				break;
			// remove these unused rays
			_discards += _channelCount.begin()->second;
			flush(_channelCount.begin()->first);
		}
	}

	return true;
}

///////////////////////////////////////////////////////
void RayCollator::flush(long rayNum) {
	for (unsigned int i = 0; i < 4; i++) {
		if (_rays[rayNum][i]) {
			_finishedRays.push_back(_rays[rayNum][i]);
		}
	}
	_rays.erase(_rays.find(rayNum));
	_channelCount.erase(_channelCount.find(rayNum));
}

///////////////////////////////////////////////////////
std::vector<EldoraDDS::Ray*>
RayCollator::raysReady() {
	// if there are four rays at the head of the queue,
	// return them. Otherwise, return an empty vector.
	
	std::vector<EldoraDDS::Ray*> r;
	
	if (_channelCount.size() > 0 ) {
		if (_channelCount.begin()->second == 4) {
			r.resize(4);
			// return the four rays at the head of the queue.
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
EldoraDDS::Ray* RayCollator::finishedRay() {
	EldoraDDS::Ray* p = 0;
	if (_finishedRays.size()) {
		p = _finishedRays[0];
		_finishedRays.erase(_finishedRays.begin());
	}
	return p;
}
///////////////////////////////////////////////////////
int RayCollator::discards() {
	int n = _discards;
	_discards = 0;
	return n;
}
