#ifndef CAPPI_READ_H
#define CAPPI_READ_H
// Symbolic names for product types
#include "ProductTypes.h"

#include "EldoraTypes.h"

#include <string>

class CappiReader {
 public:
    CappiReader();

    ~CappiReader();

    bool openFile(std::string inputFile);
    void closeFile();
    
    bool read(unsigned long index, std::vector<double> &p, int &prodType, long long &timeTag, StrMapDouble &hskpMap);

    bool findLastRecord(unsigned long &index);
    
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
