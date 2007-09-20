#include "PentekSnarfer.h"
#include <iostream>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////

PentekSnarfer::PentekSnarfer(int blockSize, 
        double tuningFreq, 
        int decimation,
        bool flipSpectrum):
  _fifo(0),
  _blockSize(blockSize),
  _sampleCount(0),
  _rate(0.0),
  _blockCount(0),
  _decimation(decimation),
  _ncoHz(tuningFreq),
  _flipSpectrum(flipSpectrum)
{

  _deviceName = "/dev/pentek/p7140/0/dn/0C";

  _deviceFd = open(_deviceName.c_str(), O_RDONLY | O_NONBLOCK);

  if (_deviceFd < 0) {
    std::cerr << "cannot access " << _deviceName << "\n";
    perror("");
    exit(1);
  }

  configurePentek();

  if (_deviceFd < 0) {
    std::cerr << "cannot access " << _deviceName << "\n";
    perror("");
    exit(1);
  }

  queryPentek();

  blockSizeSlot(_blockSize);


  // this timer will call rateTimeout to calculate a data rate
  QObject::connect(&_rateTimer, SIGNAL(timeout()), this, SLOT(rateTimeout()));
  _rateTimer.start(10000);
  _rateTime.restart();

  // this timer is called to plot the data
  QObject::connect(&_plotTimer, SIGNAL(timeout()), this, SLOT(plotTimeout()));
  _plotTimer.start(100);

  // set up the read timer
  QObject::connect(&_readTimer, SIGNAL(timeout()), this, SLOT(readDevice()));
  _readTimer.start(100);

}

//////////////////////////////////////////////////////////////////////

PentekSnarfer::~PentekSnarfer() 
{

  delete _fifo;
}

////////////////////////////////////////////////////////////

void
PentekSnarfer::configurePentek()
{

  double doublearg;
  BUFFER_CFG bufcfg;

  bufcfg.bufno = 0;
  bufcfg.intbufsize = PENTEK_BUFSIZE;
  bufcfg.bufsize = 4*bufcfg.intbufsize;
  
  if (ioctl(_deviceFd, FIOBUFSET, &bufcfg) == -1) 
  {
  std::cout << "unable to set buffer sizes for " 
  << _deviceName << std::endl;
  perror("");
  exit(1);
  }

  if (ioctl(_deviceFd, FIOCLKSRCSET, CLK_SRC_FRTPAN) == -1) 
    {
    std::cout << "unable to set the clock source for " << _deviceName << std::endl;
    perror("");
    exit(1);
  }

  if (ioctl(_deviceFd, FIOSPECTFORMSET, _flipSpectrum? SPECT_FLIP:SPECT_NORMAL) == -1) 
    {
      std::cout << "unable to set the spectral format for " << _deviceName << std::endl;
      perror("");
      exit(1);
    }

  if (ioctl(_deviceFd, FIODECIMSET, _decimation) == -1) {
    std::cout << "unable to set the decimation rate for " << _deviceName << std::endl;
    perror("");
    exit(1);
  }
 
  doublearg = 100000000.0;
  if (ioctl(_deviceFd, FIOSAMPRATESET, &doublearg) == -1) {
    std::cout << "unable to set the clock rate for " << _deviceName << std::endl;
    perror("");
    exit(1);
  }

  doublearg = _ncoHz;
  if (ioctl(_deviceFd, FIONCOSET, &doublearg) == -1) 
    {
      std::cout << "unable to set the tuning frequency for " 
		<< _deviceName << std::endl;
      perror("");
      exit(1);
    }
  

  if (ioctl(_deviceFd, FIOFLUSH, 0) == -1)
    {
      std::cout << "ioctl FLUSH failed for " << _deviceName << std::endl;
      perror("");
      exit(1);
    }
}

////////////////////////////////////////////////////////////////////////

void
PentekSnarfer::queryPentek() 
{

  BUFFER_CFG bufcfg;

  bufcfg.bufno = -1;
  bufcfg.bufsize = -1;
  bufcfg.intbufsize = -1;
  ioctl(_deviceFd, FIOBUFGET, &bufcfg);
  _bufsize    = bufcfg.bufsize;
  _intbufsize = bufcfg.intbufsize;
  
  int clkSrc = ioctl(_deviceFd, FIOCLKSRCGET, 0);

  _decimation = ioctl(_deviceFd, FIODECIMGET, 0);

  _clockRate = 0.0;
  ioctl(_deviceFd, FIOSAMPRATEGET, &_clockRate);
  
  _ncoHz = 0;
  ioctl(_deviceFd, FIONCOGET, &_ncoHz);

  _sampleRateHz = _clockRate/_decimation/4.0;

  std::cout << "  decimation: " << _decimation    << "\n"
     << "  clock rate: " << _clockRate     << "\n"
     << "  nco:        " << _ncoHz         << "\n"
     << "  sample rate " << _sampleRateHz  << "\n"
     << "  intbufsize: " << _intbufsize    << "\n"
     << "  bufsize:    " << _bufsize       << "\n"
     << "  clock src:  " << clkSrc         << "\n"
     << "\n";
}

////////////////////////////////////////////////////////////////////////

void
PentekSnarfer::blockSizeSlot(int size) {


  _blockSize = size;

  delete _fifo;
  _fifo = new Fifo(2*_blockSize);

  _I.resize(_blockSize);
  _Q.resize(_blockSize);

}

////////////////////////////////////////////////////////////////////////

void
PentekSnarfer::readDevice() 
{

  int nComplete = 0;
  int nPartial = 0;
  while(1) {
    int readN = _blockSize*2*sizeof(short int);

    int n = read(_deviceFd, &(_fifo->push_back()[0]), readN);

    if (n != readN ) {

      // std:: cout << ".";
      // partial or zero read, get rid of the bogus fifo entry
      // that we created.
      _fifo->pop_back();

      if (n != 0) {
 std::cout << "wanted " << readN << ", got " << n << "\n";
 nPartial++;
      }
      break;
    }

    nComplete++;

    _sampleCount += _blockSize;
  }
  //  if (nComplete > 0 || nPartial > 0)
  //std::cout << nComplete << " complete frames, " << nPartial << " frames " << 
  //   _sampleCount << " samples\n";

}

////////////////////////////////////////////////////////////////////////

void
PentekSnarfer::plotTimeout() {

  int nDiscards = 0;

  while (_fifo->size() > FIFO_MAX_SECS*10) {
    _fifo->pop_front();
    nDiscards++;
  }

//  if (nDiscards > 0) {
//      std::cout << "\ndiscarded " << nDiscards << " blocks of " 
//          << _blockSize << " samples\n";  
//  }

  if (_fifo->size() == 0)
    return;

  short int* buf = &(_fifo->front()[0]);

  for (unsigned int i = 0; i < _I.size(); i++) {
    _I[i] = buf[2*i];
  }

  for (unsigned int i = 0; i < _Q.size(); i++) {
    _Q[i] = buf[2*i+1];
  }

  emit addDataSignal(_I, _Q, _sampleRateHz, _ncoHz);

  _fifo->pop_front();

}

////////////////////////////////////////////////////////////////////////

void
PentekSnarfer::rateTimeout()
{

  _rate = 1000.0*_sampleCount / _rateTime.elapsed(); 
  _rateTime.restart();
  _sampleCount = 0;
  
  emit dataRateSignal(_rate);


}



