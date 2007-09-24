#include <signal.h>
#include "RRSnarfer.h"
#include <iostream>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <deque>

// statics that must be accessed both by the RRSnarfer 
// objects and the C ISR routine.

/// A mutex used to protect access to the data queues.
pthread_mutex_t bufferMutex = PTHREAD_MUTEX_INITIALIZER;

/// A queue of available empty buffers
std::deque<short*> freeBuffers;

/// A queue of buffers with data to be processed.
std::deque<short*> fullBuffers;

/// The ISR that is called by windrvr6
void Adapter_ISR(s_ChannelAdapter *pCA);

/// A handler that is called for varoiuos shutdown signals.
/// It's job is to force windrvr6 to return memory.
/// It appears that this doesn't really work. After
/// successively running RRSnarfer, Linux runs out of memory.
void shutdownSignalHandler(int signo);

/// The pointer to the channel adapter. It is needed in
/// RRSnarfer to configure the channel adapter, and
/// then in the ISR to access the adapter for fetching
/// data
s_ChannelAdapter* pCA0;

/// The last group fetched for each DMA channel.
int lastGroup[4] = {-1,-1,-1,-1};

/// Running total of FIFO full interrupts
int fifoFullInts = 0;

/// A running total of the data samples that have
/// been transfered via DMA. Used to calculate
/// the observed throughput.
int sampleCount = 0;

//////////////////////////////////////////////////////////////////////

RRSnarfer::RRSnarfer(int blockSize, 
		     double adcSampleRate, 
		     int decimationFactor, 
		     std::string gaussianFile,
		     std::string kaiserFile,
		     std::string xsvfFileName):
  _blockSize(blockSize),
  _adcSampleRate(adcSampleRate),
  _decimationFactor(decimationFactor),
  _gaussianFile(gaussianFile),
  _kaiserFile(kaiserFile),
  _xsvfFileName(xsvfFileName),
  _bufferNext(0),
  _IQnext(0),
  _dmaAllocated(false)
{

  std::cout << "*** This version of RRSnarfer works with CHA_DDC.xsvf\n";

  _deviceName = "/dev/windrvr6";

  _deviceFd = open(_deviceName.c_str(), O_RDONLY | O_NONBLOCK);

  if (_deviceFd < 0) {
    std::cerr << "cannot access " << _deviceName << "\n";
    perror("");
    exit(1);
  }

  // initialize the block size
  blockSizeSlot(_blockSize);

  // configure the Red River card

  if ( configureRedRiver()) {
    std::cerr << "Unable to configure the Red Rapids card" << std::endl;
    shutdown();
    exit (1);
  }     

  // this timer is called to plot the data
  QObject::connect(&_plotTimer, 
		   SIGNAL(timeout()), 
		   this,
		   SLOT(plotTimeout()));
  _plotTimer.start(50);

  // this timer is used to calculate the raw data rate
  QObject::connect(&_rateTimer, SIGNAL(timeout()), this, SLOT(rateTimeout()));
  _rateTimer.start(10000);
  _rateTime.restart();

}
////////////////////////////////////////////////////////////////////////

