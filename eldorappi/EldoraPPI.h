#ifndef EldoraPPI_H
#define EldoraPPI_H

#include <QDialog>
#include <QPalette>
#include <QVBoxLayout>
#include <QEvent>
#include <deque>
#include <map>
#include <set>

#include "ProductInfo.h"

#include "PpiManager.h"

// Coponents from the QtToolbox
#include "PPI.h"
#include "ColorMap.h"
#include "ColorBar.h"
#include "QtConfig.h"

// The color bar setting dialog
#include "ColorBarSettings.h"

// The designer generated header file.
#include "ui_EldoraPPI.h"

// Symbolic names for product types
#include "ProductTypes.h"

/** 
 EldoraPPI provides a traditional real-time PPI display of 
 eldora products. Two PPI displays are provided; one for the forward
 radar and one for the aft radar. Each PPI has an associated ColorBar display.
 
 The user can configure the colormap for a given product by clicking on 
 the color bar.
 
 The for and aft displays each render the same fixed set of products 
 (PROD_DM, PROD_SW, etc.). The same colormap will be used on both 
 displays for a given product. Thus if the colormap and colorbar is changed 
 for a product on the aft radar, and the same product is being displayed
 on the forward colorbar, the forward colorbar will also be updated.
 
 EldoraPPI is simply a data consumer; it does not know
 anything about the data provider. Signals and slots
 used to coordinate with other components. Product data are delivered
 to EldoraPPI by calling newProductSlot(). EldoraPPI collects 
 products for each radar until a complete set of products are
 found (for a given beam), and then the data are sent to the
 PPI display.
 
 EldoraPPI is configured via EldoraPPI.ini.
 **/
class EldoraPPI : public QDialog, public Ui::EldoraPPI {
    Q_OBJECT
    public:
        EldoraPPI(std::string title,
                QDialog* parent = 0);
        virtual ~EldoraPPI();

    signals:
        /// Emmited to indicate that Product data should be delivered for 
        /// all gates along a beam
        /// @param channel The selected channel
        /// @param forwardRadar Set true if the forward radar, false otherwise.
        void alongBeamProductSignal(
                PRODUCT_TYPES product, bool forwardRadar);

    public slots:
        /// Feed product data via this slot.
        /// @param p The product data.
        /// @param radarId Either EldoraDDS::Fore or EldoraDDS::Aft
        /// @param elDegrees The antenna pointing elevation, degrees
        /// @param prodType The product type, from PRODUCT_TYPES
        void productSlot(
                std::vector<double> p, int radarId, float elDegrees,
                int prodType);
        /// Call when the plot type is changed. 
        /// @param plotType One of the products from ProductTypes.h
        //        virtual void plotTypeSlot(
        //                int plotType);
        /// Save the ppi display to a PNG file.
        void saveImageSlot();
        /// Pause the plotting. Any received data are ignored.
        /// @param p True to enable pause.
        void pauseSlot(
                bool p);
        /// Activated when a mouse click is released for the upper color bar.
        void colorBarUpperSlot();
        /// Activated when a mouse click is released for the lower color bar.
        void colorBarLowerSlot();
        /// Activated when the ColorBarSettings dialog is finished.
        /// @param result The dialog result code
        void colorBarSettingsFinishedSlot(
                int result);
        /// Called to change the upper product type
        /// @param id The product type id.
        void productTypeUpperSlot(
                int id);
        /// Called to change the lower product type
        /// @param id The product type id.
        void productTypeLowerSlot(
                int id);
    protected:
        /// Initialize all of the color maps. It creates the master list of 
        /// colormaps which are available. These are the combination of
        /// the ColorMap builtin maps, and the ones specified in the
        /// configuration. The master list lives in _colorMaps.
        void initColorMaps();
        /// Configure the ProductInfo entry for a product, 
        /// getting values from the configuration. Create the radio buttons for 
        /// each product and add to the upper and lower control panels. Create a consecutive
        /// entry in _productIndex each time setProductInfo is called.
        void setProductInfo(
                PRODUCT_TYPES t, ///< The product type
                int index, ///< The index of this product in the _beamData and _productMaps vectors
                std::string key, ///< The key to use in the configuration
                std::string shortName, ///< Short name
                std::string longName, ///< Long name
                double defaultScaleMin, ///< Colorbar minimum
                double defaultScaleMax, ///< Colorbar maximum
                bool setChecked=false ///< If true, button will be checked
                );
        /// Create a popup for configuring the colorbar. 
        /// @param forwardRadar Set true if we are configuring the forward
        /// color bar, false if the aft.
        void colorBarPopup(
                bool forwardRadar);
        /// The manager for the upper PPI
        PPIManager _upperManager;
        /// The manager for the lower PPI
        PPIManager _lowerManager;
        /// The currently selected upper ppi type.
        PRODUCT_TYPES _prodTypeUpper;
        /// The currently selected lower ppi type.
        PRODUCT_TYPES _prodTypeLower;
        /// This set contains PRODUCT_TYPESs identifiers for all desired 
        /// products plots. It is used to filter products from
        /// the incoming data stream.
        std::set<PRODUCT_TYPES> _productList;
        /// The PPIManager differentiates incoming products based on an
        /// index that runs between 0 and the number of products. This
        /// is used to map the PRODUCT_TYPE to that index.
        std::map<PRODUCT_TYPES, int> _productIndex;
        /// The color maps assigned to each product. They are drawn from _colorMaps.
        /// The vector length must be the same as _productList, and also the number
        /// of data vectors in _beamData which are passed to PPI::addBeam().
        std::vector<ColorMap*> _productMaps;
        /// The dialog that will collect colorbar settings
        ColorBarSettings* _colorBarSettings;
        /// The master collection of available ColorMaps. They are individually identified by a name.
        std::map<std::string, ColorMap> _colorMaps;
        ///	cumulative error count
        int _errorCount[3];
        ///  last pulse number
        long long _lastPulseNum[3];
        // how often to update the statistics (in seconds)
        int _statsUpdateInterval;
       // The builtin timer will be used to calculate beam statistics.
        void timerEvent(
                QTimerEvent*);
        /// initialize all of the book keeping structures
        /// for the various plots.
        void initPlots();
        /// The configuration for EldoraPPI
        QtConfig _config;
        /// Palette for making the leds green
        QPalette _greenPalette;
        /// Platette for making the leds red
        QPalette _redPalette;
        /// Set true if the plot graphics are paused
        bool _paused;
        /// The nuber of gates
        unsigned int _gates;
        /// For each PRODUCT_TYPES, there will be an entry in this map.
        std::map<PRODUCT_TYPES, ProductInfo> _productInfo;
        /// The layout which contains the radio buttons for product
        /// selection for the upper display.
        QVBoxLayout _upperVBox;
        /// The layout which contains the radio buttons for product
        /// selection for the lower display.
        QVBoxLayout _lowerVBox;
        /// The button group for the radio buttons for product 
        /// selection for the upper display.
        QButtonGroup _upperButtonGroup;
        /// The button group for the radio buttons for product 
        /// selection for the lower display.
        QButtonGroup _lowerButtonGroup;
        /// elevation angle
        double _elevation;

};

#endif
