
#if defined(WIN32)
	#include <windows.h>
#elif defined(LINUX)
	#include <malloc.h>
#endif

#include "channeladapter.h"
#include "ca_functions.h"
#include "ca_membuffer.h"
#include "ca_lb_memmap.h"

/*
	int ChMemBuffer_Init(s_ChannelAdapter *pCA,  s_ChMemBuffer * pMemBuf, int Ch)
	
	Assumes DMA buffers have already been allocated
	
*/
int ca_MemBuffer_Init(
					   s_ChannelAdapter *pCA,	//ptr to CA struct, must have DMA bufs allocated
					   s_ChMemBuffer * pMemBuf, //ptr to MemBuffer struct
					   int Ch,					//which DMA channel to assocate with
					   int GrpsToCapture		//Space for this many grps will be allocated, stored in strcut
					   )
{	
	//Init 
	pMemBuf->LastGrp = 0;
	pMemBuf->TotalGrpsCaptured = 0;	
	pMemBuf->GrpsToCapture = GrpsToCapture;
	
	//Compute the number of addr reserved for each DMA channel, all may not be used
	pMemBuf->MaxGrpsPerCh = 2048 / (pCA->DMA.DMAChannels + 1);
	
	//Compute the number of 32bit words in each group
	pMemBuf->GrpSize32 = (pCA->DMA.BlockSizeB[Ch] * (pCA->DMA.BlockCount[Ch] + 1)) / 4;
	
	//Allocate the DMA buffer to copy data to
	pMemBuf->pDataBuffer = (UINT32 *)malloc(pMemBuf->GrpSize32  * pMemBuf->GrpsToCapture * 4);
	if(pMemBuf->pDataBuffer == NULL)
		return -1;
		
	return 0;
}

int ca_MemBuffer_Free(
					  s_ChMemBuffer * pMemBuf
					  )
{
	if(pMemBuf->pDataBuffer != NULL)
	{
		free(pMemBuf->pDataBuffer);
		return 0;
	}
	else
		return -1;

}

int ca_MemBuffer_Copy(
					  s_ChannelAdapter *pCA,  
					  s_ChMemBuffer * pMemBuf, 
					  int Ch
					  )
{
	UINT32 CurGrp;
	UINT32 *src;
	UINT32 *dest;
	int GrpsRdy;

	GrpsRdy = 0;

	
	
	//Read the CurrentGroup register and determine how many groups are ready to move
	Adapter_Read32(pCA, V4, DMA_CH0_CURGRP_ADR+(0x4*Ch), &CurGrp);
	
	//Loop through all of the avalible groups and copy them away
	while((pMemBuf->LastGrp != CurGrp) & (pMemBuf->TotalGrpsCaptured != pMemBuf->GrpsToCapture))
	{
		//Copy the group to the large memory buffer
		src  = pCA->DMA.dVirtDMAAdr[(Ch*pMemBuf->MaxGrpsPerCh)+pMemBuf->LastGrp];
		dest =  pMemBuf->pDataBuffer + (pMemBuf->GrpSize32 * pMemBuf->TotalGrpsCaptured);
		memcpy(dest, src, pMemBuf->GrpSize32*4);
		
		//Advance the counters
		if(pMemBuf->LastGrp < pCA->DMA.GrpCnt[Ch])
			pMemBuf->LastGrp++;
		else
			pMemBuf->LastGrp = 0;  //Wrap back 
			
		pMemBuf->TotalGrpsCaptured++;
		GrpsRdy++;
	}
	
	return GrpsRdy;
}


