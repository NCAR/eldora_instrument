#ifndef ELDORAQTPRODUCTSSOURCE_H_
#define ELDORAQTPRODUCTSSOURCE_H_

#include <vector>
#include "EldoraQtSource.h"
#include "DDSReader.h"
#include "ProductsTypeSupportC.h"
#include "ProductsTypeSupportImpl.h"
#include "ProductTypes.h"
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
                DDSSubscriber& subscriber,
                    std::string topicName,
                    double outputRate=20.0,
                    RADAR_CHOICE radarChoices=RADAR_FOR,
                    std::set<PRODUCT_TYPES> productChoices=std::set<PRODUCT_TYPES>());
        virtual ~EldoraQtProductsSource();
        /// Subclass DDSReader::notify(), which wil be called
        /// whenever new samples are added to the DDSReader available
        /// queue. Process the samples here.
        virtual void notify();

   signals:
        /// This signal is emitted when new Product data are available.
        /// I and Q will be of the same length, and will have at least 1 
        /// value in them.
        /// @param P P data values.
        /// @param radarId Either EldoraDDS::Fore or EldoraDDS::Aft
        /// @param prodType The product type, from PRODUCT_TYPES
       void newPData(
                std::vector<double> P, int radarId, int prodType);
        
    public slots:
        /// Set the gate mode to ONE_GATE.
        /// @param product The product type.
        /// @param forwardRadar True if forward radar, false if aft.
        /// @param gate The gate to send in.
        /// @param n The number of points.
        virtual void oneGateSlot(
                PRODUCT_TYPES product,
                    bool forwardRadar,
                    int gate,
                    int n);

        /// Set the gate mode to along beam.
        /// @param product The product type.
        /// @param forwardRadar True if forward radar, false if aft
        virtual void alongBeamSlot(
                PRODUCT_TYPES product,
                    bool forwardRadar);
        
    protected:
        /// Locate the specified product.
        /// @param prodType The specific product type
        /// @param pItem Pointer to a Products item.
        /// @param data A pointer to the data field selected by _product is returned here.
        /// @param gain The gain factor for the product is returned here.
        /// @param offset The offset for the product is returned here.
        void selectProduct(PRODUCT_TYPES prodType, 
                Products* pItem, short** data, float& gain, float& offset);
        
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
