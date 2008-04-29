#ifndef SDRSCOPE_H
#define SDRSCOPE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <deque>
#include <vector>

#include "ui_SdrScope.h"
#include <ScopePlot.h>

#include <fftw3.h>

#include <QObject>
#include <QString>
#include <QSocketNotifier>
#include <deque>
#include <QDateTime>
#include <QTimer>

/// Render Sdr time series data as:
/// <ul>
/// <li> A time series plot of I and Q.
/// <li> An I versus Q display.
/// <li> A plot of the power spectrum of the time series.
/// </ul>
///
/// Processing is performed on succesive blocks of I and Q
/// samples. These blocks are collected into the _I and _Q vectors,
/// and then passed on to a processing routine (timeSeries(),
/// IvsQ(), or powerSpectrum(). The processing routine prepares them
/// for display, and then passes the final values onto the display
/// component ( ScopePlot() ).
/// Thus, the primary algortihm is:
/// <ul>
/// <li>Fill _I and _Q</li>
/// <li>Process _I and _Q, to prepare for display.</li>
/// <li>Hand processesd results to the display.</li>
/// </ul>
/// Note that any data passed to the display component must be copied to
/// that component, as SdrScope retains ownership of the data structures
/// passed in the parameter lists.
///
/// The zeroth momment of the time series is calculated for each 
/// block and displayed in a label. Note that this momment is 
/// calculated from the time series when we are in time series
/// display mode, and from the power spectrum when we are in
/// spectrum display mode. Amazingly enough, the two methods agree!
///
/// _I and _Q are preallocated vectors which are refilled with each
/// block of time series data. If the block size is changed, these vectors
/// are reallocated.
///
class SdrScope : public QObject, private Ui::SdrScope
{

  /// The scope display type

  Q_OBJECT

    /// The type of scope display to render the next 
    /// block of time series into.
    enum SCOPETYPE {TIMESERIES=0, IVSQ=1, SPECTRUM=2};

 public:
  /// @param snarfer The provider of the time series data. We
  /// need to know who this is, because we will call it in order
  /// to specify which radar and frequency we are interested in.
  SdrScope(QDialog* parent);

  virtual ~SdrScope();
  
  /// @returns true if the scope is currently accepting data. This
  /// allows the scope to throttle the data delivery, and to 
  /// prevent delivery until the scope has been  completely
  /// initialized.
  bool acceptingData();

  public slots:
    
  /// Time series data is delivered here. Must be in _blocksize
  /// chunks
  /// length of timeseries; it is up to SdrScope to block
  /// the time series up into a blocksize of choice.
  /// @param I I data
  /// @param Q Q data
  /// @param sampleRateHz The sample rate in Hz
  /// @param tuningFreqHz The tuning frequency in Hz
  void addDataSlot(std::vector<double> I, 
		   std::vector<double> Q, 
		   double sampleRateHz,
		   double tuningFreqHz);

  /// Called when the scope type has been changed. SdrScope will
  /// be informed of the change.
  /// @param ty The scope type: 0-timeseries, 1-I versus Q, 2-Power spectrum
  void scopeTypeSlot(int ty);
      
  /// Called when the gain control has been released.
  /// @param gain The gain value.
  void gainSlot(double gain);

  /// Called when the offset control has been released.
  /// @param offset The offset value.
  void offsetSlot(double offset);

  /// Called to change the blck size. 
  /// @param index The index of the block size, from a combo list.
  void blockSizeSlot(int index);

  /// To set the pause state
  void pauseSlot(bool doPause);

  /// Enable or disable application of the Hamming window
  void hammingSlot(bool doIt);

  ///
  void progressSlot(int current, int max);

  /// To tell SdrScope about the measured rate of incoming samples
  void dataRateSlot(double rate);

 signals:
  ///
  void blockSizeSignal(int);

 protected:

  /// Compute the power spectrum and send the result to
  /// the spectrum display.
  /// @returns The zero moment, calculated from the fft results.
  double powerSpectrum();

  /// Calculate the zeroth moment, using the time
  /// series for input.
  double zeroMomentFromTimeSeries(std::vector<double>& I,
				  std::vector<double>& Q);

  /// Pre-calculate the Hamming window coefficients
  void hammingSetup(int n);

  /// Apply the Hamming window coeficients to _I and _Q
  void doHamming();

  /// set the scales according to the plot type, gain and offset.
  void setScales();

  /// The plotting component.
  ScopePlot* _plot;

  /// The I timeseries
  //  std::vector<double> _I;

  /// The Q time series
  //  std::vector<double> _Q;

  /// The power spectrum 
  std::vector<double> _spectrum;

  /// The hamming window coefficients
  std::vector<double> _hammingCoefs;

  /// The scope display type. This is controlled by
  /// scopetypeSlot().
  SCOPETYPE _scopeType;

  /// The value from the gain knob. This is controlled by
  /// gainSlot().
  double _timeSeriesGain;
  double _iqGain;
  double _spectrumGain;

  /// The value from the offset knob.This is controlled by 
  /// offsetSlot().
  double _timeSeriesOffset;
  double _spectrumOffset;

  /// The list of available block sizes. One of these will
  /// be selected in blockSizeSlot()
  std::vector<int> _blockSizes;

  /// The current block size.
  int _blockSize;

  /// The fftw plan. This is a handle used by
  /// the fftw routines.
  fftw_plan _fftwPlan;

  /// The fftw data array. The fft will
  // be performed in place, so both input data 
  /// and results are stored here.
  fftw_complex* _fftwData;

  /// Set true if the Hamming window should be applied
  bool _doHamming;

  /// the current minimum scale
  double _scaleMin;

  /// The current maximum scale
  double _scaleMax;

  /// Set true to pause the display
  bool _pause;

  /// The nominal data sample rate in sample per second.
  double _sampleRateHz;

  /// The tuning frequency in Hz
  double _tuningFreqHz;
  
  /// True if the scope is currently accepting data
  bool _acceptingData;
};
#endif
