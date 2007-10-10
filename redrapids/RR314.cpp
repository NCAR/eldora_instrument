#include <signal.h>
#include "RR314.h"
#include "RR314sim.h"
#include <iostream>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>
#include <deque>
#include <map>

using namespace RedRapids;

// instantiate the map which will record all instances
// of the RR314 class.
std::map<s_ChannelAdapter*, RR314*> RedRapids::RR314::rr314Instances;
 
//////////////////////////////////////////////////////////////////////

RR314::RR314(int devNum,
	     unsigned int gates,
	     unsigned int samples,
	     unsigned int dualPrt,
	     unsigned int startGateIQ,
	     unsigned int nGatesIQ,
	     unsigned int decimationFactor, 
	     std::string gaussianFile,
	     std::string kaiserFile,
	     std::string xsvfFileName,
	     bool simulate) throw(std::string):
  _devNum(devNum),
  _gates(gates),
  _samples(samples),
  _dualPrt(dualPrt),
  _startGateIQ(startGateIQ),
  _nGatesIQ(nGatesIQ),
  _decimationFactor(decimationFactor),
  _gaussianFile(gaussianFile),
  _kaiserFile(kaiserFile),
  _xsvfFileName(xsvfFileName),
  _bufferNext(0),
  _simulate(simulate)
{

  // initalize threading constructs
  pthread_mutex_init(&_bufferMutex, NULL);
  pthread_cond_init(&_dataAvailCond, NULL);

  if (simulate) {
    std::cout << "*** RR314 operating in simulation mode, without hardware\n";
  } else {
    std::cout << "*** This version of RR314 works with CA_DDC_4.xsvf\n";
  }

  // save a reference to our instance so that the isr
  // can locate us
  rr314Instances[&_chanAdapter] = this;

  // initialize the byte counters and last groups
  _bytes.resize(16);
  for (int c = 0; c < 16; c++) {
    _bytes[c] = 0;
    lastGroup(c, -1);
  }

  // allocate the buffers
  pthread_mutex_lock(&_bufferMutex);
  for (int i = 0; i < BUFFERPOOLSIZE; i++) {
    int* buf = new int[DMABLOCKSIZEBYTES*DMABLOCKSPERGROUP];
    _freeBuffers.push_back(buf);
  }
  pthread_mutex_unlock(&_bufferMutex);

  if (_simulate) {
    _simulator = new RedRapids::RR314sim(this);
    return;
  }

  std::string deviceName = "/dev/windrvr6";

  int deviceFd = open(deviceName.c_str(), O_RDONLY | O_NONBLOCK);

  if (deviceFd < 0) {
    std::string e("cannot access ");
    e += deviceName;
    throw(e);
  }

  // capture signals
  catchSignals();

  // configure the card
  if ( configure314()) {
    std::string e("Unable to configure the Red Rapids card ");
    throw(e);
  }     

  // display some board information
  boardInfo();

}

////////////////////////////////////////////////////////////////////////

RR314::~RR314() 
{
  // disable the hardware
  if (!_simulate) {
      RR314shutdown();
  } else {
    /// @todo Need to add thread termination for RR314sim
    /// during class destruction
    delete _simulator;
  }


  for (unsigned int i = 0; i < _freeBuffers.size(); i++ )
    delete [] _freeBuffers[i];

  for (unsigned int i = 0; i < _fullBuffers.size(); i++ )
    delete [] _fullBuffers[i];

  for (unsigned int i = 0; i < _freeBuffers.size(); i++ )
    delete [] _freeBuffers[i];

  for (unsigned int i = 0; i < _fullBuffers.size(); i++ )
    delete [] _fullBuffers[i];

  std::cout << "RR314 deleted\n";
}

//////////////////////////////////////////////////////////////////////

