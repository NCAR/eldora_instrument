#ifndef DORADERYIB_H_
#define DORADERYIB_H_

#include <boost/date_time/posix_time/posix_time.hpp>
#include "DoradeDescriptor.h"

/**
 * <p>Title: DoradeRYIB</p>
 * <p>Description: DORADE ray information block descriptor</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Revision: 1.3 $ $Date: 2004/04/13 17:05:50 $
 */

class DoradeRYIB : public DoradeDescriptor {
public:
    DoradeRYIB();
    
    DoradeRYIB(const unsigned char *data, unsigned int datalen, 
               bool isLittleEndian) throw (DescriptorException);
    
    std::ostream& printTo(std::ostream& stream) const;
    /**
     * Get ray date/time.
     * @return posix_time holding the ray's date/time.
     */
    boost::posix_time::ptime getRayDateTime() const { return _rayDateTime; }
    /**
     * Get the sweep number.
     * @return the ray's sweep number.
     */
    int getSweepNumber() const { return _sweepNumber; }
    /**
     * Get day of year: 1-366.
     * @return the ray's day of year.
     */
    short getDayOfYear() const { return _julianDay; }
    /**
     * Get hour: 0-23.
     * @return the ray's hour.
     */
    short getHour() const { return _hour; }
    /**
     * Get minute: 0-59.
     * @return the ray's minute.
     */
    short getMinute() const { return _minute; }
    /**
     * Get second: 0-59.
     * @return the ray's second.
     */
    short getSecond() const { return _second; }
    /**
     * Get millisecond: 0-999.
     * @return the ray's millisecond.
     */
    short getMillisecond() const { return _millisecond; }
    /**
     * Get azimuth (deg).
     * @return the ray's azimuth.
     */
    float getAzimuth() const { return _azimuth; }
    /**
     * Get elevation (deg).
     * @return the ray's elevation.
     */
    float getElevation() const { return _elevation; }
    /**
     * Get peak transmit power (kW).
     * @return the peak transmit power (kW).
     */
    float getPeakXmitPower() const { return _peakXmitPower; }
    /**
     * Get true scan rate (deg/s).
     * @return the true radar scan rate (deg/s).
     */
    float getTrueScanRate() const { return _scanRate; }
    /**
     * Get ray status.
     *  0 = normal
     *  1 = transition
     *  2 = bad
     *  3 = questionable
     * @return the ray's status.
     */
    int getRayStatus() const { return _rayStatus; }
    /**
     * Set ray date/time.
     * @param time posix_time holding the ray's date/time.
     */
    void setRayDateTime(boost::posix_time::ptime time);
    /**
     * Set the sweep number.
     * @param sweepNumber the ray's sweep number.
     */
    void setSweepNumber(int sweepNumber) { _sweepNumber = sweepNumber; }
    /**
     * Set azimuth (deg).
     * @param azimuth the ray's azimuth.
     */
    void setAzimuth(float azimuth) { _azimuth = azimuth; }
    /**
     * Set elevation (deg).
     * @param elevation the ray's elevation.
     */
    void setElevation(float elevation) { _elevation = elevation; }
    /**
     * Set peak transmit power (kW).
     * @param power the peak transmit power (kW).
     */
    void setPeakXmitPower(float power) { _peakXmitPower = power; }
    /**
     * Set true scan rate (deg/s).
     * @param rate the true radar scan rate (deg/s).
     */
    void setTrueScanRate(float rate) { _scanRate = rate; }
    /**
     * Set ray status.
     *  0 = normal
     *  1 = transition
     *  2 = bad
     *  3 = questionable
     * @param status the ray's status.
     */
    void setRayStatus(int status) { _rayStatus = status; }
    
    std::ostream& streamTo(std::ostream& os, bool asLittleEndian);
private:
    int _sweepNumber;
    int _julianDay;
    short _hour;
    short _minute;
    short _second;
    short _millisecond;
    float _azimuth;
    float _elevation;
    float _peakXmitPower;   // kW
    float _scanRate;        // deg/s
    int _rayStatus;         // 0 normal, 1 transition, 2 bad
    
    boost::posix_time::ptime _rayDateTime; /* not directly from the RYIB, but built from it */
};
#endif /*DORADERYIB_H_*/
