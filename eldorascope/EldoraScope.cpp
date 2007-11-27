#include "EldoraScope.h"
#include "ScopePlot.h"
#include "Knob.h"

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

#include <qwt_wheel.h>

//////////////////////////////////////////////////////////////////////
EldoraScope::EldoraScope(
        QDialog* parent) :
    QDialog(parent), _performAutoScale(false), _statsUpdateInterval(5),
            _timeSeriesPlot(TRUE), _config("NCAR", "EldoraScope"),
            _paused(false), _gateMode(ALONG_BEAM), _zeroMoment(0.0) {
    // Set up our form
    setupUi(parent);

    // Initialize fft calculations
    initFFT();

    // get our title from the coniguration
    std::string title = _config.getString("title", "EldoraScope");
    title += " ";
    //title += CP2Version::revision();
    parent->setWindowTitle(title.c_str());

    // initialize running statistics
    for (int i = 0; i < 3; i++) {
        //		_pulseCount[i]	= 0;
        //		_prevPulseCount[i] = 0;
        _errorCount[i] = 0;
        _lastPulseNum[i] = 0;
    }

    // configure the gate mode selection
    _alongBeam->setChecked(true);
    _oneGate->setChecked(false);
    QButtonGroup* gateModeButtonGroup = new QButtonGroup();
    gateModeButtonGroup->addButton(_alongBeam, ALONG_BEAM);
    gateModeButtonGroup->addButton(_oneGate, ONE_GATE);

    // configure the channel selections
    _channel = 1;
    _chan1->setChecked(true);
    _chan2->setChecked(false);
    _chan3->setChecked(false);
    _chan4->setChecked(false);
    QButtonGroup* channelButtonGroup = new QButtonGroup();
    channelButtonGroup->addButton(_chan1, 1);
    channelButtonGroup->addButton(_chan2, 2);
    channelButtonGroup->addButton(_chan3, 3);
    channelButtonGroup->addButton(_chan4, 4);
    
    // configure the radar selection
    _forwardRadar = true;
    _foreRadar->setChecked(true);
    _aftRadar->setChecked(false);
    QButtonGroup* radarButtonGroup = new QButtonGroup();
    radarButtonGroup->addButton(_foreRadar, true);
    radarButtonGroup->addButton(_aftRadar, false);
    

    // connect the controls
    connect(_autoScale, SIGNAL(released()), this, SLOT(autoScaleSlot()));
    connect(_gainKnob, SIGNAL(valueChanged(double)), this, SLOT(gainChangeSlot(double)));
    connect(_up, SIGNAL(released()), this, SLOT(upSlot()));
    connect(_dn, SIGNAL(released()), this, SLOT(dnSlot()));
    connect(_saveImage, SIGNAL(released()), this, SLOT(saveImageSlot()));
    connect(_pauseButton, SIGNAL(toggled(bool)), this, SLOT(pauseSlot(bool)));
    connect(_gateNumber, SIGNAL(activated(int)), this, SLOT(gateChoiceSlot(int)));
    connect(_xGrid, SIGNAL(toggled(bool)), _scopePlot, SLOT(enableXgrid(bool)));
    connect(_yGrid, SIGNAL(toggled(bool)), _scopePlot, SLOT(enableYgrid(bool)));
    connect(_blockSizeCombo, SIGNAL(activated(int)), this, SLOT(blockSizeSlot(int)));
    connect(channelButtonGroup, SIGNAL(buttonReleased(int)), this, SLOT(channelSlot(int)));
    connect(gateModeButtonGroup, SIGNAL(buttonReleased(int)), this, SLOT(gateModeSlot(int)));
    connect(radarButtonGroup, SIGNAL(buttonReleased(int)), this, SLOT(radarSlot(int)));

    // set the checkbox selections
    _pauseButton->setChecked(false);
    _xGrid->setChecked(true);
    _yGrid->setChecked(true);

    // initialize the book keeping for the plots.
    // This also sets up the radio buttons 
    // in the plot type tab widget
    initPlots();

    _gainKnob->setRange(-7, 7);
    _gainKnob->setTitle("Gain");

    // set the minor ticks
    _gainKnob->setScaleMaxMajor(5);
    _gainKnob->setScaleMaxMinor(5);

    _xyGraphRange = 1;
    _xyGraphCenter = 0.0;
    _knobGain = 0.0;
    _knobOffset = 0.0;
    _specGraphRange = 120.0;
    _specGraphCenter = -40.0;

    // set up the palettes
    _greenPalette = _chan0led->palette();
    _greenPalette.setColor(_chan0led->backgroundRole(), QColor("green"));
    _redPalette = _greenPalette;
    _redPalette.setColor(_chan0led->backgroundRole(), QColor("red"));

    // initialize eof leds to green
    _chan0led->setAutoFillBackground(true);
    _chan1led->setAutoFillBackground(true);
    _chan2led->setAutoFillBackground(true);
    _chan0led->setPalette(_greenPalette);
    _chan1led->setPalette(_greenPalette);
    _chan2led->setPalette(_greenPalette);

    // The initial plot type will be I and Q timeseries
    plotTypeSlot(TS_TIMESERIES_PLOT);

    // start the statistics timer
    startTimer(_statsUpdateInterval*1000);

}
//////////////////////////////////////////////////////////////////////
EldoraScope::~EldoraScope() {
}

