/*****************************************************************************************

	Red Rapids 
	797 N. Grove Rd
	Suite 101
	Richardson, TX 75081
	support@redrapids.com


	ChannelAdapter Local Bus Demo Program

	Desc:	This program shows how to capture data from the ChannelAdapter LocalBus interface.
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
				1.1	- Released against R04 of API
				1.0	- [28 August 2006 ASB (abixby@redrapids.com) & PTJ (jennings@redrapids.com)
					Inital Release

******************************************************************************************/

#define MAJ_REV	1
#define MIN_REV	2
#define SUB_REV	0x20070316


//V4 User Registers
	#define V4_REV_ADR			0x800
	#define V4_CTL_ADR			0x80C	
		#define DMA_GO			0x1


#include <stdio.h>
#include <stdlib.h>


#include "ca_bar0_memmap.h"
#include "ca_lb_memmap.h"
#include "ca_diag_memmap.h"
#include "channeladapter.h"
#include "ca_functions.h"
#include "ca_membuffer.h"


#ifdef WIN32
	#include <conio.h>
	#include <windows.h>
	#include <malloc.h>
#else
	
#endif


UINT32 ChXferDone;		//Bit map for channels than have completed transfer
int Verbose;		


s_ChMemBuffer Ch0MemBuffer;		//Large memory buffers, data from DMA space will be moved here
s_ChMemBuffer Ch1MemBuffer;
s_ChMemBuffer Ch2MemBuffer;
s_ChMemBuffer Ch3MemBuffer;

