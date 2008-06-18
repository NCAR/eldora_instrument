#ifndef DORADEWAVE_H_
#define DORADEWAVE_H_

#include "DoradeDescriptor.h"

/**
 * <p>Title: DoradeWAVE</p>
 * <p>Description: DORADE waveform information block descriptor</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Revision: 1.3 $ $Date: 2004/04/13 17:05:50 $
 */

class DoradeWAVE : public DoradeDescriptor {
public:
    DoradeWAVE(const unsigned char *data, unsigned int datalen, 
               bool isLittleEndian) throw (DescriptorException);
    
    DoradeWAVE(std::string psFileName, short numChips[6], 
        char blankingSequence[256], float repeatPeriod, 
        short repeatsPerDwell, short totalPCP, short chipOffset[6],
        short chipWidth[6], float urPCP, float uvPCP, short numGates[6],
        short gateTime[5][2]);
    
    // accessor methods
    std::string getPSFileName() { return _psFileName; }
    short getNumChips(int freqnum) { return _numChips[freqnum]; }
    const char* getBlankingSequence() { return _blankingSequence; }
    float getRepeatPeriod() { return _repeatPeriod; }
    short getRepeatsPerDwell() { return _repeatsPerDwell; }
    short getTotalPCP() { return _totalPCP; }
    short getChipOffset(int freqnum) { return _chipOffset[freqnum]; }
    short getChipWidth(int freqnum) { return _chipWidth[freqnum]; }
    float getUrPCP() { return _urPCP; }
    float getUvPCP() { return _uvPCP; }
    short getNumGates(int freqnum) { return _numGates[freqnum]; }
    short getTimeToFirstGate(int freqnum) { return _gateTime[freqnum][0]; }
    short getTimePerGate(int freqnum) { return _gateTime[freqnum][1]; }
    
    std::ostream& printTo(std::ostream& stream) const;
    std::ostream& streamTo(std::ostream& os, bool asLittleEndian);
private:
    //
    // PCP below refers to Pulse Count Period, or the period which is the 
    // greatest common divisor of all of the pulse repetition times 
    // (PRTs) in use.
    //
    std::string _psFileName;    /// pulse scheme file name, 16 characters
    short _numChips[6];         /// number of chips in a repeat [per freq. + maximum(?)]
    char _blankingSequence[256];/// blanking RAM sequence
    float _repeatPeriod;        /// milliseconds per chip repeat sequence
    short _repeatsPerDwell;     /// number of sequence repeats per dwell
    short _totalPCP;            /// number of PCP in a repeat sequence
    short _chipOffset[6];       /// offset to start of each chip, in 60 MHz counts
                                /// [per freq. + preknock]
    short _chipWidth[6];        /// width of each chip, in 60 MHz counts
                                /// [pre freq. + preknock]
    float _urPCP;               /// # of PCP that set unambiguous range, after
                                /// realtime unfolding
    float _uvPCP;               /// # of PCP that set the unambiguous velocity, 
                                /// after realtime unfolding
    short _numGates[6];         /// number of gates sampled [per freq. + one extra for ?]
    short _gateTime[5][2];      /// time to first gate and time per gate for
                                /// each frequency, in 60 MHz counts
                                /// (i.e., _gateTime[0][0] is time to first gate
                                /// for frequency 0, and _gateTime[0][1] is
                                /// time per gate for frequency 0)
};
#endif /*DORADEWAVE_H_*/
