#ifndef CA_SRAM_FUNCTIONS__H
#define CA_SRAM_FUNCTIONS__H

#if defined(__cplusplus)
    extern "C" {
#endif


#include "channeladapter.h"
#include "ca_sram_memmap.h"

void ca_SRAM_Init(s_ChannelAdapter *pCA, int SRAM, int ClkShift);
void ca_SRAM_Disable(s_ChannelAdapter *pCA, int SRAM);
int ca_SRAM_Wr(s_ChannelAdapter *pCA, int SRAM, UINT32 Addr, UINT32 Data, UINT32 ByteEnN);
int ca_SRAM_Rd(s_ChannelAdapter *pCA, int SRAM, UINT32 Addr, UINT32 *Data);
int ca_SRAM_WriteToDisk(s_ChannelAdapter *pCA, int SRAM, UINT32 Bytes, char * outFileName, char * Mode, char * Format);


#ifdef __cplusplus
}
#endif

#endif