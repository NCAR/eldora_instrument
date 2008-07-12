#include <sstream>
#include <iomanip>
#include <cassert>
#include "DoradeSITU.h"

/**
 * <p>Title: DoradeSITU</p>
 * <p>Description: DORADE in-situ parameters (SITU)</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Id$
 */
DoradeSITU::DoradeSITU(const unsigned char *data, unsigned int datalen, 
                       bool isLittleEndian) throw (DescriptorException) :
    DoradeDescriptor(data, datalen, isLittleEndian, "SITU", 4108) { 
    //
    // unpack
    //
    _nParams = grabInt(data, 8, isLittleEndian);
    for (int p = 0; p < _nParams; p++) {
        // Get exactly 8 chars, then shorten if there's a null somewhere.
        _params[p]._name = std::string((const char*)data + 12 + 16 * p, 8);
        _params[p]._name = std::string(_params[p]._name.c_str());
        // Get exactly 8 chars, then shorten if there's a null somewhere.
        _params[p]._units = std::string((const char*)data + 20 + 16 * p, 8);
        _params[p]._units = std::string(_params[p]._units.c_str());
    }
    //
    // debugging output
    //
    if (_verbose)
        std::cout << *this;
};

DoradeSITU::DoradeSITU(int nParams, std::string* names, std::string* units) : 
    DoradeDescriptor("SITU", 4108),
    _nParams(nParams) {
    for (int p = 0; p < _nParams; p++) {
        _params[p]._name = names[p];
        // truncate if necessary
        if (_params[p]._name.size() > 8)
            _params[p]._name.resize(8);
        _params[p]._units = units[p];
        // truncate if necessary
        if (_params[p]._units.size() > 8)
            _params[p]._units.resize(8);
    }
    
    if (_verbose)
        std::cout << *this;
}

std::ostream& 
DoradeSITU::printTo(std::ostream& os) const
{
    DoradeDescriptor::printTo(os);
    // @TODO add details here
    if (_verbose) {
        os << "no SITU detail printing yet..." << std::endl;
    }
    return os;
}

std::ostream&
DoradeSITU::streamTo(std::ostream& os, bool asLittleEndian)
{
    putBytes(os, _descName.data(), 4, false);   // no swapping for char data
    putInt(os, _descLen, asLittleEndian);
    putInt(os, _nParams, asLittleEndian);
    
    // fill unused params with spaces, to follow existing convention
    struct situ_param empty;
    empty._name.resize(8, ' ');
    empty._units.resize(8, ' ');

    std::string str;
    for (int p = 0; p < 256; p++) {
        // Write the params from our list up to _nParams, then write empty params
        struct situ_param param = (p < _nParams) ? _params[p] : empty;
        str = param._name;
        str.resize(8);  // force to exactly 8 chars
        putBytes(os, str.data(), 8, false); // no swapping for char data
        
        str = param._units;
        str.resize(8);  // force to exactly 8 chars
        putBytes(os, str.data(), 8, false);// no swapping for char data
    }
    return os;
}
