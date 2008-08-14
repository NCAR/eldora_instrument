#ifndef ELDORAQTPRODUCTSSOURCE_H_
#define ELDORAQTPRODUCTSSOURCE_H_

#include <vector>
#include <map>

#include "EldoraQtSource.h"
#include "DDSReader.h"
#include "ProductsTypeSupportC.h"
#include "ProductsTypeSupportImpl.h"
#include "ProductTypes.h"
#include "EldoraTypes.h"

using namespace EldoraDDS;

/// A Products DDS subscriber for Eldora data,
/// which can feed P data to Qt clients.
/// It is derived from EldoraQtSource and ProductsReader.
/// When the DDS data notification is received via the notify()
/// method, a Qt signal is emitted in order to deliver the
/// data to other Qt components. Note that this scheme is utilizing 
/// Qt4's capabilty to send signals between threads. See the 
/// Qt4 docuentation on threading and QObjects.
///
/// The EldoraQtSource rate limiting mechanism (the _capture variable)
/// is used to control the rate of data delivery to the qt client. If
/// the rate parameter is set to zero, then there is no rate limiting
/// and all data are delivered.
///
/// The organization of the desired products are specified via the
/// AlongBeam and OneGate slots.These slots specify the desired data 
/// product. Note however that if more than one product is specified
/// in the constructor, then the delivered products are always those
/// specified in the constructor, and will not be changed by these slots.
///
/// Notice that there are some very specific Eldora considerations
/// in this class. In particular, it will extract a few parameters
/// from the housekeeping information, that are needed by downstream 
/// consumers. It even computes some specific quantities, such as
/// the along beam correction for airspeed. These are sent out 
/// with the newPData signal. This is sort of a strange 
/// place to be doing this, but it is a compromise. An alternative
/// would be to send the whole housekeeping payload to the 
/// consumers. Maybe a redesign is called for. In any event, 
/// it is clear that EldoraQtProductsSource is very Eldora specific.
///

class EldoraQtProductsSource : public EldoraQtSource, public ProductsReader {
    Q_OBJECT
    public:
        /// Used to specify which (or both) radars 
        enum RADAR_CHOICE {
            RADAR_FOR, ///< The forward radar
            RADAR_AFT, ///< The aft radar
            RADAR_BOTH ///< Both radars
        };
        /// Constructor
        /// @param subscriber The DDS subscriber
        /// @param topicName  The DDS topic name for PRODUCTS
        /// @param outputRate If greater than zero, limit data delivery rate to outputRate (Hz). 
        /// If outputRate==0, no rate limiting is performed.
        /// @param radarChoice The initial radar choice, used to discrimnate between radars. If 
        /// RADAR_BOTH is specified, then data for both radars are delivered.
        /// @param productChoices The initial product type choices.
        EldoraQtProductsSource(
                DDSSubscriber& subscriber, std::string topicName,
                double outputRate=20.0, RADAR_CHOICE radarChoices=RADAR_FOR,
                std::set<PRODUCT_TYPES> productChoices=std::set<PRODUCT_TYPES>());
        virtual ~EldoraQtProductsSource();
        /// Subclass DDSReader::notify(), which wil be called
        /// whenever new samples are added to the DDSReader available
        /// queue. Process the samples here.
        virtual void notify();

    signals:
        /// This signal is emitted when new Product data are available.
        /// @param P P data values (at least 1).
        /// @param radarId Either EldoraDDS::Fore or EldoraDDS::Aft
        /// @param elDegrees Pointing angle, degrees
        /// @param prodType The product type, from PRODUCT_TYPES
        /// @param gateSpacingMeters The gate spacing in meters
        /// @param dwellWidth The width of the dwell, in degrees
        /// @param airspdCorr The airspeed correction to the radial velocity
    	/// @param rollAngle The aircraft roll angle, in degrees.
        /// @param nyquistVelocity The radar nyquist velocity, m/s. Required for
        /// velocity unfolding along the beam.
        /// @param altitudeMSL Altitude, meters abocw sea level
        /// @param latitude Latitude, degrees
        /// @param longitude Longitude, degrees
        void newPData(
                std::vector<double> P, 
                int radarId, 
                float elDegrees,
                int prodType, 
                float gateSpacingMeters, 
                double dwellWidth, 
                double airspdCorr,
                double rollAngle,
                double nyquistVelocity,
                double altitudeMSL,
                double latitude,
                double longitude);

        /// This signal is emitted when new Product data are available.
        /// @param P P data values (at least 1).
        /// @param radarId Either EldoraDDS::Fore or EldoraDDS::Aft
        /// @param prodType The product type, from PRODUCT_TYPES
    	/// @param timetag  - microsecs since epoch
        /// @param hskpMap - housekeeping in STL map
        void newPDataHskpMap(
                std::vector<double> P, 
                int radarId, 
                int prodType, 
		qlonglong timetag,
		StrMapDouble hskpMap
		);
    public slots:
        /// Set the gate mode to ONE_GATE.
        /// @param product The product type.
        /// @param forwardRadar True if forward radar, false if aft.
        /// @param gate The gate to send in.
        /// @param n The number of points.
        virtual void oneGateSlot(
                PRODUCT_TYPES product, bool forwardRadar, int gate, int n);

        /// Set the gate mode to along beam.
        /// @param product The product type.
        /// @param forwardRadar True if forward radar, false if aft
        virtual void alongBeamSlot(
                PRODUCT_TYPES product, bool forwardRadar);

    protected:
        /// Locate the specified product.
        /// @param prodType The specific product type
        /// @param pItem Pointer to a Products item.
        /// @param data A pointer to the data field selected by _product is returned here.
        /// @param gain The gain factor for the product is returned here.
        /// @param offset The offset for the product is returned here.
        void selectProduct(
                PRODUCT_TYPES prodType, Products* pItem, short** data,
                float& gain, float& offset);

        /// Calculate the groundspeed correction that can added to the radial velocity
        /// in order to remove the aircraft motion. Useful for ppi displays.
        /// Note that the operating mode of the radar is taken into consideration. Since 
        /// the correction is only applied in in dual prt mode, then in single prt
        /// mode the value will be zero.
        /// @param pItem The EldoraDDS::Products containing housekeeping data
        /// used to determine the correction.
        /// @return The correction to be added to the radial (VR) velocity.
        double airSpeedCorrection(Products* pItem);
        
        /// Compute nyquist velocity
        double nyquistVelocity(Products* pItem);
        
        /// Compute the angle swept out by the Product.
        /// This is computed from factors contained in the housekeeping.
        /// The dwell angle is: rotation rate (deg/s) * repeat sequences per dwell 
        ///    * ms per repeat sequence / 1000.0
        /// @param pItem The source of housekeeping data.
        double dwellWidth(Products* pItem);
        /// Do not collect samples any faster than this
        double _outputRate;

        /// The selected radar, either forward or aft
        RADAR_CHOICE _radarChoices;

        /// The selected product types
        std::set<PRODUCT_TYPES> _productChoices;

        /// Buffer for saving one gate of product data over successive Products
        std::vector<double> P;

};

#endif 
