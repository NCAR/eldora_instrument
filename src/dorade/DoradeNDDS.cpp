#include <sstream>
#include <iomanip>
#include <cassert>
#include "DoradeNDDS.h"

/**
 * <p>Title: DoradeNDDS</p>
 * <p>Description: DORADE Navigational Data Descriptor (NDDS)</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Id$
 */
DoradeNDDS::DoradeNDDS(const unsigned char *data, unsigned int datalen, 
                       bool isLittleEndian) throw (DescriptorException) :
    DoradeDescriptor(data, datalen, isLittleEndian, "NDDS", 16) { 
    //
    // unpack
    //
    _insFlag = grabShort(data, 8, isLittleEndian);
    _gpsFlag = grabShort(data, 10, isLittleEndian);
    _minirimsFlag = grabShort(data, 12, isLittleEndian);
    _kalmanFlag = grabShort(data, 14, isLittleEndian);
    //
    // debugging output
    //
    if (_verbose)
        std::cout << *this;
};

DoradeNDDS::DoradeNDDS(short insFlag, short gpsFlag, short minirimsFlag, 
    short kalmanFlag) : DoradeDescriptor("NDDS", 16),
    _insFlag(insFlag), _gpsFlag(gpsFlag), _minirimsFlag(minirimsFlag),
    _kalmanFlag(kalmanFlag) {
    if (_verbose)
        std::cout << *this;
}

std::ostream& 
DoradeNDDS::printTo(std::ostream& os) const
{
    DoradeDescriptor::printTo(os);
    // @TODO add details here
    if (_verbose) {
        os << "no NDDS detail printing yet..." << std::endl;
    }
    return os;
}

std::ostream&
DoradeNDDS::streamTo(std::ostream& os, bool asLittleEndian)
{
    putBytes(os, _descName.data(), 4, false);   // no swapping for char data
    putInt(os, _descLen, asLittleEndian);
    putShort(os, _insFlag, asLittleEndian);
    putShort(os, _gpsFlag, asLittleEndian);
    putShort(os, _minirimsFlag, asLittleEndian);
    putShort(os, _kalmanFlag, asLittleEndian);
    return os;
}
