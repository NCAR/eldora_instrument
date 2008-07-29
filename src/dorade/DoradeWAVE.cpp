#include <sstream>
#include <iomanip>
#include <cassert>
#include <cstring>
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

    // Pulse scheme file name.
    // Get exactly 16 chars, then shorten if there's a null somewhere.
    _psFileName = std::string((const char*)data + 8, 16);
    _psFileName = std::string(_psFileName.c_str());
    
    for (int f = 0; f < 6; f++)
        _numChips[f] = grabShort(data, 24 + 2 * f, isLittleEndian);

    // 256-byte blanking sequence
    memcpy(_blankingSequence, data + 36, 256);
    
    _repeatPeriod = grabFloat(data, 292, isLittleEndian);
    _repeatsPerDwell = grabShort(data, 296, isLittleEndian);
    _totalPCP = grabShort(data, 298, isLittleEndian);
    for (int f = 0; f < 6; f++)
        _chipOffset[f] = grabShort(data, 300 + 2 * f, isLittleEndian);
    for (int f = 0; f < 6; f++)
        _chipWidth[f] = grabShort(data, 312 + 2 * f, isLittleEndian);
    _urPCP = grabFloat(data, 324, isLittleEndian);
    _uvPCP = grabFloat(data, 328, isLittleEndian);
    for (int f = 0; f < 6; f++)
        _numGates[f] = grabShort(data, 332 + 2 * f, isLittleEndian);
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

DoradeWAVE::DoradeWAVE(std::string psFileName, short numChips[6], 
    char blankingSequence[256], float repeatPeriod, 
    short repeatsPerDwell, short totalPCP, short chipOffset[6],
    short chipWidth[6], float urPCP, float uvPCP, short numGates[6],
    short gateTime[5][2]) : DoradeDescriptor("WAVE", 364),
    _psFileName(psFileName), _repeatPeriod(repeatPeriod), 
    _repeatsPerDwell(repeatsPerDwell), _totalPCP(totalPCP), 
    _urPCP(urPCP), _uvPCP(uvPCP) {
    // truncate psFileName to 16 characters if necessary
    if (_psFileName.size() > 16)
        _psFileName.resize(16);
    assert(sizeof(blankingSequence) == sizeof(_blankingSequence));
    memcpy(_blankingSequence, blankingSequence, sizeof(_blankingSequence));
    assert(sizeof(numChips) == sizeof(_numChips));
    memcpy(_numChips, numChips, sizeof(_numChips));
    assert(sizeof(chipOffset) == sizeof(_chipOffset));
    memcpy(_chipOffset, chipOffset, sizeof(_chipOffset));
    assert(sizeof(chipWidth) == sizeof(_chipWidth));
    memcpy(_chipWidth, chipWidth, sizeof(_chipWidth));
    assert(sizeof(numGates) == sizeof(_numGates));
    memcpy(_numGates, numGates, sizeof(_numGates));
    assert(sizeof(gateTime) == sizeof(_gateTime));
    memcpy(_gateTime, gateTime, sizeof(_gateTime));
    
    if (_verbose)
        std::cout << *this;
}

std::ostream& 
DoradeWAVE::printTo(std::ostream& os) const
{
    DoradeDescriptor::printTo(os);
    // @TODO add details here
    if (_verbose) {
        os << "no WAVE detail printing yet..." << std::endl;
    }
    return os;
}

std::ostream&
DoradeWAVE::streamTo(std::ostream& os, bool asLittleEndian)
{
    putBytes(os, _descName.data(), 4, false);   // no swapping for char data
    putInt(os, _descLen, asLittleEndian);
    std::string str(_psFileName);
    str.resize(16); // force to exactly 16 chars
    putBytes(os, str.data(), 16, false);// no swapping for char data
    for (int f = 0; f < 6; f++)
        putShort(os, _numChips[f], asLittleEndian);
    putBytes(os, _blankingSequence, 256, false);// no swapping for char data
    putFloat(os, _repeatPeriod, asLittleEndian);
    putShort(os, _repeatsPerDwell, asLittleEndian);
    putShort(os, _totalPCP, asLittleEndian);
    for (int f = 0; f < 6; f++)
        putShort(os, _chipOffset[f], asLittleEndian);
    for (int f = 0; f < 6; f++)
        putShort(os, _chipWidth[f], asLittleEndian);
    putFloat(os, _urPCP, asLittleEndian);
    putFloat(os, _uvPCP, asLittleEndian);
    for (int f = 0; f < 6; f++)
        putShort(os, _numGates[f], asLittleEndian);
    for (int f = 0; f < 5; f++)
        for (int i = 0; i < 2; i++)
            putShort(os, _gateTime[f][i], asLittleEndian);
    return os;
}
