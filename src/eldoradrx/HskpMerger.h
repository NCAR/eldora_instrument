#ifndef HSKPMERGER_H_
#define HSKPMERGER_H_

#include <map>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "RR314.h"
#include "EldoraDdsC.h"

using namespace RedRapids;

/// HskpMergerEntry a pointer to an RRBuffer and an associated insert time
/// (which is set to current time when the object is instantiated).  This class
/// is only used in HskpMerger's list of RRBuffer-s waiting to be merged with
/// housekeeping.
class HskpMergerEntry {
public:
    /// HskpMergerEntry constructor.
    /// @param buf_p pointer to an RRBuffer awaiting housekeeping
    /// @param publish flag to tell whether the buffer should be published
    ///     after housekeeping is added
    /// @param capture flag to tell whether the buffer should be captured
    ///     after housekeeping is added
    HskpMergerEntry(RRBuffer* buf_p, bool publish, bool capture, 
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
    HskpMerger(int maxLatencyMsec, 
        void (*publishFunction)(RRBuffer* rbuf, EldoraDDS::Housekeeping* hskp, 
        	  bool publish, bool capture, bool textcapture));
    
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
    
    /// How many RRBuffer-s have been dropped with no housekeeping match?
    unsigned int droppedBufCount() { return _droppedBufCount; }
    
    /// How many housekeeping packets have we gotten with no matching
    /// RRBuffer-s?
    unsigned int droppedHskpCount() { return _droppedHskpCount; }
private:
    typedef std::multimap<unsigned int, HskpMergerEntry*> UnmergedEntryMap;
    /// Multimap of RRBuffer-s which are ready to be merged with housekeeping,
    /// with insert times for each one.  Entries are keyed by ray number.
    UnmergedEntryMap _unmergedRRBufs;
    
    /// Remove a single entry from the list of unmerged RRBuffer-s
    void removeEntry(UnmergedEntryMap::iterator it);
    
    /// Clear RRBuffer-s that we have kept for more than _maxLatency
    /// milliseconds
    void removeOldBuffers();
    
    /// lock for unmergedRRBufs, since it may be accessed via multiple threads
    pthread_mutex_t _unmergedMutex;
    
    /// maximum latency (in milliseconds) before incomplete RRBuffer-s may
    /// be dropped
    boost::posix_time::time_duration _maxLatency;
    
    /// function to publish completed RRBuffer-s
    void (*_publishFunction)(RRBuffer* rbuf, EldoraDDS::Housekeeping* hskp, 
    		bool publish, bool capture, bool textcapture);
    
    /// How many RRBuffers have we dropped with no housekeeping match?
    unsigned int _droppedBufCount;
    
    /// How many housekeeping packets have we gotten with no matching 
    /// RRBuffer-s?
    unsigned int _droppedHskpCount;
};
#endif /*HSKPMERGER_H_*/
