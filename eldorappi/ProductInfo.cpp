#include "PpiInfo.h"

#include "PpiInfo.h"

////////////////////////////////////////////////////////
ProductInfo::PpiInfo():
_key(""),
_id(0),
_shortName(""),
_longName(""),
_scaleMin(1),
_scaleMax(10)
{
}

////////////////////////////////////////////////////////
ProductInfo::PpiInfo(int id, 
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

ProductInfo::~PpiInfo()
{
}

////////////////////////////////////////////////////////
void
ProductInfo::setScale(double min, double max)
{
    _scaleMin = min;
    _scaleMax = max;
}

////////////////////////////////////////////////////////
double 
ProductInfo::getScaleMin()
{
    return _scaleMin;
}

////////////////////////////////////////////////////////
double 
ProductInfo::getScaleMax()
{
    return _scaleMax;
}

////////////////////////////////////////////////////////
std::string 
ProductInfo::getShortName()
{
    return _shortName;
}

////////////////////////////////////////////////////////
std::string 
ProductInfo::getLongName()
{
    return _longName;
}
////////////////////////////////////////////////////////
int 
ProductInfo::getId()
{
    return _id;
}
////////////////////////////////////////////////////////
int 
ProductInfo::getUserData()
{
    return _userData;
}
////////////////////////////////////////////////////////
void 
ProductInfo::setColorMapName(std::string mapName)
{
    _colorMapName = mapName;
}

////////////////////////////////////////////////////////
std::string 
ProductInfo::getColorMapName()
{
    return _colorMapName;
}

////////////////////////////////////////////////////////
std::string 
ProductInfo::getKey()
{
    return _key;
}
