#include <sstream>
#include <cstring>
#include <iomanip>
#include "DoradeRYIB.h"

using namespace boost::posix_time;  // ptime
using namespace boost::gregorian;   // date

/**
 * <p>Title: DoradeRYIB</p>
 * <p>Description: DORADE ray information block</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Revision: 1.3 $ $Date: 2004/04/13 17:05:50 $
 */
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
    // Keep a boost::posix_time::time_duration, which is more usable than 
    // the four time pieces above
    //
    int fracSeconds = _millisecond * (time_duration::ticks_per_second() / 1000);
    _rayTime = time_duration(_hour, _minute, _second, fracSeconds);

    //
    // debugging output
    //
    if (_verbose)
        std::cout << *this;
}

DoradeRYIB::DoradeRYIB(boost::posix_time::ptime rayDateTime, int sweepNumber,
    float azimuth, float elevation, float peakXmitPower, float scanRate,
    int rayStatus) : 
    DoradeDescriptor("RYIB", 44),
    _sweepNumber(sweepNumber), _azimuth(azimuth), _elevation(elevation),
    _peakXmitPower(peakXmitPower), _scanRate(scanRate), _rayStatus(rayStatus) {
    setRayDateTime(rayDateTime);
    if (_verbose)
        std::cout << *this;
}

std::ostream& 
DoradeRYIB::printTo(std::ostream& os) const
{
    DoradeDescriptor::printTo(os);
    if (_verbose) {
        os << "sweep number: " << _sweepNumber << std::endl;
        os << "day of year: " << _julianDay << std::endl;
        os << "time of day: ";
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
    _rayTime = time.time_of_day();
    // In addition to setting _rayTime, we need to set each of the 
    // individual time pieces.
    _julianDay = time.date().day_of_year(); // we only keep the day of the year
    _hour = _rayTime.hours();
    _minute = _rayTime.minutes();
    _second = _rayTime.seconds();
    _millisecond = _rayTime.total_milliseconds() % 1000;
}

ptime
DoradeRYIB::getRayDateTime(ptime volumeStart) const {
    // Get our year from the volume start time
    int rayYear = volumeStart.date().year();
    // If our day of the year is less than the day of the year from the
    // volume start, we've crossed the year boundary, so increment our year.
    if (_julianDay < volumeStart.date().day_of_year())
        rayYear++;
    
    // Calculate our date using the year and our day of year
    date_duration daysFromJan1(_julianDay - 1);
    date rayDate(rayYear, 1, 1);
    rayDate += daysFromJan1;
    
    // Finally, put together and return the whole date/time
    ptime rayDateTime(rayDate, _rayTime);
    return rayDateTime;
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
