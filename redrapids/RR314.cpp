#include "RR314.h"
#include "RR314sim.h"
#include "SignalCatcher.h"
#include <signal.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <iostream>
#include <deque>
#include <map>

using namespace RedRapids;

// instantiate the map which will record all instances
// of the RR314 class.
std::map<s_ChannelAdapter*, RR314*> RedRapids::RR314::rr314Instances;

//////////////////////////////////////////////////////////////////////

RR314::RR314(int devNum,
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
             bool simulate,
             int usleep,
             bool catchSignals) throw(std::string) :
    _devNum(devNum), _gates(gates), _prf(prf), _pulsewidth(pulsewidth),
            _samples(samples), _dualPrt(dualPrt),
            _internalTimer(internalTimer), _startGateIQ(startGateIQ),
            _numIQ(nGatesIQ), _gaussianFile(gaussianFile),
            _kaiserFile(kaiserFile), _xsvfFileName(xsvfFile),
            _simulate(simulate), _catchSignals(catchSignals), _bufferNext(0),
            _running(false), _usleep(usleep) {

    // initalize threading constructs
    pthread_mutex_init(&_bufferMutex, NULL);
    pthread_cond_init(&_dataAvailCond, NULL);

    // capture signals

    if (_catchSignals) {
        SignalCatcher::instance()->configure(0, shutdownSignalHandler);
    }

    if (simulate) {
        std::cout
                << "*** RR314 operating in simulation mode, without hardware\n";
    } else {
        std::cout << "*** This version of RR314 works with CA_DDC_4.xsvf\n";
    }

    // save a reference to our instance so that the isr
    // can locate us
    rr314Instances[&_chanAdapter] = this;

    // initialize the byte counters and last groups
    _bytes.resize(8);
    for (int c = 0; c < 8; c++) {
        _bytes[c] = 0;
        lastGroup(c, 0);
    }

    // allocate the buffers
    pthread_mutex_lock(&_bufferMutex);
    for (int i = 0; i < BUFFERPOOLSIZE; i++) {
        RRABPBuffer* abpBuf = new RRABPBuffer;
        abpBuf->_abp.resize(3*_gates);
        abpBuf->nextData = 0;
        abpBuf->dataIn = 0;
        abpBuf->type = RRBuffer::ABPtype;
        abpBuf->nci = _samples;
        _freeABPBuffers.push_back(abpBuf);
        RRIQBuffer* iqBuf = new RRIQBuffer;
        iqBuf->_iq.resize(2*_numIQ*_samples);
        iqBuf->nextData = 0;
        iqBuf->dataIn = 0;
        iqBuf->type = RRBuffer::IQtype;
        iqBuf->nci = _samples;
        _freeIQBuffers.push_back(iqBuf);
    }

    // populate the current buffers list
    // with empty buffers
    for (int chan = 0; chan < 8; chan += 2) {
        // even DMA channels are IQ
        _currentIQBuffer[chan] = _freeIQBuffers[0];
        _currentIQBuffer[chan]->nextData = 0;
        _currentIQBuffer[chan]->dmaChan = chan;
        _freeIQBuffers.pop_front();

        // odd DMA channels are ABP
        _currentABPBuffer[chan+1] = _freeABPBuffers[0];
        _currentABPBuffer[chan+1]->nextData = 0;
        _currentABPBuffer[chan+1]->dmaChan = chan;
        _freeABPBuffers.pop_front();
    }
    pthread_mutex_unlock(&_bufferMutex);

    if (_simulate) {
        _simulator = new RedRapids::RR314sim(this,
                _gates,
                _startGateIQ,
                _numIQ,
                _samples,
                _usleep);
        return;
    }

    std::string deviceName = "/dev/windrvr6";

    int deviceFd = open(deviceName.c_str(), O_RDONLY | O_NONBLOCK);

    if (deviceFd < 0) {
        std::string e("cannot access ");
        e += deviceName;
        throw(e);
    }

    // configure the card
    if (configure314()) {
        std::string e("Unable to configure the Red Rapids card ");
        throw(e);
    }

    // display some board information
    boardInfo();

}

////////////////////////////////////////////////////////////////////////

