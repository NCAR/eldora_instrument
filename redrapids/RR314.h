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

/// Representation of the Red Rapids 314 4 channel receiver.
/// The class manages the configuration of the receiver card,
/// allocation of the DMA buffers, and responding to DMA buffer
/// full interrupts.
///
/// The filled DMA buffers are passed to a holding buffer.
/// A condition variable will be set to allow a reading thread
/// to empty the holding buffer.
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
