
#include <stdio.h>
#include <string.h>
#ifdef WIN32
	#include <conio.h>
	#include <windows.h>
	#include <malloc.h>
#else
	
#endif


#include "channeladapter.h"
#include "ca_wbpulse_functions.h"
#include "ca_wbpulse_memmap.h"

//Local functions
UINT32 myatoi (char AsciiNum[]);
int ishex (char AsciiNum[]);


void ca_WBPulse_PrintStatusReg(
	UINT32 StatusReg	
	)
{	
		if(StatusReg & DMA_GRP_DONE)	printf("\tDMA Group Done\n");
		if(StatusReg & SNAPSHOT_DONE) 	printf("\tSnapshot Done\n");
		if(StatusReg & OVERTEMP)  		printf("\tOver Temp\n");
		if(StatusReg & TRIG_1PPS) 		printf("\tTrigger/1PPS\n");
		if(StatusReg & DDC_FF_EMPTY) 	printf("\tDDC FIFO Empty\n");
		if(StatusReg & DDC_FF_OVFLW) 	printf("\tDDC FIFO Overflow\n");
		if(StatusReg & DDC_FF_FULL) 	printf("\tDDC FIFO Full\n");
		if(StatusReg & ADCA_CURR_OVR) 	printf("\tADC-A In overrange\n");
		if(StatusReg & ADCB_CURR_OVR) 	printf("\tADC-B in overrange\n");
		if(StatusReg & ADCA_PREV_OVR) 	printf("\tADC-A has overranged\n");
		if(StatusReg & ADCB_PREV_OVR) 	printf("\tADC-B has overranged\n");
		if(StatusReg & ADC_DCM_UNLOCKED)printf("\tADC DCM unlocked\n");
			

	return;
}


// Subroutine used to load configuration data into WBDDC core
int ca_WBPulse_LoadDDCConfig(
	s_ChannelAdapter * CA0, 	//Pointer to CA 
	char * configFileName,		//char array of path to config
	int Verbose					//0-5, larger for more verbose info
	)
{
	FILE * fp;		// config file name
	char sLine[256]; 
	char seps[] = ",\t\n"; // accepted address/data separators
	char *token;			
	UINT32 Adr,Data,Verify; 
	int total_dec;
	
    total_dec = 1;
	fp = fopen(configFileName, "r");
	if(fp != NULL) // read until the end of the file
	{
		fgets(sLine, 256, fp); // read config file line
		while(!feof(fp))
		{		
			token = strtok(sLine, seps); //Get adr	
			if(strncmp(token," ",1) != 0 )  // Check for start of blank line (i.e. illegal input)
			{
				Adr   = myatoi(token);		 //convert to number
				token = strtok(NULL, seps);  //Get data
				Data  =  myatoi(token);		 //convert to number
				Adapter_Write32(CA0, V4, Adr, Data); // write data to the specified address
				Adapter_Read32(CA0, V4, Adr, &Verify); // read back the data

	            if(Adr == 0x1308) total_dec = total_dec*Data*2;
		        if(Adr == 0x1310) total_dec = total_dec*(Data+1);
	            if(Adr == 0x1318) total_dec = total_dec*(Data+1);
			
				fgets(sLine, 256, fp);  //Get next line
			}
		}

		fclose(fp); // close the file

		//printf("\nTotal DDC Decimation from Config = %d\n",total_dec);

		return total_dec;
	}
	else
	{
		fprintf(stderr, "LoadConfig: Failed to open config file");
		return -1;	
	}
}



// Subroutine used to load TOD configuration data
int ca_WBPulse_LoadToDConfig(
	s_ChannelAdapter * CA0, 	//Pointer to CA 
	char * configFileName,		//char array of path to config
	int Verbose					//0-5, larger for more verbose info
	)
{
	FILE * fp;		// TOD config file name

	char sLine[256]; 
	char seps[] = ",\t\n"; // accepted address/data separators
	char *token;			
	UINT32 Adr,Data,Verify; 

	
	fp = fopen(configFileName, "r");
	if(fp != NULL) // read until the end of the file
	{
		fgets(sLine, 256, fp); // read config file line
		while(!feof(fp))
		{
			token = strtok(sLine, seps); //Get adr	
			if(strncmp(token," ",1) != 0 )  // Check for start of blank line (i.e. illegal input)
			{
				Adr   = myatoi(token);		 //convert to number
				token = strtok(NULL, seps);  //Get data
				Data  =  myatoi(token);		 //convert to number
				Adapter_Write32(CA0, V4, Adr, Data); // write data to the specified address
				Adapter_Read32(CA0, V4, Adr, &Verify); // read back the data

				if(Verbose >= 5)
				{
					if(Adr == V4_START_SEC_ADR)		printf("\nStart Seconds Setting = 0x%x\n",Data);
					if(Adr == V4_START_FRAC_ADR)	printf("\nStart Fractional Seconds Setting = %d\n",Data);
			        if(Adr == V4_SAMPLES_ADR)		printf("\n# Samples Setting = %d\n",Data);
				    if(Adr == V4_SNAPEN_ADR)		printf("\nSnap Enable Setting = %d\n",Data);		
					if(Adr == V4_INT_EXT_ADR)		printf("\nINT/EXT Setting = %d\n",Data);
					if(Adr == V4_SECS_IN_ADR)		printf("\nTOD Seconds Initialization Value = %d\n",Data);
					if(Adr == V4_LOAD_ADR)			printf("\nTOD Load Setting = %d\n",Data);
					if(Adr == V4_CLK_FREQ_ADR)		printf("\nClock Frequency Setting = %d\n",Data);
				}
			}

			fgets(sLine, 256, fp);  //Get next line
		}

		fclose(fp); // close the file


		return 0;
	}
	else
	{
		fprintf(stderr, "ca_WBPulse_LoadTodConfig: Failed to open config file");
		return -1;	
	}
}





// convert hexadecimal text string to unsigned integer
UINT32 myatoi (char AsciiNum[])
{
	UINT32 Num;
    int loop,Power,mult,len;
    Power = Num = 0;
    len = strlen(AsciiNum) - 1;
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


/****************************************************************************
	int ishex (char AsciiNum[])

	Checks to see if a nuber is hex, part of myatoi
****************************************************************************/
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
