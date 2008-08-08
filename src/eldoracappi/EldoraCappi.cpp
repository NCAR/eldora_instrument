#include "EldoraCappi.h"
#include "CAPPI.h"
#include "Z.xpm"
#include "Paw.xpm"

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

#include <string>
#include <algorithm>

#include <iostream>
#include <time.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////
EldoraCappi::EldoraCappi(std::string inputFile, std::string title,
                         QDialog* parent) :
    QDialog(parent),
    // _prodTypeUpper(PROD_DBZ), _prodTypeLower(PROD_DBZ), 
    _statsUpdateInterval(5), _config("NCAR", "EldoraCappi"), _paused(false), 
    _gates(0), _gateSizeMeters(0.0), _dwellWidth(0), _left(-1.0), _right(1.0), 
    _bottom(-1.0), _top(1.0), _rotAngle(0.0), _rollAngle(0.0),
    _lastCappiRec(0),_rollOffset(0.0), _lastXKm(0.0), _lastYKm(0.0),_lastTime(0)
    {
    // Set up our form
    setupUi(parent);
    
    QVBoxLayout* l = new QVBoxLayout;
    _cappi = new CAPPI(cappiFrame);
    l->addWidget(_cappi);
    cappiFrame->setLayout(l);
    

    // configure check buttons
    _paused = pauseRunButton->isChecked();

    // set the title
    parent->setWindowTitle(title.c_str());

     // creat the hot key action group
    fkeysActionGroup = new QActionGroup(parent);
    fkeysActionGroup->setEnabled(true);
    fkeysActionGroup->setExclusive(false);
    
    // fill in the product list
    _productList.insert(PROD_DBZ);
    _productList.insert(PROD_VR);

    // create a couple of images to be used as button icons. They
    // are created from XPM arrays imported from the included
    // xpm files.
    QPixmap paw(Paw);
    QPixmap z(Z);
    panButton->setIcon(QIcon(paw));
    zoomButton->setIcon(QIcon(z));
    zoomButton->setChecked(true);
    
     // connect the controls
    
    connect(fkeysActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(fkeyTriggered(QAction*)));

    // The color bar popup
    connect(colorBarFor, SIGNAL(released()), this, SLOT(colorBarUpperSlot()));

    // product selection buttons
    connect(&_upperButtonGroup, SIGNAL(buttonReleased(int)), this, SLOT(productTypeUpperSlot(int)));
    
    connect(pauseRunButton, SIGNAL(toggled(bool)), this, SLOT(pauseRunSlot(bool)));

    connect(gridCheck,     SIGNAL(clicked(bool)),     this, SLOT(gridSlot(bool)));
    connect(ringsCheck,    SIGNAL(clicked(bool)),     this, SLOT(ringsSlot(bool)));
    connect(zoomIn,        SIGNAL(released()),        this, SLOT(zoomInSlot()));
    connect(zoomOut,       SIGNAL(released()),        this, SLOT(zoomOutSlot()));
    connect(backgroundButton, SIGNAL(released()),     this, SLOT(backgroundColorSlot()));
    connect(ringColorButton,  SIGNAL(released()),     this, SLOT(ringColorSlot()));
    connect(panUp,         SIGNAL(released()),        this, SLOT(panUpSlot()));
    connect(panDown,       SIGNAL(released()),        this, SLOT(panDownSlot()));
    connect(panLeft,       SIGNAL(released()),        this, SLOT(panLeftSlot()));
    connect(panRight,      SIGNAL(released()),        this, SLOT(panRightSlot()));
    connect(viewReset,     SIGNAL(released()),        this, SLOT(resetViewSlot()));
    connect(saveImage,     SIGNAL(released()),        this, SLOT(saveImageSlot()));
    connect(zoomButton,    SIGNAL(released()),        this, SLOT(cursorZoomSlot()));
    connect(panButton,     SIGNAL(released()),        this, SLOT(cursorPanSlot()));

    // initialize the color maps
    initColorMaps();

    // initialize the book keeping for the ppi displays.
    initPlots();

    // setup the displays themselves.
    int decimation = _config.getInt("Decimation", 1);
    int ppiHeight = _config.getInt("Size/MinHeight", 300);
    _upperManager.setup(_cappi, _productMaps.size(), &_productMaps, decimation, ppiHeight);
    
    // get the display specifications
    _left         = _config.getDouble("Clipping/Left", -1.0);
    _right        = _config.getDouble("Clipping/Right", 1.0);
    _bottom       = _config.getDouble("Clipping/Bottom", -0.2);
    _top          = _config.getDouble("Clipping/Top", 0.8);
    _spanKm       = _config.getDouble("Size/SpanKm", 1000.0);
    _stripDisplay = _config.getBool("Display/StripDisplay", true);
    _stripWidthKm = _config.getDouble("Display/StripWidthKm", 2.2);
    
    // run through all of the product types and configure the 
    // the for and aft displays.
    for (std::set<PRODUCT_TYPES>::iterator i = _productList.begin(); 
         i != _productList.end(); i++){
        productTypeUpperSlot(*i);
    }
    
    // add hot keys 
    for (unsigned int i = 0; i < _productList.size(); i++) {
    	QAction* F1;
    	F1 = new QAction(fkeysActionGroup);
    	F1->setEnabled(true);
    	F1->setShortcut(Qt::Key_F1 + i);
    	F1->setShortcutContext(Qt::ApplicationShortcut);
    	F1->setActionGroup(fkeysActionGroup);
    	parent->addAction(F1);

    	F1 = new QAction(fkeysActionGroup);
    	F1->setEnabled(true);
    	F1->setShortcut('1' + i);
    	F1->setShortcutContext(Qt::ApplicationShortcut);
    	F1->setActionGroup(fkeysActionGroup);
    	parent->addAction(F1);
    }


    // and then set the initial display
    productTypeUpperSlot(PROD_DBZ);
   
    // specify cursor zooming
    cursorZoomSlot();
    
    // start the statistics timer
    //_statusTimerId = startTimer(100);

    if (!_cappiReader.openFile(inputFile)) {
        std::cerr << "could not open " << inputFile << std::endl;
        exit(1);
    }
    
    _checkNewDataId = startTimer(5000);
}
//////////////////////////////////////////////////////////////////////
EldoraCappi::~EldoraCappi() {
}

