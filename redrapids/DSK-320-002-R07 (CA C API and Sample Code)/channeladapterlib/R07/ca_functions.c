#include <string.h>
#include <memory.h> 
#include <stdlib.h>

#ifdef WIN32
	#include <windows.h>
#endif


#ifdef WIN32
	#define STR_EQUAL(str1, str2)(_stricmp(str1,str2) == 0) //For cmd line parsing
#else
	#define STR_EQUAL(str1, str2)(strcasecmp(str1,str2) == 0) //For cmd line parsing
#endif


#include "ca_functions.h"
#include "ca_bar0_memmap.h"
#include "ca_lb_memmap.h"


//Local Prototypes, not for external use
void M316ADCSerialWr(s_ChannelAdapter *pCA, UINT32 Data);




/*
	int EQUAL(char * str1, char * str2)
	
	Does case insensitive comparions of two strings.  Returns 1 if strings are the same
*/
int EQUAL(
		  char * str1, 
		  char * str2
		  )
{
	return STR_EQUAL(str1, str2);
}

/*
	void PrintDMASettings(s_ChannelAdapter *pCA)
	
	Prints the Virt and Phys addr for all groups in all DMA channels. They must have been allocated before this 
	function can be used.
*/
void ca_PrintDMASettings(
	s_ChannelAdapter *pCA
	)
{
	UINT32 DMAChLeft, DMAChCntr, GrpsLeft,GrpCntr;
	UINT32 DMAAdrOffset;		

	DMAAdrOffset = 2048 / (pCA->DMA.DMAChannels + 1);

	DMAChLeft = 32;
	DMAChCntr = 0;
	

	while(DMAChLeft)
	{
	
		if(pCA->DMA.BlockSizeB[DMAChCntr] != 0)
		{
			GrpsLeft = pCA->DMA.GrpCnt[DMAChCntr] + 1;
			GrpCntr  = 0;

			printf("DMA Channel %d\n===========================\n", DMAChCntr);

			
			while(GrpsLeft)
			{
				
				printf("\tGroup %d [%x:%x] Phys = %x, Virt = %x\n", 
					GrpCntr,
					((DMAChCntr*DMAAdrOffset)+GrpCntr / 64), //page
					((DMAChCntr*DMAAdrOffset)+GrpCntr % 64), //Offset
					pCA->DMA.dPhysDMAAdr[(DMAChCntr*DMAAdrOffset)+GrpCntr], 
					pCA->DMA.dVirtDMAAdr[(DMAChCntr*DMAAdrOffset)+GrpCntr]);
				

				GrpsLeft--;
				GrpCntr++;
			}
		}
		DMAChLeft--;
		DMAChCntr++;
	}
	return;
}




/*
	void LoadCADMASettings(s_ChannelAdapter *pCA)
	
	Once DMA buffers have been allocated by the API, this function will write 
	the Block Size, Block Count, Group Count and Phys Addr to the Channel Adapter for 
	all active DMA channels.
*/