RR314::~RR314() {
    // disable the hardware
    if (!_simulate) {
        RR314shutdown();
    } else {
        /// @todo Need to add thread termination for RR314sim
        /// during class destruction
        delete _simulator;
    }

    for (unsigned int i = 0; i < _fullBuffers.size(); i++)
        delete _fullBuffers[i];

    for (unsigned int i = 0; i < _freeIQBuffers.size(); i++)
        delete _freeIQBuffers[i];

    for (unsigned int i = 0; i < _freeABPBuffers.size(); i++)
        delete _freeABPBuffers[i];

    std::cout << "RR314 deleted\n";
}

//////////////////////////////////////////////////////////////////////

void RR314::RR314shutdown() {

    // ignore multiple calls to shutdown the card.
    if (!_running)
        return;

    _running = false;

    // if in simulation, don't try to access the card. 
    // this can happen when a signal is handled while in
    // simulation mode
    if (_simulate)
        return;

    Adapter_Write32(&_chanAdapter, BRIDGE, BRG_INTRMASK_ADR, BRG_INTR_DIS);
    Adapter_Write32(&_chanAdapter, V4, V4_CTL_ADR, 0x0);

    // sleep to allow dma transfers to finish.
    sleep(1);

    Adapter_DMABufFree(&_chanAdapter);

    Adapter_Close(&_chanAdapter);

}

//////////////////////////////////////////////////////////////////////

bool RR314::loadFilters(FilterSpec& gaussian,
                        FilterSpec& kaiser) {

    bool kaiserLoaded;
    bool gaussianLoaded;

    int attempt;

    // program kaiser coefficients

    attempt = 0;
    do {
        kaiserLoaded = true;
        for (unsigned int i = 0; i < kaiser.size(); i++) {

            unsigned int readBack;
            int ramAddr = i/4;
            int ramSelect = i%4 << 4;
            int addr = 0x1000 | ramSelect | ramAddr;

            // set the address
            Adapter_Write32(&_chanAdapter, V4, 0x934, addr);

            // write the value
            Adapter_Write32(&_chanAdapter, V4, 0x938, kaiser[i]);

            // enable writing
            Adapter_Write32(&_chanAdapter, V4, 0x94C, 1);

            // disable writing (kaiser readback only succeeds if we do this)
            Adapter_Write32(&_chanAdapter, V4, 0x94C, 0);

            // read back the programmed value
            Adapter_Read32(&_chanAdapter, V4, 0x93c, &readBack);

            if (readBack != kaiser[i]) {
                std::cout << "kaiser readback failed for coefficient "
                        << std::dec << i << std::hex << ", wrote " << kaiser[i]
                        << ", read " << readBack << std::endl;

                kaiserLoaded = false;
            }
        }
        attempt++;
    } while (!kaiserLoaded && attempt < 50);

    if (kaiserLoaded) {
        std::cout << kaiser.size()
                << " Kaiser filter coefficients succesfully loaded\n";
    } else {
        std::cout << "Unable to load the Kaiser filter coefficients\n";
    }

    // program gaussian coefficients
    attempt = 0;

    do {
        gaussianLoaded = true;
        for (unsigned int i = 0; i< gaussian.size(); i++) {

            unsigned int readBack;
            int ramAddr = i%12;
            int ramSelect = i/12 << 4;
            int addr = ramSelect | ramAddr;

            // set the address
            Adapter_Write32(&_chanAdapter, V4, 0x940, addr);

            // write the value
            Adapter_Write32(&_chanAdapter, V4, 0x944, gaussian[i]);

            // enable writing
            Adapter_Write32(&_chanAdapter, V4, 0x950, 1);

            //disable writing
            Adapter_Write32(&_chanAdapter, V4, 0x950, 0);

            // read back the programmed value
            Adapter_Read32(&_chanAdapter, V4, 0x948, &readBack);

            if (readBack != gaussian[i]) {
                std::cout << "gaussian readback failed for coefficient "
                        << std::dec << i << std::hex << ", wrote "
                        << gaussian[i] << ", read " << readBack << std::endl;

                gaussianLoaded = false;
            }
        }
        attempt++;
    } while (!gaussianLoaded && attempt < 50);

    if (gaussianLoaded) {
        std::cout << gaussian.size()
                << " Gaussian filter coefficients succesfully loaded\n";
    } else {
        std::cout << "Unable to load the Gaussian filter coefficients\n";
    }

    return kaiserLoaded && gaussianLoaded;

}

//////////////////////////////////////////////////////////////////////

