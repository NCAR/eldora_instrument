/*****************************************************************************************

  Red Rapids
  797 N. Grove Rd
  Suite 101
  Richardson, TX 75081
  www.redrapids.com | support@redrapids.com


	ChannelAdapter Diagnostic Program

	The program captures ADC data from a ChannelAdapter into the FPGA FIFOs, then can DMA those samples 
	to the host and write them out to a file

	
	
	Rev History		
					1.7.01152008 [ASB] Added 332 support					
					1.6.12122007 [ASB] Added 330, 331, 334, and 336 support
					1.5.12172006 [PTJ] Added M321 support
					1.4.10102006 [PTJ] Added ADC over-range
					1.4.09262006 [PTJ] Added M316 support
					1.3	08302006 [PTJ] Modifed for R03 of API and diag version of firmware
					1.2.08172006 [PTJ] Added VRange support for M314.  Telem ADC will now intr when full
										and will be read via target rds even if the other ADCs use DMAs to 
										move data.
					1.1.08082006 [PTJ] Added external trigger options, changed default to DMA of 
										captured data
					1.1.07122006 [PTJ] Added -asy option to support M320 
					1.0	- [2 June 2006 ASB (abixby@redrapids.com) & PTJ (jennings@redrapids.com)
					Inital Release

******************************************************************************************/



#include <stdio.h>
#include <stdlib.h>

#include "ca_bar0_memmap.h"
#include "ca_diag_memmap.h"
#include "ca_lb_memmap.h"
#include "channeladapter.h"
#include "ca_functions.h"
#include "ca_diag_functions.h"

#ifdef WIN32
	#include <conio.h>
	#include <windows.h>
#else
	
#endif

#define MAJ_REV		1
#define MIN_REV		7	
#define SUB_REV		20080115
#define PI 3.1415926535897932384626433832795

UINT32 ChXferDone;		//Bit map for channels that have completed transfer
int Verbose;			//Non zero value provides many messages to the terminal

