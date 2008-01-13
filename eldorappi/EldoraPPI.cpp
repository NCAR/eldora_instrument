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
    QDialog(parent), _statsUpdateInterval(5), _config("NCAR", "EldoraPPI"),
            _paused(false), _gates(0), _ppiType(PROD_DBZ) {
    // Set up our form
    setupUi(parent);

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

    // connect the controls
    
    // The color bar popup
    connect(colorBarFore, SIGNAL(released()), this, SLOT(colorBarReleasedSlot()));
    connect(colorBarAft, SIGNAL(released()), this, SLOT(colorBarReleasedSlot()));

    // product selection buttons
    connect(&_forwardButtonGroup, SIGNAL(buttonReleased(int)), this, SLOT(productTypeSlotFore(int)));
    connect(&_aftButtonGroup, SIGNAL(buttonReleased(int)), this, SLOT(productTypeSlotAft(int)));
    
    //    connect(_saveImage, SIGNAL(released()), this, SLOT(saveImageSlot()));
    //    connect(_pauseButton, SIGNAL(toggled(bool)), this, SLOT(pauseSlot(bool)));

    // set the checkbox selections
    //    _pauseButton->setChecked(false);

    // initialize the color maps
    initColorMaps();

    // initialize the book keeping for the ppi displays.
    initPlots();

    // start the statistics timer
    startTimer(_statsUpdateInterval*1000);

    // let the data sources get themselves ready
    sleep(1);

}
//////////////////////////////////////////////////////////////////////
EldoraPPI::~EldoraPPI() {
}

//////////////////////////////////////////////////////////////////////
void EldoraPPI::productSlot(
        std::vector<double> p, int radarId) {

    if (_paused)
        return;

    if (p.size() != _gates) {
        _gates = p.size();
        configurePPI();
    }

    if (radarId == 0) {

    } else {

    }

}

//////////////////////////////////////////////////////////////////////
void EldoraPPI::configurePPI() {

    ppiForward->configure(7, _gates, 720, 0.100*2*_gates, 1);
    ppiAft->configure(7, _gates, 720, 0.100*2*_gates, 1);
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
    setPpiInfo(PROD_DM, index++, "DM", "DM", "Power", -60.0, 20.0);
    setPpiInfo(PROD_DBZ, index++, "DBZ", "DBZ", "Reflectivity", -60.0, 20.0, true);
    setPpiInfo(PROD_VR, index++, "VR", "VR", "Velocity (radial)", -30.0, 30.0);
    setPpiInfo(PROD_VS, index++, "VS", "VS", "Velocity (short pulse)", -30.0, 30.0);
    setPpiInfo(PROD_VL, index++, "VL", "VL", "Velocity (long pulse)", -30.0, 30.0);
    setPpiInfo(PROD_SW, index++, "SW", "SW", "Spectral width", 0.0, 30.0);
    setPpiInfo(PROD_NCP, index++, "NCP", "NCP", "Normalized coherent power", 0.0, 1.0);

    // The buttons were created and assigned to layouts in setPpiInfo.
    // Attach these layouts to the button groups
    forwardGroupBox->setLayout(&_forwardVBox);
    aftGroupBox->setLayout(&_aftVBox);

}

//////////////////////////////////////////////////////////////////////
void EldoraPPI::timerEvent(
        QTimerEvent*) {

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
    for (int i = 0; i < mapNames.size(); i++) {
        _colorMaps[mapNames[i]] = ColorMap(0.0, 1.0, mapNames[i]);
    }

    // get the names of all existing colormaps saved in the configuration.
    std::vector<std::string> names = _config.childGroups("ColorMaps");

    std::vector<std::vector<int> > emptyMap;
    // save colormaps for each of these choices.
    for (int i = 0; i < names.size(); i++) {
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
void EldoraPPI::setPpiInfo(
        PRODUCT_TYPES t, int index, std::string key, std::string shortName,
        std::string longName, double defaultScaleMin, double defaultScaleMax,
        bool setChecked) {
    // create the configuration keys
    std::string minKey = key;
    minKey += "/min";

    std::string maxKey = key;
    maxKey += "/max";

    std::string mapKey = key;
    mapKey += "/colorMap";

    // get the configuration values
    double min = _config.getDouble(minKey, defaultScaleMin);
    double max = _config.getDouble(maxKey, defaultScaleMax);
    std::string mapName = _config.getString(mapKey, "default");
    if (_colorMaps.find(mapName) == _colorMaps.end()) {
        mapName = "default";
    }

    _productList.insert(t);
    // set the ppi configuration
    _ppiInfo[t]
            = PpiInfo(t, index, key, shortName, longName, mapName, min, max);

    _config.sync();

    // Assign the color map, by copying one of our base maps.
    ColorMap* map = new ColorMap(_colorMaps[mapName]);
    std::cout << "_productMaps for index " << index << " is " << map << "\n";
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
void EldoraPPI::colorBarReleasedSlot() {
    // get the current settings
    double min = _ppiInfo[_ppiType].getScaleMin();
    double max = _ppiInfo[_ppiType].getScaleMax();
    std::string currentName = _ppiInfo[_ppiType].getColorMapName();

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
void EldoraPPI::colorBarSettingsFinishedSlot(
        int result) {
    // see if the OK button was hit
    if (result == QDialog::Accepted) {
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
        _ppiInfo[_ppiType].setColorMapName(newMapName);

        // configure the color bar with the new map and ranges.
        // first figure out whch entry in the _productMaps vector
        // corresponds to this product.
        int index = _ppiInfo[_ppiType].getUserData();
        // get rid of the existing map
        delete _productMaps[index];
        // create a new map
        ColorMap* newMap = new ColorMap(_colorMaps[newMapName]);
        _productMaps[index] = newMap;
        // set range on the new color map
        _productMaps[index]->setRange(scaleMin, scaleMax);
        // configure the color bar with it
        colorBarFore->configure(*_productMaps[index]);

        // assign the new scale values to the current product
        _ppiInfo[_ppiType].setScale(scaleMin, scaleMax);
        // save the new values in the configuration
        // create the configuration keys
        std::string key = _ppiInfo[_ppiType].getKey();
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
void EldoraPPI::productTypeSlotFore(int id) {
    // set the ppiType
    _ppiType = (PRODUCT_TYPES)id;
    
    // get the _productsMap index
    int index = _ppiInfo[_ppiType].getUserData();
    
    // configure the color bar with it
    colorBarFore->configure(*_productMaps[index]);
}
//////////////////////////////////////////////////////////////////////////////
void EldoraPPI::productTypeSlotAft(int id) {
    // set the ppiType
    _ppiType = (PRODUCT_TYPES)id;
    
    // get the _productsMap index
    int index = _ppiInfo[_ppiType].getUserData();
    
    // configure the color bar with it
    colorBarAft->configure(*_productMaps[index]);
}


