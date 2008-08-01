#ifndef EldoraCappi_H
#define EldoraCappi_H

#include <QDialog>
#include <QPalette>
#include <QVBoxLayout>
#include <QEvent>
#include <QAction>
#include <QActionGroup>
#include <deque>
#include <map>
#include <set>

#include "EldoraTypes.h"

#include "ProductInfo.h"

#include "CappiManager.h"
#include "CAPPI.h"

#include "CappiReader.h"

// Coponents from the QtToolbox
#include "ColorMap.h"
#include "ColorBar.h"
#include "QtConfig.h"

// The color bar setting dialog
#include "ColorBarSettings.h"

// The designer generated header file.
#include "ui_EldoraCappi.h"
// Symbolic names for product types
#include "ProductTypes.h"
#define flteq(val1, val2, tol) (fabs(val1-val2) <= tol)
/** 
 EldoraCappi provides a real-time CAPPI display of 
 eldora products from one radar. 
 
 The user can configure the colormap for a given product by clicking on 
 the color bar.
 
 The upper and lower displays each render the same fixed set of products 
 (PROD_DM, PROD_SW, etc.). The same colormap will be used on both 
 displays for a given product. Thus if the colormap and colorbar is changed 
 for a product on the upper display, and the same product is being displayed
 on the lower display, the lower colorbar will also be updated.
 
 In addition to radio buttons, hot keys are provided for selecting
 the data product for each display. Number keys, starting at 1,
 will select products on the upper display. Function keys, starting at F1,
 do the same for the lower display. 
 
 EldoraCappi is simply a data consumer; it does not know
 anything about the data provider. Signals and slots
 used to coordinate with other components. Product data are delivered
 to EldoraCappi by calling newProductSlot(). EldoraCappi collects 
 products for each radar until a complete set of products are
 found (for a given beam), and then the data are sent to the
 PPI display.
 
 EldoraCappi is configured via EldoraCappi.ini.
 **/
class EldoraCappi : public QDialog, public Ui::EldoraCappi {
    Q_OBJECT
    public:
    EldoraCappi(std::string inputFile, std::string title,
                    QDialog* parent = 0);
        virtual ~EldoraCappi();

    public slots:
        /// Feed product data via this slot. If the operating 
        /// characteristics change, such as the number of gates,
        /// or the gate spacing, the displays will be reconfigured.
        /// @param p The product data.
        /// @param radarId Either EldoraDDS::Fore or EldoraDDS::Aft
        /// @param elDegrees The antenna pointing elevation, degrees
        /// @param prodType The product type, from PRODUCT_TYPES
        /// @param gateSizeMeters The width of each range gates. Needed to 
        /// draw the range rings on the ppi.
    	/// @param dwellWidth The angular width of one dwell.
        /// @param airspdCorr The airspeed correction that can be added to the 
        /// radial velocity if desired.
        /// @param rollAngle The aircraft roll angle.
                         //                std::vector<double> p, 
        void productSlot(DoubleVec p,
                int prodType,
                qlonglong timeTag,
                StrMapDouble hskpMap);

        /// Call when the plot type is changed.  - unused!
        /// @param plotType One of the products from ProductTypes.h
        //                virtual void plotTypeSlot( int plotType);
        /// Save the ppi display to a PNG file.
        void saveImageSlot();
        /// Pause/Run the plotting. Any data received while paused are ignored.
        /// @param p True to enable pause.
        void pauseRunSlot(
                bool p);
        /// Activated when a mouse click is released for the upper color bar.
        void colorBarUpperSlot();
        /// Activated when a mouse click is released for the lower color bar.
        // void colorBarLowerSlot();
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
        // void productTypeLowerSlot( int id);
        /// Called when a function key is pressed, to switch the 
        /// product selection
        void fkeyTriggered(QAction *qa);
        /// Call to select zooming with the cursor on the ppi.
        void cursorZoomSlot();
        /// Call to select panning with the cursor on the ppi.
        void cursorPanSlot();
        
        void panUpSlot();
        void panDownSlot();
        void panLeftSlot();
        void panRightSlot();
        void resetViewSlot();
        void ringsSlot(bool);
        void gridSlot(bool);
        void backgroundColorSlot();
        void ringColorSlot();
        void zoomInSlot();
        void zoomOutSlot();

  signals:
                              // std::vector<double> p, 
        void newProductSignal(DoubleVec p,
                int prodType,
                qlonglong timeTag,
                StrMapDouble hskpMap);
        
    protected:
        /// poll for new data
        void pollNewData();
        
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
        void pan(double x, double y);
        /// The manager for the upper PPI
        CappiManager _upperManager;
#ifdef LOWER_WINDOW
        /// The manager for the lower PPI
        //        CappiManager _lowerManager;
#endif
        /// The currently selected upper ppi type.
        PRODUCT_TYPES _prodTypeUpper;
        /// The currently selected lower ppi type.
        //        PRODUCT_TYPES _prodTypeLower;
        /// This set contains PRODUCT_TYPESs identifiers for all desired 
        /// products plots. It is used to filter products from
        /// the incoming data stream.
        std::set<PRODUCT_TYPES> _productList;
        /// The CappiManager differentiates incoming products based on an
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
        /// The configuration for EldoraCappi
        QtConfig _config;
        /// Palette for making the leds green
        QPalette _greenPalette;
        /// Platette for making the leds red
        QPalette _redPalette;
        /// Set true if the plot graphics are paused
        bool _paused;
        /// The nuber of gates
        unsigned int _gates;
        /// The gate size in meters
        double _gateSizeMeters;
        /// The angular width of one dwell
        double _dwellWidth;
        /// X coordinate of the left clipping plane (+/- 1.0)
        double _left;
        /// X coordinate of the right clipping plane (+/- 1.0)
        double _right;
        /// Y coordinate of the bottom clipping plane (+/- 1.0)
        double _bottom;
        /// Y coordinate of the top clipping plane (+/- 1.0)
        double _top;
        /// For each PRODUCT_TYPES, there will be an entry in this map.
        std::map<PRODUCT_TYPES, ProductInfo> _productInfo;
        /// The layout which contains the radio buttons for product
        /// selection for the upper display.
        QVBoxLayout _upperVBox;
        /// The layout which contains the radio buttons for product
        /// selection for the lower display.
        QVBoxLayout _lowerVBox;
        /// The button group for the radio buttons for product 
        /// selection for the upper display. Each button in the group
        /// is assigned an id equal to the PRODUCT_TYPE for that 
        /// product.
        QButtonGroup _upperButtonGroup;
        /// The button group for the radio buttons for product 
        /// selection for the lower display. Each button in the group
        /// is assigned an id equal to the PRODUCT_TYPE for that 
        /// product.
        QButtonGroup _lowerButtonGroup;
        /// rotation angle
        double _rotAngle;
        /// aircraft roll angle
        double _rollAngle;
        /// Action group for the fkeys shortcuts
        QActionGroup* fkeysActionGroup;
        /// timer id for status update
        int _statusTimerId;
        /// timer id for checking for new data 
        int _checkNewDataId;
        /// the CappiReader object
        CappiReader _cappiReader;
        unsigned long _lastCappiRec;
        /// an additional "fine-tune" of the roll angle correction
        double _rollOffset;
        /// last location of aircraft, relative to first record plotted
        double _lastXKm;
        double _lastYKm;
        /// time of last record;
        qlonglong _lastTime;
        /// product type of last record
        int _lastProdType;
        
        


};

#endif