//////////////////////////////////////////////////////////////////////
void EldoraCappi::productSlot(
        std::vector<double> p, 
        int prodType, 
        double timeTag,
        StrMapDouble hskpMap) {
    
    PRODUCT_TYPES productType = (PRODUCT_TYPES) prodType;
    double gateSizeMeters = hskpMap["gateSpacingMeters"];
    double dwellWidth = hskpMap["dwellWidth"];
    double rotAngle = hskpMap["rotAngle"];
    double rollAngle = hskpMap["roll"];
    double airspdCorr = hskpMap["airSpdCorr"];
    double heading  = hskpMap["heading"];
    double radarTiltAngle = hskpMap["radarTiltAngle"];
    double groundSpeedEW = hskpMap["groundSpeedEW"];
    double groundSpeedNS = hskpMap["groundSpeedNS"];
    

    double adjustedAngle = rollAngle + rotAngle + _rollOffset;
    if (0) {
    	std::cout << prodType << " " << (unsigned long long)timeTag << " " << rotAngle << "\n";
    }
    if (0) {
    std::cerr << "EldoraCappi::productSlot() time:" << (unsigned long long)timeTag 
    	<< " prodType:" << prodType  
    	<< " p[0]:" << p[0] 
    	<< " \n   groundSpeedEW:" << groundSpeedEW
    	<< " groundSpeedNS:" << groundSpeedNS
    	<< " \n   tilt:" << radarTiltAngle
    	<< " heading:" << heading
    	<< " rollAngle:" << rollAngle
    	<< " rotAngle:" << rotAngle
    	<< " \n   adjustedAngle:" << adjustedAngle
    	<< " airspdCorr:" << airspdCorr
        <<   std::endl;   
    }

    // is this an angle we want to look at?
    if (! flteq(adjustedAngle, 90.0, dwellWidth/2.0)
        && ! ( flteq(adjustedAngle, 270.0, dwellWidth/2.0)) )
        return;

    
    // did we already handle a beam with this product type that matched?
    // (there could be two beams that are equally close to the
    // dwellWidth/2.0 )
    if ( (timeTag == _lastTime) && (prodType == _lastProdType)) {
        return;
    }
    
    // ignore products that we are not interested in. Of course, the 
    // caller is not supposed to send them to us, but perform a
    // sanity check here anyway.
    if (_productList.find(productType) == _productList.end())
        return;
    
    // ignore product if we are paused.
    if (_paused)
        return;

    // if the product size has changed, reconfigure the ppi displays
    if (p.size() != _gates || gateSizeMeters != _gateSizeMeters || dwellWidth != _dwellWidth) {
   	
    	_dwellWidth = dwellWidth;
        _gates = p.size();
        _gateSizeMeters = gateSizeMeters;
        
        _upperManager.configureCAPPI(_productList.size(), _gates,  _gateSizeMeters, 
                                   _left, _right, _bottom, _top, _spanKm, _stripDisplay,
                                   _stripWidthKm);
        
        // display sbome of the vitals
        QString t;
        //        t.setNum(nBeams);
        // beamsText->setText(t);
        t.setNum(dwellWidth, 'f', 2);
        dwellWidthText->setText(t);
        t.setNum(_gates);
        gatesText->setText(t);
        
    }

    // apply the airspeed correction to VR
    /// @todo vr correction not done for cappi?
    //if (productType == PROD_VR) {
    //    for (unsigned int i = 0; i < p.size(); i++)
    //        p[i] += airspdCorr;
    //}
    
    // Map the product type into the zero based index for the CAPPIManager.
    int index = _productInfo[productType].getUserData();

    double corRotAngle = rotAngle + rollAngle;
    double cartAngle;
   
    cartAngle = 450 - heading;
    // is the horizontal beam pointed to the right of the plane?
    if ((80.0 <= corRotAngle)  && (corRotAngle <= 100.0)) {
        cartAngle = cartAngle + 90 - radarTiltAngle;
    } else {
        // no, it's pointing to the left side
        cartAngle = cartAngle - 90  + radarTiltAngle;
    }
    
    while (cartAngle < 0.0) 
    	cartAngle += 360.0;
    while (cartAngle >= 360.0)
    	cartAngle -= 360.0;
    
    // compute relative position, based on ground speed
    if (_lastTime) {
    	double secs = timeTag - _lastTime;
        _lastXKm += (groundSpeedNS * secs)/1.0e3;   // m to km
        _lastYKm += (groundSpeedEW * secs)/1.0e3;
        double groundSpeed = sqrt(groundSpeedNS*groundSpeedNS + groundSpeedEW*groundSpeedEW);
        if (0) {
        	std::cout << "ground speed:" << groundSpeed
          << " deltaT:" << secs
          << " distance:" << groundSpeed*secs/1000.0
          << " _lastXKm:"      << _lastXKm
          << " _lastYKm:"      << _lastYKm
          << " EW:" << groundSpeedEW
          << " NS:" << groundSpeedNS
          << "\n";
        }
    }
    _lastTime = timeTag;
    _lastProdType = prodType;
    
   // send the product to the appropriate ppi manager
    if (_upperManager.newProduct(p, _lastXKm, _lastYKm, cartAngle, index)) {
        	_rotAngle = cartAngle;
        	_rollAngle = rollAngle;
    }
}
//////////////////////////////////////////////////////////////////////
void EldoraCappi::saveImageSlot() {
    QString f = _config.getString("imageSaveDirectory", "c:/").c_str();

    QFileDialog d( this, tr("Save EldoraCappi Image"), f,
            tr("PNG files (*.png);;All files (*.*)"));
    d.setFileMode(QFileDialog::AnyFile);
    d.setViewMode(QFileDialog::Detail);
    d.setAcceptMode(QFileDialog::AcceptSave);
    d.setConfirmOverwrite(true);
    d.setDefaultSuffix("png");
    d.setDirectory(f);

    f = "EldoraCappi-";
    f += QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
    f += ".png";
    d.selectFile(f);
    if (d.exec()) {
        QStringList saveNames = d.selectedFiles();
        //_cappi->saveImageToFile(saveNames[0].toStdString());
        f = d.directory().absolutePath();
        _config.setString("imageSaveDirectory", f.toStdString());
    }
}

