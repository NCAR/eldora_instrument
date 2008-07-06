#include <sstream>
#include <cstring>
#include <iomanip>
#include "DoradeVOLD.h"

using namespace boost::posix_time;  // ptime
using namespace boost::gregorian;   // date

/**
 * <p>Title: DoradeVOLD</p>
 * <p>Description: DORADE volume descriptor</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Revision: 1.3 $ $Date: 2004/04/13 17:05:50 $
 */
DoradeVOLD::DoradeVOLD(const unsigned char *data, unsigned int datalen, 
                       bool isLittleEndian) throw (DescriptorException) :
    DoradeDescriptor(data, datalen, isLittleEndian, "VOLD", 72) { 
    //
    // unpack
    //
    _formatVersionNum = grabShort(data, 8, isLittleEndian);
    _volumeNum = grabShort(data, 10, isLittleEndian);
    _maxDataRecLen = grabInt(data, 12, isLittleEndian);
    // get exactly 20 characters, either by silently truncating or by 
    // appending nulls
    _projectName = std::string((const char*)data + 16, 20);
    _projectName.resize(20);
    _year = grabShort(data, 36, isLittleEndian);
    // Year is often generated using a "struct tm", where it is recorded
    // as years since 1900.  Correct it now if that's the case.
    if (_year < 1900)
        _year += 1900;
    _month = grabShort(data, 38, isLittleEndian);
    _day = grabShort(data, 40, isLittleEndian);
    _hour = grabShort(data, 42, isLittleEndian);
    _minute = grabShort(data, 44, isLittleEndian);
    _second = grabShort(data, 46, isLittleEndian);
    // get exactly 8 characters for each of these, either by silently 
    // truncating or by appending nulls
    _flightOrIOP = std::string((const char*)data + 48, 8);
    _flightOrIOP.resize(8);
    _generatingFacility = std::string((const char*)data + 56, 8);
    _generatingFacility.resize(8);
    _genYear = grabShort(data, 64, isLittleEndian);
    // Year is often generated using a "struct tm", where it is recorded
    // as years since 1900.  Correct it now if that's the case.
    if (_genYear < 1900)
        _genYear += 1900;
    _genMonth = grabShort(data, 66, isLittleEndian);
    _genDay = grabShort(data, 68, isLittleEndian);
    _sensorCount = grabShort(data, 70, isLittleEndian);

    //
    // Build a boost::posix_time::ptime, which is much more usable than the six 
    // time pieces above
    //
    _volumeDateTime = ptime(date(_year, _month, _day),
                            time_duration(_hour, _minute, _second));

    //
    // debugging output
    //
    if (_verbose)
        std::cout << *this;
}

DoradeVOLD::DoradeVOLD(short formatVersionNum, short volumeNum, 
    int maxDataRecLen, std::string projectName, 
    boost::posix_time::ptime volumeDateTime, std::string flightOrIOP, 
    std::string generatingFacility, boost::gregorian::date genDate, 
    short sensorCount) : DoradeDescriptor("VOLD", 72),
    _formatVersionNum(formatVersionNum), _volumeNum(volumeNum), 
    _maxDataRecLen(maxDataRecLen), _projectName(projectName),
    _flightOrIOP(flightOrIOP), _generatingFacility(generatingFacility),
    _sensorCount(sensorCount) {
    // force _projectName to 20 characters, appending nulls if necessary.
    _projectName.resize(20);
    // force _flightOrIOP and _generatingFacility to 8 characters, appending 
    // nulls if necessary.
    _flightOrIOP.resize(8);
    _generatingFacility.resize(8);
    // dates and times
    setVolumeDateTime(volumeDateTime);
    setGenerationDate(genDate);
    
    if (_verbose)
        std::cout << *this;
}

std::ostream& 
DoradeVOLD::printTo(std::ostream& os) const
{
    DoradeDescriptor::printTo(os);
    if (_verbose) {
        os << "  format version: " << _formatVersionNum << std::endl;
        os << "  volume number: " << _volumeNum << std::endl;
        os << "  max data rec length: " << _maxDataRecLen << std::endl;
        os << "  project name: " << _projectName << std::endl;
        os << "  volume start time: " << _volumeDateTime << std::endl;
        os << "  flight or IOP: " << _flightOrIOP << std::endl;
        os << "  generating facility: " << _generatingFacility << std::endl;
        os << "  generation date: " << _genYear << "/" << _genMonth << 
            "/" << _genDay << std::endl;
        os << "  sensor descriptor count: " << _sensorCount << std::endl;
    }
    return os;
}

void
DoradeVOLD::setVolumeDateTime(ptime time) {
    _volumeDateTime = time;
    _year = _volumeDateTime.date().year();
    _month = _volumeDateTime.date().month();
    _day = _volumeDateTime.date().day();
    _hour = _volumeDateTime.time_of_day().hours();
    _minute = _volumeDateTime.time_of_day().minutes();
    _second = _volumeDateTime.time_of_day().seconds();
}

void
DoradeVOLD::setGenerationDate(date genDate) {
    _genYear = genDate.year();
    _genMonth = genDate.month();
    _genDay = genDate.day();
}

std::ostream&
DoradeVOLD::streamTo(std::ostream& os, bool asLittleEndian)
{
    putBytes(os, _descName.data(), 4, false); // no swapping for char data
    putInt(os, _descLen, asLittleEndian);
    putShort(os, _formatVersionNum, asLittleEndian);
    putShort(os, _volumeNum, asLittleEndian);
    putInt(os, _maxDataRecLen, asLittleEndian);
    putBytes(os, _projectName.data(), 20, false);   // no swapping for char data
    putShort(os, _year, asLittleEndian);
    putShort(os, _month, asLittleEndian);
    putShort(os, _day, asLittleEndian);
    putShort(os, _hour, asLittleEndian);
    putShort(os, _minute, asLittleEndian);
    putShort(os, _second, asLittleEndian);
    putBytes(os, _flightOrIOP.data(), 8, false);           // no swapping for char data
    putBytes(os, _generatingFacility.data(), 8, false);    // no swapping for char data
    putShort(os, _genYear, asLittleEndian);
    putShort(os, _genMonth, asLittleEndian);
    putShort(os, _genDay, asLittleEndian);
    putShort(os, _sensorCount, asLittleEndian);
    return os;
}
