/*****************************************************************************************

 Red Rapids 
 797 N. Grove Rd
 Suite 101
 Richardson, TX 75081
 support@redrapids.com


 ChannelAdapter Local Bus Demo Program

 Desc: This program shows how to capture data from the ChannelAdapter LocalBus interface.
   The example assumes the 4 ch LB interface is used.  This program is only for use 
   with the CA Example FPGA code.  When the DMA Go bit (0x0 of 0x80C) is set to 1, 
   each channel will move a sythentic data set across the bus as fast as the bus will
   allow.  Each 32bit word will consist of a channel ID on [31:27] and a count on
   [26:0].

   As each DMA groups is done it will be copied from the DMA buffer to a seperate 
   holding buffer so that the DMA buffer can be reused.  When the progam is done 
   collecting data it will write each channel out to a txt file.   
 
 Rev History 
    
    1.2 - Release against R05 of API
    1.1 - Released against R04 of API
    1.0 - [28 August 2006 ASB (abixby@redrapids.com) & PTJ (jennings@redrapids.com)
     Inital Release

******************************************************************************************/

#define MAJ_REV 1
#define MIN_REV 2
#define SUB_REV 0x1120


//V4 User Registers
 #define V4_REV_ADR   0x800
 #define V4_CTL_ADR   0x80C 


#include <stdio.h>
#include <stdlib.h>


#include "ca_bar0_memmap.h"
#include "ca_lb_memmap.h"
#include "ca_diag_memmap.h"
#include "channeladapter.h"
#include "ca_functions.h"
#include "ca_membuffer.h"
#include "ca_registers.h"


#ifdef WIN32
	#include <conio.h>
	#include <windows.h>
	#include <malloc.h>
#else
	
#endif


DWORD ChXferDone;		//Bit map for channels than have completed transfer	

s_ChMemBuffer MemBuffer_Ch[16];		//Large memory buffers, data from DMA space will be moved here

//Set DDC Values
DWORD decimation = 12;
DWORD gates	 = 1000;
DWORD samples	 = 100;
DWORD dprt	 = 0;
DWORD index	 = 0;
DWORD length	 = 10;
DWORD Timers	 = TIMER0|TIMER1;

