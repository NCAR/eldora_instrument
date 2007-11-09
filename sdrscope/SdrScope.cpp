#include <math.h>
#include "SdrScope.h"

#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QTextBrowser>
#include <QSpinBox>
#include <QButtonGroup>
#include <QLayout>
#include <QComboBox>
#include <QLCDNumber>
#include <QProgressBar>
#include <QApplication>

#include "qwt_knob.h"
#include <iostream>

#include "fftw3.h"

/////////////////////////////////////////////////////////////////////////
SdrScope::SdrScope(QDialog* parent):
    QDialog(parent),
  _scopeType(SPECTRUM),
  _timeSeriesGain(0),
  _iqGain(0),
  _spectrumGain(0),
  _timeSeriesOffset(0),
  _spectrumOffset(0),
  _blockSize(0),
  _fftwData(0),
  _doHamming(false),
  _scaleMin(-1.0),
  _scaleMax(1.0),
  _pause(false),
  _sampleRateHz(1.0),
  _tuningFreqHz(1.0)
{

    setupUi(parent);
    
  // set up the acceptable block sizes
  int s = 32;
  for (int i=0; i<12; i++) {
    _blockSizes.push_back(s);
    comboBoxBlocksize->insertItem(i, QString("%1").arg(s));
    s *= 2;
  }

  QButtonGroup* plotTypeButtonGroup = new QButtonGroup();
  plotTypeButtonGroup->addButton(tsButton, TIMESERIES);
  plotTypeButtonGroup->addButton(iqButton, IVSQ);
  plotTypeButtonGroup->addButton(tsButton, SPECTRUM);
  connect(plotTypeButtonGroup, SIGNAL(buttonRelease(int)),
          this, SLOT(scopeTypeSlot(int)));
  
  
  // preallocate the _I and _Q vectors.
  blockSizeSlot(4);
  comboBoxBlocksize->setCurrentIndex(4);

  // set the inital display type
  //buttonGroupScopeType->setButton(_scopeType);

  // configure the gain knob
  qwtKnobGain->setScale(-2.0, 2.0, 0.5);
  qwtKnobGain->setRange(-2.0, 2.0);
  qwtKnobGain->setValue(_timeSeriesGain);

  // configure the offset knob
  qwtKnobOffset->setScale(-2.0, 2.0, 0.5);
  qwtKnobOffset->setRange(-2.0, 2.0);
  qwtKnobOffset->setValue(_timeSeriesOffset);

  setScales();

  pushButtonPause->setChecked(true);

}

/////////////////////////////////////////////////////////////////////////
SdrScope::~SdrScope() 
{
  delete _plot;
}

/////////////////////////////////////////////////////////////////////////
void 
SdrScope::addDataSlot(std::vector<double>& I, 
		      std::vector<double>& Q,
		      double sampleRateHz,
		      double tuningFreqHz)
{

  if (_pause)
    return;

  if (tuningFreqHz != _tuningFreqHz || _sampleRateHz != sampleRateHz) {
    _tuningFreqHz = tuningFreqHz;
    _sampleRateHz = sampleRateHz;
    setScales();
  }

  switch (_scopeType) {

  case TIMESERIES:

    _plot->TimeSeries(I, Q, _scaleMin, _scaleMax, sampleRateHz);
    LCDNumberPower->display(zeroMomentFromTimeSeries(I, Q));
    break;
    
  case IVSQ:
    _plot->IvsQ(I, Q, _scaleMin, _scaleMax, sampleRateHz);
    LCDNumberPower->display(zeroMomentFromTimeSeries(I, Q));
    break;
    
  case SPECTRUM:
    for(unsigned int j = 0; j < I.size(); j++)
      {
	// transfer the data to the fftw input space
	_fftwData[j][0] = I[j];
	_fftwData[j][1] = Q[j];
	
      }
    double zeroMoment = powerSpectrum();
    
    _plot->Spectrum(_spectrum, _scaleMin, _scaleMax, sampleRateHz, false, 
      "frequency", "power");
    LCDNumberPower->display(zeroMoment);

    break;
  }
}

/////////////////////////////////////////////////////////////////////////

void
SdrScope::scopeTypeSlot(int id) 
{
  _scopeType = (SCOPETYPE)id;

  switch (_scopeType) {
  case TIMESERIES:
    qwtKnobGain->setEnabled(true);
    qwtKnobGain->setValue(_timeSeriesGain);
    qwtKnobOffset->setEnabled(true);
    qwtKnobOffset->setValue(_timeSeriesOffset);
    break;
  case IVSQ:
    qwtKnobGain->setEnabled(true);
    qwtKnobGain->setValue(_iqGain);
    qwtKnobOffset->setEnabled(false);
    break;
  case SPECTRUM:
    qwtKnobGain->setEnabled(true);
    qwtKnobGain->setValue(_spectrumGain);
    qwtKnobOffset->setEnabled(true);
    qwtKnobOffset->setValue(_spectrumOffset);
    break;
  }

  setScales();

}
  
/////////////////////////////////////////////////////////////////////////

void
SdrScope::gainSlot(double gain) 
{
  switch (_scopeType) {
  case TIMESERIES:
    _timeSeriesGain = gain;
    break;
  case IVSQ:
    _iqGain = gain;
    break;
  case SPECTRUM:
    _spectrumGain = gain;
    break;
  }

  setScales();

}
  
/////////////////////////////////////////////////////////////////////////

void
SdrScope::offsetSlot(double offset) 
{
  switch (_scopeType) {
  case TIMESERIES:
    _timeSeriesOffset = offset;
    break;
  case IVSQ:
    break;
  case SPECTRUM:
    _spectrumOffset = offset;
    break;
  }

  setScales();

}
  