//////////////////////////////////////////////////////////////////////
void EldoraScope::initFFT() {

    // configure the block/fft size selection
    /// @todo add logic to insure that smallest fft size is a power of two.
    int fftSize = _config.getInt("SmallestFFT", 16);
    int maxFftSize = _config.getInt("LargestFFT", 1024);
    for (; fftSize <= maxFftSize; fftSize = fftSize*2) {
        _blockSizeChoices.push_back(fftSize);
        QString l = QString("%1").arg(fftSize);
        _blockSizeCombo->addItem(l, QVariant(fftSize));
    }
    // select the middle choice for the block size
    _blockSizeIndex = (_blockSizeChoices.size()-1)/2;
    if (_blockSizeIndex < 0)
        _blockSizeIndex = 0;
    _blockSizeCombo->setCurrentIndex(_blockSizeIndex);

    //  set up fft for power calculations: 
    _fftwData.resize(_blockSizeChoices.size());
    _fftwPlan.resize(_blockSizeChoices.size());
    for (unsigned int i = 0; i < _blockSizeChoices.size(); i++) {
        // allocate the data space for fftw
        int blockSize = _blockSizeChoices[i];
        _fftwData[i] = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)
                * blockSize);
        // create the plan.
        _fftwPlan[i] = fftw_plan_dft_1d(blockSize, _fftwData[i], _fftwData[i], 
        FFTW_FORWARD, 
        FFTW_ESTIMATE);
    }
    //  power correction factor applied to (uncorrected) powerSpectrum() output:
    _powerCorrection = 0.0; //  use for power correction to dBm

}

//////////////////////////////////////////////////////////////////////
void EldoraScope::productSlot() {
    if (_paused)
        return;
    //				processProduct(pProduct);
}

//////////////////////////////////////////////////////////////////////
void EldoraScope::timeSeriesSlot(
        std::vector<double> I,
            std::vector<double> Q,
            double sampleRateHz,
            double tuningFreqHz) {
    if (_paused)
        return;

    processTimeSeries(I, Q);
}

