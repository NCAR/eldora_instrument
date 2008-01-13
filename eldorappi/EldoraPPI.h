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
        EldoraPPI(
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
        /// @param int radarId Either EldoraDDS::Fore or EldoraDDS::Aft
        void productSlot(
                std::vector<double> p, int radarId);
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
        /// Activated when a mouse click is released for the forward color bar.
        void colorBarForSlot();
        /// Activated when a mouse click is released for the aft color bar.
        void colorBarAftSlot();
        /// Activated when the ColorBarSettings dialog is finished.
        /// @param result The dialog result code
        void colorBarSettingsFinishedSlot(int result);
        /// Called to change the forward product type
        /// @param id The product type id.
        void productTypeForSlot(int id);
        /// Called to change the aft product type
        /// @param id The product type id.
        void productTypeAftSlot(int id);
    protected:
        // Configure the PPI displays when there is a change in the operating
        // configuration, such as the number of gates, etc.
        void configurePPI();
        /// Initialize all of the color maps. It creates the master list of 
        /// colormaps which are available. These are the combination of
        /// the ColorMap builtin maps, and the ones specified in the
        /// configuration. The master list lives in _colorMaps.
        void initColorMaps();
        /// Configure the ProductInfo entry for a product, 
        /// getting values from the configuration.
        void setPpiInfo(PRODUCT_TYPES t, ///< The product type
            int index,                   ///< The index of this product in the _beamData and _productMaps vectors
            std::string key,             ///< The key to use in the configuration
            std::string shortName,       ///< Short name
            std::string longName,        ///< Long name
            double defaultScaleMin,      ///< Colorbar minimum
            double defaultScaleMax,      ///< Colorbar maximum
            bool setChecked=false        ///< If true, button will be checked
            );    
        /// Create a popup for configuring the colorbar. 
        /// @param forwardRadar Set true if we are configuring the forward
        /// color bar, false if the aft.
        void colorBarPopup(bool forwardRadar);
        /// The currently selected forward ppi type.
        PRODUCT_TYPES _prodTypeFor;
        /// The currently selected aft ppi type.
        PRODUCT_TYPES _prodTypeAft;
        /// This set contains PRODUCT_TYPESs identifiers for all desired 
        /// products plots. It is used to filter products from
        /// the incoming data stream.
        std::set<PRODUCT_TYPES> _productList;
        /// This set contains the list of all received products 
        /// that are on the desired list, and have the same beam id.
        /// It is used to track incoming products, so that when a 
        /// complete set of products has been received, a new beam can 
        /// be drawn.
        std::set<PRODUCT_TYPES> _currentProducts;
        /// Will hold the beam values for all product variables in one beam
        std::vector<std::vector<double> > _beamData;
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
        /// Used to collect product data from beams
        std::vector<double> _ProductData;
        // how often to update the statistics (in seconds)
        int _statsUpdateInterval;
        /// The current selected product type.
        PRODUCT_TYPES _productType;
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
        int _gates;
        /// For each PRODUCT_TYPES, there will be an entry in this map.
        std::map<PRODUCT_TYPES, ProductInfo> _productInfo;
        QVBoxLayout _forwardVBox;
        QVBoxLayout _aftVBox;
        QButtonGroup _forwardButtonGroup;
        QButtonGroup _aftButtonGroup;
        

};

#endif
