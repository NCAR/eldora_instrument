#include <cassert>
#include <sstream>
#include <cstring>
#include <arpa/inet.h>
#include "DoradeDescriptor.h"

/**
 * <p>Title: DoradeDescriptor</p>
 * <p>Description: Absract class to hold one of the many types of
 *    DORADE descriptors</p>
 * <p>Copyright: Copyright (c) 2003</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Revision: 1.3 $ $Date: 2004/04/13 17:05:50 $
 */
/* $Id: DoradeDescriptor.java,v 1.3 2004/04/13 17:05:50 burghart Exp $ */
DoradeDescriptor::DoradeDescriptor(std::string descName, unsigned int descLen) 
        throw (DescriptorException) : _descName(descName), _descLen(descLen), 
        _verbose(false) {
    if (descLen < 8) {
        throw DescriptorException("DORADE descriptor be at least 8 bytes");
    }
    if (descName.size() != 4) {
        throw DescriptorException("Bad descriptor name '" + descName + 
                "' should be 4 bytes long.");
    }
}
DoradeDescriptor::DoradeDescriptor(const unsigned char* data, 
        unsigned int datalen, bool isLittleEndian) throw (DescriptorException) :
        _verbose(false) {
    initFromData(data, datalen, isLittleEndian);
}

DoradeDescriptor::DoradeDescriptor(const unsigned char* data, 
        unsigned int datalen, bool isLittleEndian, 
        std::string expectedName) throw (DescriptorException) :
        _verbose(false) {
    initFromData(data, datalen, isLittleEndian);
    if (_descName != expectedName)
        throw DescriptorException("expected a '" + expectedName + 
                                  "' descriptor, but got '" + _descName + "'");
}

DoradeDescriptor::DoradeDescriptor(const unsigned char* data, 
        unsigned int datalen, bool isLittleEndian, std::string expectedName, 
        unsigned int expectedLen) throw (DescriptorException) :
        _verbose(false) {
    initFromData(data, datalen, isLittleEndian);
    if (_descName != expectedName)
        throw DescriptorException("expected a '" + expectedName + 
                                  "' descriptor, but got '" + _descName + "'");
    if (_descLen != expectedLen) {
        std::stringstream ss;
        ss << "expected a " << expectedLen;
        ss << " byte descriptor, but descriptor length is ";
        ss << _descLen << " bytes";
        throw DescriptorException(ss.str());
    }
}

void
DoradeDescriptor::initFromData(const unsigned char* data,
                               unsigned int datalen, bool isLittleEndian) 
throw (DescriptorException) {
    if (datalen < 8)
        throw DescriptorException("DORADE descriptor must be at least 8 bytes");
    _descName.assign((char*)data, 4);
    _descLen = grabInt(data, 4, isLittleEndian);
}

DoradeDescriptor::~DoradeDescriptor() {
}

/*
 * Unpack a two-byte integer from the given byte array.
 */
short 
DoradeDescriptor::grabShort(const unsigned char* bytes, int offset, 
                            bool isLittleEndian) {
    bool hostLittleEndian = (htonl(1) != 1);
    bool swap = (hostLittleEndian != isLittleEndian);
    short val;
    char* vptr = (char*)&val;
    if (swap) {
        vptr[1] = bytes[offset + 0];
        vptr[0] = bytes[offset + 1];
    } else {
        memcpy(vptr, bytes + offset, 2);
    }

    return (val);
}

/*
 * Unpack a four-byte integer from the given byte array.
 */
int 
DoradeDescriptor::grabInt(const unsigned char* bytes, int offset, 
                          bool isLittleEndian) {
    bool hostLittleEndian = (htonl(1) != 1);
    bool swap = (hostLittleEndian != isLittleEndian);
    int val;
    char* vptr = (char*)&val;
    if (swap) {
        vptr[3] = bytes[offset + 0];
        vptr[2] = bytes[offset + 1];
        vptr[1] = bytes[offset + 2];
        vptr[0] = bytes[offset + 3];
    } else {
        memcpy(vptr, bytes + offset, 4);
    }

    return (val);
}

/*
 * Unpack a four-byte IEEE float from the given byte array.
 */
