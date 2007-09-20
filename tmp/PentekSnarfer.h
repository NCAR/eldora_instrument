#ifndef PENTEKSNARFER_H
#define PENTEKSNARFER_H

#include <sys/types.h>
#include <sys/stat.h>

#include <string>
#include <vector>

#include <qobject.h>
#include <qstring.h>
#include <qfile.h>
#include <qthread.h>
#include <qtimer.h>
#include <qdatetime.h>
#include "ptkddr.h"

#include "Fifo.h"

///
/// PentekSnarfer implements a bandwidth limited data source for the Pentek radio cards. 
/// It uses Qt services for interaction with consumers of the digital receiver
/// data.
///
/// The founding consideration for PentekSnarfer is that the data stream from the Pentek 
/// digital receiver is usually very "bursty". The agregate data rates from the receiver
/// can be very high, and this data may arrive in large chunks spaced irregularly in time.
/// Conversly, the data consumers that are receiving this data are usually unable to 
/// process all of it. As an example, a Qt based plotting program would not be able
/// to render MBs of timerseries data, and even if it could, the human eye would not be
/// able to make use of it.
//
///  Thus PentekSnarfer has two functions: to discard a fraction of
/// the incoming data stream, and to deliver segments of the remaining data stream in a
/// more or less regular timing. These functions are accomplished through the use of two 
/// timers and a memory based FIFO. One timer is used to periodically check to see if
/// data is available from the card. If there is data, it is read in large chunks,
/// broken into smaller pieces, and placed in the FIFO. The second timer is used to 
/// peridically remove a single entry from the FIFO and send it to the data consumer. 
/// The same timer that periodically sends entries to the consumers also trims the FIFO
/// to a fixed size, discard the excess entries.
///
class PentekSnarfer: public QObject
{

  /// The maximum number of seconds of data
  /// to keep in the Fifo
#define FIFO_MAX_SECS  10

  /// the size of the Pentek interrupt buffer. Must be a power of two
  /// Also found that there is an upper limit to this size
  /// @todo find out what the rules are for this.
#define PENTEK_BUFSIZE 262144
  Q_OBJECT

    public:

  PentekSnarfer(int blockSize, 
		double tuningFreq, 
		int decimation,
		bool flipSpectrum);

  virtual ~PentekSnarfer();
  
  public slots:
  
  void readDevice();

  void blockSizeSlot(int size);

  void plotTimeout();

  void rateTimeout();

 signals:

  void addDataSignal(std::vector<double>&, std::vector<double>&, 
		     double sampeRateHz, double tuningFreqHz);

  void progressSignal(int, int);

  void dataRateSignal(double);

 public:

  /// The fifo file descriptor for receiving data. 
  int _deviceFd;

 protected:

  void setProgress();

  void configurePentek();

  void queryPentek();

  std::vector<double> _I;

  std::vector<double> _Q;
  
  Fifo* _fifo;

  std::string _deviceName;

  QTimer _readTimer;

  QTimer _plotTimer;

  QTimer _rateTimer;

  QTime  _rateTime;

  int _blockSize;

  double _sampleCount;

  double _rate;

  int _blockCount;

  double _clockRate;

  double _sampleRateHz;

  int _decimation;

  double  _ncoHz;

  bool _flipSpectrum;

  /// The configured bufsize (p7140_bufsize)
  int _bufsize;

  /// The configured interrupt bufsize (p7140_ibufsize)
  int _intbufsize;

};


#endif
