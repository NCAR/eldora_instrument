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
    DoradeASIB();
    
    DoradeASIB(const unsigned char *data, unsigned int datalen, 
               bool isLittleEndian) throw (DescriptorException);

    std::ostream& printTo(std::ostream& out) const;
    /**
     * Get longitude (deg).
     * @return longitude (deg).
     */
    float getLongitude() const { return _longitude; }
    /**
     * Get latitude (deg).
     * @return latitude (deg).
     */
    float getLatitude() const { return _latitude; }
    /**
     * Get MSL altitude (km).
     * @return MSL altitude (km).
     */
    float getAltitudeMSL() const { return _altitudeMSL; }
    /**
     * Get AGL altitude (km).
     * @return AGL altitude (km).
     */
    float getAltitudeAGL() const { return _altitudeAGL; }
    /**
     * Get E-W ground speed (m/s).
     * @return E-W ground speed (m/s).
     */
    float getGroundSpeedEW() const { return _groundSpeedEW; }
    /**
     * Get N-S ground speed (m/s).
     * @return N-S ground speed (m/s).
     */
    float getGroundSpeedNS() const { return _groundSpeedNS; }
    /**
     * Get aircraft vertical velocity (m/s).
     * @return aircraft vertical velocity (m/s).
     */
    float getVerticalVelocity() const { return _verticalVelocity; }
    /**
     * Get aircraft heading (deg).
     * @return aircraft heading (deg).
     */
    float getHeading() const { return _heading; }
    /**
     * Get aircraft roll (deg).
     * @return aircraft roll (deg).
     */
    float getRoll() const { return _roll; }
    /**
     * Get aircraft pitch (deg).
     * @return aircraft pitch (deg).
     */
    float getPitch() const { return _pitch; }
    /**
     * Get aircraft yaw (deg).
     * @return aircraft yaw (deg).
     */
    float getYaw() const { return _yaw; }
    /**
     * Get antenna scan angle (deg).
     * @return antenna scan angle (deg).
     */
    float getAntennaScanAngle() const { return _antennaScanAngle; }
    /**
     * Get antenna tilt angle (deg).
     * @return antenna tilt angle (deg).
     */
    float getAntennaTiltAngle() const { return _antennaTiltAngle; }
    /**
     * Get U wind component (m/s).
     * @return U wind component (m/s).
     */
    float getUWind() const { return _uWind; }
    /**
     * Get V wind component (m/s).
     * @return V wind component (m/s).
     */
    float getVWind() const { return _vWind; }
    /**
     * Get W wind component (m/s).
     * @return W wind component (m/s).
     */
    float getWWind() const { return _wWind; }
    /**
     * Get heading rate of change (deg/s).
     * @return heading rate of change (deg/s).
     */
    float getHeadingChangeRate() const { return _headingChangeRate; }
    /**
     * Get pitch rate of change (deg/s).
     * @return pitch rate of change (deg/s).
     */
    float getPitchChangeRate() const { return _pitchChangeRate; }
    /**
     * Set longitude (deg).
     * @param longitude longitude (deg).
     */
    void setLongitude(float longitude) { _longitude = longitude; }
    /**
     * Set latitude (deg).
     * @param latitude latitude (deg).
     */
    void setLatitude(float latitude) { _latitude = latitude; }
    /**
     * Set MSL altitude (km).
     * @param altitude MSL altitude (km).
     */
    void setAltitudeMSL(float altitude) { _altitudeMSL = altitude; }
    /**
     * Set AGL altitude (km).
     * @param altitude AGL altitude (km).
     */
    void setAltitudeAGL(float altitude) { _altitudeAGL = altitude; }
    /**
     * Set E-W ground speed (m/s).
     * @param speed E-W ground speed (m/s).
     */
    void setGroundSpeedEW(float speed) { _groundSpeedEW = speed; }
    /**
     * Set N-S ground speed (m/s).
     * @param speed N-S ground speed (m/s).
     */
    void setGroundSpeedNS(float speed ) { _groundSpeedNS = speed; }
    /**
     * Set aircraft vertical velocity (m/s).
     * @param speed aircraft vertical velocity (m/s).
     */
    void setVerticalVelocity(float speed) { _verticalVelocity = speed; }
    /**
     * Set aircraft heading (deg).
     * @param heading aircraft heading (deg).
     */
    void setHeading(float heading) { _heading = heading; }
    /**
     * Set aircraft roll (deg).
     * @param roll aircraft roll (deg).
     */
    void setRoll(float roll) { _roll = roll; }
    /**
     * Set aircraft pitch (deg).
     * @param pitch aircraft pitch (deg).
     */
    void setPitch(float pitch) { _pitch = pitch; }
    /**
     * Set aircraft yaw (deg).
     * @param yaw aircraft yaw (deg).
     */
    void setYaw(float yaw) { _yaw = yaw; }
    /**
     * Set antenna scan angle (deg).
     * @param angle antenna scan angle (deg).
     */
    void setAntennaScanAngle(float angle) { _antennaScanAngle = angle; }
    /**
     * Set antenna tilt angle (deg).
     * @param angle antenna tilt angle (deg).
     */
    void setAntennaTiltAngle(float angle) { _antennaTiltAngle = angle; }
    /**
     * Set U wind component (m/s).
     * @param speed U wind component (m/s).
     */
    void setUWind(float speed) { _uWind = speed; }
    /**
     * Set V wind component (m/s).
     * @param speed V wind component (m/s).
     */
    void setVWind(float speed) { _vWind = speed; }
    /**
     * Set W wind component (m/s).
     * @param speed W wind component (m/s).
     */
    void setWWind(float speed) { _wWind = speed; }
    /**
     * Set heading rate of change (deg/s).
     * @param rate heading rate of change (deg/s).
     */
    void setHeadingChangeRate(float rate) { _headingChangeRate = rate; }
    /**
     * Set pitch rate of change (deg/s).
     * @param rate pitch rate of change (deg/s).
     */
    void setPitchChangeRate(float rate) { _pitchChangeRate = rate; }
    
    std::ostream& streamTo(std::ostream& os, bool asLittleEndian);
private:
    float _longitude;
    float _latitude;
    float _altitudeMSL; // km
    float _altitudeAGL; // km
    float _groundSpeedEW; // m/s
    float _groundSpeedNS; // m/s
    float _verticalVelocity; // m/s
    float _heading; // deg clockwise from true north
    float _roll;
    float _pitch;
    float _yaw;
    float _antennaScanAngle;  // deg clockwise from up w.r.t. airframe
    float _antennaTiltAngle; // deg toward nose
    float _uWind; // m/s
    float _vWind; // m/s
    float _wWind; // m/s
    float _headingChangeRate; // deg/s
    float _pitchChangeRate; // deg/s
};
#endif /*DORADEASIB_H_*/
