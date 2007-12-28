#include "PulseCollator.h"
#include <iostream>
#include <assert.h>

///////////////////////////////////////////////////////
PulseCollator::PulseCollator(int radarId) :
	_radarId(radarId), _discards(0) {
}
///////////////////////////////////////////////////////
PulseCollator::~PulseCollator() {

}
///////////////////////////////////////////////////////
bool PulseCollator::addPulse(EldoraDDS::Pulse* p) {
	
	// put the pulse int the queue. Flush the queue if this is
	// the foruth pulse for it's timestamp, and there are
	// incompletely filled timestamps earlier than this one.
	
	// but first of all, if this belongs to a different radar,
	// don't consider it.
	if (p->radarId != _radarId)
		return false;

	int chan = p->chan;
	// sanity check on the channel
	assert (chan > 0 && chan < 5);

	long long timestamp = p->timestamp;

	// if this is a new timestamp, initialize the 
	// _timestamps and _pulses entries.
	if (_timestamps.find(timestamp) == _timestamps.end()) {
		// new timestamp, so add to time stamp list and
		// intialize _pulses entry.
		_timestamps[timestamp] = 0;
		_pulses[timestamp].resize(4);
		for (unsigned int i = 0; i < 4; i++)
			_pulses[timestamp][i] = 0;
	}

	// bump the count of pulses for this time stamp.
	_timestamps[timestamp]++;
	
	// save the pulse
	assert(_pulses[timestamp][chan-1] == 0);
	_pulses[timestamp][chan-1] = p;

	// see if we have a complete set of pulses, i.e. 
	// 4 channels. If so, check and remove incomplete 
	// pulse sets for earlier timetags.
	if (_timestamps[timestamp] == 4) {
		// We have now collected 4 pulses for this timestamp.
		// If there are timestamps ahead of this one
		// without 4 pulses, then we have dropped a pulse
		// or gotten then out of order. Discard those.
		while (_timestamps.size()) {
			if (_timestamps.begin()->second == 4)
				break;
			// remove these unused pulses
			_discards += _timestamps.begin()->second;
			flush(_timestamps.begin()->first);
		}
	}

	return true;
}

///////////////////////////////////////////////////////
void PulseCollator::flush(long long timestamp) {
	for (unsigned int i = 0; i < 4; i++) {
		if (_pulses[timestamp][i]) {
			_finishedPulses.push_back(_pulses[timestamp][i]);
		}
	}
	_pulses.erase(_pulses.find(timestamp));
	_timestamps.erase(_timestamps.find(timestamp));
}

///////////////////////////////////////////////////////
std::vector<EldoraDDS::Pulse*>
PulseCollator::pulsesReady() {
	// if there are four pulses at the head of the queue,
	// return them. Otherwise, return an empty vector.
	
	std::vector<EldoraDDS::Pulse*> r;
	
	if (_timestamps.size() > 0 ) {
		if (_timestamps.begin()->second == 4) {
			r.resize(4);
			// return the four pulses at the head of the queue.
			r = _pulses.begin()->second;
			// remove the pulses from the queue
			_pulses.erase(_pulses.begin());
			// remove the timestamp entry
			_timestamps.erase(_timestamps.begin());
			
		}
	}
	
	return r;
}
///////////////////////////////////////////////////////
EldoraDDS::Pulse* PulseCollator::finishedPulse() {
	EldoraDDS::Pulse* p = 0;
	if (_finishedPulses.size()) {
		p = _finishedPulses[0];
		_finishedPulses.erase(_finishedPulses.begin());
	}
	return p;
}
///////////////////////////////////////////////////////
int PulseCollator::discards() {
	int n = _discards;
	_discards = 0;
	return n;
}
