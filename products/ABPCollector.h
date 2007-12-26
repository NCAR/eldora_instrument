#ifndef ABPCOLLECTOR_H_
#define ABPCOLLECTOR_H_

#include "PulseTypeSupportC.h"
#include <vector>

/// Collect all ABP pulses for the same pulse and radarId. 
/// When the next out of sequence pulse number comes in, 
/// move the currently collected pulses 
/// to an invalid queue, and return them when requested.
/// Four pulses with identical pulse numbers and radarId are
/// the criteria for a complete set. Thus when the ABPCollect.size() 
/// equals 4, a complete set is available. The owner can remove them
/// directly from the vector, or reap them via finishedPulse().
class ABPCollector: public std::vector<EldoraDDS::Pulse*> {
    public:
        /// Constructor.
        /// @param radarId The radar identifier that we will search for
        ABPCollector(
                int radarId);
        /// Destructor
        virtual ~ABPCollector();
        /// Add another pulse
        /// @param pPulse The pulse
        void addPulse(EldoraDDS::Pulse* pPulse);
        /// Tell the collector that the current saved pulses 
        /// are no longer needed.
        void flush();
        /// @return Pulses that have been finished. They will end up here if
        /// flush() is called by the application, or if a sequence error was
        /// detected by addPulse(). In the latter case, _discards gets
        /// incremented. 0 is returned if none are available.
        EldoraDDS::Pulse* finishedPulse();
        /// @return the number of discarded pulses since the last time
        /// this function was called.
        int discards();
        
    protected:
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
