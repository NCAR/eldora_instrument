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


class CappiGen : public QObject {
    Q_OBJECT
public:
    CappiGen(std::string storageDir);
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
                std::vector<double> p, int radarId, float elDegrees,
                int prodType, float gateSizeMeters, double dwellWidth, 
                double airspdCorr, double rollAngle, 
		qlonglong timetag,
		double longitude,
		double latitude,
		double elevation,
		double heading,
		double roll,
		double pitch,
		double groundSpeedEW,
		double groundSpeedNS);

  
private:
  std::string _storageDir;

};

#endif // CAPPIGEN_H
