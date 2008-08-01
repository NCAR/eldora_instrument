#include "EldoraCappi.h"
#include "CAPPI.h"
#include "Z.xpm"
#include "Paw.xpm"

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
    CAPPI *cappi = new CAPPI(CAPPI_Parent);
    

    // configure check buttons
    _paused = pauseRunButton->isChecked();

    // set the title
    parent->setWindowTitle(title.c_str());

    // initialize running statistics
    for (int i = 0; i < 3; i++) {
        _errorCount[i] = 0;
        _lastPulseNum[i] = 0;
    }

    // creat the hot key action group
    fkeysActionGroup = new QActionGroup(parent);
    fkeysActionGroup->setEnabled(true);
    fkeysActionGroup->setExclusive(false);
    
    // fill in the product list
    _productList.insert(PROD_DBZ);
    _productList.insert(PROD_VR);
#ifdef NOTDEF
    _productList.insert(PROD_DM);
    _productList.insert(PROD_VS);
    _productList.insert(PROD_VL);
    _productList.insert(PROD_SW);
    _productList.insert(PROD_NCP);
#endif
    
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
    //    connect(colorBarAft, SIGNAL(released()), this, SLOT(colorBarLowerSlot()));

    // product selection buttons
    connect(&_upperButtonGroup, SIGNAL(buttonReleased(int)), this, SLOT(productTypeUpperSlot(int)));
    //    connect(&_lowerButtonGroup, SIGNAL(buttonReleased(int)), this, SLOT(productTypeLowerSlot(int)));
    
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
    _upperManager.setup(cappi, _productMaps.size(), &_productMaps, decimation, ppiHeight);
#ifdef LOWER_WINDOW
    _lowerManager.setup(ppiAft, 7, &_productMaps, decimation, ppiHeight);
#endif
    
    // get the display clipping specifications
    _left = _config.getDouble("Clipping/Left", -1.0);
    _right = _config.getDouble("Clipping/Right", 1.0);
    _bottom = _config.getDouble("Clipping/Bottom", -0.2);
    _top = _config.getDouble("Clipping/Top", 0.8);
    
    // run through all of the product types and configure the 
    // the for and aft displays.
    for (std::set<PRODUCT_TYPES>::iterator i = _productList.begin(); 
         i != _productList.end(); i++){
        productTypeUpperSlot(*i);
#ifdef LOWER_WINDOW
        productTypeLowerSlot(*i);
#endif
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
#ifdef LOWER_WINDOW
    productTypeLowerSlot(PROD_DBZ);
#endif
   
    // specify cursor zooming
    cursorZoomSlot();
    
    // start the statistics timer
    _statusTimerId = startTimer(100);

    _checkNewDataId = startTimer(500);

    if (!_cappiReader.openFile(inputFile)) {
        std::cerr << "could not open " << inputFile << std::endl;
        exit(1);
    }
    

}
//////////////////////////////////////////////////////////////////////
EldoraCappi::~EldoraCappi() {
}

