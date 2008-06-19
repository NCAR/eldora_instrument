#ifndef DORADENDDS_H_
#define DORADENDDS_H_

#include "DoradeDescriptor.h"

/**
 * <p>Title: DoradeNDDS</p>
 * <p>Description: DORADE Navigational Data Descriptor (NDDS)</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Id$
 */

class DoradeNDDS : public DoradeDescriptor {
public:
    DoradeNDDS(const unsigned char *data, unsigned int datalen, 
               bool isLittleEndian) throw (DescriptorException);
    
    DoradeNDDS(short insFlag, short gpsFlag, short minirimsFlag, 
        short kalmanFlag);
    
    // accessor methods
    // @TODO add accessor methods
    
    std::ostream& printTo(std::ostream& stream) const;
    std::ostream& streamTo(std::ostream& os, bool asLittleEndian);
private:
    short _insFlag;         /// 0 = no INS data, 1 = data recorded
    short _gpsFlag;         /// 0 = no GPS data, 1 = data recorded
    short _minirimsFlag;    /// 0 = no MiniRIMS data, 1 = data recorded
    short _kalmanFlag;      /// 0 = no kalman data, 1 = data recorded
};
#endif /*DORADENDDS_H_*/