void ca_LoadDMASettings(
	s_ChannelAdapter *pCA
	)
{

	UINT32 DMAChLeft, DMAChCntr, GrpsLeft,GrpCntr;
	UINT32 DMAAdrPerCh;		
	UINT32 Page, Offset;
	UINT32 Adr;
	

	DMAAdrPerCh = 2048 / (pCA->DMA.DMAChannels + 1);

	DMAChLeft = 32;
	DMAChCntr = 0;
	
	//Set QL Fifo flags
	Adapter_Write32(pCA, BRIDGE, 0x68, 0x0f0f0f0f);
	Adapter_Write32(pCA, BRIDGE, 0x6C, 0x0f0f0f0f);


	while(DMAChLeft)
	{
	
		if(pCA->DMA.BlockSizeB[DMAChCntr] != 0)
		{
			GrpsLeft = pCA->DMA.GrpCnt[DMAChCntr] + 1;
			GrpCntr  = 0;

			Adapter_Write32(pCA, V4, DMA_CURPHYS0_ADR + (DMAChCntr*4), 0x1); //Unlock curr adr 
			Adapter_Write32(pCA, V4, DMA_CH0_BLKSIZE_ADR + (DMAChCntr*4), pCA->DMA.BlockSizeB[DMAChCntr]); //Load Block Size	 
			Adapter_Write32(pCA, V4, DMA_CH0_BLKCNT_ADR + (DMAChCntr*4), pCA->DMA.BlockCount[DMAChCntr]); //Load Block Count
			Adapter_Write32(pCA, V4, DMA_CH0_GRPCNT_ADR + (DMAChCntr*4), pCA->DMA.GrpCnt[DMAChCntr]);  //Load Group Count 

			//Load physical adr to the hardware
			while(GrpsLeft)
			{
				Page   = (((DMAChCntr*DMAAdrPerCh)+GrpCntr) / 64);
				Offset = (((DMAChCntr*DMAAdrPerCh)+GrpCntr) % 64);
				Adr    = pCA->DMA.dPhysDMAAdr[(DMAChCntr*DMAAdrPerCh)+GrpCntr];
				


				//printf("\tLoading Page = %x, Offset = %x, Adr  = %x\n", Page, Offset*4, Adr);
				Adapter_Write32(pCA, V4, DMA_PAGE_ADR, Page); //Set current page 
				Adapter_Write32(pCA, V4, DMA_PHYS0_ADR + (Offset*4), Adr);	
		
				GrpsLeft--;
				GrpCntr++;
			}

			//Adapter_Write32(pCA, V4, DMA_CH0_GRPSPERINT_ADR + (DMAChCntr*4), 0); //Set 1 grp per intr
		}

		DMAChLeft--;
		DMAChCntr++;
	}
	return;

}


/*
	void PrintDMAChannel(s_ChannelAdapter *pCA, char * fName, int Ch, char * Mode)

	Desc:	Writes the contents of a the DMA channel to a file.  

	Allowed Mode values:
		"8bit-txt"		| 8 bit offset binary numbers as ASCII text
		"16bit-txt"		| 16 bit 2s numbers as ASCII text
		"32bit_h-txt"	| 32 bit hex values as ASCII text
*/
void ca_PrintDMAChannel(
	s_ChannelAdapter *pCA, 
	char * fName, 
	int Ch, 
	char * Mode
	)
{
	int SampCntr32;
	FILE * fp;
	UINT32 * pUINT32;
	int GrpSize32;	//Num 32bit samples in each group
	int TxtBinN;	//1 for txt, 0 for binary
	UINT32 DMAAdrPerCh;
	int GrpsLeft, CurGrp;
	short Hi,Low;


	DMAAdrPerCh = 2048 / (pCA->DMA.DMAChannels + 1); 
	if(DMAAdrPerCh > 1024)	//1024 is max groups per channel
		DMAAdrPerCh = 1024;

		
	//Set ascii or binary output mode
	if(EQUAL(Mode, "8bit-ob-txt") | EQUAL(Mode, "16bit-txt") | EQUAL(Mode, "32bit_h-txt"))
	{
		TxtBinN = 1;
		fp = fopen(fName, "w");
		if(fp == NULL)
		{
			printf("Failed to open %s for writing\n", fName);
			return;
		}
	}
	else
	{
		printf("PrintDMAChannel: PrintDMAChannel: %s mode not supported yet.\n", Mode);
		return; 
	}
		



	CurGrp    = 0;
	GrpsLeft  = pCA->DMA.GrpCnt[Ch] + 1;
	GrpSize32 = (pCA->DMA.BlockSizeB[Ch] * (pCA->DMA.BlockCount[Ch] + 1)) / 4;
	while(GrpsLeft)
	{
		pUINT32 = (UINT32 *)pCA->DMA.dVirtDMAAdr[(Ch*DMAAdrPerCh)+CurGrp];
		
		if(TxtBinN)	//In ascii mode
		{
			for(SampCntr32=0; SampCntr32<GrpSize32; SampCntr32+=2)
			{
				if(EQUAL(Mode, "8bit-ob-txt"))
				{
					fprintf(fp, "%d\n", (*(pUINT32+SampCntr32+1) >> 24 & 0xFF) -128);
					fprintf(fp, "%d\n", (*(pUINT32+SampCntr32+1) >> 16 & 0xFF)-128);
					fprintf(fp, "%d\n", (*(pUINT32+SampCntr32+1) >> 8 & 0xFF)-128);
					fprintf(fp, "%d\n", (*(pUINT32+SampCntr32+1) >> 0 & 0xFF)-128);

					fprintf(fp, "%d\n", (*(pUINT32+SampCntr32) >> 24 & 0xFF)-128);
					fprintf(fp, "%d\n", (*(pUINT32+SampCntr32) >> 16 & 0xFF)-128);
					fprintf(fp, "%d\n", (*(pUINT32+SampCntr32) >> 8 & 0xFF)-128);
					fprintf(fp, "%d\n", (*(pUINT32+SampCntr32) >> 0 & 0xFF)-128);
				}
				else if(EQUAL(Mode, "16bit-txt"))
				{
					Low = (*(pUINT32+SampCntr32+1)) & 0xFFFF;
					Hi  = (*(pUINT32+SampCntr32+1)>>16) & 0xFFFF;
					fprintf(fp, "%d\n%d\n" ,Hi, Low);

					Low = (*(pUINT32+SampCntr32)) & 0xFFFF;
					Hi  = (*(pUINT32+SampCntr32)>>16) & 0xFFFF;
					fprintf(fp, "%d\n%d\n" ,Hi, Low);
				}
				else if(EQUAL(Mode, "32bit_h-txt")) 
				{
					fprintf(fp, "%x\n", *(pUINT32+SampCntr32+1));
					fprintf(fp, "%x\n", *(pUINT32+SampCntr32));
				}
			}
		}
		else
		{
			fprintf(stderr, "Mode not supported yet\n");
		}

		CurGrp++;
		GrpsLeft--;
	}
	
	fclose(fp);
}