int RR314::configure314() {

    unsigned int result;

    //Flags
    int ProgramFPGA; // program fpga
    ProgramFPGA = 0;

    int V4LoadCheck; // if set, verify that V4 has been loaded
    V4LoadCheck = _xsvfFileName.size()>0;
    V4LoadCheck = 1;

    //Init all flags to default values
    Adapter_Zero(&_chanAdapter);

    // The device number
    _chanAdapter.DevNum = _devNum;

    // set assembly information, whatever that is
    strcpy(_chanAdapter.Asy, "M314");

    // Open dmaChan adapter
    if (Adapter_Open(&_chanAdapter)) {
        fprintf(stderr,
        "Failed to find and/or open card. (%x)\n",
        _chanAdapter.ReturnStatus);
        return -1;
    } else {
        printf("Opened ChannelAdapter device %d\n", _chanAdapter.DevNum);
    }

    // send a hard reset to the card
    Adapter_Write32(&_chanAdapter, BRIDGE, BRG_FPGARESET_ADR, 1);
    Adapter_Write32(&_chanAdapter, BRIDGE, BRG_FPGARESET_ADR, 0);

    // Disable all interupts
    Adapter_Write32(&_chanAdapter, V4, V4_MASK_ADR, 0x0);

    //Load the v4 if needed
    if (_xsvfFileName.size() > 0) {
        std::cout << "Loading bitstream " << _xsvfFileName << std::endl;
        char name[_xsvfFileName.size()+1];
        strcpy(name, _xsvfFileName.c_str());
        if (Adapter_LoadXSVF(&_chanAdapter, name, 1)) {
            printf("V4 and / or PROM failed to load, will now exit...\n");
            Adapter_Close(&_chanAdapter);
            return -1;
        }
    }

    // set up the filters. Will do nothing if either of
    // the filter file paths is empty.
    if (filterSetup()) {
        // error initializing the filters
        return -1;
    }

    // set the sample clock
    _ClkSettings.ClkSrc = SYNTH; // can be either SYNTH or EXT
    Adapter_SampleClkSelect(&_chanAdapter, &_ClkSettings);
    sleep(1); //Allow time for synthesizer to lock.
    // Code here should reset your DCMs and allow time for them to lock,
    // then check their status
    // Soft Reset, self clearing
    Adapter_Write32(&_chanAdapter, V4, V4_CTL_ADR, SOFT_RST);
    sleep(1);
    // Check for DCM Lock
    Adapter_Read32(&_chanAdapter, V4, V4_STAT_ADR, &result); //Clear old status reg
    Adapter_Read32(&_chanAdapter, V4, V4_STAT_ADR, &result);
    if (result & ADC_DCM_UNLOCKED) {
        printf("ADC DCM Failed to Lock. STATUS REG = %x\n Exiting Now...\n",
               result);
        Adapter_Close(&_chanAdapter);
        return -1;
    }

    // Reset Timer DCM
    Adapter_Write32(&_chanAdapter, V4, V4_CTL_ADR, TIMER_DCM_RST);
    sleep(1);
    Adapter_Read32(&_chanAdapter, V4, V4_STAT_ADR, &result); //clear old status reg
    Adapter_Read32(&_chanAdapter, V4, V4_STAT_ADR, &result);
    if (result & TIMER_DCM_UNLOCKED) {
        printf("Timer DCM Failed to Lock. STATUS REG = %x\n Exiting Now...\n",
               result);
        Adapter_Close(&_chanAdapter);
        return -1;
    }

    // Reset Decimator clocks
    Adapter_Write32(&_chanAdapter, V4, DEC_RST_REG, RST_ACT);
    Adapter_Write32(&_chanAdapter, V4, DEC_RST_REG, RST_CLR);

    // Set M314 VRANGE mode.  This will touch the register that controls the sample
    // clk select, so it is done as RMW.
    // Set ADC range to 2Vpp 
    Adapter_Read32(&_chanAdapter, BRIDGE, BRG_HWCONF_ADR, &result);
    result |= ADCA_VRNG1;
    Adapter_Write32(&_chanAdapter, BRIDGE, BRG_HWCONF_ADR, result);

    // Flush FIFOs
    Adapter_Write32(&_chanAdapter, V4, V4_CTL_ADR, ADCAFF_FLUSH);
    sleep(1);
    Adapter_Write32(&_chanAdapter, V4, V4_CTL_ADR, 0x0);
    sleep(1);
    Adapter_Read32(&_chanAdapter, V4, V4_STAT_ADR, &result); //Clear old status reg


    // reset Pulse Pair Processor
    Adapter_Write32(&_chanAdapter, V4, PP_RST, PP_RST_ACT);
    sleep(1);
    Adapter_Write32(&_chanAdapter, V4, PP_RST, PP_RST_CLR);
    Adapter_Read32(&_chanAdapter, V4, V4_STAT_ADR, &result); //clear old status reg


    // Select Timing Source Internal/External
    if (_internalTimer)
        Adapter_Write32(&_chanAdapter, V4, TIMING_SEL, INTERNAL_TIMING);
    else
        Adapter_Write32(&_chanAdapter, V4, TIMING_SEL, EXTERNAL_TIMING);

    //Enable GPIO
    Adapter_Write32(&_chanAdapter, BRIDGE, BRG_GPIO_ADR, BRG_M314GPIO_EN);

    ///////////////////////////////////////////////////////////////////////////////
    //	
    // Allocate DMA space for each channel. The channel 
    // adapter library will create the scatter/gather 
    // memory references which are then downloaded
    // to the RR card for use by the DMA engine.

    printf("Allocating DMA space...");
    // Bitstream supports 8 DMA channels, indexed to 0
    _chanAdapter.DMA.DMAChannels = 7;

    unsigned int i;
    for (i = 0; i < _chanAdapter.DMA.DMAChannels+1; i++) {
        int chan = i;
        _chanAdapter.DMA.GrpCnt[chan] = DMANUMGROUPS-1;
        _chanAdapter.DMA.BlockSizeB[chan] = DMABLOCKSIZEBYTES;
        _chanAdapter.DMA.BlockCount[chan] = DMABLOCKSPERGROUP-1;
    }

    if (Adapter_DMABufAllocate(&_chanAdapter)) {
        // unable to allocate DMA space
        fprintf(stderr, "Failure allocating DMA memory\n");
        Adapter_Close(&_chanAdapter);
        return -1;
    } else {
        // DMA allocated, load to card
        ca_LoadDMASettings(&_chanAdapter);
    }
    printf("DMA memory allocation done.\n");

    // Enable DMA interrupt on every group for all 
    for (i = 0; i < _chanAdapter.DMA.DMAChannels+1; i++)
        Adapter_Write32(&_chanAdapter, V4, DMA_CH0_GRPSPERINT_ADR+(0x4*i), 0x0);

    //Enable DMA group done interrupt after each group transfer 
    Adapter_Write32(&_chanAdapter, V4, DMA_INT_MASK_ADR, 0x0FFFF);

    //Enable V4 interrupts, 
    Adapter_Write32(&_chanAdapter, V4, V4_MASK_ADR, 
    DMA_GRP_DONE | INTR_EN | ADCA_FF_FULL | ADCB_FF_FULL |
    ADCC_FF_FULL | ADCD_FF_FULL);

    //Allow bridge to create PCI intr
    Adapter_Write32(&_chanAdapter, BRIDGE, BRG_INTRMASK_ADR, BRG_INTR_EN);

    // initialize the pulse pair processor
    if (!pulsepairInit()) {
    	return -1;
    }    
    
    // initialize the timers
    if (_internalTimer) {
        if (!timerInit())
            return -1;
    }

    // Start the DDC
    Adapter_Write32(&_chanAdapter, V4, KAISER_ADDR, 0);
            
    return 0;

}