int main(int argc, char* argv[]) 
{
	s_ChannelAdapter CA0;
	s_ClkSettings ClkSettings;
	UINT32 Dummy, Dummy2;
	
	
	//Flags to set from command line
	int ii; 
	int ProgramFPGA, SimCntrEn;
	int BinOutEn, TxtOutEn;
	int ChAEn, ChBEn, ChCEn, ChDEn, TelmEn;
	int DMAEn;
	int V4LoadCheck;
	int ExtTriggerEn;
	int M316ADCAutoCal; 
	int AttnTest;
	int ADCA_BDACConfig;
	int ADCB_BDACConfig;
	int DitherEn;
	int RandomizerEn;
	char XSVFFileName[256];
	int ChAAtten, ChBAtten, ADCLowRange;
	UINT32 M316ADCConfig[8];			//Holds config for M316 ADC

	int DACenable;
	int ADCA_BDAC;
	int ADCB_BDAC;
	UINT32 Cmd, Input;
	UINT32 adca_bdac_pos, adca_bdac_neg, adcb_bdac_pos, adcb_bdac_neg;
	int TXAdac_data_select;
	int TXBdac_data_select;
	int TXAdacpoints;
	int TXBdacpoints;
	int	SinCnt = 0;
	float SinVal = 0.0;
	int SinValInt = 0;
	short SinValShort = 0;
	int i = 0;
	int TxChGrpsLeft;
	int TxGrpCounter;
	short *ptr;
	
	printf("Red Rapids\n");
	printf("ChannelAdapter ADC Diagnostic Program ver %d.%d.%08d\n", MAJ_REV, MIN_REV, SUB_REV);
	printf("Built %s @ %s\n", __DATE__, __TIME__);
	printf("\n%s\n", Adapter_GetAPIVerInfo(&Dummy));


	//Init all flags to default values
	Adapter_Zero(&CA0);
	CA0.DevNum = 0;
	ProgramFPGA = 0;
	strcpy(CA0.Asy, "");	//Must specify on the command line
	memset(&ClkSettings, 0, sizeof(s_ClkSettings));
	ClkSettings.ClkSrc  = SYNTH;
	SimCntrEn           = 0;
	BinOutEn	        = 0;
	TxtOutEn            = 1;
	ChAEn				= 0;
	ChBEn               = 0;
	ChCEn               = 0;
	ChDEn               = 0;
	TelmEn				= 0;
	DMAEn				= 1;
	Verbose             = 0;
	M316ADCAutoCal		= 0; 
	ChAAtten            = 31;	//27 matches gain for no-gain/no atten version
	ChBAtten            = 31;
	ADCLowRange         = 0;    //0 for Vrange High, 1 for Vrange low
	V4LoadCheck         = 1; 
	ExtTriggerEn        = 0;
	AttnTest			= 0;
	DitherEn            = 0;
	RandomizerEn        = 0;

	DACenable = 0;
	ADCA_BDACConfig = 0;
	ADCB_BDACConfig = 0;
	ADCA_BDAC = 0;
	ADCB_BDAC = 0;
	adca_bdac_pos = 0;
	adca_bdac_neg = 0;
	adcb_bdac_pos = 0;
	adcb_bdac_neg = 0;
	TXAdac_data_select = 0; //zeros
	TXBdac_data_select = 0; //zeros
	TXAdacpoints = 10;
	TXBdacpoints = 10;
	SinCnt = 0;
	SinVal = 0.0;
	SinValInt = 0;

	ChXferDone = 0;		//A bit will be set high for each active ch.  When the data collect for that ch is
						//done the bit will be set low.

	//Parse the cmd line 
	for(ii=1;ii<argc;ii++)
	{
		if(EQUAL(argv[ii],"-help") || EQUAL(argv[ii],"-HELP") || EQUAL(argv[ii],"-?") || EQUAL(argv[ii],"?"))
		{
			printf("Command Usage\n");
			printf("diagnostic:\n");
			printf("\t-v <n> \t\t\tenable verbose output, n=[1:5]\n");
			printf("\t-dev <n> \t\tDevice Number to use\n");
			printf("\t-asy \t\t\t{M314|M316|M320|M321\n");
			printf("\t     \t\t\t M332|M334|M336|M330|M331}\n");
			printf("\t-clk \t\t\t{int|extref|extsamp}\tSelect Clock source\n");
			printf("\t-sim	\t\tenable sim data counter\n");
			printf("\t-xsvf <name.xsvf>\t{XSVF file to program to FPGA before capture}\n");
			printf("\t-adca \t\t\tenable ADC-A Data Capture\n");
			printf("\t-adcb	\t\tenable ADC-B Data Capture\n");
			printf("\t-noloadcheck \t\tdo not check for V4 CONF_DONE\n");
			printf("\t-exttrig \t\tEnable external trigger\n");

			printf("\n\t== Model 314 / 334 Options ==\n");
			printf("\t-lowrange  \t\tSet ADC Range to Low Range\n");
			printf("\t-adcc	\t\tenable ADC-C Data Capture\n");
			printf("\t-adcd	\t\tenable ADC-D Data Capture\n");
			printf("\t-telad \t\t\tenable Telemety ADC Capture\n");
			printf("\n");

			printf("\n\t== Model 316 / 336 Options ==\n");
			printf("\t-adccal\t\t\tPerform ADC Auto-Calibration and exit\n");
			printf("\n");
			
			printf("\n\t== Model 320 / 330 Options ==\n");
			printf("\t-lowrange  \t\tSet ADC Range to Low Range\n");
			printf("\t-attna <n> \t\tCh A Atten [0-31], 27 default\n");	
			printf("\t-attnb <n> \t\tCh B Atten [0-31], 27 default\n");
			printf("\t-attntest\t\tVerify Attn control from FPGA and exit\n");
			printf("\n");
			
			printf("\n\t== Model 321 / 331 Options ==\n");
			printf("\t-lowrange  \t\tSet ADC Range to Low Range\n");
			printf("\t-dither   \t\tEnables ADC Dither\n");
			printf("\t-randomize\t\tEnables ADC Data Randomizer\n");
			printf("\n");

			printf("\n\t== Model 332 Options ==\n");
			printf("\t-lowrange  \t\tSet ADC Range to Low Range\n");
			printf("\t-dither   \t\tEnables ADC Dither\n");
			printf("\t-randomize\t\tEnables ADC Data Randomizer\n");
			printf("\t-dualdac \t\tEnable the Dual DAC output\n");
			printf("\t-txadacinput \t\tSelect the source for the TXA DAC data {buffer|host|adc}\n");
			printf("\t           \t\t[Default: zeros]\n");
			printf("\t-txbdacinput \t\tSelect the source for the TXB DAC data {buffer|host|adc}\n");
			printf("\t           \t\t[Default: zeros]\n");
			printf("\t-txadacpoints <n>\tSelect the number of samples in the TXAdac data buffer [1-1024]\n");
			printf("\t           \t\t**NOTE: Should be power of 2 if DAC source is host\n");
			printf("\t-txbdacpoints <n>\tSelect the number of samples in the TXBdac data buffer [1-1024]\n");
			printf("\t           \t\t**NOTE: Should be power of 2 if DAC source is host\n");
			printf("\t-adcabdacpos     \tSet Channel A Bias DAC positive bias [0x0 - 3FF]\n");
			printf("\t-adcabdacneg     \tSet Channel A Bias DAC negative bias [0x0 - 3FF]\n");
			printf("\t-adcbbdacpos     \tSet Channel B Bias DAC positive bias [0x0 - 3FF]\n");
			printf("\t-adcbbdacneg     \tSet Channel B Bias DAC negative bias [0x0 - 3FF]\n");
			printf("\n");
		
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
		 
	
		if(EQUAL(argv[ii],"-attna"))
		{
			ChAAtten = atoi(argv[ii+1]);
			ii++;
			if(Verbose>=1) printf("\t>Set ChA Atten to %d\n", ChAAtten);
			
		}

		if(EQUAL(argv[ii],"-attnb"))
		{
			ChBAtten = atoi(argv[ii+1]);
			ii++;
			if(Verbose>=1) printf("\t>Set ChB Atten to %d\n", ChBAtten);
			
		}

		if(EQUAL(argv[ii],"-attntest"))
		{
			AttnTest = 1;
			if(Verbose>=1) printf("\t>Will perform M320 V4 attn ctl test\n");
		}

		if(EQUAL(argv[ii],"-lowrange"))
		{
			ADCLowRange = 1;
			if(Verbose>=1) printf("\t>Set ADC range to low\n");
			
		}
		
		if(EQUAL(argv[ii],"-exttrig"))
		{
			ExtTriggerEn = 1;
			if(Verbose>=1) printf("\t>External trigger enabled\n");
			
		}
		
		if(EQUAL(argv[ii],"-noloadcheck"))
		{
			V4LoadCheck = 0;
			if(Verbose>=1) printf("\t>V4 CONF_DONE check disabled.\n");
			
		}
	
		if(EQUAL(argv[ii],"-adca"))
		{
			if(Verbose>=1) printf("\t>ADC-A Active...\n");
			ChAEn = 1;
		}

		if(EQUAL(argv[ii],"-adcb"))
		{
			if(Verbose>=1) printf("\t>ADC-B Active...\n");
			ChBEn = 1;
		}

		if(EQUAL(argv[ii],"-adcc"))
		{
			if(Verbose>=1) printf("\t>ADC-C Active...\n");
			ChCEn = 1;
		}

		if(EQUAL(argv[ii],"-adcd"))
		{
			if(Verbose>=1) printf("\t>ADC-D Active...\n");
			ChDEn = 1;
		}
		
		if(EQUAL(argv[ii],"-telad"))
		{
			if(Verbose>=1) printf("\t>Telemetry ADC Active...\n");
			TelmEn = 1;
		}
		
		if(EQUAL(argv[ii],"-sim"))
		{
			if(Verbose>=1) printf("\t>Sim Data Counter Enable...\n");
			SimCntrEn = 1;
		}
		
		if(EQUAL(argv[ii],"-nodma"))
		{
			if(Verbose>=1) printf("\t>DMA transfer of collected data diabled...\n");
			DMAEn = 0;
		}

		if(EQUAL(argv[ii],"-adccal"))
		{
			if(Verbose>=1) printf("\t>Will perform M316/M336 ADC Cal...\n");
			M316ADCAutoCal = 1;
		}

		if(EQUAL(argv[ii],"-dither"))
		{
			if(Verbose>=1) printf("\t>Will enable ADC dither...\n");
			DitherEn = 1;
		}

		if(EQUAL(argv[ii],"-randomize"))
		{
			if(Verbose>=1) printf("\t>Will enable ADC randomizer...\n");
			RandomizerEn = 1;
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

		if(EQUAL(argv[ii],"-xsvf"))
		{
			if(Verbose>=1) printf("\tWill program with %s...\n",argv[ii+1]);
			ProgramFPGA = 1;
			strcpy(XSVFFileName, argv[ii+1]);
			ii++;
		}

		if(EQUAL(argv[ii],"-dualdac"))
		{
			if(Verbose>=1) printf("\tDual DAC output enabled...\n");
			DACenable = 1;
		}

		if(EQUAL(argv[ii],"-TXAdacinput"))
		{
			if(Verbose>=1) printf("\tSetting TXA DAC input to %s...\n",argv[ii+1]);
			if(EQUAL(argv[ii+1], "buffer"))
				TXAdac_data_select = 1;
			else if(EQUAL(argv[ii+1], "host"))
				TXAdac_data_select = 2;
			else if(EQUAL(argv[ii+1], "adc"))
				TXAdac_data_select = 3;
			ii++;
		}

		if(EQUAL(argv[ii],"-TXBdacinput"))
		{
			if(Verbose>=1) printf("\tSetting TXB DAC input to %s...\n",argv[ii+1]);
			if(EQUAL(argv[ii+1], "buffer"))
				TXBdac_data_select = 1;
			else if(EQUAL(argv[ii+1], "host"))
				TXBdac_data_select = 2;
			else if(EQUAL(argv[ii+1], "adc"))
				TXBdac_data_select = 3;
			ii++;
		}

		if(EQUAL(argv[ii],"-TXAdacpoints"))
		{
			TXAdacpoints = atoi(argv[ii+1]);
			if(Verbose>=1) printf("\tSetting TXA DAC buffer to %d point sine wave...\n", TXAdacpoints);
			ii++;
		}

		if(EQUAL(argv[ii],"-TXBdacpoints"))
		{
			TXBdacpoints = atoi(argv[ii+1]);
			if(Verbose>=1) printf("\tSetting TXB DAC buffer to %d point sine wave...\n", TXBdacpoints);
			ii++;
		}

		if(EQUAL(argv[ii],"-adcabdacpos"))
		{
			adca_bdac_pos = atoi(argv[ii+1]);
			ADCA_BDACConfig = 1;
			if(Verbose>=1) printf("\tSetting ADCA Bias DAC positive bias to %d...\n", adca_bdac_pos);
			ii++;
		}

		if(EQUAL(argv[ii],"-adcabdacneg"))
		{
			adca_bdac_neg = atoi(argv[ii+1]);
			ADCA_BDACConfig = 1;
			if(Verbose>=1) printf("\tSetting ADCA Bias DAC negative bias to %d...\n", adca_bdac_neg);
			ii++;
		}

		if(EQUAL(argv[ii],"-adcbbdacpos"))
		{
			adcb_bdac_pos = atoi(argv[ii+1]);
			ADCB_BDACConfig = 1;
			if(Verbose>=1) printf("\tSetting ADCB Bias DAC positive bias to %d...\n", adcb_bdac_pos);
			ii++;
		}

		if(EQUAL(argv[ii],"-adcbbdacneg"))
		{
			adcb_bdac_neg = atoi(argv[ii+1]);
			ADCB_BDACConfig = 1;
			if(Verbose>=1) printf("\tSetting ADCB Bias DAC negative bias to %d...\n", adcb_bdac_neg);
			ii++;
		}

	}	



	//Do some checks that all the required info has been set via the cmd line
	//A valid ASY number has been selected
	if(!(EQUAL(CA0.Asy, "M314") | EQUAL(CA0.Asy, "M320") | EQUAL(CA0.Asy, "M316") | EQUAL(CA0.Asy, "M321") | EQUAL(CA0.Asy, "M330") | EQUAL(CA0.Asy, "M331") | EQUAL(CA0.Asy, "M334") | EQUAL(CA0.Asy, "M336") | EQUAL(CA0.Asy, "M332")))
	{
		printf("Must selected M314, M316, M320, M321, \nM332, M330, M331, M334, or M336 with -asy option\n");
		return -1;
	}

	//Check at least one of the ADCs has been enabled, M316/336 Calibration routine has been selected,
	//M320 attenuator test has been selected, or M332 Bias DAC configuration has been selected
	if(!(ChAEn|ChBEn|ChCEn|ChDEn|TelmEn|M316ADCAutoCal|AttnTest|ADCA_BDACConfig|ADCB_BDACConfig))
	{
		printf("Must enable at least one ADC channel, or perform M316/336 calibration, \nM320/330 attenuator test, or M332 Bias DAC configuration\n");
		return -1;
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




	//Load the v4 if needed
	if(ProgramFPGA)
	{
		printf("Loading bitstream %s\n", XSVFFileName);
		if(Adapter_LoadXSVF(&CA0, XSVFFileName, 1))
		{
			printf("V4 and / or PROM failed to load, will now exit...\n");
			Adapter_Close(&CA0);
			return -1;	
		}
	}

	//Check the V4 has a valid load.  Some early versions of hw don't support this
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
	

	//Read back V4 DMA core rev information
	if(Verbose>=3)
	{
		Adapter_Read32(&CA0, BRIDGE, BRG_REV_ADR, &Dummy);
		printf("PCI Bridge rev is %x, ", Dummy);

		Adapter_Read32(&CA0, V4, DMA_REV_ADR, &Dummy);
		printf("V4 DMA Rev Code = %x, ", Dummy);

		Adapter_Read32(&CA0, V4, V4_REV_ADR, &Dummy);  
		printf("User Logic Rev Code =  %x\n", Dummy);
	}
	  
	
	
	//If M316 ADC cal is desired, do this and exit
	if(M316ADCAutoCal)
	{
		if(Verbose>=2) printf("Running M316 ADC Auto-Cal procedure...");
		ca_M316ADCCal(&CA0); 	//Perform a cal on the ADC
		if(Verbose>=2) printf("done.\n");
	}

	if(EQUAL(CA0.Asy, "M316") | EQUAL(CA0.Asy, "M336"))
	{
		M316ADCConfig[0] = 0xB2FF | (0x1<<16);	//Wr to Addr 1 (B2FF = CLK0, B6FF = CLK90)
		M316ADCConfig[1] = 0x007F | (0x2<<16);	//Wr to Addr 2
		M316ADCConfig[2] = 0x007F | (0xA<<16);	//Wr to Addr A
		M316ADCConfig[3] = 0x807F | (0x3<<16);	//Wr to Addr 3
		M316ADCConfig[4] = 0x807F | (0xB<<16);	//Wr to Addr B
		M316ADCConfig[5] = 0x3FFF | (0xD<<16);	//Wr to Addr D
		M316ADCConfig[6] = 0x07FF | (0xE<<16);	//Wr to Addr E
		M316ADCConfig[7] = 0x007F | (0xF<<16);	//Wr to Addr F
		ca_M316ADCConfig(&CA0, M316ADCConfig, 8);

		//Reset IDELAY control module
		Adapter_Write32(&CA0, V4, V4_IDLY_ADR, 0x4|BUS_66); //Set IDELAYCTL rst
		Adapter_Write32(&CA0, V4, V4_IDLY_ADR, BUS_66);		//Clear IDELAYCTL rst
	}

	
	//Configure the sample clk and sample clk DCM
	Adapter_SampleClkSelect(&CA0, &ClkSettings);
	printf("Sleeping 1 sec to allow the synth to lock....\n");
	Adapter_uSleep(1E6);	
	Adapter_Write32(&CA0, V4, V4_CTL_ADR, SOFT_RST); //Soft Reset, self clearing
	Adapter_uSleep(1E6);							 //Sleep to allow DCMs to lock after reset
	Adapter_Read32(&CA0, V4, V4_STAT_ADR, &Dummy);   //Rd once to clear old status
	Adapter_Read32(&CA0, V4, V4_STAT_ADR, &Dummy);
	if(Verbose>=3) printf("\tStatus Register (x810) after DCM reset = %x\n",Dummy);
	if(Verbose>=5) ca_Diag_PrintStatusReg(Dummy);
	if((Dummy & ADC_DCM_UNLOCKED))  //Bit 1 of 0x810 == 1 for DCM unlocked. If unlocked we can do no more
	{
		printf("ADC DCM failed to lock!\n");
		ca_Diag_PrintStatusReg(Dummy);
		Adapter_Close(&CA0);
		return -1;
	}
	

	//If desired perform the M320 V4 Atten ctl test and exit
	if(AttnTest & (EQUAL(CA0.Asy, "M320") | EQUAL(CA0.Asy, "M330")))
	{

		//Enable Atten Ctl from FPGA
		Adapter_Write32(&CA0, BRIDGE, BRG_M320_FPGAATTNCTL_ADR, ALLOW_FPGA_ATTN_CTL);

		//Set the A and B atten through the V4
		Adapter_Write32(&CA0, V4, V4_ATTNCTL_ADR, ATTNCTL_EN|CHA_ATTN_SEL|ChAAtten); 
		Adapter_Write32(&CA0, V4, V4_ATTNCTL_ADR, ATTNCTL_EN|ATTN_SET|CHA_ATTN_SEL|ChAAtten);
		Adapter_Write32(&CA0, V4, V4_ATTNCTL_ADR, ATTNCTL_EN|CHA_ATTN_SEL|ChAAtten);

		Adapter_Write32(&CA0, V4, V4_ATTNCTL_ADR, ATTNCTL_EN|CHB_ATTN_SEL|ChBAtten); 
		Adapter_Write32(&CA0, V4, V4_ATTNCTL_ADR, ATTNCTL_EN|ATTN_SET|CHB_ATTN_SEL|ChBAtten);
		Adapter_Write32(&CA0, V4, V4_ATTNCTL_ADR, ATTNCTL_EN|CHB_ATTN_SEL|ChBAtten);

		Adapter_Read32(&CA0, BRIDGE, BRG_M320CHA_ATTN_ADR, &Dummy);
		printf("Ch A Atten Setting: Exp %x, Got %x\n", ChAAtten, Dummy);

		Adapter_Read32(&CA0, BRIDGE, BRG_M320CHB_ATTN_ADR, &Dummy);
		printf("Ch B Atten Setting: Exp %x, Got %x\n", ChBAtten, Dummy);

		Adapter_Close(&CA0);
		return 0;
	}

	//If desired perform M332 Bias DAC configuration
	if((ADCA_BDACConfig | ADCB_BDACConfig) & (EQUAL(CA0.Asy, "M332")))
	{
		/* Cmd options:
		0x1 - load positive bias value
		0x2 - load negative bias value
		0x9 - load and update positive bias DAC output
		0xa - load and update negative bias DAC output
		0xe - Bias DAC sleep
		*/

		//DACSel = 0 for Channel A Bias DAC or 1 for Channel B Bias DAC
		if(ADCA_BDACConfig)
		{
			//Load and update the positive bias output
			Cmd = 0x9;
			Input = adca_bdac_pos;
			AD_BDAC_SDI_Wr(&CA0, 0x0, Cmd, Input);
			//Load and update the negative bias output
			Cmd = 0xa;
			Input = adca_bdac_neg;
			AD_BDAC_SDI_Wr(&CA0, 0x0, Cmd, Input);
		}
		if(ADCB_BDACConfig)
		{
			//Load and update the positive bias output
			Cmd = 0x9;
			Input = adcb_bdac_pos;
			AD_BDAC_SDI_Wr(&CA0, 0x1, Cmd, Input);
			//Load and update the negative bias output
			Cmd = 0xa;
			Input = adcb_bdac_neg;
			AD_BDAC_SDI_Wr(&CA0, 0x1, Cmd, Input);
		}
	}

	//Set M320/M330 Attenuators (these registers are reserved on M314/M334) and VRANGE.  This will touch bits
	//used for the clk settting set by Adapter_SampleClkSelect() above
	if(EQUAL(CA0.Asy, "M320") | EQUAL(CA0.Asy, "M330"))
	{
		Adapter_Write32(&CA0, BRIDGE, BRG_M320CHA_ATTN_ADR, ChAAtten);  
		Adapter_Write32(&CA0, BRIDGE, BRG_M320CHB_ATTN_ADR, ChBAtten);

		if(Verbose>=3)
		{
			Adapter_Read32(&CA0, BRIDGE, BRG_M320CHA_ATTN_ADR, &Dummy);
			printf("Set Ch A Atten to %x\n", Dummy);
			Adapter_Read32(&CA0, BRIDGE, BRG_M320CHB_ATTN_ADR, &Dummy);
			printf("Set Ch B Atten to %x\n", Dummy);
		}

		
		//Set ADC range to 2Vpp or 1Vpp
		Adapter_Read32(&CA0, BRIDGE, BRG_HWCONF_ADR, &Dummy);  //Get current reg setting 
		
		Dummy &= ~(ADCA_VRNG1|ADCB_VRNG1);  //Force range bits to 0
		if(ADCLowRange) //1 indicates 1Vpp mode
			Dummy |= (ADCA_VRNG1|ADCB_VRNG1); //1Vpp
		else
			Dummy = Dummy;					  //2Vpp, bits 5 and 4 are low
		Adapter_Write32(&CA0, BRIDGE, BRG_HWCONF_ADR, Dummy);
		
	}
	//Set M314/M334 ADC range to 2Vpp or 1Vpp
	else if(EQUAL(CA0.Asy, "M314") | EQUAL(CA0.Asy, "M334")) 
	{
		Adapter_Read32(&CA0, BRIDGE, BRG_HWCONF_ADR, &Dummy);  //Get current reg setting
		Dummy &= ~(ADCA_VRNG1);  //Force range bits to 0
		if(ADCLowRange) //1 indicates 1Vpp mode
			Dummy |= (ADCA_VRNG0); //1Vpp
		else
			Dummy |= (ADCA_VRNG1); //1Vpp
		Adapter_Write32(&CA0, BRIDGE, BRG_HWCONF_ADR, Dummy);
	}

	else if(EQUAL(CA0.Asy, "M316") | EQUAL(CA0.Asy, "M336"))
	{
		/*do nothing*/
	}

	//Set M321/M331 ADC dither, Vrange and randomizer
	else if(EQUAL(CA0.Asy, "M321") | EQUAL(CA0.Asy, "M331"))
	{
		Dummy = M321_ADCA_LORANGE*ADCLowRange | M321_ADCB_LORANGE*ADCLowRange;
		Dummy |= M321_ADCA_RAND_ON*RandomizerEn | M321_ADCB_RAND_ON*RandomizerEn;
		Dummy |= M321_ADCA_DITHER_ON*DitherEn | M321_ADCB_DITHER_ON*DitherEn;
		Adapter_Write32(&CA0, BRIDGE, BRG_HWCONF_ADR, Dummy);
	}

	//Set M332 ADC dither, Vrange and randomizer
	else if(EQUAL(CA0.Asy, "M332"))
	{
		Dummy = M332_ADCA_LORANGE*ADCLowRange | M332_ADCB_LORANGE*ADCLowRange;
		Dummy |= M332_ADCA_RAND_ON*RandomizerEn | M332_ADCB_RAND_ON*RandomizerEn;
		Dummy |= M332_ADCA_DITHER_ON*DitherEn | M332_ADCB_DITHER_ON*DitherEn;
		Adapter_Write32(&CA0, BRIDGE, BRG_HWCONF_ADR, Dummy);
	}

	//Flush all ADC FIFOs now that the ADC clk is valid 
	Adapter_Write32(&CA0, V4, V4_CTL_ADR, TELADCFF_FLUSH|ADCAFF_FLUSH|ADCBFF_FLUSH|ADCCFF_FLUSH|ADCDFF_FLUSH);
	Adapter_Read32(&CA0, V4, V4_STAT_ADR, &Dummy);		//Rd once to clear old status
	Adapter_Read32(&CA0, V4, V4_STAT_ADR, &Dummy);		//Rd again to get current status 

	/*
		If we want to transfer data via DMAs, allocate space and setup the DMA engine.
		The block size also sets the threshold at which the DMA engine will start to request a
		transfer. The block size is set very close to the 2k limit of the FIFOs so that the FIFOs will
		fill and cause the snapshot to stop.  On some assemblies channels 2 & 3 are unused, 
		but space will be allocated for them anyway.
	*/
	if(DMAEn)
	{
		if(Verbose>=4) printf("Allocating DMA space...");
		//Allocate DMA Memory
		CA0.DMA.DMAChannels = 3;				//Firmware supports 4 DMA channels, indexed to 0
			CA0.DMA.GrpCnt[0]      = 1;			//2 Groups, Group count is indexed to 0
			CA0.DMA.BlockSizeB[0]  = 1024*4;	
			CA0.DMA.BlockCount[0]  = 1;			//2 Blocks, BlockCount is indexed to 0
	
			CA0.DMA.GrpCnt[1]      = 1;			
			CA0.DMA.BlockSizeB[1]  = 1024*4;
			CA0.DMA.BlockCount[1]  = 1;
	
			CA0.DMA.GrpCnt[2]      = 1;			
			CA0.DMA.BlockSizeB[2]  = 1024*4;	
			CA0.DMA.BlockCount[2]  = 1;
	
			CA0.DMA.GrpCnt[3]      = 1;			
			CA0.DMA.BlockSizeB[3]  = 1024*4;
			CA0.DMA.BlockCount[3]  = 1;		
				
			if(Adapter_DMABufAllocate(&CA0))
			{
				fprintf(stderr, "Failure allocating DMA memory\n");
				Adapter_Close(&CA0);
				return -1;
			}
			else
			{
				ca_LoadDMASettings(&CA0);
			}

			if(Verbose>=4) printf("done.\n");
	}
	

	//Set the snapshot size to 8k samples for all channels
	if(EQUAL(CA0.Asy, "M316") | EQUAL(CA0.Asy, "M336"))
	{
		Adapter_Write32(&CA0, V4, V4_ADCA_SNAPCNT_ADR, 16384*ChAEn);	
		Adapter_Write32(&CA0, V4, V4_ADCB_SNAPCNT_ADR, 16384*ChBEn);
	}
	else
	{
		Adapter_Write32(&CA0, V4, V4_ADCA_SNAPCNT_ADR, 8192*ChAEn);	//8192 samples
		Adapter_Write32(&CA0, V4, V4_ADCB_SNAPCNT_ADR, 8192*ChBEn); 
		Adapter_Write32(&CA0, V4, V4_ADCC_SNAPCNT_ADR, 8192*ChCEn);
		Adapter_Write32(&CA0, V4, V4_ADCD_SNAPCNT_ADR, 8192*ChDEn);
	}	


	
	//Enable intr generation user logic.  If we're using DMAs, a group done from each active channel
	//will be our clue to stop. If we're going to use target reads, the FIFO for each channel going 
	//full will be our signal to stop
	if(1)
	{
		//Enable DMA group done intr after each group transfers, default is 1 grp per intr.
		Adapter_Write32(&CA0, V4, DMA_INT_MASK_ADR, 0xF);

		//Enable intr for sample clk DCM out of lock or DMA Group done
		Adapter_Read32(&CA0, V4, V4_STAT_ADR, &Dummy);   //Rd to clear old status
		Adapter_Read32(&CA0, V4, V4_STAT_ADR, &Dummy);
		Adapter_Write32(&CA0, V4, V4_MASK_ADR, 
			ADCA_FF_FULL*ChAEn|ADCB_FF_FULL*ChBEn|ADCC_FF_FULL*ChCEn|ADCD_FF_FULL*ChDEn|TELAD_FF_FULL*TelmEn|
			ADCA_PREV_OVR*ChAEn|ADCB_PREV_OVR*ChBEn|ADCC_PREV_OVR*ChCEn|ADCD_PREV_OVR*ChDEn|
			ADCA_FF_OVFLW*ChAEn|ADCB_FF_OVFLW*ChBEn|ADCC_FF_OVFLW*ChCEn|ADCD_FF_OVFLW*ChDEn|
			ADC_DCM_UNLOCKED|DMA_GRP_DONE|INTR_EN);
	}

	//Allow bridge to create PCI intr
	Adapter_Write32(&CA0, BRIDGE, BRG_INTRMASK_ADR, BRG_INTR_EN); 

	//Set a bit for each active ch.  Will be cleared by ISR as channels finish
	ChXferDone = (TelmEn*0x10) | (ChDEn*0x8) | (ChCEn*0x4) | (ChBEn*0x2) | (ChAEn*0x1);

	//Enable ADC data collection.  Form the control word and write it to the control register.
	//All of the control register bits should be static before the capture enables are set
	//so we will write the control register, OR in the capture enables, and write to the control reg again.
	Dummy = (ADCA_SIM*SimCntrEn | ADCB_SIM*SimCntrEn | ADCC_SIM*SimCntrEn | ADCD_SIM*SimCntrEn | DMA_EN*DMAEn);
	if(ExtTriggerEn)
	{
		printf("Using external trigger, make sure one is connected\n");

		//Force all ADCs to wait for the external trigger
		Dummy |= (EXTTRIG_EN);  
		
		//Enable trigger switch
		Adapter_Read32(&CA0, BRIDGE, BRG_HWCONF_ADR, &Dummy2);
		Dummy2 &= ~(0x4); //Clear Ext Trigger bit
		Dummy2 |= M314_EXT_TRIG_ON;  
		Adapter_Write32(&CA0, BRIDGE, BRG_HWCONF_ADR, Dummy2);
	}
	Dummy |= (TELADC_CAP*TelmEn | ADCA_CAP*ChAEn | ADCB_CAP*ChBEn | ADCC_CAP*ChCEn | ADCD_CAP*ChDEn); //Enable FIFOs to collect ADC data
	if(Verbose>=3) printf("Starting data collect with 0x%x\n", Dummy);
	Adapter_Write32(&CA0, V4, V4_CTL_ADR, Dummy);

	//If not using ext trigger set the int trigger
	if(!ExtTriggerEn)
	{
		if(Verbose>=3) printf("Using internal trigger to start data collect\n");
		Adapter_Write32(&CA0, V4, V4_INTTRIG_ADR, 0x0);
		Adapter_Write32(&CA0, V4, V4_INTTRIG_ADR, 0x1);
		Adapter_Write32(&CA0, V4, V4_INTTRIG_ADR, 0x0);
	}

	//Wait for data collection to finish or exit via a keyboard hit
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
	Adapter_Write32(&CA0, BRIDGE, BRG_INTRMASK_ADR, BRG_INTR_DIS);
	Adapter_Write32(&CA0, V4, V4_CTL_ADR, 0x0);
	Adapter_Write32(&CA0, V4, V4_INTTRIG_ADR, 0x0);


	///Write collected data out to disk, either via target reads or directly from the host DMA buffer
	if(!DMAEn)  //Wait long enough to ensure data has been collected, then stop collection
	{
		Adapter_Read32(&CA0, V4, V4_MASK_ADR, &Dummy);
		if(Verbose>=3) printf("\tMask Register (x%x) after collect = \t%x\n",V4_MASK_ADR, Dummy);
		Adapter_Read32(&CA0, V4, V4_STAT_ADR, &Dummy);
		if(Verbose>=3) printf("\tStatus Register (x810) after collect = \t%x\n",Dummy);
		if(Verbose>=5) ca_Diag_PrintStatusReg(Dummy);

		//do target writes to the hardware FIFOs to get the captured data.  Since the telemety ADC can't be
		//DMAed, always do target access if it was used
		if(EQUAL(CA0.Asy, "M316") | EQUAL(CA0.Asy, "M336"))
		{
			/*
			if(ChAEn) ADCFifoTargetWriteToDisk(&CA0, V4_ADCA_LOW_ADR, 2048, "8bit-ob-txt", "chAout.txt");
			if(ChBEn) ADCFifoTargetWriteToDisk(&CA0, V4_ADCB_LOW_ADR, 2048, "8bit-ob-txt", "chBout.txt");
			*/
		}
		else
		{
			/*
			if(ChAEn) ADCFifoTargetWriteToDisk(&CA0, V4_ADCA_LOW_ADR, 2048, "16bit-txt", "chAout.txt");
			if(ChBEn) ADCFifoTargetWriteToDisk(&CA0, V4_ADCB_LOW_ADR, 2048, "16bit-txt", "chBout.txt");
			if(ChCEn) ADCFifoTargetWriteToDisk(&CA0, V4_ADCC_LOW_ADR, 2048, "16bit-txt", "chCout.txt");
			if(ChDEn) ADCFifoTargetWriteToDisk(&CA0, V4_ADCD_LOW_ADR, 2048, "16bit-txt", "chDout.txt");
			if(TelmEn) ADCFifoTargetWriteToDisk(&CA0,V4_TELAD_LOW_ADR, 2048, "12bit-ob-txt", "telout.txt");
			*/
		}
		
		
	}
	else  //Samples were transfered to the host via DMA already
	{
		Adapter_Read32(&CA0, V4, V4_MASK_ADR, &Dummy);
		if(Verbose>=3) printf("\tMask Register (x%x) after collect = \t%x\n",V4_MASK_ADR, Dummy);
		Adapter_Read32(&CA0, V4, V4_STAT_ADR, &Dummy);
		if(Verbose>=3) printf("\tStatus Register (x810) after collect = %x\n",Dummy);
		if(Verbose>=5) ca_Diag_PrintStatusReg(Dummy);

	
		//If DMAs were used, write data out from the DMA buffer on the host for any active channel
		if(EQUAL(CA0.Asy, "M316") | EQUAL(CA0.Asy, "M336"))
		{
			if(ChAEn)
				ca_PrintDMAChannel(&CA0, "chAout.txt", 0, "8bit-ob-txt");	
			if(ChBEn)
				ca_PrintDMAChannel(&CA0, "chBout.txt", 1, "8bit-ob-txt");	
		}
		else
		{
			
			if(ChAEn)
				ca_PrintDMAChannel(&CA0, "chAout.txt", 0, "16bit-txt");
			if(ChBEn)
				ca_PrintDMAChannel(&CA0, "chBout.txt", 1, "16bit-txt");	
			if(ChCEn)
				ca_PrintDMAChannel(&CA0, "chCout.txt", 2, "16bit-txt");	
			if(ChDEn)
				ca_PrintDMAChannel(&CA0, "chDout.txt", 3, "16bit-txt");
		}
		
	
		//Since the telemety ADC can't be DMAed, always do target access if it was used
		//if(TelmEn) ADCFifoTargetWriteToDisk(&CA0, V4_TELAD_LOW_ADR, 2048, "12bit-ob-txt", "telout.txt");

	//For M332
	if (DACenable)
	{
		// release the DAC reset
		Adapter_Write32(&CA0, BRIDGE, BRG_M332_DAC_CTL_ADR, 0x1e);
		Adapter_Write32(&CA0, V4, V4_TXADAC_BUFFER_CNT_SELN, TXAdacpoints-1);
		Adapter_Write32(&CA0, V4, V4_TXBDAC_BUFFER_CNT_SELN, TXBdacpoints-1);
		Adapter_Write32(&CA0, V4, V4_TXADAC_OUTPUT_REG_SELN, TXAdac_data_select);
		Adapter_Write32(&CA0, V4, V4_TXBDAC_OUTPUT_REG_SELN, TXBdac_data_select);

		//LoadConfig(&CA0, "DAC_cfg.txt");

		Adapter_Read32(&CA0, V4, V4_CTL_ADR, &Dummy);
		Adapter_Write32(&CA0, V4, V4_CTL_ADR, Dummy | 0x8000); //soft reset
		Adapter_uSleep((UINT32)1E6);
	
		//If the DAC source is the test buffer, fill the TXAdac test buffer with sin wave data
		if (TXAdac_data_select == 0x1)
		{
			for (i = 0; i<TXAdacpoints; i++)
			{
				SinCnt = i;
				SinVal = sin((((float)SinCnt))*2*PI/(float)TXAdacpoints)*32767.0;

				SinValInt = ((UINT32)SinVal)&0xffff;
		
				Adapter_Write32(&CA0, V4, V4_TXADAC_BUFFER_STARTADDR + 0x4*i, SinValInt);
				Adapter_Read32(&CA0, V4, V4_TXADAC_BUFFER_STARTADDR + 0x4*i, &Dummy);
				//printf("0x%x, 0x%x\n", SinValInt, Dummy);
			}
		}
		//If the DAC source is the test buffer, fill the TXBdac test buffer with sin wave data
		if (TXBdac_data_select == 0x1)
		{
			for (i = 0; i<TXBdacpoints; i++)
			{
				SinCnt = i;
				SinVal = sin((((float)SinCnt))*2*PI/(float)TXBdacpoints)*32767.0;

				SinValInt = ((UINT32)SinVal)&0xffff;
		
				Adapter_Write32(&CA0, V4, V4_TXBDAC_BUFFER_STARTADDR + 0x4*i, SinValInt);
				Adapter_Read32(&CA0, V4, V4_TXBDAC_BUFFER_STARTADDR + 0x4*i, &Dummy);
				//printf("0x%x, 0x%x\n", SinValInt, Dummy);
			}
		}

		//A pulls data from channel 2's DMA buffer if host memory is chosen as the DAC source
		//Load channel 2's DMA buffer with a sin wave
		if(TXAdac_data_select == 0x2)
		{
			TxChGrpsLeft = CA0.DMA.GrpCnt[2]+1;
			TxGrpCounter = 0;
			SinCnt = 0;
			while(TxChGrpsLeft)
			{
				ptr = CA0.DMA.dVirtDMAAdr[1024+TxGrpCounter];
				//printf("\tPointer is: 0x%x\n", CA0.DMA.dVirtDMAAdr[1024+TxGrpCounter]);

				for (i = 0; i<(CA0.DMA.BlockSizeB[2] * (CA0.DMA.BlockCount[2]+1))/2; i+=1)
				{
					SinVal = sin((((float)SinCnt))*2*PI/(float)TXAdacpoints)*32767.0;
					SinValShort = ((short)SinVal)&0xffff;
					//load memory so that first data out is on MSB of memory word
					ptr[i+3-2*(i%4)] = SinValShort; 
					SinCnt += 1;
				}

				TxChGrpsLeft--;
				TxGrpCounter++;
			}

			//set control bits to mask off DMAs
			Adapter_Read32(&CA0, V4, V4_CTL_ADR, &Dummy);  //Get current reg setting 
			Dummy |= (0x10040); //Mask bit 6 for channel A
			Adapter_Write32(&CA0, V4, V4_CTL_ADR, Dummy); //Write control reg

		}

		//B pulls data from channel 3's DMA buffer if host memory is chosen as the DAC source
		//Load channel 3's DMA buffer with a sin wave
		if(TXBdac_data_select == 0x2)
		{
			TxChGrpsLeft = CA0.DMA.GrpCnt[3]+1;
			TxGrpCounter = 0;
			SinCnt = 0;
			while(TxChGrpsLeft)
			{
				ptr = CA0.DMA.dVirtDMAAdr[1536+TxGrpCounter];
				//printf("\tPointer is: 0x%x\n", CA0.DMA.dVirtDMAAdr[1536+TxGrpCounter]);

				for (i = 0; i<(CA0.DMA.BlockSizeB[3] * (CA0.DMA.BlockCount[3]+1))/2; i+=1)
				{
					SinVal = sin((((float)SinCnt))*2*PI/(float)TXBdacpoints)*32767.0;
					SinValShort = ((short)SinVal)&0xffff;
					//load memory so that first data out is on MSB of memory word
					ptr[i+3-2*(i%4)] = SinValShort;
					SinCnt += 1;
				}

				TxChGrpsLeft--;
				TxGrpCounter++;
			}	
			//set control bits to mask off DMAs
			Adapter_Read32(&CA0, V4, V4_CTL_ADR, &Dummy);  //Get current reg setting 
			Dummy |= (0x10020); //Mask bit 5 for channel B
			Adapter_Write32(&CA0, V4, V4_CTL_ADR, Dummy); //Write control reg

		}

		//enable DAC output
		Adapter_Write32(&CA0, V4, V4_TXADAC_OUTPUT_EN, 0x1);
		Adapter_Write32(&CA0, V4, V4_TXBDAC_OUTPUT_EN, 0x1);

		printf("Enabling DAC output press any key to exit...\n");

		while(!(_kbhit()))
		{
			Adapter_uSleep(500);
		}
		
		//disable DAC output
		Adapter_Write32(&CA0, V4, V4_TXADAC_OUTPUT_REG_SELN, 0x0);
		Adapter_Write32(&CA0, V4, V4_TXBDAC_OUTPUT_REG_SELN, 0x0);
		Adapter_Write32(&CA0, V4, V4_TXADAC_OUTPUT_EN, 0x0);
		Adapter_Write32(&CA0, V4, V4_TXBDAC_OUTPUT_EN, 0x0);
	}
		
	}

	//Close the card and exit
	Adapter_Close(&CA0);
	return 0;
}


void Adapter_ISR(s_ChannelAdapter *pCA)
{
	UINT32 Stat, Mask, DMAStat, NewMask;
	UINT32 Dummy;

	if(Verbose>=4) printf("Adapter_ISR: Entered for device %d....\n", pCA->DevNum);

	//Go read status reg from the V4 to determine the cause of the intr.  All bits can 
	//be set regardless of the mask bits so we'll read that too.
	Adapter_Read32(pCA, V4, V4_STAT_ADR, &Stat);
	Adapter_Read32(pCA, V4, V4_MASK_ADR, &Mask);
	NewMask = Mask;

	if(Verbose>=4) printf("Adapter_ISR: Status = %x, Mask = %x\n", Stat, Mask);


	if((Mask&Stat) & DMA_GRP_DONE)
	{
		//Need to read the DMA status register.  Each bit maps to a DMA channel 
		//and indicated the the number for groups between intr for that channel has 
		//happened.  If this intr happens we must be in DMA mode, and since we are only
		//transfering one group per channel, we can mark that channel as done.
		if(Verbose>=4) printf("ISR: DMA group done.\n");
		Adapter_Read32(pCA, V4, DMA_INT_STAT_ADR, &DMAStat);
		if(Verbose>=4) printf("\tDMA Status reg = %x\n", DMAStat);
		ChXferDone &= ~DMAStat;
	}	

	//If one of these intr happens it's becasue we're in target rd mode and the snapshot is 
	//done, so mark the ch as done
	if((Mask&Stat) & ADCA_FF_FULL)
	{
		if(Verbose>=3) printf("ISR: ADC-A FIFO full.\n");
		NewMask &= ~ADCA_FF_FULL;
	}	

	if((Mask&Stat) & ADCB_FF_FULL)
	{
		if(Verbose>=4) printf("ISR: ADC-B FIFO full.\n");
		NewMask &= ~ADCB_FF_FULL;
	}	

	if((Mask&Stat) & ADCC_FF_FULL)
	{
		if(Verbose>=4) printf("ISR: ADC-C FIFO full.\n");
		NewMask &= ~ADCC_FF_FULL;
	}	

	if((Mask&Stat) & ADCD_FF_FULL)
	{
		if(Verbose>=4) printf("ISR: ADC-D FIFO full.\n");
		NewMask &= ~ADCD_FF_FULL;
	}	

	if((Mask&Stat) & TELAD_FF_FULL)
	{
		if(Verbose>=4) printf("ISR: Telem. FIFO full.\n");
		NewMask &= ~TELAD_FF_FULL;
		ChXferDone &= ~(0x10);
	}
	
	// FIFO Overflows
	if((Mask&Stat) & ADCA_FF_OVFLW)
	{
		if(Verbose>=4) printf("ISR: ADC-A FIFO Overflow.\n");
		NewMask &= ~ADCA_FF_OVFLW;
	}	

	if((Mask&Stat) & ADCB_FF_OVFLW)
	{
		if(Verbose>=4) printf("ISR: ADC-B FIFO Overflow.\n");
		NewMask &= ~ADCB_FF_OVFLW;
	}	

	if((Mask&Stat) & ADCC_FF_OVFLW)
	{
		if(Verbose>=4) printf("ISR: ADC-C FIFO Overflow.\n");
		NewMask &= ~ADCC_FF_OVFLW;
	}	

	if((Mask&Stat) & ADCD_FF_OVFLW)
	{
		if(Verbose>=4) printf("ISR: ADC-D FIFO Overflow.\n");
		NewMask &= ~ADCA_FF_OVFLW;
	}	

	//ADC Over-range
	if((Mask&Stat) & ADCA_PREV_OVR)
	{
		printf("ADC-A has over-ranged!\n");
		NewMask &= ~ADCA_PREV_OVR;
	}

	if((Mask&Stat) & ADCB_PREV_OVR)
	{
		printf("ADC-B has over-ranged!\n");
		NewMask &= ~ADCB_PREV_OVR;
	}

	if((Mask&Stat) & ADCC_PREV_OVR)
	{
		printf("ADC-C has over-ranged!\n");
		NewMask &= ~ADCC_PREV_OVR;
	}

	if((Mask&Stat) & ADCD_PREV_OVR)
	{
		printf("ADC-D has over-ranged!\n");
		NewMask &= ~ADCD_PREV_OVR;
	}

	
	if((Mask&Stat) & ADC_DCM_UNLOCKED)
	{
		//Since the DCM out of lock is not likely to clear without user intervention,
		//we will stop allowing it to cause interrupts.
		printf("ISR: ADC DCM out of lock.\n");
		NewMask &= ~ADC_DCM_UNLOCKED;
	}


	//Re-enable intr if there are still channels to transfer
	if(ChXferDone != 0x0)
	{
		//Check to see if the mask has been changed, if so update it
		if(NewMask != Mask)
			Adapter_Write32(pCA, V4, V4_MASK_ADR, NewMask);

		Adapter_Write32(pCA, BRIDGE, BRG_INTRMASK_ADR, BRG_INTR_EN); 
	}
	
	

	return;
}