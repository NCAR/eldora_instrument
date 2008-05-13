#include "HskpMerger.h"
#include <limits.h> // for UINT_MAX
#include <TimetagUtils.h>
using namespace boost::posix_time;

/// Compute ray number from a time tag (microseconds since 1970/01/01 00:00 
/// UTC), a transmit start time, and dwell time.  If the given time is
/// before the transmit start time, return BAD_RAYNUM.
static const unsigned int BAD_RAYNUM = UINT_MAX;
static unsigned int 
timetagToRayNum(long long timetag, ptime xmitStart, time_duration dwellTime) {
    long long usecDiff = timetag - ptimeToTimetag(xmitStart);
    if (usecDiff < 0)
        return BAD_RAYNUM;
    unsigned int usecsPerDwell = dwellTime.total_microseconds();
    return(usecDiff / usecsPerDwell); // ray count starts at 0
}

HskpMerger::HskpMerger(RR314* rr314, int maxLatencyMsec, 
        void (*publishFunction)(RREntry* rentry, EldoraDDS::Housekeeping* hskp)) : 
    _rr314(rr314),
    _maxLatency(milliseconds(maxLatencyMsec)),
    _publishFunction(publishFunction),
    _droppedBufCount(0),
    _droppedHskpCount(0) {
    pthread_mutex_init(&_unmergedRRMutex, NULL);
    pthread_mutex_init(&_unmergedHskpMutex, NULL);
}

HskpMerger::~HskpMerger() {
    pthread_mutex_destroy(&_unmergedRRMutex);
    pthread_mutex_destroy(&_unmergedHskpMutex);
}

void 
HskpMerger::newRRBuffer(RRBuffer* rrbuf, bool publish, bool capture, 
		bool textcapture) {
    RREntry* entry = new RREntry(rrbuf, publish, capture, textcapture);
    // lock the list of unmerged RRBuffer-s, so nobody steps on us (and
    // vice versa) while we're adding this RRBuffer
    pthread_mutex_lock(&_unmergedRRMutex);
    _unmergedRRBufs.insert(std::pair<unsigned int, RREntry*>(rrbuf->rayNum, entry));
    pthread_mutex_unlock(&_unmergedRRMutex);
    
//    // As a rule, housekeeping should arrive *after* the associated RRBuffer-s,
//    // so we expect zero matches from this mergeAndSend().
//    int nsent = mergeAndSend(rrbuf->rayNum);
//    if (nsent != 0)
//        std::cerr << __FUNCTION__ << ": housekeeping arrived before RRBuf " <<
//            rrbuf->rayNum << ", dev " << rrbuf->boardNumber() <<
//            ", channel " << rrbuf->chanId << std::endl;
    
    // Clear out any entries which are older than the max latency time
    clearOldEntries();
}

void 
HskpMerger::newHskp(EldoraDDS::Housekeeping* hskp) {
    unsigned int rayNum = timetagToRayNum(hskp->timetag, _rr314->xmitStartTime(),
    		_rr314->dwellDuration());
    if (rayNum == BAD_RAYNUM) {
        std::cerr << __FUNCTION__ << ": unexpected hskp time " << 
            timetagToPtime(hskp->timetag).time_of_day() <<
            " is before xmit start time " << 
            _rr314->xmitStartTime().time_of_day() << std::endl;
        return;
    }
    
    // lock the list and add this housekeeping, then unlock
    pthread_mutex_lock(&_unmergedHskpMutex);
    _unmergedHskps[rayNum] = new HskpEntry(hskp);
    pthread_mutex_unlock(&_unmergedHskpMutex);
    
    // If anything matches now, send it out
    mergeAndSend(rayNum);
    
    // Clear out any entries which are older than the max latency time
    clearOldEntries();
}