int main(int argc, char* argv[]) 
{
	
	s_ChannelAdapter CA0;		/*Handle to ChannelAdapter device*/
	s_ClkSettings ClkSettings;	/*Struct of sample clk setttings*/
	
	DWORD Dummy;
	
	//Flags to set from command line
	int GrpsToCapture;	
	int V4LoadCheck;			/*Set to 1 to force check the V4 is loaded*/

	//Set default values
	V4LoadCheck   = 1;
	GrpsToCapture = 2;
	
	printf("Red Rapids\n");
	printf("ChannelAdapter LocalBus Demo Program v %x.%x.%x\n", MAJ_REV, MIN_REV, SUB_REV);
	printf("Built %s @ %s\n", __DATE__, __TIME__);
	printf("\n%s\n", Adapter_GetAPIVerInfo(&Dummy));

	
	//Init all flags to default values
	Adapter_Zero(&CA0);
	CA0.DevNum = 0;
	strcpy(CA0.Asy, "M314");	//Default to M314 hw
	ClkSettings.ClkSrc = SYNTH;     // can be either SYNTH or EXT

	ChXferDone = 0;			//A bit will be set high for each active ch.  When the data collect for that ch is
					//done the bit will be set low.

	//Open hardware link
	if(Adapter_Open(&CA0))
	{
		fprintf(stderr, "Failed to find and/or open card. (%x)\n", CA0.ReturnStatus);
		return -1;
	}
	else
	{
		printf("Opened ChannelAdapter device %d\n", CA0.DevNum);
	}
	
	// Disable all interupts
  	Adapter_Write32(&CA0, V4, V4_MASK_ADR, 0x0);
	
	//Check the V4 has a valid load.  
 	if(V4LoadCheck)
 	{
 		Adapter_Read32(&CA0, BRIDGE, BRG_FPGA_STAT, &Dummy);
 		if(!(Dummy & FPGA_PROG_DONE))
 		{
 			printf("V4 is not loaded, please check PROM contents or load an XSVF file to the V4.\n");
 			Adapter_Close(&CA0);
 			return -1;	
 		}
 	}
	
	Adapter_Read32(&CA0, BRIDGE, BRG_REV_ADR, &Dummy);
	printf("PCI Bridge rev is %x\n", Dummy);
	Adapter_Read32(&CA0, V4, DMA_REV_ADR, &Dummy);  
	printf("V4 DMA Rev (Offset 0x0) =  %x, ", Dummy);
	Adapter_Read32(&CA0, V4, V4_REV_ADR, &Dummy);  
	printf("User Logic Rev (Offset 0x800) =  %x\n", Dummy);
	
	//Display current board temp
	printf("Current temp is %.2f C\n", caGetTemp(&CA0));
	
	
	/*********************Init Block **********************************************************/
		/*
		This section contains examples of configuration steps that should be taken to configure 
		the CA hardware for use.  Since this example does not use the ADCs it is not required,
		but is provided as an example for programs that do
		*/
	
		Adapter_SampleClkSelect(&CA0, &ClkSettings);
		//Code here should reset your DCMs and allow time for them to lock,
		//then check their status
		//Adapter_uSleep(1000000);
		//Soft Reset, self clearing
  		Adapter_Write32(&CA0, V4, V4_CTL_ADR, SOFT_RST); 
		//Wait for DCM to Relock
  		Adapter_uSleep(1000000);     
		
		//Check for DCM Lock
  		Adapter_Read32(&CA0, V4, V4_STAT_ADR, &Dummy); //Clear old status reg
  		Adapter_Read32(&CA0, V4, V4_STAT_ADR, &Dummy);
  		if (Dummy & ADC_DCM_UNLOCKED) {
    			printf("DCM Failed to Lock. STATUS REG = %x\n Exiting Now...\n", Dummy);
    			Adapter_Close(&CA0);
    			return -1;
  		}
				
		/*
		Set M314 VRANGE mode.  This will touch the register that controls the sample
		clk select, so it is done as RMW.
		*/
		//Set ADC range to 2Vpp 
		Adapter_Read32(&CA0, BRIDGE, BRG_HWCONF_ADR, &Dummy);
		Dummy |= ADCA_VRNG1;
		Adapter_Write32(&CA0, BRIDGE, BRG_HWCONF_ADR, Dummy);
		
		// Flush FIFOs
		Adapter_Write32(&CA0, V4, V4_CTL_ADR, ADCAFF_FLUSH);
			
	/***************End Init Blcok***************************************************************/
	
	
	//Allocate DMA space for each channel to trasfer data into.  Once a group has finshed an interrupt will
	//be signaled and the data will be copied away to the large holding buffer. 
		
	printf("Allocating DMA space...");
	//Allocate DMA Memory
	CA0.DMA.DMAChannels = 15;				//Firmware supports 4 DMA channels, indexed to 0
			
	unsigned int i;
	for (i = 0; i < CA0.DMA.DMAChannels+1; i++) {
		int chan = i;
		CA0.DMA.GrpCnt[chan] 	 = 7; 		//8 Groups (indexed to 0)
		CA0.DMA.BlockSizeB[chan] = 1024; 	//8kB Blocks (half a FIFO)
		CA0.DMA.BlockCount[chan] = 15; 		//16 Blocks (index to 0) for GrpSize of 128kB, Linux max
	}		
			
	if(Adapter_DMABufAllocate(&CA0))
	{
		fprintf(stderr, "Failure allocating DMA memory\n");
		Adapter_Close(&CA0);
		return -1;
	}
	else	
		LoadCADMASettings(&CA0);
			
	printf("DMA memory allocation done.\n");
	

	//Setup large storage buffer to move data to after each DMA group is done
	for (i = 0; i < CA0.DMA.DMAChannels+1; i++) {
		int chan = i;
		if (caChMemBuffer_Init(&CA0, &MemBuffer_Ch[chan], chan, GrpsToCapture))
		{
			fprintf(stderr, "Could not allocate storage buffer, will now exit.\n");
			Adapter_Close(&CA0);
			return 0;
		}
	}
	
	//Enable intr generation user logic.  
	// set to interrupt every group for all active channels
	for(i = 0; i < CA0.DMA.DMAChannels+1; i++)
		Adapter_Write32(&CA0, V4, DMA_CH0_GRPSPERINT_ADR+(0x4*i), 0x0); 
	
  	//Enable DMA group done intr after each group transfers, 
	Adapter_Write32(&CA0, V4, DMA_INT_MASK_ADR, 0xFFFF);
			
	//Enable V4 interrupts, 
	Adapter_Write32(&CA0, V4, V4_MASK_ADR, DMA_GRP_DONE | INTR_EN);

	//Allow bridge to create PCI intr
	Adapter_Write32(&CA0, BRIDGE, BRG_INTRMASK_ADR, BRG_INTR_EN); 
	

	/*********************Timer and Pulse Pair Init Block **********************************************************/
		/*
		This section initializes the timers and pulse pair processors.
		*/

  	//Decimation Setup
	DWORD Dec_Factor = decimation*2 - 1;
	Adapter_Write32(&CA0, V4, DEC_REG, Dec_Factor);// Decimation Register
	
	//Pulse Pair Setup
	Adapter_Write32(&CA0, V4, M_REG, gates);     // # of Gates
  	Adapter_Write32(&CA0, V4, N_REG, samples);   // # of Samples
  	Adapter_Write32(&CA0, V4, DPRT_REG, dprt);      // Dual Prt(Off)
	Adapter_Write32(&CA0, V4, IQ_START_IDX, index);     // index of start of IQ capture
	Adapter_Write32(&CA0, V4, IQ_GATE_LEN, length);    // # of Gate of IQ capture

	Adapter_Read32(&CA0, V4, M_REG, &gates);
  	Adapter_Read32(&CA0, V4, N_REG, &samples);
  	Adapter_Read32(&CA0, V4, DPRT_REG, &dprt);
	Adapter_Read32(&CA0, V4, IQ_START_IDX, &index);
	Adapter_Read32(&CA0, V4, IQ_GATE_LEN, &length);
	
  	printf("Gates = %d, Samples = %d, Dual Prt = %d\n", gates, samples, dprt);
	printf("IQ Index = %d, IQ Length = %d\n", index, length);
  	printf("Pulse Width = %d, Decimation Factor = %d\n", gates, decimation);

  	//Reset Timers
	Adapter_Write32(&CA0, V4, MT_DATA, 0x0);    	   // Enable Timer
  	Adapter_Write32(&CA0, V4, MT_WR, WRITE_ON);                // Turn on Write Strobes
	Adapter_Write32(&CA0, V4, MT_ADDR, CONTROL_REG|TIMER0|TIMER1|TIMER2|TIMER3);    // Control Register
  	Adapter_Write32(&CA0, V4, MT_ADDR, DELAY_REG|TIMER0|TIMER1|TIMER2|TIMER3); // Address Timer 0
	Adapter_Write32(&CA0, V4, MT_ADDR, WIDTH_REG|TIMER0|TIMER1|TIMER2|TIMER3); // Address Timer 0
	Adapter_Write32(&CA0, V4, MT_ADDR, PERIOD_REG|TIMER0|TIMER1|TIMER2|TIMER3); // Address Timer 0
	Adapter_Write32(&CA0, V4, MT_ADDR, PRT_REG|TIMER0|TIMER1|TIMER2|TIMER3); // Mult PRT Register Timer 0
	Adapter_Write32(&CA0, V4, MT_WR, WRITE_OFF);                // Turn on Write Strobes
	
	//Gating Timer Setup
	printf("CONTROL REG: %x\n", CONTROL_REG|Timers|TIMER_EN);
	Adapter_Write32(&CA0, V4, MT_ADDR, CONTROL_REG|Timers);    // Control Register
  	Adapter_Write32(&CA0, V4, MT_DATA, TIMER_ON);    	   // Enable Timer
  	Adapter_Write32(&CA0, V4, MT_WR, WRITE_ON);                // Turn on Write Strobes
  
  
  	//Delay Register
	Adapter_Write32(&CA0, V4, MT_ADDR, DELAY_REG|Timers); // Address Timer 0
  	Adapter_Write32(&CA0, V4, MT_DATA, 0);      // Value Timer 0
  
  
  	//Pulse Width Register
	Adapter_Write32(&CA0, V4, MT_ADDR, WIDTH_REG|Timers); // Address Timer 0
  	Adapter_Write32(&CA0, V4, MT_DATA, gates); // Value Timer 0 (Testing Purposes)
	
  	
	//Period Register
  	Adapter_Write32(&CA0, V4, MT_ADDR, PERIOD_REG|Timers); // Address Timer 0
  	if (dprt == 0) {
		//Adapter_Write32(&CA0, V4, MT_DATA, 1000);  // Single PRT @ 1kHz PRF
		Adapter_Write32(&CA0, V4, MT_DATA, 5000);  // For Power Curve Measurements
	}
	else {
		Adapter_Write32(&CA0, V4, MT_DATA, 250);  // Mult PRT 5/4 @ 1kHz and 800Hz PRFs
	}
	
	//Multiple PRT Register
	Adapter_Write32(&CA0, V4, MT_ADDR, PRT_REG|Timers); // Mult PRT Register Timer 0
  	if (dprt == 1) {
		Adapter_Write32(&CA0, V4, MT_DATA, 0x0054);   // Mult PRT Value Timer 0
	}
	else {
  		Adapter_Write32(&CA0, V4, MT_DATA, 0x0000);   // Mult PRT Value Timer 0
	}
	
	//Adapter_Write32(&CA0, V4, 0xA60, 0x0);   // DC I Removal
	//Adapter_Write32(&CA0, V4, 0xA64, 0x0);   // DC Q Removal
  
	printf("ENABLING: %x\n", PRT_REG|Timers|TIMER_EN|ADDR_TRIG);
	Adapter_Write32(&CA0, V4, MT_ADDR, PRT_REG|Timers|TIMER_EN|ADDR_TRIG); // Set Global Enable
  	Adapter_Write32(&CA0, V4, MT_WR, WRITE_OFF);    // Turn off Write Strobes
  
  
 	Adapter_Write32(&CA0, V4, KAISER_ADDR, 0); 	// Start the DDC
		
	/*********************End of Timer and Pulse Pair Init Block **********************************************************/	
	
	
	//Set a bit for each active ch.  Will be cleared by ISR as channels finish
	ChXferDone = 0xFFFF;

	//printf("Press Any Key to Begin Data Capture and Transfer.\n");
	//getchar();
	
	//Allow DMAs to start
	Adapter_Write32(&CA0, V4, V4_CTL_ADR, DMA_EN | ADCA_CAP); 

	//Wait for data collection to finish or exit via a yeyboard hit
	printf("Waiting for data to transfer...\n");
	while(ChXferDone != 0x0)
	{
		
	}
	printf("done!\n");

	
	//Disable intr, data collection and DMAs
	Adapter_Write32(&CA0, V4, V4_CTL_ADR, 0x0);
	Adapter_Write32(&CA0, BRIDGE, BRG_INTRMASK_ADR, BRG_INTR_DIS);
	
	// ********************************************************
	// DMA core status reads
	//*********************************************************
	Adapter_Read32(&CA0, V4, V4_STAT_ADR, &Dummy);
	printf("\tV4 Status reg = %x\n", Dummy);
	Adapter_Read32(&CA0, V4, DMA_INT_STAT_ADR, &Dummy);
	printf("\tDMA Status reg = %x\n", Dummy);
	


	//Write out the large buffer contents now that collect is done
	
	if ((Timers&TIMER0)>>4) {
	  caChMemBuffer_WriteToDisk(&MemBuffer_Ch[0], "ChA_IQ.txt", "txt", "16bit-txt");
	  caChMemBuffer_WriteToDisk(&MemBuffer_Ch[1], "ChA_P.txt", "txt", "32bit_h_txt");
	  caChMemBuffer_WriteToDisk(&MemBuffer_Ch[2], "ChA_A.txt", "txt", "32bit_h_txt");
	  caChMemBuffer_WriteToDisk(&MemBuffer_Ch[3], "ChA_B.txt", "txt", "32bit_h_txt");
	}
	if ((Timers&TIMER1)>>5) {
	  caChMemBuffer_WriteToDisk(&MemBuffer_Ch[4], "ChB_IQ.txt", "txt", "16bit-txt");
	  caChMemBuffer_WriteToDisk(&MemBuffer_Ch[5], "ChB_P.txt", "txt", "32bit_h_txt");
	  caChMemBuffer_WriteToDisk(&MemBuffer_Ch[6], "ChB_A.txt", "txt", "32bit_h_txt");
	  caChMemBuffer_WriteToDisk(&MemBuffer_Ch[7], "ChB_B.txt", "txt", "32bit_h_txt");
	}
	if ((Timers&TIMER2)>>6) {
	  caChMemBuffer_WriteToDisk(&MemBuffer_Ch[8], "ChC_IQ.txt", "txt", "16bit-txt");
	  caChMemBuffer_WriteToDisk(&MemBuffer_Ch[9], "ChC_P.txt", "txt", "32bit_h_txt");
	  caChMemBuffer_WriteToDisk(&MemBuffer_Ch[10], "ChC_A.txt", "txt", "32bit_h_txt");
	  caChMemBuffer_WriteToDisk(&MemBuffer_Ch[11], "ChC_B.txt", "txt", "32bit_h_txt");
	}
	if ((Timers&TIMER3)>>7) {
	  caChMemBuffer_WriteToDisk(&MemBuffer_Ch[12], "ChD_IQ.txt", "txt", "16bit-txt");
	  caChMemBuffer_WriteToDisk(&MemBuffer_Ch[13], "ChD_P.txt", "txt", "32bit_h_txt");
	  caChMemBuffer_WriteToDisk(&MemBuffer_Ch[14], "ChD_A.txt", "txt", "32bit_h_txt");
	  caChMemBuffer_WriteToDisk(&MemBuffer_Ch[15], "ChD_B.txt", "txt", "32bit_h_txt");
	}
	
	//Close the card and exit
	Adapter_Close(&CA0);
	return 0;
}


