#ifndef PPIINFO_H_
#define PPIINFO_H_

#include <string>

/// Keep track of the configuration information for each 
/// product type.
class PpiInfo {
public:
    PpiInfo();
    PpiInfo(int id, 
        std::string key, 
        std::string shortName, 
        std::string longName,
        std::string colorMapName,
        double scaleMin, 
        double scaleMax
        );
    virtual ~PpiInfo();

    int getId();
    std::string getKey();
    void setScale(double min, double max);
    void setColorMapName(std::string mapName);
    std::string getColorMapName();
    double getScaleMin();
    double getScaleMax();
    std::string getShortName();
    std::string getLongName();


protected:
    int _id;
    std::string _key;
    std::string _shortName;
    std::string _longName;
    double _scaleMin;
    double _scaleMax;
    std::string _colorMapName;
};

#endif /*PPIINFO_H_*/
