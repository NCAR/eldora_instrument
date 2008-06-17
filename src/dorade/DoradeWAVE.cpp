#include <sstream>
#include <iomanip>
#include "DoradeWAVE.h"

/**
 * <p>Title: DoradeWAVE</p>
 * <p>Description: DORADE waveform information block descriptor</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Revision: 1.3 $ $Date: 2004/04/13 17:05:50 $
 */
DoradeWAVE::DoradeWAVE(const unsigned char *data, unsigned int datalen, 
                       bool isLittleEndian) throw (DescriptorException) :
    DoradeDescriptor(data, datalen, isLittleEndian, "WAVE", 364) { 
    //
    // unpack
    //
    foo;
    // Pulse scheme file name.  Force to 16 characters by silently truncating
    // or appending nulls.
    _psFileName = std::string((const char*)data + 8, 16);
    _psFileName.resize(16);
    
    // number of chips in a repeat, one per frequency + maximum(?)
    for (int f = 0; f < 6; f++)
        _numChips[f] = grabShort(data, 12 + 2 * f, isLittleEndian);
    
    // 256-byte blanking sequence
    memcpy(_blankingSequence, data + 36, 256);
    
    // milliseconds per chip repeat sequence
    _repeatPeriod = grabFloat(data, 292, isLittleEndian);
    
    // number of sequence repeats per dwell
    _repeatsPerDwell = grabShort(data, 296, isLittleEndian);
    
    //
    // PCP below refers to Pulse Count Period, or the period which is the 
    // greatest common divisor of all of the pulse repetition times 
    // (PRTs) in use.
    //
    
    // number of PCP in a repeat sequence
    _totalPCP = grabShort(data, 298, isLittleEndian);
    
    // offset to start of each chip in 60 MHz counts, one per freq. +
    // one for preknock
    for (int f = 0; f < 6; f++)
        _chipOffset[f] = grabShort(data, 300 + 2 * f, isLittleEndian);
    
    // width of each chip in 60 MHz counts, one per freq. + one for preknock
    for (int f = 0; f < 6; f++)
        _chipWidth[f] = grabShort(data, 312 + 2 * f, isLittleEndian);

    // # of PCP that set unambiguous range, after realtime unfolding
    _urPCP = grabFloat(data, 324, isLittleEndian);

    // # of PCP that set unambiguous velocity, after realtime unfolding
    _uvPCP = grabFloat(data, 328, isLittleEndian);
    
    // number of sampled gates, per freq. + one for ??
    for (int f = 0; f < 6; f++)
        _numGates[f] = grabShort(data, 332 + 2 * f, isLittleEndian);
    
    // time to first gate and time per gate for each frequency, in 60 MHz counts
    // (i.e., _gateTime[0][0] is time to first gate for frequency 0, and 
    // _gateTime[0][1] is time per gate for frequency 0)
    for (int f = 0; f < 5; f++) {
        for (int i = 0; i < 2; i++) {
            _gateTime[f][i] = grabShort(data, 344 + (2 * f + i) * 2, 
                                        isLittleEndian);
        }
    }

    //
    // debugging output
    //
    if (_verbose)
        std::cout << *this;
}

DoradeWAVE::DoradeWAVE(boost::posix_time::ptime rayDateTime, int sweepNumber,
    float azimuth, float elevation, float peakXmitPower, float scanRate,
    int rayStatus) : 
    DoradeDescriptor("WAVE", 44),
    _sweepNumber(sweepNumber), _azimuth(azimuth), _elevation(elevation),
    _peakXmitPower(peakXmitPower), _scanRate(scanRate), _rayStatus(rayStatus) {
    setRayDateTime(rayDateTime);
    if (_verbose)
        std::cout << *this;
}

std::ostream& 
DoradeWAVE::printTo(std::ostream& os) const
{
    DoradeDescriptor::printTo(os);
    if (_verbose) {
        os << "sweep number: " << _sweepNumber << std::endl;
        os << "Julian day: " << _julianDay << std::endl;
        os << "hh:mm:ss.mmm: ";
        os << std::setw(2) << std::setfill('0') << _hour << ":";
        os << std::setw(2) << std::setfill('0') << _minute << ":";
        os << std::setw(2) << std::setfill('0') << _second << ".";
        os << std::setw(3) << std::setfill('0') << _millisecond << std::endl;
        os << "azimuth: " << _azimuth << std::endl;
        os << "elevation: " << _elevation << std::endl;
        os << "peak power: " << _peakXmitPower << std::endl;
        os << "scan rate: " << _scanRate << std::endl;
        os << "ray status: " << _rayStatus << std::endl;
    }
    return os;
}

void
DoradeWAVE::setRayDateTime(ptime time) {
    _rayDateTime = time;
    // In addition to setting _rayDateTime, we need to set each of the 
    // individual time pieces.
    _julianDay = time.date().day_of_year();
    _hour = time.time_of_day().hours();
    _minute = time.time_of_day().minutes();
    _second = time.time_of_day().seconds();
    _millisecond = time.time_of_day().total_milliseconds() % 1000;
}

std::ostream&
DoradeWAVE::streamTo(std::ostream& os, bool asLittleEndian)
{
    putBytes(os, _descName.data(), 4, false); // no swapping for char data
    putInt(os, _descLen, asLittleEndian);
    putInt(os, _sweepNumber, asLittleEndian);
    putInt(os, _julianDay, asLittleEndian);
    putShort(os, _hour, asLittleEndian);
    putShort(os, _minute, asLittleEndian);
    putShort(os, _second, asLittleEndian);
    putShort(os, _millisecond, asLittleEndian);
    putFloat(os, _azimuth, asLittleEndian);
    putFloat(os, _elevation, asLittleEndian);
    putFloat(os, _peakXmitPower, asLittleEndian);
    putFloat(os, _scanRate, asLittleEndian);
    putInt(os, _rayStatus, asLittleEndian);
    return os;
}
