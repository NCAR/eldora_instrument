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

s_ChMemBuffer MemBuffer_Ch[16];	//Large memory buffers, data from DMA space will be moved here

unsigned int ChXferDone;	       //Bit map for channels than have completed transfer	

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

/// Running total of FIFO full interrupts
int fifoFullInts = 0;

/// A running total of the data samples that have
/// been transfered via DMA. Used to calculate
/// the observed throughput.
int sampleCount = 0;

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

  _deviceName = "/dev/windrvr6";

  _deviceFd = open(_deviceName.c_str(), O_RDONLY | O_NONBLOCK);

  if (_deviceFd < 0) {
    std::cerr << "cannot access " << _deviceName << "\n";
    perror("");
    exit(1);
  }

  // configure the Red River card

  if ( configureRedRiver()) {
    std::cerr << "Unable to configure the Red Rapids card" << std::endl;
    shutdown();
    exit (1);
  }     

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

int
RR314::configureRedRiver()
{

  int GrpsToCapture = 2;
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

  unsigned int Dummy;

  //Flags
  int ProgramFPGA;                  // program fpga
  ProgramFPGA = 0;

  int V4LoadCheck;                  // if set, verify that V4 has been loaded
  V4LoadCheck         = _xsvfFileName.size()>0; 
  V4LoadCheck         = 0;

  pCA0 = &_CA0;

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
      Adapter_Read32(&_CA0, BRIDGE, BRG_FPGA_STAT, &Dummy);
      if(!(Dummy & FPGA_PROG_DONE))
	{
	  printf("V4 is not loaded, please check PROM contents or load an XSVF file to the V4.\n");
	  Adapter_Close(&_CA0);
	  return -1;	
	}
    }
	
  // get some of the rev numbers from the card 
  Adapter_Read32(&_CA0, BRIDGE, BRG_REV_ADR, &Dummy);
  printf("PCI Bridge rev is %x\n", Dummy);
  Adapter_Read32(&_CA0, V4, DMA_REV_ADR, &Dummy);  
  printf("V4 DMA Rev (Offset 0x0) =  %x, ", Dummy);
  Adapter_Read32(&_CA0, V4, V4_REV_ADR, &Dummy);  
  printf("User Logic Rev (Offset 0x800) =  %x\n", Dummy);
	
  // Display current board temp
  printf("Current temp is %.2f C\n", ca_GetTemp(&_CA0));
	
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
  Adapter_Read32(&_CA0, V4, V4_STAT_ADR, &Dummy); //Clear old status reg
  Adapter_Read32(&_CA0, V4, V4_STAT_ADR, &Dummy);
  if (Dummy & ADC_DCM_UNLOCKED) {
    printf("DCM Failed to Lock. STATUS REG = %x\n Exiting Now...\n", Dummy);
    Adapter_Close(&_CA0);
    return -1;
  }
				
  // Set M314 VRANGE mode.  This will touch the register that controls the sample
  // clk select, so it is done as RMW.
  // Set ADC range to 2Vpp 
  Adapter_Read32(&_CA0, BRIDGE, BRG_HWCONF_ADR, &Dummy);
  Dummy |= ADCA_VRNG1;
  Adapter_Write32(&_CA0, BRIDGE, BRG_HWCONF_ADR, Dummy);
		
  // Flush FIFOs
  Adapter_Write32(&_CA0, V4, V4_CTL_ADR, ADCAFF_FLUSH);
			
  ///////////////////////////////////////////////////////////////////////////////
  //	
  // Allocate DMA space for each channel to trasfer data into.  Once a group has finshed an interrupt will
  // be signaled and the data will be copied away to the large holding buffer. 
		
  printf("Allocating DMA space...");
  // Bitstream supports 16 DMA channels, indexed to 0
  _CA0.DMA.DMAChannels = 15;				
			
  unsigned int i;
  for (i = 0; i < _CA0.DMA.DMAChannels+1; i++) {
    int chan = i;
    _CA0.DMA.GrpCnt[chan]      = DMANUMGROUPS-1;  
    _CA0.DMA.BlockSizeB[chan]  = DMABLOCKSIZE*8; 
    _CA0.DMA.BlockCount[chan]  = DMABLOCKSPERGROUP-1;   
    //_CA0.DMA.GrpCnt[chan]     = 7;       // 8 Groups (indexed to 0)
    //_CA0.DMA.BlockSizeB[chan] = 1024;    // 8kB Blocks (half a FIFO)
    //_CA0.DMA.BlockCount[chan] = 15;      // 16 Blocks (index to 0) for GrpSize of 128kB, Linux max
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
	

  //Setup large storage buffer to move data to after each DMA group is done
  for (i = 0; i < _CA0.DMA.DMAChannels+1; i++) {
    int chan = i;
    if (ca_MemBuffer_Init(&_CA0, &MemBuffer_Ch[chan], chan, GrpsToCapture))
      {
	fprintf(stderr, "Could not allocate storage buffer, will now exit.\n");
	Adapter_Close(&_CA0);
	return 0;
      }
  }
	
  // Enable interrupt generation user logic.  
  // Set to interrupt every group for all active channels
  for(i = 0; i < _CA0.DMA.DMAChannels+1; i++)
    Adapter_Write32(&_CA0, V4, DMA_CH0_GRPSPERINT_ADR+(0x4*i), 0x0); 
	
  //Enable DMA group done interrupt after each group transfer 
  Adapter_Write32(&_CA0, V4, DMA_INT_MASK_ADR, 0xFFFF);
			
  //Enable V4 interrupts, 
  Adapter_Write32(&_CA0, V4, V4_MASK_ADR, DMA_GRP_DONE | INTR_EN);

  //Allow bridge to create PCI intr
  Adapter_Write32(&_CA0, BRIDGE, BRG_INTRMASK_ADR, BRG_INTR_EN); 
	

  /////////////////////////////////////////////////////////////////////////
  //                Timer and Pulse Pair Init Block
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
  
  Adapter_Write32(&_CA0, V4, KAISER_ADDR, 0); 	// Start the DDC
		
  // Set a bit for each active ch.  Will be cleared by ISR as channels finish
  ChXferDone = 0xFFFF;

  // Allow DMAs to start
  Adapter_Write32(&_CA0, V4, V4_CTL_ADR, DMA_EN | ADCA_CAP); 

  return 0;
 
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

/// Copy one group to the destination
void
dmaCopy(UINT32* dest, s_ChannelAdapter* pCA, int chan, int group) {

  int MaxGrpsPerCh = 2048 / (pCA->DMA.DMAChannels + 1); 
  if(MaxGrpsPerCh > 1024) //1024 is max groups per channel
    MaxGrpsPerCh = 1024;

  UINT32* src = (UINT32*)pCA->DMA.dVirtDMAAdr[(chan*MaxGrpsPerCh)+group];
  memcpy(dest, src, DMABLOCKSPERGROUP * DMABLOCKSIZE * 8);

  std::cout << "chan " << chan << ",  group " << group << std::endl;

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

  //  printf("Adapter_ISR: Device %d, Status %x, Mask %x, Control %x, DMA Mask %x, 
  //     DMA Status %x FifoFullInts %d sampleCout %d\n", 
  // pCA->DevNum, Status, Mask, Control, DMAMask, DMAStatus, fifoFullInts, sampleCount);

  // see if we have a DMA interrupt
  if (Status & DMA_GRP_DONE) {
    // yes, process groups for each DMA channel
    for (int chan = 0; chan < 16; chan++) {
      if ((1 << chan) & DMAStatus) {
	processDMAGroups(pCA, chan);
      } 
    }  
  }   

  int Dummy = 0;
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
