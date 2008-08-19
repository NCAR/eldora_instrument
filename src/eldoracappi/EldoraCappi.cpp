#include "EldoraCappi.h"
#include "CAPPI.h"
#include "Z.xpm"
#include "Paw.xpm"


//////////////////////////////////////////////////////////////////////
EldoraCappi::EldoraCappi(std::string inputFile, std::string title,
		QDialog* parent) :
	QDialog(parent), _statsUpdateInterval(5), _config("NCAR", "EldoraCappi"),
			_gates(0), _gateSizeDeg(0.0), _dwellWidth(0), _lastCappiRec(-1),
			_rollOffset(0.0), _lat(0.0),
			_lon(0.0), _firstPos(true), _firstTimer(true),
			_disableTimer(false){
	// Set up our form
	setupUi(parent);

	// create the CAPPI display
	_cappi = new CAPPI(cappiFrame);

	// set up the layout management for the CAPPI
	QVBoxLayout* l = new QVBoxLayout;
	l->addWidget(_cappi);
	cappiFrame->setLayout(l);

	// create the time controller, inserting into the 
	// box provided by the form.
	_cappiTime = new CappiTime(this->timeBox);
	
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
	connect(colorBar, SIGNAL(released()), this, SLOT(colorBarSlot()));

	// product selection buttons
	connect(&_buttonGroup, SIGNAL(buttonReleased(int)), this, SLOT(productTypeSlot(int)));

	connect(gridCheck, SIGNAL(clicked(bool)), this, SLOT(gridSlot(bool)));
	connect(zoomIn, SIGNAL(released()), this, SLOT(zoomInSlot()));
	connect(zoomOut, SIGNAL(released()), this, SLOT(zoomOutSlot()));
	connect(backgroundButton, SIGNAL(released()), this, SLOT(backgroundColorSlot()));
	connect(gridColorButton, SIGNAL(released()), this, SLOT(gridColorSlot()));
	connect(panUp, SIGNAL(released()), this, SLOT(panUpSlot()));
	connect(panDown, SIGNAL(released()), this, SLOT(panDownSlot()));
	connect(panLeft, SIGNAL(released()), this, SLOT(panLeftSlot()));
	connect(panRight, SIGNAL(released()), this, SLOT(panRightSlot()));
	connect(viewReset, SIGNAL(released()), this, SLOT(resetViewSlot()));
	connect(saveImage, SIGNAL(released()), this, SLOT(saveImageSlot()));
	connect(zoomButton, SIGNAL(released()), this, SLOT(cursorZoomSlot()));
	connect(panButton, SIGNAL(released()), this, SLOT(cursorPanSlot()));
	connect(_cappi, SIGNAL(gridDelta(double)), this, SLOT(gridDeltaSlot(double)));
	
	connect(_cappiTime, SIGNAL(apply(CappiTime::MODE, ptime, ptime)), this, SLOT(setTimeSlot(CappiTime::MODE, ptime, ptime)));

	// initialize the color maps
	initColorMaps();

	// initialize the book keeping for the ppi displays.
	initPlots();

	// setup the displays themselves.
	int decimation = _config.getInt("Decimation", 1);
	int ppiHeight = _config.getInt("Size/MinHeight", 300);
	_manager.setup(_cappi, _productMaps.size(), &_productMaps, decimation,
			ppiHeight);

	// get the display specifications
	_imageTitle = _config.getString("ImageTitle", "NCAR/Eldora");
	
	double timeSpanHr = _config.getDouble("Display/TimeSpanHrs", 0.25);
	_timeSpan = time_duration(seconds((int)(timeSpanHr*3600.0)));
	
	_spanDeg = _config.getDouble("Display/SpanDeg", 10.0);
	_stripDisplay = _config.getBool("Display/StripDisplay", true);
	_stripWidthDeg = _config.getDouble("Display/StripWidthKm", 2.2)/111.2;

	// and the radar specs that are provided in our configuration:
	_radarTiltAngle = _config.getDouble("Radar/TiltAngleDegs", 15.57);

	// run through all of the product types and configure the 
	// the for and aft displays.
	for (std::set<PRODUCT_TYPES>::iterator i = _productList.begin(); i
			!= _productList.end(); i++) {
		productTypeSlot(*i);
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
	}

	// and then set the initial display
	productTypeSlot(PROD_DBZ);

	// specify cursor zooming
	cursorZoomSlot();

	if (!_cappiReader.openFile(inputFile)) {
		std::cerr << "could not open " << inputFile << std::endl;
		exit(1);
	}

	// set the starting limits
	getTimeLimits();
	
	// put us in realtime mode, to display the most recent data
	_mode = CappiTime::REALTIME;
	
	// start the statistics timer
	_statusTimerId = startTimer(5000);

	// start the data polling timer
	_checkNewDataId = startTimer(100);
}

