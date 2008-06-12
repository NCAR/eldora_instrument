#ifndef DORADEFRAD_H_
#define DORADEFRAD_H_

#include <string>
#include "DoradeDescriptor.h"

/**
 * <p>Title: DoradeFRAD</p>
 * <p>Description: DORADE ray information block descriptor</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Revision: 1.3 $ $Date: 2004/04/13 17:05:50 $
 */

class DoradeFRAD : public DoradeDescriptor {
public:
    /**
     * Construct a DoradeFRAD from raw data.
     * @param data the raw data.
     * @param datalen length of the raw data.
     * @param isLittleEndian true iff the raw data are little-endian.
     * @param headerOnly true iff just the 52-byte header portion of the 
     *    FRAD descriptor should be unpacked from the raw data (i.e., 
     *    everything but the "data" portion of the descriptor).  The ELDORA 
     *    housekeeper sends such "dataless" FRAD-s.  This parameter defaults 
     *    to false.
     */
    DoradeFRAD();
    
    DoradeFRAD(const unsigned char *data, unsigned int datalen, 
               bool isLittleEndian, bool headerOnly = false) 
        throw (DescriptorException);
    
    // inline accessors
    int getDataSystemStatus() const { return _dataSystemStatus; }
    std::string getRadarName() const { return _radarName; }
    float getTestPulsePower() const { return _testPulsePower; }
    float getTestPulseStart() const { return _testPulseStart; }
    float getTestPulseWidth() const { return _testPulseWidth; }
    float getTestPulseFreq() const { return _testPulseFreq; }
    short getTestPulseAttenuation() const { return _testPulseAttenuation; }
    short getTestPulseFNum() const { return _testPulseFNum; }
    float getNoisePower() const { return _noisePower; }
    int getRayCount() const { return _rayCount; }
    short getFirstGate() const { return _firstRecordedGate; }
    short getLastGate() const { return _lastRecordedGate; }
    
    void setDataSystemStatus(int status) { _dataSystemStatus = status; }
    void setRadarName(std::string radarName) { 
        _radarName = radarName;
        // silently truncate at 8 characters if necessary
        if (_radarName.size() > 8)
            _radarName.resize(8);
    }
    void setTestPulsePower(float power) { _testPulsePower = power; } // dBm?
    void setTestPulseStart(float start) { _testPulseStart = start; }
    void setTestPulseWidth(float width) { _testPulseWidth = width; }
    void setTestPulseFreq(float freq) { _testPulseFreq = freq; } // GHz?
    void setTestPulseAttenuation(short atten) { _testPulseAttenuation = atten; }
    void setTestPulseFNum(short fnum) { _testPulseFNum = fnum; }
    void setNoisePower(float power) { _noisePower = power; } // dBm?
    void setRayCount(int count) { _rayCount = count; }
    void setFirstGate(short gate) { _firstRecordedGate = gate; }
    void setLastGate(short gate) { _lastRecordedGate = gate; }
    
    std::ostream& streamTo(std::ostream& os, bool asLittleEndian);
protected:
    std::ostream& printTo(std::ostream& out) const;
private:
    int _dataSystemStatus;
    std::string _radarName;
    float _testPulsePower;  /* dBm? */
    float _testPulseStart;   /* m? */
    float _testPulseWidth;  /* m? */
    float _testPulseFreq;   /* Hz? */
    short _testPulseAttenuation;    /* dB? */
    short _testPulseFNum;           /* ? */
    float _noisePower;      /* dBm? */
    int _rayCount;
    short _firstRecordedGate;
    short _lastRecordedGate;
    // short[][]: _dataArray; skipped for the time being
};
#endif /*DORADEFRAD_H_*/
