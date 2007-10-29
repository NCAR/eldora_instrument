

#include "ca_sram_functions.h"
#include "ca_functions.h"

//Internal functions
void ca_SRAM_SetClkFwd(s_ChannelAdapter *pCA, int Taps);

void ca_SRAM_Init(
				 s_ChannelAdapter *pCA, 
				 int SRAM, 
				 int ClkShift
				 )
{
	if(ClkShift != 0)
	{
		ca_SRAM_SetClkFwd(pCA, ClkShift);
		Adapter_uSleep(500E3);		//Allow time for SRAM DLL to lock
	}


	Adapter_Write32(pCA, V4, V4_SRAMA_CTL_ADR+(0x10*SRAM), 0x1);	//Disable SRAM
	Adapter_Write32(pCA, V4, V4_SRAMA_CTL_ADR+(0x10*SRAM), 0x0);	//Enable SRAM 
	Adapter_uSleep(100E3);		//Allow time for SRAM DLL to lock
	
	
	Adapter_Write32(pCA, V4, V4_SRAMA_CTL_ADR+(0x10*SRAM), 0x2);	//Perform CAL
	Adapter_Write32(pCA, V4, V4_SRAMA_CTL_ADR+(0x10*SRAM), 0x0);

	Adapter_uSleep(1E6);		//Wait for cal to finish
}

/*
	void ca_SRAMDisable

	Puts the SRAM in a low power state with the clock stopped.
*/
void ca_SRAM_Disable( 
					s_ChannelAdapter *pCA, 
					int SRAM
					)
{
	Adapter_Write32(pCA, V4, V4_SRAMA_CTL_ADR+(0x10*SRAM), 0x1);	//Disable SRAM
	return;
}


/*
	void ca_SRAMSetClkFwd(s_ChannelAdapter *pCA, int Taps)

	Assumes reset has already put us back to 0
*/	
void ca_SRAM_SetClkFwd(
					s_ChannelAdapter *pCA, 
					int Taps
					)
{
	int x;
	int increment;

	increment = 0x1;   // Set to increment 
	for(x=0; x<Taps; x++)
	{
		Adapter_Write32(pCA, V4, V4_SRAMA_CTL_ADR, (0x1<<5)|(increment<<4)); //Commit advance
		Adapter_uSleep(1);		//Allow time for DCM phase shift
	}
}



int ca_SRAM_Wr(
			  s_ChannelAdapter *pCA, 
			  int SRAM, 
			  UINT32 Addr, 
			  UINT32 Data, 
			  UINT32 ByteEnN
			  )
{
		
	Addr    = Addr & 0x3FFFFE;	//Limit Addr
	ByteEnN = ByteEnN & 0xF;

	
	Adapter_Write32(pCA, V4, V4_SRAMA_WRDATA_ADR+(0x10*SRAM), Data);				//Set Data
	Adapter_Write32(pCA, V4, V4_SRAMA_ADDR_ADR+(0x10*SRAM), (SRAM_WREN)|(ByteEnN<<22)|Addr);	//Set Addr & BE

	return 0;
}

int ca_SRAM_Rd(
			  s_ChannelAdapter *pCA, 
			  int SRAM, 
			  UINT32 Addr, 
			  UINT32 *Data
			  )
{
	UINT32 Dummy;
	
	Addr   = Addr & 0x3FFFFF;	//Limit Addr
	

	Adapter_Write32(pCA, V4, V4_SRAMA_ADDR_ADR+(0x10*SRAM), (SRAM_RDEN)|Addr);	//Set Addr
	Adapter_Read32(pCA,  V4, V4_SRAMA_RDDATA_ADR+(0x10*SRAM), &Dummy);

	*(Data) = Dummy;

	return 0;
}


int ca_SRAM_WriteToDisk(
	s_ChannelAdapter *pCA, 
	int SRAM,			//0-3, -1 for auto
	UINT32 Bytes,		
	char * outFileName, 
	char * Mode,
	char * Format
	)
{
	UINT32 Addr, Data;
	UINT32 Cntr;
	FILE * txt_fp;
	FILE * bin_fp;
	short sLo, sHi;


	txt_fp = NULL;
	bin_fp = NULL;

	//Open a txt or binary file as needed, leave other ptr at NULL
	if(EQUAL(Mode,"txt"))
		txt_fp = fopen(outFileName, "w");
	else if(EQUAL(Mode,"bin"))
		bin_fp = fopen(outFileName, "wb");
	else
	{
		printf("ca_SRAM_WriteToDisk: Failed to open file for output\n");
		return -1;
	}
	

	if(EQUAL(Format,"cplx-16bit-wbpulse"))	//16 bit complex data from WB Pulse, with header
	{
		for(Cntr=0; Cntr<Bytes; Cntr+=2)
		{
			
			SRAM = Cntr/0x3FFFFF;
			Addr = Cntr%0x3FFFFF;

			ca_SRAM_Rd(pCA, SRAM, Addr, &Data);
			
			//Strip the header marker out and put it to the screen
			if(Cntr == 0x0)
			{
				//Do nothing, header marker
			}
			else if(Cntr == 0x2)
			{
				//Do nothing, header marker
			}
			else if(Cntr == 0x4)
			{
				printf("ca_SRAM_WriteToDisk: Start time found in header was %x: ", Data);
			}
			else if(Cntr == 0x6)
			{
				printf("%x\n", Data);
			}
			else
			{
				if(txt_fp != NULL)
				{
					sHi = ((Data&0xFFFF0000)>>16);
					sLo = Data&0xFFFF;
					fprintf(txt_fp, "%d,\t%d\n", sHi ,sLo );
				}
				else if(bin_fp != NULL)
				{
					Data = (((Data&0xFFFF0000)>>16) | ((Data&0xFFFF)<<16));  //Flip I/Q to match MATLAB scripts
					fwrite(&Data, sizeof(UINT32), 1, bin_fp);
				}
				else
					return -1;
			}

		}
	}
	else
	{
		printf("ca_SRAM_WriteToDisk: Format did not match any supported mode.\n");	
		return -1;
	}
	/*
	else if(EQUAL(Mode,"16bit-ts-txt")) //Each 32bit SRAM word has two 16bit samples packed into it
	{
		//Write out header information
		ca_SRAM_Rd(pCA, SRAM, 0x0, &Data);
		fprintf(fp, "%x\n", Data);
		ca_SRAM_Rd(pCA, SRAM, 0x2, &Data);
		fprintf(fp, "%x\n", Data);
		ca_SRAM_Rd(pCA, SRAM, 0x4, &Data);
		fprintf(fp, "%x\n", Data);
		ca_SRAM_Rd(pCA, SRAM, 0x6, &Data);
		fprintf(fp, "%x\n", Data);

		//Each 32bit read gets 4 bytes
		Addr = 0x8;
		for(BytesRead = 16; BytesRead<Bytes; BytesRead+=4)
		{
			ca_SRAM_Rd(pCA, SRAM, Addr, &Data);

			Lo = 0xFFFF&Data;
			Hi = (Data>>16)&0xFFFF;

			//fprintf(fp, "[%x] : %x\n", Addr, Data);
			
			if(Hi+1 != Lo)
				fprintf(fp2, "Error at [%x] : %x\n", Addr, Data);
			
			Addr += 0x2; //Adv for next read
		}
	}
	*/


	if(txt_fp != NULL)
		fclose(txt_fp);

	if(bin_fp != NULL)
		fclose(bin_fp);

	return 0;
}