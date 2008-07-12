#include <sstream>
#include <iomanip>
#include <cassert>
#include "DoradeFRIB.h"

/**
 * <p>Title: DoradeFRIB</p>
 * <p>Description: DORADE Field Radar Information Block (FRIB)</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Id$
 */
DoradeFRIB::DoradeFRIB(const unsigned char *data, unsigned int datalen, 
                       bool isLittleEndian) throw (DescriptorException) :
    DoradeDescriptor(data, datalen, isLittleEndian, "FRIB", 264) { 
    //
    // unpack
    //
    _dataSysId = grabInt(data, 8, isLittleEndian);
    _waveguideLossOut = grabFloat(data, 12, isLittleEndian);
    _waveguideLossIn = grabFloat(data, 16, isLittleEndian);
    _rjointLoss = grabFloat(data, 20, isLittleEndian);
    _antennaVDim = grabFloat(data, 24, isLittleEndian);
    _antennaHDim = grabFloat(data, 28, isLittleEndian);
    _antennaNoiseTemp = grabFloat(data, 32, isLittleEndian);
    _rcvrNoiseFigure = grabFloat(data, 36, isLittleEndian);
    for (int chan = 0; chan < 5; chan++)
        _xmitPower[chan] = grabFloat(data, 40 + 4 * chan, isLittleEndian);
    _xBandGain = grabFloat(data, 60, isLittleEndian);
    for (int chan = 0; chan < 5; chan++)
        _rcvrGain[chan] = grabFloat(data, 64 + 4 * chan, isLittleEndian);
    for (int chan = 0; chan < 5; chan++)
        _ifGain[chan] = grabFloat(data, 84 + 4 * chan, isLittleEndian);
    _conversionGain = grabFloat(data, 104, isLittleEndian);
    for (int chan = 0; chan < 5; chan++)
        _scaleFactor[chan] = grabFloat(data, 108 + 4 * chan, isLittleEndian);
    _processorConst = grabFloat(data, 128, isLittleEndian);
    _tubeToAntennaDelay = grabInt(data, 132, isLittleEndian);
    _chipToADDelay = grabInt(data, 136, isLittleEndian);
    _testPulseADDelay = grabInt(data, 140, isLittleEndian);
    _modulatorRiseTime = grabInt(data, 144, isLittleEndian);
    _modulatorFallTime = grabInt(data, 148, isLittleEndian);
    _peakPwrOffset = grabFloat(data, 152, isLittleEndian);
    _testPulsePwrOffset = grabFloat(data, 156, isLittleEndian);
    _antennaEAngle = grabFloat(data, 160, isLittleEndian);
    _antennaHAngle = grabFloat(data, 164, isLittleEndian);
    _encoderAntennaUp = grabFloat(data, 168, isLittleEndian);
    _pitchAntennaUp = grabFloat(data, 172, isLittleEndian);
    _extraDataFlag = grabShort(data, 176, isLittleEndian);
    _indepFreqGate = grabShort(data, 178, isLittleEndian);
    _tsGate = grabShort(data, 180, isLittleEndian);
    _numBaseParams = grabShort(data, 182, isLittleEndian);
    // Get exactly 80 chars, then shorten if there's a null somewhere.
    _hdrFileName = std::string((const char*)data + 184, 80);
    _hdrFileName = std::string(_hdrFileName.c_str());
    //
    // debugging output
    //
    if (_verbose)
        std::cout << *this;
};

