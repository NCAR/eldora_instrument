/**********************************************************************
 			         MCPL header
                         A portion of the MCPL library
                  Copyright 1990 Kevin J. Lynch, Ironics Inc.
 *********************************************************************/

/**
mcpl.h	
2.00    KJL	Sat Jan 20 15:14:53 EST 1990 Production Release
**/

  /**
  Type definitions
  **/

#define FIFO32		/* FifoType is a 32-bit value	*/

#ifndef TRUE
#define TRUE	1
#define FALSE	0
#endif

#ifndef BOOLEAN
#define BOOLEAN unsigned int
#endif
 
typedef unsigned long 	VmeAddress ;	  /* 32-bits of VMEbus address	*/
typedef unsigned short 	VmeAttributeType ;/* Transfer attribute		*/
typedef unsigned short 	CompStatusType ;  /* Completion Status control	*/
typedef unsigned short 	NackCountType ;   /* DTX Nack Count */
typedef unsigned short 	LinkAttributeType;/* Link attribute		*/
typedef unsigned short 	MCPLCWType ;	  /* MCPL contol word		*/

struct TPB
{
  VmeAddress	SourceAddress ;	/* Transfer source address	*/
  VmeAddress	SinkAddress ;	/* Transfer sink address	*/
  unsigned long SrcA_SnkA ;	/* Source then Sink		*/
  unsigned long	Length ;	/* Transfer length (minus one)	*/
  unsigned long CompStatus_Nacks;/* Completion Status, Nack Count*/
  VmeAddress	LinkAddress ;	/* Link Address			*/
  unsigned long LinkA_McplCW ;	/* Link Attribute, MCPL Control */
  VmeAddress	MboxAddress ;	/* TPB mailbox address		*/
} ;

/* Wait For Trigger bit in Link Attribute Word */
#define WAITFORTRIG_MASK	(LinkAttributeType)0x4000
#define WFT_FALSE		(LinkAttributeType)0x4000
#define WFT_TRUE		(LinkAttributeType)0x0000


#define LOW16_MASK	(unsigned long)0x0000FFFF /* mask for low FIFO bits */
#define HIGH16_MASK	(unsigned long)0xFFFF0000 /* mask for high FIFO bits */

#define LOADED		(BOOLEAN)TRUE	/* Value for InitTPBLoaded */
#define NOTLOADED	(BOOLEAN)FALSE	/* Value for InitTPBLoaded */

#define ENABLE_ADDR_MAPPING	(BOOLEAN)TRUE /* Map Dual Port Addr ranges */
#define DISABLE_ADDR_MAPPING	(BOOLEAN)FALSE /* Don't Map Dual Port Addrs */

/* Control Word bit fields */
#define CW_NODE_MASK 	(0x1F<<11)	/* NODE, mask */
#define CW_NODE_SHFT	11		/* NODE, shift */
#define CW_ENT_MASK	(0x1F<<6)	/* TABLE ENTRY, mask */
#define CW_ENT_SHFT	6		/* TABLE ENTRY, shift */
#define CW_COM_MASK	(0x7<<3)	/* COMMAND, mask */
#define CW_COM_SHFT	3		/* CMMAND, shift */
#define CW_BURST_MASK   (0x01<<2)	/* Burst/Lock Mask */
#define CW_BURST_SHFT	2		/* Burst/Lock shift */
#define CW_HAND_MASK	(0x01<<1)	/* Handshake Mask */
#define CW_HAND_SHFT	1		/* Handshake shift */
#define CW_CONN_MASK	(0x01<<0)	/* Connect Control Mask */
#define CW_CONN_SHFT	0		/* Connect Control shift */

/* MCPL Control Word definitions */
#define DTL	(0x0)	/* Dispatch Table Load */
#define DTX	(0x1)	/* Dispatch Table Execute */
#define RTX	(0x2)	/* Response Table Execute */
#define DTT     (0x3)	/* Dispatch Table Trigger */
#define DTD	(0x4)	/* Dispatch Table Disable (local only)*/
#define RSVD	(0x5)	/* Reserved */
#define BSIG	(0x6)	/* Broadcast Signal */
#define BDAT	(0x7)	/* Broadcast Data */

/* Transfer modes definitions */
 /* Burst/Locked */
#define TM_BURST	(0x1)		/* TM: Burst mode	*/
#define TM_LOCKED	(0x0)		/* TM: Locked mode	*/
 /* Handshake Mode */
#define TM_COMPELLED	(0x0)		/* TM: Compelled mode	*/
#define TM_NCOMPELLED	(0x1)		/* TM: NonCompelled mode*/
 /* Connect Control */
#define TM_CONN		(0x1)		/* TM: Establish Logical Connect */ 
#define TM_NOCONN	(0x0)		/* TM: Local Connect Relinquish */

/*************** Board Registers ********************/
/*                                                  */
/* 	Control Register	BoardBaseAddress+00
** 	Status Regsiter		BoardBaseAddress+02
** 	FIFO Register		BoardBaseAddress+04
** 	Start Register		BoardBaseAddress+06
*/

