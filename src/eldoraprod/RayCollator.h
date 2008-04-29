#ifndef RAYCOLLATOR_H_
#define RAYCOLLATOR_H_

#include "RayTypeSupportC.h"
#include <vector>
#include <map>

/// Collect all ABP rays for the same ray and radarId. 
/// When the next out of sequence ray number comes in, 
/// move the currently collected rays 
/// to an invalid queue, and return them when requested.
/// Four rays with identical ray numbers and radarId are
/// the criteria for a complete set. Thus when the ABPCollect.size() 
/// equals 4, a complete set is available. The owner can remove them
/// directly from the vector, or reap them via finishedRay().
class RayCollator {
    public:
        /// Constructor.
        /// @param radarId The radar identifier that we will search for
        RayCollator(int radarId);
        /// Destructor
        virtual ~RayCollator();
        /// Add another ray
        /// @param pRay The ray
        /// @return True if the ray was for this radar, and thus
        /// kept by us. False if the ray is not handled here.
        bool addRay(EldoraDDS::Ray* pRay);
        /// @return If 4 matched rays are ready, return them. Otherwise
        ////return an empty vector.
        std::vector<EldoraDDS::Ray*> raysReady();
        /// @return Rays that have been finished. They will end up here if
        /// flush() is called by the application, or if a sequence error was
        /// detected by addRay(). In the latter case, _discards gets
        /// incremented. 0 is returned if none are available.
        EldoraDDS::Ray* finishedRay();
        /// @return the number of discarded rays since the last time
        /// this function was called.
        int discards();
        
    protected:
    	/// Remove rays associated with a given rayNum
    	/// from _rays. remove the rayNum from _rayNums.
    	/// @param rayNum The ray identifier.
    	void flush(long rayNum);
    	/// A map keyed by rayNum, that counts the number of 
    	/// channels collected for a given rayNum. When any entry
    	/// in _channelCount reaches 4, then we have rays from all
    	/// 4 channels available to process.
    	std::map<long, int> _channelCount;
    	/// A ray accumulator, to hold incoming Rays.
    	/// Collect Rays for all 4 channels, for a given rayNum.
    	/// The vector is initialized to zero.  When a ray for a
    	/// channel is received, it is placed in the corresponding
    	/// element in the vector.
    	std::map<long, std::vector<EldoraDDS::Ray*> > _rays;
        /// finished rays are collected here. Call 
        /// finishedRay() to reclaim them.
        std::vector<EldoraDDS::Ray*> _finishedRays;
        /// The radarId that we are processing
        int _radarId;
        /// The number of rays that are discarded; i.e.
        /// moved to the _finishedRays list by addRay().
        int _discards;

};

#endif /*RAYCOLLATOR_H_*/