int
HskpMerger::mergeAndSend(unsigned int rayNum) {
	pthread_mutex_lock(&_unmergedRRMutex);
	pthread_mutex_lock(&_unmergedHskpMutex);

	int nmatches = 0;

	if ((_unmergedRRBufs.find(rayNum) == _unmergedRRBufs.end()) || 
	    (_unmergedHskps.find(rayNum) == _unmergedHskps.end())) {
		pthread_mutex_unlock(&_unmergedRRMutex);
		pthread_mutex_unlock(&_unmergedHskpMutex);
		return 0;
	}

	// Get the Housekeeping for this rayNum
    HskpEntry* hentry = _unmergedHskps.find(rayNum)->second;
	EldoraDDS::Housekeeping* hskp = hentry->hskp();
    pthread_mutex_unlock(&_unmergedHskpMutex); // done with the hskp list
	
    // Get iterator bounds for all RRBuffer entries which match this ray number
	typedef UnmergedRREntryMap::iterator UREMIterator;
    std::pair<UREMIterator, UREMIterator> range = _unmergedRRBufs.equal_range(rayNum);
    
    for (UREMIterator it = range.first; it != range.second; /* inc in loop */) {
        // Keep the current iterator and increment now, since incrementing
        // after "removeRREntry()" seems to create intermittent problems.
        UREMIterator current = it++;
        RREntry* rentry = current->second;
        _publishFunction(rentry, hskp);
        // We're done with this entry
        removeRREntry(current);
        nmatches++;
   }
    
    pthread_mutex_unlock(&_unmergedRRMutex);
    return nmatches;
}

// Caller must hold the _unmergedRRMutex lock when calling this...
void
HskpMerger::removeRREntry(UnmergedRREntryMap::iterator it) {
    RREntry* entry = it->second;
    entry->rrBuffer()->returnBuffer();
    delete entry;
    _unmergedRRBufs.erase(it);
}

// Caller must hold the _unmergedHskpMutex lock when calling this...
void
HskpMerger::removeHskpEntry(UnmergedHskpEntryMap::iterator it) {
	HskpEntry* entry = it->second;
	delete entry->hskp();
	delete entry;
    _unmergedHskps.erase(it);
}

void
HskpMerger::clearOldEntries() {
    ptime now = microsec_clock::universal_time();
    
    // Pass through our RRBuffer list, removing entries that are too old until 
    // the list is empty, or until we hit the first entry that is recent enough 
    // to keep
    pthread_mutex_lock(&_unmergedRRMutex);
    for (UnmergedRREntryMap::iterator it = _unmergedRRBufs.begin(); 
         it != _unmergedRRBufs.end(); /* incremented in loop */) {
        // Increment the iterator now (and keep its current value), since 
        // we'll be removing the current entry, and doing "it++" *after*
        // the removal seems to give intermittent problems.
        UnmergedRREntryMap::iterator current = it++;
        RREntry* entry = current->second;
        time_duration latency = now - entry->insertTime();
        if (latency > _maxLatency) {
//            std::cerr << "@" << now.time_of_day() << ": Removing entry " << 
//                _rr314->boardNumber() << "/" << it->first << "/" << 
//                entry->rrBuffer()->dmaChan << " after " << latency << 
//                " seconds waiting for housekeeping" << std::endl;
            // Clear this entry from our list
            removeRREntry(current);
            
            _droppedBufCount++;
        } else {
            break;
        }
    }
    pthread_mutex_unlock(&_unmergedRRMutex);   
    
    // Pass through our Housekeeping list, removing entries that are too old 
    // until the list is empty, or until we hit the first entry that is recent 
    // enough to keep
    pthread_mutex_lock(&_unmergedHskpMutex);
    for (UnmergedHskpEntryMap::iterator it = _unmergedHskps.begin(); 
         it != _unmergedHskps.end(); /* incremented in loop */) {
        // Increment the iterator now (and keep its current value), since 
        // we may remove the current entry, and doing "it++" *after*
        // the removal seems to give intermittent problems.
        UnmergedHskpEntryMap::iterator current = it++;
        HskpEntry* entry = current->second;
        time_duration latency = now - entry->insertTime();
        if (latency > _maxLatency) {
            // Clear this entry from our list
            removeHskpEntry(current);
        } else {
            break;
        }
    }
    pthread_mutex_unlock(&_unmergedHskpMutex);    
}
