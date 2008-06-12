#include <sstream>
#include <cstring>
#include <iomanip>
#include "DoradeRYIB.h"

using namespace boost::posix_time;  // ptime
using namespace boost::gregorian;   // date

/**
 * <p>Title: DoradeRYIB</p>
 * <p>Description: DORADE aircraft/ship information block</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Revision: 1.3 $ $Date: 2004/04/13 17:05:50 $
 */
DoradeRYIB::DoradeRYIB() : DoradeDescriptor("RYIB", 44) {
    if (_verbose)
        std::cout << *this;
}

DoradeRYIB::DoradeRYIB(const unsigned char *data, unsigned int datalen, 
                       bool isLittleEndian) throw (DescriptorException) :
    DoradeDescriptor(data, datalen, isLittleEndian, "RYIB", 44) { 
    //
    // unpack
    //
    _sweepNumber = grabInt(data, 8, isLittleEndian);
    _julianDay = grabInt(data, 12, isLittleEndian);
    _hour = grabShort(data, 16, isLittleEndian);
    _minute = grabShort(data, 18, isLittleEndian);
    _second = grabShort(data, 20, isLittleEndian);
    _millisecond = grabShort(data, 22, isLittleEndian);
    _azimuth = grabFloat(data, 24, isLittleEndian);
    _elevation = grabFloat(data, 28, isLittleEndian);
    _peakXmitPower = grabFloat(data, 32, isLittleEndian);   // kW
    _scanRate = grabFloat(data, 36, isLittleEndian);        // deg/s
    _rayStatus = grabInt(data, 40, isLittleEndian);         // 0 normal, 1 transition, 2 bad

    //
    // Build a boost::posix_time::ptime, which is much more usable than the five 
    // time pieces above
    //
    
    // Get time of day for the ray
    int fracSeconds = _millisecond * (time_duration::ticks_per_second() / 1000);
    time_duration rayTimeOfDay(_hour, _minute, _second, fracSeconds);
    
    // Get current time (and year) according to our clock.
    ptime now = second_clock::universal_time();
    
    // difference in days between day of the year according to us and day of
    // the year according to the RYIB
    int daydiff = now.date().day_of_year() - _julianDay;  // -365 to 365

    // Assign the year from our clock to the ray. If the ray's day-of-year 
    // disagrees significantly with our day-of-year, assume that we're 
    // near a year boundary and need to add or subtract a year when 
    // assigning to the ray.
    int rayYear = now.date().year();
    if (daydiff >= 364)
        rayYear++;
    else if (daydiff < -364)
        rayYear--;

    date rayDate = date(rayYear, 1, 1) + date_duration(_julianDay - 1);
    _rayDateTime = ptime(rayDate, rayTimeOfDay);
    
    // If we're not within five minutes of now, something's wrong...
    int diffSecs = (_rayDateTime - now).total_seconds();
    if (abs(diffSecs) > 300) {
        std::stringstream ss;
        ss << "Closest RYIB time we can get is " << diffSecs <<
            " seconds from now";
        throw DescriptorException(ss.str());
    }

    //
    // debugging output
    //
    if (_verbose)
        std::cout << *this;
}

std::ostream& 
DoradeRYIB::printTo(std::ostream& os) const
{
    DoradeDescriptor::printTo(os);
    if (_verbose) {
        os << "sweep number: " << _sweepNumber << std::endl;
        os << "Julian day: " << _julianDay << std::endl;
        os << "hh:mm:ss.mmm: ";
        os << std::setw(2) << std::setfill('0') << _hour << ":";
        os << std::setw(2) << std::setfill('0') << _minute << ":";
        os << std::setw(2) << std::setfill('0') << _second << ".";
        os << std::setw(3) << std::setfill('0') << _millisecond << std::endl;
        os << "azimuth: " << _azimuth << std::endl;
        os << "elevation: " << _elevation << std::endl;
        os << "peak power: " << _peakXmitPower << std::endl;
        os << "scan rate: " << _scanRate << std::endl;
        os << "ray status: " << _rayStatus << std::endl;
    }
    return os;
}

void
DoradeRYIB::setRayDateTime(ptime time) {
    _rayDateTime = time;
    // In addition to setting _rayDateTime, we need to set each of the 
    // individual time pieces.
    _julianDay = time.date().day_of_year();
    _hour = time.time_of_day().hours();
    _minute = time.time_of_day().minutes();
    _second = time.time_of_day().seconds();
    _millisecond = time.time_of_day().total_milliseconds() % 1000;
}

std::ostream&
DoradeRYIB::streamTo(std::ostream& os, bool asLittleEndian)
{
    putBytes(os, _descName.data(), 4, false); // no swapping for char data
    putInt(os, _descLen, asLittleEndian);
    putInt(os, _sweepNumber, asLittleEndian);
    putInt(os, _julianDay, asLittleEndian);
    putShort(os, _hour, asLittleEndian);
    putShort(os, _minute, asLittleEndian);
    putShort(os, _second, asLittleEndian);
    putShort(os, _millisecond, asLittleEndian);
    putFloat(os, _azimuth, asLittleEndian);
    putFloat(os, _elevation, asLittleEndian);
    putFloat(os, _peakXmitPower, asLittleEndian);
    putFloat(os, _scanRate, asLittleEndian);
    putInt(os, _rayStatus, asLittleEndian);
    return os;
}