/////////////////////////////////////////////////////////////////////////
void RR314::newIQData(short* src,
                      int chan,
                      int n) {

    // verify that we got an iq dmaChan
    assert(!(chan & 1) && (chan < 8));

    RRIQBuffer* pBuf = _currentIQBuffer[chan];
    // loop through all src samples
    for (int i = 0; i < n; i++) {
        //				std::cout << "dataIn " << pBuf->dataIn << "  nextData "
        //						<< pBuf->nextData << "   i " << i << "   src[i] " << src[i]
        //						<< "\n";
        //  fill the current buffer from the source
        switch (pBuf->dataIn) {
        case 0:
            pBuf->chanId = src[i];
            pBuf->dmaChan = chan;
            pBuf->dataIn++;
            break;
        case 1:
            pBuf->chanId = (src[i]<< 16) | (pBuf->chanId & 0xffff);
            pBuf->dataIn++;
            if (pBuf->chanId != (chan+1)) {
                std::cout << "RR data error, channel id should be " << chan+1
                        << ", got " << pBuf->chanId << " instead\n";
                pBuf->chanId = chan + 1;
            }
            pBuf->chanId = pBuf->chanId/2 + 1;

            break;
        case 2:
            pBuf->prtId = src[i];
            pBuf->dataIn++;
            break;
        case 3:
            pBuf->prtId = (src[i]<< 16) | (pBuf->prtId & 0xffff);
            pBuf->dataIn++;
            break;
        case 4:
            pBuf->pulseCount = src[i];
            pBuf->dataIn++;
            break;
        case 5:
            pBuf->pulseCount = (src[i]<< 16) | (pBuf->pulseCount & 0xffff);
            pBuf->dataIn++;
            break;
        default:
            pBuf->_iq[pBuf->nextData] = src[i];
            pBuf->nextData++;
            pBuf->dataIn++;
            if (pBuf->dataIn == (2*_numIQ+6))
                pBuf->dataIn = 0;
            if (pBuf->nextData == pBuf->_iq.size()) {
                // current buffer has been filled
                // lock access to the queues
                pthread_mutex_lock(&_bufferMutex);
                // update the accounting
                addBytes(chan, pBuf->_iq.size()*sizeof(src[0]));
                if (_freeIQBuffers.size() > 0) {
                    // if we have more available free buffers, 
                    // put it in the full queue
                    _fullBuffers.push_back(pBuf);
                    // signal that new data is available
                    pthread_cond_broadcast(&_dataAvailCond);
                    // get a new buffer empty buffer from the free list
                    pBuf = _freeIQBuffers[0];
                    pBuf->nextData = 0;
                    pBuf->dataIn = 0;
                    pBuf->dmaChan = chan;
                    // save it as the current buffer being filled
                    _currentIQBuffer[chan] = pBuf;
                    // remove from the free list
                    _freeIQBuffers.pop_front();
                } else {
                    // no empty buffers available, so just reuse this 
                    // one. For right now, print out an error complaint.
                    // Later on we may need to add some accounting and 
                    // notification scheme.
                    pBuf->nextData = 0;
                    pBuf->dataIn = 0;
                    // std::cout << "iq buffer unavailable " <<__FILE__ << ":" << __LINE__ << "\n";
                }
                // unlock queue acess
                pthread_mutex_unlock(&_bufferMutex);
            }
            break;
        }
    }
}
/////////////////////////////////////////////////////////////////////////
void RR314::newABPData(int* src,
                       int chan,
                       int n) {

    // verify that we got an abp channel
    assert((chan & 1) && (chan < 8));

    //std::cout << "newABPData for board:" << boardNumber() << "  chan:" << chan << " src[5]:" << src[5] << "\n";
    
    RRABPBuffer* pBuf = _currentABPBuffer[chan];
    // loop through all src samples
    for (int i = 0; i < n; i++) {

        // fill the current buffer from the source
        switch (pBuf->dataIn) {
        case 0:
            pBuf->dmaChan = chan;
            pBuf->chanId = src[i];
            /// @todo A hack for the moment; for some reason the channel ID from
            /// the card is all fouled up. Map the DMA channel (1, 3, 5, 7) to (1,2,3,4)
            pBuf->chanId = chan/2 + 1;
            pBuf->dataIn++;
            break;
        case 1:
            pBuf->prtId = src[i];
            pBuf->dataIn++;
            break;
        case 2:
            pBuf->pulseCount = src[i];
            pBuf->dataIn++;
            break;
        default:
            // A and B scaled by full scale
            if (((pBuf->nextData) % 3) < 2) {
                pBuf->_abp[pBuf->nextData] = src[i]*ABPSCALE;
            } else {
                // P scaled by 2 times full scale
                pBuf->_abp[pBuf->nextData] = src[i]*ABPSCALE/2.0;
            }
            pBuf->nextData++;
            pBuf->dataIn++;
            if (pBuf->nextData == pBuf->_abp.size()) {
                // current buffer has been filled
                // lock access to the queues
                pthread_mutex_lock(&_bufferMutex);
                // update the accounting
                addBytes(chan, pBuf->_abp.size()*sizeof(src[0]));
                if (_freeABPBuffers.size() > 0) {
                    // put it in the full queue
                    _fullBuffers.push_back(pBuf);
                    // signal that new data is available
                    pthread_cond_broadcast(&_dataAvailCond);
                    pBuf = _freeABPBuffers[0];
                    pBuf->nextData = 0;
                    pBuf->dataIn = 0;
                    // save it as the current buffer being filled
                    _currentABPBuffer[chan] = pBuf;
                    // remove from the free list
                    _freeABPBuffers.pop_front();
                } else {
                    // no empty buffers available, so just reuse this 
                    // one. For right now, print out an error complaint.
                    // Later on we may need to add some accounting and 
                    // notification scheme.
                    pBuf->nextData = 0;
                    pBuf->dataIn = 0;
                    // std::cout << "abp buffer unavailable " <<__FILE__ << ":" << __LINE__ << "\n";
                }
                // unlock queue acess
                pthread_mutex_unlock(&_bufferMutex);
            }
            break;
        }
    }
}