void
RRSnarfer::plotTimeout() 
{
  
  // lock access to the buffer pools
  pthread_mutex_lock(&bufferMutex);

  // do we have anything remaining?
  if (fullBuffers.size() == 0) {
    pthread_mutex_unlock(&bufferMutex);
    return;
  }
        
  // fill I and Q buffers
  short int* pBuf = fullBuffers[0];
  //for (int i = 0; i < _blockSize; i = i + 2) {
  while (_IQnext < _blockSize) {
    // if we have reached the end of this buffer, try 
    // to use a new one
    if (_bufferNext > DMABLOCKSIZE*DMABLOCKSPERGROUP*4) {
      _bufferNext = 0;
      freeBuffers.push_back(fullBuffers[0]);
      fullBuffers.pop_front();
      if (fullBuffers.size() == 0) {
	pthread_mutex_unlock(&bufferMutex);
	return;
      }
      pBuf = fullBuffers[0];
    }
    // fill the I/Q arrays
    
    // Added by Tom 10/24/06
    if (pBuf[_bufferNext+3] == 0xFFFFAAAA) {
      _bufferNext += 4;
      //       printf("New Data Stream\n");
    }

    //printf("I1 = %x        Q1 = %x\n", pBuf[_bufferNext+3], pBuf[_bufferNext+2]);
    //printf("I0 = %x        Q0 = %x\n", pBuf[_bufferNext+1], pBuf[_bufferNext]);

    //printf("L %x\n%x\n", pBuf[_bufferNext+1], pBuf[_bufferNext]);
    //printf("Q0 %x       I0 %x\n", pBuf[_bufferNext], pBuf[_bufferNext+1]);
    _Q[_IQnext+1] = pBuf[_bufferNext++];      

    _I[_IQnext+1] = pBuf[_bufferNext++];

    //printf("H %x\n%x\n", pBuf[_bufferNext+1], pBuf[_bufferNext]);
    //printf("Q1 %x      I1 %x\n", pBuf[_bufferNext], pBuf[_bufferNext+1]);
    _Q[_IQnext  ] = pBuf[_bufferNext++];      

    _I[_IQnext  ] = pBuf[_bufferNext++];
    _IQnext += 2;
  }

  // got here, so _I and _Q are full

  // return  extra buffers
  while (fullBuffers.size() > BUFFERPOOLSIZE/2) {
    freeBuffers.push_back(fullBuffers[0]);
    fullBuffers.pop_front();
    // since we are throwing away data, we will have
    // a gap in the time series. Thus, we will
    // discard what we have collected in _I and _Q
    _bufferNext = 0;
  }

  // unlock access to the buffer pools
  pthread_mutex_unlock(&bufferMutex);

  // send the data to the plot
  emit addDataSignal(_I, _Q, 
		     _adcSampleRate/(4*_decimationFactor),
		     _adcSampleRate);

  // reset the IQ pointer
  _IQnext = 0;

  return;

}

////////////////////////////////////////////////////////////////////////

void
RRSnarfer::rateTimeout()
{

  double rate = 1000.0*sampleCount / _rateTime.elapsed(); 
  _rateTime.restart();
  sampleCount = 0;
  
  emit dataRateSignal(rate);
}

////////////////////////////////////////////////////////////////////////

void
RRSnarfer::blockSizeSlot(int size) {

  _blockSize = size;

  _I.resize(_blockSize);
  _Q.resize(_blockSize);
}

//////////////////////////////////////////////////////////////////////

RRSnarfer::~RRSnarfer() 
{
  shutdown();
  std::cout << "RRSnarfer deleted\n";
}

//////////////////////////////////////////////////////////////////////

void
RRSnarfer::shutdown() {

  Adapter_Write32(&_CA0, BRIDGE, BRG_INTRMASK_ADR, BRG_INTR_DIS);
  Adapter_Write32(&_CA0, V4, V4_CTL_ADR, 0x0);

  if (_dmaAllocated) {
    Adapter_DMABufFree(&_CA0);
    std::cout << "DMA buffers returned\n";
  }

  Adapter_Close(&_CA0); 
  
  for (unsigned int i = 0; i < freeBuffers.size(); i++ )
    delete [] freeBuffers[i];

  for (unsigned int i = 0; i < fullBuffers.size(); i++ )
    delete [] fullBuffers[i];

}

//////////////////////////////////////////////////////////////////////

