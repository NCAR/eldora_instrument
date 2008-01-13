#include "PpiInfo.h"

#include "PpiInfo.h"

////////////////////////////////////////////////////////
PpiInfo::PpiInfo():
_key(""),
_id(0),
_shortName(""),
_longName(""),
_scaleMin(1),
_scaleMax(10)
{
}

////////////////////////////////////////////////////////
PpiInfo::PpiInfo(int id, 
                 int userData,
                 std::string key, 
                 std::string shortName, 
                 std::string longName,
                 std::string colorMapName,
                 double scaleMin, 
                 double scaleMax):
_id(id),
_userData(userData),
_key(key),
_shortName(shortName),
_longName(longName),
_colorMapName(colorMapName),
_scaleMin(scaleMin),
_scaleMax(scaleMax)
{
}

////////////////////////////////////////////////////////

PpiInfo::~PpiInfo()
{
}

////////////////////////////////////////////////////////
void
PpiInfo::setScale(double min, double max)
{
    _scaleMin = min;
    _scaleMax = max;
}

////////////////////////////////////////////////////////
double 
PpiInfo::getScaleMin()
{
    return _scaleMin;
}

////////////////////////////////////////////////////////
double 
PpiInfo::getScaleMax()
{
    return _scaleMax;
}

////////////////////////////////////////////////////////
std::string 
PpiInfo::getShortName()
{
    return _shortName;
}

////////////////////////////////////////////////////////
std::string 
PpiInfo::getLongName()
{
    return _longName;
}
////////////////////////////////////////////////////////
int 
PpiInfo::getId()
{
    return _id;
}
////////////////////////////////////////////////////////
int 
PpiInfo::getUserData()
{
    return _userData;
}
////////////////////////////////////////////////////////
void 
PpiInfo::setColorMapName(std::string mapName)
{
    _colorMapName = mapName;
}

////////////////////////////////////////////////////////
std::string 
PpiInfo::getColorMapName()
{
    return _colorMapName;
}

////////////////////////////////////////////////////////
std::string 
PpiInfo::getKey()
{
    return _key;
}
