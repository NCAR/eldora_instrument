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

    virtual std::ostream& printTo(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& out, const DoradeDescriptor&);

protected:
    void initFromData(const unsigned char* data, unsigned int datalen,
                      bool isLittleEndian) throw (DescriptorException);

    static short grabShort(const unsigned char* bytes, int offset, 
                           bool isLlittleEndian);
    static int grabInt(const unsigned char* bytes, int offset, 
                       bool isLlittleEndian);
    static float grabFloat(const unsigned char* bytes, int offset, 
                           bool isLittleEndian);
    static double grabDouble(const unsigned char* bytes, int offset, 
                             bool isLittleEndian);

    std::string _descName;
    unsigned int _descLen;
    bool _verbose; 
};

#endif /*DORADEDESCRIPTOR_H_*/