//////////////////////////////////////////////////////////////////////
void EldoraCappi::productSlot(
        std::vector<double> p, 
        int prodType, 
        qlonglong timeTag,
        StrMapDouble hskpMap) {
    
    PRODUCT_TYPES productType = (PRODUCT_TYPES) prodType;
    double gateSizeMeters = hskpMap["gateSpacingMeters"];
    double dwellWidth = hskpMap["dwellWidth"];
    double rotAngle = hskpMap["rotAngle"];
    double rollAngle = hskpMap["roll"];
    double airspdCorr = hskpMap["airSpdCorr"];
    double heading  = hskpMap["heading"];
    double radarTiltAngle = hskpMap["radarTiltAngle"];
    double groundSpeedEW = hskpMap["groundSweepEW"];
    double groundSpeedNS = hskpMap["groundSweepNS"];
    

    double adjustedAngle = rollAngle + rotAngle + _rollOffset;

     std::cerr << "EldoraCappi::productSlot() time ="
              << timeTag << " prodType = " <<prodType  << " p[0] =" << p[0] <<
        std::endl;   
    
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
        
        //        int nBeams = (int) nearbyint(360.0/_dwellWidth);
        _upperManager.configureCAPPI(_productList.size(), _gates,  _gateSizeMeters, 
                                   _left, _right, _bottom, _top);
#ifdef LOWER_WINDOW
        _lowerManager.configureCAPPI(_productList.size(), _gates,  _gateSizeMeters, 
                                   _left, _right, _bottom, _top);
#endif
        
        // display some of the vitals
        QString t;
        //        t.setNum(nBeams);
        // beamsText->setText(t);
        t.setNum(dwellWidth, 'f', 2);
        dwellWidthText->setText(t);
        t.setNum(_gates);
        gatesText->setText(t);
        
    }
    // apply the airspeed correction to VR
    if (productType == PROD_VR) {
        for (unsigned int i = 0; i < p.size(); i++)
            p[i] += airspdCorr;
    }
    
    // Map the product type into the zero based index for the CAPPIManager.
    int index = _productInfo[productType].getUserData();

    double corRotAngle = rotAngle + rollAngle;
    double cartAngle;
    // convert the Eldora heading and radarTitleAngle pointing angle to a
    // cartesian pointing angle
    // is the horizontal beam pointed to the right of the plane?
    if ((80.0 <= corRotAngle)  && (corRotAngle <= 100.0)) {
        cartAngle = heading + 90.0 - radarTiltAngle;
    } else {
        // no, it's pointing to the left side
        cartAngle = heading + 270.0 + radarTiltAngle;
    }
    
    if (cartAngle < 0.0) 
    	cartAngle += 360.0;
    if (cartAngle >= 360.0)
    	cartAngle -= 360.0;

    // compute relative position, based on ground speed
    if (_lastTime) {
        double secs = (timeTag - _lastTime)/1.0e6;  // microseconds to seconds
        _lastXKm += (groundSpeedNS * secs)/1.0e3;   // m to km
        _lastYKm += (groundSpeedEW * secs)/1.0e3;
    }
    _lastTime = timeTag;
    _lastProdType = prodType;
    
    
    

    // send the product to the appropriate ppi manager
    if (_upperManager.newProduct(p, _lastXKm, _lastYKm, cartAngle, index)) {
        	_rotAngle = cartAngle;
        	_rollAngle = rollAngle;
    }
#ifdef LOWER_WINDOW
    _lowerManager.newProduct(p, cartAngle, index);
#endif
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
        //ppiFore->saveImageToFile(saveNames[0].toStdString());
        //ppiAft->saveImageToFile(saveNames[0].toStdString());
        f = d.directory().absolutePath();
        _config.setString("imageSaveDirectory", f.toStdString());
    }
}

