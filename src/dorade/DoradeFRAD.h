#ifndef DORADEFRAD_H_
#define DORADEFRAD_H_

#include <string>
#include <cstring>
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
     * @param assumeTwoByteData true iff it can be assumed that parameter
     *    data included in the FRAD (if any) are strictly two-byte values.
     */
    DoradeFRAD(const unsigned char *data, unsigned int datalen, 
               bool isLittleEndian, bool headerOnly = false,
               bool assumeTwoByteData = true) 
        throw (DescriptorException);
    
    DoradeFRAD(int dataSystemStatus, std::string radarName, 
        float testPulsePower, float testPulseStart, float testPulseWidth, 
        float testPulseFreq, short testPulseAttenuation, short testPulseFNum, 
        float noisePower, int rayCount, 
        short firstRecordedGate, short lastRecordedGate);
    
    ~DoradeFRAD();
    
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
    /**
     * Get the array of strictly two-byte data from the FRAD.  The order of 
     * the data, assuming np parameters and ng gates is:
     * 
     *      parm<0> at gate<0>
     *      parm<1> at gate<0>
     *      ...
     *      parm<np-1> at gate<0>
     *      parm<0> at gate<1>
     *      parm<1> at gate<1>
     *      ...
     *      parm<np-1> at gate<ng-1>
     * @return the array of short data from the FRAD.
     */
    const short* getShortData() const { return _shortData; }
    int getSizeShortData() const { return _nShortData; }
    
    
    /**
     * Add an array of strictly two-byte data to the FRAD.  The order of 
     * the data, assuming np parameters and ng gates should be:
     * 
     *      parm<0> at gate<0>
     *      parm<1> at gate<0>
     *      ...
     *      parm<np-1> at gate<0>
     *      parm<0> at gate<1>
     *      parm<1> at gate<1>
     *      ...
     *      parm<np-1> at gate<ng-1>
     * @param data  array of shorts containing the data
     * @param nData the length of the data array
     */
    void putTwoByteData(short* data, unsigned int nData) {
        if (_shortData)
            delete[] _shortData;
        _nShortData = nData;
        _shortData = new short[_nShortData];
        _descLen = 52 + 2 * _nShortData;
        memcpy(_shortData, data, _nShortData * sizeof(short));
    }
    
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
    // We keep the data portion of the FRAD as either an array of 
    // shorts (in local-machine order), or as a completely opaque array
    // of bytes.
    short* _shortData;
    int _nShortData;
    // Keep an opaque copy of data if it's given to us.
    unsigned char* _opaqueData;
};
#endif /*DORADEFRAD_H_*/
