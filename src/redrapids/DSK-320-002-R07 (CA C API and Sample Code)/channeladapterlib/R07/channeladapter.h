/**********************************************************************************************
	Red Rapids
	797 N. Grove Rd
	Suite 101
	Richardson, TX 75081
	www.redrapids.com | support@redrapids.com

	Desc: ChannelAdapter API - R04

			Rev History
				R04	-	Merged VxWorks back into mainline code.  
***********************************************************************************************/

#ifndef CHANNELADAPTER__H
#define CHANNELADAPTER__H

#if defined(__cplusplus)
    extern "C" {
#endif

#include <stdio.h>

#if defined(VXWORKS)
	#include "vxWorks.h"
#endif


#ifndef _WINDRVR_H_
	#if defined(WIN32)
		typedef unsigned int UINT32;
		typedef unsigned __int64 UINT64; 
	#elif defined(WIN64)
		typedef unsigned int UINT32;
		typedef unsigned __int64 UINT64; 
	#elif defined(LINUX)
		typedef unsigned int UINT32;
		typedef long long UINT64;
	#elif defined(VXWORKS)
		typedef short WORD;
		typedef unsigned int UINT32;
		
		#define CHADPT_BOARD_MAX               4          /* Maximum number of boards */
		#define CHADPT_PCI_MEM_AREAS           3          /* Number of PCI mem areas  */
		#define CHADPT_DMA_BUFF_ALIGN          8
		#define CHADPT_DMA_MAX_CNT             32
		#define CHADPT_PCI_VENDOR_ID          (0x17D2)
		#define CHADPT_PCI_DEVICE_ID          (0x00CA)
		
		#define CHADPT_314STRING               "M314"
		#define CHADPT_316STRING               "M316"
		#define CHADPT_320STRING               "M320"
		
		/* For AD SDI Interface  -- for M320 */
		#define SDO_M320  0x1
		#define SDIO_M320 0x2
		#define CSBn_M320 0x4
		#define SCLK_M320 0x8
		
		/* For AD SDI Interface  -- for M314 switches vSDIO and vSLCK */
		#define SDO_M314  0x1
		#define SDIO_M314 0x8
		#define CSBn_M314 0x4
		#define SCLK_M314 0x2
		
		/* For IDT part */
		#define IDT_DCLK_RD_ADR		0xD9
		#define IDT_DCLK_WR_ADR		0xD8
		
		/* Enumeration of Channel Adapter interrupts */
		/* for vxWorks Interrupt parser.             */
		typedef enum
		{
		    CHADPT_ADC_DCM_UNLOCKED   = 0,
		    CHADPT_SRAM_DCM_UNLOCKED  = 1,
		    CHADPT_ADCD_PREV_OVR      = 2,
		    CHADPT_ADCC_PREV_OVR      = 3,
		    CHADPT_ADCB_PREV_OVR      = 4,
		    CHADPT_ADCA_PREV_OVR      = 5,
		    CHADPT_ADCD_CURR_OVR      = 6,
		    CHADPT_ADCC_CURR_OVR      = 7,
		    CHADPT_ADCB_CURR_OVR      = 8,
		    CHADPT_ADCA_CURR_OVR      = 9,
		    CHADPT_ADCD_FF_FULL       = 10,
		    CHADPT_ADCC_FF_FULL       = 11,
		    CHADPT_ADCB_FF_FULL       = 12,
		    CHADPT_ADCA_FF_FULL       = 13,
		    CHADPT_TELAD_FF_FULL      = 14,
		    CHADPT_ADCD_FF_OVFLW      = 15,
		    CHADPT_ADCC_FF_OVFLW      = 16,
		    CHADPT_ADCB_FF_OVFLW      = 17,
		    CHADPT_ADCA_FF_OVFLW      = 18,
		    CHADPT_TELAD_FF_OVFLW     = 19,
		    CHADPT_DMA_GRP_DONE       = 30,
		    CHADPT_INTR_INDX_COUNT
		
		} CHADPT_INTR_INDX;
		
		
		#define V4_STAT_MASK   ( DMA_GRP_DONE      | \
		                         TELAD_FF_OVFLW    | \
		                         ADCA_FF_OVFLW     | \
		                         ADCB_FF_OVFLW     | \
		                         ADCC_FF_OVFLW     | \
		                         ADCD_FF_OVFLW     | \
		                         TELAD_FF_FULL     | \
		                         ADCA_FF_FULL      | \
		                         ADCB_FF_FULL      | \
		                         ADCC_FF_FULL      | \
		                         ADCD_FF_FULL      | \
		                         ADCA_CURR_OVR     | \
		                         ADCB_CURR_OVR     | \
		                         ADCC_CURR_OVR     | \
		                         ADCD_CURR_OVR     | \
		                         ADCA_PREV_OVR     | \
		                         ADCB_PREV_OVR     | \
		                         ADCC_PREV_OVR     | \
		                         ADCD_PREV_OVR     | \
		                         SRAM_DCM_UNLOCKED | \
		                         ADC_DCM_UNLOCKED)
	 
	#endif
#endif /* WINDRVR_H */



//Always assign 64bit data type
typedef UINT64 QWORD; 

