#include <signal.h>
#include "RR314.h"
#include <iostream>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <deque>
#include <map>

// statics that must be accessed both by the RR314
// objects and the C ISR routine.

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
extern s_ChannelAdapter* pCA0;

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

void processDMAGroups(s_ChannelAdapter *pCA, int chan, RR314* pRR314) 
{

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
    pthread_mutex_lock(&(pRR314->bufferMutex));

    while (pRR314->lastGroup(chan) != (int)currentGroup) {

      // update the last group index
      pRR314->lastGroup(chan, pRR314->lastGroup(chan)+1);
      // handle the wrap around
      if (pRR314->lastGroup(chan) == DMANUMGROUPS) 
	pRR314->lastGroup(chan, 0);

      // sum the number of bytes for this channel
      pRR314->addBytes(chan, DMABLOCKSPERGROUP*DMABLOCKSIZEBYTES);

      // add the data from the group to the buffer pool, if possible
      // otherwise ignore that group
      if (pRR314->freeBuffers.size() > 0) {
	// move a buffer from the free list to the filled list
	short int* pBuf = pRR314->freeBuffers[0];
	//fullBuffers.push_back(pRR314->freeBuffers[0]);
	//pRR314->freeBuffers.pop_front();
	// copy dma region into that buffer
	dmaCopy((UINT32*)pBuf, pCA, chan, pRR314->lastGroup(chan));
      } else {
	groupsDropped++;
      }
    } 

    // release lock on the buffer pool
    pthread_mutex_unlock(&(pRR314->bufferMutex));

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

  // find out the RR314 instances associated with this interrupt.
  RR314* pRR314 = RR314::rr314Instances[pCA];

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
	processDMAGroups(pCA, chan, pRR314);
      } 
    }  
  }   

  // check for AD fifos full.
  int result = 0;
  if((Mask & Status) & ADCA_FF_FULL)    {
    result |= ADCAFF_FLUSH;
    std::cout << "ADCA fifo full\n";
    pRR314->fifoFullInts(pRR314->fifoFullInts()+1);;
  }
    
  if((Mask & Status) & ADCB_FF_FULL)   {
    result |= ADCBFF_FLUSH;
    std::cout << "ADCB fifo full\n";
    pRR314->fifoFullInts(pRR314->fifoFullInts()+1);;
  }

  if((Mask & Status) & ADCC_FF_FULL)         {
    result |= ADCCFF_FLUSH;
    std::cout << "ADCC fifo full\n";
    pRR314->fifoFullInts(pRR314->fifoFullInts()+1);;
  }
  
  if((Mask & Status) & ADCD_FF_FULL)    {
    result |= ADCDFF_FLUSH;
    std::cout << "ADCD fifo full\n";
    pRR314->fifoFullInts(pRR314->fifoFullInts()+1);;
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
 
  return;
}

//////////////////////////////////////////////////////////////////////
void
shutdownSignalHandler(int signo){

  Adapter_Write32(pCA0, BRIDGE, BRG_INTRMASK_ADR, BRG_INTR_DIS);
  Adapter_Write32(pCA0, V4, V4_CTL_ADR, 0x0);
  Adapter_DMABufFree(pCA0);
  Adapter_Close(pCA0); 
  std::cout << "caught signal; exiting...\n";

  exit(1);
}
