#ifndef DORADERYIB_H_
#define DORADERYIB_H_

#include <QtCore/QDateTime>
#include "DoradeDescriptor.h"

/**
 * <p>Title: DoradeRYIB</p>
 * <p>Description: DORADE ray information block descriptor</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Revision: 1.3 $ $Date: 2004/04/13 17:05:50 $
 */

class DoradeRYIB : public DoradeDescriptor {
public:
    DoradeRYIB(const unsigned char *data, unsigned int datalen, 
               bool isLittleEndian) throw (DescriptorException);
    std::ostream& printTo(std::ostream& stream) const;
    
    QDateTime getRayDateTime() const { return _rayDateTime; }

private:
    int _sweepNumber;
    short _julianDay;
    short _hour;
    short _minute;
    short _second;
    short _millisecond;
    float _azimuth;
    float _elevation;
    float _lastPeakPower;   // kW
    float _scanRate;        // deg/s
    int _rayStatus;         // 0 normal, 1 transition, 2 bad
    
    QDateTime _rayDateTime; /* not directly from the RYIB, but built from it */
};
#endif /*DORADERYIB_H_*/