/////////////////////////////////////////////////////////////////////////

RRBuffer* RR314::nextBuffer() {
    RRBuffer* pBuf = 0;

    pthread_mutex_lock(&_bufferMutex);

    while (_fullBuffers.size() == 0) {
        pthread_cond_wait(&_dataAvailCond, &_bufferMutex);
    }

    pBuf = _fullBuffers[0];

    _fullBuffers.pop_front();

    pthread_mutex_unlock(&_bufferMutex);

    return pBuf;
}

////////////////////////////////////////////////////////////////////////

void RR314::returnBuffer(RRBuffer* buf) {
    pthread_mutex_lock(&_bufferMutex);

    if (buf->type == RRBuffer::ABPtype) {
        RRABPBuffer * pBuf = dynamic_cast<RRABPBuffer*>(buf);
        assert(pBuf != 0);
        _freeABPBuffers.push_back(pBuf);
    } else {
        RRIQBuffer * pBuf = dynamic_cast<RRIQBuffer*>(buf);
        assert(pBuf != 0);
        _freeIQBuffers.push_back(pBuf);
    }

    pthread_mutex_unlock(&_bufferMutex);
}
////////////////////////////////////////////////////////////////////////

int RR314::numFreeABPBuffers() {
    pthread_mutex_lock(&_bufferMutex);

    int result = _freeABPBuffers.size();

    pthread_mutex_unlock(&_bufferMutex);

    return result;
}

