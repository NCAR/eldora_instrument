#ifndef RR314_H
#define RR314_H

#include <sys/types.h>
#include <sys/stat.h>

#include <string>
#include <vector>
#include <deque>
#include <map>

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

#define DMANUMGROUPS      8
#define DMABLOCKSIZEBYTES 4008
#define DMABLOCKSPERGROUP 10
#define BUFFERPOOLSIZE    2*DMANUMGROUPS
#define DMA_GROUPS_PER_INT 10

void shutdownSignalHandler(int signo);

namespace RedRapids {
//////////////////////////////////////////////////////////////////////
///
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
	     unsigned int decimationFactor, ///< Downconvertor decimation count
	     std::string gaussianFile,      ///< The file of gaussian filter coefficients,blank if none.
	     std::string kaiserFile,        ///< The file of kaiser filter coefficients, blank if none.
	     std::string xsvfFileName       ///< The xsvfFile to be loaded. Blank if not to be loaded.
     );

  /// Destructor
  virtual ~RR314();
  
  /// A map of our instances, indexed by the channel adapter
  /// variabble pointer. Use by C routines to find our instance.
  static std::map<s_ChannelAdapter*, RR314*> rr314Instances;

  /// Start the data capture and processing. The 
  /// FPGA processing is started by enabling the
  /// kasier filter and the A/D capture, and the dma 
  /// transfer is enabled for the V4
  void start();

  /// @return The cumulative number of bytes processed
  int bytes();

  ///@return The cumulative number of bytes processed for a given channel
  int bytes(int chan);

  /// accumlate byte counts
  /// @param chan The channel
  /// @param bytes Add these bytes
  void addBytes(int chan, int bytes);

  /// set the last DMA transfer group
  /// @param chan The channel
  /// @param group The group
  void lastGroup(int chan, int group);

  /// @return The last DMA transfer group
  /// @param chan The channel
  int lastGroup(int chan);

  /// set the number of fifofull interrupts
  void fifoFullInts(int n);

  /// 2retun The number of fifoFull interrupts
  int fifoFullInts();

  /// get some info about the board
  void boardInfo();

  /// A mutex used to protect access to the data queues.
  pthread_mutex_t bufferMutex;

  /// A queue of available empty buffers
  std::deque<short*> freeBuffers;

  /// A queue of buffers with data to be processed.
  std::deque<short*> fullBuffers;

 protected:

  /// The fifo file descriptor for receiving data. 
  int _deviceFd;

  /// Configure ignal catching so that DMA operations
  /// and memory can be cleaned up on a signal
  void catchSignals();

  /// Configure the card and DMA operations
  int configure314();

  /// configure the filters and the decimation value
  int filterSetup();

  /// program the cofficients for the gaussian and
  /// kaiser filters
  bool loadFilters(FilterSpec& gaussian, FilterSpec& kaiser);

  /// Confirure the timers
  void timerInit();

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

  /// the number of bytes captured for each channel
  int _bytes[16];

  /// the last DMA group transferred
  int _lastGroup[16];

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

  /// The number of fifo full interrupts 
  int _fifoFullInts;
};
}

 
#endif
