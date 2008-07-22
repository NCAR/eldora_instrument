#ifndef ELDORATYPES_H
#define ELDORATYPES_H

typedef std::vector<std::string> StringVec;
typedef std::vector<std::string>::iterator StringVecIter;
typedef std::map<std::string, double> StrMapDouble;

#include "netcdf-3/netcdfcpp.h"
typedef std::map<std::string,NcVar *> NcVarMap;
#endif // ELDORATYPES_H
