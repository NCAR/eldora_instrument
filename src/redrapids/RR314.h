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

#include <boost/date_time/posix_time/posix_time.hpp>

#include "FilterSpec.h"
#include "DDCregisters.h"

// RR dmaChan adapter includes
#include "ca_bar0_memmap.h"
#include "ca_diag_memmap.h"
#include "ca_lb_memmap.h"
#include "channeladapter.h"
#include "ca_functions.h"
#include "ca_membuffer.h"

#define POW31 ((unsigned int)pow(2, 31.0))
#define POW32 ((unsigned int)pow(2, 32.0))
/// The scale factor for converting the AB ints to floats
/// in milliwatts into a 50 ohm load
#define ABSCALE ( (1.0/POW31)*(1.0/(2.0*50))*1000.0 )
/// The scale factor for converting the AB ints to floats
/// in milliwatts into a 50 ohm load
#define PSCALE  ( (1.0/POW32)*(1.0/(2.0*50))*1000.0 )
/// The number of groups in a DMA transfer.
#define DMANUMGROUPS      8
/// The size of each DMA block.
#define DMABLOCKSIZEBYTES 1024
/// The number of DMA blocks per DMA groups.
#define DMABLOCKSPERGROUP 16
/// The number of buffers preallocated in each (iq and abp) buffer pool.
#define BUFFERPOOLSIZE  1000

/// shutdownSignalHandler is a handler that is called for 
/// various shutdown signals. It's job is to force windrvr6 
/// to return memory. Signal handlers are register ted call 
/// this routine when some signals are received.
/// @param signo The signal number will be delivered here.
/// @param userData User data is delivered here, but unused in this function.
void shutdownSignalHandler(int signo, void* userData);

namespace RedRapids {

class RRBuffer;
class RRABPBuffer;
class RRIQBuffer;
class RR314sim;

//////////////////////////////////////////////////////////////////////
///
/// Representation of the Red Rapids 314 4 channel receiver.
/// The class manages the configuration of the receiver card,
/// allocation of the DMA buffers, and the response to DMA buffer
/// full interrupts.
///
/// Two types of data are handled: abp data, which consists of
/// abp values for all gates, and iq data, which consists of 
/// i and q values for a selected series of gates within one pulse.
/// These data are accumulated in the corresponding buffer types
/// RRABPBuffer and RRIQBuffer. The IQ data are delivered from the RR314 on
/// DMA channels 0,2, 4 and 6. The ABP data are delivered from the RR314 on 
/// DMA chanels 1, 3, 5 and 7.
///
/// The data are collected in buffers, derived from RRBuffer. The ABP
/// values for one set of gates, which cover measurements made over 
/// one coherent integration period, are placed into an indivdual RRABPBuffer. 
/// 'Samples' number of timeseries i and q values are aggregated into 
/// one RRIQBuffer. Thus the ABP and IQ buffers will be produced at the same
/// rate.
///
/// The filled buffers are placed in a holding queue. These
/// buffers are delivered to a client when the client calls the
/// nextBuffer() function. A condition variable in nextBuffer()
/// allows the reading thread to suspend while waiting for 
/// a new holding buffer to become filled. Buffers that are
/// passed to the consumer must be returned to RR314 via returnBuffer()
/// when the consumer is finshed with them.
///
class RR314 {

public:

	/// Constructor
    /// @param devnum The device number.
    /// @param gates The number of gates.
    /// @param prf The pulse repeition frequency in Hz.
    /// @param pulsewidth Pulse width in ns, used to set the downconvertor decimation count.
    /// @param samples Number of samples in a timeseries block, or used in the 
    /// calculation of a single ABP ray.
    /// @param dualPrt True if operating in dual prt mode.
    /// @param internalTimer True if we will use the external timer
    /// @param startGateIQ The starting gate for IQ capture.
    /// @param nGatesIQ The number of successive gtes for IQ capture.
	/// @param gausianFile The file of gaussian filter coefficients,blank if none.
	/// @param kaiserFile The file of kaiser filter coefficients, blank if none.
	/// @param xsvfFile The xsvfFile to be loaded. Blank if not to be loaded.
	/// @param simulate Set true if simulation instead of real hardware
    /// @param usleep The sleep value for the simulator, if in simulation mode
    /// @param catchSignals Set true if the class is supposed to catch signals and
    /// correctly shutdown the cards. If the user decides to catch signals 
    /// themself (by setting this to false), they MUST call RR314shutdown in order 
    /// to terminate the DMA transfers correctly.
	RR314(int devNum, 
	        unsigned int gates, 
	        unsigned int prf,
	        unsigned int pulsewidth, 
	        unsigned int samples,
	        unsigned int dualPrt, 
	        bool internalTimer,
	        unsigned int startGateIQ,
	        unsigned int nGatesIQ, 
	        std::string gaussianFile,
	        std::string kaiserFile, 
	        std::string xsvfFile,
	        bool simulate = false, 
	        int usleep = 30000,
	        bool catchSignals = true)
                throw (std::string);

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

