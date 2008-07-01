#ifndef DORADEDESCRIPTOR_H_
#define DORADEDESCRIPTOR_H_

#include <exception>
#include <iostream>
#include <string>
#include <map>

/**
 * <p>Title: DescriptorException</p>
 * <p>Description: generic class for <code>Exception</code>-s from
 * <code>DoradeDescriptor</code>-s.</p>
 * <p>Copyright: Copyright (c) 2003</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @version $Revision: 1.3 $ $Date: 2004/04/13 17:05:50 $
 */
class DescriptorException {
public:
    DescriptorException(std::string msg) { _msg = msg; }
    
    const char* what() const throw() {
        return _msg.c_str();
    }
private:
    std::string _msg;
};

/**
 * <p>Title: DoradeDescriptor</p>
 * <p>Description: Abstract class providing shared function for all 
 * DORADE descriptors</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Revision$
 */

class DoradeDescriptor {
public:
    std::string getDescName() { return _descName; }
    unsigned int getDescLen() { return _descLen; }

    DoradeDescriptor(std::string descName, unsigned int descLen)
        throw (DescriptorException);
    
    DoradeDescriptor(const unsigned char* data, unsigned int datalen,
                     bool isLittleEndian) throw (DescriptorException);

    DoradeDescriptor(const unsigned char* data, unsigned int datalen,
                     bool isLittleEndian, std::string expectedName) 
        throw (DescriptorException);
    
    DoradeDescriptor(const unsigned char* data, unsigned int datalen,
                     bool isLittleEndian, std::string expectedName,
                     unsigned int expectedLen) throw (DescriptorException);
    
    virtual ~DoradeDescriptor();
    /**
     * Set verbose state of the descriptor.
     * @param verbose true iff the descriptor should be verbose.
     * @return the previous verbose state.
     */
    bool setVerbose(bool verbose);
    bool getVerbose();
    /**
     * Put the binary representation of the descriptor (i.e., *real* DORADE)
     * to the given stream.
     * @param os the stream to be written.
     * @param asLittleEndian true iff the binary representation should be 
     *      written in little-endian order.  Although the DORADE spec calls for
     *      big-endian, there is a long history of writing little-endian
     *      DORADE(-like) files.  This parameter defaults to false.
     */
    virtual std::ostream& streamTo(std::ostream& os, 
            bool asLittleEndian = false);
    /**
     * Print a text representation of the descriptor to the given stream
     */
    virtual std::ostream& printTo(std::ostream& os) const;
    
    friend std::ostream& operator<<(std::ostream& out, const DoradeDescriptor&);

protected:
    void initFromData(const unsigned char* data, unsigned int datalen,
                      bool isLittleEndian) throw (DescriptorException);
    /**
     * Unpack a DORADE 2-byte integer value
     * @param bytes pointer to the raw data array.
     * @param offset byte offset to the bytes to be unpacked.
     * @param isLittleEndian true iff the bytes to be unpacked are in
     *      little-endian order.
     */
    static short grabShort(const unsigned char* bytes, int offset, 
                           bool isLlittleEndian);
    /**
     * Unpack a DORADE 4-byte integer value
     * @param bytes pointer to the raw data array.
     * @param offset byte offset to the bytes to be unpacked.
     * @param isLittleEndian true iff the bytes to be unpacked are in
     *      little-endian order.
     */
    static int grabInt(const unsigned char* bytes, int offset, 
                       bool isLlittleEndian);
    /**
     * Unpack a DORADE 4-byte floating point value
     * @param bytes pointer to the raw data array.
     * @param offset byte offset to the bytes to be unpacked.
     * @param isLittleEndian true iff the bytes to be unpacked are in
     *      little-endian order.
     */
    static float grabFloat(const unsigned char* bytes, int offset, 
                           bool isLittleEndian);
    /**
     * Unpack a DORADE 8-byte floating point value
     * @param bytes pointer to the raw data array.
     * @param offset byte offset to the bytes to be unpacked.
     * @param isLittleEndian true iff the bytes to be unpacked are in
     *      little-endian order.
     */
    static double grabDouble(const unsigned char* bytes, int offset, 
                             bool isLittleEndian);
    
    /**
     * Pack a short into a 2-byte DORADE representation.
     * @param os the destination output stream.
     * @param value the value to be written.
     * @param asLittleEndian true iff the value should be written in
     *      little-endian order.
     */
    static std::ostream& putShort(std::ostream& os, short value, 
                                  bool asLittleEndian);
    
    /**
     * Pack an int into a 4-byte DORADE representation.
     * @param os the destination output stream.
     * @param value the value to be written.
     * @param asLittleEndian true iff the value should be written in
     *      little-endian order.
     */
    static std::ostream& putInt(std::ostream& os, int value, 
                                bool asLittleEndian);

    /**
     * Pack a float into a 4-byte DORADE representation.
     * @param os the destination output stream.
     * @param value the value to be written.
     * @param asLittleEndian true iff the value should be written in
     *      little-endian order.
     */
    static std::ostream& putFloat(std::ostream& os, float value, 
                                  bool asLittleEndian);

    /**
     * Pack a double into a 8-byte DORADE representation.
     * @param os the destination output stream.
     * @param value the value to be written.
     * @param asLittleEndian true iff the value should be written in
     *      little-endian order.
     */
    static std::ostream& putDouble(std::ostream& os, double value, 
                                   bool asLittleEndian);

    /**
     * Pack a series of bytes to the given stream.
     * @param os the destination output stream.
     * @param bytes the bytes to be packed.
     * @param count the number of bytes to be packed.
     * @param swap true if the order of the bytes should be swapped when 
     *      writing.
     */
    static std::ostream& putBytes(std::ostream& os, const char* bytes, 
                                  unsigned int count, bool swap);
    
    std::string _descName;
    unsigned int _descLen;
    bool _verbose; 
};

#endif /*DORADEDESCRIPTOR_H_*/
