#ifndef DORADEPARM_H_
#define DORADEPARM_H_

#include "DoradeDescriptor.h"

/**
 * <p>Title: DoradePARM</p>
 * <p>Description: DORADE PARaMeter Descriptor (PARM)</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Revision: 1.3 $ $Date: 2004/04/13 17:05:50 $
 */

class DoradePARM : public DoradeDescriptor {
public:
    DoradePARM(const unsigned char *data, unsigned int datalen, 
        bool isLittleEndian) throw (DescriptorException);
    
    DoradePARM(std::string name, std::string description, std::string units,
        short usedIPPs, short usedFreqs, float rcvrBandwidth, short pulseWidth,
        short polarization, short nSamples, short paramType,
        std::string thresholdParm, float thresholdValue, float scale,
        float offset, int missingValue);
    
    // accessor methods
    // @TODO add accessor methods
    std::string getParamName() { return _name; }
    int getParamType() { return _paramType; }
    
    std::ostream& printTo(std::ostream& stream) const;
    std::ostream& streamTo(std::ostream& os, bool asLittleEndian);
private:
    std::string _name;          /// name, 8 characters
    std::string _description;   /// description, 40 characters
    std::string _units;         /// units, 8 characters
    short _usedIPPs;            /// bit flag showing which inter-pulse
                                /// periods are used for this parameter:
                                ///   bit 0 set to 1 if IPP 0 is used
                                ///   bit 1 set to 1 if IPP 1 is used
                                ///   etc.
    short _usedFreqs;           /// bit flag showing which frequencies are used
                                /// for this parameter:
                                /// bit 0 set to 1 if frequency 0 is used
                                /// bit 1 set to 1 if frequency 1 is used
                                /// etc.
    float _rcvrBandwidth;       /// receiver bandwidth, MHz
    short _pulseWidth;          /// pulse width, m
    short _polarization;        /// polarization:
                                ///   0 - horizontal
                                ///   1 - vertical
                                ///   2 - circular, right-handed
                                ///   3 - elliptical
                                ///   4 - circular, left-handed
                                ///   5 - dual polarization
    short _nSamples;            /// number of samples in a dwell time
    short _paramType;           /// parameter type:
                                ///   1 - 8-bit integer
                                ///   2 - 16-bit integer
                                ///   3 - 32-bit integer
                                ///   4 - 32-bit IEEE float
    std::string _thresholdParm; /// threshold parameter name, 8 characters
    float _thresholdValue;      /// threshold value, units as theshold parameter
    float _scale;               /// scale factor: meteorological value = 
                                /// (recorded value - offset) / scale
    float _offset;              /// offset factor: see above
    int _missingValue;          /// missing/deleted value flag: it is 
                                /// recommended that -128 is used for the 8-bit 
                                /// parameter type, and -999 for all other types
};
#endif /*DORADEPARM_H_*/