//////////////////////////////////////////////////////////////////////
void EldoraScope::saveImageSlot() {
    QString f = _config.getString("imageSaveDirectory", "c:/").c_str();

    QFileDialog d(this,
            tr("Save EldoraScope Image"),
            f,
            tr("PNG files (*.png);;All files (*.*)"));
    d.setFileMode(QFileDialog::AnyFile);
    d.setViewMode(QFileDialog::Detail);
    d.setAcceptMode(QFileDialog::AcceptSave);
    d.setConfirmOverwrite(true);
    d.setDefaultSuffix("png");
    d.setDirectory(f);

    f = "EldoraScope-";
    f += QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
    f += ".png";
    d.selectFile(f);
    if (d.exec()) {
        QStringList saveNames = d.selectedFiles();
        _scopePlot->saveImageToFile(saveNames[0].toStdString());
        f = d.directory().absolutePath();
        _config.setString("imageSaveDirectory", f.toStdString());
    }
}
//////////////////////////////////////////////////////////////////////
void EldoraScope::processTimeSeries(
        std::vector<double>& Idata,
            std::vector<double>& Qdata) {
    if (!_timeSeriesPlot)
        return;

    PlotInfo* pi = &_tsPlotInfo[_tsPlotType];
    switch (pi->getDisplayType()) {
    case ScopePlot::SPECTRUM: {

        // compute the power spectrum
        _zeroMoment = powerSpectrum(Idata, Qdata);
        displayData();
        break;
    }
    case SCOPE_PLOT_TIMESERIES:
    case SCOPE_PLOT_IVSQ: {
        I.resize(Idata.size());
        Q.resize(Qdata.size());
        I = Idata;
        Q = Qdata;
        _zeroMoment = zeroMomentFromTimeSeries(I, Q);
        displayData();
        break;
    }
    default:
        // ignore others
        break;
    }
}

//////////////////////////////////////////////////////////////////////
void EldoraScope::processProduct() {
    // if we are displaying a raw plot, just ignore
    if (_timeSeriesPlot)
        return;

    PRODUCT_PLOT_TYPES prodType;// = pProduct->header.prodType;

    if (prodType == _productPlotType) {
        displayData();
    }
}

//////////////////////////////////////////////////////////////////////
void EldoraScope::displayData() {
    double yBottom = _xyGraphCenter - _xyGraphRange;
    double yTop = _xyGraphCenter + _xyGraphRange;

    QString l = QString("%1").arg(_zeroMoment, 6, 'f', 1);
    _powerDB->setText(l);

    if (_timeSeriesPlot) {
        PlotInfo* pi = &_tsPlotInfo[_tsPlotType];

        std::string xlabel;
        switch (pi->getDisplayType()) {
        case ScopePlot::TIMESERIES:
            if (_performAutoScale)
                autoScale(I, Q);
            xlabel = ((_gateMode == ONE_GATE) ? std::string("Time")
                    : std::string("Gate"));
            _scopePlot->TimeSeries(I, Q, yBottom, yTop, 1, xlabel, "I - Q");
            break;
        case ScopePlot::IVSQ:
            if (_performAutoScale)
                autoScale(I, Q);
            _scopePlot->IvsQ(I, Q, yBottom, yTop, 1, "I", "Q");
            break;
        case ScopePlot::SPECTRUM:
            _scopePlot->Spectrum(_spectrum,
                    _specGraphCenter-_specGraphRange /2.0,
                    _specGraphCenter+_specGraphRange/2.0,
                    1000000,
                    false,
                    "Frequency (Hz)",
                    "Power (dB)");
            break;
        }

    } else {
        if (_performAutoScale)
            autoScale(_ProductData);
        // send in the product id, which ScopePlot::Product() uses
        // to decide if axis rescaling is needed.
        PlotInfo* pi = &_productPlotInfo[_productPlotType];
        _scopePlot->Product(_ProductData,
                pi->getId(),
                yBottom,
                yTop,
                _ProductData.size(),
                "Gate",
                pi->getLongName());
    }
}

