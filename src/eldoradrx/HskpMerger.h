#ifndef HSKPMERGER_H_
#define HSKPMERGER_H_

#include <iostream>
#include <map>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "RR314.h"
#include "EldoraDdsC.h"

using namespace RedRapids;

/// RREntry keeps a pointer to an RRBuffer and an associated insert time
/// (which is set to current time when the object is instantiated).  This class
/// is only used in HskpMerger's list of RRBuffer-s waiting to be merged with
/// housekeeping.
class RREntry {
public:
    /// RREntry constructor.
    /// @param buf_p pointer to an RRBuffer awaiting housekeeping
    /// @param publish flag to tell whether the buffer should be published
    ///     after housekeeping is added
    /// @param capture flag to tell whether the buffer should be captured
    ///     after housekeeping is added
	/// @param textcapture flag to tell if captured data should be saved as text
    RREntry(RRBuffer* buf_p, bool publish, bool capture, 
    		bool textcapture) : 
        _rrbuf_p(buf_p), 
        _insertTime(boost::posix_time::microsec_clock::universal_time()),
        _publish(publish),
        _capture(capture),
        _textcapture(textcapture) {}
    /// Return the RRBuffer pointer.
    /// @return the RRBuffer pointer.
    RRBuffer* rrBuffer() const { return _rrbuf_p; }
    /// Return the insert time associated with this object.
    /// @return the insert time for this object.
    boost::posix_time::ptime insertTime() const { return _insertTime; }
    
    bool publish() { return _publish; }
    bool capture() { return _capture; }
    bool textcapture() { return _textcapture; }
private:
    RRBuffer* _rrbuf_p;
    boost::posix_time::ptime _insertTime;
    bool _publish;
    bool _capture;
    bool _textcapture;
};

/// HskpEntry keeps a pointer to an EldoraDDS::Housekeeping, an associated 
/// insert time (which is set to current time when the object is instantiated),
/// and a count of RRBuffer-s it has been matched with.  
/// This class is only used in HskpMerger's list of Housekeeping-s waiting to 
/// be merged with RRBuffer-s.
class HskpEntry {
public:
    /// HskpEntry constructor.
    /// @param buf_p EldoraDDS::Housekeeping waiting to be merged
    HskpEntry(EldoraDDS::Housekeeping* hskp) : 
        _hskp(hskp), 
        _insertTime(boost::posix_time::microsec_clock::universal_time()),
        _matchCount(0) {}
    /// Return a pointer to the Housekeeping.
    /// @return pointer to the Housekeeping.
    EldoraDDS::Housekeeping* hskp() const { return _hskp; }
    /// Return the insert time associated with this object.
    /// @return the insert time for this object.
    boost::posix_time::ptime insertTime() const { return _insertTime; }
    
    /// Return the current count of RRBuffer-s to which this housekeeping
    /// entry has been matched.
    /// @return the current match count.
    unsigned int matchCount() const { return _matchCount; }
    
    /// Increment the count of RRBuffer-s to which this housekeeping
    /// entry has been matched.
    /// @return the new match count.
    unsigned int incMatchCount() { return _matchCount++; }
private:
    EldoraDDS::Housekeeping* _hskp;
    boost::posix_time::ptime _insertTime;
    unsigned int _matchCount;
};

/// HskpMerger keeps a list of RRBuffer-s which are complete except for 
/// housekeeping, merges incoming houskeeping with them, and will call 
/// functions to publish and/or capture completed buffers.  If RRBuffers are 
/// kept too long without matching housekeeping, they are dropped.  The 
/// RRBuffer::returnBuffer() method is called for each buffer after it is
/// either completed or dropped.
class HskpMerger {
public:
    /// Instantiate a HskpMerger.
    /// @param maxLatency RRBuffer-s without matching housekeeping will be 
    ///     kept for at least maxLatency milliseconds before being dropped.
    /// @param publishFunction function to be called when an RRBuffer has had
    ///     housekeeping merged.
    HskpMerger(RR314* rr314, int maxLatencyMsec, 
        void (*publishFunction)(RREntry* rentry, EldoraDDS::Housekeeping* hskp)); 
    
