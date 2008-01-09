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
/// which can feed P data to Qt clients..
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
class EldoraQtProductsSource : public EldoraQtSource, public ProductsReader {
Q_OBJECT
    public:
        EldoraQtProductsSource(
                DDSSubscriber& subscriber,
                    std::string topicName,
                    double outputRate=20.0,
                    bool bothRadars=false);
        virtual ~EldoraQtProductsSource();
        /// Subclass DDSReader::notify(), which wil be called
        /// whenever new samples are added to the DDSReader available
        /// queue. Process the samples here.
        virtual void notify();

        /// Locate the currently selected product.
        /// @param pItem Pointer to a Products item.
        /// @param data A pointer to the data field selected by _product is returned here.
        /// @param gain The gain factor for the product is returned here.
        /// @param offset The offset for the product is returned here.
        void selectProduct(Products* pItem, short** data, float& gain, float& offset);
        
   signals:
        /// This signal is emitted when new Product data are available.
        /// I and Q will be of the same length, and will have at least 1 
        /// value in them.
        /// @param P P data values.
        /// @param int radarId Either EldoraDDS::Fore or EldoraDDS::Aft
       void newPData(
                std::vector<double> P, int radarId);
        
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
        /// The selected radar, either forward or aft
        EldoraDDS::RadarChoice _radarId;
        
        /// If true, ignore the _radarId choice, and send both 
        /// radars.
        bool _bothRadars;

        /// The selected product type
        PRODUCT_TYPES _product;
        
        /// Buffer for saving one gate of product data over successive Products
        std::vector<double> P;

};

#endif 
