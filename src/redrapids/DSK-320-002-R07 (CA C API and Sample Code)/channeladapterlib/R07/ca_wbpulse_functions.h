#ifndef CA_WBPULSE_FUNCTIONS__H
#define CA_WBPULSE_FUNCTIONS__H

#if defined(__cplusplus)
    extern "C" {
#endif

void ca_WBPulse_PrintStatusReg(UINT32 StatusReg);
int ca_WBPulse_LoadDDCConfig(s_ChannelAdapter * CA0, char * configFileName,	int Verbose);
int ca_WBPulse_LoadToDConfig(s_ChannelAdapter * CA0, char * configFileName,	int Verbose);


#ifdef __cplusplus
}
#endif

#endif