/*
	int caFindClkSettings(s_ClkSettings *pClkSettings, double ClkFreq)

	This function computes the settings needed to configure the 
	digital clk distribution part.  It should be called first and then
	and changes can be made to the elements of the struct prior to calling the
	clk config function.
*/
int ca_FindClkSettings(
	s_ClkSettings *pClkSettings, 
	double ClkFreq
	)
{
	printf("caFindClkSettings: Function deprecated in R05. Remove calling code\n");
	return 0;
}

/*
	float caGetTemp(s_ChannelAdapter *pCA)
	
	Returns the current temp in C 
*/
float ca_GetTemp(
	s_ChannelAdapter *pCA
	)
{
	UINT32 TempStatus;
	short TempStatusTmp;
	double Temp;

	Adapter_Read32(pCA, BRIDGE, BRG_TEMPCTL_ADR, &TempStatus);
	TempStatusTmp = (TempStatus>>16) & 0xffff;
	TempStatusTmp = TempStatusTmp >> 3;
	Temp = ((double)TempStatusTmp)  * 0.0625;

	return (float)Temp;
}

/*
	void caSetTempLimit(s_ChannelAdapter *pCA, double TempThresh)
	
	Takes the desired temp limit in C.  Above this limit the bidge will 
	drive the overtemp signal to the V4.
*/
void ca_SetTempLimit(
	s_ChannelAdapter *pCA, 
	double TempThresh
	)
{
	UINT32 HexTempThresh;
	
	TempThresh = TempThresh/0.0625;
	HexTempThresh = (((UINT32)TempThresh) << 3) + 7;
	Adapter_Write32(pCA, BRIDGE, BRG_TEMPCTL_ADR, HexTempThresh);
}

/*
	void caM316ADCConfig(s_ChannelAdapter *pCA, char Data[15])

	This function will change the configuration of ADC08D1500
	ADC.  The serial clk can run at a max 100 MHz, which is much faster
	than target wrs over the PCI bus.  each register access consisters of a 
	32bit field, "0000 0000 0001 A3:A0 D15 D15:D0

  SCLK	V14		SCLK		Bit 3
  SDATA	V15		SDIO		Bit 1
  SCSN	V16		CSN			Bit 2
  CAL	V13		CLK_FUNC	BRG_ACLK_CTL_ADR	

  PDQ B10
  PD G10
*/
void ca_M316ADCConfig(
	s_ChannelAdapter *pCA, 
	UINT32 *Data, 
	int NumCmds
	)
{
	int Addr;

	//Stuff flag and addr into Data[]
	for(Addr=0; Addr<NumCmds; Addr++)
	{
		Data[Addr] = Data[Addr] & 0xFFFFF;	
		Data[Addr] = Data[Addr] | (0x001<<20);
		M316ADCSerialWr(pCA, Data[Addr]);
	}
}

