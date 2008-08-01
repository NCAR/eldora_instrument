#ifndef CAPPIGEN_H
#define CAPPIGEN_H
#include <QObject>
#include <deque>
#include <map>
#include <set>
#include <vector>
#include <string>

// Symbolic names for product types
#include "ProductTypes.h"
#include "EldoraTypes.h"


class CappiGen : public QObject {
    Q_OBJECT
public:
    CappiGen(std::string storageDir, std::string nc_template, double angleTol);
  virtual ~CappiGen();
    public slots:
        /// Feed product data via this slot. If the operating 
        /// characteristics change, such as the number of gates,
        /// or the gate spacing, the CAPPI product will be computed
        /// with the new parameters
        /// @param p The product data.
        /// @param radarId Either EldoraDDS::Fore or EldoraDDS::Aft
        /// @param elDegrees The antenna pointing elevation, degrees
        /// @param prodType The product type, from PRODUCT_TYPES
        /// @param gateSizeMeters The width of each range gates. Needed to 
        /// compute the area covered by each gate
    	/// @param dwellWidth The angular width of one dwell.
        /// @param airspdCorr The airspeed correction that can be added to the 
        /// radial velocity if desired.
        /// @param rollAngle The aircraft roll angle.

        void productSlot(
                std::vector<double> p, 
		int radarId, 
                int prodType, 
		qlonglong timetag,
		StrMapDouble hskpMap);
  
private:
  int openOutput(std::string &name);
  std::string _storageDir;
  std::string _nc_template;
  NcFile *_nc_output;
  NcVarMap _ncVarMap;
  StringVec _hskpVarNames; // variables we need from hskpMap, passed in hskpMap
  StringVec _extraNames;   // other variables we need to access in the netCDF file
  unsigned long _recordCount;
  unsigned long _maxCells;
  double _angleTol; // angle tolerance for recording a particular beam
                    // (is this angle close enough to horizontal?)
  unsigned long _totalCount;
  NcVar *_unixTimeVar;
  NcVar *_msTimeVar;
  NcVar *_prodTypeVar;


};

#endif // CAPPIGEN_H
