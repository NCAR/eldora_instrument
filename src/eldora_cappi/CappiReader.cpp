#include "CappiReader.h"

CappiReader::CappiReader():
        _nc_input(NULL)
{
    _hskpVarNames.push_back("lon");
    _hskpVarNames.push_back("lat");
    _hskpVarNames.push_back("rotAngle");
    _hskpVarNames.push_back("gateSpacingMeters");
    _hskpVarNames.push_back("dwellWidth");
    _hskpVarNames.push_back("elevation");
    _hskpVarNames.push_back("heading");
    _hskpVarNames.push_back("roll");
    _hskpVarNames.push_back("pitch");
    _hskpVarNames.push_back("yaw");
    _hskpVarNames.push_back("groundSpeedEW");
    _hskpVarNames.push_back("groundSpeedNS");
    _hskpVarNames.push_back("airSpdCorr");

    _otherVarNames.push_back("product");
    _otherVarNames.push_back("prodType");
    _otherVarNames.push_back("unixTime");
    _otherVarNames.push_back("microsec");
    _otherVarNames.push_back("numCells");
    
}

CappiReader::~CappiReader()
{
    closeFile();
    
}
void CappiReader::closeFile()
{
    if (_nc_input) {
        delete _nc_input;
        _ncVarMap.clear();
        _nc_input = NULL;
    }
}


bool CappiReader::openFile(std::string inputFile)
{
    _fileName = inputFile; // remember the name, we'll need to re-open the
                           // file
    closeFile();  // in case it is already open
    
    _nc_input = new NcFile(inputFile.c_str(), NcFile::ReadOnly);
    if (! _nc_input->is_valid()){
        std::cerr << "could not open " << inputFile << std::endl;
        return false;
    }
    StringVecIter si;
    NcVar *var;
    for (si = _hskpVarNames.begin(); si != _hskpVarNames.end(); ++si){
        const char *varName = (*si).c_str();
        //        std::cerr << "locating netCDF variable: " << varName << std::endl;
        var = _nc_input->get_var(varName);
        if (!var) {
            std::cerr << "can not locate " << varName << " in " << inputFile << std::endl;
            return false;
        } 
        _ncVarMap [*si] = var;
    }

    for (si = _otherVarNames.begin(); si != _otherVarNames.end(); ++si){
        const char *varName = (*si).c_str();
        //        std::cerr << "locating netCDF variable: " << varName << std::endl;
        var = _nc_input->get_var(varName);
        if (!var) {
            std::cerr << "can not locate " << varName << " in " << inputFile << std::endl;
            return false;
        } 
        _ncVarMap [*si] = var;
    }

    NcVar *product = _ncVarMap["product"];
    _maxCells =product->get_dim(1)->size();

    // what's the largest record index?
    _curRecordCount = product->get_dim(0)->size();
    
    return true;
   
}
bool CappiReader::read(unsigned long index, std::vector<double> &p, int &prodType, long long &timeTag, StrMapDouble &hskpMap)
{
    
    if (index >= _curRecordCount) {
        return false;
    }
    StringVecIter si;
    double value;
    bool ok;
    long count=1;

    NcVar *prodTypeVar = _ncVarMap["prodType"];
    ok = prodTypeVar->set_cur(index);
    if (!ok) {
	std::cerr << "set_cur(" << "prodType" << ") failed" << std::endl;
	exit(1);
    }
    ok = prodTypeVar->get(&prodType, count);
    if (!ok) {
      std::cerr << "put(" << "prodType" << ") failed" << std::endl;
      exit(1);
    }
    NcVar *numCellsVar = _ncVarMap["numCells"];

    int numCells ;
    ok = numCellsVar->set_cur(index);
    if (!ok) {
	std::cerr << "set_cur(" << "numCells" << ") failed" << std::endl;
	exit(1);
    }
    ok = numCellsVar->get(&numCells, count);
    if (!ok) {
      std::cerr << "put(" << "numCells" << ") failed" << std::endl;
      exit(1);
    }

    // time handling
    long unixTime, microsec;
    NcVar *unixTimeVar = _ncVarMap["unixTime"];
    NcVar *msTimeVar = _ncVarMap["microsec"];

    ok = unixTimeVar->set_cur(index);
    if (!ok) {
	std::cerr << "set_cur(" << "unixTime" << ") failed" << std::endl;
	exit(1);
    }

    ok = unixTimeVar->get(&unixTime,count);
    if (!ok) {
      std::cerr << "put(" << "unixTime" << ") failed" << std::endl;
      exit(1);
    }
    ok = msTimeVar->set_cur(index);
    if (!ok) {
	std::cerr << "set_cur(" << "microsec" << ") failed" << std::endl;
	exit(1);
    }
    ok = msTimeVar->get(&microsec,count);
    if (!ok) {
      std::cerr << "put(" << "microsec" << ") failed" << std::endl;
      exit(1);
    }
    timeTag = unixTime *  100000 + microsec;
    // retrieve all housekeeping values and place into hskpMap
    for (si = _hskpVarNames.begin(); si != _hskpVarNames.end(); ++si){
      std::string varName = (*si);
      NcVar *var = _ncVarMap[varName];
      ok = var->set_cur(index);
      if (!ok) {
	std::cerr << "set_cur(" << *si << ") failed" << std::endl;
        return false;
	// exit(1);
      }
      
      ok = _ncVarMap[varName]->get(&value,count);
      if (!ok) {
	std::cerr << "put(" << *si << ") failed" << std::endl;
        return false;
	// exit(1);
      }
      hskpMap[varName] = value;
      //      std::cerr << varName << "=> " << value << std::endl;
    }
    // finally, retrieve the product itself
    NcVar *pVar = _ncVarMap["product"];
    if (!pVar) {
      std::cerr << "product NcVar is NULL" << std::endl;
      exit(1);
    }
    NcValues_double *ncd = (NcValues_double *)pVar->get_rec(index);
    if (!ncd) {
      std::cerr << "get_rec(" << "product" << ") failed" << std::endl;
      exit(1);
    }
        
    int i;
    p.clear();  // start empty
    for (i = 0; i < numCells; i++){
        p.push_back(ncd->as_double(i));
    }
    std::cerr << "CappiReader::read() time ="
              << timeTag << " prodType = " <<prodType  << " p[0] =" << p[0] <<
        std::endl;
    return true;
}

// reopen the file, and determine which is the last record

bool CappiReader::findLastRecord(unsigned long &index)
{
    bool ok = openFile(_fileName);
    
    if (!ok) return false;
    index = _curRecordCount-1;
    return true;
    
}



