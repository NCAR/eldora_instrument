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
    QDialog(parent), _statsUpdateInterval(5),
            _config("NCAR", "EldoraPPI"),
            _paused(false), _gates(0){
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
//    connect(_saveImage, SIGNAL(released()), this, SLOT(saveImageSlot()));
//    connect(_pauseButton, SIGNAL(toggled(bool)), this, SLOT(pauseSlot(bool)));

    // set the checkbox selections
//    _pauseButton->setChecked(false);

    // initialize the book keeping for the plots.
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
    
    std::cout << "product from radar " << radarId << "\n";
    
    if (_paused)
        return;
    
    if (p.size() != _gates) {
        _gates = p.size();
        configurePPI();
    }
    
    if (radarId == 0){
        
    } else {
        
    }

}

//////////////////////////////////////////////////////////////////////
void EldoraPPI::configurePPI() {
    
    ppiFore->configure(6, _gates, 720, 0.100*2*_gates, 1);
    ppiAft->configure(6, _gates, 720, 0.100*2*_gates, 1);
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
void
EldoraPPI::initColorMaps() 
{

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
void 
EldoraPPI::setPpiInfo(PRODUCT_TYPES t, 
                   std::string key,             
                   std::string shortName,       
                   std::string longName,        
                   double defaultScaleMin,      
                   double defaultScaleMax,      
                   int ppiVarIndex)
{
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

    // set the ppi configuration
    _ppiInfo[t] = PpiInfo(t, key, shortName, longName, mapName, min, max, ppiVarIndex);

    _config.sync();

}

