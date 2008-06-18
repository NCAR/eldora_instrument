#ifndef DORADERADD_H_
#define DORADERADD_H_

#include "DoradeDescriptor.h"

/**
 * <p>Title: DoradeRADD</p>
 * <p>Description: DORADE RADar Descriptor (RADD)</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Revision: 1.3 $ $Date: 2004/04/13 17:05:50 $
 */

class DoradeRADD : public DoradeDescriptor {
public:
    DoradeRADD(const unsigned char *data, unsigned int datalen, 
               bool isLittleEndian) throw (DescriptorException);
    
    DoradeRADD(std::string radarName, float radarConstant, float peakPower,
        float noisePower, float rcvrGain, float antennaGain, 
        float radarSysGain, float hBeamWidth, float vBeamWidth,
        int radarType, int scanMode, float scanRate, float startAngle,
        float stopAngle, short nParams, short nDescriptors,
        short dataCompressionCode, short dataReductionAlgorithm,
        float reductionParam1, float reductionParam2, float radarLongitude,
        float radarLatitude, float radarAltitude, float unambiguousVelocity,
        float unambiguousRange, short nFrequencies, short nIPPs,
        float* frequencies, float* IPPs);
    
    // accessor methods
    std::string getRadarName() { return _radarName; }
    float getRadarConstant() { return _radarConstant; }
    float getPeakPower() { return _peakPower; }
    float getNoisePower() { return _noisePower; }
    float getRcvrGain() { return _rcvrGain; }
    float getAntennaGain() { return _antennaGain; }
    float getRadarSysGain() { return _radarSysGain; }
    float getHBeamWidth() { return _hBeamWidth; }
    float getVBeamWidth() { return _vBeamWidth; }
    short getRadarType() { return _radarType; }
    short getScanMode() { return _scanMode; }
    float getScanRate() { return _scanRate; }
    float getStartAngle() { return _startAngle; }
    float getStopAngle() { return _stopAngle; }
    short getNParams() { return _nParams; }
    short getNDescriptors() { return _nDescriptors; }
    short getDataCompressionCode() { return _dataCompressionCode; }
    short getDataReductionAlgorithm() { return _dataReductionAlgorithm; }
    float getReductionParam1() { return _reductionParam1; }
    float getReductionParam2() { return _reductionParam2; }
    float getRadarLongitude() { return _radarLongitude; }
    float getRadarLatitude() { return _radarLatitude; }
    float getRadarAltitude() { return _radarAltitude; }
    float getUnambiguousVelocity() { return _unambiguousVelocity; }
    float getUnambiguousRange() { return _unambiguousRange; }
    short getNFrequencies() { return _nFrequencies; }
    short getNIPPs() { return _nIPPs; }
    float getFrequency(int freqnum) { return _frequencies[freqnum]; }
    float getIPP(int freqnum) { return _IPPs[freqnum]; }
    
    std::ostream& printTo(std::ostream& stream) const;
    std::ostream& streamTo(std::ostream& os, bool asLittleEndian);
private:
    std::string _radarName;     /// radar name, 8 characters
    float _radarConstant;       /// radar constant
    float _peakPower;           /// nominal peak power, kW
    float _noisePower;          /// nominal noise power, dBm
    float _rcvrGain;            /// receiver gain, dB
    float _antennaGain;         /// antenna gain, dB
    float _radarSysGain;        /// radar system gain, dB
    float _hBeamWidth;          /// horizontal beam width, deg
    float _vBeamWidth;          /// vertical beam width, deg
    short _radarType;             /// radar type:
                                ///     0 - ground
                                ///     1 - airborne fore
                                ///     2 - airborne aft
                                ///     3 - airborne tail
                                ///     4 - airborne lower fuselage
                                ///     5 - shipborne
    short _scanMode;              /// scan mode:
                                ///     0 - calibration
                                ///     1 - PPI
                                ///     2 - coplane
                                ///     3 - RHI
                                ///     4 - vertical
                                ///     5 - target
                                ///     6 - manual
                                ///     7 - idle
                                ///     8 - surveillance
                                ///     9 - vertical sweep
    float _scanRate;            /// nominal scan rate, deg/s
    float _startAngle;          /// nominal start angle, deg
    float _stopAngle;           /// nominal stop angle, deg
    short _nParams;             /// number of parameter descriptors for this radar
    short _nDescriptors;        /// total number of descriptors for this radar
    short _dataCompressionCode; /// data compression format code:
                                /// 0 - no compression
                                /// 1 - compression is used
    short _dataReductionAlgorithm;  /// data reduction algorithm:
                                /// 0 - no data reduction
                                /// 1 - data between two rotation angles
                                /// 2 - data between two concentric circles
                                /// 3 - data between two altitudes
    float _reductionParam1;     /// data reduction parameter #1: 
                                ///   for 1:  smallest positive angle, deg
                                ///   for 2: inner circle diameter, km
                                ///   for 3: minimum altitude, km
    float _reductionParam2;     /// data reduction parameter #2: 
                                ///   for 1:  largest positive angle, deg
                                ///   for 2: outer circle diameter, km
                                ///   for 3: maximum altitude, km
    float _radarLongitude;      /// radar longitude (for airborne radar only,
                                /// this should be airport longitude)
    float _radarLatitude;       /// radar latitude (for airborne radar only,
                                /// this should be airport latitude)
    float _radarAltitude;       /// radar altitude above mean sea level (for 
                                /// airborned radar only, this should be airport
                                /// altitude)
    float _unambiguousVelocity; /// effective unambiguous velocity, m/s
    float _unambiguousRange;    /// effective unambiguous range, km
    short _nFrequencies;        /// number of frequencies transmitted
    short _nIPPs;               /// number of inter-pulse periods (IPPs) xmitted
    float _frequencies[5];      /// frequencies, GHz
    float _IPPs[5];             /// inter-pulse periods, ms
};
#endif /*DORADERADD_H_*/