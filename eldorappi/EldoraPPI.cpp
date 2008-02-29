#include "EldoraPPI.h"
#include "PPI.h"
#include "SvnVersion.h"

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
#include <string>
#include <algorithm>

#include <iostream>
#include <time.h>

//////////////////////////////////////////////////////////////////////
EldoraPPI::EldoraPPI(
        QDialog* parent) :
    QDialog(parent), _prodTypeFor(PROD_DBZ), _prodTypeAft(PROD_DBZ), 
    _statsUpdateInterval(5), _config("NCAR", "EldoraPPI"), _paused(false), 
    _gates(0), _forwardElevation(0.0), _aftElevation(0.0)
    {
    // Set up our form
    setupUi(parent);

    // configure pause
    _paused = pause->isChecked();
    
    // get our title from the coniguration
    std::string title = _config.getString("title", "EldoraPPI");
    title += " ";
    title += SvnVersion::revision();
    parent->setWindowTitle(title.c_str());

    // initialize running statistics
    for (int i = 0; i < 3; i++) {
        _errorCount[i] = 0;
        _lastPulseNum[i] = 0;
    }

    // fill in the product list
    _productList.insert(PROD_DM);
    _productList.insert(PROD_DBZ);
    _productList.insert(PROD_VR);
    _productList.insert(PROD_VS);
    _productList.insert(PROD_VL);
    _productList.insert(PROD_SW);
    _productList.insert(PROD_NCP);

    // connect the controls
    
    // The color bar popup
    connect(colorBarFor, SIGNAL(released()), this, SLOT(colorBarForSlot()));
    connect(colorBarAft, SIGNAL(released()), this, SLOT(colorBarAftSlot()));

    // product selection buttons
    connect(&_forwardButtonGroup, SIGNAL(buttonReleased(int)), this, SLOT(productTypeForSlot(int)));
    connect(&_aftButtonGroup, SIGNAL(buttonReleased(int)), this, SLOT(productTypeAftSlot(int)));
    
    //    connect(_saveImage, SIGNAL(released()), this, SLOT(saveImageSlot()));
    connect(pause, SIGNAL(toggled(bool)), this, SLOT(pauseSlot(bool)));

    // set the checkbox selections
    //    _pauseButton->setChecked(false);

    // initialize the color maps
    initColorMaps();

    // initialize the book keeping for the ppi displays.
    initPlots();

    // configure the displays themselves.
    int decimation = _config.getInt("Decimation", 1);
    _forManager.setup(ppiFor, 7, &_productMaps, decimation);
    _aftManager.setup(ppiAft, 7, &_productMaps, decimation);
    
    // run through all of the product types and configure the 
    // the for and aft displays.
    for (std::set<PRODUCT_TYPES>::iterator i = _productList.begin(); 
    i != _productList.end(); i++){
        productTypeForSlot(*i);
        productTypeAftSlot(*i);
    }
    // and then set the initial display
    productTypeForSlot(PROD_DBZ);
    productTypeAftSlot(PROD_DBZ);
   
    // start the statistics timer
    startTimer(100);

}
//////////////////////////////////////////////////////////////////////
EldoraPPI::~EldoraPPI() {
}

//////////////////////////////////////////////////////////////////////
void EldoraPPI::productSlot(
        std::vector<double> p, int radarId, float elDegrees, int prodType) {
    
    PRODUCT_TYPES productType = (PRODUCT_TYPES) prodType;
  
    // ignore products that we are not interested in. Of course, the 
    // caller is not supposed to send them to us, but perform a
    // sanity check here anyway.
    if (_productList.find(productType) == _productList.end())
        return;
    
    // ignore product if we are paused.
    if (_paused)
        return;
    
    // if the product size has changed, reconfigure the ppi displays
    if (p.size() != _gates) {
        _gates = p.size();
        _forManager.configurePPI(_productList.size(), _gates, 400);
        _aftManager.configurePPI(_productList.size(), _gates, 400);
    }

    // Map the product type into the zero based index for the PPIManager.
    int index = _productInfo[productType].getUserData();
    
    // send the product to the appropriate ppi manager
    if (_forManager.newProduct(p, elDegrees, index))
        	_forwardElevation = elDegrees;
    if (_aftManager.newProduct(p, elDegrees, index))
        	_aftElevation = elDegrees;
}