void
RR314::RR314shutdown() {

  Adapter_Write32(&_chanAdapter, BRIDGE, BRG_INTRMASK_ADR, BRG_INTR_DIS);
  Adapter_Write32(&_chanAdapter, V4, V4_CTL_ADR, 0x0);

  Adapter_DMABufFree(&_chanAdapter);

  Adapter_Close(&_chanAdapter); 
  
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
RR314::configure314() {

  unsigned int result;

  //Flags
  int ProgramFPGA;                  // program fpga
  ProgramFPGA = 0;

  int V4LoadCheck;                  // if set, verify that V4 has been loaded
  V4LoadCheck         = _xsvfFileName.size()>0; 
  V4LoadCheck         = 1;

  //Init all flags to default values
  Adapter_Zero(&_chanAdapter);

  // The device number
  _chanAdapter.DevNum = _devNum;

  // set assembly information, whatever that is
  strcpy(_chanAdapter.Asy, "M314"); 


  // Open channel adapter
  if(Adapter_Open(&_chanAdapter)) {
    fprintf(stderr, "Failed to find and/or open card. (%x)\n", _chanAdapter.ReturnStatus);
    return -1;
  } else {
    printf("Opened ChannelAdapter device %d\n", _chanAdapter.DevNum);
  }

  // Disable all interupts
  Adapter_Write32(&_chanAdapter, 
		  V4, 
		  V4_MASK_ADR, 
		  0);

  //Load the v4 if needed
  if(_xsvfFileName.size() > 0) {
    std::cout << "Loading bitstream " << _xsvfFileName << std::endl;
    char name[_xsvfFileName.size()+1];
    strcpy(name, _xsvfFileName.c_str());
    if(Adapter_LoadXSVF(&_chanAdapter, name, 1)) {
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

  // Disable all interupts
  Adapter_Write32(&_chanAdapter, V4, V4_MASK_ADR, 0x0);
	
  //Check the V4 has a valid load.  
  if(V4LoadCheck)
    {
      Adapter_Read32(&_chanAdapter, BRIDGE, BRG_FPGA_STAT, &result);
      if(!(result & FPGA_PROG_DONE))
	{
	  printf("V4 is not loaded, please check PROM contents or load an XSVF file to the V4.\n");
	  Adapter_Close(&_chanAdapter);
	  return -1;	
	}
    }
	
  // set the sample clock
  _ClkSettings.ClkSrc = SYNTH;    // can be either SYNTH or EXT
  Adapter_SampleClkSelect(&_chanAdapter, &_ClkSettings);

  // Code here should reset your DCMs and allow time for them to lock,
  // then check their status
  //Adapter_uSleep(1000000);

  // Soft Reset, self clearing
  Adapter_Write32(&_chanAdapter, V4, V4_CTL_ADR, SOFT_RST); 

  // Wait for DCM to Relock
  Adapter_uSleep(1000000);     
		
  // Check for DCM Lock
  Adapter_Read32(&_chanAdapter, V4, V4_STAT_ADR, &result); //Clear old status reg
  Adapter_Read32(&_chanAdapter, V4, V4_STAT_ADR, &result);
  if (result & ADC_DCM_UNLOCKED) {
    printf("DCM Failed to Lock. STATUS REG = %x\n Exiting Now...\n", result);
    Adapter_Close(&_chanAdapter);
    return -1;
  }
				
  // Set M314 VRANGE mode.  This will touch the register that controls the sample
  // clk select, so it is done as RMW.
  // Set ADC range to 2Vpp 
  Adapter_Read32(&_chanAdapter, BRIDGE, BRG_HWCONF_ADR, &result);
  result |= ADCA_VRNG1;
  Adapter_Write32(&_chanAdapter, BRIDGE, BRG_HWCONF_ADR, result);
		
  // Flush FIFOs
  Adapter_Write32(&_chanAdapter, V4, V4_CTL_ADR, ADCAFF_FLUSH);
			
  ///////////////////////////////////////////////////////////////////////////////
  //	
  // Allocate DMA space for each channel. The channel 
  // adapter library will create the scatter/gather 
  // memory references which are then downloaded
  // to the RR card for use by the DMA engine.
		
  printf("Allocating DMA space...");
  // Bitstream supports 16 DMA channels, indexed to 0
  _chanAdapter.DMA.DMAChannels = 15;				
			
  unsigned int i;
  for (i = 0; i < _chanAdapter.DMA.DMAChannels+1; i++) {
    int chan = i;
    _chanAdapter.DMA.GrpCnt[chan]      = DMANUMGROUPS-1;  
    _chanAdapter.DMA.BlockSizeB[chan]  = DMABLOCKSIZEBYTES; 
    _chanAdapter.DMA.BlockCount[chan]  = DMABLOCKSPERGROUP-1;   
  }		
			
  if(Adapter_DMABufAllocate(&_chanAdapter))
    {
      // unable to allocate DMA space
      fprintf(stderr, "Failure allocating DMA memory\n");
      Adapter_Close(&_chanAdapter);
      return -1;
    }
  else	
    {
      // DMA allocated, load to card
      ca_LoadDMASettings(&_chanAdapter);
    }
  printf("DMA memory allocation done.\n");
	
  // Enable DMA interrupt on every group for all 
  for(i = 0; i < _chanAdapter.DMA.DMAChannels+1; i++)
    Adapter_Write32(&_chanAdapter, V4, DMA_CH0_GRPSPERINT_ADR+(0x4*i), 0x0); 
	
  //Enable DMA group done interrupt after each group transfer 
  Adapter_Write32(&_chanAdapter, V4, DMA_INT_MASK_ADR, 0xFFFF);
			
  //Enable V4 interrupts, 
  Adapter_Write32(&_chanAdapter, V4, V4_MASK_ADR, DMA_GRP_DONE | INTR_EN);

  //Allow bridge to create PCI intr
  Adapter_Write32(&_chanAdapter, BRIDGE, BRG_INTRMASK_ADR, BRG_INTR_EN); 
	
  // initialize the timers
  timerInit();

  return 0;
 
}

/////////////////////////////////////////////////////////////////////////
void
RR314::newData(unsigned int* src, int chan, int n) {

  pthread_mutex_lock(&_bufferMutex);

  if (_freeBuffers.size()) {
    int* pBuf = _freeBuffers[0];
    _fullBuffers.push_back(pBuf);
    _freeBuffers.pop_front();
    for (int i = 0; i < n; i++) {
      pBuf[i] = src[i];
    }
    addBytes(chan, n*sizeof(*src));
    // signal that new data is available
    pthread_cond_broadcast(&_dataAvailCond);
  } else {
    /// @todo add dropped buffer accounting here
  }

  pthread_mutex_unlock(&_bufferMutex);
}

/////////////////////////////////////////////////////////////////////////

int*
RR314::nextBuffer() {
  int* pBuf = 0;

  pthread_mutex_lock(&_bufferMutex);

  while (_fullBuffers.size() == 0 ) {
    pthread_cond_wait(&_dataAvailCond, &_bufferMutex);
  }

  pBuf = _fullBuffers[0];

  _fullBuffers.pop_front();

  pthread_mutex_unlock(&_bufferMutex);

  return pBuf;
}

////////////////////////////////////////////////////////////////////////

void
RR314::returnBuffer(int* buf) {
  pthread_mutex_lock(&_bufferMutex);

  _freeBuffers.push_back(buf);

  pthread_mutex_unlock(&_bufferMutex);
}
////////////////////////////////////////////////////////////////////////

int
RR314::numFreeBuffers() {
  pthread_mutex_lock(&_bufferMutex);

  int result = _freeBuffers.size();

  pthread_mutex_unlock(&_bufferMutex);
  
  return result;
}

/////////////////////////////////////////////////////////////////////////

void 
RR314::fifoFullInts(int n) {
  _fifoFullInts = n;
}

/////////////////////////////////////////////////////////////////////////

int 
RR314::fifoFullInts() {
  return _fifoFullInts;
}

/////////////////////////////////////////////////////////////////////////

void
RR314::boardInfo() {

  if (_simulate)
    return;

  unsigned int result;

  // get some of the rev numbers from the card 
  Adapter_Read32(&_chanAdapter, BRIDGE, BRG_REV_ADR, &result);
  printf("PCI Bridge rev is %x\n", result);
  Adapter_Read32(&_chanAdapter, V4, DMA_REV_ADR, &result);  
  printf("V4 DMA Rev (Offset 0x0) =  %x, ", result);
  Adapter_Read32(&_chanAdapter, V4, V4_REV_ADR, &result);  
  printf("User Logic Rev (Offset 0x800) =  %x\n", result);
  printf("Current temp is %.2f C\n", ca_GetTemp(&_chanAdapter));
	
}

/////////////////////////////////////////////////////////////////////////

void 
RR314::timerInit() 
{
  //
  //    This section initializes the timers and pulse pair processors.

  // Decimation Setup
  unsigned int Dec_Factor = _decimationFactor*2 - 1;
  Adapter_Write32(&_chanAdapter, V4, DEC_REG, Dec_Factor);// Decimation Register
	
  //Pulse Pair Setup
  Adapter_Write32(&_chanAdapter, V4, M_REG, _gates);               // # of Gates
  Adapter_Write32(&_chanAdapter, V4, N_REG, _samples);             // # of Samples
  Adapter_Write32(&_chanAdapter, V4, DPRT_REG, _dualPrt);          // Dual Prt(Off)
  Adapter_Write32(&_chanAdapter, V4, IQ_START_IDX, _startGateIQ);  // index of start of IQ capture
  Adapter_Write32(&_chanAdapter, V4, IQ_GATE_LEN, _nGatesIQ);      // # of Gate of IQ capture

  Adapter_Read32(&_chanAdapter, V4, M_REG, &_gates);
  Adapter_Read32(&_chanAdapter, V4, N_REG, &_samples);
  Adapter_Read32(&_chanAdapter, V4, DPRT_REG, &_dualPrt);
  Adapter_Read32(&_chanAdapter, V4, IQ_START_IDX, &_startGateIQ);
  Adapter_Read32(&_chanAdapter, V4, IQ_GATE_LEN, &_nGatesIQ);
	
  printf("Gates = %d, Samples = %d, Dual Prt = %d\n", _gates, _samples, _dualPrt);
  printf("IQ Index = %d, IQ Length = %d\n", _startGateIQ, _nGatesIQ);
  printf("Pulse Width = %d, Decimation Factor = %d\n", _gates, _decimationFactor);

  // Reset Timers
  Adapter_Write32(&_chanAdapter, V4, MT_DATA, 0x0);                                     // Enable Timer
  Adapter_Write32(&_chanAdapter, V4, MT_WR,   WRITE_ON);                                // Turn on Write Strobes
  Adapter_Write32(&_chanAdapter, V4, MT_ADDR, CONTROL_REG|TIMER0|TIMER1|TIMER2|TIMER3); // Control Register
  Adapter_Write32(&_chanAdapter, V4, MT_ADDR, DELAY_REG|TIMER0|TIMER1|TIMER2|TIMER3);   // Address Timer 0
  Adapter_Write32(&_chanAdapter, V4, MT_ADDR, WIDTH_REG|TIMER0|TIMER1|TIMER2|TIMER3);   // Address Timer 0
  Adapter_Write32(&_chanAdapter, V4, MT_ADDR, PERIOD_REG|TIMER0|TIMER1|TIMER2|TIMER3);  // Address Timer 0
  Adapter_Write32(&_chanAdapter, V4, MT_ADDR, PRT_REG|TIMER0|TIMER1|TIMER2|TIMER3);     // Mult PRT Register Timer 0
  Adapter_Write32(&_chanAdapter, V4, MT_WR, WRITE_OFF);                                 // Turn on Write Strobes
	
  // Gating Timer Setup
  unsigned int Timers = TIMER0|TIMER1|TIMER2|TIMER3;
  printf("CONTROL REG: %x\n", CONTROL_REG|Timers|TIMER_EN);
  Adapter_Write32(&_chanAdapter, V4, MT_ADDR, CONTROL_REG|Timers);  // Control Register
  Adapter_Write32(&_chanAdapter, V4, MT_DATA, TIMER_ON);            // Enable Timer
  Adapter_Write32(&_chanAdapter, V4, MT_WR,   WRITE_ON);            // Turn on Write Strobes
  
  // Delay Register
  Adapter_Write32(&_chanAdapter, V4, MT_ADDR, DELAY_REG|Timers); // Address Timer 0
  Adapter_Write32(&_chanAdapter, V4, MT_DATA, 0);                // Value Timer 0
  
  
  // Pulse Width Register
  Adapter_Write32(&_chanAdapter, V4, MT_ADDR, WIDTH_REG|Timers); // Address Timer 0
  Adapter_Write32(&_chanAdapter, V4, MT_DATA, _gates);            // Value Timer 0 (Testing Purposes)

  // Period Register
  Adapter_Write32(&_chanAdapter, V4, MT_ADDR, PERIOD_REG|Timers); // Address Timer 0
  if (_dualPrt == 0) {
    // Adapter_Write32(&_chanAdapter, V4, MT_DATA, 1000);  // Single PRT @ 1kHz PRF
    Adapter_Write32(&_chanAdapter, V4, MT_DATA, 5000);     // For Power Curve Measurements
  }
  else {
    Adapter_Write32(&_chanAdapter, V4, MT_DATA, 250);      // Mult PRT 5/4 @ 1kHz and 800Hz PRFs
  }
	
  //Multiple PRT Register
  Adapter_Write32(&_chanAdapter, V4, MT_ADDR, PRT_REG|Timers); // Mult PRT Register Timer 0
  if (_dualPrt) {
    Adapter_Write32(&_chanAdapter, V4, MT_DATA, 0x0054);   // Mult PRT Value Timer 0
  }
  else {
    Adapter_Write32(&_chanAdapter, V4, MT_DATA, 0x0000);   // Mult PRT Value Timer 0
  }
	
  Adapter_Write32(&_chanAdapter, V4, MT_ADDR, PRT_REG|Timers|TIMER_EN|ADDR_TRIG); // Set Global Enable
  Adapter_Write32(&_chanAdapter, V4, MT_WR,   WRITE_OFF);                         // Turn off Write Strobes
  
}

////////////////////////////////////////////////////////////////////////

void 
RR314::start() 
{
  if (_simulate) {
    _simulator->start();
    return;
  }

  // Start the DDC
  Adapter_Write32(&_chanAdapter, V4, KAISER_ADDR, 0);
		
  // Enable the DMAs and turn on the ADCs
  Adapter_Write32(&_chanAdapter, V4, V4_CTL_ADR, DMA_EN | ADCA_CAP); 

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
    return -1;
  }

  // for Tom's bitstream:

  int Decimation, Dec_Factor;

  Decimation = _decimationFactor;
  Dec_Factor = Decimation*2 - 1;

  //Decimation Setup
  Adapter_Write32(&_chanAdapter, V4, 0x958, 0x1); // Turn on Write Strobe
  Adapter_Write32(&_chanAdapter, V4, 0x954, Dec_Factor);// Decimation Register
  Adapter_Write32(&_chanAdapter, V4, 0x958, 0x0);  // Turn off Write Strobe

  return 0;
}

//////////////////////////////////////////////////////////////////////
void
RR314::addBytes(int chan, int bytes) {
  _bytes[chan] += bytes;
}

//////////////////////////////////////////////////////////////////////
std::vector<unsigned long>
RR314::bytes() 
{
  std::vector<unsigned long> result = _bytes;
  for (int i = 0; i < 16; i++)
    _bytes[i] = 0;

  return result;
}

//////////////////////////////////////////////////////////////////////

void
RR314::lastGroup(int chan, int group) {
  _lastGroup[chan] = group;
}

//////////////////////////////////////////////////////////////////////
int
RR314::lastGroup(int chan)
{
  return _lastGroup[chan];
}

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
void
shutdownSignalHandler(int signo){

  // stop all RR314 cards amd free their DMA allocations
  // iterate through all instances of RR314

  std::map<s_ChannelAdapter*, RR314*>::iterator  p;
  for (p = RR314::rr314Instances.begin(); p != RR314::rr314Instances.end(); p++) {
    
    s_ChannelAdapter* pCA = p->first;

    std::cout << "Stopping RR314 device " << pCA->DevNum << std::endl;
    p->second->RR314shutdown();

  }

  std::cout << "caught signal; exiting...\n";
  exit(1);
}