void Adapter_ISR(s_ChannelAdapter *pCA)
{
	DWORD Status, Mask, DMAStat, DMAMask;
	int GrpsDone;

	//printf("Adapter_ISR: Entered....\n");
	Adapter_Read32(pCA, V4, V4_STAT_ADR, &Status);
	Adapter_Read32(pCA, V4, V4_MASK_ADR, &Mask);
	Adapter_Read32(pCA, V4, DMA_INT_STAT_ADR, &DMAStat);
	Adapter_Read32(pCA, V4, DMA_INT_MASK_ADR, &DMAMask);
	
  	printf("Status = %x\n", Status);
  	printf("DMA Status = %x\n", DMAStat);
	
	if (Mask & Status & DMA_GRP_DONE)
	{
		// Channel A
		if((DMAStat & 0x1) & (ChXferDone & 0x1)) //Ch 0 DMA Done
		{
			printf("Moving ChA IQ\n");
			GrpsDone = caChMemBuffer_Copy(pCA,  &MemBuffer_Ch[0], 0);
			if(GrpsDone > 2)
				fprintf(stderr, "Multiple groups (%d) were ready for Ch 0 , you might be falling behind.\n", GrpsDone);
		}

 		if((DMAStat & 0x2) & (ChXferDone & 0x2)) //Ch 1 DMA Done
 		{
 			printf("Moving ChA P\n");
			GrpsDone = caChMemBuffer_Copy(pCA,  &MemBuffer_Ch[1], 1);
			if(GrpsDone > 2)
				fprintf(stderr, "Multiple groups (%d) were ready for Ch 1 , you might be falling behind.\n", GrpsDone);
		}
		
		if((DMAStat & 0x4) & (ChXferDone & 0x4)) //Ch 2 DMA Done
		{
			printf("Moving ChA A\n");
			GrpsDone = caChMemBuffer_Copy(pCA,  &MemBuffer_Ch[2], 2);
			if(GrpsDone > 2)
				fprintf(stderr, "Multiple groups (%d) were ready for Ch 2 , you might be falling behind.\n", GrpsDone);
		}
		if((DMAStat & 0x8) & (ChXferDone & 0x8)) //Ch 3 DMA Done
		{
			printf("Moving ChA B\n");
			GrpsDone = caChMemBuffer_Copy(pCA,  &MemBuffer_Ch[3], 3);
			if(GrpsDone > 2)
				fprintf(stderr, "Multiple groups (%d) were ready for Ch 3 , you might be falling behind.\n", GrpsDone);
		}
		
		// Channel B
		if((DMAStat & 0x10) & (ChXferDone & 0x10)) //Ch 4 DMA Done
		{
			printf("Moving ChB IQ\n");
			GrpsDone = caChMemBuffer_Copy(pCA,  &MemBuffer_Ch[4], 4);
			if(GrpsDone > 2)
				fprintf(stderr, "Multiple groups (%d) were ready for Ch 4 , you might be falling behind.\n", GrpsDone);
		}

 		if((DMAStat & 0x20) & (ChXferDone & 0x20)) //Ch 5 DMA Done
 		{
 			printf("Moving ChB P\n");
			GrpsDone = caChMemBuffer_Copy(pCA,  &MemBuffer_Ch[5], 5);
			if(GrpsDone > 2)
				fprintf(stderr, "Multiple groups (%d) were ready for Ch 5 , you might be falling behind.\n", GrpsDone);
		}
		
		if((DMAStat & 0x40) & (ChXferDone & 0x40)) //Ch 6 DMA Done
		{
			printf("Moving ChB A\n");
			GrpsDone = caChMemBuffer_Copy(pCA,  &MemBuffer_Ch[6], 6);
			if(GrpsDone > 2)
				fprintf(stderr, "Multiple groups (%d) were ready for Ch 6 , you might be falling behind.\n", GrpsDone);
		}
		if((DMAStat & 0x80) & (ChXferDone & 0x80)) //Ch 7 DMA Done
		{
			printf("Moving ChB B\n");
			GrpsDone = caChMemBuffer_Copy(pCA,  &MemBuffer_Ch[7], 7);
			if(GrpsDone > 2)
				fprintf(stderr, "Multiple groups (%d) were ready for Ch 7 , you might be falling behind.\n", GrpsDone);
		}
		
		// Channel C
		if((DMAStat & 0x100) & (ChXferDone & 0x100)) //Ch 8 DMA Done
		{
			printf("Moving ChC IQ\n");
			GrpsDone = caChMemBuffer_Copy(pCA,  &MemBuffer_Ch[8], 8);
			if(GrpsDone > 2)
				fprintf(stderr, "Multiple groups (%d) were ready for Ch 8 , you might be falling behind.\n", GrpsDone);
		}

 		if((DMAStat & 0x200) & (ChXferDone & 0x200)) //Ch 9 DMA Done
 		{
 			printf("Moving ChC P\n");
			GrpsDone = caChMemBuffer_Copy(pCA,  &MemBuffer_Ch[9], 9);
			if(GrpsDone > 2)
				fprintf(stderr, "Multiple groups (%d) were ready for Ch 9 , you might be falling behind.\n", GrpsDone);
		}
		
		if((DMAStat & 0x400) & (ChXferDone & 0x400)) //Ch 10 DMA Done
		{
			printf("Moving ChC A\n");
			GrpsDone = caChMemBuffer_Copy(pCA,  &MemBuffer_Ch[10], 10);
			if(GrpsDone > 2)
				fprintf(stderr, "Multiple groups (%d) were ready for Ch 10 , you might be falling behind.\n", GrpsDone);
		}
		if((DMAStat & 0x800) & (ChXferDone & 0x800)) //Ch 11 DMA Done
		{
			printf("Moving ChC B\n");
			GrpsDone = caChMemBuffer_Copy(pCA,  &MemBuffer_Ch[11], 11);
			if(GrpsDone > 2)
				fprintf(stderr, "Multiple groups (%d) were ready for Ch 11 , you might be falling behind.\n", GrpsDone);
		}
		
		// Channel D
		if((DMAStat & 0x1000) & (ChXferDone & 0x1000)) //Ch 12 DMA Done
		{
			printf("Moving ChD IQ\n");
			GrpsDone = caChMemBuffer_Copy(pCA,  &MemBuffer_Ch[12], 12);
			if(GrpsDone > 2)
				fprintf(stderr, "Multiple groups (%d) were ready for Ch 12 , you might be falling behind.\n", GrpsDone);
		}

 		if((DMAStat & 0x2000) & (ChXferDone & 0x2000)) //Ch 13 DMA Done
 		{
 			printf("Moving ChD P\n");
			GrpsDone = caChMemBuffer_Copy(pCA,  &MemBuffer_Ch[13], 13);
			if(GrpsDone > 2)
				fprintf(stderr, "Multiple groups (%d) were ready for Ch 13 , you might be falling behind.\n", GrpsDone);
		}
		
		if((DMAStat & 0x4000) & (ChXferDone & 0x4000)) //Ch 14 DMA Done
		{
			printf("Moving ChD A\n");
			GrpsDone = caChMemBuffer_Copy(pCA,  &MemBuffer_Ch[14], 14);
			if(GrpsDone > 2)
				fprintf(stderr, "Multiple groups (%d) were ready for Ch 14 , you might be falling behind.\n", GrpsDone);
		}
		if((DMAStat & 0x8000) & (ChXferDone & 0x8000)) //Ch 15 DMA Done
		{
			printf("Moving ChD B\n");
			GrpsDone = caChMemBuffer_Copy(pCA,  &MemBuffer_Ch[15], 15);
			if(GrpsDone > 2)
				fprintf(stderr, "Multiple groups (%d) were ready for Ch 15 , you might be falling behind.\n", GrpsDone);
		}
		
		if(MemBuffer_Ch[0].GrpsToCapture == MemBuffer_Ch[0].TotalGrpsCaptured)		ChXferDone &= ~0x1;
		if(MemBuffer_Ch[1].GrpsToCapture == MemBuffer_Ch[1].TotalGrpsCaptured)		ChXferDone &= ~0x2;
		if(MemBuffer_Ch[2].GrpsToCapture == MemBuffer_Ch[2].TotalGrpsCaptured)		ChXferDone &= ~0x4;
		if(MemBuffer_Ch[3].GrpsToCapture == MemBuffer_Ch[3].TotalGrpsCaptured)		ChXferDone &= ~0x8;
		
		if(MemBuffer_Ch[4].GrpsToCapture == MemBuffer_Ch[4].TotalGrpsCaptured)		ChXferDone &= ~0x10;
		if(MemBuffer_Ch[5].GrpsToCapture == MemBuffer_Ch[5].TotalGrpsCaptured)		ChXferDone &= ~0x20;
		if(MemBuffer_Ch[6].GrpsToCapture == MemBuffer_Ch[6].TotalGrpsCaptured)		ChXferDone &= ~0x40;
		if(MemBuffer_Ch[7].GrpsToCapture == MemBuffer_Ch[7].TotalGrpsCaptured)		ChXferDone &= ~0x80;
		
		if(MemBuffer_Ch[8].GrpsToCapture == MemBuffer_Ch[8].TotalGrpsCaptured)		ChXferDone &= ~0x100;
		if(MemBuffer_Ch[9].GrpsToCapture == MemBuffer_Ch[9].TotalGrpsCaptured)		ChXferDone &= ~0x200;
		if(MemBuffer_Ch[10].GrpsToCapture == MemBuffer_Ch[10].TotalGrpsCaptured)	ChXferDone &= ~0x400;
		if(MemBuffer_Ch[11].GrpsToCapture == MemBuffer_Ch[11].TotalGrpsCaptured)	ChXferDone &= ~0x800;
		
		if(MemBuffer_Ch[12].GrpsToCapture == MemBuffer_Ch[12].TotalGrpsCaptured)	ChXferDone &= ~0x1000;
		if(MemBuffer_Ch[13].GrpsToCapture == MemBuffer_Ch[13].TotalGrpsCaptured)	ChXferDone &= ~0x2000;
		if(MemBuffer_Ch[14].GrpsToCapture == MemBuffer_Ch[14].TotalGrpsCaptured)	ChXferDone &= ~0x4000;
		if(MemBuffer_Ch[15].GrpsToCapture == MemBuffer_Ch[15].TotalGrpsCaptured)	ChXferDone &= ~0x8000;
		
 		if((Timers&TIMER0)>>4 == 0)  {
 		  ChXferDone &= ~0x1;
		  ChXferDone &= ~0x2;
		  ChXferDone &= ~0x4;
		  ChXferDone &= ~0x8;
		}
		if((Timers&TIMER1)>>5 == 0)  {
		  ChXferDone &= ~0x10;
		  ChXferDone &= ~0x20;
		  ChXferDone &= ~0x40;
		  ChXferDone &= ~0x80;
		}
		if((Timers&TIMER2)>>6 == 0)  {
		  ChXferDone &= ~0x100;
		  ChXferDone &= ~0x200;
		  ChXferDone &= ~0x400;
		  ChXferDone &= ~0x800;
		}
		if((Timers&TIMER3)>>7 == 0)  {
		  ChXferDone &= ~0x1000;
		  ChXferDone &= ~0x2000;
		  ChXferDone &= ~0x4000;
		  ChXferDone &= ~0x8000;
		}
 	}			
	
	if (Mask & Status & ADCA_FF_FULL)
	{
		printf("ChA FIFO FULL!\n");
	}
	if (Mask & Status & ADCB_FF_FULL)
	{
		printf("ChB FIFO FULL!\n");
	}
	if (Mask & Status & ADCC_FF_FULL)
	{
		printf("ChC FIFO FULL!\n");
	}
	if (Mask & Status & ADCD_FF_FULL)
	{
		printf("ChD FIFO FULL!\n");
	}
			
	//Re-enable intr if there are still channels to transfer
	if(ChXferDone != 0x0)
		Adapter_Write32(pCA, BRIDGE, BRG_INTRMASK_ADR, BRG_INTR_EN); 
	
	return;
}

	