/*
	int caChMemBuffer_WriteToDisk(s_ChMemBuffer * pMemBuf, char * fName, char * fileMode, char *dataMode)
	
	This function takes data which is located in a virtual memory area and writes it out to a file.

	Inputs :	s_ChMemBuffer * pMemBuf - Pointer to the s_ChMemBuffer to be written out
				char * fName - Name of the output file
				char * fileMode - "txt" or "binary"
				char *dataMode - "lbexample"		| Local Bus Example
								 "8bit-txt"			| 8bit  OB data, ASCII text
								 "16bit-txt"		| 16bit 2s data, ASCII text
								 "32bit_h_txt"		| 32bit hex data, ASCII text
								 "16bit-header-txt"	| 16bit 2s data with headers, ASCII text


*/
int ca_MemBuffer_WriteToDisk(
							 s_ChMemBuffer * pMemBuf,	//ptr to MemBuf struct
							 char *fName,				//str array with output file name
							 char *fileMode,			//"txt" or "bin"
							 char *dataMode				//"lbexample", "real-16bit", "complex-16bit"
							 )
{
	int TxtFile;
	int RealMode;
	FILE * fp;
	int x;
	short Hi1,Low1, Hi0,Low0;
	unsigned int ChID, PulseCount;

	UINT32 Dummy;
	UINT32 dwHi, dwLow;	//For 32bit samples
	
	
	//Set Modes
	TxtFile = 1;
	RealMode = 1;

		
	fp = fopen(fName, "w");
	if(fp == NULL)
	{
		fprintf(stderr, "caChMemBuffer_WriteToDisk: Failed to open output file %s\n", fName);
		return -1;	
	}
	

	//Write the whole buffer to disk, one 64 bit chunk at a time
	for(x=0; x < pMemBuf->GrpSize32*pMemBuf->GrpsToCapture; x+=2)
	{
		if(EQUAL(dataMode, "lbexample"))
		{
			/*
				Each 32bit word is 
					[31:27] = ChID, 
					[26:0]  = Count
				Will print Ch <tab> Count 
			*/
		
			dwHi  = *(pMemBuf->pDataBuffer+x+1);
			dwLow = *(pMemBuf->pDataBuffer+x);

			fprintf(fp, "%x\t%8x\n", (dwLow&0xF8000000)>>27, dwLow&0x07FFFFFF);
			fprintf(fp, "%x\t%8x\n",  (dwHi&0xF8000000)>>27, dwHi&0x07FFFFFF);	
		
		}
		else if(EQUAL(dataMode, "32bit_h_txt"))
		{	
			dwHi  = *(pMemBuf->pDataBuffer+x+1);
			dwLow = *(pMemBuf->pDataBuffer+x);

			fprintf(fp, "%x\t%8x\n", dwLow);
			fprintf(fp, "%x\t%8x\n", dwHi);

		}
		else if(EQUAL(dataMode, "16bit-txt"))
		{
			Hi0 = (*(pMemBuf->pDataBuffer+x+1)>>16) & 0xFFFF;
			Low0 = *(pMemBuf->pDataBuffer+x+1) & 0xFFFF;
			Hi1 = (*(pMemBuf->pDataBuffer+x)>>16) & 0xFFFF;
			Low1 = *(pMemBuf->pDataBuffer+x) & 0xFFFF;
			fprintf(fp, "%d\n%d\n%d\n%d\n", Hi0, Low0, Hi1, Low1);
		}
		else if(EQUAL(dataMode, "16bit-header-txt"))
		{
			//Xfers were on 64bit boundries, so re-arange them here
			Hi0 = (*(pMemBuf->pDataBuffer+x+1)>>16) & 0xFFFF;
			Low0 = *(pMemBuf->pDataBuffer+x+1) & 0xFFFF;
			Hi1 = (*(pMemBuf->pDataBuffer+x)>>16) & 0xFFFF;
			Low1 = *(pMemBuf->pDataBuffer+x) & 0xFFFF;

			//Look for the marker at the start of a pulse
			if(Hi0 == -32768) //-0 in 2s
				fprintf(fp, "==== Start of Pulse %d ====\n",Low1);
			else
				fprintf(fp, "%d\n%d\n%d\n%d\n", Hi0, Low0, Hi1, Low1);
		}
		else if(EQUAL(dataMode, "complex-16bit"))
		{
			Hi0 = (*(pMemBuf->pDataBuffer+x+1)>>16) & 0xFFFF;
			Low0 = *(pMemBuf->pDataBuffer+x+1) & 0xFFFF;
			Hi1 = (*(pMemBuf->pDataBuffer+x)>>16) & 0xFFFF;
			Low1 = *(pMemBuf->pDataBuffer+x) & 0xFFFF;
			fprintf(fp, "%d, %d\n%d, %d\n", Hi0, Low0, Hi1, Low1);
		}
		else
		{
			fprintf(stderr, "caChMemBuffer_WriteToDisk: Mode not supported!\n");
			fclose(fp);
			return -1;
		}		
	} /*end for*/
	
	
	fclose(fp);
	return 0;	
}
