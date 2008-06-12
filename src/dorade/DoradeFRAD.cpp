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
 * @version $Id$
 */
DoradeFRAD::DoradeFRAD(const unsigned char *data, unsigned int datalen, 
                       bool isLittleEndian, bool headerOnly) 
    throw (DescriptorException) :
    DoradeDescriptor(data, datalen, isLittleEndian, "FRAD") { 
    //
    // We either expect a complete FRAD descriptor including data, or just the
    // 52-byte portion before the data (which the ELDORA housekeeper sends
    // out).
    //
    unsigned int expectedLen = headerOnly ? 52 : _descLen;
    if (datalen < expectedLen) {
        std::stringstream ss;
        ss << "Bad FRAD data size: " << datalen << 
            " is less than the expected " << expectedLen;
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
    _testPulsePower = grabFloat(data, 20, isLittleEndian);
    _testPulseStart = grabFloat(data, 24, isLittleEndian);
    _testPulseWidth = grabFloat(data, 28, isLittleEndian);
    _testPulseFreq = grabFloat(data, 32, isLittleEndian);
    _testPulseAttenuation = grabShort(data, 36, isLittleEndian);
    _testPulseFNum = grabShort(data, 38, isLittleEndian);
    _noisePower = grabFloat(data, 40, isLittleEndian);
    _rayCount = grabInt(data, 44, isLittleEndian);
    _firstRecordedGate = grabShort(data, 48, isLittleEndian);
    _lastRecordedGate = grabShort(data, 50, isLittleEndian);
    
    if (! headerOnly) {
        // @TODO unpack the data portion of the FRAD
    }

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
        os << "test pulse level: " << _testPulsePower << std::endl;
        os << "test pulse start: " << _testPulseStart << std::endl;
        os << "test pulse width: " << _testPulseWidth << std::endl;
        os << "test pulse freq." << _testPulseFreq << std::endl;
        os << "test pulse attenuation: " << _testPulseAttenuation << 
            std::endl;
        os << "test pulse fnum: " << _testPulseFNum << std::endl;
        os << "noise power: " << _noisePower << std::endl;
        os << "ray count: " << _rayCount << std::endl;
        os << "first recorded gate: " << _firstRecordedGate << std::endl;
        os << "last recorded gate: " << _lastRecordedGate << std::endl;
    }
    return os;
}
