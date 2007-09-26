#ifndef RR314_H
#define RR314_H

#include <sys/types.h>
#include <sys/stat.h>

#include <string>
#include <vector>

#include <stdio.h>
#include <stdlib.h>

#include "FilterSpec.h"
#include "DDCregisters.h"

// RR channel adapter incudes
#include "ca_bar0_memmap.h"
#include "ca_diag_memmap.h"
#include "ca_lb_memmap.h"
#include "channeladapter.h"
#include "ca_functions.h"
#include "ca_membuffer.h"

#define DMANUMGROUPS      100  
#define DMABLOCKSIZE      1024
#define DMABLOCKSPERGROUP 16
#define BUFFERPOOLSIZE    2*DMANUMGROUPS
#define DMA_GROUPS_PER_INT 10

///
/// RRSnarfer implements a bandwidth limited data source for the Red River radio cards. 
/// It uses Qt services for interaction with consumers of the digital receiver
/// data.
///
/// The founding consideration for RRSnarfer is that the data stream from the Pentek 
/// digital receiver is usually very "bursty". The agregate data rates from the receiver
/// can be very high, and this data may arrive in large chunks spaced irregularly in time.
/// Conversly, the data consumers that are receiving this data are usually unable to 
/// process all of it. As an example, a Qt based plotting program would not be able
/// to render MBs of timerseries data, and even if it could, the human eye would not be
/// able to make use of it.
//
///  Thus RRSnarfer has two functions: to discard a fraction of
/// the incoming data stream, and to deliver segments of the remaining data stream in a
/// more or less regular timing. These functions are accomplished through the use of two 
/// timers and a memory based FIFO. One timer is used to periodically check to see if
/// data is available from the card. If there is data, it is read in large chunks,
/// broken into smaller pieces, and placed in the FIFO. The second timer is used to 
/// peridically remove a single entry from the FIFO and send it to the data consumer. 
/// The same timer that periodically sends entries to the consumers also trims the FIFO
/// to a fixed size, discard the excess entries.
///
class RR314
{

    public:

  /// Constructor
  RR314(unsigned int gates,
	     unsigned int samples,
	     unsigned int dualPrt,
	     unsigned int startGateIQ,
	     unsigned int nGatesIQ,
	     double adcSampleRate,          ///< Clock rate of the ADC
	     unsigned int decimationFactor,          ///< Downconvertor decimation count
	     std::string gaussianFile,      ///< The file containing the gaussian filter coefficients
	     std::string kaiserFile,        ///< The file containing the kaiser filter coefficients
	     std::string xsvfFileName       ///< The xsvfFile to be loaded. Blank if not to be loaded.
     );

  /// Destructor
  virtual ~RR314();
  
 public:

  /// The fifo file descriptor for receiving data. 
  int _deviceFd;

 protected:

  int configureRedRiver();

  /// configure the filters and the decimation value
  int filterSetup();

  /// program the cofficients for the gaussian and
  /// kaiser filters
  bool loadFilters(FilterSpec& gaussian, FilterSpec& kaiser);

  /// stop the RR card and return allocated space
  void shutdown();

  /// The device node for the RR card.
  std::string _deviceName;

  /// the sample rate of the ADC (Hz)
  double _adcSampleRate;

  /// The decimation factor (1-127)
  unsigned int _decimationFactor;

  /// The path to the file containing the gaussian filter definitions
  std::string _gaussianFile;

  /// The path to the file containing the kaiser filter coefficients
  std::string _kaiserFile;

  /// The path to a RR bitstream file, if it will be
  /// loaded
  std::string _xsvfFileName;

  /// The channel adapter, used in RR interfaces
  s_ChannelAdapter _CA0;

  /// Clock setting for the M314, used in RR interfaces.
  s_ClkSettings _ClkSettings;

  /// set true if the DMA allocation has been made
  bool _dmaAllocated;

  /// points to the next location in an incoming data buffer
  /// to retrieve data from.
  int _bufferNext; 

  /// The number of gates
  unsigned int _gates;

  /// The number of samples
  unsigned int _samples;

  /// dual prt true or false
  unsigned int _dualPrt;

  /// the start gate number of IQ capture
  unsigned int _startGateIQ;

  /// The number of IQ gates to capture
  unsigned int _nGatesIQ;
};


#endif