//////////////////////////////////////////////////////////////////////
double EldoraScope::powerSpectrum(
        std::vector<double>& Idata,
            std::vector<double>& Qdata) {

    int blockSize = _blockSizeChoices[_blockSizeIndex];

    _spectrum.resize(blockSize);
    int n = Idata.size();
    if (blockSize < n) {
        n = blockSize;
    }
    for (int j = 0; j < n; j++) {
        // transfer the data to the fftw input space
        _fftwData[_blockSizeIndex][j][0] = Idata[j];
        _fftwData[_blockSizeIndex][j][1] = Qdata[j];
    }
    // zero pad, if we are looking at along beam data.
    for (int j = n; j < blockSize; j++) {
        _fftwData[_blockSizeIndex][j][0] = 0;
    }

    // apply the hamming window to the time series
    //  if (_doHamming)
    //    doHamming();

    // caclulate the fft
    fftw_execute(_fftwPlan[_blockSizeIndex]);

    double zeroMoment = 0.0;

    // reorder and copy the results into _spectrum
    for (int i = 0; i < blockSize/2; i++) {
        double pow = _fftwData[_blockSizeIndex][i][0]
                * _fftwData[_blockSizeIndex][i][0]
                + _fftwData[_blockSizeIndex][i][1]
                        * _fftwData[_blockSizeIndex][i][1];

        zeroMoment += pow;

        pow /= blockSize*blockSize;
        pow = 10.0*log10(pow);
        _spectrum[i+blockSize/2] = pow;
    }

    for (int i = blockSize/2; i < blockSize; i++) {
        double pow = _fftwData[_blockSizeIndex][i][0]
                * _fftwData[_blockSizeIndex][i][0]
                + _fftwData[_blockSizeIndex][i][1]
                        * _fftwData[_blockSizeIndex][i][1];

        zeroMoment += pow;

        pow /= blockSize*blockSize;
        pow = 10.0*log10(pow);
        _spectrum[i - blockSize/2] = pow;
    }

    zeroMoment /= blockSize*blockSize;
    zeroMoment = 10.0*log10(zeroMoment);

    // correct unscaled power data using knob setting: 
    for (int j = 0; j < blockSize; j++) {
        _spectrum[j] += _powerCorrection;
    }

    return zeroMoment;
}

////////////////////////////////////////////////////////////////////
void EldoraScope::plotTypeSlot(
        int plotType) {

    // find out the index of the current page
    int pageNum = _typeTab->currentIndex();

    // get the radio button id of the currently selected button
    // on that page.
    int ptype = _tabButtonGroups[pageNum]->checkedId();

    if (pageNum == 0) {
        // change to a raw plot type
        TS_PLOT_TYPES plotType = (TS_PLOT_TYPES)ptype;
        plotTypeChange(&_tsPlotInfo[plotType],
                plotType,
                (PRODUCT_PLOT_TYPES)0 ,
                true);
    } else {
        // change to a product plot type
        PRODUCT_PLOT_TYPES productType = (PRODUCT_PLOT_TYPES)ptype;
        plotTypeChange(&_productPlotInfo[productType],
                (TS_PLOT_TYPES)0,
                productType,
                false);
    }
}
//////////////////////////////////////////////////////////////////////
void EldoraScope::tabChangeSlot(
        QWidget* w) {
    // find out the index of the current page
    int pageNum = _typeTab->currentIndex();

    // get the radio button id of the currently selected button
    // on that page.
    int ptype = _tabButtonGroups[pageNum]->checkedId();

    if (pageNum == 0) {
        // change to a raw plot type
        TS_PLOT_TYPES plotType = (TS_PLOT_TYPES)ptype;
        plotTypeChange(&_tsPlotInfo[plotType],
                plotType,
                (PRODUCT_PLOT_TYPES)0 ,
                true);
    } else {
        // change to a product plot type
        PRODUCT_PLOT_TYPES productType = (PRODUCT_PLOT_TYPES)ptype;
        plotTypeChange(&_productPlotInfo[productType],
                (TS_PLOT_TYPES)0,
                productType,
                false);
    }
}

