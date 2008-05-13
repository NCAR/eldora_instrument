//
// Utilities for dealing with timetag, which represents a time in 
// microseconds since 1970/01/01 00:00:00 UTC.
//
#include <boost/date_time/posix_time/posix_time.hpp>

/// Convert a boost::posix_time::ptime to a timetag (microseconds since
/// 1970/01/01 00:00:00 UTC)
long long ptimeToTimetag(const boost::posix_time::ptime pTime);

/// Convert a timetag (microseconds since 1970/01/01 00:00:00 UTC) into
/// the equivalent boost::posix_time::ptime
boost::posix_time::ptime timetagToPtime(long long timetag);

