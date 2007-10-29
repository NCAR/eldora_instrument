/*******************************************************************************************************

	Red Rapids 
	797 N. Grove Rd.
	Suite 101
	Richardson, TX 75081
	www.redrapids.com | support@redrapids.com

	Filename: main.c
	Product:  ChannelAdaptor

	Description:	This example shows how to load a Xilinx XSVF file to the the ChannelAdapter 
					to program the V4 FPGA and / or the configuration PROM.
					
	Rev History:	
					1.4.20070316	Linked to R05.0316 of API
					1.4.11202006	Linked to R05 of API.
					1.3.09282006	Linked to R04 of API. Bug fix for Linux Mem leak
					1.2.08292006	Linked to R03 of API. Bug fix for some 
									PROMs that wouldn't program.
					1.1.08252006	Linked to R02 of API (PTJ)
					1.1.08082006	Linked to R01 of API (PTJ)
					1.1 (20060610)  Added -prom options (PTJ)	
					1.0	(20060322)	Intial release [PTJ & ATS] 
					


********************************************************************************************************/
#ifdef WIN32
  #include <conio.h>
  #include <stdlib.h>
#endif

#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include "channeladapter.h"
#include "ca_bar0_memmap.h"
#include "ca_functions.h"


#define MAJ_REV		0x1
#define MIN_REV		0x4
#define SUB_REV		0x20070316

int main(int argc, char* argv[])
{
	s_ChannelAdapter CA0;
	UINT32 Dummy;
	char Char = '0';
	char XSVFFileName[256];
	int FileFlag;
	int ii;
	int FPGAMode;
	int RevCheck;

	printf("Red Rapids\n");
	printf("ChannelAdapter Programming Utilty %x.%x.%08x\n", MAJ_REV, MIN_REV, SUB_REV);
	printf("Built %s @ %s\nUsing:\n", __DATE__, __TIME__);
	printf("\n%s\n", Adapter_GetAPIVerInfo(&Dummy));


	Adapter_Zero(&CA0);
	CA0.DevNum = 0;
	strcpy(CA0.Asy, "M314");	//Default to M314 hw, no clks are used so this can be set to M320 or M314 regarless of HW
	
	FileFlag = 0;
	RevCheck = 0;	//Only print revs and exit
	FPGAMode = 1;   //The LoadXSVF route has a special mode if a PROM is being programmed
 
	for(ii=1;ii<argc;ii++)
	{
		if(EQUAL(argv[ii],"-help") || EQUAL(argv[ii],"-HELP") || EQUAL(argv[ii],"-?") || EQUAL(argv[ii],"?"))
		{
			printf("Command Useage\n");
			printf("\t-dev 	    <0> (ChannelAdapter to program)\n");
			printf("\t-xsvf 	<filename.bit> (XSVF file to load)\n");
			printf("\t-revcheck print revison information\n");
			printf("\t-prom     the target XSVF will program the PROM\n");
			return -1;
		}
		
		if(EQUAL(argv[ii],"-xsvf"))
		{
			strcpy(XSVFFileName, argv[ii+1]);  /* set config file to filename specified at commandline */
			FileFlag = 1;
			ii++;
		}

		if(EQUAL(argv[ii],"-revcheck"))
		{
			RevCheck = 1;
		}

		if(EQUAL(argv[ii],"-prom"))
		{
			FPGAMode = 0;
		}

		if(EQUAL(argv[ii],"-dev"))
		{
			CA0.DevNum = atoi(argv[ii+1]);
			printf("Will program device %d\n", CA0.DevNum);
			ii++;
		}

	}	

	if(!(FileFlag | RevCheck) )
	{
		printf("Must specify either -xsvf and / or -revcheck\n");
		return -1;	
	}
	
	
	
	if(Adapter_Open(&CA0))
	{
		fprintf(stderr, "Error opening ChannelAdapter device\n");
		return -1;
	}
	else
		printf("Opened device %d found at [%x:%x.%x] with a PCI bridge rev of %x\n", CA0.DevNum, CA0.Bus, 
			CA0.Slot, CA0.Function, CA0.RevCode);

		
	

	if(FileFlag)
	{
		printf("Loading XSVF file %s ...\n", XSVFFileName);
		if(Adapter_LoadXSVF(&CA0, XSVFFileName, FPGAMode))
		{
			printf("XSVF load failed.\n");
			Adapter_Close(&CA0);
			return -1;
		}
		printf("done\n");
	}
	

	//Check for V4 load OK
	Adapter_Read32(&CA0, BRIDGE, 0x104, &Dummy); 
	if(!Dummy&0x1)
	{
		printf("V4 failed to load, done bit was %x\n", Dummy);
		Adapter_Close(&CA0);
		return 0;
	}

	if(RevCheck)
	{
		//Read back V4 rev information for sanaity
		Adapter_Read32(&CA0, V4, 0x0, &Dummy);  
		printf("V4 DMA Rev Code (Offset 0x0) =  %x\n", Dummy);

		//Read back V4 rev information for sanaity
		Adapter_Read32(&CA0, V4, 0x800, &Dummy);  
		printf("V4 User Logic Rev Code (Offset 0x800) =  %x\n", Dummy);
	}
	
	Adapter_Close(&CA0);
	return 0;
}

//We don't user ISRs for this app, just a stub
void Adapter_ISR(s_ChannelAdapter *pCA)
{

}