////////////////////////////////////////////////////////////////////////

int RR314::numFreeIQBuffers() {
    pthread_mutex_lock(&_bufferMutex);

    int result = _freeIQBuffers.size();

    pthread_mutex_unlock(&_bufferMutex);

    return result;
}

/////////////////////////////////////////////////////////////////////////

void RR314::fifoFullInts(int n) {
    _fifoFullInts = n;
}

/////////////////////////////////////////////////////////////////////////

int RR314::fifoFullInts() {
    return _fifoFullInts;
}

/////////////////////////////////////////////////////////////////////////

void RR314::boardInfo() {

    if (_simulate)
        return;

    unsigned int result;

    // get some of the rev numbers from the card 
    Adapter_Read32(&_chanAdapter, V4, SVN_REV_ADR, &result);
    printf("SVN Firmware Rev = %i\n", result);
    Adapter_Read32(&_chanAdapter, BRIDGE, BRG_REV_ADR, &result);
    printf("PCI Bridge Rev = %x\n", result);
    Adapter_Read32(&_chanAdapter, V4, DMA_REV_ADR, &result);
    printf("V4 DMA Rev (Offset 0x0) =  %x, ", result);
    Adapter_Read32(&_chanAdapter, V4, V4_REV_ADR, &result);
    printf("User Logic Rev (Offset 0x800) =  %x\n", result);
    printf("Current temp is %.2f C\n", temperature());

}

/////////////////////////////////////////////////////////////////////////