int main(int argc, char* argv[]) 
{
	s_ChannelAdapter CA0;		/*Handle to ChannelAdapter device*/
	s_ClkSettings ClkSettings;	/*Struct of sample clk setttings*/
	
	UINT32 Dummy;
	int x;
	
	
	//Flags to set from command line
	int ii; 
	int GrpsToCapture;	
	int V4LoadCheck;			/*Set to 1 to force check the V4 is loaded*/
	float SampClkFreqMHz;
	int ADCRange;				/*Sets 1 of 2 Vpp ADC range*/
	UINT32 M316ADCConfig[16];	/*Holds config for M316 ADC*/

	//Set default values
	V4LoadCheck   = 1;
	GrpsToCapture = 10;
	Verbose       = 0;
	ADCRange	  = 0;			/*0 = 2Vpp Mode*/

	printf("Red Rapids\n");
	printf("ChannelAdapter LocalBus Demo Program v %x.%x.%08x\n", MAJ_REV, MIN_REV, SUB_REV);
	printf("Built %s @ %s\n", __DATE__, __TIME__);
	printf("\n%s\n", Adapter_GetAPIVerInfo(&Dummy));

	
	//Init all flags to default values
	Adapter_Zero(&CA0);
	CA0.DevNum = 0;
	strcpy(CA0.Asy, "M320");	//Default to M320 hw, since we don't use the sample clk this is fine for all hw



	ChXferDone = 0;		//A bit will be set high for each active ch.  When the data collect for that ch is
						//done the bit will be set low.
	//Parse the cmd line 
	for(ii=1;ii<argc;ii++)
	{
		if(EQUAL(argv[ii],"-help") || EQUAL(argv[ii],"-HELP") || EQUAL(argv[ii],"-?") || EQUAL(argv[ii],"?"))
		{
			printf("Command Usage\n");
			printf("localbus:\n");
			printf("\t-v <n>\t\tenable verbose output, n=[1:5]\n");
			printf("\t-dev <n> \t\tDevice Number to use\n");
			printf("\t-asy <M314|M316|M320>\tHardware Version\n");
			printf("\t-clk <int|extsamp|extref>\tClock Soruce\n");
			printf("\t-clkfreq <f>\tSample Clk Freq (in MHz, default = 213.0)\n");
			printf("\t-groups <n>\tGroups to capture, each grp = 32k samples (10 by default)\n");
			exit(-1);
		}
		
		if(EQUAL(argv[ii],"-v"))
		{
			Verbose = atoi(argv[ii+1]);
			ii++;
			if(Verbose>=1) printf("\t>Verbose output enabled...\n");
			
		}
		
		if(EQUAL(argv[ii],"-dev"))
		{
			CA0.DevNum = atoi(argv[ii+1]);
			ii++;
			if(Verbose>=1) printf("\t>Will use device %d...\n", CA0.DevNum);
		}
		
		if(EQUAL(argv[ii],"-asy"))
		{
			strcpy(CA0.Asy, argv[ii+1]);
			ii++;
			if(Verbose>=1) printf("\t>Using Asy = %s...\n", CA0.Asy);
		}
		
		
		if(EQUAL(argv[ii],"-clk"))
		{
			if(EQUAL(argv[ii+1], "extref"))
			{
				if(Verbose>=1) printf("\t>Using external 10MHz reference clk...\n");
				ClkSettings.ClkSrc = SYNTH;
			}
			else if (EQUAL(argv[ii+1], "int"))
			{
				if(Verbose>=1) printf("\t>Using on-board clk...\n");
				ClkSettings.ClkSrc = SYNTH;
			}
			else if (EQUAL(argv[ii+1], "extsamp"))
			{
				if(Verbose>=1) printf("\t>Using external sample clk...\n");
				ClkSettings.ClkSrc = EXT;
			}
			ii++;
		}

		if(EQUAL(argv[ii],"-clkfreq"))
		{
			SampClkFreqMHz = atof(argv[ii+1]);
			ii++;
			if(Verbose>=1) printf("\t>ADC Sample Clk set to %f MHz...\n", SampClkFreqMHz);
		}

		if(EQUAL(argv[ii],"-groups"))
		{
			GrpsToCapture = atoi(argv[ii+1]);
			ii++;
			if(Verbose>=1) printf("\t>Groups to capture set to %d \n", GrpsToCapture);
			
		}
			
	}	


	

	//Open hardware link
	if(Adapter_Open(&CA0))
	{
		fprintf(stderr, "Failed to find and/or open card. (%x)\n", CA0.ReturnStatus);
		return -1;
	}
	else
	{
		if(Verbose>=3) printf("Opened ChannelAdapter device %d at [%x:%x.%x]\n", CA0.DevNum, CA0.Bus, CA0.Slot, CA0.Function);
	}
	
	

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
	
	//Read back V4 DMA core rev information and user rev information
	if(Verbose>=2)
	{
		Adapter_Read32(&CA0, BRIDGE, BRG_REV_ADR, &Dummy);
		printf("PCI Bridge rev is %x\n", Dummy);
		Adapter_Read32(&CA0, V4, DMA_REV_ADR, &Dummy);  
	 	printf("V4 DMA Rev (Offset 0x0) =  %x, ", Dummy);
		Adapter_Read32(&CA0, V4, V4_REV_ADR, &Dummy);  
		printf("User Logic Rev (Offset 0x800) =  %x\n", Dummy);
	}

	
	
	/*********************Init Block **********************************************************/
		/*
		This section contains examples of configuration steps that should be taken to configure 
		the CA hardware for use.  Since this example does not use the ADCs it is not required,
		but is provided as an example for programs that do
		*/
	
		Adapter_SampleClkSelect(&CA0, &ClkSettings);
		//Code here should reset your DCMs and allow time for them to lock,
		//then check their status
	
	
		/*
			Each version of the ChannelAdapter requires some specfic setup
		*/
		if(EQUAL(CA0.Asy, "M320"))
		{
			/*
				Set M320 Attenuators and VRANGE.  This will touch the register
				used for the clk settting set by Adapter_SampleClkSelect() above
				so it will be done as RMW.  Not all M320s have attenuators, but 
				setting them has no effect on units that do not have them
			*/
			Adapter_Write32(&CA0, BRIDGE, BRG_M320CHA_ATTN_ADR, 0x1F);  
			Adapter_Write32(&CA0, BRIDGE, BRG_M320CHB_ATTN_ADR, 0x1F);
	
			//Set ADC range to 2Vpp or 1Vpp
			Adapter_Read32(&CA0, BRIDGE, BRG_HWCONF_ADR, &Dummy);  //Get current reg setting 
			
			Dummy &= ~(ADCA_VRNG1|ADCB_VRNG1);  //Force range bits to 0
			if(ADCRange) //1 indicates 1Vpp mode
				Dummy |= (ADCA_VRNG1|ADCB_VRNG1); //1Vpp
			else
				Dummy = Dummy;					  //2Vpp, bits 5 and 4 are low
			Adapter_Write32(&CA0, BRIDGE, BRG_HWCONF_ADR, Dummy);
			
		}
		else if(EQUAL(CA0.Asy, "M314")) 
		{
			/*
				Set M314 VRANGE mode.  This will touch the register that controls the sample
				clk select, so it is done as RMW.
			*/
			
	
			//Set ADC range to 2Vpp or 1Vpp
			Adapter_Read32(&CA0, BRIDGE, BRG_HWCONF_ADR, &Dummy);  //Get current reg setting
			Dummy &= ~(ADCA_VRNG1);  //Force range bits to 0
			if(ADCRange) //1 indicates 1Vpp mode
				Dummy |= (ADCA_VRNG0); //1Vpp
			else
				Dummy |= (ADCA_VRNG1); //1Vpp
			Adapter_Write32(&CA0, BRIDGE, BRG_HWCONF_ADR, Dummy);
		}
		else if(EQUAL(CA0.Asy, "M316"))
		{
			/*
				Set M316 ADC range and load ADC settings.  
			*/
			//Set ADC range to 2Vpp or 1Vpp
			Adapter_Read32(&CA0, BRIDGE, BRG_HWCONF_ADR, &Dummy);  //Get current reg setting
			Dummy &= ~(ADCA_VRNG1);  //Force range bits to 0
			if(ADCRange) //1 indicates 1Vpp mode
				Dummy |= (ADCA_VRNG0); //1Vpp
			else
				Dummy |= (ADCA_VRNG1); //1Vpp
			Adapter_Write32(&CA0, BRIDGE, BRG_HWCONF_ADR, Dummy);
	
			
			M316ADCConfig[0] = 0xB2FF | (0x1<<16);	//Wr to Addr 
			ca_M316ADCConfig(&CA0, M316ADCConfig, 1);
	
		}
	
	
	/***************End Init Blcok***************************************************************/
	
	
	//Allocate DMA space for each channel to trasfer data into.  Once a group has finshed an interrupt will
	//be signaled and the data will be copied away to the large holding buffer. 
	if(1)
	{
		if(Verbose>=4) printf("Allocating DMA space...");
		//Allocate DMA Memory
		CA0.DMA.DMAChannels = 3;				//Firmware supports 4 DMA channels, indexed to 0
			CA0.DMA.GrpCnt[0]      = 7;			//8 Groups (indexed to 0)
			CA0.DMA.BlockSizeB[0]  = 4096*8;    //8kB Blocks (half a FIFO)	
			CA0.DMA.BlockCount[0]  = 0;	    //16 Blocks (index to 0) for GrpSize 128kB, Linux max

			CA0.DMA.GrpCnt[1]      = 7;			//8 Groups (indexed to 0)
			CA0.DMA.BlockSizeB[1]  = 4096*8;    //8kB Blocks (half a FIFO)	
			CA0.DMA.BlockCount[1]  = 0;	    //16 Blocks (index to 0) for GrpSize 128kB, Linux max
			
			CA0.DMA.GrpCnt[2]      = 7;			//8 Groups (indexed to 0)
			CA0.DMA.BlockSizeB[2]  = 4096*8;    //8kB Blocks (half a FIFO)	
			CA0.DMA.BlockCount[2]  = 0;	    //16 Blocks (index to 0) for GrpSize 128kB, Linux max
			
			CA0.DMA.GrpCnt[3]      = 7;			//8 Groups (indexed to 0)
			CA0.DMA.BlockSizeB[3]  = 4096*8;    //8kB Blocks (half a FIFO)	
			CA0.DMA.BlockCount[3]  = 0;	    //16 Blocks (index to 0) for GrpSize 128kB, Linux max
		
			if(Adapter_DMABufAllocate(&CA0))
			{
				fprintf(stderr, "Failure allocating DMA memory\n");
				Adapter_Close(&CA0);
				return -1;
			}
			else
				ca_LoadDMASettings(&CA0);

			if(Verbose>=4) printf("done.\n");
	}

	//Setup large storage buffer to move data to after each DMA group is done
	if(ca_MemBuffer_Init(&CA0, &Ch0MemBuffer, 0, GrpsToCapture))
	{
		fprintf(stderr, "Could not allocate storage buffer, will now exit.\n");	
		Adapter_Close(&CA0);
		return 0;	
	}

	if(ca_MemBuffer_Init(&CA0, &Ch1MemBuffer, 1, GrpsToCapture))
	{
		fprintf(stderr, "Could not allocate storage buffer, will now exit.\n");	
		Adapter_Close(&CA0);
		return 0;	
	}
	
	if(ca_MemBuffer_Init(&CA0, &Ch2MemBuffer, 2, GrpsToCapture))
	{
		fprintf(stderr, "Could not allocate storage buffer, will now exit.\n");	
		Adapter_Close(&CA0);
		return 0;	
	}
	
	if(ca_MemBuffer_Init(&CA0, &Ch3MemBuffer, 3, GrpsToCapture))
	{
		fprintf(stderr, "Could not allocate storage buffer, will now exit.\n");	
		Adapter_Close(&CA0);
		return 0;	
	}
	
	
	//Enable intr generation user logic.  
	// set to interrupt every group for all active channels
	for(x=0; x < CA0.DMA.DMAChannels+1; x++)
		Adapter_Write32(&CA0, V4, DMA_CH0_GRPSPERINT_ADR+(0x4*x), 0x0); 
	
		
	//Enable DMA group done intr after each group transfers, 
	Adapter_Write32(&CA0, V4, DMA_INT_MASK_ADR, 0xF);


	//Allow bridge to create PCI intr
	Adapter_Write32(&CA0, BRIDGE, BRG_INTRMASK_ADR, BRG_INTR_EN); 

	
	

	//Set a bit for each active ch.  Will be cleared by ISR as channels finish
	ChXferDone = 0xF;


	//Allow DMAs to start
	Adapter_Write32(&CA0, V4, V4_CTL_ADR, DMA_GO); 

	//Wait for data collection to finish or exit via a yeyboard hit
	printf("Waiting for data to transfer, press any key to stop waiting...");
	while(ChXferDone != 0x0)
	{
		Adapter_uSleep(500);
		if(_kbhit())
		{
			printf("exited via keypress...\n");
			ChXferDone = 0x0;
		}
	}
	printf("done!\n");

	

	
	//Disable intr, data collection and DMAs
	Adapter_Write32(&CA0, V4, V4_CTL_ADR, 0x0);
	Adapter_Write32(&CA0, BRIDGE, BRG_INTRMASK_ADR, BRG_INTR_DIS);
	
	// ********************************************************
	// DMA core status reads
	//*********************************************************
	if(Verbose>=5)
	{
		Adapter_Read32(&CA0, V4, DMA_REV_ADR, &Dummy);
		printf("\tDMA revision reg = %x\n", Dummy);
		Adapter_Read32(&CA0, V4, DMA_PAGE_ADR, &Dummy);
		printf("\tDMA address page reg = %x\n", Dummy);
		Adapter_Read32(&CA0, V4, DMA_INT_STAT_ADR, &Dummy);
		printf("\tDMA Status reg = %x\n", Dummy);
		Adapter_Read32(&CA0, V4, DMA_INT_MASK_ADR, &Dummy);
		printf("\tDMA Mask reg = %x\n", Dummy);
		Adapter_Read32(&CA0, V4, DMA_CH0_BLKSIZE_ADR, &Dummy);
		printf("\tDMA Ch0 BlockSize reg = %x\n", Dummy);
		Adapter_Read32(&CA0, V4, DMA_CH0_BLKCNT_ADR, &Dummy);
		printf("\tDMA Ch0 BlockCnt reg = %x\n", Dummy);
		Adapter_Read32(&CA0, V4, DMA_CH0_GRPCNT_ADR, &Dummy);
		printf("\tDMA Ch0 GroupCnt reg = %x\n", Dummy);
		Adapter_Read32(&CA0, V4, DMA_CH0_CURBLK_ADR, &Dummy);
		printf("\tDMA Ch0 Current Block reg = %x\n", Dummy);
		Adapter_Read32(&CA0, V4, DMA_CH0_CURGRP_ADR, &Dummy);
		printf("\tDMA Ch0 Current Group reg = %x\n", Dummy);
		Adapter_Read32(&CA0, V4, DMA_CH0_GRPSPERINT_ADR, &Dummy);
		printf("\tDMA Ch0 Groups/Interrupt reg = %x\n", Dummy);
		Adapter_Read32(&CA0, V4, DMA_CURPHYS0_ADR, &Dummy);
		printf("\tDMA Ch0 Current Addr reg = %x\n", Dummy);
		Adapter_Read32(&CA0, V4, DMA_PHYS0_ADR, &Dummy);
		printf("\tDMA Ch0 Group 0 Addr reg = %x\n", Dummy);
	}



	//Write out the large buffer contents now that collect is done
	if(1) ca_MemBuffer_WriteToDisk(&Ch0MemBuffer, "ch0_out.txt", "txt", "lbexample");
	if(1) ca_MemBuffer_WriteToDisk(&Ch1MemBuffer, "ch1_out.txt", "txt", "lbexample");
	if(1) ca_MemBuffer_WriteToDisk(&Ch2MemBuffer, "ch2_out.txt", "txt", "lbexample");
	if(1) ca_MemBuffer_WriteToDisk(&Ch3MemBuffer, "ch3_out.txt", "txt", "lbexample");
	
	
	//Close the card and exit
	Adapter_Close(&CA0);
	return 0;
}


