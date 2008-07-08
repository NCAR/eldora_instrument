#include <sstream>
#include <iomanip>
#include <cassert>
#include "DoradeRAWD.h"

/**
 * <p>Title: DoradeRAWD</p>
 * <p>Description: DORADE Raw Data Descriptor (RAWD)</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Id$
 */
DoradeRAWD::DoradeRAWD(const unsigned char *data, unsigned int datalen, 
                       bool isLittleEndian) throw (DescriptorException) :
    DoradeDescriptor(data, datalen, isLittleEndian, "RAWD") { 
    //
    // Unpack.  Note that RAWD is a weird descriptor, since it lacks the
    // 4-byte descriptor length at byte 4 that all the other descriptors
    // have.  We have to calculate descriptor length ourselves.
    //
                           
    // force _xLabel to 8 characters by truncating or appending nulls
    _xLabel = std::string((const char*)data + 4, 8);
    _xLabel.resize(8);
    _xMax = grabFloat(data, 12, isLittleEndian);
    _xMin = grabFloat(data, 16, isLittleEndian);    
    // force _yLabel to 8 characters by truncating or appending nulls
    _yLabel = std::string((const char*)data + 20, 8);
    _yLabel.resize(8);
    _yMax = grabFloat(data, 28, isLittleEndian);
    _yMin = grabFloat(data, 32, isLittleEndian);    
    _numPoints = grabInt(data, 36, isLittleEndian);

    int len = 32 * _numPoints;
    _data = new char[len];
    memcpy(_data, data + 40, len);
    
    // Calculate the descriptor length ourselves, since the descriptor does
    // not carry the information directly as all other descriptors do.
    _descLen = 40 + len;
    
    if (_verbose)
        std::cout << *this;
};

DoradeRAWD::DoradeRAWD(std::string xLabel, float xMax, float xMin,
        std::string yLabel, float yMax, float yMin, int numPoints,
        const char* data) : 
    DoradeDescriptor("RAWD", -1), _xLabel(xLabel), _xMax(xMax), _xMin(xMin),
    _yLabel(yLabel), _yMax(yMax), _yMin(yMin), _numPoints(numPoints) {
    // force label lengths to 8 characters, either by truncating or by
    // appending nulls
    _xLabel.resize(8);
    _yLabel.resize(8);

    int len = 32 * _numPoints;
    _data = new char[len];
    memcpy(_data, data, len);
    
    // We have to calculate the descriptor length ourselves
    _descLen = 40 + len;
}

DoradeRAWD::~DoradeRAWD() {
    delete _data;
}

std::ostream& 
DoradeRAWD::printTo(std::ostream& os) const
{
    DoradeDescriptor::printTo(os);
    // @TODO add details here
    if (_verbose) {
        os << "no RAWD detail printing yet..." << std::endl;
    }
    return os;
}

std::ostream&
DoradeRAWD::streamTo(std::ostream& os, bool asLittleEndian)
{
    putBytes(os, _descName.data(), 4, false);   // no swapping for char data
// All other descriptors put the _descLen here, but RAWD is an exception
//    putInt(os, _descLen, asLittleEndian);
    putBytes(os, _xLabel.data(), 8, false);   // no swapping for char data
    putFloat(os, _xMax, asLittleEndian);
    putFloat(os, _xMin, asLittleEndian);
    putBytes(os, _yLabel.data(), 8, false);   // no swapping for char data
    putFloat(os, _yMax, asLittleEndian);
    putFloat(os, _yMin, asLittleEndian);
    putInt(os, _numPoints, asLittleEndian);
    putBytes(os, _data, sizeof(_data), false);
    return os;
}