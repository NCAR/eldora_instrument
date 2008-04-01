#include <cassert>
#include <sstream>
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
DoradeDescriptor::DoradeDescriptor(const unsigned char* data, 
        unsigned int datalen, bool isLittleEndian) 
throw (DescriptorException) :
        _verbose(false) {
    initFromData(data, datalen, isLittleEndian);
}

DoradeDescriptor::DoradeDescriptor(const unsigned char* data, 
        unsigned int datalen, bool isLittleEndian, 
        std::string expectedName) 
throw (DescriptorException) :
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
        ss << "expected a " << expectedLen << " byte descriptor, but got ";
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

/**
 * Unpack a two-byte integer from the given byte array.
 * @param bytes  byte array to be read
 * @param offset  number of bytes to skip in the byte array before reading
 * @param isLlittleEndian  true iff bytes contains little-endian data
 * @return the unpacked integer value
 */
short 
DoradeDescriptor::grabShort(const unsigned char* bytes, int offset, 
                            bool bytesLittleEndian) {
    bool hostLittleEndian = (htonl(1) != 1);
    bool swap = (hostLittleEndian != bytesLittleEndian);
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

/**
 * Unpack a four-byte integer from the given byte array.
 * @param bytes  byte array to be read
 * @param offset  number of bytes to skip in the byte array before reading
 * @param isLlittleEndian  true iff bytes contains little-endian data
 * @return the unpacked integer value
 */
int 
DoradeDescriptor::grabInt(const unsigned char* bytes, int offset, 
                          bool bytesLittleEndian) {
    bool hostLittleEndian = (htonl(1) != 1);
    bool swap = (hostLittleEndian != bytesLittleEndian);
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

//    /**
//     * Unpack a four-byte integer from the given byte array.
//     * @param bytes  byte array to be read
//     * @param offset  number of bytes to skip in the byte array before reading
//     * @return the unpacked integer value
//     */
//    int grabInt(byte[] bytes, int offset) {
//        return grabInt(bytes, offset, littleEndianData);
//    }

/**
 * Unpack a four-byte IEEE float from the given byte array.
 * @param bytes  byte array to be read
 * @param offset  number of bytes to skip in the byte array before reading
 * @param isLittleEndian true iff bytes contains little-endian data
 * @return the unpacked float value
 */
float 
DoradeDescriptor::grabFloat(const unsigned char* bytes, int offset, 
                            bool bytesLittleEndian) {
    bool hostLittleEndian = (htonl(1) != 1);
    bool swap = (hostLittleEndian != bytesLittleEndian);
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

/**
 * Unpack an eight-byte IEEE float from the given byte array.
 * @param bytes  byte array to be read
 * @param offset  number of bytes to skip in the byte array before reading
 * @param isLittleEndian true iff bytes contains little-endian data
 * @return the unpacked double value
 */
double 
DoradeDescriptor::grabDouble(const unsigned char* bytes, int offset, 
                             bool bytesLittleEndian) {
    bool hostLittleEndian = (htonl(1) != 1);
    bool swap = (hostLittleEndian != bytesLittleEndian);
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

/**
 * Set verbose state of the descriptor.
 * @param verbose true iff the descriptor should be verbose.
 * @return the previous verbose state.
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