float 
DoradeDescriptor::grabFloat(const unsigned char* bytes, int offset, 
                            bool isLittleEndian) {
    bool hostLittleEndian = (htonl(1) != 1);
    bool swap = (hostLittleEndian != isLittleEndian);
    float val;
    unsigned char *vptr = (unsigned char*)&val;

    assert(sizeof(float) == 4);
    if (swap) {
        vptr[0] = bytes[offset + 3];
        vptr[1] = bytes[offset + 2];
        vptr[2] = bytes[offset + 1];
        vptr[3] = bytes[offset + 0];
    } else {
        memcpy(vptr, bytes + offset, 4);
    }
    return val;
}

/*
 * Unpack an eight-byte IEEE float from the given byte array.
 */
double 
DoradeDescriptor::grabDouble(const unsigned char* bytes, int offset, 
                             bool isLittleEndian) {
    bool hostLittleEndian = (htonl(1) != 1);
    bool swap = (hostLittleEndian != isLittleEndian);
    double val;
    unsigned char *vptr = (unsigned char*)&val;

    assert(sizeof(double) == 8);
    if (swap) {
        vptr[0] = bytes[offset + 7];
        vptr[1] = bytes[offset + 6];
        vptr[2] = bytes[offset + 5];
        vptr[3] = bytes[offset + 4];
        vptr[4] = bytes[offset + 3];
        vptr[5] = bytes[offset + 2];
        vptr[6] = bytes[offset + 1];
        vptr[7] = bytes[offset + 0];
    } else {
        memcpy(vptr, bytes + offset, 8);
    }
    return val;
}

/*
 * Pack a short into a 2-byte DORADE representation.
 */
std::ostream&
DoradeDescriptor::putShort(std::ostream& os, short value, bool asLittleEndian) 
{
    bool hostLittleEndian = (htonl(1) != 1);
    bool swap = (hostLittleEndian != asLittleEndian);
    assert(sizeof(short) == 2);
    putBytes(os, (char*)&value, 2, swap);
    return os;
}

/*
 * Pack an int into a 4-byte DORADE representation.
 */
std::ostream&
DoradeDescriptor::putInt(std::ostream& os, int value, bool asLittleEndian) 
{
    bool hostLittleEndian = (htonl(1) != 1);
    bool swap = (hostLittleEndian != asLittleEndian);
    assert(sizeof(int) == 4);
    putBytes(os, (char*)&value, 4, swap);
    return os;
}

/*
 * Pack a float into a 4-byte DORADE representation.
 */
std::ostream&
DoradeDescriptor::putFloat(std::ostream& os, float value, bool asLittleEndian) 
{
    bool hostLittleEndian = (htonl(1) != 1);
    bool swap = (hostLittleEndian != asLittleEndian);
    assert(sizeof(float) == 4);
    putBytes(os, (char*)&value, 4, swap);
    return os;
}

/*
 * Pack a double into a 8-byte DORADE representation.
 */
std::ostream&
DoradeDescriptor::putDouble(std::ostream& os, double value, bool asLittleEndian) 
{
    bool hostLittleEndian = (htonl(1) != 1);
    bool swap = (hostLittleEndian != asLittleEndian);
    assert(sizeof(double) == 8);
    putBytes(os, (char*)&value, 8, swap);
    return os;
}

/*
 * Pack a series of bytes to the given stream.
 */
std::ostream&
DoradeDescriptor::putBytes(std::ostream& os, const char* bytes, 
        unsigned int count, bool swap)
{
    int start = swap ? count - 1 : 0;
    int end = swap ? -1 : count;
    int step = swap ? -1 : 1;
    for (int i = start; i != end; i += step)
        os << bytes[i];
    return os;
}

/*
 * Set verbose state of the descriptor.
 */
bool 
DoradeDescriptor::setVerbose(bool verbose) 
{
    bool oldVerbose = _verbose;
    _verbose = verbose;
    return oldVerbose;
}

bool 
DoradeDescriptor::getVerbose() 
{
    return _verbose;
}

std::ostream& 
operator<<(std::ostream& out, const DoradeDescriptor& desc)
{
    return desc.printTo(out);
};

std::ostream& 
DoradeDescriptor::printTo(std::ostream& os) const
{
    os << _descName << " (" << _descLen << " bytes)" << std::endl;
    return os;
};

std::ostream&
DoradeDescriptor::streamTo(std::ostream& os, bool asLittleEndian)
{
    char null = '\0';
    putBytes(os, _descName.data(), 4, false);
    putInt(os, _descLen, asLittleEndian);
    // Fill the rest of the descriptor with nulls
    for (int i = 0; i < _descLen - 8; i++)
        putBytes(os, &null, 1, false);
    return os;
}