////////////////////////////////////////////////////////////////////
void EldoraScope::plotTypeChange(
        PlotInfo* pi,
            TS_PLOT_TYPES newPlotType,
            PRODUCT_PLOT_TYPES newProductType,
            bool isTsPlot) {

    // save the gain and offset of the current plot type
    PlotInfo* currentPi;
    if (_timeSeriesPlot) {
        currentPi = &_tsPlotInfo[_tsPlotType];
    } else {
        currentPi = &_productPlotInfo[_productPlotType];
    }
    currentPi->setGain(pi->getGainMin(), pi->getGainMax(), _knobGain);
    currentPi->setOffset(pi->getOffsetMin(), pi->getOffsetMax(), _xyGraphCenter);

    // restore gain and offset for new plot type
    gainChangeSlot(pi->getGainCurrent());
    _xyGraphCenter = pi->getOffsetCurrent();

    // set the knobs for the new plot type
    _gainKnob->setValue(_knobGain);

    // change the plot type
    if (isTsPlot) {
        _tsPlotType = newPlotType;
    } else {
        _productPlotType = newProductType;
    }

    // set flag indicating if new plot is a timeseries type
    _timeSeriesPlot = isTsPlot;

}

////////////////////////////////////////////////////////////////////
void EldoraScope::initPlots() {

    _pulsePlots.insert(TS_TIMESERIES_PLOT);
    _pulsePlots.insert(TS_IVSQ_PLOT);
    _pulsePlots.insert(TS_SPECTRUM_PLOT);

    _productPlots.insert(PROD_DBMHC);
    _productPlots.insert(PROD_DBMVC);
    _productPlots.insert(PROD_DBZ);
    _productPlots.insert(PROD_WIDTH);
    _productPlots.insert(PROD_VEL);
    _productPlots.insert(PROD_SNR);
    _productPlots.insert(PROD_RHOHV);
    _productPlots.insert(PROD_PHIDP);
    _productPlots.insert(PROD_ZDR);

    _tsPlotInfo[TS_TIMESERIES_PLOT] = PlotInfo(TS_TIMESERIES_PLOT,
            SCOPE_PLOT_TIMESERIES,
            "I and Q",
            "S:  I and Q",
            -5.0,
            5.0,
            0.0,
            -5.0,
            5.0,
            0.0);
    _tsPlotInfo[TS_IVSQ_PLOT] = PlotInfo(TS_IVSQ_PLOT,
            SCOPE_PLOT_IVSQ,
            "I vs Q",
            "S:  I vs Q",
            -5.0,
            5.0,
            0.0,
            -5.0,
            5.0,
            0.0);
    _tsPlotInfo[TS_SPECTRUM_PLOT] = PlotInfo(TS_SPECTRUM_PLOT,
            SCOPE_PLOT_SPECTRUM,
            "Power Spectrum",
            "S:  Power Spectrum",
            -5.0,
            5.0,
            0.0,
            -5.0,
            5.0,
            0.0);

    _productPlotInfo[PROD_DBMHC] = PlotInfo(PROD_DBMHC,
            SCOPE_PLOT_PRODUCT,
            "H Dbm",
            "Sh: Dbm",
            -5.0,
            5.0,
            0.0,
            -5.0,
            5.0,
            0.0);
    _productPlotInfo[PROD_DBMVC] = PlotInfo(PROD_DBMVC,
            SCOPE_PLOT_PRODUCT,
            "V Dbm",
            "Sv: Dbm",
            -5.0,
            5.0,
            0.0,
            -5.0,
            5.0,
            0.0);
    _productPlotInfo[PROD_DBZ] = PlotInfo(PROD_DBZ,
            SCOPE_PLOT_PRODUCT,
            "Dbz",
            "S : Dbz",
            -5.0,
            5.0,
            0.0,
            -5.0,
            5.0,
            0.0);
    _productPlotInfo[PROD_WIDTH] = PlotInfo(PROD_WIDTH,
            SCOPE_PLOT_PRODUCT,
            "Width",
            "S:  Width",
            -5.0,
            5.0,
            0.0,
            -5.0,
            5.0,
            0.0);
    _productPlotInfo[PROD_VEL] = PlotInfo(PROD_VEL,
            SCOPE_PLOT_PRODUCT,
            "Velocity",
            "S:  Velocity",
            -5.0,
            5.0,
            0.0,
            -5.0,
            5.0,
            0.0);
    _productPlotInfo[PROD_SNR] = PlotInfo(PROD_SNR,
            SCOPE_PLOT_PRODUCT,
            "SNR",
            "S:  SNR",
            -5.0,
            5.0,
            0.0,
            -5.0,
            5.0,
            0.0);
    _productPlotInfo[PROD_RHOHV] = PlotInfo(PROD_RHOHV,
            SCOPE_PLOT_PRODUCT,
            "Rhohv",
            "S:  Rhohv",
            -5.0,
            5.0,
            0.0,
            -5.0,
            5.0,
            0.0);
    _productPlotInfo[PROD_PHIDP] = PlotInfo(PROD_PHIDP,
            SCOPE_PLOT_PRODUCT,
            "Phidp",
            "S:  Phidp",
            -5.0,
            5.0,
            0.0,
            -5.0,
            5.0,
            0.0);
    _productPlotInfo[PROD_ZDR] = PlotInfo(PROD_ZDR,
            SCOPE_PLOT_PRODUCT,
            "Zdr",
            "S:  Zdr",
            -5.0,
            5.0,
            0.0,
            -5.0,
            5.0,
            0.0);

    // remove the one tab that was put there by designer
    _typeTab->removeTab(0);

    // add tabs, and save the button group for
    // for each tab.
    QButtonGroup* pGroup;

    pGroup = addTSTypeTab("I & Q", _pulsePlots);
    _tabButtonGroups.push_back(pGroup);

    pGroup = addProductTypeTab("Products", _productPlots);
    _tabButtonGroups.push_back(pGroup);

    connect(_typeTab, SIGNAL(currentChanged(QWidget *)), this, SLOT(tabChangeSlot(QWidget*)));
}

