#include <signal.h>
#include "RR314.h"
#include <iostream>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <deque>

// statics that must be accessed both by the RR314
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
int lastGroup[16] = {
  -1,-1,-1,-1,
  -1,-1,-1,-1,
  -1,-1,-1,-1,
  -1,-1,-1,-1
};

/// The number of groups fetched for each DMA channel.
int totalGroups[16] = {
  0,0,0,0,
  0,0,0,0,
  0,0,0,0,
  0,0,0,0
};

/// Running total of FIFO full interrupts
int fifoFullInts = 0;

// bytes transferred per channel
int _bytes[16];

//////////////////////////////////////////////////////////////////////

RR314::RR314(unsigned int gates,
	     unsigned int samples,
	     unsigned int dualPrt,
	     unsigned int startGateIQ,
	     unsigned int nGatesIQ,
	     double adcSampleRate, 
	     unsigned int decimationFactor, 
	     std::string gaussianFile,
	     std::string kaiserFile,
	     std::string xsvfFileName):
  _gates(gates),
  _samples(samples),
  _dualPrt(dualPrt),
  _startGateIQ(startGateIQ),
  _nGatesIQ(nGatesIQ),
  _adcSampleRate(adcSampleRate),
  _decimationFactor(decimationFactor),
  _gaussianFile(gaussianFile),
  _kaiserFile(kaiserFile),
  _xsvfFileName(xsvfFileName),
  _bufferNext(0),
  _dmaAllocated(false)
{

  std::cout << "*** This version of RRSnarfer works with CA_DDC_4.xsvf\n";

  // initialize the byte counters
  for (int i = 0; i < 16; i++)
    _bytes[i] = 0;

  _deviceName = "/dev/windrvr6";

  _deviceFd = open(_deviceName.c_str(), O_RDONLY | O_NONBLOCK);

  if (_deviceFd < 0) {
    std::cerr << "cannot access " << _deviceName << "\n";
    perror("");
    exit(1);
  }

  // capture signals
  catchSignals();

  // configure the card
  if ( configure314()) {
    std::cerr << "Unable to configure the Red Rapids card" << std::endl;
    shutdown();
    exit (1);
  }     

  // display some board information
  boardInfo();

}

////////////////////////////////////////////////////////////////////////


RR314::~RR314() 
{
  shutdown();
  std::cout << "RR314 deleted\n";
}

//////////////////////////////////////////////////////////////////////

