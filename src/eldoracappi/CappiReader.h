#ifndef CAPPI_READ_H
#define CAPPI_READ_H
// Symbolic names for product types
#include "ProductTypes.h"

#include "EldoraTypes.h"
#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
using boost::posix_time::ptime;

class CappiReader {
 public:
    CappiReader();

    ~CappiReader();

    bool openFile(std::string inputFile);
    
    void closeFile();
    
    bool read(unsigned long index, std::vector<double> &p, int &prodType, ptime &timeTag, StrMapDouble &hskpMap);

    bool findLastRecord(unsigned long &index);
    
    /// get the time stamp for the specified record number
    /// @param rec The record number.
    /// @param timeTag The time stamp is returned here.
    /// @return True if a timetag is availabel for this record, false otherwise.
    bool getTime(unsigned long rec, ptime& timeTag);
    
private:
    NcFile *_nc_input;
    NcVarMap _ncVarMap;
    
    unsigned long _curRecordCount;
    StringVec _hskpVarNames;
    StringVec _otherVarNames;
    unsigned int _maxCells;
    std::string _fileName;

};



#endif // CAPPPI_READ_H
