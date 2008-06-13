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
    DoradeFRAD(const unsigned char *data, unsigned int datalen, 
               bool isLittleEndian, bool headerOnly = false) 
        throw (DescriptorException);
    
    DoradeFRAD(int dataSystemStatus, std::string radarName, 
        float testPulsePower, float testPulseStart, float testPulseWidth, 
        float testPulseFreq, short testPulseAttenuation, short testPulseFNum, 
        float noisePower, int rayCount, 
        short firstRecordedGate, short lastRecordedGate);
    
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
    
    std::ostream& printTo(std::ostream& out) const;

    std::ostream& streamTo(std::ostream& os, bool asLittleEndian);
protected:
    void setRadarName(std::string radarName) { 
        _radarName = radarName;
        // silently truncate at 8 characters if necessary
        if (_radarName.size() > 8)
            _radarName.resize(8);
    }
    
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
