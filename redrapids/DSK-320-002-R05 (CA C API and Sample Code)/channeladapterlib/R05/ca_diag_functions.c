
#include "channeladapter.h"
#include "ca_diag_memmap.h"
#include "ca_sram_memmap.h"
#include "ca_functions.h"


void ca_SRAMSetClkFwd(s_ChannelAdapter *pCA, int Taps);

void ca_Diag_PrintStatusReg(
	UINT32 StatusReg
	)
{	
		if(StatusReg & DMA_GRP_DONE) printf("\tDMA Group Done\n");

		if(StatusReg & SNAP_DONE) printf("\tSnapshot Done\n");

		if(StatusReg & OVERTEMP)  printf("\tOver Temp\n");
		
		if(StatusReg & TRIG_1PPS) printf("\tTrigger/1PPS\n");
		
		if(StatusReg & TELAD_FF_EMPTY) printf("\tTelem FIFO Empty\n");
		if(StatusReg & ADCA_FF_EMPTY) printf("\tADC-A FIFO Empty\n");
		if(StatusReg & ADCB_FF_EMPTY) printf("\tADC-B FIFO Empty\n");
		if(StatusReg & ADCC_FF_EMPTY) printf("\tADC-C FIFO Empty\n");
		if(StatusReg & ADCD_FF_EMPTY) printf("\tADC-D FIFO Empty\n");
		
		if(StatusReg & TELAD_FF_OVFLW) printf("\tTelm. FIFO Overflow\n");
		if(StatusReg & ADCA_FF_OVFLW) printf("\tADC-A FIFO Overflow\n");
		if(StatusReg & ADCB_FF_OVFLW) printf("\tADC-B FIFO Overflow\n");
		if(StatusReg & ADCC_FF_OVFLW) printf("\tADC-C FIFO Overflow\n");
		if(StatusReg & ADCD_FF_OVFLW) printf("\tADC-D FIFO Overflow\n");
	
		if(StatusReg & TELAD_FF_FULL) printf("\tTelm. FIFO Full\n");
		if(StatusReg & ADCA_FF_FULL) printf("\tADC-A FIFO Full\n");
		if(StatusReg & ADCB_FF_FULL) printf("\tADC-B FIFO Full\n");
		if(StatusReg & ADCC_FF_FULL) printf("\tADC-C FIFO Full\n");
		if(StatusReg & ADCD_FF_FULL) printf("\tADC-D FIFO Full\n");
		
		if(StatusReg & ADCA_CURR_OVR) printf("\tADC-A In overrange\n");
		if(StatusReg & ADCB_CURR_OVR) printf("\tADC-B in overrange\n");
		if(StatusReg & ADCC_CURR_OVR) printf("\tADC-C in overrange\n");
		if(StatusReg & ADCD_CURR_OVR) printf("\tADC-D in overrange\n");
		
		if(StatusReg & ADCA_PREV_OVR) printf("\tADC-A has overranged\n");
		if(StatusReg & ADCB_PREV_OVR) printf("\tADC-B has overranged\n");
		if(StatusReg & ADCC_PREV_OVR) printf("\tADC-C has overranged\n");
		if(StatusReg & ADCD_PREV_OVR) printf("\tADC-D has overranged\n");
		
		if(StatusReg &SRAM_DCM_UNLOCKED) printf("\tIDELAY DCM unlocked\n");
		if(StatusReg &ADC_DCM_UNLOCKED) printf("\tADC DCM unlocked\n");
			

	return;
}



void ca_Diag_ADCFifoTargetWriteToDisk(
	s_ChannelAdapter *pCA, 
	UINT32 FIFOLowAdr, 
	UINT32 Size, 
	char * Mode, 
	char * chFname
	)
{
	FILE * fp;
	int x;
	UINT32 High, Low;
	short Data;
	
	fp = fopen(chFname, "w");

	if(EQUAL(Mode, "8bit-ob-txt"))
	{
		for(x=0; x<Size; x++)
		{
			Adapter_Read32(pCA, V4, FIFOLowAdr+0x4, &High);
			fprintf(fp, "%d\n", (High >> 24 & 0xFF) -128);
			fprintf(fp, "%d\n", (High >> 16 & 0xFF)-128);
			fprintf(fp, "%d\n", (High >> 8 & 0xFF)-128);
			fprintf(fp, "%d\n", (High >> 0 & 0xFF)-128);
	
			Adapter_Read32(pCA, V4, FIFOLowAdr, &Low);
			fprintf(fp, "%d\n", (Low >> 24 & 0xFF)-128);
			fprintf(fp, "%d\n", (Low >> 16 & 0xFF)-128);
			fprintf(fp, "%d\n", (Low >> 8 & 0xFF)-128);
			fprintf(fp, "%d\n", (Low >> 0 & 0xFF)-128);
		}
	}
	else if (EQUAL(Mode, "16bit-txt"))
	{
		for(x=0; x<Size; x++)
		{
			Adapter_Read32(pCA, V4, FIFOLowAdr+0x4, &High);
			Data = (High>>16)&0xFFFF;
			fprintf(fp, "%d\n", Data);
			Data = (High & 0xFFFF);
			fprintf(fp, "%d\n", Data);
	
			Adapter_Read32(pCA, V4, FIFOLowAdr, &Low);
			Data = (Low >> 16)&0xFFFF;
			fprintf(fp, "%d\n", Data);
			Data = (Low & 0xFFFF);
			fprintf(fp, "%d\n", Data);
		}
	}
	else if (EQUAL(Mode, "12bit-ob-txt"))
	{
		for(x=0; x<Size; x++)
		{
			Adapter_Read32(pCA, V4, FIFOLowAdr+0x4, &High);
			Data = (High>>16)&0xFFFF;
			fprintf(fp, "%d\n", Data-2048);
			Data = (High & 0xFFFF);
			fprintf(fp, "%d\n", Data-2048);
	
			Adapter_Read32(pCA, V4, FIFOLowAdr, &Low);
			Data = (Low >> 16)&0xFFFF;
			fprintf(fp, "%d\n", Data-2048);
			Data = (Low & 0xFFFF);
			fprintf(fp, "%d\n", Data-2048);
		}
	}	
	
	fclose(fp);
}
