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
#define DMA_GROUPS_PER_INT 10

#define BUFFERPOOLSIZE  1000

/// A handler that is called for varoiuos shutdown signals.
/// It's job is to force windrvr6 to return memory.
/// It appears that this doesn't really work. After
/// successively running RRSnarfer, Linux runs out of memory.
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
  RR314(int devNum,
	unsigned int gates,
	     unsigned int samples,
	     unsigned int dualPrt,
	     unsigned int startGateIQ,
	     unsigned int nGatesIQ,
	     unsigned int decimationFactor, ///< Downconvertor decimation count
	     std::string gaussianFile,      ///< The file of gaussian filter coefficients,blank if none.
	     std::string kaiserFile,        ///< The file of kaiser filter coefficients, blank if none.
	     std::string xsvfFileName       ///< The xsvfFile to be loaded. Blank if not to be loaded.
	) throw (std::string);

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

  /// Accept new data for this class. This function
  /// may be called from other threads. It will 
  /// copy the data into a free buffer, and signal
  /// the data availability with a broadcast on
  /// _dataAvailCond.
  /// @param src The source of the data
  /// @param chan The channel of this data
  /// @param n The number of data elements in src to be transferred
  void newData(unsigned int* src, int chan, int n);

  /// Return the next available data buffer. If there
  /// is no data available, wait until there is.
  /// The caller must return the buffer when finished
  /// by calling returnBuffer().
  /// @return A buffer of data
  int* nextBuffer();

  /// Return the used buffer to the pool.
  /// @param buf The buffer to be returned.
  void returnBuffer(int* buf);

  /// @return The number of free buffers
  int numFreeBuffers();

  /// @return The cumulative number of bytes processed
  unsigned long bytes();

  ///@return The cumulative number of bytes processed for a given channel
  unsigned long bytes(int chan);

  /// @return The number of fifoFull interrupts
  int fifoFullInts();

  /// get some info about the board
  void boardInfo();

  /// set the last DMA transfer group
  /// @param chan The channel
  /// @param group The group
  void lastGroup(int chan, int group);

  /// @return The last DMA transfer group
  /// @param chan The channel
  int lastGroup(int chan);

  /// set the number of fifofull interrupts
  void fifoFullInts(int n);

  /// accumluate byte counts
  /// @param chan The channel
  /// @param bytes Add these bytes
  void addBytes(int chan, int bytes);

 protected:

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

  /// Configure the timers
  void timerInit();

  /// stop the RR card and return allocated space
  void shutdown();

  /// mutex that protects access to _freeBuffers and _fullBuffers
  pthread_mutex_t _bufferMutex;

  /// The condition variable used to trigger the 
  /// data available condition
  pthread_cond_t _dataAvailCond;

  /// A queue of available empty buffers
  std::deque<int*> _freeBuffers;

  /// A queue of buffers with data to be processed.
  std::deque<int*> _fullBuffers;

  /// The gaussian filter decimation factor (1-127)
  unsigned int _decimationFactor;

  /// The path to the file containing the gaussian filter definitions
  std::string _gaussianFile;

  /// The path to the file containing the kaiser filter coefficients
  std::string _kaiserFile;

  /// The path to a RR bitstream file, if it will be
  /// loaded
  std::string _xsvfFileName;

  /// the number of bytes captured for each channel
  unsigned long  _bytes[16];

  /// the last DMA group transferred
  int _lastGroup[16];

  /// The channel adapter, used in RR interfaces
  s_ChannelAdapter _chanAdapter;

  /// Clock setting for the M314, used in RR interfaces.
  s_ClkSettings _ClkSettings;

  /// set true if the DMA allocation has been made
  bool _dmaAllocated;

  /// points to the next location in an incoming data buffer
  /// to retrieve data from.
  int _bufferNext; 

  /// The RR314 device number, starting at 0.
  int _devNum;

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