/*

*/
void M316ADCSerialWr(
	s_ChannelAdapter *pCA, 
	UINT32 Data
	)
{
	int Bit;
	UINT32 SerBit;

	Adapter_Write32(pCA, BRIDGE, BRG_M316ADC_SER_ADR, 0x0); //Put down CSN and CLK

	for(Bit=31; Bit>=0; Bit--)
	{
		SerBit = (Data>>Bit)&0x1;

		//printf("caM316ADCSerialWr: Bit %d = %x\n", Bit, SerBit);

		Adapter_Write32(pCA, BRIDGE, BRG_M316ADC_SER_ADR, (0x1<<2));	//Force clk low and CSn high
		Adapter_Write32(pCA, BRIDGE, BRG_M316ADC_SER_ADR, 0x0);			//Force clk low
		Adapter_Write32(pCA, BRIDGE, BRG_M316ADC_SER_ADR, (SerBit<<1));			//Set data bit with clk low
		Adapter_Write32(pCA, BRIDGE, BRG_M316ADC_SER_ADR, (SerBit<<1)|(0x1<<3)); //Raise clk with data held
		Adapter_Write32(pCA, BRIDGE, BRG_M316ADC_SER_ADR, (SerBit<<1));			//Lower clk with data held
		Adapter_Write32(pCA, BRIDGE, BRG_M316ADC_SER_ADR, (0x1<<2));	//Force clk low and CSn high
	}

}

void ca_M316ADCCal(
	s_ChannelAdapter *pCA
	)
{
	Adapter_Write32(pCA, BRIDGE, BRG_M316ADC_CAL_ADR, 0x0);
	Adapter_Write32(pCA, BRIDGE, BRG_M316ADC_CAL_ADR, 0x1);
	Adapter_Write32(pCA, BRIDGE, BRG_M316ADC_CAL_ADR, 0x0);
}

/*	UINT32 AD_DAC_SDI_RdWrN(s_ChannelAdapter *pCA, int RdWrN, short Adr, UINT32 Data)
	Performs configuration of the AD9779A DAC. Command word is 16 bits:
	w/r, x, x, addr, addr, addr, addr, addr, data, data, data, data, data, data, data, data */
UINT32 AD_DAC_SDI_RdWrN(s_ChannelAdapter *pCA, int RdWrN, short Adr, UINT32 Data)
{
	UINT32 CmdWrd = 0;    //Adr / Data pair to be written
	UINT32 DAC_CTL_Value=0;
	UINT32 SDIProgReg = 0;
	UINT32 SDIReadReg = 0;
	int x;
	
	//Form the command word
	CmdWrd = ((RdWrN&0x1)<<15)| ((Adr&0x1F)<<8) | (Data&0xFF);  //Always 1 byte xfer
	Adapter_Write32(pCA, BRIDGE, BRG_ADCCLK_SER_ADR, 0x74); //force all CSB bits low (clk, DAC, BDAC_A, BDAC_B)
	Adapter_Read32(pCA, BRIDGE, BRG_ADCCLK_SER_ADR, &DAC_CTL_Value);
	DAC_CTL_Value = DAC_CTL_Value & ~0x10; // clear the DAC CSB bit
	Adapter_Write32(pCA, BRIDGE, BRG_ADCCLK_SER_ADR, DAC_CTL_Value); //DAC CSB must go low to start programming cycle


	for(x=0; x<=15; x++) 	//Loop throug 16 bits of Adr / Data
	{
		//printf("\tCommand bit is 0x%x\n", (CmdWrd>>15)&0x1);
		Adapter_Write32(pCA, BRIDGE, BRG_ADCCLK_SER_ADR, 0x64 | (((CmdWrd>>15)&0x1)<<1)); //Shift into the SDI bit for writing to the DAC
		Adapter_Read32(pCA, BRIDGE, BRG_ADCCLK_SER_ADR, &SDIProgReg);
		Adapter_Write32(pCA, BRIDGE, BRG_ADCCLK_SER_ADR, 0x6C | (((CmdWrd>>15)&0x1)<<1)); //set SLCK to transfer the bit
		Adapter_Read32(pCA, BRIDGE, BRG_ADCCLK_SER_ADR, &SDIProgReg);
		SDIReadReg = SDIReadReg | (SDIProgReg & 0x1)<<(15-x); //Form the read word
		CmdWrd = (CmdWrd<<1);            //shift the MSB off
	}

	Adapter_Write32(pCA, BRIDGE, BRG_ADCCLK_SER_ADR, 0x6C);
	Adapter_Read32(pCA, BRIDGE, BRG_ADCCLK_SER_ADR, &DAC_CTL_Value);
	DAC_CTL_Value = DAC_CTL_Value | 0x10; // set the DAC CSB bit
	Adapter_Write32(pCA, BRIDGE, BRG_ADCCLK_SER_ADR, DAC_CTL_Value); // set CSB for Dac serial programming to 1
	
	if (RdWrN == 1)
		printf("\tRead 0x%x from DAC address 0x%x\n", SDIReadReg & 0xff, Adr);

	pCA->ReturnStatus = 0x0;
	return pCA->ReturnStatus;
}