void
RR314::shutdown() {

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
RR314::loadFilters(FilterSpec& gaussian, FilterSpec& kaiser) {

  bool kaiserLoaded;
  bool gaussianLoaded;

  int attempt;
  
  // program kaiser coefficients

  attempt = 0;
  do {
    kaiserLoaded = true;
    for (unsigned int i = 0; i < kaiser.size(); i++) {

      unsigned int readBack;
      int ramAddr   = i/4;
      int ramSelect = i%4 << 4;
      int addr      = 0x1000 | ramSelect | ramAddr;

      // set the address
      Adapter_Write32(&_CA0, V4, 0x934, addr);

      // write the value
      Adapter_Write32(&_CA0, V4, 0x938, kaiser[i]);

      // enable writing
      Adapter_Write32(&_CA0, V4, 0x94C, 1);

      // disable writing (kaiser readback only succeeds if we do this)
      Adapter_Write32(&_CA0, V4, 0x94C, 0);

      // read back the programmed value
      Adapter_Read32(&_CA0, V4, 0x93c, &readBack);

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

      unsigned int readBack;
      int ramAddr   = i%12;
      int ramSelect = i/12 << 4;
      int addr      = ramSelect | ramAddr;

      // set the address
      Adapter_Write32(&_CA0, V4, 0x940, addr);

      // write the value
      Adapter_Write32(&_CA0, V4, 0x944, gaussian[i]);

      // enable writing
      Adapter_Write32(&_CA0, V4, 0x950, 1);     

      //disable writing
      Adapter_Write32(&_CA0, V4, 0x950, 0);

      // read back the programmed value
      Adapter_Read32(&_CA0, V4, 0x948, &readBack);

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
void
RR314::catchSignals() {

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

}

//////////////////////////////////////////////////////////////////////

int
RR314::configure314()
{

  unsigned int result;

  //Flags
  int ProgramFPGA;                  // program fpga
  ProgramFPGA = 0;

  int V4LoadCheck;                  // if set, verify that V4 has been loaded
  V4LoadCheck         = _xsvfFileName.size()>0; 
  V4LoadCheck         = 0;

  pCA0 = &_CA0;

  pthread_mutex_lock(&bufferMutex);

  for (int i = 0; i < BUFFERPOOLSIZE; i++) {
    short* buf = new short[DMABLOCKSIZEBYTES*DMABLOCKSPERGROUP];
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

  // set up the filters. Will do nothing if either of
  // the filter file paths is empty.
  if (filterSetup()) {
    // error initializing the filters
    return -1;
  }

  // Disable all interupts
  Adapter_Write32(&_CA0, V4, V4_MASK_ADR, 0x0);
	
  //Check the V4 has a valid load.  
  if(V4LoadCheck)
    {
      Adapter_Read32(&_CA0, BRIDGE, BRG_FPGA_STAT, &result);
      if(!(result & FPGA_PROG_DONE))
	{
	  printf("V4 is not loaded, please check PROM contents or load an XSVF file to the V4.\n");
	  Adapter_Close(&_CA0);
	  return -1;	
	}
    }
	
  // set the sample clock
  _ClkSettings.ClkSrc = SYNTH;    // can be either SYNTH or EXT
  Adapter_SampleClkSelect(&_CA0, &_ClkSettings);

  // Code here should reset your DCMs and allow time for them to lock,
  // then check their status
  //Adapter_uSleep(1000000);

  // Soft Reset, self clearing
  Adapter_Write32(&_CA0, V4, V4_CTL_ADR, SOFT_RST); 

  // Wait for DCM to Relock
  Adapter_uSleep(1000000);     
		
  // Check for DCM Lock
  Adapter_Read32(&_CA0, V4, V4_STAT_ADR, &result); //Clear old status reg
  Adapter_Read32(&_CA0, V4, V4_STAT_ADR, &result);
  if (result & ADC_DCM_UNLOCKED) {
    printf("DCM Failed to Lock. STATUS REG = %x\n Exiting Now...\n", result);
    Adapter_Close(&_CA0);
    return -1;
  }
				
  // Set M314 VRANGE mode.  This will touch the register that controls the sample
  // clk select, so it is done as RMW.
  // Set ADC range to 2Vpp 
  Adapter_Read32(&_CA0, BRIDGE, BRG_HWCONF_ADR, &result);
  result |= ADCA_VRNG1;
  Adapter_Write32(&_CA0, BRIDGE, BRG_HWCONF_ADR, result);
		
  // Flush FIFOs
  Adapter_Write32(&_CA0, V4, V4_CTL_ADR, ADCAFF_FLUSH);
			
  ///////////////////////////////////////////////////////////////////////////////
  //	
  // Allocate DMA space for each channel. The channel 
  // adapter library will create the scatter/gather 
  // memory references which are then downloaded
  // to the RR card for use by the DMA engine.
		
  printf("Allocating DMA space...");
  // Bitstream supports 16 DMA channels, indexed to 0
  _CA0.DMA.DMAChannels = 15;				
			
  unsigned int i;
  for (i = 0; i < _CA0.DMA.DMAChannels+1; i++) {
    int chan = i;
    _CA0.DMA.GrpCnt[chan]      = DMANUMGROUPS-1;  
    _CA0.DMA.BlockSizeB[chan]  = DMABLOCKSIZEBYTES; 
    _CA0.DMA.BlockCount[chan]  = DMABLOCKSPERGROUP-1;   
  }		
			
  if(Adapter_DMABufAllocate(&_CA0))
    {
      // unable to allocate DMA space
      fprintf(stderr, "Failure allocating DMA memory\n");
      Adapter_Close(&_CA0);
      return -1;
    }
  else	
    {
      // DMA allocated, load to card
      ca_LoadDMASettings(&_CA0);
    }
  printf("DMA memory allocation done.\n");
	
  // Enable DMA interrupt on every group for all 
  for(i = 0; i < _CA0.DMA.DMAChannels+1; i++)
    Adapter_Write32(&_CA0, V4, DMA_CH0_GRPSPERINT_ADR+(0x4*i), 0x0); 
	
  //Enable DMA group done interrupt after each group transfer 
  Adapter_Write32(&_CA0, V4, DMA_INT_MASK_ADR, 0xFFFF);
			
  //Enable V4 interrupts, 
  Adapter_Write32(&_CA0, V4, V4_MASK_ADR, DMA_GRP_DONE | INTR_EN);

  //Allow bridge to create PCI intr
  Adapter_Write32(&_CA0, BRIDGE, BRG_INTRMASK_ADR, BRG_INTR_EN); 
	
  // initialize the timers
  timerInit();

  return 0;
 
}

/////////////////////////////////////////////////////////////////////////

void
RR314::boardInfo() {

  unsigned int result;

  // get some of the rev numbers from the card 
  Adapter_Read32(&_CA0, BRIDGE, BRG_REV_ADR, &result);
  printf("PCI Bridge rev is %x\n", result);
  Adapter_Read32(&_CA0, V4, DMA_REV_ADR, &result);  
  printf("V4 DMA Rev (Offset 0x0) =  %x, ", result);
  Adapter_Read32(&_CA0, V4, V4_REV_ADR, &result);  
  printf("User Logic Rev (Offset 0x800) =  %x\n", result);
  printf("Current temp is %.2f C\n", ca_GetTemp(&_CA0));
	
}

/////////////////////////////////////////////////////////////////////////

void 
RR314::timerInit() 
{
  //
  //    This section initializes the timers and pulse pair processors.

  // Decimation Setup
  unsigned int Dec_Factor = _decimationFactor*2 - 1;
  Adapter_Write32(&_CA0, V4, DEC_REG, Dec_Factor);// Decimation Register
	
  //Pulse Pair Setup
  Adapter_Write32(&_CA0, V4, M_REG, _gates);               // # of Gates
  Adapter_Write32(&_CA0, V4, N_REG, _samples);             // # of Samples
  Adapter_Write32(&_CA0, V4, DPRT_REG, _dualPrt);          // Dual Prt(Off)
  Adapter_Write32(&_CA0, V4, IQ_START_IDX, _startGateIQ);  // index of start of IQ capture
  Adapter_Write32(&_CA0, V4, IQ_GATE_LEN, _nGatesIQ);      // # of Gate of IQ capture

  Adapter_Read32(&_CA0, V4, M_REG, &_gates);
  Adapter_Read32(&_CA0, V4, N_REG, &_samples);
  Adapter_Read32(&_CA0, V4, DPRT_REG, &_dualPrt);
  Adapter_Read32(&_CA0, V4, IQ_START_IDX, &_startGateIQ);
  Adapter_Read32(&_CA0, V4, IQ_GATE_LEN, &_nGatesIQ);
	
  printf("Gates = %d, Samples = %d, Dual Prt = %d\n", _gates, _samples, _dualPrt);
  printf("IQ Index = %d, IQ Length = %d\n", _startGateIQ, _nGatesIQ);
  printf("Pulse Width = %d, Decimation Factor = %d\n", _gates, _decimationFactor);

  // Reset Timers
  Adapter_Write32(&_CA0, V4, MT_DATA, 0x0);                                     // Enable Timer
  Adapter_Write32(&_CA0, V4, MT_WR,   WRITE_ON);                                // Turn on Write Strobes
  Adapter_Write32(&_CA0, V4, MT_ADDR, CONTROL_REG|TIMER0|TIMER1|TIMER2|TIMER3); // Control Register
  Adapter_Write32(&_CA0, V4, MT_ADDR, DELAY_REG|TIMER0|TIMER1|TIMER2|TIMER3);   // Address Timer 0
  Adapter_Write32(&_CA0, V4, MT_ADDR, WIDTH_REG|TIMER0|TIMER1|TIMER2|TIMER3);   // Address Timer 0
  Adapter_Write32(&_CA0, V4, MT_ADDR, PERIOD_REG|TIMER0|TIMER1|TIMER2|TIMER3);  // Address Timer 0
  Adapter_Write32(&_CA0, V4, MT_ADDR, PRT_REG|TIMER0|TIMER1|TIMER2|TIMER3);     // Mult PRT Register Timer 0
  Adapter_Write32(&_CA0, V4, MT_WR, WRITE_OFF);                                 // Turn on Write Strobes
	
  // Gating Timer Setup
  unsigned int Timers = TIMER0|TIMER1|TIMER2|TIMER3;
  printf("CONTROL REG: %x\n", CONTROL_REG|Timers|TIMER_EN);
  Adapter_Write32(&_CA0, V4, MT_ADDR, CONTROL_REG|Timers);  // Control Register
  Adapter_Write32(&_CA0, V4, MT_DATA, TIMER_ON);            // Enable Timer
  Adapter_Write32(&_CA0, V4, MT_WR,   WRITE_ON);            // Turn on Write Strobes
  
  // Delay Register
  Adapter_Write32(&_CA0, V4, MT_ADDR, DELAY_REG|Timers); // Address Timer 0
  Adapter_Write32(&_CA0, V4, MT_DATA, 0);                // Value Timer 0
  
  
  // Pulse Width Register
  Adapter_Write32(&_CA0, V4, MT_ADDR, WIDTH_REG|Timers); // Address Timer 0
  Adapter_Write32(&_CA0, V4, MT_DATA, _gates);            // Value Timer 0 (Testing Purposes)

  // Period Register
  Adapter_Write32(&_CA0, V4, MT_ADDR, PERIOD_REG|Timers); // Address Timer 0
  if (_dualPrt == 0) {
    // Adapter_Write32(&_CA0, V4, MT_DATA, 1000);  // Single PRT @ 1kHz PRF
    Adapter_Write32(&_CA0, V4, MT_DATA, 5000);     // For Power Curve Measurements
  }
  else {
    Adapter_Write32(&_CA0, V4, MT_DATA, 250);      // Mult PRT 5/4 @ 1kHz and 800Hz PRFs
  }
	
  //Multiple PRT Register
  Adapter_Write32(&_CA0, V4, MT_ADDR, PRT_REG|Timers); // Mult PRT Register Timer 0
  if (_dualPrt) {
    Adapter_Write32(&_CA0, V4, MT_DATA, 0x0054);   // Mult PRT Value Timer 0
  }
  else {
    Adapter_Write32(&_CA0, V4, MT_DATA, 0x0000);   // Mult PRT Value Timer 0
  }
	
  //Adapter_Write32(&_CA0, V4, 0xA60, 0x0);   // DC I Removal
  //Adapter_Write32(&_CA0, V4, 0xA64, 0x0);   // DC Q Removal
  
  printf("ENABLING: %x\n", PRT_REG|Timers|TIMER_EN|ADDR_TRIG);
  Adapter_Write32(&_CA0, V4, MT_ADDR, PRT_REG|Timers|TIMER_EN|ADDR_TRIG); // Set Global Enable
  Adapter_Write32(&_CA0, V4, MT_WR,   WRITE_OFF);                         // Turn off Write Strobes
  
}

////////////////////////////////////////////////////////////////////////

void 
RR314::start() 
{

  Adapter_Write32(&_CA0, V4, KAISER_ADDR, 0); 	// Start the DDC
		
  // Allow DMAs to start
  Adapter_Write32(&_CA0, V4, V4_CTL_ADR, DMA_EN | ADCA_CAP); 

}

////////////////////////////////////////////////////////////////////////

int
RR314::filterSetup()
{

  // if we don't have two filter coefficient files specified, then
  // don't try to load the filters.
  if (_gaussianFile.size() == 0 || _kaiserFile.size() == 0)
    return 0;

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

  int Decimation, Dec_Factor;

  Decimation = _decimationFactor;
  Dec_Factor = Decimation*2 - 1;

  //Decimation Setup
  Adapter_Write32(&_CA0, V4, 0x958, 0x1); // Turn on Write Strobe
  Adapter_Write32(&_CA0, V4, 0x954, Dec_Factor);// Decimation Register
  Adapter_Write32(&_CA0, V4, 0x958, 0x0);  // Turn off Write Strobe

  return 0;
}

//////////////////////////////////////////////////////////////////////
int
RR314::bytes() 
{
  int sum = 0;
  for (int i = 0; i < 16; i++) {
    sum += _bytes[i];
  }

  return sum;
}

//////////////////////////////////////////////////////////////////////
int
RR314::bytes(int chan ) 
{
  return _bytes[chan];
}

//////////////////////////////////////////////////////////////////////

/// Copy one group to the destination
void
dmaCopy(UINT32* dest, s_ChannelAdapter* pCA, int chan, int group) {

  int MaxGrpsPerCh = 2048 / (pCA->DMA.DMAChannels + 1); 
  if(MaxGrpsPerCh > 1024) //1024 is max groups per channel
    MaxGrpsPerCh = 1024;

  UINT32* src = (UINT32*)pCA->DMA.dVirtDMAAdr[(chan*MaxGrpsPerCh)+group];
  memcpy(dest, src, DMABLOCKSPERGROUP * DMABLOCKSIZEBYTES);

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
      // check for group number wrap around
      lastGroup[chan]++ ;
      if (lastGroup[chan] == DMANUMGROUPS) 
	lastGroup[chan] = 0;

      // update number of groups for this channel
      totalGroups[chan]++;
      // sum the number of bytes for this channel
      _bytes[chan] += DMABLOCKSPERGROUP*DMABLOCKSIZEBYTES;

      // add the data from the group to the buffer pool, if possible
      // otherwise ignore that group
      if (freeBuffers.size() > 0) {
	// move a buffer from the free list to the filled list
	short int* pBuf = freeBuffers[0];
	//fullBuffers.push_back(freeBuffers[0]);
	//freeBuffers.pop_front();
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
  unsigned int Status;
  unsigned int Mask;
  unsigned int Control;
  unsigned int DMAMask;
  unsigned int DMAStatus;

  Adapter_Read32(pCA, V4, V4_STAT_ADR,       &Status);
  Adapter_Read32(pCA, V4, V4_MASK_ADR,       &Mask);
  Adapter_Read32(pCA, V4, V4_CTL_ADR,        &Control);
  Adapter_Read32(pCA, V4, DMA_INT_MASK_ADR,  &DMAMask);
  Adapter_Read32(pCA, V4, DMA_INT_STAT_ADR,  &DMAStatus);

  // see if we have a DMA interrupt
  if (Status & DMA_GRP_DONE) {
    // yes, process groups for each DMA channel
    for (int chan = 0; chan < 16; chan++) {
      if ((1 << chan) & DMAStatus) {
	processDMAGroups(pCA, chan);
      } 
    }  
  }   

  // print the running total of groups transferred
  //  for (int c = 0 ; c < 16; c++) {
  //  printf ("%04d ", totalGroups[c]);
  //}
  //printf("\n");

  // check for AD fifos full.
  int result = 0;
  if((Mask & Status) & ADCA_FF_FULL)    {
    result |= ADCAFF_FLUSH;
    std::cout << "ADCA fifo full\n";
    fifoFullInts++;
  }
    
  if((Mask & Status) & ADCB_FF_FULL)   {
    result |= ADCBFF_FLUSH;
    std::cout << "ADCB fifo full\n";
    fifoFullInts++;
  }

  if((Mask & Status) & ADCC_FF_FULL)         {
    result |= ADCCFF_FLUSH;
    std::cout << "ADCC fifo full\n";
    fifoFullInts++;
  }
  
  if((Mask & Status) & ADCD_FF_FULL)    {
    result |= ADCDFF_FLUSH;
    std::cout << "ADCD fifo full\n";
    fifoFullInts++;
  }

  /// @todo if any of the a/ds had full fifos, then
  /// the dma and A/Ds need to be reenabled. This is an artifact
  /// from some old RR code, does it really need to
  /// be doen? Does Tom's bistream observe this?
  if (result) {
    result |= DMA_EN;
    Adapter_Write32(pCA, V4, V4_CTL_ADR, result);

    //  Enable FIFOs to collect ADC data
    result |= ADCA_CAP |  ADCB_CAP |  ADCC_CAP |  ADCD_CAP;
    Adapter_Write32(pCA, V4, V4_CTL_ADR, result);
  }

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
