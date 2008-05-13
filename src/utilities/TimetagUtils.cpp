#include "TimetagUtils.h"

/// Convert a timetag (microseconds since 1970/01/01 00:00 UTC) into
/// a boost::posix_time::ptime
boost::posix_time::ptime 
timetagToPtime(long long timetag) {
    return(boost::posix_time::from_time_t(timetag / 1000000) + 
            boost::posix_time::microseconds(timetag % 1000000));
}

/// Convert a boost::posix_time::ptime into a time tag (microseconds since 
/// 1970/01/01 00:00 UTC)
static const boost::posix_time::ptime UNIX_EPOCH_PTIME = 
    boost::posix_time::from_time_t(0);
long long 
ptimeToTimetag(const boost::posix_time::ptime pt) {
    boost::posix_time::time_duration tDiff = 
        pt - UNIX_EPOCH_PTIME; // since 1970/1/1 0:00 UTC
    long long usecs = ((long long)tDiff.fractional_seconds() * 1000000) /
        boost::posix_time::time_duration::ticks_per_second();
    return (((long long)tDiff.total_seconds()) * 1000000 + usecs);
}

