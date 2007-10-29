#ifndef CA_FUNCTIONS__H
#define CA_FUNCTIONS__H

#if defined(__cplusplus)
    extern "C" {
#endif


#include "channeladapter.h"



int EQUAL(char * str1, char * str2); /*Checks strs are equal*/

void ca_LoadDMASettings(s_ChannelAdapter *pCA); /*Loads DMA buffer addr to hardware*/
void ca_PrintDMAChannel(s_ChannelAdapter *pCA, char * fName, int Ch, char * Mode); /*Writes all DMA buffers for a ch to disk*/
void ca_PrintDMASettings(s_ChannelAdapter *pCA); /*Displays DMA information to screen*/

	
int ca_FindClkSettings(s_ClkSettings *pClkSettings, double ClkFreq);	/*Finds clk chip settings based on freq*/

/*Temp Monitoring Functions*/
float ca_GetTemp(s_ChannelAdapter *pCA);	/*Returns current temp in C*/
void ca_SetTempLimit(s_ChannelAdapter *pCA, double TempThresh); /*Sets limit in C to cause overtemp signal to V4*/

/*M316 Functions*/
void ca_M316ADCConfig(s_ChannelAdapter *pCA, UINT32 *Data, int NumCmds);
void ca_M316ADCCal(s_ChannelAdapter *pCA);

#ifdef __cplusplus
}
#endif

#endif