//////////////////////////////////////////////////////////////////////
void EldoraPPI::saveImageSlot() {
    QString f = _config.getString("imageSaveDirectory", "c:/").c_str();

    QFileDialog d( this, tr("Save EldoraPPI Image"), f,
            tr("PNG files (*.png);;All files (*.*)"));
    d.setFileMode(QFileDialog::AnyFile);
    d.setViewMode(QFileDialog::Detail);
    d.setAcceptMode(QFileDialog::AcceptSave);
    d.setConfirmOverwrite(true);
    d.setDefaultSuffix("png");
    d.setDirectory(f);

    f = "EldoraPPI-";
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
void EldoraPPI::initPlots() {

    int index = 0;
    setProductInfo(PROD_DM, index++, "DM", "DM", "Power", -60.0, 20.0);
    setProductInfo(PROD_DBZ, index++, "DBZ", "DBZ", "Reflectivity", -60.0, 20.0, true);
    setProductInfo(PROD_VR, index++, "VR", "VR", "Velocity (radial)", -30.0, 30.0);
    setProductInfo(PROD_VS, index++, "VS", "VS", "Velocity (short pulse)", -30.0, 30.0);
    setProductInfo(PROD_VL, index++, "VL", "VL", "Velocity (long pulse)", -30.0, 30.0);
    setProductInfo(PROD_SW, index++, "SW", "SW", "Spectral width", 0.0, 30.0);
    setProductInfo(PROD_NCP, index++, "NCP", "NCP", "Normalized coherent power", 0.0, 1.0);

    // The buttons were created and assigned to layouts in setPpiInfo.
    // Attach these layouts to the button groups
    forwardGroupBox->setLayout(&_forwardVBox);
    aftGroupBox->setLayout(&_aftVBox);

}

//////////////////////////////////////////////////////////////////////
void EldoraPPI::timerEvent(
        QTimerEvent*) {

    QString angle;
    angle.setNum(_forwardElevation, 'f', 1);
    forElev->setText(angle);
    angle.setNum(_aftElevation, 'f', 1);
    aftElev->setText(angle);
 
}

//////////////////////////////////////////////////////////////////////
void EldoraPPI::pauseSlot(
        bool p) {
    _paused = p;
}

//////////////////////////////////////////////////////////////////////
void EldoraPPI::initColorMaps() {

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
void EldoraPPI::setProductInfo(
        PRODUCT_TYPES t, int index, std::string key, std::string shortName,
        std::string longName, double defaultScaleMin, double defaultScaleMax,
        bool setChecked) {
    
    // Add this product to our list of products. The set
    // will allow for rapid determination of whther a product is one 
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
    _forwardVBox.addWidget(button);
    _forwardButtonGroup.addButton(button);
    _forwardButtonGroup.setId(button, t);

    // aft button
    button = new QRadioButton(tr(shortName.c_str()));
    if (setChecked)
        button->setChecked(true);
    button->setToolTip(longName.c_str());
    _aftVBox.addWidget(button);
    _aftButtonGroup.addButton(button);
    _aftButtonGroup.setId(button, t);
    
}
//////////////////////////////////////////////////////////////////////
void EldoraPPI::colorBarForSlot() {
    colorBarPopup(true);
}
//////////////////////////////////////////////////////////////////////
void EldoraPPI::colorBarAftSlot() {
    colorBarPopup(false);
}
//////////////////////////////////////////////////////////////////////
void EldoraPPI::colorBarPopup(bool forwardRadar) {
    
    // get the current settings for the selected product in
    // the selected colormap.
    PRODUCT_TYPES prodType;
    if (forwardRadar)
        prodType = _prodTypeFor;
    else
        prodType = _prodTypeAft;
    
    double min = _productInfo[prodType].getScaleMin();
    double max = _productInfo[prodType].getScaleMax();
    std::string currentName = _productInfo[prodType].getColorMapName();

    // create the color bar settings dialog
    std::vector<std::string> mapNames;
    for (std::map<std::string, ColorMap>::iterator i = _colorMaps.begin(); i
            != _colorMaps.end(); i++) {
        mapNames.push_back(i->first);
    }
    _colorBarSettings = new ColorBarSettings(min, max, currentName, mapNames, forwardRadar, this);

    // connect the finished slot so that the dialog status 
    // can be captuyred when the dialog closes
    connect(_colorBarSettings, SIGNAL(finished(int)), this, SLOT(colorBarSettingsFinishedSlot(int)));

    // and show it
    _colorBarSettings->show();

}
//////////////////////////////////////////////////////////////////////////////
void EldoraPPI::colorBarSettingsFinishedSlot(
        int result) {
    // see if the OK button was hit
    if (result == QDialog::Accepted) {
        
        // which colorbar are we aorking with?
        PRODUCT_TYPES prodType;
        if (_colorBarSettings->forwardRadar())
            prodType = _prodTypeFor;
        else
            prodType = _prodTypeAft;
        
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
        if (prodType == _prodTypeFor) 
            colorBarFor->configure(*_productMaps[index]);
        if (prodType == _prodTypeAft) 
            colorBarAft->configure(*_productMaps[index]);         

        // assign the new scale values to the current product
        _productInfo[_prodTypeFor].setScale(scaleMin, scaleMax);
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
void EldoraPPI::productTypeForSlot(int id) {
    // set the ppiType
    _prodTypeFor = (PRODUCT_TYPES)id;
    
    // get the _productsMap index
    int index = _productInfo[_prodTypeFor].getUserData();
    
    // inform the display
    _forManager.selectVar(index);
    
    // configure the color bar with it
    colorBarFor->configure(*_productMaps[index]);
}
//////////////////////////////////////////////////////////////////////////////
void EldoraPPI::productTypeAftSlot(int id) {
    // set the ppiType
    _prodTypeAft = (PRODUCT_TYPES)id;
    
    // get the _productsMap index
    int index = _productInfo[_prodTypeAft].getUserData();
    
    // inform the display
    _aftManager.selectVar(index);
    
    // configure the color bar with it
    colorBarAft->configure(*_productMaps[index]);
}


