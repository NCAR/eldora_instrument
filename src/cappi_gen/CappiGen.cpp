#include <iostream>

#include "CappiGen.h"
#include "math.h"

#define flteq(val1, val2, tol) (fabs(val1-val2) <= tol)

CappiGen::CappiGen(std::string storageDir, std::string nc_template)
  : _storageDir(storageDir),_nc_template(nc_template), _nc_output(NULL),
    _recordCount(0), _maxCells(0)
{
  // list the variables to be copied to the netCDF output file 

  // NOTE: this list of variable names must be a subset of the names defined in
  // ../ddsqt/EldoraQtProductsSource.cpp
  // and must be the same names used in cappi.cdl
  _hskpVarNames.push_back("lon");
  _hskpVarNames.push_back("lat");
  _hskpVarNames.push_back("corRotAngle");
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

  _extraNames.push_back("product");
  _extraNames.push_back("prodType");
  //  _extraNames.push_back("time");  // we'll add this later, if needed
  _extraNames.push_back("unixTime");
  _extraNames.push_back("microsec");
  _extraNames.push_back("numCells");

}
CappiGen::~CappiGen()
{
  delete _nc_output; // causes implicit close()
}

int CappiGen::openOutput(std::string &oname)
{
  _recordCount = 0;
  std::string output = _storageDir + "/" + oname;  //XXX add date string
  std::string cmd = "cp "+ _nc_template + " " + output;
  int rc = system(cmd.c_str());
  if (rc != 0) {
    std::cerr << "couldn't create " << output << " from " << _nc_template 
	 << std::endl;
    return(1);
  }
  _nc_output = new NcFile(output.c_str(), NcFile::Write);
  if (! _nc_output->is_valid()) {
    std::cerr << "couldn't write " << output << std::endl;
    return(1);
  }
  StringVecIter si;
  NcVar *var; 
  for (si = _hskpVarNames.begin(); si != _hskpVarNames.end(); ++si){
    const char *varName = (*si).c_str();
    std::cerr << "locating netCDF variable: " << varName << std::endl;
    var = _nc_output->get_var(varName);
    if (!var) {
      std::cerr << "can not locate " << varName << " in " << output;
      return 1;
    } 
    _ncVarMap [*si] = var;
  }
  for (si = _extraNames.begin(); si != _extraNames.end(); ++si){
    const char *varName = (*si).c_str();
    std::cerr << "locating netCDF variable: " << varName << std::endl;
    var = _nc_output->get_var(varName);
    if (!var) {
      std::cerr << "can not locate " << varName << " in " << output;
      return 1;
    } 
    _ncVarMap [*si] = var;
  }
  // diagnostic - what are the dimensions?
  NcVar *product = _ncVarMap["product"];
  int num_dims = product->num_dims();
  for (int i = 0; i < num_dims; ++i) {
    NcDim *dim = product->get_dim(i);
    std::cerr << "dim name = " << dim->name() << " size = " << dim->size() 
	      << std::endl;
  }
  _maxCells = product->get_dim(1)->size();
  return 0;
}

void CappiGen::productSlot(
        std::vector<double> p, 
        int radarId, 
        int prodType, 
	qlonglong timetag,
	StrMapDouble hskpMap)
{
  bool ok;

  double rotAngle = hskpMap["corRotAngle"];
  // only process horizontal beams 

  if (flteq(rotAngle, 90.0, 1) || flteq(rotAngle, 270.0, 1.0)) {
    int rc = 0;
    if (!_nc_output) {
      std::string output = "cappi.nc"; // XXX add date string to name
      rc = openOutput(output); 
      if (rc != 0) {
	std::cerr << "openOutput() failed" << std::endl;
	exit(1);
      }

    }

    NcVar *prodTypeVar = _ncVarMap["prodType"];
    ok = prodTypeVar->put(&prodType, _recordCount);
    if (!ok) {
      std::cerr << "put(" << "prodType" << ") failed" << std::endl;
      exit(1);
    }
    const int numCells = std::min(p.size(), (size_t) _maxCells);
    NcVar *numCellsVar = _ncVarMap["numCells"];
    ok = numCellsVar->put(&numCells, _recordCount);
    if (!ok) {
      std::cerr << "put(" << "numCells" << ") failed" << std::endl;
      exit(1);
    }

    // copy the product into an array, and write to output file
    double product[_maxCells];
    std::cerr << "copying " << numCells << " gates" << std::endl;
    int i;
    for (i = 0; i < numCells; ++i) {
      product[i] = p[i];
    }

    NcVar *pVar = _ncVarMap["product"];
    if (!pVar) {
      std::cerr << "product NcVar is NULL" << std::endl;
      exit(1);
    }
    ok = pVar->put_rec(product,_recordCount);
    if (!ok) {
      std::cerr << "put_rec(" << "product" << ") failed" << std::endl;
      exit(1);
    }

    // time handling
    long unixTime, microsec;
    unixTime = timetag / 1000000;
    microsec = timetag % 1000000;
    NcVar *unixTimeVar = _ncVarMap["unixTime"];
    NcVar *msTimeVar = _ncVarMap["microsec"];

    ok = unixTimeVar->put(&unixTime,_recordCount);
    if (!ok) {
      std::cerr << "put(" << "unixTime" << ") failed" << std::endl;
      exit(1);
    }
    ok = msTimeVar->put(&microsec,_recordCount);
    if (!ok) {
      std::cerr << "put(" << "unixTime" << ") failed" << std::endl;
      exit(1);
    }

    StringVecIter si;
    double value;

    for (si = _hskpVarNames.begin(); si != _hskpVarNames.end(); ++si){
      std::string varName = (*si);
      value = hskpMap[varName];
      ok = _ncVarMap[varName]->put(&value,_recordCount);
      if (!ok) {
	std::cerr << "put(" << *si << ") failed" << std::endl;
	exit(1);
      }
    }
    ++_recordCount;
    // any readers want to see this record now
    _nc_output->sync();

    std::cerr << "radarId = " << radarId << ",rotAngle = " << rotAngle << std::endl;
  }
}