////////////////////////////////////////////////////////////////////
void EldoraCappi::initPlots() {

    int index = 0;
    setProductInfo(PROD_DBZ, index++, "DBZ", "DBZ", "Reflectivity", -60.0, 20.0, true);
    setProductInfo(PROD_VR, index++, "VR", "VR", "Velocity (radial)", -30.0, 30.0);

    // The buttons were created and assigned to layouts in setPpiInfo.
    // Attach these layouts to the button groups
    //    forwardGroupBox->setLayout(&_upperVBox);
    // aftGroupBox->setLayout(&_lowerVBox);

}

//////////////////////////////////////////////////////////////////////
/// triggered from startTimer()

void EldoraCappi::timerEvent(
        QTimerEvent*event) {

    if (_statusTimerId == event->timerId()) {
      QString t;
      t.setNum(_rotAngle, 'f', 1);
      angleText->setText(t);
      t.setNum(_rollAngle, 'f', 1);
      rollText->setText(t);
    } else if (_checkNewDataId == event->timerId()) {
        pollNewData();
    }
               
 
}

//////////////////////////////////////////////////////////////////////
void EldoraCappi::pauseRunSlot(
        bool p) {
    _paused = p;
    pauseRunButton->setText(p ? "Run" : "Pause");
}

//////////////////////////////////////////////////////////////////////
void EldoraCappi::initColorMaps() {

    // get the builtin maps. Note that we are expecting there to
    // be one named default.
    std::vector<std::string> mapNames = ColorMap::builtinMaps();
    for (unsigned int i = 0; i < mapNames.size(); i++) {
        _colorMaps[mapNames[i]] = ColorMap(0.0, 1.0, mapNames[i]);
    }

    // get the names of all existing colormaps saved in the configuration.
    std::vector<std::string> names = _config.childGroups("ColorMaps");

    std::vector<std::vector<int> > emptyMap;
    // save colormaps for each of these choices.
    for (unsigned int i = 0; i < names.size(); i++) {
        std::vector<std::vector<int> > colors;
        std::string mapKey("ColorMaps/");
        mapKey += names[i];
        colors = _config.getArray(mapKey, "RGB", emptyMap);
        // create the map. Use a bogus range, which will be reset 
        // when the map is used by a variable.
        // also note that this can allow ua to overide any of
        // the builtin maps.
        if (colors.size() != 0) {
            _colorMaps[names[i]] = ColorMap(0.0, 1.0, colors);
        }
    }

}
//////////////////////////////////////////////////////////////////////
void EldoraCappi::setProductInfo(
        PRODUCT_TYPES t, int index, std::string key, std::string shortName,
        std::string longName, double defaultScaleMin, double defaultScaleMax,
        bool setChecked) {
    
    // Add this product to our list of products. The set
    // will allow for rapid determination of whteher a product is one 
    // that we want.
    _productList.insert(t);
    
    // create the configuration key names
    std::string minKey = key;
    minKey += "/min";
    std::string maxKey = key;
    maxKey += "/max";
    std::string mapKey = key;
    mapKey += "/colorMap";

    // get the product configuration values
    double min = _config.getDouble(minKey, defaultScaleMin);
    double max = _config.getDouble(maxKey, defaultScaleMax);
    std::string mapName = _config.getString(mapKey, "default");
    if (_colorMaps.find(mapName) == _colorMaps.end()) {
        mapName = "default";
    }

    // Now initialize  all of the configuration information for this product.
    _productInfo[t]
            = ProductInfo(t, index, key, shortName, longName, mapName, min, max);

    // synchronoze the configuration. But why here?
    _config.sync();

    // Put a new map into the color map vector. Remember that the 
    // location of the color map in _productMaps will correspond
    // to the product entry in _productList
    ColorMap* map = new ColorMap(_colorMaps[mapName]);
    map->setRange(min, max);
    _productMaps.push_back(map);

    // create the product selection buttons, forward and aft
    QRadioButton* button;

    // forward button
    button = new QRadioButton(tr(shortName.c_str()));
    if (setChecked)
        button->setChecked(true);
    button->setToolTip(longName.c_str());
    _upperVBox.addWidget(button);
    _upperButtonGroup.addButton(button);
    _upperButtonGroup.setId(button, t);

    // aft button
    button = new QRadioButton(tr(shortName.c_str()));
    if (setChecked)
        button->setChecked(true);
    button->setToolTip(longName.c_str());
    _lowerVBox.addWidget(button);
    _lowerButtonGroup.addButton(button);
    _lowerButtonGroup.setId(button, t);
    
}
//////////////////////////////////////////////////////////////////////
void EldoraCappi::colorBarUpperSlot() {
    colorBarPopup(true);
}

