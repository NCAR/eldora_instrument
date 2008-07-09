#ifndef DORADEVOLD_H_
#define DORADEVOLD_H_

#include <boost/date_time/posix_time/posix_time.hpp>
#include "DoradeDescriptor.h"

/**
 * <p>Title: DoradeVOLD</p>
 * <p>Description: DORADE volume descriptor</p>
 * <p>Copyright: Copyright (c) 2008</p>
 * <p>Company: University Corporation for Atmospheric Research</p>
 * @author Chris Burghart
 * @version $Revision: 1.3 $ $Date: 2004/04/13 17:05:50 $
 */

class DoradeVOLD : public DoradeDescriptor {
public:
    DoradeVOLD(const unsigned char *data, unsigned int datalen, 
               bool isLittleEndian) throw (DescriptorException);
    
    DoradeVOLD(short formatVersionNum, short volumeNum, int maxDataRecLen,
        std::string projectName, boost::posix_time::ptime volumeDateTime,
        std::string flightOrIOP, std::string generatingFacility,
        boost::gregorian::date genDate, short sensorCount);
    
    // accessors
    short getFormatVersionNum() { return _formatVersionNum; }
    short getVolumeNum() { return _volumeNum; }
    int getMaxDataRecLen() { return _maxDataRecLen; }
    std::string getProjectName() { return _projectName; }
    boost::posix_time::ptime getVolumeDateTime() { return _volumeDateTime; }
    std::string getFlightOrIOP() { return _flightOrIOP; }
    std::string getGeneratingFacility() { return _generatingFacility; }
    boost::gregorian::date getGenerationDate() { 
        return boost::gregorian::date(_genYear, _genMonth, _genDay);
    }
    short getSensorCount() { return _sensorCount; }
    
    // mutators
    /**
     * Set volume start date/time.
     * @param time posix_time holding the volume's start date/time.
     */
    void setVolumeDateTime(boost::posix_time::ptime time);
    /**
     * Set generation date.
     * @param genDate boost::gregorian::date holding the generation date.
     */
    void setGenerationDate(boost::gregorian::date genDate);

    std::ostream& printTo(std::ostream& stream) const;
    std::ostream& streamTo(std::ostream& os, bool asLittleEndian);

private:
    short _formatVersionNum;
    short _volumeNum;
    int _maxDataRecLen;
    std::string _projectName;   // 20 characters
    // the next six elements hold the volume start time
    short _year;
    short _month;
    short _day;
    short _hour;
    short _minute;
    short _second;
    std::string _flightOrIOP;           // 8 characters
    std::string _generatingFacility;    // 8 characters
    // the next three elements hold the generation date of the volume
    short _genYear;
    short _genMonth;
    short _genDay;
    short _sensorCount;

    // This ptime is constructed as a convenience from the volume start time
    // elements above.  It is not part of the external representation of a
    // VOLD.
    boost::posix_time::ptime _volumeDateTime;
};
#endif /*DORADEVOLD_H_*/
