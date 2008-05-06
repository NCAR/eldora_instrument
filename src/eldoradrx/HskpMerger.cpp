#include "HskpMerger.h"
using namespace boost::posix_time;

/// Convert a time tag (microseconds since 1970/01/01 00:00 UTC) into
/// a boost::posix_time::ptime
ptime timetagToPtime(long long timetag) {
    return(from_time_t(timetag / 1000000) + microseconds(timetag % 1000000));
}

/// Convert a boost::posix_time::ptime into a time tag (microseconds since 
/// 1970/01/01 00:00 UTC)
long long ptimeToTimetag(ptime pt) {
    time_duration tDiff = pt - from_time_t(0); // since 1970/1/1 0:00 UTC
    long long usecs = ((long long)tDiff.fractional_seconds() * 1000000) /
        time_duration::ticks_per_second();
    return (((long long)tDiff.total_seconds()) * 1000000 + usecs);
}

/// Compute ray number from a time tag (microseconds since 1970/01/01 00:00 
/// UTC), a transmit start time, and dwell time
unsigned int timetagToRayNum(long long timetag, ptime xmitStart, 
        time_duration dwellTime) {
    long long usecDiff = timetag - ptimeToTimetag(xmitStart);
    unsigned int usecsPerDwell = dwellTime.total_microseconds();
    return((usecDiff / usecsPerDwell) + 1); // ray count starts at 1
}

HskpMerger::HskpMerger(int maxLatencyMsec, 
        void (*publishFunction)(RRBuffer* rbuf, EldoraDDS::Housekeeping* hskp, 
        	  bool publish, bool capture, bool textcapture)) : 
    _maxLatency(milliseconds(maxLatencyMsec)),
    _publishFunction(publishFunction),
    _droppedBufCount(0),
    _droppedHskpCount(0) {
    pthread_mutex_init(&_unmergedMutex, NULL);
}

HskpMerger::~HskpMerger() {
    pthread_mutex_destroy(&_unmergedMutex);
}

void 
HskpMerger::newRRBuffer(RRBuffer* rrbuf, bool publish, bool capture, 
		bool textcapture) {
	return;
    // lock the list of unmerged RRBuffer-s, so nobody steps on us (and
    // vice versa) while we're adding this RRBuffer
    pthread_mutex_lock(&_unmergedMutex);
    HskpMergerEntry* entry = 
        new HskpMergerEntry(rrbuf, publish, capture, textcapture);
    _unmergedRRBufs.insert(std::pair<unsigned int, HskpMergerEntry*>(rrbuf->rayNum, entry));
    pthread_mutex_unlock(&_unmergedMutex);
    // Clear out any entries in _unmergedRRBufs which are older than
    // the max latency time
    removeOldBuffers();
}

void 
HskpMerger::newHskp(EldoraDDS::Housekeeping* hskp) {
    int nmatches = 0;
    // lock _unmergedRRBufs to prevent thread problems
    pthread_mutex_lock(&_unmergedMutex);

    // Figure out the housekeeping's ray number.  We use info from the first 
    // RRBuffer in our list, assuming that *all* RRBuffer-s in our list will 
    // have the same transmit start time and dwell duration.
    UnmergedEntryMap::iterator it = _unmergedRRBufs.begin();
    if (it == _unmergedRRBufs.end()) {
        _droppedHskpCount++;
        std::cerr << __FUNCTION__ << ": no RRBuffer-s; dropped hskp @ " <<
            timetagToPtime(hskp->timetag).time_of_day() << std::endl;
        pthread_mutex_unlock(&_unmergedMutex);
        return;
    }
    
    HskpMergerEntry* entry = it->second;
    RRBuffer* rrbuf = entry->rrBuffer();
    unsigned int hskpRayNum = timetagToRayNum(hskp->timetag, 
            rrbuf->xmitStartTime(), rrbuf->dwellDuration());
    
    // Get iterator bounds for all entries which match this housekeeping's 
    // ray number
    std::pair<UnmergedEntryMap::iterator, UnmergedEntryMap::iterator> range = 
        _unmergedRRBufs.equal_range(hskpRayNum);
    
    for (it = range.first; it != range.second; it++) {
        entry = it->second;
        rrbuf = entry->rrBuffer();
        // If the radar of the RRBuffer matches the radar of the 
        // housekeeping, we can publish & capture, and then remove
        // the RRBuffer from our list.
    	bool rrbufIsFore = (rrbuf->boardNumber() == 0);
    	bool radarMatch = 
    		(! strncmp(hskp->radarName, "FORE", 4) && rrbufIsFore) ||
    		(! strncmp(hskp->radarName, "AFT", 3) && ! rrbufIsFore);

    	std::cerr << "rrbuf is " << (rrbufIsFore ? "fore" : "aft") << 
    		", and hskp is '" << hskp->radarName << "'" << std::endl;
    	std::cerr << "ray num match @ " << rrbuf->rayNum << 
    		(radarMatch ? " with " : " without ") << "radar match" << std::endl;
    	if (radarMatch) {
    	    nmatches++;
        	_publishFunction(rrbuf, hskp, entry->publish(), entry->capture(),
        			entry->textcapture());
        	// We're done with this entry
        	removeEntry(it);
    	}
    }
    
    if (nmatches == 0) {
        _droppedHskpCount++;
        std::cerr << __FUNCTION__ << ": dropped unmatched hskp @ " <<
            hskpRayNum << std::endl;
    }
    
    pthread_mutex_unlock(&_unmergedMutex);
}

// Caller should hold the _unmergedMutex lock when calling this...
void
HskpMerger::removeEntry(UnmergedEntryMap::iterator it) {
    HskpMergerEntry* entry = it->second;
    _unmergedRRBufs.erase(it);
    entry->rrBuffer()->returnBuffer();
    delete entry;
}

void
HskpMerger::removeOldBuffers() {
    ptime now = microsec_clock::universal_time();
    
    pthread_mutex_lock(&_unmergedMutex);
    
    // Pass through our list, removing entries that are too old until the list 
    // is empty, or until we hit the first entry that is recent enough to keep
    for (UnmergedEntryMap::iterator it = _unmergedRRBufs.begin(); 
         it != _unmergedRRBufs.end(); it++) {
        HskpMergerEntry* entry = it->second;
        time_duration latency = now - entry->insertTime();
        if (latency > _maxLatency) {
            std::cerr << "@" << now.time_of_day() << ": Removing entry " << 
                entry->rrBuffer()->rayTime.time_of_day() <<
                " after " << to_simple_string(latency) << 
                " seconds waiting for housekeeping" << std::endl;
            // Clear this entry from our queue, and return the associated 
            // RRBuffer to its parent's free list.
            removeEntry(it);
            
            _droppedBufCount++;
        } else {
            break;
        }
    }
    pthread_mutex_unlock(&_unmergedMutex);    
}