/*	UINT32 AD_BDAC_SDI_RdWrN(s_ChannelAdapter *pCA, int RdWrN, short Adr, UINT32 Data)
	Performs configuration of the LTC1661 DACs. Command word is 16 bits (MSB to LSB):
	cmd, cmd, cmd, cmd, input, input, input, input, input, input, input, input, input, input, X, X */
UINT32 AD_BDAC_SDI_Wr(s_ChannelAdapter *pCA, int DACSel, short Ctrl, UINT32 Input)
{
	UINT32 CmdWrd = 0;    //Ctrl command / Input pair to be written
	UINT32 DAC_CTL_Value=0;
	UINT32 SDIProgReg = 0;
	UINT32 BDAC_CSB_SELECT = 0;
	UINT32 BDAC_CSB_DISABLE = 0;
	int x;
	
	/* DACSel= 0-ChA, 1-ChB */
	if(DACSel == 0)
	{
		BDAC_CSB_SELECT = 0x1<<5; //bit 5, CSB for BDAC-A
		BDAC_CSB_DISABLE = 0x1<<6; //bit 6, CSB for BDAC-B
	}
	else if(DACSel == 1)
	{
		BDAC_CSB_SELECT = 0x1<<6;
		BDAC_CSB_DISABLE = 0x1<<5;
	}

	//Form the command word
	CmdWrd = ((Ctrl&0xF)<<15)| ((Input&0x3FF)<<11) | (0x11);  //Lower 2 bits are dont-care
	printf("Command word is 0x%x\n", CmdWrd);
	Adapter_Write32(pCA, BRIDGE, BRG_ADCCLK_SER_ADR, 0x74); //force all CSB bits inactive (clk, DAC, BDAC_A, BDAC_B)
	Adapter_Read32(pCA, BRIDGE, BRG_ADCCLK_SER_ADR, &DAC_CTL_Value);
	DAC_CTL_Value = DAC_CTL_Value & ~0x8; //SCLK must go low before CS goes low to start the programming cycle
	Adapter_Write32(pCA, BRIDGE, BRG_ADCCLK_SER_ADR, DAC_CTL_Value);
	DAC_CTL_Value = DAC_CTL_Value & ~BDAC_CSB_SELECT;
	Adapter_Write32(pCA, BRIDGE, BRG_ADCCLK_SER_ADR, DAC_CTL_Value); //BDAC CSB must go low to start programming cycle

	for(x=0; x<=15; x++) 	//Loop throug 16 bits of Cmd / Input
	{
		printf("\tCommand bit is 0x%x\n", (CmdWrd>>15)&0x1);
		Adapter_Write32(pCA, BRIDGE, BRG_ADCCLK_SER_ADR, BDAC_CSB_DISABLE | 0x14 | (((CmdWrd>>15)&0x1)<<1)); //Shift into the SDI bit for writing to the DAC
		Adapter_Read32(pCA, BRIDGE, BRG_ADCCLK_SER_ADR, &SDIProgReg);
		Adapter_Write32(pCA, BRIDGE, BRG_ADCCLK_SER_ADR, BDAC_CSB_DISABLE | 0x1C | (((CmdWrd>>15)&0x1)<<1)); //set SLCK to transfer the bit
		Adapter_Read32(pCA, BRIDGE, BRG_ADCCLK_SER_ADR, &SDIProgReg);
		CmdWrd = (CmdWrd<<1);            //shift the MSB off
	}

	Adapter_Write32(pCA, BRIDGE, BRG_ADCCLK_SER_ADR, (BDAC_CSB_DISABLE | 0x1C)); //force all other CSB bits inactive
	Adapter_Read32(pCA, BRIDGE, BRG_ADCCLK_SER_ADR, &DAC_CTL_Value);
	DAC_CTL_Value = DAC_CTL_Value | BDAC_CSB_SELECT; // BDAC CSB must go high to end programming cycle
	Adapter_Write32(pCA, BRIDGE, BRG_ADCCLK_SER_ADR, DAC_CTL_Value); 
	
	pCA->ReturnStatus = 0x0;
	return pCA->ReturnStatus;
}