//////////////////////////////////////////////////////////////////////
EldoraCappi::~EldoraCappi() {
}

//////////////////////////////////////////////////////////////////////
void EldoraCappi::getTimeLimits() {
	
	unsigned long lastRec;
	bool ok;
	ok = _cappiReader.findLastRecord(lastRec);
	if (!ok) {
		std::cerr << "findLastRecord failed" << std::endl;
		return;
	}
	
	if (!_cappiReader.getTime(lastRec, _latestTime)) {
		std::cerr << "unable to get time of last record\n";
		return;
	}
	
	if (!_cappiReader.getTime(0, _earliestTime)) {
		std::cerr << "unable to get time of first record\n";
		return;
	}
	
	_stopTime = _latestTime;
	_startTime = _stopTime - _timeSpan;
	
	// configure the capi time information
	_cappiTime->setLimits(_earliestTime, _latestTime, _timeSpan);
	_cappiTime->setStartTime(_startTime);
	_cappiTime->setStopTime(_stopTime);

}

//////////////////////////////////////////////////////////////////////
void EldoraCappi::productSlot(std::vector<double> p, unsigned long rec, int prodType,
		ptime timeTag, StrMapDouble hskpMap) {

	PRODUCT_TYPES productType = (PRODUCT_TYPES) prodType;

	// is this an angle we want to look at?
	double adjustedAngle = hskpMap["roll"] + hskpMap["rotAngle"] + _rollOffset;
	double dwellWidth = hskpMap["dwellWidth"];

	if (! flteq(adjustedAngle, 90.0, dwellWidth/2.0) && ! ( flteq(adjustedAngle, 270.0, dwellWidth/2.0)))
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

	_lat = hskpMap["lat"];
	_lon = hskpMap["lon"];

	/// @todo Be prepared to implement a different Q/C check
	/// for position as necessary.
	/// don't accept positions where lat/lon are close to zero/zero. This
	/// is to weed out cases where the position feed (INS?) is not
	/// working. I'm just making a guess at this right now.
	if (fabs(_lat)< 0.1 && fabs(_lon) < 0.1) {
		return;
	}

	if (_firstPos) {
		_firstPos = false;
		_firstLat = floor(_lat+0.5);
		_firstLon = floor(_lon+0.5);
	}

	// if the product size has changed, reconfigure the ppi displays
	double gateSizeDeg = hskpMap["gateSpacingMeters"]/111.2/1000.0;
	if (p.size() != _gates || gateSizeDeg != _gateSizeDeg) {
		// for some very strange reason, if this print is not here, then all zooms 
		// cause a view reset which negates the zoom!
		std::cout << "configure " << _gates << "  " << _gateSizeDeg << "\n";
		
		_gates = p.size();
		_gateSizeDeg = gateSizeDeg;
		_manager.configureCAPPI(_productList.size(), _gates, _gateSizeDeg,
				_spanDeg, _stripDisplay, _stripWidthDeg, _firstLon, _firstLat,
				seconds(_timeSpan.seconds()));
	}

	// apply the airspeed correction to VR
	/// @todo vr correction not done for cappi?
	//double airspdCorr = hskpMap["airSpdCorr"];
	//if (productType == PROD_VR) {
	//    for (unsigned int i = 0; i < p.size(); i++)
	//        p[i] += airspdCorr;
	//}

	// Map the product type into the zero based index for the CAPPIManager.
	int index = _productInfo[productType].getUserData();

	_lastTime = timeTag;
	_lastProdType = prodType;

	double cartAngle = pointingAngle(adjustedAngle, hskpMap["heading"],
			_radarTiltAngle);

	// send the product to the appropriate ppi manager
	_manager.newProduct(p, timeTag, (_lon - _firstLon), (_lat - _firstLat),
			cartAngle, index);
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
		QImage* image = _cappi->getImage();
		if (image) {
			std::ostringstream s;
			time_facet* timeFacet = new time_facet("%m-%d-%y %H:%M");
			std::ostringstream imageString;
			imageString.imbue(std::locale(std::locale::classic(), timeFacet));
			
			QPainter painter(image);
			painter.setPen(Qt::black);
			painter.setFont(QFont("Arial", 14));
			imageString << " " << _startTime << "   " << _stopTime;
			painter.drawText(image->rect(), Qt::AlignRight | Qt::AlignTop, 
					imageString.str().c_str());
			imageString.str("");
			imageString << _imageTitle << " ";
			painter.drawText(image->rect(), Qt::AlignLeft | Qt::AlignTop, 
					imageString.str().c_str());
			painter.end();
			image->save(saveNames[0].toStdString().c_str(), "PNG", 100);
			delete image;
		}
		f = d.directory().absolutePath();
		_config.setString("ImageSaveDirectory", f.toStdString());
	}
}

