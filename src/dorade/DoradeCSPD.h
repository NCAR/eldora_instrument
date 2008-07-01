#ifndef DORADECSPD_H_
#define DORADECSPD_H_

#include "DoradeDescriptor.h"

/**
 * <p>Title: DoradeCSPD</p>
 * <p>Description: DORADE Cell SPacing Descriptor (CSPD)</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Revision: 1.3 $ $Date: 2004/04/13 17:05:50 $
 */

class DoradeCSPD : public DoradeDescriptor {
public:
    DoradeCSPD(const unsigned char *data, unsigned int datalen, 
               bool isLittleEndian) throw (DescriptorException);
    
    DoradeCSPD(short numSegments, short distToFirstGate, short spacing[6], 
        short cellsInSegment[6]);
    
    short getNSegments() { return _numSegments; }
    const short* getNCells() { return _cellsInSegment; }
    const short* getCellSpacing() { return _spacing; }
    
    // accessor methods
    // @TODO add accessor methods
    
    std::ostream& printTo(std::ostream& stream) const;
    std::ostream& streamTo(std::ostream& os, bool asLittleEndian);
private:
    short _numSegments;         /// Number of segments that contain contiguous
                                /// cells of equal widths (maximum 6)
    short _distToFirstGate;     /// distance to first gate, m
    short _spacing[6];          /// cell spacing for each segment, m
    short _cellsInSegment[6];   /// number of cells in each segment
};
#endif /*DORADECSPD_H_*/