int LoadConfig(s_ChannelAdapter *pCA, char * configFileName)
{
	FILE * fp;
	char sLine[256];
	char seps[] = ",\t\n";
	char *token;
	UINT32 Adr,Data;
	
	fp = fopen(configFileName, "r");
	if(fp != NULL)
	{
		fgets(sLine, 256, fp);
		while(!feof(fp))
		{
			while(sLine[0] == ' ')
				strcpy(sLine,(char *)(sLine + 1));
			
			if (sLine[0] != '/')
			{
				token = strtok(sLine, seps); //Get adr
				//printf("Token=%s\n", token);
				Adr   = myatoi(token);
				token = strtok(NULL, seps);
				//printf("Token=%s\n", token);
				Data  =  myatoi(token);
				//printf("\tAddr = 0x%x, Data = 0x%x\n", Adr, Data);

				AD_DAC_SDI_RdWrN(pCA, 0x0, Adr, Data); // write address 1
				AD_DAC_SDI_RdWrN(pCA, 0x1, Adr, 0x00); // read address 1
			}

			fgets(sLine, 256, fp);  //Get next line
		}

		fclose(fp);
		return 0;
	}
	else
	{
		fprintf(stderr, "LoadConfig: Failed to open config file");
		return -1;	
	}
}

UINT32 myatoi (char AsciiNum[])
{
	UINT32 Num;
    int loop,Power,mult,len;
    Power = Num = 0;
    len = strlen(AsciiNum) - 1;

	while(AsciiNum[0] == ' ')
		strcpy(AsciiNum,(char *)(AsciiNum + 1));

    if (AsciiNum[len] == '\n')
    {
        AsciiNum[len] = '\0';
    }
    if (ishex(AsciiNum))
    {
        len = strlen(AsciiNum) - 1;
        for (loop=len; loop>=0; loop--)
        {
            mult = 1 << Power;
            Power+=4;
            switch (AsciiNum[loop])
            {
                case '0': Num += 0 * mult; break;
                case '1': Num += 1 * mult; break;
                case '2': Num += 2 * mult; break;
                case '3': Num += 3 * mult; break;
                case '4': Num += 4 * mult; break;
                case '5': Num += 5 * mult; break;
                case '6': Num += 6 * mult; break;
                case '7': Num += 7 * mult; break;
                case '8': Num += 8 * mult; break;
                case '9': Num += 9 * mult; break;
                case 'a':
                case 'A': Num += 10 * mult; break;
                case 'b':
                case 'B': Num += 11 * mult; break;
                case 'c':
                case 'C': Num += 12 * mult; break;
                case 'd':
                case 'D': Num += 13 * mult; break;
                case 'e':
                case 'E': Num += 14 * mult; break;
                case 'f':
                case 'F': Num += 15 * mult; break;
            }
        }
    }
    else
    {
        Num = atoi(AsciiNum);
    }
    return (Num);
}


int ishex (char AsciiNum[])
{
    int result;

    if ((AsciiNum[0] == '0') && (AsciiNum[1] == 'x' || AsciiNum[1] == 'X'))
    {
        strcpy(AsciiNum,(char *)(AsciiNum + 2));
        result = 1;
    }
    else
    {
        result = 0;
    }
    return(result);
}