bool RR314::pulsepairInit() {
    //
    //    This section initializes the pulse pair processors.

    // Decimation Setup
    int decimationFactor;
    switch (_pulsewidth) {
    case 250:
        decimationFactor = _0_25us;
        break;
    case 500:
        decimationFactor = _0_50us;
        break;
    case 750:
        decimationFactor = _0_75us;
        break;
    case 1000:
        decimationFactor = _1_00us;
        break;
    case 1250:
        decimationFactor = _1_25us;
        break;
    case 1500:
        decimationFactor = _1_50us;
        break;
    case 1750:
        decimationFactor = _1_75us;
        break;
    case 2000:
        decimationFactor = _2_00us;
        break;
    default:
        std::cout
                << "pulse width must be one of: 250, 500, 750, 1000, 1250, 1500, 1750, 2000\n";
        return false;
    }

    unsigned int Dec_Factor = decimationFactor*2 - 1;
    Adapter_Write32(&_chanAdapter, V4, DEC_REG, decimationFactor);// Decimation Register

    //Pulse Pair Setup
    Adapter_Write32(&_chanAdapter, V4, M_REG, _gates); // # of Gates
    Adapter_Write32(&_chanAdapter, V4, N_REG, _samples); // # of Samples
    Adapter_Write32(&_chanAdapter, V4, DPRT_REG, _dualPrt); // Dual Prt(Off)
    Adapter_Write32(&_chanAdapter, V4, IQ_START_IDX, _startGateIQ); // index of start of IQ capture
    Adapter_Write32(&_chanAdapter, V4, IQ_GATE_LEN, _numIQ); // # of Gate of IQ capture

    Adapter_Read32(&_chanAdapter, V4, M_REG, &_gates);
    Adapter_Read32(&_chanAdapter, V4, N_REG, &_samples);
    Adapter_Read32(&_chanAdapter, V4, DPRT_REG, &_dualPrt);
    Adapter_Read32(&_chanAdapter, V4, IQ_START_IDX, &_startGateIQ);
    Adapter_Read32(&_chanAdapter, V4, IQ_GATE_LEN, &_numIQ);
   
    return true;

}

/////////////////////////////////////////////////////////////////////////

bool RR314::timerInit() {
    //
    //    This section initializes the timers.

    // Decimation Setup
    int decimationFactor;
    switch (_pulsewidth) {
    case 250:
        decimationFactor = _0_25us;
        break;
    case 500:
        decimationFactor = _0_50us;
        break;
    case 750:
        decimationFactor = _0_75us;
        break;
    case 1000:
        decimationFactor = _1_00us;
        break;
    case 1250:
        decimationFactor = _1_25us;
        break;
    case 1500:
        decimationFactor = _1_50us;
        break;
    case 1750:
        decimationFactor = _1_75us;
        break;
    case 2000:
        decimationFactor = _2_00us;
        break;
    default:
        std::cout
                << "pulse width must be one of: 250, 500, 750, 1000, 1250, 1500, 1750, 2000\n";
        return false;
    }

    printf("Internal Timing Variables\n");
    printf("# of Gates          = %d\n", _gates);
    printf("# of Samples        = %d\n", _samples);
    printf("Dual PRT (1=on)     = %d\n", _dualPrt);
    printf("IQ Start Gate       = %d\n", _startGateIQ);
    printf("# of IQ Gates       = %d\n", _numIQ);
    printf("TX Pulse Width (us) = %f\n", float(decimationFactor/8.0));
    printf("RX Pulse Width (us) = %f\n", float(decimationFactor/8.0*_pulsewidth));
    printf("PRT Period     (us) = %f\n", float(1.0/_prf*1.0e6));
    printf("Data Rate (MHz)     = %f\n", float(8.0/decimationFactor));

    // Gating Timer Setup
    unsigned int Timers= TIMER0|TIMER1|TIMER2|TIMER3;
    //printf("CONTROL REG: %x\n", CONTROL_REG|Timers|TIMER_EN);
    Adapter_Write32(&_chanAdapter, V4, MT_ADDR, CONTROL_REG|Timers); // Control Register
    Adapter_Write32(&_chanAdapter, V4, MT_DATA, TIMER_ON); // Enable Timer
    Adapter_Write32(&_chanAdapter, V4, MT_WR, WRITE_ON); // Turn on Write Strobes

    // Delay Register
    Adapter_Write32(&_chanAdapter, V4, MT_ADDR, DELAY_REG|Timers); // Address Timer 0
    Adapter_Write32(&_chanAdapter, V4, MT_DATA, 0); // Value Timer 0

    // Pulse Width Register
    Adapter_Write32(&_chanAdapter, V4, MT_ADDR, WIDTH_REG|Timers); // Address Timer 0
    Adapter_Write32(&_chanAdapter, V4, MT_DATA, _gates*60*decimationFactor/8); // Value Timer 0 (Testing Purposes)

    // Period Register
    double prtClock = (60e6);
    int periodCount = (int) (prtClock/_prf);
    //std::cout << "Period register value:" << periodCount << "\n";
    Adapter_Write32(&_chanAdapter, V4, MT_ADDR, PERIOD_REG|Timers); // Address Timer 0
    if (_dualPrt == 0) {
        Adapter_Write32(&_chanAdapter, V4, MT_DATA, periodCount);
    } else {
        Adapter_Write32(&_chanAdapter, V4, MT_DATA, periodCount); // Mult PRT 5/4 @ 1kHz and 800Hz PRFs
    }

    //Multiple PRT Register
    Adapter_Write32(&_chanAdapter, V4, MT_ADDR, PRT_REG|Timers); // Mult PRT Register Timer 0
    if (_dualPrt) {
        Adapter_Write32(&_chanAdapter, V4, MT_DATA, 0x0054); // Mult PRT Value Timer 0
    } else {
        Adapter_Write32(&_chanAdapter, V4, MT_DATA, 0x0000); // Mult PRT Value Timer 0
    }

    // Enable and Trigger All Timers 
    Adapter_Write32(&_chanAdapter, V4, 
    MT_ADDR, 
    PRT_REG|Timers|TIMER_EN|ADDR_TRIG); // Set Global Enable
    Adapter_Write32(&_chanAdapter, V4, MT_WR, WRITE_OFF); // Turn off Write Strobes

    return true;

}

