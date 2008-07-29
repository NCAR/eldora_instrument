#include <sstream>
#include <iomanip>
#include <cassert>
#include <cstring>
#include "DoradeCSPD.h"

/**
 * <p>Title: DoradeCSPD</p>
 * <p>Description: DORADE Cell SPacing Descriptor (CSPD)</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Id$
 */
DoradeCSPD::DoradeCSPD(const unsigned char *data, unsigned int datalen, 
                       bool isLittleEndian) throw (DescriptorException) :
    DoradeDescriptor(data, datalen, isLittleEndian, "CSPD", 36) { 
    //
    // unpack
    //
    _numSegments = grabShort(data, 8, isLittleEndian);
    _distToFirstGate = grabShort(data, 10, isLittleEndian);
    for (int seg = 0; seg < 6; seg++)
        _spacing[seg] = grabShort(data, 12 + 2 * seg, isLittleEndian);
    for (int seg = 0; seg < 6; seg++)
        _cellsInSegment[seg] = grabShort(data, 24 + 2 * seg, isLittleEndian);
    //
    // debugging output
    //
    if (_verbose)
        std::cout << *this;
};

DoradeCSPD::DoradeCSPD(short numSegments, short distToFirstGate, 
    short spacing[6], short cellsInSegment[6]) : DoradeDescriptor("CSPD", 36),
    _numSegments(numSegments), _distToFirstGate(distToFirstGate) {
    assert(sizeof(spacing) == sizeof(_spacing));
    memcpy(_spacing, spacing, sizeof(_spacing));
    assert(sizeof(cellsInSegment) == sizeof(_cellsInSegment));
    memcpy(_cellsInSegment, cellsInSegment, sizeof(_cellsInSegment));

    if (_verbose)
        std::cout << *this;
}

std::ostream& 
DoradeCSPD::printTo(std::ostream& os) const
{
    DoradeDescriptor::printTo(os);
    // @TODO add details here
    if (_verbose) {
        os << "no CSPD detail printing yet..." << std::endl;
    }
    return os;
}

std::ostream&
DoradeCSPD::streamTo(std::ostream& os, bool asLittleEndian)
{
    putBytes(os, _descName.data(), 4, false);   // no swapping for char data
    putInt(os, _descLen, asLittleEndian);
    putShort(os, _numSegments, asLittleEndian);
    putShort(os, _distToFirstGate, asLittleEndian);
    for (int seg = 0; seg < 6; seg++)
        putShort(os, _spacing[seg], asLittleEndian);
    for (int seg = 0; seg < 6; seg++)
        putShort(os, _cellsInSegment[seg], asLittleEndian);
    return os;
}