void Adapter_ISR(s_ChannelAdapter *pCA)
{
	UINT32 DMAStat;
	int GrpsDone;

	if(Verbose>=4) printf("Adapter_ISR: Entered....\n");

	//Since the only possible cause of an intr is a DMA group done, process it 
	if(1)
	{
		//Need to read the DMA status register.  Each bit maps to a DMA channel 
		//and indicated the the number for groups between intr for that channel has 
		//happened.  If this intr happens we must be in DMA mode, and since we are only
		//transfering one group per channel, we can mark that channel as done.
		if(Verbose>=4) printf("ISR: DMA group done.\n");
		Adapter_Read32(pCA, V4, DMA_INT_STAT_ADR, &DMAStat);
		if(Verbose>=4) printf("\tDMA Status reg = %x\n", DMAStat);
		
		if(DMAStat & 0x1) //Ch 0 DMA Done
		{
			GrpsDone = ca_MemBuffer_Copy(pCA,  &Ch0MemBuffer, 0);
			if(GrpsDone > 2)
				fprintf(stderr, "Multiple groups (%d) were ready for Ch 0 , you might be falling behind.\n", GrpsDone);
		}

		if(DMAStat & 0x2) //Ch 1 DMA Done
		{
			GrpsDone = ca_MemBuffer_Copy(pCA,  &Ch1MemBuffer, 1);
			if(GrpsDone > 2)
				fprintf(stderr, "Multiple groups (%d) were ready for Ch 1, you might be falling behind.\n", GrpsDone);
		}
		
		if(DMAStat & 0x4) //Ch 2 DMA Done
		{
			GrpsDone = ca_MemBuffer_Copy(pCA,  &Ch2MemBuffer, 2);
			if(GrpsDone > 2)
				fprintf(stderr, "Multiple groups (%d) were ready for Ch 2, you might be falling behind.\n", GrpsDone);
		}
		
		if(DMAStat & 0x8) //Ch 3 DMA Done
		{
			GrpsDone = ca_MemBuffer_Copy(pCA,  &Ch3MemBuffer, 3);
			if(GrpsDone > 2)
				fprintf(stderr, "Multiple groups (%d) were ready for Ch 3, you might be falling behind.\n", GrpsDone);
		}
		
		if(Ch0MemBuffer.GrpsToCapture == Ch0MemBuffer.TotalGrpsCaptured)
			ChXferDone &= ~0x1;

		if(Ch1MemBuffer.GrpsToCapture == Ch1MemBuffer.TotalGrpsCaptured)
			ChXferDone &= ~0x2;
			
		if(Ch2MemBuffer.GrpsToCapture == Ch2MemBuffer.TotalGrpsCaptured)
			ChXferDone &= ~0x4;
			
		if(Ch3MemBuffer.GrpsToCapture == Ch3MemBuffer.TotalGrpsCaptured)
			ChXferDone &= ~0x8;
	}	


	//Re-enable intr if there are still channels to transfer
	if(ChXferDone != 0x0)
		Adapter_Write32(pCA, BRIDGE, BRG_INTRMASK_ADR, BRG_INTR_EN); 
	
	return;
}



	
