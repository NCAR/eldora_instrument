#include <iostream>

#include "CappiGen.h"

CappiGen::CappiGen(std::string storageDir)
: _storageDir(storageDir)
{
}
CappiGen::~CappiGen()
{
}

void CappiGen::productSlot(
        std::vector<double> p, 
        int radarId, 
        float rotAngle, 
        int prodType, 
        float gateSizeMeters,
        double dwellWidth,
        double airspdCorr,
        double rollAngle,
	qlonglong timetag,
	double longitude,
	double latitude,
	double elevation,
	double heading,
	double roll,
	double pitch,
	double groundSpeedEW,
	double groundSpeedNS)
  
{
  std::cerr << "radarId = " << radarId << 
",rotAngle = " << rotAngle << std::endl;
}
