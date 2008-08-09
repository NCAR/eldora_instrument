#include <iostream>
#include <cstdlib>

#include <boost/filesystem/operations.hpp>
namespace fs = boost::filesystem;


#include "CappiGen.h"
#include "math.h"

#define flteq(val1, val2, tol) (fabs(val1-val2) <= tol)

CappiGen::CappiGen(std::string outputFilename, std::string nc_template, double angleTol)
  : _outputFilename(outputFilename),_nc_template(nc_template), _nc_output(NULL),
    _recordCount(0), _maxCells(0), _angleTol(angleTol),_totalCount(0),
    _unixTimeVar(NULL), _msTimeVar(NULL), _prodTypeVar(NULL)
{
  // list the variables to be copied to the netCDF output file 

  // NOTE: this list of variable names must be a subset of the names defined in
  // ../ddsqt/EldoraQtProductsSource.cpp
  // and must be the same names used in cappi.cdl
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

int CappiGen::openOutput(std::string &output)
{
  fs::path path(output);
  
  // if the file doesn't already exist, create it from the template
  if (! fs::exists(path) ) {
  
      std::string cmd = "cp "+ _nc_template + " " + output;
      int rc = system(cmd.c_str());
      if (rc != 0) {
          std::cerr << "couldn't create " << output << " from " << _nc_template 
                    << std::endl;
          return(1);
      }
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
    //    std::cerr << "locating netCDF variable: " << varName << std::endl;
    var = _nc_output->get_var(varName);
    if (!var) {
        std::cerr << "can not locate " << varName << " in " << output << std::endl;
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
  std::cerr << "Products dimension: ";
  for (int i = 0; i < num_dims; ++i) {
    NcDim *dim = product->get_dim(i);
    std::cerr << "dim name = " << dim->name() << " size = " << dim->size() 
	      << std::endl;
  }
  _maxCells = product->get_dim(1)->size();
  // add new records after the current one
  _recordCount =  product->get_dim(0)->size();
  _unixTimeVar = _ncVarMap["unixTime"];
  _msTimeVar = _ncVarMap["microsec"];
  _prodTypeVar = _ncVarMap["prodType"];
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

  double corRotAngle = hskpMap["rotAngle"] + hskpMap["roll"];
  // only process horizontal beams 

  // we record all beams that are "close enough" to horizontal, so we can
  // apply a manual rotational correction later, and select the beams we
  // really want.
  
  ++_totalCount;  // how many beams have we received?
  
  if (flteq(corRotAngle, 90.0, _angleTol) || flteq(corRotAngle, 270.0, _angleTol)) {
    int rc = 0;
    if (!_nc_output) {
      rc = openOutput(_outputFilename); 
      if (rc != 0) {
	std::cerr << "openOutput() failed" << std::endl;
	exit(1);
      }

    }
    long count=1;

    ok = _prodTypeVar->set_cur(_recordCount);
    if (!ok) {
	std::cerr << "set_cur(" << "prodType" << ") failed" << std::endl;
	exit(1);
    }
    ok = _prodTypeVar->put(&prodType, count);
    if (!ok) {
      std::cerr << "put(" << "prodType" << ") failed" << std::endl;
      exit(1);
    }
    const int numCells = std::min(p.size(), (size_t) _maxCells);
    NcVar *numCellsVar = _ncVarMap["numCells"];

    ok = numCellsVar->set_cur(_recordCount);
    if (!ok) {
	std::cerr << "set_cur(" << "numCells" << ") failed" << std::endl;
	exit(1);
    }
    ok = numCellsVar->put(&numCells, count);
    if (!ok) {
      std::cerr << "put(" << "numCells" << ") failed" << std::endl;
      exit(1);
    }

    // copy the product into an array, and write to output file
    float product[_maxCells];
    //    std::cerr << "copying " << numCells << " gates" << std::endl;
    int i;
    for (i = 0; i < numCells; ++i) {
      product[i] = p[i];
    }

    NcVar *pVar = _ncVarMap["product"];
    if (!pVar) {
      std::cerr << "product NcVar is NULL" << std::endl;
      exit(1);
    }
    ok = pVar->put_rec(&product[0],_recordCount);
    if (!ok) {
      std::cerr << "put_rec(" << "product" << ") failed" << std::endl;
      exit(1);
    }

    // time handling
    long unixTime, microsec;
    unixTime = timetag / 1000000;
    microsec = timetag % 1000000;

    ok = _unixTimeVar->set_cur(_recordCount);
    if (!ok) {
	std::cerr << "set_cur(" << "unixTime" << ") failed" << std::endl;
	exit(1);
    }

    ok = _unixTimeVar->put(&unixTime,count);
    if (!ok) {
      std::cerr << "put(" << "unixTime" << ") failed" << std::endl;
      exit(1);
    }
    ok = _msTimeVar->set_cur(_recordCount);
    if (!ok) {
	std::cerr << "set_cur(" << "microsec" << ") failed" << std::endl;
	exit(1);
    }
    ok = _msTimeVar->put(&microsec,count);
    if (!ok) {
      std::cerr << "put(" << "microsec" << ") failed" << std::endl;
      exit(1);
    }

    StringVecIter si;
    double value;
    
    std::cerr << "CappiGen::productSlot\n";
    for (si = _hskpVarNames.begin(); si != _hskpVarNames.end(); ++si){
      std::string varName = (*si);
      value = hskpMap[varName];
      //      std::cerr << varName << "=> " << value << std::endl;
      NcVar *var = _ncVarMap[varName];
      ok = var->set_cur(_recordCount);
      if (!ok) {
	std::cerr << "set_cur(" << *si << ") failed" << std::endl;
	exit(1);
      }
      
      ok = _ncVarMap[varName]->put(&value,count);
      if (!ok) {
	std::cerr << "put(" << *si << ") failed" << std::endl;
	exit(1);
      }
    }
    std::cerr << "Record: " << _recordCount << " ,corRot = " <<corRotAngle <<
        " ,groundSpeedEW = " << hskpMap["groundSpeedEW"] <<std::endl;
    
    ++_recordCount;
    // any readers want to see these records on a regular basis
    _nc_output->sync();
    if ((_recordCount % 100) == 1) {
        std::cerr << "Output " << _recordCount << " records\n";
    }
   

    //    std::cerr << "radarId = " << radarId << ",corRotAngle = " << corRotAngle << std::endl;
  } // end if corrected rotation angle is close enough
  if ((_totalCount % 100) == 1) {
        std::cerr << "received " << _totalCount << " total records\n";
  }
  
}
