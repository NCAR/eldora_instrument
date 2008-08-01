#ifndef ELDORATYPES_H
#define ELDORATYPES_H

#include <vector>
#include <map>

typedef std::vector<std::string> StringVec;
typedef std::vector<double> DoubleVec;
typedef std::vector<std::string>::iterator StringVecIter;
typedef std::map<std::string, double> StrMapDouble;
typedef std::map<std::string, double>::iterator StrMapDoubleIter;

#include "netcdfcpp.h"
typedef std::map<std::string,NcVar *> NcVarMap;
#endif // ELDORATYPES_H