	/// Accept new iq data for this class. This function
	/// may be called from other threads. It will 
	/// copy the data into a free buffer, collecting
	/// values in that buffer until a _numIQGates pairs plus the 
	/// housekeeping data have been satisfied. When a
	/// complete series has been assembled (_samples samples), data 
	/// availability is signaled with a broadcast on _dataAvailCond.
	/// @param src The source of the data
	/// @param chan The channel of this data
	/// @param n The number of items
	void newIQData(short* src, int chan, int n);

	/// Accept new abp data for this class. This function
	/// may be called from other threads. It will 
	/// copy the data into a free buffer, collecting
	/// values in that buffer until a _numIQGates pairs plus the 
	/// housekeeping data have been satisfied. When a
	/// complete series has been assembled, data availability 
	/// is signaled with a broadcast on _dataAvailCond.
	/// @param src The source of the data
	/// @param chan The channel of this data
	/// @param n The number of items
	void newABPData(int* src, int chan, int n);

	/// Return the next available data buffer. 
	/// If there
	/// is no data available, wait until there is.
	/// The caller must return the buffer when finished
	/// by calling its returnBuffer() method.
	/// @return A buffer of data
	RRBuffer* nextBuffer();

	/// Return the used buffer to the pool.
	/// @param buf The buffer to be returned.
	void returnBuffer(RRBuffer* buf);

	/// Return The number of free ABP buffers
	/// @return The number of free buffers
	int numFreeABPBuffers();

	/// Return The number of free iq buffers
	/// @return The number of free buffers
	int numFreeIQBuffers();

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
	
	/// @return The board temperature (degC). If in
	/// simulate mode, return a random but reasonable value
	double temperature();
	
	/// @return The board number
	int boardNumber();
	
	/// Set the time of the first transmit pulse.
	/// @param startTime The boost::posix_time::ptime of the first transmit
	///    pulse.
	void setXmitStartTime(boost::posix_time::ptime startTime);
	
	/// Time of first transmit pulse
	boost::posix_time::ptime xmitStartTime() {
	    return _xmitStartTime;
	}
	
	/// Dwell duration (= nsamples / PRF).
	boost::posix_time::time_duration dwellDuration() {
	    return boost::posix_time::microseconds((1000000 * _samples) / _prf);
	}

	/// Ray time (at the middle of the ray integration period)
	boost::posix_time::ptime rayTime(unsigned int rayNum) {
	    // The math is a little convoluted here because multiply and divide
	    // operations for posix_time::time_duration only take integer operands.
		return _xmitStartTime + (dwellDuration() * (2 * rayNum + 1)) / 2;
	}
	
	/// Samples per ray
	unsigned int samples() { return _samples; }
	
	/// First IQ gate
	unsigned int firstIQGate() { return _startGateIQ; }
	
	/// Number of IQ gates
	unsigned int numIQGates() { return _numIQGates; }
	
	/// Starts the timers.
	void startInternalTimer();
protected:

	/// Configure the card and DMA operations.
	int configure314();

	/// Configure the filters and the decimation value.
	int filterSetup();

	/// Program the cofficients for the gaussian and
	/// kaiser filters.
	bool loadFilters(FilterSpec& gaussian, FilterSpec& kaiser);

	/// Configure the pulse pair processor.
	/// @return true if succesful, false otherwise.
	bool pulsepairInit();
	
	/// Configure the timers.
	/// @return true if succesful, false otherwise.
	bool timerInit();
		
	/// Accumlulate byte counts.
	/// @param chan The channel
	/// @param bytes Add these bytes
	void addBytes(int chan, int bytes);

	/// Mutex that protects access to _freeBuffers and _fullBuffers.
	pthread_mutex_t _bufferMutex;

	/// The condition variable used to trigger the 
	/// data available condition.
	pthread_cond_t _dataAvailCond;

	/// A queue of buffers with data to be processed.
	std::deque<RRBuffer*> _fullBuffers;