//////////////////////////////////////////////////////////////////////
QButtonGroup* EldoraScope::addTSTypeTab(
        std::string tabName,
            std::set<TS_PLOT_TYPES> types) {
    // The page that will be added to the tab widget
    QWidget* pPage = new QWidget;
    // the layout manager for the page, will contain the buttons
    QVBoxLayout* pVbox = new QVBoxLayout;
    // the button group manager, which has nothing to do with rendering
    QButtonGroup* pGroup = new QButtonGroup;

    std::set<TS_PLOT_TYPES>::iterator i;

    for (i = types.begin(); i != types.end(); i++) {
        // create the radio button
        int id = _tsPlotInfo[*i].getId();
        QRadioButton* pRadio = new QRadioButton;
        const QString label = _tsPlotInfo[*i].getLongName().c_str();
        pRadio->setText(label);

        // put the button in the button group
        pGroup->addButton(pRadio, id);
        // assign the button to the layout manager
        pVbox->addWidget(pRadio);

        // set the first radio button of the group
        // to be selected.
        if (i == types.begin()) {
            pRadio->setChecked(true);
        }
    }
    // associate the layout manager with the page
    pPage->setLayout(pVbox);

    // put the page on the tab
    _typeTab->insertTab(-1, pPage, tabName.c_str());

    // connect the button released signal to our plot type change slot.
    connect(pGroup, SIGNAL(buttonReleased(int)), this, SLOT(plotTypeSlot(int)));

    return pGroup;
}

