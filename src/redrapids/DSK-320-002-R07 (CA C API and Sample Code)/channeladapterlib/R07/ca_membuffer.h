#ifndef CA_MEMBUFFER__H
#define CA_MEMBUFFER__H

#if defined(__cplusplus)
    extern "C" {
#endif

typedef struct {
	UINT32 *pDataBuffer;	/*Pointer on which to allocate buffer*/
	int GrpsToCapture;  	/*Number of Grps to capture*/
	int MaxGrpsPerCh;   	/*Max number of Number of DMA groups per channel*/
	UINT32 GrpSize32;	    /*Number of 32bit words in each group*/  
	
	int LastGrp;			/*Tracks last DMA group to transfer*/
	int TotalGrpsCaptured;	/*Tracks total groups transfred*/
} s_ChMemBuffer; 

int ca_MemBuffer_Init(s_ChannelAdapter *pCA,  s_ChMemBuffer * pMemBuf, int Ch, int GrpsToCapture);
int ca_MemBuffer_Free(s_ChMemBuffer * pMemBuf);
int ca_MemBuffer_Copy(s_ChannelAdapter *pCA,  s_ChMemBuffer * pMemBuf, int Ch);
int ca_MemBuffer_WriteToDisk(s_ChMemBuffer * pMemBuf, char * fName, char * fileMode, char *dataMode);
int ca_MemBuffer_CheckCount(s_ChMemBuffer * pMemBuf, int Ch, UINT32 *ErrCnt);

#ifdef __cplusplus
}
#endif

#endif