#ifndef RR314_H
#define RR314_H

#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

#include <string>
#include <vector>
#include <deque>
#include <map>

#include <stdio.h>
#include <stdlib.h>

#include "FilterSpec.h"
#include "DDCregisters.h"

// RR channel adapter includes
#include "ca_bar0_memmap.h"
#include "ca_diag_memmap.h"
#include "ca_lb_memmap.h"
#include "channeladapter.h"
#include "ca_functions.h"
#include "ca_membuffer.h"

/// The number of groups in a DMA transfer.
#define DMANUMGROUPS      8
/// The size of each DMA block.
#define DMABLOCKSIZEBYTES 4008
/// The number of DMA blocks per DMA groups.
#define DMABLOCKSPERGROUP 10
/// The number of buffers preallocated in the buffer pool.
#define BUFFERPOOLSIZE  1000

/// shutdownSignalHandler is a handler that is called for 
/// various shutdown signals. It's job is to force windrvr6 
/// to return memory. Signal handlers are register ted call 
/// this routine when some signals are received.
void shutdownSignalHandler(int signo);

namespace RedRapids {

  class RR314sim;
  struct RRbuffer {
    std::vector<int> _data;
    int channel;
    int nSamples;
  };

  //////////////////////////////////////////////////////////////////////
  ///
  /// Representation of the Red Rapids 314 4 channel receiver.
  /// The class manages the configuration of the receiver card,
  /// allocation of the DMA buffers, and responding to DMA buffer
  /// full interrupts.
  ///
  /// The filled DMA buffers are passed to a holding buffer. These
  /// buffers are delivered to a client when the client calls the
  /// nextBuffer() function. A condition variable in nextBuffer()
  /// allows the reading thread to suspend while waiting for 
  /// a new holding buffer to become filled. Buffers that are
  /// passed to the consumer must be returned to RR314 via returnBuffer()
  /// when the consumer is finshed with them.
  ///
  class RR314
  {

  public:

    /// Constructor
    /// @param decimationFactor Downconvertor decimation count
    /// @param gausianFile The file of gaussian filter coefficients,blank if none.
    /// @param kaiserFile The file of kaiser filter coefficients, blank if none.
    /// @param xsvfFile The xsvfFile to be loaded. Blank if not to be loaded.
    /// @param simulate Set true if simulation instead of real hardware
    RR314(int devNum,
	  unsigned int gates,
	  unsigned int samples,
	  unsigned int dualPrt,
	  unsigned int startGateIQ,
	  unsigned int nGatesIQ,
	  unsigned int decimationFactor, 
	  std::string gaussianFile,      
	  std::string kaiserFile, 
	  std::string xsvfFile,
	  bool simulate = false
	  ) throw (std::string);

    /// Destructor
    virtual ~RR314();
  
    /// A map of our instances, indexed by the channel adapter
    /// variable pointer. Use by C routines to find our instance.
    static std::map<s_ChannelAdapter*, RR314*> rr314Instances;

    /// Start the data capture and processing. The 
    /// FPGA processing is started by enabling the
    /// kasier filter and the A/D capture, and the dma 
    /// transfer is enabled for the V4.
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

    /// Return the next available data buffer. 
    /// If there
    /// is no data available, wait until there is.
    /// The caller must return the buffer when finished
    /// by calling returnBuffer().
    /// @return A buffer of data
    RRbuffer* nextBuffer();

    /// Return the used buffer to the pool.
    /// @param buf The buffer to be returned.
    void returnBuffer(RRbuffer* buf);

    /// Return The number of free buffers
    /// @return The number of free buffers
    int numFreeBuffers();

    /// Get the cumulative number of bytes processed. 
    /// @return The cumulative number of bytes processed since 
    /// the last call to this function.
    std::vector<unsigned long> bytes();

    /// Return The number of fifoFull interrupts.
    /// @return The number of fifoFull interrupts.
    int fifoFullInts();

    /// Get some info about the board.
    void boardInfo();

    /// Set the last DMA transfer group.
    /// @param chan The channel
    /// @param group The group
    void lastGroup(int chan, int group);

    /// Get the last DMA group processed for a channel.
    /// @return The last DMA transfer group
    /// @param chan The channel
    int lastGroup(int chan);

    /// Set the number of fifofull interrupts.
    void fifoFullInts(int n);

    /// Stop the RR card and return DMA allocated space.
    void RR314shutdown();

  protected:

    /// Configure ignal catching so that DMA operations
    /// and memory can be cleaned up on a signal.
    void catchSignals();

    /// Configure the card and DMA operations.
    int configure314();

    /// Configure the filters and the decimation value.
    int filterSetup();

    /// Program the cofficients for the gaussian and
    /// kaiser filters.
    bool loadFilters(FilterSpec& gaussian, FilterSpec& kaiser);

    /// Configure the timers.
    void timerInit();

    /// Accumlulate byte counts.
    /// @param chan The channel
    /// @param bytes Add these bytes
    void addBytes(int chan, int bytes);

    /// Mutex that protects access to _freeBuffers and _fullBuffers.
    pthread_mutex_t _bufferMutex;

    /// The condition variable used to trigger the 
    /// data available condition.
    pthread_cond_t _dataAvailCond;

    /// A queue of available empty buffers.
    std::deque<RRbuffer*> _freeBuffers;

    /// A queue of buffers with data to be processed.
    std::deque<RRbuffer*> _fullBuffers;

    /// The gaussian filter decimation factor (1-127).
    unsigned int _decimationFactor;

    /// The path to the file containing the gaussian filter definitions.
    std::string _gaussianFile;

    /// The path to the file containing the kaiser filter coefficients.
    std::string _kaiserFile;

    /// The path to a RR bitstream file, if it will be
    /// loaded.
    std::string _xsvfFileName;

    /// The number of bytes captured for each channel.
    std::vector<unsigned long>  _bytes;

    /// The last DMA group transferred.
    int _lastGroup[8];

    /// The channel adapter, used in RR interfaces.
    s_ChannelAdapter _chanAdapter;

    /// Clock setting for the M314, used in RR interfaces.
    s_ClkSettings _ClkSettings;

    /// Set true if the DMA allocation has been made.
    //  bool _dmaAllocated;

    /// Points to the next location in an incoming data buffer
    /// to retrieve data from.
    int _bufferNext; 

    /// The RR314 device number, starting at 0.
    int _devNum;

    /// The number of gates.
    unsigned int _gates;

    /// The number of samples.
    unsigned int _samples;

    /// Dual prt true or false.
    unsigned int _dualPrt;

    /// The start gate number of IQ capture.
    unsigned int _startGateIQ;

    /// The number of IQ gates to capture.
    unsigned int _nGatesIQ;

    /// The number of fifo full interrupts. 
    int _fifoFullInts;

    /// Set true if simulate
    bool _simulate;

    /// The simulator
    RedRapids::RR314sim* _simulator;
  };
}
#endif
