#include <sstream>
#include <iomanip>
#include "DoradePARM.h"

/**
 * <p>Title: DoradePARM</p>
 * <p>Description: DORADE DORADE PARaMeter Descriptor (PARM)</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Id$
 */
DoradePARM::DoradePARM(const unsigned char *data, unsigned int datalen, 
                       bool isLittleEndian) throw (DescriptorException) :
    DoradeDescriptor(data, datalen, isLittleEndian, "PARM", 104) {
    //
    // unpack
    //
                           
    // Get exactly 8 chars, then shorten if there's a null somewhere.
    _name = std::string((const char*)data + 8, 8);
    _name = std::string(_name.c_str());
    // Get exactly 40 chars, then shorten if there's a null somewhere.
    _description = std::string((const char*)data + 16, 40);
    _description = std::string(_description.c_str());
    // Get exactly 8 chars, then shorten if there's a null somewhere.
    _units = std::string((const char*)data + 56, 8);
    _units = std::string(_units.c_str());
    _usedIPPs = grabShort(data, 64, isLittleEndian);
    _usedFreqs = grabShort(data, 66, isLittleEndian);
    _rcvrBandwidth = grabFloat(data, 68, isLittleEndian);
    _pulseWidth = grabShort(data, 72, isLittleEndian);
    _polarization = grabShort(data, 74, isLittleEndian);
    _nSamples = grabShort(data, 76, isLittleEndian);
    _paramType = grabShort(data, 78, isLittleEndian);
    // Get exactly 8 chars, then shorten if there's a null somewhere.
    _thresholdParm = std::string((const char*)data + 80, 8);
    _thresholdParm = std::string(_thresholdParm.c_str());
    _thresholdValue = grabFloat(data, 88, isLittleEndian);
    _scale = grabFloat(data, 92, isLittleEndian);
    _offset = grabFloat(data, 96, isLittleEndian);
    _missingValue = grabInt(data, 100, isLittleEndian);

    //
    // debugging output
    //
    if (_verbose)
        std::cout << *this;
};

DoradePARM::DoradePARM(std::string name, std::string description, 
    std::string units, short usedIPPs, short usedFreqs, float rcvrBandwidth, 
    short pulseWidth, short polarization, short nSamples, short paramType,
    std::string thresholdParm, float thresholdValue, float scale,
    float offset, int missingValue) : 
    DoradeDescriptor("PARM", 104), _name(name), _description(description), 
    _units(units), _usedIPPs(usedIPPs), _usedFreqs(usedFreqs), 
    _rcvrBandwidth(rcvrBandwidth), _pulseWidth(pulseWidth), 
    _polarization(polarization), _nSamples(nSamples), _paramType(paramType),
    _thresholdParm(thresholdParm), _thresholdValue(thresholdValue), 
    _scale(scale), _offset(offset), _missingValue(missingValue) {
    // Truncate the strings to the appropriate lengths if necessary
    if (_name.size() > 8)
        _name.resize(8);
    if (_description.size() > 40)
        _description.resize(40);
    if (_units.size() > 8)
        _units.resize(8);
    if (_thresholdParm.size() > 8)
        _thresholdParm.resize(8);

    if (_verbose)
        std::cout << *this;
}

std::ostream& 
DoradePARM::printTo(std::ostream& os) const
{
    DoradeDescriptor::printTo(os);
    if (_verbose) {
        os << "  name: " << _name << std::endl;
        os << "  description: " << _description << std::endl;
        os << "  units: " << _units << std::endl;
        os << std::setbase(16);
        os << "  IPPs used: 0x" << _usedIPPs << std::endl;
        os << "  frequencies used: 0x" << _usedFreqs << std::endl;
        os << std::setbase(10);
        os << "  receiver bandwidth: " << _rcvrBandwidth << " MHz" << std::endl;
        os << "  pulse width: " << _pulseWidth << " m" << std::endl;
        os << "  polarization type: " << _polarization << std::endl;
        os << "  number of samples per dwell: " << _nSamples << std::endl;
        os << "  parameter type: " << _paramType << std::endl;
        os << "  threshold parameter: " << _thresholdParm << std::endl;
        os << "  threshold value: " << _thresholdValue << std::endl;
        os << "  scale: " << _scale << std::endl;
        os << "  offset: " << _offset << std::endl;
        os << "  missing value: " << _missingValue << std::endl;
    }
    return os;
}

std::ostream&
DoradePARM::streamTo(std::ostream& os, bool asLittleEndian)
{
    std::string str;
    
    putBytes(os, _descName.data(), 4, false);   // no swapping for char data
    putInt(os, _descLen, asLittleEndian);
    
    str = _name;
    str.resize(8);  // force exactly 8 chars
    putBytes(os, str.data(), 8, false);  // no swapping for char data
    
    str = _description;
    str.resize(40); // force exactly 40 chars
    putBytes(os, str.data(), 40, false);  // no swapping for char data
    
    str = _units;
    str.resize(8);  // force exactly 8 chars
    putBytes(os, str.data(), 8, false); // no swapping for char data
    
    putShort(os, _usedIPPs, asLittleEndian);
    putShort(os, _usedFreqs, asLittleEndian);
    putFloat(os, _rcvrBandwidth, asLittleEndian);
    putShort(os, _pulseWidth, asLittleEndian);
    putShort(os, _polarization, asLittleEndian);
    putShort(os, _nSamples, asLittleEndian);
    putShort(os, _paramType, asLittleEndian);
    
    str = _thresholdParm;
    str.resize(8);  // force exactly 8 chars
    putBytes(os, str.data(), 8, false); // no swapping for char data
    
    putFloat(os, _thresholdValue, asLittleEndian);
    putFloat(os, _scale, asLittleEndian);
    putFloat(os, _offset, asLittleEndian);
    putInt(os, _missingValue, asLittleEndian);
    return os;
}
