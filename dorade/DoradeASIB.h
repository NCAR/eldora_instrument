#ifndef DORADEASIB_H_
#define DORADEASIB_H_

#include "DoradeDescriptor.h"

/**
 * <p>Title: DoradeASIB</p>
 * <p>Description: DORADE aircraft/ship information block descriptor</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Revision: 1.3 $ $Date: 2004/04/13 17:05:50 $
 */

class DoradeASIB : public DoradeDescriptor {
public:
    DoradeASIB(const unsigned char *data, unsigned int datalen, 
               bool isLittleEndian) throw (DescriptorException);
    std::ostream& printTo(std::ostream& out) const;
private:
    float _longitude;
    float _latitude;
    float _altitudeMSL; // km
    float _altitudeAGL; // km
    float _groundSpeedEW; // m/s
    float _groundSpeedNS; // m/s
    float _verticalVelocity; // m/s
    float _antennaHeading; // deg clockwise from true north
    float _rollAngle;
    float _pitchAngle;
    float _yawAngle;
    float _antennaScanAngle;  // deg clockwise from up w.r.t. airframe
    float _antennaFixedAngle; // deg toward nose
    float _uWind; // m/s
    float _vWind; // m/s
    float _wWind; // m/s
    float _headingChangeRate; // deg/s
    float _pitchChangeRate; // deg/s
};
#endif /*DORADEASIB_H_*/
