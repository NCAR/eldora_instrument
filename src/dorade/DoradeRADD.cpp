#include <sstream>
#include <iomanip>
#include "DoradeRADD.h"

/**
 * <p>Title: DoradeRADD</p>
 * <p>Description: DORADE RADar Descriptor (RADD)</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Revision: 1.3 $ $Date: 2004/04/13 17:05:50 $
 */
DoradeRADD::DoradeRADD(const unsigned char *data, unsigned int datalen, 
                       bool isLittleEndian) throw (DescriptorException) :
    DoradeDescriptor(data, datalen, isLittleEndian, "RADD", 144) { 
    //
    // unpack
    //
                           
    // force _radarName to 8 characters by truncating or appending nulls
    _radarName = std::string((const char*)data + 8, 8);
    _radarName.resize(8);
    _radarConstant = grabFloat(data, 16, isLittleEndian);
    _peakPower = grabFloat(data, 20, isLittleEndian);
    _noisePower = grabFloat(data, 24, isLittleEndian);
    _rcvrGain = grabFloat(data, 28, isLittleEndian);
    _antennaGain = grabFloat(data, 32, isLittleEndian);
    _radarSysGain = grabFloat(data, 36, isLittleEndian);
    _hBeamWidth = grabFloat(data, 40, isLittleEndian);
    _vBeamWidth = grabFloat(data, 44, isLittleEndian);
    _radarType = grabShort(data, 48, isLittleEndian);
    _scanMode = grabShort(data, 50, isLittleEndian);
    _scanRate = grabFloat(data, 52, isLittleEndian);
    _startAngle = grabFloat(data, 56, isLittleEndian);
    _stopAngle = grabFloat(data, 60, isLittleEndian);
    _nParams = grabShort(data, 64, isLittleEndian);
    _nDescriptors = grabShort(data, 66, isLittleEndian);
    _dataCompressionCode = grabShort(data, 68, isLittleEndian);
    _dataReductionAlgorithm = grabShort(data, 70, isLittleEndian);
    _reductionParam1 = grabFloat(data, 72, isLittleEndian);
    _reductionParam2 = grabFloat(data, 76, isLittleEndian);
    _radarLongitude = grabFloat(data, 80, isLittleEndian);
    _radarLatitude = grabFloat(data, 84, isLittleEndian);
    _radarAltitude = grabFloat(data, 88, isLittleEndian); 
    _unambiguousVelocity = grabFloat(data, 92, isLittleEndian);
    _unambiguousRange = grabFloat(data, 96, isLittleEndian);
    _nFrequencies = grabShort(data, 100, isLittleEndian);
    _nIPPs = grabShort(data, 102, isLittleEndian);
    for (int f = 0; f < 5; f++)
        _frequencies[f] = grabFloat(data, 104 + 4 * f, isLittleEndian);
    for (int i = 0; i < 5; i++)
        _IPPs[i] = grabFloat(data, 124 + 4 * i, isLittleEndian);
    //
    // debugging output
    //
    if (_verbose)
        std::cout << *this;
};

DoradeRADD::DoradeRADD(std::string radarName, float radarConstant, 
    float peakPower, float noisePower, float rcvrGain, float antennaGain, 
    float radarSysGain, float hBeamWidth, float vBeamWidth,
    int radarType, int scanMode, float scanRate, float startAngle,
    float stopAngle, short nParams, short nDescriptors,
    short dataCompressionCode, short dataReductionAlgorithm,
    float reductionParam1, float reductionParam2, float radarLongitude,
    float radarLatitude, float radarAltitude, float unambiguousVelocity,
    float unambiguousRange, short nFrequencies, short nIPPs,
    float* frequencies, float* IPPs) : DoradeDescriptor("RADD", 144),
    _radarName(radarName), _radarConstant(radarConstant), _peakPower(peakPower),
    _noisePower(noisePower), _rcvrGain(rcvrGain), _antennaGain(antennaGain),
    _radarSysGain(radarSysGain), _hBeamWidth(hBeamWidth),
    _vBeamWidth(vBeamWidth), _radarType(radarType), _scanMode(scanMode),
    _scanRate(scanRate), _startAngle(startAngle), _stopAngle(stopAngle),
    _nParams(nParams), _nDescriptors(nDescriptors), 
    _dataCompressionCode(dataCompressionCode),
    _dataReductionAlgorithm(dataReductionAlgorithm),
    _reductionParam1(reductionParam1), _reductionParam2(reductionParam2),
    _radarLongitude(radarLongitude), _radarLatitude(radarLatitude), 
    _radarAltitude(radarAltitude), _unambiguousVelocity(unambiguousVelocity),
    _unambiguousRange(unambiguousRange), _nFrequencies(nFrequencies),
    _nIPPs(nIPPs) {
    // force _radarName to 8 characters by truncating or appending nulls
    _radarName.resize(8);
    for (int f = 0; f < _nFrequencies; f++)
        _frequencies[f] = frequencies[f];
    for (int i = 0; i < _nIPPs; i++)
        _IPPs[i] = IPPs[i];
    
    if (_verbose)
        std::cout << *this;
}

std::ostream& 
DoradeRADD::printTo(std::ostream& os) const
{
    DoradeDescriptor::printTo(os);
    // @TODO add details here
    if (_verbose) {
        os << "no RADD detail printing yet..." << std::endl;
    }
    return os;
}

std::ostream&
DoradeRADD::streamTo(std::ostream& os, bool asLittleEndian)
{
    putBytes(os, _descName.data(), 4, false);   // no swapping for char data
    putInt(os, _descLen, asLittleEndian);
    putBytes(os, _radarName.data(), 8, false);   // no swapping for char data
    putFloat(os, _radarConstant, asLittleEndian);
    putFloat(os, _peakPower, asLittleEndian);
    putFloat(os, _noisePower, asLittleEndian);
    putFloat(os, _rcvrGain, asLittleEndian);
    putFloat(os, _antennaGain, asLittleEndian);
    putFloat(os, _radarSysGain, asLittleEndian);
    putFloat(os, _hBeamWidth, asLittleEndian);
    putFloat(os, _vBeamWidth, asLittleEndian);
    putShort(os, _radarType, asLittleEndian);
    putShort(os, _scanMode, asLittleEndian);
    putFloat(os, _scanRate, asLittleEndian);
    putFloat(os, _startAngle, asLittleEndian);
    putFloat(os, _stopAngle, asLittleEndian);
    putShort(os, _nParams, asLittleEndian);
    putShort(os, _nDescriptors, asLittleEndian);
    putShort(os, _dataCompressionCode, asLittleEndian);
    putShort(os, _dataReductionAlgorithm, asLittleEndian);
    putFloat(os, _reductionParam1, asLittleEndian);
    putFloat(os, _reductionParam2, asLittleEndian);
    putFloat(os, _radarLongitude, asLittleEndian);
    putFloat(os, _radarLatitude, asLittleEndian);
    putFloat(os, _radarAltitude, asLittleEndian);
    putFloat(os, _unambiguousVelocity, asLittleEndian);
    putFloat(os, _unambiguousRange, asLittleEndian);
    putShort(os, _nFrequencies, asLittleEndian);
    putShort(os, _nIPPs, asLittleEndian);
    for (int f = 0; f < 5; f++)
        putFloat(os, _frequencies[f], asLittleEndian);
    for (int i = 0; i < 5; i++)
        putFloat(os, _IPPs[i], asLittleEndian);
    return os;
}