	/// A queue of available empty iq buffers.
	std::deque<RRIQBuffer*> _freeIQBuffers;

	/// A queue of available empty buffers.
	std::deque<RRABPBuffer*> _freeABPBuffers;

	/// The current IQ buffers being filled, one per
	/// DMA channel. Once the 
	/// buffer for a channel is filled, it is transfered
	/// to the _fullBuffer list and the condition
	/// variable is signalled.
	std::map<int, RRIQBuffer*> _currentIQBuffer;

	/// The current ABP buffers being filled, one per
	/// DMA channel. Once the 
	/// buffer for a channel is filled, it is transfered
	/// to the _fullBuffer list and the condition
	/// variable is signalled.
	std::map<int, RRABPBuffer*> _currentABPBuffer;

	/// The number of bytes captured for each channel.
	std::vector<unsigned long> _bytes;

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
	
	/// The prf in hz,
	int _prf;
	
	/// The pulsewidth in ns.
	int _pulsewidth;

	/// The number of samples that go into each ray.
	unsigned int _samples;

    /// The number of IQ gates to capture.
    unsigned int _numIQGates;

	/// Dual prt true or false.
	unsigned int _dualPrt;

	/// The start gate number of IQ capture.
	unsigned int _startGateIQ;

    /// The path to the file containing the gaussian filter definitions.
    std::string _gaussianFile;

    /// The path to the file containing the kaiser filter coefficients.
    std::string _kaiserFile;

    /// The path to a RR bitstream file, if it will be
    /// loaded.
    std::string _xsvfFileName;

	/// Set true if simulate
	bool _simulate;

	/// The number of fifo full interrupts. 
	int _fifoFullInts;

	/// The simulator
	RedRapids::RR314sim* _simulator;
	
	/// Set true when running, false when stopped.
	bool _running;
	
	/// Set true if signal capture will be configured and 
	/// managed within this class.
	bool _catchSignals;
	
	/// The sleep value sent to the simulator when in simulation mode
	int _usleep;
	
	/// Use the internal timer
	bool _internalTimer;
	
	/// Time of the first xmit pulse.
	boost::posix_time::ptime _xmitStartTime;
};

class RRBuffer {
public:
    /// A virtual member is required in order for dynamic_cast
    /// to work.
    virtual ~RRBuffer() {}

    /// The DMA channel number
    int dmaChan;
    /// The channel id delivered by the RR314
    unsigned int chanId;
    /// The prt id delivered by the RR314
    unsigned int prtId;
    /// The ray number
    unsigned int rayNum;
    /// The time for this ray (middle of the integration period).
    boost::posix_time::ptime rayTime;
    /// The type of derived class
    enum {IQtype, ABPtype} type;
    /// the number of coherent integrations
    int nci;
    /// Return this buffer to its parent's free list.
    void returnBuffer() { _parent->returnBuffer(this); }

    /// board number
    int boardNumber() const { return _parent->boardNumber(); }
    
    /// time of the first transmit pulse
    boost::posix_time::ptime xmitStartTime() const { 
        return _parent->xmitStartTime(); 
    }
    
    /// dwell time
    boost::posix_time::time_duration dwellDuration() const { 
        return _parent->dwellDuration(); 
    }
    
    /// Parent RR314
    RR314* parent() const { return _parent; }
protected:
    RRBuffer(RR314* parent) : _parent(parent) {};
    // pointer to the parent RR314
    RR314* _parent;
};
/// A buffer type used to collect an incoming abp sample stream and 
/// for deliver to consumers. The buffer will always contain one
/// complete set of ABP values for all gates.
class RRABPBuffer : public RRBuffer {
public:
    RRABPBuffer(RR314* parent) : RRBuffer(parent) {};
    
    // position in the data for the current ray
    unsigned int _posInRay;
    // the ABP data
    std::vector<float> _abp;
};

/// A buffer type used to collect an incoming iq sample stream and 
/// for deliver to consumers. _samples number of iq sequences,
/// each sequence covering the range of iq gates, are aggregated
/// into one RRIQBuffer.
class RRIQBuffer : public RRBuffer {
public:
    RRIQBuffer(RR314* parent) : RRBuffer(parent) {};
    
    /// xmit pulse of the current sample
    unsigned int _pulseCount;
    /// position in the data for the current sample
    unsigned int _posInSample;
    /// number of samples completely filled so far
    unsigned int _samplesFilled;
    /// the IQ data
    std::vector<short> _iq;
};

}
#endif