/////////////////////////////////////////////////////////////////////////

void
SdrScope::blockSizeSlot(int index) 
{
  if (_blockSizes[index] != _blockSize) {

    _blockSize = _blockSizes[index];

    emit blockSizeSignal(_blockSize);

    // preallocate vextors
    _spectrum.resize(_blockSize);

    // recalculate the hamming coefficients
    hammingSetup(_blockSize);

    // set up the fftw
    if (_fftwData) {
      fftw_destroy_plan(_fftwPlan);
      fftw_free(_fftwData);
    }

    // allocate the data space for fftw
    _fftwData  = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * _blockSize);

    // create the plan.
    _fftwPlan = fftw_plan_dft_1d(_blockSize, _fftwData, _fftwData,
				 FFTW_FORWARD, FFTW_ESTIMATE);

    setScales();
  }

}
  
/////////////////////////////////////////////////////////////////////////

void
SdrScope::pauseSlot(bool doPause )
{

  _pause = doPause;
  pushButtonPause->setText(_pause? "Paused":"Pause");

//  if (_pause)
//    pushButtonPause->setPaletteForegroundColor(QColor("red"));
//  else
//    pushButtonPause->unsetPalette();
					     

}

/////////////////////////////////////////////////////////////////////////

void
SdrScope::hammingSlot(bool doIt )
{

  _doHamming = doIt;

}

/////////////////////////////////////////////////////////////////////////

void 
SdrScope::dataRateSlot(double rate)
{

  QString l = QString("%1").arg(rate, 6, 'f', 0);
  textDataRate->setText(l);

}

/////////////////////////////////////////////////////////////////////////

double
SdrScope::powerSpectrum()
{

  // apply the hamming window to the time series
  if (_doHamming)
    doHamming();

  // caclulate the fft
  fftw_execute(_fftwPlan);


  double zeroMoment = 0.0;
  int n = _blockSize;

  // reorder and copy the results int _spectrum
  for (int i = 0 ; i < n/2; i++) {
    double pow =      _fftwData[i][0] * _fftwData[i][0] +
      _fftwData[i][1] * _fftwData[i][1];

    zeroMoment += pow;

    pow /= n*n;
    pow = 10.0*log10(pow);
    _spectrum[i+n/2] = pow;
  }

  for (int i = n/2 ; i < n; i++) {
    double pow =      _fftwData[i][0] * _fftwData[i][0] +
      _fftwData[i][1] * _fftwData[i][1];

    zeroMoment += pow;

    pow /= n*n;
    pow = 10.0*log10(pow);
    _spectrum[i - n/2] = pow;
  }

  zeroMoment /= n*n;
  zeroMoment = 10.0*log10(zeroMoment);

  return zeroMoment;
}

////////////////////////////////////////////////////////////////////////

double
SdrScope::zeroMomentFromTimeSeries(std::vector<double>& I,
				   std::vector<double>& Q)
{

  double p = 0;
  int n = I.size();

  for (unsigned int i = 0; i < I.size(); i++) {
    p += I[i]*I[i]
      + Q[i]*Q[i];
  }

  p /= n;

  p = 10.0*log10(p);

  return p;

}

////////////////////////////////////////////////////////////////////////

void
SdrScope::hammingSetup(int n) {

  _hammingCoefs.resize(n);

  for (int i = 0; i < n; i++) {
    _hammingCoefs[i] = 0.54 - 0.46*(cos(2.0*M_PI*i/(n-1)));
  }

}

////////////////////////////////////////////////////////////////////////

void
SdrScope::doHamming() {

  for (int i = 0; i < _blockSize; i++) {
    _fftwData[i][0] *= _hammingCoefs[i];
    _fftwData[i][1] *= _hammingCoefs[i];
  }
}

////////////////////////////////////////////////////////////////////////

void
SdrScope::setScales() {

  switch (_scopeType) {

  case TIMESERIES:
    {
      double factor = pow(10.0, -_timeSeriesGain);
      double offset = factor*1000.0*_timeSeriesOffset;
      _scaleMin  = -1000.0*factor - offset;
      _scaleMax  =  1000.0*factor - offset;
      break;
    }    
  case IVSQ:
    {
      double factor = pow(10.0, -_iqGain);
      _scaleMin  = -1000.0*factor;
      _scaleMax  =  1000.0*factor;
    }
    break;
  
  case SPECTRUM:
    {
      double decades = pow(10.0, 1-_spectrumGain);
      double offset = _spectrumOffset*decades;
      _scaleMin  = -offset;
      _scaleMax  = -offset + decades;
      break;


    }

 }

  // now update some diagnostic information
  double freqSpan = _sampleRateHz/2.0;
  double freqResolution = 2*freqSpan/_blockSize;
  double timeSpan = _blockSize/_sampleRateHz;

  QString l;

  l = QString("%1").arg(_tuningFreqHz/1.0e6,5,'f',3);
  textTuning->setText(l);

  l = QString("%1").arg(freqSpan,6,'f',0);
  textFreqRange->setText(l);

  l = QString("%1").arg(freqResolution,6,'f',1);
  textFreqRes->setText(l);

  l = QString("%1").arg(timeSpan,6,'f',4);
  textTimeSpan->setText(l);

  l = QString("%1").arg((int)_sampleRateHz);
  textSampleRate->setText(l);

}

////////////////////////////////////////////////////////////////////////
void
SdrScope::progressSlot(int current, int max)
{

}