typedef unsigned long IV3272 ; 		/* MCPL register base address	*/
typedef unsigned short CntlRegType ; 	/* MCPL Control Register	*/
typedef unsigned short StatRegType ; 	/* MCPL Status Register		*/
#ifdef FIFO32	/* defined at front of mcpl.h 	*/
typedef unsigned long FifoType ; 	/* MCPL FIFO Register 32 bit 	*/
#else
typedef unsigned short FifoType ;	 /* MCPL FIFO Register 16 bit	*/
#endif
typedef unsigned short StartRegType ;	/* MCPL Start Register		*/

#define CNTLREGOFFSET	(IV3272)0x0
#define STATREGOFFSET	(IV3272)0x2
#define FIFOREGOFFSET	(IV3272)0x4
#define STARTREGOFFSET	(IV3272)0x6

/* Control Register field definitions */

/* Field 1: Command */
#define CR_1_RESET	(CntlRegType)0x0000	/* reset request	*/
#define CR_1_INIT_TPB	(CntlRegType)0x0001	/* init TPB loaded	*/
#define CR_1_HOLD	(CntlRegType)0x0002	/* hold request		*/
#define CR_1_TRIG	(CntlRegType)0x0003	/* trigger request	*/

#define CR_1_MASK	(CntlRegType)(0x3)	/* #1: COMMAND field mask*/
#define CR_1_SHFT	0
#define CR_1_MIN	0x0
#define CR_1_MAX	0x3

#define CR_1_DEF	CR_1_RESET<<CR_1_SHFT	/* #1: default value	*/

/* Field 2: FIFO size */
#define CR_2_D32	(CntlRegType)(0x0) /* FIFO data width 32 bits	*/
#define CR_2_D16	(CntlRegType)(0x1) /* FIFO data width 16 bits	*/

#define CR_2_MASK	(CntlRegType)(0x1<<2)	/* #2: FIFO data withd mask*/
#define CR_2_SHFT	2
#define	CR_2_MIN	0x0
#define CR_2_MAX	0x1

#ifdef FIFO32
#define CR_2_DEF	CR_2_D32<<CR_2_SHFT	/* #2: default value	*/
#else
#define CR_2_DEF	CR_2_D16<<CR_2_SHFT	/* #2: default value	*/
#endif

/* Field 3: Release Mode */

#define CR_3_BCLR	(CntlRegType)(0x1) /* release on BCLR*		*/
#define CR_3_RAB	(CntlRegType)(0x2) /* release after burst	*/
#define CR_3_LOCK	(CntlRegType)(0x3) /* lock until done		*/

#define CR_3_MASK	(CntlRegType)(0x3<<3) /* #3: release mode mask	*/
#define CR_3_SHFT	3
#define	CR_3_MIN	0x1
#define CR_3_MAX	0x3

#define CR_3_DEF	CR_3_RAB<<CR_3_SHFT	/* #3: default value	*/

/* Field 4: timeout enable */

#define CR_4_TOENA	(CntlRegType)(0x0) /* timeout enable		*/
#define CR_4_TODIS	(CntlRegType)(0x1) /* timeout disable		*/

#define CR_4_MASK	(CntlRegType)(0x1<<5)	/* #4: timeout enable mask*/
#define CR_4_SHFT	5
#define	CR_4_MIN	0x0
#define CR_4_MAX	0x1

#define CR_4_DEF	CR_4_TOENA<<CR_4_SHFT	/* #4: default value	*/

/* Field 5: timeout value */

#define CR_5_TO16	(CntlRegType)(0x0) /* timeout 16 usec	*/
#define CR_5_TO12	(CntlRegType)(0x1) /* timeout 12 usec	*/
#define CR_5_TO8	(CntlRegType)(0x2) /* timeout 8 usec	*/
#define CR_5_TO4	(CntlRegType)(0x3) /* timeout 4 usec	*/

#define CR_5_MASK	(CntlRegType)(0x3<<6)	/* #5: timeout value	*/
#define CR_5_SHFT	6
#define	CR_5_MIN	0x0
#define CR_5_MAX	0x3

#define CR_5_DEF	CR_5_TO16<<CR_5_SHFT	/* #5: default value	*/

/* Field 6: DB control byte */

#define CR_6_NULL	(CntlRegType)(0x00) /* NULL value		*/

#define CR_6_MASK	(CntlRegType)(0xFF<<8) /* #6: DB cntrl byte (db I/O)*/
#define CR_6_SHFT	8
#define	CR_6_MIN	0x00
#define CR_6_MAX	0x00

#define CR_6_DEF	CR_6_NULL<<CR_6_SHFT /* #6: default value */


/* Status Register Definitions */
/* SR bit fields */

