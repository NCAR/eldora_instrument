#ifndef ABPCOLLECTOR_H_
#define ABPCOLLECTOR_H_

#include "PulseTypeSupportC.h"
#include <vector>
#include <map>

/// Collect all ABP pulses for the same pulse and radarId. 
/// When the next out of sequence pulse number comes in, 
/// move the currently collected pulses 
/// to an invalid queue, and return them when requested.
/// Four pulses with identical pulse numbers and radarId are
/// the criteria for a complete set. Thus when the ABPCollect.size() 
/// equals 4, a complete set is available. The owner can remove them
/// directly from the vector, or reap them via finishedPulse().
class ABPCollector {
    public:
        /// Constructor.
        /// @param radarId The radar identifier that we will search for
        ABPCollector(int radarId);
        /// Destructor
        virtual ~ABPCollector();
        /// Add another pulse
        /// @param pPulse The pulse
        /// @return True if the pulse was for this radar, and thus
        /// kept by us. False if the pulse is not handled here.
        bool addPulse(EldoraDDS::Pulse* pPulse);
        /// @return If 4 matched pulses are ready, return them. Otherwise
        ////return an empty vector.
        std::vector<EldoraDDS::Pulse*> pulsesReady();
        /// @return Pulses that have been finished. They will end up here if
        /// flush() is called by the application, or if a sequence error was
        /// detected by addPulse(). In the latter case, _discards gets
        /// incremented. 0 is returned if none are available.
        EldoraDDS::Pulse* finishedPulse();
        /// @return the number of discarded pulses since the last time
        /// this function was called.
        int discards();
        
    protected:
    	/// Remove pulses associated with a given timestamp
    	/// from _pulses. remove the timestamp from _timestamps.
    	/// @param timestamp The pulse identifier.
    	void flush(long long timestamp);
    	/// An map keyed by timestamp, that counts the number of 
    	/// pulses collected for a given timestamp. When any entry
    	/// in _timestamps reached 4, then we have pulses from all
    	/// 4 channels available to process.
    	std::map<long long, int> _timestamps;
    	/// A pulse accumulator, to hold incoming pulses.
    	/// Collect all pulses for all 4 channels, for a given timestamp.
    	/// The vector is initialized to zero. When a pulse for a
    	/// channel is received, it is placed in the corresponding
    	/// element in the vector.
    	std::map<long long, std::vector<EldoraDDS::Pulse*> > _pulses;
        /// finished pulses are collected here. Call 
        /// finishedPulse() to reclaim them.
        std::vector<EldoraDDS::Pulse*> _finishedPulses;
        /// The radarId that we are processing
        int _radarId;
        /// The number of pulses that are discarded; i.e.
        /// moved to the _finishedPulses list by addPulse().
        int _discards;

};

#endif /*ABPCOLLECTOR_H_*/