bool
RRSnarfer::loadFilters(FilterSpec& gaussian, FilterSpec& kaiser) {

  bool kaiserLoaded;
  bool gaussianLoaded;

  int attempt;
  
  // program kaiser coefficients

  attempt = 0;
  do {
    kaiserLoaded = true;
    for (unsigned int i = 0; i < kaiser.size(); i++) {

      DWORD readBack;
      DWORD ramAddr   = i/4;
      DWORD ramSelect = i%4 << 4;
      DWORD addr      = 0x1000 | ramSelect | ramAddr;

      // set the address
      Adapter_Write32(&_CA0, V4, 0x934, addr);

      // write the value
      Adapter_Write32(&_CA0, V4, 0x938, kaiser[i]);

      // enable writing
      Adapter_Write32(&_CA0, V4, 0x94C, 1);

      // disable writing (kaiser readback only succeeds if we do this)
      Adapter_Write32(&_CA0, V4, 0x94C, 0);

      // read back the programmed value
      Adapter_Read32(&_CA0, V4, 0x93c, (DWORD*)&readBack);

      if (readBack != kaiser[i]) {
	std::cout << "kaiser readback failed for coefficient " << std::dec << i 
		  << std::hex << ", wrote " << kaiser[i] << ", read "
		  << readBack << std::endl;
  
	kaiserLoaded = false;
      }
    }
    attempt++;
  } while (!kaiserLoaded && attempt < 50);

  if (kaiserLoaded) {
    std::cout << kaiser.size() << " Kaiser filter coefficients succesfully loaded\n";
  } else {
    std::cout << "Unable to load the Kaiser filter coefficients\n";
  }

  // program gaussian coefficients
  attempt = 0;
  
  do {
    gaussianLoaded = true;
    for (unsigned int i = 0; i< gaussian.size(); i++) {

      DWORD readBack;
      DWORD ramAddr   = i%12;
      DWORD ramSelect = i/12 << 4;
      DWORD addr      = ramSelect | ramAddr;

      // set the address
      Adapter_Write32(&_CA0, V4, 0x940, addr);

      // write the value
      Adapter_Write32(&_CA0, V4, 0x944, gaussian[i]);

      // enable writing
      Adapter_Write32(&_CA0, V4, 0x950, 1);     

      //disable writing
      Adapter_Write32(&_CA0, V4, 0x950, 0);

      // read back the programmed value
      Adapter_Read32(&_CA0, V4, 0x948, (DWORD*)&readBack);

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
    std::cout << gaussian.size() << " Gaussian filter coefficients succesfully loaded\n";
  } else {
    std::cout << "Unable to load the Gaussian filter coefficients\n";
  }

  return kaiserLoaded && gaussianLoaded;

}

//////////////////////////////////////////////////////////////////////

int
RRSnarfer::configureRedRiver()
{

  ///////////////////////////////////////////////////////////////////////
  //
  // Signal handling
  //
  // install signal handler for abort signals
  struct sigaction new_action, old_action;
     
  /* Set up the structure to specify the new action. */
  new_action.sa_handler = shutdownSignalHandler;
  sigemptyset (&new_action.sa_mask);
  new_action.sa_flags = 0;
     
  sigaction (SIGINT, NULL, &old_action);
  if (old_action.sa_handler != SIG_IGN)
    sigaction (SIGINT, &new_action, NULL);
  sigaction (SIGHUP, NULL, &old_action);
  if (old_action.sa_handler != SIG_IGN)
    sigaction (SIGHUP, &new_action, NULL);
  sigaction (SIGTERM, NULL, &old_action);
  if (old_action.sa_handler != SIG_IGN)
    sigaction (SIGTERM, &new_action, NULL);

  DWORD Dummy;

  //Flags
  int ProgramFPGA;                  // program fpga
  ProgramFPGA = 0;

  int V4LoadCheck;                  // if set, verify that V4 has been loaded
  V4LoadCheck         = _xsvfFileName.size()>0; 
  V4LoadCheck         = 0;

  pCA0 = &_CA0;

  _ClkSettings.ClkSrc = SYNTH;    // can be either SYNTH or EXT

  pthread_mutex_lock(&bufferMutex);

  for (int i = 0; i < BUFFERPOOLSIZE; i++) {
    short* buf = new short[DMABLOCKSIZE*DMABLOCKSPERGROUP*4];
    freeBuffers.push_back(buf);
  }

  pthread_mutex_unlock(&bufferMutex);

  //Init all flags to default values
  Adapter_Zero(&_CA0);
  _CA0.DevNum = 0;
  strcpy(_CA0.Asy, "M314"); 

  // Open channel adapter
  if(Adapter_Open(&_CA0)) {
    fprintf(stderr, "Failed to find and/or open card. (%x)\n", _CA0.ReturnStatus);
    return -1;
  } else {
    printf("Opened ChannelAdapter device %d\n", _CA0.DevNum);
  }

  // Disable all interupts
  Adapter_Write32(&_CA0, 
		  V4, 
		  V4_MASK_ADR, 
		  0);

  //Read from the QL to test out connections
  Adapter_Read32(&_CA0, 
		 BRIDGE, 
		 BRG_REV_ADR, 
		 &Dummy);
  printf("PCI Bridge rev is %x\n", Dummy);

  //Load the v4 if needed
  if(_xsvfFileName.size() > 0) {
    std::cout << "Loading bitstream " << _xsvfFileName << std::endl;
    char name[_xsvfFileName.size()+1];
    strcpy(name, _xsvfFileName.c_str());
    if(Adapter_LoadXSVF(&_CA0, name, 1)) {
      printf("V4 and / or PROM failed to load, will now exit...\n");
      Adapter_Close(&_CA0);
      return -1; 
    }
  }

  //Check the V4 has a valid load.  Some early versions of hw don't support this
  //  if(V4LoadCheck) {
  //Adapter_Read32(&_CA0, 
  // BRIDGE, 
  // BRG_V4_STAT, 
  // &Dummy);
  //if(!(Dummy & V4_PROG_DONE)) {
  //  printf("V4 is not loaded, please check PROM contents or load an XSVF file to the V4.\n");
  //  Adapter_Close(&_CA0);
  //  return -1; 
  //}
  //}
 
  //Read back V4 DMA core rev information 
  Adapter_Read32(&_CA0, 
		 V4, 
		 DMA_REV_ADR, 
		 &Dummy);  
  printf("V4 DMA Rev Code = %x, ", Dummy);

  //Read back V4 user rev information 
  Adapter_Read32(&_CA0, 
		 V4, 
		 V4_REV_ADR, 
		 &Dummy);  
  printf("User Logic Rev Code =  %x\n", Dummy);

  Adapter_SampleClkSelect(&_CA0, &_ClkSettings);

  Adapter_uSleep(1000000);

  //Soft Reset, self clearing
  Adapter_Write32(&_CA0, 
		  V4, 
		  V4_CTL_ADR, 
		  SOFT_RST); 

  //Wait for DCM to Relock
  Adapter_uSleep(1000000);     

  //Clear old status reg
  Adapter_Read32(&_CA0, V4, V4_STAT_ADR, &Dummy);

  // Flush all ADC FIFOs
  Adapter_Write32(&_CA0, 
		  V4, 
		  V4_CTL_ADR, 
		  TELADCFF_FLUSH|ADCAFF_FLUSH|ADCBFF_FLUSH|
		  ADCCFF_FLUSH|ADCDFF_FLUSH);

  // Enable GPIO Pins
  //  Adapter_Write32(&_CA0, BRIDGE, BRG_GPIO_ADR, BRG_GPIO_EN);

  ///////////////////////////////////////////////////////////////////////////////
  //
  //                DMA memory configuration
  //
  // Allocate DMA Memory
  // Firmware supports 4 DMA channels, indexed to 0

  _CA0.DMA.DMAChannels = 0;

  for (unsigned int i = 0; i <_CA0.DMA.DMAChannels+1; i++) {

    int chan = i;

    std::cout << "Allocating DMA space for channel " << chan << std::endl;

    // This pathetic API has you write 1 less than the number for
    // a couple of parameters.
    _CA0.DMA.GrpCnt[chan]      = DMANUMGROUPS-1;  
    _CA0.DMA.BlockSizeB[chan]  = DMABLOCKSIZE*8; 
    _CA0.DMA.BlockCount[chan]  = DMABLOCKSPERGROUP-1;   

  }

  if(Adapter_DMABufAllocate(&_CA0)){
    fprintf(stderr, "Adapter_DMABufAllocate, failure allocating DMA memory\n");
    Adapter_Close(&_CA0);
    return -1;
  }  else {
    LoadCADMASettings(&_CA0);
    _dmaAllocated = true;
  }

  std::cout << "Each group interrupt corresponds to " 
	    << DMABLOCKSIZE*8*DMABLOCKSPERGROUP << " bytes\n";

  std::cout << "Total space allocated for DMA buffering is "
	    << DMABLOCKSIZE*8*DMABLOCKSPERGROUP*DMANUMGROUPS*4
	    << " bytes\n";
 
  //////////////////////////////////////////////////////////////////////
  //
  // Enable DMA and then channel
  //
  //

  for (unsigned int i = 0; i <_CA0.DMA.DMAChannels+1; i++) {
    Adapter_Write32(&_CA0, 
		    V4, 
		    DMA_CH0_GRPSPERINT_ADR+4*i, 
		    DMA_GROUPS_PER_INT);
  }

  Dummy = 0;
  for (unsigned int i = 0; i <_CA0.DMA.DMAChannels+1; i++) {
    Dummy |= (1 << i);
  }
  Adapter_Write32(&_CA0, 
		  V4, 
		  DMA_INT_MASK_ADR, 
		  Dummy);
  printf("DMA_INT_MASK set to %x\n", Dummy);

  Dummy = 0;
  Dummy |= DMA_EN | ADCA_CAP; 
  Adapter_Write32(&_CA0, 
		  V4, 
		  V4_CTL_ADR,
		  Dummy);

  // Select interrupts
  Adapter_Write32(&_CA0, 
		  V4, 
		  V4_MASK_ADR, 
		  //ADC_DCM_UNLOCKED |
		  //ADCA_FF_FULL |
		  //ADCB_FF_FULL |
		  //ADCC_FF_FULL |
		  //ADCD_FF_FULL |
		  DMA_GRP_DONE
		  | INTR_EN);

  //Allow bridge to create PCI intr
  Adapter_Write32(&_CA0, 
		  BRIDGE, 
		  BRG_INTRMASK_ADR, 
		  BRG_INTR_EN); 


  ///////////////////////////////////////////////////////////////////////
  //
  // Timer
  //
  //

  //Pulse Pair Setup
  Adapter_Write32(&_CA0, V4, 0x95C, 100);     // # of Gates
  Adapter_Write32(&_CA0, V4, 0x960, 10);     // # of Dwells
  Adapter_Write32(&_CA0, V4, 0x964, 0);      // Dual Prt(Off)

  DWORD gates, dwells, dprt;

  Adapter_Read32(&_CA0, V4, 0x95C, &gates);
  Adapter_Read32(&_CA0, V4, 0x960, &dwells);
  Adapter_Read32(&_CA0, V4, 0x964, &dprt);
  printf("Gates = %d, Dwells = %d, Dual Prt = %d\n", gates, dwells, dprt);
  printf("Pulse Width = %d, Decimation Factor = %d\n", gates*_decimationFactor, _decimationFactor);

  //Gating Timer Setup
  Adapter_Write32(&_CA0, V4, 0x928, 0x1038); // Control Register
  Adapter_Write32(&_CA0, V4, 0x92C, 0x1);    // Enable Timer
  Adapter_Write32(&_CA0, V4, 0x930, 0x3);    // Turn on Write Strobes
  Adapter_Write32(&_CA0, V4, 0x928, 0x1039); // Delay Register Timer 0
  Adapter_Write32(&_CA0, V4, 0x92C, 0);      // Delay Value Timer 0
  Adapter_Write32(&_CA0, V4, 0x928, 0x103A); // Pulse Width Register Timer 0
  //Adapter_Write32(&_CA0, V4, 0x92C, 120/_decimationFactor);     // Pulse Width Value Timer 0
  Adapter_Write32(&_CA0, V4, 0x92C, 128/_decimationFactor);     // Pulse Width Value Timer 0
  Adapter_Write32(&_CA0, V4, 0x928, 0x103B); // Period Register Timer 0
  //Adapter_Write32(&_CA0, V4, 0x92C, 12000/_decimationFactor-1);   // Period Value Timer 0
  Adapter_Write32(&_CA0, V4, 0x92C, 1024/_decimationFactor-1);   // Period Value Timer 0
  Adapter_Write32(&_CA0, V4, 0x928, 0x903B); // Set Global Enable
  Adapter_Write32(&_CA0, V4, 0x930, 0x0);    // Turn off Write Strobes
  
  if (filterSetup()) {
    return -1;
  }

  Adapter_Write32(&_CA0, V4, 0x934, 0); // Start the filter
 
  return 0;
 
}

////////////////////////////////////////////////////////////////////////

int
RRSnarfer::filterSetup()
{
  ////////////////////////////////////////////////////////////////////////
  //
  // Filter setup
  //
  //


  FilterSpec gaussian(_gaussianFile);
  if (!gaussian.ok()){
    std::cerr << "Incorrect or unaccesible filter definition: " << _gaussianFile << std::endl;
    return -1;
  }

  FilterSpec kaiser(_kaiserFile);
  if (!kaiser.ok()){
    std::cerr << "Incorrect or unaccesible filter definition: " << _kaiserFile << std::endl;
    return -1;
  }

  if (!loadFilters(gaussian, kaiser)) {
    std::cerr << "Unable to load filters\n";
    exit(1);
  }

  // for Tom's bitstream:

  DWORD Decimation, Dec_Factor;

  Decimation = _decimationFactor;
  Dec_Factor = Decimation*2 - 1;

  //Decimation Setup
  Adapter_Write32(&_CA0, V4, 0x958, 0x1); // Turn on Write Strobe
  Adapter_Write32(&_CA0, V4, 0x954, Dec_Factor);// Decimation Register
  Adapter_Write32(&_CA0, V4, 0x958, 0x0);  // Turn off Write Strobe

  return 0;
}


//////////////////////////////////////////////////////////////////////
/// Copy one group to the destination
void
dmaCopy(UINT32* dest, s_ChannelAdapter* pCA, int chan, int group) {

  int MaxGrpsPerCh = 2048 / (pCA->DMA.DMAChannels + 1); 
  if(MaxGrpsPerCh > 1024) //1024 is max groups per channel
    MaxGrpsPerCh = 1024;

  UINT32* src = (UINT32*)pCA->DMA.dVirtDMAAdr[(chan*MaxGrpsPerCh)+group];
  memcpy(dest, src, DMABLOCKSPERGROUP * DMABLOCKSIZE * 8);

  return;
}

//////////////////////////////////////////////////////////////////////
void processDMAGroups(s_ChannelAdapter *pCA, int chan) {

  int groupsDropped = 0;

  // get the DMA current group value
  unsigned int cg;
  Adapter_Read32(pCA, V4, DMA_CH0_CURGRP_ADR+(0x4*chan), &cg);

  int currentGroup = cg;
  
  // loop through the groups. Recall that lastGroup is the 
  // index of the last group that we processed. currentGroup
  // is the index of the most recent group that is available to be processed.
  // Copy all groups inclusive between lastGroup+1 and currentGroup.
      
  if (currentGroup >= 0 && currentGroup < DMANUMGROUPS) {

    // lock access to the buffer pool
    pthread_mutex_lock(&bufferMutex);

    while (lastGroup[chan] != (int)currentGroup) {
      // every group that is available counts as data that 
      // has been transferred, even if we can't use it.
      sampleCount += DMABLOCKSPERGROUP*DMABLOCKSIZE*2;

      // check for group number wrap around
      lastGroup[chan]++;
      if (lastGroup[chan] == DMANUMGROUPS) 
	lastGroup[chan] = 0;

      // add the data from the group to the buffer pool, if possible
      // otherwise ignore that group
      if (freeBuffers.size() > 0) {
	// move a buffer from the free list to the filled list
	short int* pBuf = freeBuffers[0];
	fullBuffers.push_back(freeBuffers[0]);
	freeBuffers.pop_front();
	// copy dma region into that buffer
	dmaCopy((UINT32*)pBuf, pCA, chan, lastGroup[chan]);
      } else {
	groupsDropped++;
      }
    } 

    // release lock on the buffer pool
    pthread_mutex_unlock(&bufferMutex);

  } else {
    std::cout << "BOGUS GROUP COUNT: " << currentGroup << "\n";
  } 
}

//////////////////////////////////////////////////////////////////////
void Adapter_ISR(s_ChannelAdapter *pCA)
{
  DWORD Status;
  DWORD Mask;
  DWORD Control;
  DWORD DMAMask;
  DWORD DMAStatus;

  Adapter_Read32(pCA, V4, V4_STAT_ADR,       &Status);
  Adapter_Read32(pCA, V4, V4_MASK_ADR,       &Mask);
  Adapter_Read32(pCA, V4, V4_CTL_ADR,        &Control);
  Adapter_Read32(pCA, V4, DMA_INT_MASK_ADR,  &DMAMask);
  Adapter_Read32(pCA, V4, DMA_INT_STAT_ADR,  &DMAStatus);

  //  printf("Adapter_ISR: Device %d, Status %x, Mask %x, Control %x, DMA Mask %x, DMA Status %x FifoFullInts %d sampleCout %d\n", 
  // pCA->DevNum, Status, Mask, Control, DMAMask, DMAStatus, fifoFullInts, sampleCount);

  // see if we have a DMA interrupt
  if (Status & DMA_GRP_DONE) {
    // yes, process groups for each DMA channel
    for (int chan = 0; chan < 1; chan++) {
      if ((1 << chan) & DMAStatus) {
	processDMAGroups(pCA, chan);
      } 
    }  
  }   

  DWORD Dummy = 0;
  if((Mask & Status) & ADCA_FF_FULL)    {
    // Flush ADC A FIFO
    Dummy |= ADCAFF_FLUSH;
    fifoFullInts++;
  }
    
  if((Mask & Status) & ADCB_FF_FULL)   {
    Dummy |= ADCBFF_FLUSH;
    fifoFullInts++;
  }

  if((Mask & Status) & ADCC_FF_FULL)         {
    Dummy |= ADCDFF_FLUSH;
    fifoFullInts++;
  }
  
  if((Mask & Status) & ADCD_FF_FULL)    {
    Dummy |= ADCBFF_FLUSH;
    fifoFullInts++;
  }

  if (Dummy) {
    Dummy |= DMA_EN;
    Adapter_Write32(pCA, V4, V4_CTL_ADR, Dummy);

    //  Enable FIFOs to collect ADC data
    Dummy |= ADCA_CAP; 
    Adapter_Write32(pCA, V4, V4_CTL_ADR, Dummy);
  }

  
  if((Mask & Status) & TELAD_FF_FULL)    printf("ISR: Telem. FIFO full.\n");
  if((Mask & Status) & ADC_DCM_UNLOCKED) printf("ISR: ADC DCM out of lock.\n");

 
  //Re-enable intr
  Adapter_Write32(pCA, BRIDGE, BRG_INTRMASK_ADR, BRG_INTR_EN); 
 
  if (fifoFullInts > 200) {
    std::cout << "fifo overflows exceed 200\n";
    exit(1);
  }

  return;
}

//////////////////////////////////////////////////////////////////////
void
shutdownSignalHandler(int signo){

  for (unsigned int i = 0; i < freeBuffers.size(); i++ )
    delete [] freeBuffers[i];

  for (unsigned int i = 0; i < fullBuffers.size(); i++ )
    delete [] fullBuffers[i];

  Adapter_Write32(pCA0, BRIDGE, BRG_INTRMASK_ADR, BRG_INTR_DIS);
  Adapter_Write32(pCA0, V4, V4_CTL_ADR, 0x0);
  Adapter_DMABufFree(pCA0);
  Adapter_Close(pCA0); 
  std::cout << "caught signal; exiting...\n";

  exit(1);
}