#define SR_NODE_ADDR	(StatRegType)0xF800	/* mask for Node Addr.	*/
#define SR_DT_DISABLE	(StatRegType)0x0004	/* mask for DT disable bit*/
#define SR_WFT		(StatRegType)0x0002	/* mask for Waiting for Trig*/
#define SR_BUSY		(StatRegType)0x0001	/* mask for Busy bit	*/

/* Start Register */
#define START_CODE (StartRegType)0x1	/* start code for StartReg	*/

/* ERROR CODES */

typedef unsigned int ErrorType;

/* codes returned by the library */

#define NOERROR		(ErrorType)0x0	/* No Error			*/
#define ERANGE		(ErrorType)0x80	/* value range out of bounds 	*/
#define BAD_VALUE	(ErrorType)0x81	/* Inappropriate value used	*/
#define BAD_LINK	(ErrorType)0x82	/* Bad link found in chain eval	*/
#define BAD_EXEC_PT	(ErrorType)0x83	/* CurrentLink and Execution	*/
					/* Address of TPB mismatch	*/
#define BD_NOT_WAITING	(ErrorType)0x84	/* Board was not Waiting 	*/
					/* when execution was ordered	*/
#define UN_INIT		(ErrorType)0x85	/* Global variables un-initialized */
#define BD_NOT_BUSY	(ErrorType)0x86	/* Board is not in the BUSY State */
#define BAD_ALIGNMENT	(ErrorType)0x87	/* Bad TPB longword alignment	*/
#define NO_REPLY	(ErrorType)0x88 /* TPB never replied with mail */
#define BD_STUCK	(ErrorType)0x89 /* No Reply to HOLD or RESET */
#define RESET_OK	(ErrorType)0x8A /* Reset completed OK */

/* Codes returned by MCPL node during completion status */

/* Standard Completion Status */
#define NO_MAIL			0x0000
#define INIT_OK 		0x1FFF
#define HOLD_OK			0x2FFF
#define CLEAR_OK		0x4FFF
#define XMIT_OK			0x5FFF
#define RCV_OK			0x6FFF
#define VME_OK			0x7FFF
#define BSIG_OK 		0x8FFF
#define BSIG_RCV 		0x9FFF
#define BDAT_OK 		0xaFFF
#define BDAT_RCV 		0xbFFF

/* Selected Error Completion Codes */
#define INIT_NOT_BUSY 		0xFF0A
#define LOCAL_VME_ON_READ	0xFF32
#define LOCAL_VME_ON_WRITE_1	0xFF33
#define LOCAL_VME_ON_WRITE_2	0xFF34
#define LATE_VME_ERROR		0xFF41
#define TARGET_CRC_ERROR	0xFF43
#define MtoV_NACK		0xFF57
#define MtoV_TIMEOUT		0xFF58
#define STATUS_TIMEOUT		0xFF60
#define STATUS_TIMEOUT_1	0xFF61
#define STATUS_TIMEOUT_2	0xFF62
#define STATUS_TIMEOUT_3	0xFF63
#define STATUS_TIMEOUT_4	0xFF64
#define STATUS_TIMEOUT_5	0xFF65
#define VtoM_NACK		0xFF94
#define TARGET_VME_ERROR	0xFF95
#define INIT_CRC_ERROR 		0xFF98
#define LOCAL_VME_ERROR		0xFF9A
#define VtoM_RTX_NACK		0xFF9D
#define VtoM_TIMEOUT		0xFF9F

 /** Range Check Macro
 	return: NOERROR if min < value > max
	 	ERANGE otherwise
 **/
#define RANGE_CHK(min, max, value) \
((min > value) || (max < value)) ? ERANGE : NOERROR

 /** 
 Board Array
 **/

#define NUMOFBOARDS 4

struct BoardInfo {
	ErrorType 	GlobalsUp;
	BOOLEAN		InitTPBLoaded;
	BOOLEAN 	UseLocal;

	struct TPB 	RemoteDefault;
	struct TPB 	LocalDefault;
	struct TPB 	SysTPB;

	VmeAddress 	DualPortAddress;
	unsigned int 	DualPortLength;
	VmeAttributeType LocalAttr;
	unsigned int 	LocalAddress;

	IV3272	 	BoardAddress;
	unsigned int 	NodeAddress;
	VmeAddress	CurrentLinkAddress;
	LinkAttributeType CurrentLinkAttr;
	CntlRegType	CntlRegImage;
	CntlRegType	LastCntlReg;
};

struct BoardInfo BoardArray[NUMOFBOARDS];


/* Useful Symbols
*/

/* Handy Src/Snk Address and Attribute Symbols	*/
#define MCPLADDR	(VmeAddress)0x00000000
#define MCPLATTR	(VmeAttributeType)0x0001
#define TABLEATTR	(VmeAddress)0x0002

/* Handy Transfer Lengths			*/
#define TPB_LENGTH	(unsigned long)0x00000007

/* Timeout countdown, waiting for not BUSY after HOLD, used in InitNode
*/
#define WAIT_FOR_NOT_BUSY	10000

/* end of mcpl.h */
