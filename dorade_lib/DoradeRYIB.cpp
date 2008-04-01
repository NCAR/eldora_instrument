#include <sstream>
#include <cstring>
#include <iomanip>
#include "DoradeRYIB.h"
/**
 * <p>Title: DoradeRYIB</p>
 * <p>Description: DORADE aircraft/ship information block</p>
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
    _lastPeakPower = grabFloat(data, 32, isLittleEndian);   // kW
    _scanRate = grabFloat(data, 36, isLittleEndian);        // deg/s
    _rayStatus = grabInt(data, 40, isLittleEndian);         // 0 normal, 1 transition, 2 bad

    //
    // Build a QDateTime, which is more usable than the five time pieces above
    //
    
    // Set the date assuming the ray time is close to now.
    QDateTime now = QDateTime::currentDateTime().toUTC();
    int nowYear = now.date().year();
    
    // difference in days between day of the year according to us and day of
    // the year according to the RYIB
    int daydiff = now.date().dayOfYear() - _julianDay;  // -365 to 365

    // correct year if the ray's date and our current date straddle the 
    // year boundary
    if (daydiff >= 364) {
        _rayDateTime = QDateTime(QDate(nowYear + 1, 1, 1),
                                 QTime(_hour, _minute, _second, _millisecond));
    }
    else if (daydiff < -364) {
        _rayDateTime = QDateTime(QDate(nowYear - 1, 12, 31),
                                 QTime(_hour, _minute, _second, _millisecond));
    }
    else {
        QDate rayDate = QDate(nowYear, 1, 1).addDays(_julianDay - 1);
        _rayDateTime = QDateTime(rayDate,
                                 QTime(_hour, _minute, _second, _millisecond));
    }
    
    // the time we built is UTC, so make sure the time zone in the QDateTime
    // reflects that
    _rayDateTime.setTimeSpec(Qt::UTC);
    
    // If we're not within five minutes of now, something's wrong...
    if (abs(now.secsTo(_rayDateTime)) > 300) {
        std::stringstream ss;
        ss << "Closest RYIB time we can get is " << now.secsTo(_rayDateTime) <<
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
        os << "peak power: " << _lastPeakPower << std::endl;
        os << "scan rate: " << _scanRate << std::endl;
        os << "ray status: " << _rayStatus << std::endl;
    }
    return os;
}