DoradeFRIB::DoradeFRIB(int dataSysId, float waveguideLossOut, 
    float waveguideLossIn, float rjointLoss, float antennaVDim, 
    float antennaHDim, float antennaNoiseTemp, float rcvrNoiseFigure, 
    float xmitPower[5], float xBandGain, float rcvrGain[5], float ifGain[5],
    float conversionGain, float scaleFactor[5], float processorConst,
    int tubeToAntennaDelay, int chipToADDelay, int testPulseADDelay,
    int modulatorRiseTime, int modulatorFallTime, float peakPwrOffset,
    float testPulsePwrOffset, float antennaEAngle, float antennaHAngle,
    float encoderAntennaUp, float pitchAntennaUp, short extraDataFlag,
    short indepFreqGate, short tsGate, short numBaseParams,
    std::string hdrFileName) : DoradeDescriptor("FRIB", 264), 
    _dataSysId(dataSysId), _waveguideLossOut(waveguideLossOut), 
    _waveguideLossIn(waveguideLossIn), _rjointLoss(rjointLoss),
    _antennaVDim(antennaVDim), _antennaHDim(antennaHDim),
    _antennaNoiseTemp(antennaNoiseTemp), _rcvrNoiseFigure(rcvrNoiseFigure),
    _xBandGain(xBandGain), _conversionGain(conversionGain),
    _processorConst(processorConst), _tubeToAntennaDelay(tubeToAntennaDelay),
    _chipToADDelay(chipToADDelay), _testPulseADDelay(testPulseADDelay), 
    _modulatorRiseTime(modulatorRiseTime), _modulatorFallTime(modulatorFallTime),
    _peakPwrOffset(peakPwrOffset), _testPulsePwrOffset(testPulsePwrOffset),
    _antennaEAngle(antennaEAngle), _antennaHAngle(antennaHAngle), 
    _encoderAntennaUp(encoderAntennaUp), _pitchAntennaUp(pitchAntennaUp),
    _extraDataFlag(extraDataFlag), _indepFreqGate(indepFreqGate), 
    _tsGate(tsGate), _numBaseParams(numBaseParams), _hdrFileName(hdrFileName) {
    // truncate _hdrFileName at 80 characters if necessary
    if (_hdrFileName.size() > 80)
        _hdrFileName.resize(80);
    assert(sizeof(xmitPower) == sizeof(_xmitPower));
    memcpy(_xmitPower, xmitPower, sizeof(_xmitPower));
    assert(sizeof(rcvrGain) == sizeof(_rcvrGain));
    memcpy(_rcvrGain, rcvrGain, sizeof(_rcvrGain));
    assert(sizeof(ifGain) == sizeof(_ifGain));
    memcpy(_ifGain, ifGain, sizeof(_ifGain));
    assert(sizeof(scaleFactor) == sizeof(_scaleFactor));
    memcpy(_scaleFactor, scaleFactor, sizeof(_scaleFactor));
    
    if (_verbose)
        std::cout << *this;
}

std::ostream& 
DoradeFRIB::printTo(std::ostream& os) const
{
    DoradeDescriptor::printTo(os);
    // @TODO add details here
    if (_verbose) {
        os << "no FRIB detail printing yet..." << std::endl;
    }
    return os;
}

std::ostream&
DoradeFRIB::streamTo(std::ostream& os, bool asLittleEndian)
{
    putBytes(os, _descName.data(), 4, false);   // no swapping for char data
    putInt(os, _descLen, asLittleEndian);
    putInt(os, _dataSysId, asLittleEndian);
    putFloat(os, _waveguideLossOut, asLittleEndian);
    putFloat(os, _waveguideLossIn, asLittleEndian);
    putFloat(os, _rjointLoss, asLittleEndian);
    putFloat(os, _antennaVDim, asLittleEndian);
    putFloat(os, _antennaHDim, asLittleEndian);
    putFloat(os, _antennaNoiseTemp, asLittleEndian);
    putFloat(os, _rcvrNoiseFigure, asLittleEndian);
    for (int chan = 0; chan < 5; chan++)
        putFloat(os, _xmitPower[chan], asLittleEndian);
    putFloat(os, _xBandGain, asLittleEndian);
    for (int chan = 0; chan < 5; chan++)
        putFloat(os, _rcvrGain[chan], asLittleEndian);
    for (int chan = 0; chan < 5; chan++)
        putFloat(os, _ifGain[chan], asLittleEndian);
    putFloat(os, _conversionGain, asLittleEndian);
    for (int chan = 0; chan < 5; chan++)
        putFloat(os, _scaleFactor[chan], asLittleEndian);
    putFloat(os, _processorConst, asLittleEndian);
    putInt(os, _tubeToAntennaDelay, asLittleEndian);
    putInt(os, _chipToADDelay, asLittleEndian);
    putInt(os, _testPulseADDelay, asLittleEndian);
    putInt(os, _modulatorRiseTime, asLittleEndian);
    putInt(os, _modulatorFallTime, asLittleEndian);
    putFloat(os, _peakPwrOffset, asLittleEndian);
    putFloat(os, _testPulsePwrOffset, asLittleEndian);
    putFloat(os, _antennaEAngle, asLittleEndian);
    putFloat(os, _antennaHAngle, asLittleEndian);
    putFloat(os, _encoderAntennaUp, asLittleEndian);
    putFloat(os, _pitchAntennaUp, asLittleEndian);
    putShort(os, _extraDataFlag, asLittleEndian);
    putShort(os, _indepFreqGate, asLittleEndian);
    putShort(os, _tsGate, asLittleEndian);
    putShort(os, _numBaseParams, asLittleEndian);
    std::string str(_hdrFileName);
    str.resize(80);    // force exactly 80 characters
    putBytes(os, str.data(), 80, false);   // no swapping for char data
    return os;
}
