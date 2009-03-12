#ifndef EldoraCappi_H
#define EldoraCappi_H


#include <deque>
#include <map>
#include <set>
#include <sstream>
#include <locale>
#include <boost/date_time/posix_time/posix_time.hpp>
using boost::posix_time::ptime;
using boost::posix_time::seconds;
using boost::posix_time::time_facet;
#include <iomanip>
#include <string>
#include <algorithm>
#include <iostream>
#include <time.h>
#include <math.h>

#include <QDialog>
#include <QPalette>
#include <QVBoxLayout>
#include <QEvent>
#include <QAction>
#include <QActionGroup>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QButtonGroup>
#include <QLabel>
#include <QTimer>
#include <QSpinBox>	
#include <QLCDNumber>
#include <QSlider>
#include <QLayout>
#include <QTabWidget>
#include <QWidget>
#include <QRadioButton>
#include <QButtonGroup>
#include <QFrame>
#include <QPushButton>
#include <QPalette>
#include <QDateTime>
#include <QFileDialog>
#include <QColorDialog>
#include <QPixmap>
#include <QIcon>
#include <QPainter>

#include "EldoraTypes.h"
#include "ProductInfo.h"
#include "CappiManager.h"
#include "CAPPI.h"
#include "CappiReader.h"
#include "CappiTime.h"
#include "utilities/TimetagUtils.h"

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
 
 In addition to radio buttons, hot keys are provided for selecting
 the data product for the display. Function keys do this for the display. 
 
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
	EldoraCappi(std::string inputFile, std::string title, QDialog* parent = 0);
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
	/// draw the grid lines on the ppi.
	/// @param dwellWidth The angular width of one dwell.
	/// @param airspdCorr The airspeed correction that can be added to the 
	/// radial velocity if desired.
	/// @param rollAngle The aircraft roll angle.
	//                std::vector<double> p, 
	void productSlot(DoubleVec p, unsigned long rec, int prodType, ptime timeTag,
			StrMapDouble hskpMap);

	void displayRecord(unsigned long rec);

	/// Call when the plot type is changed.  - unused!
	/// @param plotType One of the products from ProductTypes.h
	//                virtual void plotTypeSlot( int plotType);
	/// Save the ppi display to a PNG file.
	void saveImageSlot();
	/// Activated when a mouse click is released for the color bar.
	void colorBarSlot();
	/// Activated when the ColorBarSettings dialog is finished.
	/// @param result The dialog result code
	void colorBarSettingsFinishedSlot(int result);
	/// Called to change the product type
	/// @param id The product type id.
	void productTypeSlot(int id);
	/// Called when a function key is pressed, to switch the 
	/// product selection
	void fkeyTriggered(QAction *qa);
	/// Call to select zooming with the cursor on the ppi.
	void cursorZoomSlot();
	/// Call to select panning with the cursor on the ppi.
	void cursorPanSlot();
	/// Update the display of the grid spacing
	void gridDeltaSlot(double spacingDeg);
	/// receive a time specification
	void setTimeSlot(CappiTime::MODE mode, ptime startTime, ptime stopTime);

	void panUpSlot();
	void panDownSlot();
	void panLeftSlot();
	void panRightSlot();
	void resetViewSlot();
	void autoSaveSlot(bool);
	void gridSlot(bool);
	void backgroundColorSlot();
	void gridColorSlot();
	void zoomInSlot();
	void zoomOutSlot();

signals:
	void newProductSignal(DoubleVec p, int prodType, ptime timeTag,
			StrMapDouble hskpMap);

protected:
	/// poll for new data
	void pollNewData();
	/// Determine the earliest and lates times in the file
	void getTimeLimits();
	/// Initialize all of the color maps. It creates the master list of 
	/// colormaps which are available. These are the combination of
	/// the ColorMap builtin maps, and the ones specified in the
	/// configuration. The master list lives in _colorMaps.
	void initColorMaps();
	/// Configure the ProductInfo entry for a product, 
	/// getting values from the configuration. Create the radio buttons for 
	/// each product and add to the control panel. Create a consecutive
	/// entry in _productIndex each time setProductInfo is called.
	void setProductInfo(PRODUCT_TYPES t, ///< The product type
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
	void colorBarPopup();
	void pan(double x, double y);
	/// compute the pojnting angle of an eldora beam
	double pointingAngle(double rotangle, double heading, double tiltAngle);
	/// Automatically save an image without prompting.
	/// @param index A number that will be suffixed to the file name
	void saveImageAuto(int index);
	CAPPI* _cappi;
	/// The manager for the CAPPI
	CappiManager _manager;
	/// The cappi time control interface
	CappiTime* _cappiTime;
	/// The currently selected product.
	PRODUCT_TYPES _prodType;
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
	// how often to update the statistics (in seconds)
	int _statsUpdateInterval;
	// The builtin timer will be used to calculate beam statistics.
	void timerEvent(QTimerEvent*);
	/// initialize all of the book keeping structures
	/// for the various plots.
	void initPlots();
	/// The configuration for EldoraCappi
	QtConfig _config;
	/// Palette for making the leds green
	QPalette _greenPalette;
	/// Platette for making the leds red
	QPalette _redPalette;
	/// The number of gates
	unsigned int _gates;
	/// The gate size in degrees
	double _gateSizeDeg;
	/// The angular width of one dwell
	double _dwellWidth;
	/// The tilt angle of the radar
	double _radarTiltAngle;
	/// For each PRODUCT_TYPES, there will be an entry in this map.
	std::map<PRODUCT_TYPES, ProductInfo> _productInfo;
	/// The layout which contains the radio buttons for product
	/// selection for the display.
	QVBoxLayout _vBox;
	/// The button group for the radio buttons for product 
	/// selection for the display. Each button in the group
	/// is assigned an id equal to the PRODUCT_TYPE for that 
	/// product.
	QButtonGroup _buttonGroup;
	/// Action group for the fkeys shortcuts
	QActionGroup* fkeysActionGroup;
	/// timer id for status update
	int _statusTimerId;
	/// timer id for checking for new data 
	int _checkNewDataId;
	/// the CappiReader
	CappiReader _cappiReader;
	unsigned long _lastCappiRec;
	/// an additional "fine-tune" of the roll angle correction
	double _rollOffset;
	double _firstLat;
	double _firstLon;
	double _lat;
	double _lon;
	/// time of last record;
	ptime _lastTime;
	/// product type of last record
	int _lastProdType;
	/// The X and Y span of the CAPPI
	double _spanDeg;
	/// Set true if the display will use strips for each beam
	bool _stripDisplay;
	/// The strip width of a strip type display in  degrees
	double _stripWidthDeg;
	/// The minimum time span to display.
	time_duration _timeSpan;
	/// The earliest time in this file
	ptime _earliestTime;
	/// The latest time in this file
	ptime _latestTime;
	/// The display mode we are in. If REALTIME,
	/// all new data will be appended
	CappiTime::MODE _mode;
	/// The time of first data to display
	ptime _startTime;
	/// The time of the last data to display,
	/// unless in REATLIME mode
	ptime _stopTime;
	/// set true until we have captured the origin position
	bool _firstPos;
	/// Set true until the first data timer triggers
	bool _firstTimer;
	/// The title to be placed on the image capture
	std::string _imageTitle;
	/// set true to disable plotting via the timer.
	bool _disableTimer;
	/// set true to enable auto image save
	bool _autoSaveImage;
	/// The current index for the auto image file name
	int _autoImageIndex;

};

#endif