    ~HskpMerger();
    
    /// Accept a RRBuffer which is awaiting housekeeping.  When housekeeping
    /// is added to an RRBuffer, the publish function (if non-null) will be 
    /// called with the completed buffer, then the capture function (if 
    /// non-null) will be called.
    /// @param rrbuf the new RRBuffer awaiting housekeeping
    /// @param publish a flag to tell if the buffer should be published
    ///     when housekeeping is merged
    /// @param capture a flag to tell if the buffer should be captured when
    ///     housekeeping is merged
    /// @param textcapture a flag to tell if captured data should be saved as
    ///		text
    void newRRBuffer(RRBuffer* rrbuf, bool publish, bool capture, 
    		bool textcapture);
    
    /// Accept new housekeeping and attempt to merge it with RRBuffer-s we
    /// have.  The Housekeeping we are given will be deleted when we are
    /// done with it.
    /// @param hskp the new housekeeping
    void newHskp(EldoraDDS::Housekeeping* hskp);
    
    /// Print stats to a stream (resets the status counters)
    void showStats(std::ostream& os);
    
private:
	/// Our associated RedRapids card
	RR314* _rr314;
	
    typedef std::multimap<unsigned int, RREntry*> UnmergedRREntryMap;
    /// Multimap of RRBuffer-s which are ready to be merged with housekeeping,
    /// with insert times for each one.  Entries are keyed by ray number.
    /// We use a multimap since there can be multiple RRBuffer-s for each
    /// ray number.
    UnmergedRREntryMap _unmergedRRBufs;
    
    typedef std::map<unsigned int, HskpEntry*> UnmergedHskpEntryMap;
    /// Map of Housekeeping-s which are ready to be merged with RRBuffer-s
    /// with insert times for each one.  Entries are keyed by ray number.
    UnmergedHskpEntryMap _unmergedHskps;
    
    /// lock for unmergedRRBufs and unmergedHskp, since they may be accessed 
    /// via multiple threads
    pthread_mutex_t _unmergedRRMutex;
    pthread_mutex_t _unmergedHskpMutex;
    
    /// Remove a single entry from the list of unmerged RRBuffer-s
    void removeRREntry(UnmergedRREntryMap::iterator it);
    
    /// Remove a single entry from the list of unmerged Housekeeping-s
    void removeHskpEntry(UnmergedHskpEntryMap::iterator it);

    /// Merge matching RRBuffer-s and Housekeeping-s at rayNum and send out the
    /// results.
    /// @param rayNum the ray number to test for matches
    /// @param newHskp is this call based on new housekeeping?
    /// @return the number of RRBuffer-s that were matched with housekeeping
    ///	    and sent out
    int mergeAndSend(unsigned int rayNum, bool newHskp);
    
    /// Clear RRBuffer-s and Housekeeping-s that we have kept for more than 
    /// _maxLatency milliseconds
    void clearOldEntries();
    
    /// maximum latency (in milliseconds) before incomplete RRBuffer-s may
    /// be dropped
    boost::posix_time::time_duration _maxLatency;
    
    /// function to publish completed RRBuffer-s
    void (*_publishFunction)(RREntry* rbuf, EldoraDDS::Housekeeping* hskp);

    /// How many RRBuffer-s arrived later than their housekeeping?
    unsigned int _lateABPCount;
    unsigned int _lateTSCount;
    
    /// How many RRBuffers have we dropped with no housekeeping match?
    unsigned int _droppedABPCount;
    unsigned int _droppedTSCount;
    
    /// How many housekeeping packets have we gotten with no matching 
    /// RRBuffer-s?
    unsigned int _droppedHskpCount;
    
    /// How many matched RRBuffer-s have we sent out?
    unsigned int _sentCount;
};
#endif /*HSKPMERGER_H_*/
