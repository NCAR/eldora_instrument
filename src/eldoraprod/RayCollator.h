#ifndef RAYCOLLATOR_H_
#define RAYCOLLATOR_H_

#include "EldoraDdsTypeSupportC.h"
#include <vector>
#include <map>

/// Collect all ABP rays for the same rayNum and radarId.
/// Rays will be also aggegated according to prtId.
/// When the next ray number is received that is out of sequence,
/// move the currently collected rays to an invalid queue, and
/// return them when requested.
/// The criteria for a complete set of rays is 4 rays with identical
/// rayIds, for all prtIds. Thus in the case of a single prt, we will have
/// 4 rays, in the case of dual prts, we will have 8, etc.
///
/// When a complete set has been collected, raysReady() will return a
/// non-zero length vector. At this point rayCollector does not know or
/// care anything more about those rays.
///
/// Rays that are collected but never fill a complete set are collected,
/// and returned via unusedRay(). The user should call unusedRay() repeatdly
/// to reclaim ownership of these rays. unusedRay() will return null when no more
/// are available.
///
/// Remember: a completed set of rays will be returned in a two dimensioned
/// vector. The first dimension represents the PRT id. The sub-vector will contain
/// 4 rays (one for each channel).
class RayCollator {
    public:
        /// Constructor.
        /// @param radarId The radar identifier that we will search for.
        /// @param The number of distinct prtIds. They must be zero based; i.e.
        /// the first prtId will be zero, etc.
        RayCollator(int radarId, int numPrtIds);
        /// Destructor
        virtual ~RayCollator();
        /// Add another ray
        /// @param pRay The ray
        /// @return True if the ray was for this radar, and thus
        /// kept by us. False if the ray is not handled here.
        bool addRay(EldoraDDS::Ray* pRay);
        /// @return If 4 matched rays are ready for all prtIds, return them.
        /// Otherwise return an empty vector.
        std::vector< std::vector< EldoraDDS::Ray*> > raysReady();
        /// @return Rays that have not been used, . They will end up here if
        /// flush() is called by the application, or if a sequence error was
        /// detected by addRay(). In the latter case, _discards gets
        /// incremented. 0 is returned if none are available.
        EldoraDDS::Ray* unusedRay();
        /// @return the number of discarded rays since the last time
        /// this function was called.
        int discards();

    protected:
    	/// Move rays associated with a given rayNum
    	/// from _rays to _finishedRays. Remove the rayNum from _rayNums.
    	/// @param rayNum The ray identifier.
    	void flush(long rayNum);
    	/// A map keyed by rayNum, that counts the number of
    	/// channels collected for a given rayNum for a given prtId. When any entry
    	/// in _channelCount reaches 4*_numPrtIds, then we have rays from all
    	/// 4 channels available to process.
    	std::map<long, int> _channelCount;
    	/// A ray accumulator, to hold incoming Rays. It is a map of
    	/// a two dimensional vector (i.e. array). The ray number indexes the map.
    	/// The array will collect multiple channels for a given prtId.
    	/// The vector is initialized to zero.  When a ray for a
    	/// channel is received, it is placed in the corresponding
    	/// element in the vector. If rays for a new ray number come in before
    	/// the previous ray number was completed, the previous rays are transferred
    	/// to _unusedRays.
    	std::map<long, std::vector< std::vector<EldoraDDS::Ray*> > > _rays;
        /// Unused rays are collected here. Call unusedRay() frequently
    	/// to reclaim them.
        std::vector< EldoraDDS::Ray*> _unusedRays;
        /// The radarId that we are processing
        int _radarId;
        /// The number of rays that are discarded; i.e.
        /// moved to the _finishedRays list by addRay().
        int _discards;
        /// The number of distinct prtIds
        int _numPrtIds;
        /// diagnostic dump
        void dump(std::string id);

};

#endif /*RAYCOLLATOR_H_*/
