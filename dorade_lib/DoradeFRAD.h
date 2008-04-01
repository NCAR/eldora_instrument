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
    DoradeFRAD(const unsigned char *data, unsigned int datalen, 
               bool isLittleEndian) throw (DescriptorException);
    // inline accessors
    int getDataSystemStatus() const { return _dataSystemStatus; }
    std::string getRadarName() const { return _radarName; }
    float getTestPulseLevel() const { return _testPulseLevel; }
    float getTestPulseDistance() const { return _testPulseDist; }
    float getTestPulseWidth() const { return _testPulseWidth; }
    float getTestPulseFreq() const { return _testPulseFreq; }
    short getTestPulseAttenuation() const { return _testPulseAttenuation; }
    short getTestPulseFnum() const { return _testPulseFnum; }
    float getNoisePower() const { return _noisePower; }
    int getRayCount() const { return _rayCount; }
    short getFirstGate() const { return _firstRecordedGate; }
    short getLastGate() const { return _lastRecordedGate; }
protected:
    std::ostream& printTo(std::ostream& out) const;
private:
    int _dataSystemStatus;
    std::string _radarName;
    float _testPulseLevel;  /* dBm? */
    float _testPulseDist;   /* m? */
    float _testPulseWidth;  /* m? */
    float _testPulseFreq;   /* Hz? */
    short _testPulseAttenuation;    /* dB? */
    short _testPulseFnum;           /* ? */
    float _noisePower;      /* dBm? */
    int _rayCount;
    short _firstRecordedGate;
    short _lastRecordedGate;
    // short[][]: _dataArray; skipped for the time being
};
#endif /*DORADEFRAD_H_*/