//////////////////////////////////////////////////////////////////////
QButtonGroup* EldoraScope::addProductTypeTab(
        std::string tabName,
            std::set<PRODUCT_PLOT_TYPES> types) {
    // The page that will be added to the tab widget
    QWidget* pPage = new QWidget;
    // the layout manager for the page, will contain the buttons
    QVBoxLayout* pVbox = new QVBoxLayout;
    // the button group manager, which has nothing to do with rendering
    QButtonGroup* pGroup = new QButtonGroup;

    std::set<PRODUCT_PLOT_TYPES>::iterator i;

    for (i = types.begin(); i != types.end(); i++) {
        // create the radio button
        int id = _productPlotInfo[*i].getId();
        QRadioButton* pRadio = new QRadioButton;
        const QString label = _productPlotInfo[*i].getLongName().c_str();
        pRadio->setText(label);

        // put the button in the button group
        pGroup->addButton(pRadio, id);
        // assign the button to the layout manager
        pVbox->addWidget(pRadio);

        // set the first radio button of the group
        // to be selected.
        if (i == types.begin()) {
            pRadio->setChecked(true);
        }
    }
    // associate the layout manager with the page
    pPage->setLayout(pVbox);

    // put the page on the tab
    _typeTab->insertTab(-1, pPage, tabName.c_str());

    // connect the button released signal to our plot type change slot.
    connect(pGroup, SIGNAL(buttonReleased(int)), this, SLOT(plotTypeSlot(int)));

    return pGroup;
}

//////////////////////////////////////////////////////////////////////
void EldoraScope::timerEvent(
        QTimerEvent*) {

    //int rate[3];
    //for (int i = 0; i < 3; i++) {
    //		rate[i] = (_pulseCount[i] - _prevPulseCount[i])/(double)_statsUpdateInterval;
    //		_prevPulseCount[i] = _pulseCount[i];
    //}
    //	_chan0pulseCount->setNum(_pulseCount[0]/1000);
    //	_chan0pulseRate->setNum(rate[0]);
    //	_chan0errors->setNum(_errorCount[0]);
    //	_chan1pulseCount->setNum(_pulseCount[1]/1000);
    //	_chan1pulseRate->setNum(rate[1]);
    //	_chan1errors->setNum(_errorCount[1]);
    //	_chan2pulseCount->setNum(_pulseCount[2]/1000);
    //	_chan2pulseRate->setNum(rate[2]);
    //	_chan2errors->setNum(_errorCount[2]);

    //	std::cout << "Packet errors = " <<
    //	  _errorCount[0] << " " << _errorCount[1] << " " <<
    //	  _errorCount[2] << std::endl;

}

//////////////////////////////////////////////////////////////////////
void EldoraScope::gainChangeSlot(
        double gain) {

    // keep a local copy of the gain knob value
    _knobGain = gain;

    _powerCorrection = gain;

    _xyGraphRange = pow(10.0, -gain);

    _gainKnob->setValue(gain);

}

//////////////////////////////////////////////////////////////////////
void EldoraScope::upSlot() {
    bool spectrum = false;

    if (_timeSeriesPlot) {
        PlotInfo* pi = &_tsPlotInfo[_tsPlotType];
        if (pi->getDisplayType() == ScopePlot::SPECTRUM) {
            spectrum = true;
        }
    }

    if (!spectrum) {
        _xyGraphCenter -= 0.03*_xyGraphRange;
    } else {
        _specGraphCenter -= 0.03*_specGraphRange;
    }
    displayData();
}

//////////////////////////////////////////////////////////////////////
void EldoraScope::dnSlot() {

    bool spectrum = false;

    if (_timeSeriesPlot) {
        PlotInfo* pi = &_tsPlotInfo[_tsPlotType];
        if (pi->getDisplayType() == ScopePlot::SPECTRUM) {
            spectrum = true;
        }
    }

    if (!spectrum) {
        _xyGraphCenter += 0.03*_xyGraphRange;
    } else {
        _specGraphCenter += 0.03*_specGraphRange;
    }

    displayData();
}