//////////////////////////////////////////////////////////////////////
void EldoraCappi::colorBarPopup(bool upper) {
    
    // get the current settings for the selected product in
    // the selected colormap.
    PRODUCT_TYPES prodType;
    prodType = _prodTypeUpper;
    
    double min = _productInfo[prodType].getScaleMin();
    double max = _productInfo[prodType].getScaleMax();
    std::string currentName = _productInfo[prodType].getColorMapName();

    // create the color bar settings dialog
    std::vector<std::string> mapNames;
    for (std::map<std::string, ColorMap>::iterator i = _colorMaps.begin(); i
            != _colorMaps.end(); i++) {
        mapNames.push_back(i->first);
    }

    _colorBarSettings = new ColorBarSettings(min, max, currentName, mapNames, upper, this);

    // connect the finished slot so that the dialog status 
    // can be captuyred when the dialog closes
    connect(_colorBarSettings, SIGNAL(finished(int)), this, SLOT(colorBarSettingsFinishedSlot(int)));

    // and show it
    _colorBarSettings->show();

}
//////////////////////////////////////////////////////////////////////////////
void EldoraCappi::colorBarSettingsFinishedSlot(
        int result) {
    // see if the OK button was hit
    if (result == QDialog::Accepted) {
        
        // which colorbar are we aorking with?
        PRODUCT_TYPES prodType;
        //        if (_colorBarSettings->forwardRadar())
        prodType = _prodTypeUpper;
            //        else
            // prodType = _prodTypeLower;
        
        // get the scale values from the settings dialog
        double scaleMin = _colorBarSettings->getMinimum();
        double scaleMax = _colorBarSettings->getMaximum();

        // if the user inverted the values, swap them
        if (scaleMin > scaleMax) {
            double temp = scaleMax;
            scaleMax = scaleMin;
            scaleMin = temp;
        }

        // get the map name
        std::string newMapName = _colorBarSettings->getMapName();

        // save the new map name
        _productInfo[prodType].setColorMapName(newMapName);

        // configure the color bar with the new map and ranges.
        // first figure out whch entry in the _productMaps vector
        // corresponds to this product.
        int index = _productInfo[prodType].getUserData();
        // get rid of the existing map
        delete _productMaps[index];
        // create a new map
        ColorMap* newMap = new ColorMap(_colorMaps[newMapName]);
        _productMaps[index] = newMap;
        // set range on the new color map
        _productMaps[index]->setRange(scaleMin, scaleMax);
        
        // configure the color bar with it
        colorBarFor->configure(*_productMaps[index]);

        // assign the new scale values to the current product
        _productInfo[_prodTypeUpper].setScale(scaleMin, scaleMax);
        // save the new values in the configuration
        // create the configuration keys
        std::string key = _productInfo[prodType].getKey();
        std::string minKey = key + "/min";
        std::string maxKey = key + "/max";
        std::string mapKey = key + "/colorMap";

        // set the configuration values
        _config.setDouble(minKey, scaleMin);
        _config.setDouble(maxKey, scaleMax);
        _config.setString(mapKey, newMapName);
    }
}
//////////////////////////////////////////////////////////////////////////////
void EldoraCappi::productTypeUpperSlot(int id) {
    // set the ppiType
    _prodTypeUpper = (PRODUCT_TYPES)id;
    
    // get the _productsMap index
    int index = _productInfo[_prodTypeUpper].getUserData();
    
    // inform the display
    _upperManager.selectVar(index);
    
    // configure the color bar with it
    colorBarFor->configure(*_productMaps[index]);
}