////////////////////////////////////////////////////////////////////////

void RR314::start() {

    _running = true;
    if (_simulate) {
        _simulator->start();
        return;
    }

    // Enable the DMAs and turn on the ADCs
    Adapter_Write32(&_chanAdapter, V4, V4_CTL_ADR, DMA_EN | ADCA_CAP);

    std::cout << "RR314 device " << _devNum << " started\n";

}

////////////////////////////////////////////////////////////////////////

int RR314::filterSetup() {

    // if we don't have two filter coefficient files specified, then
    // don't try to load the filters.
    if (_gaussianFile.size() == 0 || _kaiserFile.size() == 0)
        return 0;

    FilterSpec gaussian(_gaussianFile);
    if (!gaussian.ok()) {
        std::cerr << "Incorrect or unaccesible filter definition: "
                << _gaussianFile << std::endl;
        return -1;
    }

    FilterSpec kaiser(_kaiserFile);
    if (!kaiser.ok()) {
        std::cerr << "Incorrect or unaccesible filter definition: "
                << _kaiserFile << std::endl;
        return -1;
    }

    if (!loadFilters(gaussian, kaiser)) {
        std::cerr << "Unable to load filters\n";
        return -1;
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////
void RR314::addBytes(int chan,
                     int bytes) {
    _bytes[chan] += bytes;
}

//////////////////////////////////////////////////////////////////////
std::vector<unsigned long> RR314::bytes() {
    std::vector<unsigned long> result = _bytes;
    for (int i = 0; i < 8; i++)
        _bytes[i] = 0;

    return result;
}

//////////////////////////////////////////////////////////////////////

void RR314::lastGroup(int chan,
                      int group) {
    _lastGroup[chan] = group;
}

//////////////////////////////////////////////////////////////////////
int RR314::lastGroup(int chan) {
    return _lastGroup[chan];
}

//////////////////////////////////////////////////////////////////////
int RR314::boardNumber() {
    return _devNum;
}

//////////////////////////////////////////////////////////////////////
double RR314::temperature() {
    if (_simulate)
        return 20.0 + 2.0*(0.5 - (1.0*rand())/RAND_MAX);

    return ca_GetTemp(&_chanAdapter);
}

//////////////////////////////////////////////////////////////////////
void shutdownSignalHandler(int signo,
                           void* userData) {

    // stop all RR314 cards amd free their DMA allocations
    // iterate through all instances of RR314

    std::map<s_ChannelAdapter*, RR314*>::iterator p;
    for (p = RR314::rr314Instances.begin(); p != RR314::rr314Instances.end(); p++) {

        s_ChannelAdapter* pCA = p->first;

        std::cout << "Signal " << signo << " caught, stopping RR314 device "
                << pCA->DevNum << std::endl;
        p->second->RR314shutdown();

    }
}
