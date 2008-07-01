#ifndef DORADERAWD_H_
#define DORADERAWD_H_

#include "DoradeDescriptor.h"

/**
 * <p>Title: DoradeRAWD</p>
 * <p>Description: DORADE Raw Data Descriptor (RAWD)</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Revision: 1.3 $ $Date: 2004/04/13 17:05:50 $
 */

class DoradeRAWD : public DoradeDescriptor {
public:
    DoradeRAWD(const unsigned char *data, unsigned int datalen, 
               bool isLittleEndian) throw (DescriptorException);
    
    DoradeRAWD(std::string xLabel, float xMax, float xMin, 
               std::string yLabel, float yMax, float yMin, int numPoints,
               const char* data);
    
    ~DoradeRAWD();
    
    // accessor methods
    // @TODO add accessor methods
    
    std::ostream& printTo(std::ostream& stream) const;
    std::ostream& streamTo(std::ostream& os, bool asLittleEndian);
private:
    std::string _xLabel;        /// x label, 8 characters
    float _xMax;                /// x max value
    float _xMin;                /// x min value
    std::string _yLabel;        /// y label, 8 characters
    float _yMax;                /// y max value
    float _yMin;                /// y min value
    int _numPoints;             /// number of points
    char* _data;                /// structure unknown, but size is 32 * _numPoints     
};
#endif /*DORADERAWD_H_*/