////////////////////////////////////////////////////////////////////
void EldoraCappi::initPlots() {

	int index = 0;
	setProductInfo(PROD_DBZ, index++, "DBZ", "DBZ", "Reflectivity", -60.0,
			20.0, true);
	setProductInfo(PROD_VR, index++, "VR", "VR", "Velocity (radial)", -30.0,
			30.0);
	gridSlot(true);

}

//////////////////////////////////////////////////////////////////////
void EldoraCappi::timerEvent(QTimerEvent*event) {

	if (_statusTimerId == event->timerId()) {
		QString tlat = QString("%1").arg(_lat, 0, 'f', 2);
		this->latText->setText(tlat);
		QString tlon = QString("%1").arg(_lon, 0, 'f', 2);
		this->lonText->setText(tlon);
	} else if (_checkNewDataId == event->timerId()) {
		if (_mode == CappiTime::REALTIME && !_disableTimer) {
			pollNewData();
		}
		// if this is the first time the timer has been called,
		// reset it to a 5 second interval
		if (_firstTimer) {
			_firstTimer = false;
			killTimer(_checkNewDataId);
			_checkNewDataId = startTimer(5000);
		}
	}
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
void EldoraCappi::setProductInfo(PRODUCT_TYPES t, int index, std::string key,
		std::string shortName, std::string longName, double defaultScaleMin,
		double defaultScaleMax, bool setChecked) {

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
	_productInfo[t] = ProductInfo(t, index, key, shortName, longName, mapName,
			min, max);

	// synchronize the configuration. But why here?
	_config.sync();

	// Put a new map into the color map vector. Remember that the 
	// location of the color map in _productMaps will correspond
	// to the product entry in _productList
	ColorMap* map = new ColorMap(_colorMaps[mapName]);
	map->setRange(min, max);
	_productMaps.push_back(map);

	// create the product selection buttons
	QRadioButton* button;

	// button
	button = new QRadioButton(tr(shortName.c_str()));
	if (setChecked)
		button->setChecked(true);
	button->setToolTip(longName.c_str());
	_vBox.addWidget(button);
	_buttonGroup.addButton(button);
	_buttonGroup.setId(button, t);
}

//////////////////////////////////////////////////////////////////////
void EldoraCappi::colorBarSlot() {

	// get the current settings for the selected product in
	// the selected colormap.
	PRODUCT_TYPES prodType;
	prodType = _prodType;

	double min = _productInfo[prodType].getScaleMin();
	double max = _productInfo[prodType].getScaleMax();
	std::string currentName = _productInfo[prodType].getColorMapName();

	// create the color bar settings dialog
	std::vector<std::string> mapNames;
	for (std::map<std::string, ColorMap>::iterator i = _colorMaps.begin(); i
			!= _colorMaps.end(); i++) {
		mapNames.push_back(i->first);
	}

	_colorBarSettings = new ColorBarSettings(min, max, currentName, mapNames, this);

	// connect the finished slot so that the dialog status 
	// can be captuyred when the dialog closes
	connect(_colorBarSettings, SIGNAL(finished(int)), this, SLOT(colorBarSettingsFinishedSlot(int)));

	// and show it
	_colorBarSettings->show();
}

//////////////////////////////////////////////////////////////////////////////
void EldoraCappi::colorBarSettingsFinishedSlot(int result) {

	// see if the OK button was hit
	if (result == QDialog::Accepted) {

		PRODUCT_TYPES prodType;
		prodType = _prodType;

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
		colorBar->configure(*_productMaps[index]);

		// assign the new scale values to the current product
		_productInfo[_prodType].setScale(scaleMin, scaleMax);
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

		// get rid of all of the existing beams
		_cappi->clearDisplay();

		// force a redraw
		_lastCappiRec = 0;

		// redraw
		pollNewData();

	}
}

//////////////////////////////////////////////////////////////////////////////
void EldoraCappi::productTypeSlot(int id) {
	// set the ppiType
	_prodType = (PRODUCT_TYPES)id;

	// get the _productsMap index
	int index = _productInfo[_prodType].getUserData();

	// inform the display
	_manager.selectVar(index);

	// configure the color bar with it
	colorBar->configure(*_productMaps[index]);
}

//////////////////////////////////////////////////////////////////////////////
void EldoraCappi::fkeyTriggered(QAction* qa) {

	// ignore keys that we are not expecting

	int fkey = qa->shortcut() - Qt::Key_F1;
	if (fkey >= 0 && fkey < (int)_productList.size()) {
		// Got a function key. See if this product index exists in _productInfo
		for (std::map<PRODUCT_TYPES, ProductInfo>::iterator i =
				_productInfo.begin(); i != _productInfo.end(); i++) {
			if (i->second.getUserData() == fkey) {
				// matched it
				productTypeSlot(i->second.getId());
				QAbstractButton* button =
						_buttonGroup.button(i->second.getId());
				if (button)
					button->setChecked(true);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////

void EldoraCappi::gridSlot(bool enabled) {
	_cappi->grids(enabled);
}

///////////////////////////////////////////////////////////////////////
void EldoraCappi::backgroundColorSlot() {
	QColor color = QColorDialog::getColor("blue");
	_cappi->backgroundColor(color);
}

///////////////////////////////////////////////////////////////////////
void EldoraCappi::gridColorSlot() {
	QColor color = QColorDialog::getColor("black");
	_cappi->gridColor(color);
}
///////////////////////////////////////////////////////////////////////
void EldoraCappi::pan(double x, double y) {
	_cappi->pan(x, y);
}
///////////////////////////////////////////////////////////////////////
void EldoraCappi::panUpSlot() {
	pan(0.0, 0.1);
}
///////////////////////////////////////////////////////////////////////
void EldoraCappi::panDownSlot() {
	pan(0.0, -0.1);
}
///////////////////////////////////////////////////////////////////////
void EldoraCappi::panLeftSlot() {
	pan(-0.1, 0.0);
}
///////////////////////////////////////////////////////////////////////
void EldoraCappi::panRightSlot() {
	pan(0.1, 0.0);
}
///////////////////////////////////////////////////////////////////////
void EldoraCappi::resetViewSlot() {
	_cappi->resetView();
}
///////////////////////////////////////////////////////////////////////
void EldoraCappi::zoomInSlot() {
	_cappi->setZoom(_cappi->getZoom()*2.0);
}
///////////////////////////////////////////////////////////////////////
void EldoraCappi::zoomOutSlot() {
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
void EldoraCappi::gridDeltaSlot(double deltaDeg) {
	this->gridText->setNum(deltaDeg);
}	
///////////////////////////////////////////////////////////////////////
void EldoraCappi::setTimeSlot(CappiTime::MODE mode, ptime startTime, ptime stopTime) {

	_disableTimer = true;
	
	_startTime = startTime;
	_stopTime = stopTime;
	_mode = mode;
	_lastCappiRec = 0;
	_cappi->clearDisplay();
	pollNewData();
	
	_disableTimer = false;
}

///////////////////////////////////////////////////////////////////////
void EldoraCappi::pollNewData() {
	
	unsigned long lastRec;
	ptime recordTime;
	bool ok;
	
	ok = _cappiReader.findLastRecord(lastRec);
	if (!ok) {
		std::cerr << "findLastRecord failed" << std::endl;
		return;
	}
	
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	
	for (unsigned long rec = _lastCappiRec+1; rec <= lastRec; ++rec) {
		if (!_cappiReader.getTime(rec, recordTime)) {
			std::cerr << "unable to get tme for record " << rec << "\n";
			continue;
		}
		if ( 
			(recordTime >= _startTime && _mode == CappiTime::REALTIME) ||
			(_mode == CappiTime::FIXED && recordTime >= _startTime && recordTime <= _stopTime))
		{
			displayRecord(rec);
		} 
		
	}
	
	// In real time mode, update the end of time in the time controller
	if (_mode == CappiTime::REALTIME && recordTime != not_a_date_time) {
		_latestTime = recordTime;
		_stopTime = _latestTime;
		_cappiTime->setLimits(_earliestTime, _latestTime, _timeSpan);
		_cappiTime->setStopTime(_stopTime);
	}
	
	_lastCappiRec = lastRec;
	
	QApplication::restoreOverrideCursor();

}
///////////////////////////////////////////////////////////////////////
void EldoraCappi::displayRecord(unsigned long rec) {
	int prodType;
	ptime timeTag;
	StrMapDouble hskpMap;
	std::vector<double> product;
	bool ok = _cappiReader.read(rec, product, prodType, timeTag, hskpMap);
	if (ok) {
		this->productSlot(product, rec, prodType, timeTag, hskpMap);
	} else {
		std::cerr << "unable to get record " << rec << "\n";
	}
	QCoreApplication::processEvents();
}

///////////////////////////////////////////////////////////////////////
double EldoraCappi::pointingAngle(double rotAngle, double heading,
		double tiltAngle) {

	double cartAngle;

	cartAngle = 450 - heading;
	// is the horizontal beam pointed to the right of the plane?
	if ((80.0 <= rotAngle) && (rotAngle <= 100.0)) {
		cartAngle = cartAngle - 90 + tiltAngle;
	} else {
		// no, it's pointing to the left side
		cartAngle = cartAngle + 90 - tiltAngle;
	}

	while (cartAngle < 0.0)
		cartAngle += 360.0;
	while (cartAngle >= 360.0)
		cartAngle -= 360.0;

	return cartAngle;
}
