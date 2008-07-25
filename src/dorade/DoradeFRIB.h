#ifndef DORADEFRIB_H_
#define DORADEFRIB_H_

#include "DoradeDescriptor.h"

/**
 * <p>Title: DoradeFRIB</p>
 * <p>Description: DORADE Field Radar Information Block (FRIB)</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Revision: 1.3 $ $Date: 2004/04/13 17:05:50 $
 */

class DoradeFRIB : public DoradeDescriptor {
public:
    DoradeFRIB(const unsigned char *data, unsigned int datalen, 
               bool isLittleEndian) throw (DescriptorException);
    
    DoradeFRIB(int dataSysId, float waveguideLossOut, float waveguideLossIn,
        float rjointLoss, float antennaVDim, float antennaHDim,
        float antennaNoiseTemp, float rcvrNoiseFigure, float xmitPower[5],
        float xBandGain, float rcvrGain[5], float ifGain[5],
        float conversionGain, float scaleFactor[5], float processorConst,
        int tubeToAntennaDelay, int chipToADDelay, int testPulseADDelay,
        int modulatorRiseTime, int modulatorFallTime, float peakPwrOffset,
        float testPulsePwrOffset, float antennaEAngle, float antennaHAngle,
        float encoderAntennaUp, float pitchAntennaUp, short extraDataFlag,
        short indepFreqGate, short tsGate, short numBaseParams,
        std::string hdrFileName);

    // accessor methods
    // @TODO add accessor methods
    float getxBandGain() { return _xBandGain;}
    float getWaveGuideLossIn() { return _waveguideLossIn;}
    float getEncodeAntennaUp() { return _encoderAntennaUp;}
        
    
    std::ostream& printTo(std::ostream& stream) const;
    std::ostream& streamTo(std::ostream& os, bool asLittleEndian);
private:
    int _dataSysId;             /// data system id
    float _waveguideLossOut;    /// waveguide loss: xmitter to antenna, dB
    float _waveguideLossIn;     /// waveguide loss: antenna to LNA, dB
    float _rjointLoss;          /// rotary joint loss, dB
    float _antennaVDim;         /// antenna vertical dimension, m
    float _antennaHDim;         /// antenna horizontal dimension, m
    float _antennaNoiseTemp;    /// antenna noise temperature, K
    float _rcvrNoiseFigure;     /// receiver noise figure, dB
    float _xmitPower[5];        /// nominal peak xmit power per channel, dBm
    float _xBandGain;           /// X band gain, dB
    float _rcvrGain[5];         /// measured receiver gain per channel, dB
    float _ifGain[5];           /// measured IF gain per channel, dB
    float _conversionGain;      /// A/D conversion gain, dB
    float _scaleFactor[5];      /// scale factor to account for differences in
                                /// the individual channels, and the inherent
                                /// gain due to summing over the dwell time
    float _processorConst;      /// constant used to scale dBz to units the 
                                /// display processors understand
    int _tubeToAntennaDelay;    /// delay from RF being applied to
                                /// tube and energy leaving antenna, ns
    int _chipToADDelay;         /// delay from a chip generated in the 
                                /// timing module and the RF pulse entering
                                /// the A/D converters, ns
    int _testPulseADDelay;      /// delay from timing module test pulse edge 
                                /// and test pulse arrival at A/D, ns
    int _modulatorRiseTime;     /// HPA modulator rise time (time between HPA 
                                /// video on and modulator full on), ns
    int _modulatorFallTime;     /// HPA modulator fall time (time between HPA
                                /// video off and modulator full off), ns
    float _peakPwrOffset;     /// add this to the power meter reading of 
                                /// peak output power to yield actual peak
                                /// power, dB
    float _testPulsePwrOffset;     /// add this to the power meter reading of 
                                /// test pulse power to yield actual test pulse
                                /// power, dB
    float _antennaEAngle;       /// E plane angle (tilt): angle between radar
                                /// beam and a plane normal to the aircraft
                                /// longitudinal axis (positive is toward the
                                /// nose of the aircraft), deg
    float _antennaHAngle;       /// H plane angle: following convention for 
                                /// roll, deg
    float _encoderAntennaUp;    /// Encoder reading minus IRU roll angle
                                /// when antenna is up and horizontal
    float _pitchAntennaUp;      /// antenna pitch angle (measured with
                                /// transit) minus IRU pitch angle when
                                /// antenna is pointing up
    short _extraDataFlag;       /// bit field listing extra data recorded:
                                /// 0x01 - independent frequency data recorded
                                /// 0x02 - time series data recorded
    short _indepFreqGate;       /// gate number for the independent frequency
                                /// data comes from
    short _tsGate;              /// gate number for time series data
    short _numBaseParams;       /// number of base parameters
    std::string _hdrFileName;   /// name of the header file, 80 characters
};
#endif /*DORADEFRIB_H_*/
