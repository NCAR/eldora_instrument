#ifndef DORADESITU_H_
#define DORADESITU_H_

#include "DoradeDescriptor.h"

/**
 * <p>Title: DoradeSITU</p>
 * <p>Description: DORADE in-situ parameters (SITU)</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Id$
 */

class DoradeSITU : public DoradeDescriptor {
public:
    DoradeSITU(const unsigned char *data, unsigned int datalen, 
               bool isLittleEndian) throw (DescriptorException);
    
    DoradeSITU(int nParams, std::string* names, std::string* units);
    
    // accessor methods
    // @TODO add accessor methods
    
    std::ostream& printTo(std::ostream& stream) const;
    std::ostream& streamTo(std::ostream& os, bool asLittleEndian);
private:
    int  _nParams;          /// number of parameters
    struct situ_param {
        std::string _name;  /// param name, 8 characters
        std::string _units; /// param units, 8 characters
    } _params[256];
};
#endif /*DORADESITU_H_*/
