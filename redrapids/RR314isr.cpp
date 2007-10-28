#include "RR314isr.h"
#include <iostream>

using namespace RedRapids;

//////////////////////////////////////////////////////////////////////

void
sendGroupToRR314(s_ChannelAdapter* pCA, int chan, RR314* pRR314) {

  int group = pRR314->lastGroup(chan);

  int MaxGrpsPerCh = 2048 / (pCA->DMA.DMAChannels + 1); 
  if(MaxGrpsPerCh > 1024) //1024 is max groups per dmaChan
    MaxGrpsPerCh = 1024;

  // send it to RR314
  if (chan %1) {
      // abp dmaChan
      int* src = (int*)pCA->DMA.dVirtDMAAdr[(chan*MaxGrpsPerCh)+group];
      pRR314->newABPData(src, chan, DMABLOCKSPERGROUP * DMABLOCKSIZEBYTES/(sizeof (int)));
  } else {
      // iq dmaChan
      short* src = (short*)pCA->DMA.dVirtDMAAdr[(chan*MaxGrpsPerCh)+group];
      pRR314->newIQData(src, chan, DMABLOCKSPERGROUP * DMABLOCKSIZEBYTES/(sizeof (short)));
  }

  return;
}

//////////////////////////////////////////////////////////////////////

void processDMAGroups(s_ChannelAdapter *pCA, int chan, RR314* pRR314) {

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

    while (pRR314->lastGroup(chan) != (int)currentGroup) {

      // update the last group index
      pRR314->lastGroup(chan, pRR314->lastGroup(chan)+1);
      // handle the wrap around
      if (pRR314->lastGroup(chan) == DMANUMGROUPS) 
	pRR314->lastGroup(chan, 0);

      // send the group to our RR314 class
      sendGroupToRR314(pCA, chan, pRR314);
    } 
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
    // yes, process groups for each DMA dmaChan
    for (int chan = 0; chan < 8; chan++) {
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

