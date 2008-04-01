#include <iostream>
#include <sstream>
#include <cstring>
#include <iomanip>
#include "DoradeFRAD.h"
/**
 * <p>Title: DoradeFRAD</p>
 * <p>Description: DORADE aircraft/ship information block</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Revision: 1.3 $ $Date: 2004/04/13 17:05:50 $
 */
DoradeFRAD::DoradeFRAD(const unsigned char *data, unsigned int datalen, 
                       bool isLittleEndian) throw (DescriptorException) :
    DoradeDescriptor(data, datalen, isLittleEndian, "FRAD") { 
    //
    // We either expect a complete FRAD descriptor including data, or just the
    // 52-byte portion before the data (which the ELDORA housekeeper sends
    // out).
    //
    if (datalen != 52 && datalen != _descLen) {
        std::stringstream ss;
        ss << "Bad FRAD data size: " << datalen << "; expected 52 or " <<
            _descLen;
        throw DescriptorException(ss.str());
    }
    //
    // unpack
    //
    _dataSystemStatus = grabInt(data, 8, isLittleEndian);
    char tmpName[9];
    tmpName[8] = '\0';
    memcpy(tmpName, data + 12, 8);
    _radarName = tmpName;
    _testPulseLevel = grabFloat(data, 20, isLittleEndian);
    _testPulseDist = grabFloat(data, 24, isLittleEndian);
    _testPulseWidth = grabFloat(data, 28, isLittleEndian);
    _testPulseFreq = grabFloat(data, 32, isLittleEndian);
    _testPulseAttenuation = grabShort(data, 36, isLittleEndian);
    _testPulseFnum = grabShort(data, 38, isLittleEndian);
    _noisePower = grabFloat(data, 40, isLittleEndian);
    _rayCount = grabInt(data, 44, isLittleEndian);
    _firstRecordedGate = grabShort(data, 48, isLittleEndian);
    _lastRecordedGate = grabShort(data, 50, isLittleEndian);

    //
    // debugging output
    //
    if (_verbose)
        std::cout << *this;
}

std::ostream& 
DoradeFRAD::printTo(std::ostream& os) const
{
    DoradeDescriptor::printTo(os);
    if (_verbose) {
        os << "data system status: " << _dataSystemStatus << std::endl;
        os << "radar name: " << _radarName << std::endl;
        os << "test pulse level: " << _testPulseLevel << std::endl;
        os << "test pulse start: " << _testPulseDist << std::endl;
        os << "test pulse width: " << _testPulseWidth << std::endl;
        os << "test pulse freq." << _testPulseFreq << std::endl;
        os << "test pulse attenuation: " << _testPulseAttenuation << 
            std::endl;
        os << "test pulse fnum: " << _testPulseFnum << std::endl;
        os << "noise power: " << _noisePower << std::endl;
        os << "ray count: " << _rayCount << std::endl;
        os << "first recorded gate: " << _firstRecordedGate << std::endl;
        os << "last recorded gate: " << _lastRecordedGate << std::endl;
    }
    return os;
}
