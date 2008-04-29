#include <sstream>
#include <cstring>
#include "DoradeASIB.h"
/**
 * <p>Title: DoradeASIB</p>
 * <p>Description: DORADE aircraft/ship information block</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Revision: 1.3 $ $Date: 2004/04/13 17:05:50 $
 */
DoradeASIB::DoradeASIB(const unsigned char *data, unsigned int datalen, 
                       bool isLittleEndian) throw (DescriptorException) :
    DoradeDescriptor(data, datalen, isLittleEndian, "ASIB", 80) {
    //
    // unpack
    //
    _longitude = grabFloat(data, 8, isLittleEndian);
    _latitude = grabFloat(data, 12, isLittleEndian);
    _altitudeMSL = grabFloat(data, 16, isLittleEndian);
    _altitudeAGL = grabFloat(data, 20, isLittleEndian);
    _groundSpeedEW = grabFloat(data, 24, isLittleEndian);
    _groundSpeedNS = grabFloat(data, 28, isLittleEndian);
    _verticalVelocity = grabFloat(data, 32, isLittleEndian);
    _heading = grabFloat(data, 36, isLittleEndian);
    _roll = grabFloat(data, 40, isLittleEndian);
    _pitch = grabFloat(data, 44, isLittleEndian);
    _yaw = grabFloat(data, 48, isLittleEndian);
    _antennaScanAngle = grabFloat(data, 52, isLittleEndian);
    _antennaTiltAngle = grabFloat(data, 56, isLittleEndian);
    _uWind = grabFloat(data, 60, isLittleEndian);
    _vWind = grabFloat(data, 64, isLittleEndian);
    _wWind = grabFloat(data, 68, isLittleEndian);
    _headingChangeRate = grabFloat(data, 72, isLittleEndian);
    _pitchChangeRate = grabFloat(data, 76, isLittleEndian);

    //
    // debugging output
    //
    if (_verbose)
        std::cout << *this;
}

std::ostream& 
DoradeASIB::printTo(std::ostream& os) const
{
    DoradeDescriptor::printTo(os);
    if (_verbose) {
        os << "  longitude: " << _longitude << std::endl;
        os << "  latitude: " << _latitude << std::endl;
        os << "  altitude (MSL): " << _altitudeMSL << std::endl;
        os << "  altitude (AGL): " << _altitudeAGL << std::endl;
        os << "  EW ground speed: " << _groundSpeedEW << std::endl;
        os << "  NS ground speed: " << _groundSpeedNS << std::endl;
        os << "  vertical velocity: " << _verticalVelocity << std::endl;
        os << "  heading: " << _heading << std::endl;
        os << "  roll: " << _roll << std::endl;
        os << "  pitch: " << _pitch << std::endl;
        os << "  yaw: " << _yaw << std::endl;
        os << "  scan angle: " << _antennaScanAngle << std::endl;
        os << "  tilt angle: " << _antennaTiltAngle << std::endl;
        os << "  u wind: " << _uWind << std::endl;
        os << "  v wind: " << _vWind << std::endl;
        os << "  w wind: " << _wWind << std::endl;
        os << "  heading change rate: " << _headingChangeRate << std::endl;
        os << "  pitch change rate: " << _pitchChangeRate << std::endl;
    }
    return os;
}