//////////////////////////////////////////////////////////////////////
void EldoraScope::autoScale(
        std::vector<double>& data) {
    if (data.size() == 0) {
        _performAutoScale = false;
        return;
    }

    // find the min and max
    double min = *std::min_element(data.begin(), data.end());
    double max = *std::min_element(data.begin(), data.end());

    // adjust the gains
    adjustGainOffset(min, max);
    _performAutoScale = false;
}

//////////////////////////////////////////////////////////////////////
void EldoraScope::autoScale(
        std::vector<double>& data1,
            std::vector<double>& data2) {
    if (data1.size() == 0 || data2.size() == 0) {
        _performAutoScale = false;
        return;
    }

    // find the min and max
    double min1 = *std::min_element(data1.begin(), data1.end());
    double min2 = *std::min_element(data2.begin(), data2.end());
    double min = std::min(min1, min2);

    double max1 = *std::max_element(data1.begin(), data1.end());
    double max2 = *std::max_element(data2.begin(), data2.end());
    double max = std::max(max1, max2);

    // adjust the gains
    adjustGainOffset(min, max);

    _performAutoScale = false;
}

//////////////////////////////////////////////////////////////////////
void EldoraScope::adjustGainOffset(
        double min,
            double max) {
    double factor = 0.8;
    _xyGraphCenter = (min+max)/2.0;
    _xyGraphRange = (1/factor)*(max - min)/2.0;

    _knobGain = -log10(_xyGraphRange);

    _gainKnob->setValue(_knobGain);
}

//////////////////////////////////////////////////////////////////////
void EldoraScope::autoScaleSlot() {
    _performAutoScale = true;
}

//////////////////////////////////////////////////////////////////////
void EldoraScope::pauseSlot(
        bool p) {
    _paused = p;
}

//////////////////////////////////////////////////////////////////////
void EldoraScope::dataMode() {
    if (_gateMode == ONE_GATE) {
emit         		                                                oneGateSignal(_channel,
                _forwardRadar,
                _gateChoice,
                _blockSizeChoices[_blockSizeIndex]);
    } else {
emit         		                                                alongBeamSignal(_channel, _forwardRadar);
    }
}

//////////////////////////////////////////////////////////////////////
void EldoraScope::gateModeSlot(
        int m) {
    _gateMode = (GATE_MODE)m;

    dataMode();
}

//////////////////////////////////////////////////////////////////////
void EldoraScope::gateListSlot(
        std::vector<int> gates) {
    _gates = gates;
    for (unsigned int i = 0; i < _gates.size(); i++) {
        QString l = QString("%1").arg(i);
        _gateNumber->addItem(l, _gates[i]);
    }
    // default to the first avaiable gate
    _gateChoice = _gates[0];

    dataMode();
}

//////////////////////////////////////////////////////////////////////
void EldoraScope::channelSlot(
        int c) {
    _channel = c;
    // tell the data source about our decision

    dataMode();
}

//////////////////////////////////////////////////////////////////////
void EldoraScope::radarSlot(
        int forwardRadar) {
    _forwardRadar = forwardRadar,
    // tell the data source about our decision

    dataMode();
}//////////////////////////////////////////////////////////////////////
void EldoraScope::gateChoiceSlot(
        int index) {
    _gateChoice = _gates[index];

    dataMode();
}

//////////////////////////////////////////////////////////////////////
void EldoraScope::blockSizeSlot(
        int index) {
    _blockSizeIndex = index;
    dataMode();
}

////////////////////////////////////////////////////////////////////////

double EldoraScope::zeroMomentFromTimeSeries(
        std::vector<double>& I,
            std::vector<double>& Q) {
    double p = 0;
    int n = I.size();

    for (unsigned int i = 0; i < I.size(); i++) {
        p += I[i]*I[i] + Q[i]*Q[i];
    }

    p /= n;
    p = 10.0*log10(p);
    return p;
}