////////////////////////////////////////////////////////////////////
void EldoraCappi::initPlots() {

    int index = 0;
    //    setProductInfo(PROD_DM, index++, "DM", "DM", "Power", -60.0, 20.0);

    setProductInfo(PROD_DBZ, index++, "DBZ", "DBZ", "Reflectivity", -60.0, 20.0, true);
    setProductInfo(PROD_VR, index++, "VR", "VR", "Velocity (radial)", -30.0, 30.0);
#ifdef NOTDEF
    setProductInfo(PROD_VS, index++, "VS", "VS", "Velocity (short pulse)", -30.0, 30.0);
    setProductInfo(PROD_VL, index++, "VL", "VL", "Velocity (long pulse)", -30.0, 30.0);
    setProductInfo(PROD_SW, index++, "SW", "SW", "Spectral width", 0.0, 30.0);
    setProductInfo(PROD_NCP, index++, "NCP", "NCP", "Normalized coherent power", 0.0, 1.0);
#endif

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
#ifdef LOWER_WINDOW
//////////////////////////////////////////////////////////////////////
void EldoraCappi::colorBarLowerSlot() {
    colorBarPopup(false);
}
#endif
//////////////////////////////////////////////////////////////////////
void EldoraCappi::colorBarPopup(bool upper) {
    
    // get the current settings for the selected product in
    // the selected colormap.
    PRODUCT_TYPES prodType;
#ifdef LOWER_WINDOW
    if (upper)
        prodType = _prodTypeUpper;
    else
        prodType = _prodTypeLower;
#else
    prodType = _prodTypeUpper;
#endif
    
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
        //       if (prodType == _prodTypeUpper) 
            colorBarFor->configure(*_productMaps[index]);
            // if (prodType == _prodTypeLower) 
            // colorBarAft->configure(*_productMaps[index]);         

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
#ifdef LOWER_WINDOW
//////////////////////////////////////////////////////////////////////////////
void EldoraCappi::productTypeLowerSlot(int id) {
    // set the ppiType
    _prodTypeLower = (PRODUCT_TYPES)id;
    
    // get the _productsMap index
    int index = _productInfo[_prodTypeLower].getUserData();
    
    // inform the display
    _lowerManager.selectVar(index);
    
    // configure the color bar with it
    colorBarAft->configure(*_productMaps[index]);
}
//////////////////////////////////////////////////////////////////////////////
#endif
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
#ifdef LOWER_WINDOW
                             	productTypeLowerSlot(i->second.getId());
				QAbstractButton* button = 
					_lowerButtonGroup.button(i->second.getId());
				if(button)
					button->setChecked(true);
#endif
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
    //    ppiFor->rings(enabled);
    // ppiAft->rings(enabled);
}
///////////////////////////////////////////////////////////////////////

void EldoraCappi::gridSlot(bool enabled)
{
    // ppiFor->grids(enabled);
    // ppiAft->grids(enabled);
}
///////////////////////////////////////////////////////////////////////
void
EldoraCappi::backgroundColorSlot() 
{
    QColor color = QColorDialog::getColor("blue");

    // ppiFor->backgroundColor(color);
    // ppiAft->backgroundColor(color);
}

///////////////////////////////////////////////////////////////////////
void
EldoraCappi::ringColorSlot() 
{
    QColor color = QColorDialog::getColor("black");

    // ppiFor->gridRingsColor(color);
    // ppiAft->gridRingsColor(color);
}

///////////////////////////////////////////////////////////////////////
void
EldoraCappi::pan(double x, double y)
{
    //  ppiFor->pan(x, y);
    //   ppiAft->pan(x, y);
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
    // ppiFor->resetView();
    // ppiAft->resetView();
}
///////////////////////////////////////////////////////////////////////

void EldoraCappi::zoomInSlot()
{
    // ppiFor->setZoom(ppiFor->getZoom()*2.0);
    // ppiAft->setZoom(ppiAft->getZoom()*2.0);
}

///////////////////////////////////////////////////////////////////////

void EldoraCappi::zoomOutSlot()
{
    // ppiFor->setZoom(ppiFor->getZoom()*0.5);
    // ppiAft->setZoom(ppiAft->getZoom()*0.5);
}

///////////////////////////////////////////////////////////////////////
void EldoraCappi::cursorZoomSlot() {
    // ppiFor->cursorZoom();
    // ppiAft->cursorZoom();
}

///////////////////////////////////////////////////////////////////////
void EldoraCappi::cursorPanSlot() {
    // ppiFor->cursorPan();
    // ppiAft->cursorPan();
}


////
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
    int prodType;
    qlonglong timeTag;
    StrMapDouble hskpMap;
    std::vector<double> product;
    for (rec = _lastCappiRec; rec < lastRec; ++rec){
        std::cerr  << "reading record # " << rec << std::endl;
        ok = _cappiReader.read(rec, product, prodType, timeTag, hskpMap);
        if (ok) {
            // emit signal
            emit newProductSignal(product, prodType, timeTag, hskpMap);
        }
    }
    _lastCappiRec = lastRec-1;
    
}