typedef struct {
	UINT32 DMAChannels;			//0-31 
	UINT32 En64BitAdr;			

	UINT32 CurGroup[32];			//Can be used to keep track of which group is interrupting
	
	UINT32 BlockSizeB[32];		//Block Size in bytes
	UINT32 BlockCount[32];		//Number of blocks in a group -1
	UINT32 GrpCnt[32];			//Number of groups -1
	
	#if defined(X64)
		UINT32 * dVirtDMAAdr[2048];
	#else  
		UINT32	dVirtDMAAdr[2048];	
	#endif
	UINT32  dPhysDMAAdr[2048];
	UINT64	qVirtDMAAdr[2048];	
	UINT64  qPhysDMAAdr[2048];
	
	#if defined(VXWORKS)
		UINT32	dBaseDMAAdr[2048];
		UINT64  qBaseDMAAdr[2048];
	#endif /* VXWORKS */

	UINT32 Status;
} s_DMAConfig;

typedef enum {
	BRIDGE = 0,
	V4     = 2,
} BAR;

typedef enum {
	OFF		= 0,
	SYNTH	= 1,
	EXT		= 2,
} CLKSRC; 

typedef struct {
	CLKSRC ClkSrc;			//OFF, SYNTH, EXT
	double ClkFreqMHz;		//Sample clk freq in MHz
	UINT32 M316ClkConfig[3];	//M316 Clk settings

	int PriSecN;			//ADC Clk Source 0 for primary, 1 for secondary
	int SynthEn;			//1 to enable syth power, 0 to disable;
} s_ClkSettings;

typedef struct {
	int DevNum;			//Device Number on Bus, 0 is first device found
	UINT32 Bus;			//Reserved for future use
	UINT32 Slot;		//Reserved for future use
	UINT32 Function;	//Reserved for future use
	int UnitsFound;		//Number of ChannelAdapters found durring open
	
	char Asy[8];		//Assmebly information

	UINT32 RevCode;		//Rev code of bridge chip	

	s_DMAConfig DMA;	//DMA config for each channel

	UINT32 BrgIntrMask;	//Bridge interrupt mask
	
	UINT32 DevStatus;	//Status bit defined below

	UINT32 ReturnStatus;	//Holds return value from last function call

	FILE * debugFP;
	
	#if defined(VXWORKS)
		UINT32   	BAR0Ptr;		    /* Addr of BAR0  (PLX_MEM) */
		UINT32   	BAR1Ptr;		    /* Not used (IO Space)     */
		UINT32   	BAR2Ptr;		    /* Addr of BAR2  (V4)      */
		UINT32   	BAR3Ptr;		    /* Not used                */
		UINT32   	BAR4Ptr;		    /* Not used                */
	
	    /* Additions for vxWorks  */
	    UINT32      memBase[CHADPT_PCI_MEM_AREAS];
	    BOOL        init;               /* init flags        */
	    UINT32      ivec;               /* interrupt vector  */
		VOIDFUNCPTR intrHndlr[CHADPT_INTR_INDX_COUNT];
	    UINT32      fpgaIntrStatus;
	    UINT32      pciIntrStatus;
		UINT32      busNum;             /* PCI bus number */
	
	    /* */
	    UINT32       vSDO;
		UINT32       vSDIO;
		UINT32       vCSBn;
		UINT32       vSLCK;
	#endif /* VXWORKS */
} s_ChannelAdapter;


//Bit Flags for DevStatus
	#define	DEV_OPEN_OK		0x1
	#define DMA_ALLOC_OK	0x2
	 

char * Adapter_GetAPIVerInfo(UINT32 *Version); 

UINT32 Adapter_Zero(s_ChannelAdapter *pCA);

UINT32 Adapter_Open(s_ChannelAdapter *pCA);	//Opens a ChannelAdapter
UINT32 Adapter_Close(s_ChannelAdapter *pCA);	//Opens a ChannelAdapter

UINT32 Adapter_Read32(s_ChannelAdapter *pCA, BAR Bar, UINT32 Offset, UINT32 *pData);
UINT32 Adapter_Read64(s_ChannelAdapter *pCA, BAR Bar, UINT32 Offset, QWORD *pData);

UINT32 Adapter_Write32(s_ChannelAdapter *pCA, BAR Bar, UINT32 Offset, UINT32 Data);
UINT32 Adapter_Write64(s_ChannelAdapter *pCA, BAR Bar, UINT32 Offset, QWORD Data);

UINT32 Adapter_DMABufAllocate(s_ChannelAdapter *pCA);
UINT32 Adapter_DMABufFree(s_ChannelAdapter *pCA);

void Adapter_ISR(s_ChannelAdapter *pCA);

UINT32 Adapter_SampleClkSelect(s_ChannelAdapter *pCA, s_ClkSettings *pClkSettings);

UINT32 Adapter_LoadXSVF(s_ChannelAdapter *pCA, char * Filename, UINT32 PROM);

void Adapter_uSleep(UINT32 uSec);

/*Vx Specfic Functions*/
#if defined(VXWORKS)
	STATUS Adapter_GlblIntrEnBl(s_ChannelAdapter *pCA );
	STATUS Adapter_GlblIntrDsbl(s_ChannelAdapter *pCA );
	
	void   Adapter_Intr(s_ChannelAdapter *pCA );
	
	STATUS Adapter_IntrEnbl (s_ChannelAdapter *pCA, CHADPT_INTR_INDX  intrIndx);
	STATUS Adapter_IntrDsbl (s_ChannelAdapter *pCA, CHADPT_INTR_INDX  intrIndx);
	
	STATUS Adapter_IntrHdlrInstall(s_ChannelAdapter *pCA,
		                                CHADPT_INTR_INDX  intrIndx,
		                                VOIDFUNCPTR       intrHndlr);
	
	STATUS Adapter_IntrHdlrRemove( s_ChannelAdapter *pCA,
	                                    CHADPT_INTR_INDX  intrIndx);
#endif /* VXWORKS */


#ifdef __cplusplus
}
#endif

#endif





