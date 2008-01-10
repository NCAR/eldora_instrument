#ifndef EldoraPPI_H
#define EldoraPPI_H

#include <QDialog>
#include <QPalette>
#include <qevent.h>
#include <deque>
#include <map>
#include <set>

#include "PpiInfo.h"

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
 eldora 
 
 EldoraPPI is simply a data consumer; it does not know
 anything about the data provider. Signals and slots
 used to coordinate with other components. Data are delivered
 to EldoraPPI by calling  newProductSlot().
 
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
        /// Activated when a mouse click is released for the color bar.
        void colorBarReleasedSlot();
        /// Activated when the ColorBarSettings dialog is finished.
        /// @param result The dialog result code
        void colorBarSettingsFinishedSlot(int result);
    protected:
        // Configure the PPI displays when there is a change in the operating
        // configuration, such as the number of gates, etc.
        void configurePPI();
        /// Initialize the color maps.
        void initColorMaps();
        /// Configure the PpiInfo entry for a product, 
        /// getting values from the configuration.
        void setPpiInfo(
                PRODUCT_TYPES t, ///< The product type
                std::string key, ///< The key to use in the configuration
                std::string shortName, ///< Short name
                std::string longName, ///< Long name
                double defaultScaleMin, ///< Colorbar minimum
                double defaultScaleMax, ///< Colorbar maximum
                int ppiVarIndex ///< Variable index on the selection tab
                );
        /// The currently selected ppi type.
        PRODUCT_TYPES _ppiType;
        /// This set contains PRODUCT_TYPESs identifiers for all desired 
        /// products plots. It is used to filter products from
        /// the incoming data stream.
        std::set<PRODUCT_TYPES> _productList;
        /// This set contains the list of all received products 
        /// that are on the desired list, and have the same beam id.
        /// When the length reaches the same size as _productList, 
        /// then we have all products for a given beam.
        std::set<PRODUCT_TYPES> _currentProducts;
        /// Will hold the beam values for all S band variables in one beam
        std::vector<std::vector<double> > _beamData;
        /// Color maps for each product
        std::vector<ColorMap*> _maps;
        /// The dialog that will collect colorbar settings
        ColorBarSettings* _colorBarSettings;
        /// The available ColorMaps. They are individually identified by a name.
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
        /// The number of gates
        int gates;
        /// The nuber of gates
        int _gates;
        /// For each PRODUCT_TYPES, there will be an entry in this map.
        std::map<PRODUCT_TYPES, PpiInfo> _ppiInfo;

};

#endif