void EldoraCappi::fkeyTriggered(QAction* qa) {
	
	// ignore keys that we are not expecting
	
	int numkey = qa->shortcut() - '1';	
	if (numkey >= 0 && numkey < (int)_productList.size()) {
		// Lower display:
		// Got a number key. See if this product index exists in _productInfo
		for (std::map<PRODUCT_TYPES, ProductInfo>::iterator i = _productInfo.begin();
		i != _productInfo.end(); i++) {
			if (i->second.getUserData() == numkey ) {
				// Matched it.
			}
		}
	} else {
		int fkey = qa->shortcut() - Qt::Key_F1;
		if (fkey >= 0 && fkey < (int)_productList.size()) {
			// Upper display:
			// Got a function key. See if this product index exists in _productInfo
			for (std::map<PRODUCT_TYPES, ProductInfo>::iterator i = _productInfo.begin();
			i != _productInfo.end(); i++) {
				if (i->second.getUserData() == fkey ) {
					// matched it
					productTypeUpperSlot(i->second.getId());
					QAbstractButton* button = 
						_upperButtonGroup.button(i->second.getId());
					if(button)
						button->setChecked(true);	
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////

void EldoraCappi::ringsSlot(bool enabled)
{
    _cappi->rings(enabled);
}
///////////////////////////////////////////////////////////////////////

void EldoraCappi::gridSlot(bool enabled)
{
    _cappi->grids(enabled);
}
///////////////////////////////////////////////////////////////////////
void
EldoraCappi::backgroundColorSlot() 
{
    QColor color = QColorDialog::getColor("blue");

    _cappi->backgroundColor(color);
}

///////////////////////////////////////////////////////////////////////
void
EldoraCappi::ringColorSlot() 
{
    QColor color = QColorDialog::getColor("black");

    _cappi->gridRingsColor(color);
}
///////////////////////////////////////////////////////////////////////
void
EldoraCappi::pan(double x, double y)
{
     _cappi->pan(x, y);
}
///////////////////////////////////////////////////////////////////////
void 
EldoraCappi::panUpSlot()
{
    pan(0.0, 0.1);
}
///////////////////////////////////////////////////////////////////////
void 
EldoraCappi::panDownSlot()
{
    pan(0.0, -0.1);
}
///////////////////////////////////////////////////////////////////////
void 
EldoraCappi::panLeftSlot()
{
    pan(-0.1, 0.0);
}
///////////////////////////////////////////////////////////////////////
void 
EldoraCappi::panRightSlot()
{
    pan(0.1,0.0);
}
///////////////////////////////////////////////////////////////////////
void
EldoraCappi::resetViewSlot()
{
    _cappi->resetView();
}
///////////////////////////////////////////////////////////////////////
void EldoraCappi::zoomInSlot()
{
    _cappi->setZoom(_cappi->getZoom()*2.0);
}
///////////////////////////////////////////////////////////////////////
void EldoraCappi::zoomOutSlot()
{
    _cappi->setZoom(_cappi->getZoom()*0.5);
}
///////////////////////////////////////////////////////////////////////
void EldoraCappi::cursorZoomSlot() {
    _cappi->cursorZoom();
}
///////////////////////////////////////////////////////////////////////
void EldoraCappi::cursorPanSlot() {
    _cappi->cursorPan();
}
///////////////////////////////////////////////////////////////////////
void EldoraCappi::pollNewData() 
{
    unsigned long lastRec ;
    bool ok;
    ok = _cappiReader.findLastRecord(lastRec);
    if (!ok) {
        std::cerr << "findLastRecord failed" << std::endl;
        return;
    }
    
    unsigned long rec;
    for (rec = _lastCappiRec; rec < lastRec; ++rec){
    	displayRecord(rec);
    }
    _lastCappiRec = lastRec-1;
    
}
///////////////////////////////////////////////////////////////////////
void EldoraCappi::displayRecord(unsigned long rec) {
	int prodType;
	double timeTag;
	StrMapDouble hskpMap;
	std::vector<double> product;
	bool ok = _cappiReader.read(rec, product, prodType, timeTag, hskpMap);
	if (ok) {
	    // emit signal
	    emit newProductSignal(product, prodType, timeTag, hskpMap);
	} else {
		std::cerr << "unable to get record " << rec << "\n";
	}
}
