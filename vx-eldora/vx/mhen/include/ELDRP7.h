/*
 *	$Id$
 *
 *	Module: ELDRP7		 
 *	Original Author: Shawn Copeland/Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:   	 $Date$
 *
 * revision history
 * ----------------
 *   12/11/90  Added MAXCHIPS, MAXPCP  (SBC)
 *   02/21/91  Changed ATOD0BASE to 0x600000 (SBC)
 *   08/21/91  Added DSPQ and Collator definitions
 *
 * $Log$
 *
 *
 * description:
 *
 *  Include file with ELDORA RP7 parameters.        
 */
#ifndef INCELDRP7
#define INCELDRP7

/* General purpose constants */
#define MAXRADARS  5  /* Maximum number of "radars" currently implemented */
#define MAXPCP   255  /* Maximum number of PCP's in a repetition interval */
#define MAXGATES 4094 /* Maximum number of gates possible */

/* Base address of cards */

#define TIMBASE    (0x100000)
#define ATOD0BASE  (0x600000)
#define DSP0BASE   (0x400000)
#define COL0BASE   (0x300000)

/* Timing Module Address offsets */

#define TIMGATE0   (0x00000)
#define TIMGATE1   (0x02000)
#define TIMGATE2   (0x04000)
#define TIMGATE3   (0x06000)
#define TIMGATE4   (0x08000)
#define TIMGATE5   (0x0a000)
#define TIMGATE6   (0x0c000)
#define TIMGATE7   (0x0e000)
#define TIMGATE8   (0x10000)
#define TIMGATE9   (0x12000)
#define TIMPCPN    (0x14000)
#define TIMBLANK   (0x16000)
#define TIMCHIP    (0x18000)
#define TIMON      (0x1a000)
#define TIMOFF     (0x1c000)

/* A to D Board Address offsets */

#define ATOD_QH    (0x00000)
#define ATOD_QL    (0x04000)
#define ATOD_IH    (0x08000)
#define ATOD_IL    (0x0c000)
#define ATOD_CH    (0x10000)  /* base address of upper 64k of choice ram */
#define ATOD_CL    (0x20000)  /* base address of lower 64k of choice ram */
#define ATOD_Z     (0x30000)
#define ATOD_DC    (0x38000)
#define ATOD_M     (0x38800)
#define ATOD_IHSDB (0x39000)
#define ATOD_QHSDB (0x39800)
#define ATOD_ZHSDB (0x3a000)

/*************************************************************************
 *                                                                       *
 *           DSPQ and Collator Boards common Address offsets             *
 *                                                                       *
 *************************************************************************
 *************************************************************************/

#define	A_PORT	0xFFDF04
#define	B_PORT	0xFFDF08
#define	C_PORT	0xFFDF10
#define	A_DSY	0xFFDF20
#define	B_DSY	0xFFDF40
#define	C_DSY	0xFFDF80
#define	FIFOCLR	0xFFDF00
#define A_SH1	0xFFDF24
#define B_SH1	0xFFDF48
#define C_SH1	0xFFDF90
/*
VME offsets from board address for access to
DSP's on the board (only DSP0 on Collator; DSP0,1,2,3 on Quad DSP)
*/
#define		PIO	0x10000 /* Same as DSPSEL			*/
#define		DSPOFF	0x4000 	/* DSP offset incr from board offset    */
#define		DSPSEL	0x10000 /* DSP board offset			*/
#define		DSP0SEL	0x10000 /* DSP0 select (only choice on Collator)*/
#define		DSP1SEL	0x14000 /* DSP1 select (Quad DSP only)          */
#define		DSP2SEL	0x18000	/* DSP2 select (Quad DSP only)		*/
#define		DSP3SEL	0x1C000 /* DSP3 select (Quad DSP only)		*/     
#define		LTCHSEL 0x18000	/* Parameter latch select 		*/
#define		DAISY	0x00000	/* Daisy-chain enable latch		*/
#define         DLEDON  0x4000  /* Turns DSPQB test LED on              */
#define         DLEDOFF 0x8000  /* Turns DSPQB test LED off             */

/************************************************************************
 ************************************************************************/

/* DSPQ Board Address offsets */

#define	DRDFLG	0xFFDF1C
#define CLRINT1 0xFFDF0C
#define CLRINT2 0xFFDF14
#define INTOUT2 0xFFDF18

/* Collator Board Address offsets */

#define	CRDLTCH	0xFFDE00
#define	CRDRFLG	0xFFDE10
#define	CDAC	0xFFDE30
#define	CLEDON	0xFFDE60
#define	CLEDOFF	0xFFDE70
#define	FSENSE	0xFFDF80
#define	A_SENSE	0
#define	B_SENSE	1
#define	C_SENSE	2
#define	ALLSENSE 3
#define	DPRAM	0x00000  /* Dual-ported RAM on Collator board	*/
#define	BIMSEL	0x14000	 /* Bus Interrupter Module on Collator	*/

/* Collator BIM's Internal Registers */

#define cntrl_reg0 0x1   /* BIM Control register 0 */
#define cntrl_reg1 0x3   /* BIM Control register 1 */
#define cntrl_reg2 0x5   /* BIM Control register 2 */
#define cntrl_reg3 0x7   /* BIM Control register 3 */
#define vec_reg0 0x9     /* BIM Vector Register 0 */
#define vec_reg1 0xb     /* BIM Vector Register 1 */
#define vec_reg2 0xd     /* BIM Vector Register 2 */
#define vec_reg3 0xf     /* BIM Vector Register 3 */

/**************************************************************************
 *           DSPQ and Collator common DSP32C Address offsets              *
 **************************************************************************
 **************************************************************************/

/*
Parallel IO offsets for PIO mode 1 -- 8 bit transfers, DSP32 emulation ONLY!!
PCR[9]=0,PCR[1]=0
*/
#define		CPARL	0x01	/* Parallel Address Register -lo byte 	 */
#define		CPARH	0x11 /*				     -hi byte 	 */  
#define		CPDRL	0x21	/* Parallel Data Register    -lo byte 	 */
#define		CPDRH	0x31	/*	       		     -hi byte 	 */
#define		CEMRL	0x41	/* Error Mask Register	     -lo byte	 */
#define		CEMRH	0x51	/*     	    		     -hi byte 	 */    
#define		CESR	0x61	/* Error Status Register                 */
#define		CPCRL	0x71	/* Parallel Control Register -lo byte	 */
#define		CPIRL	0x81	/* Parallel Interrupt Reg    -hi byte  	 */   
 
/*
Parallel IO offsets for PIO 8 bit transfer mode.
PCR[9]=0,PCR[1]=1
*/

#define		PARL	0x01	/* Parallel Address Register-lo byte 	*/
#define		PARH	0x11    /*			    -hi byte 	*/     
#define		PDRL	0x21	/* Parallel Data Register   -lo byte 	*/
#define		PDRH	0x31	/*			    -hi byte 	*/
#define		EMRL	0x41	/* Error Mask Register	    -lo byte	*/
#define		EMRH	0x51	/* 			    -hi byte 	*/     
#define		ESR	0x61	/* Error Status Register		*/
#define		PCRL	0x71	/* Parallel Control Register-lo byte	*/
#define		PIRL	0x81	/* Parallel Interrupt Reg   -lo byte	*/     
#define		PIRH	0x91	/* 			    -hi byte	*/
#define		PCRH	0xA1	/* Parallel Control Register-hi byte	*/
#define		PARE	0xB1	/* Parallel Address Register-Extended	*/
#define		PDR2L	0xC1	/* Parallel Data Register 2 -lo byte	*/
#define		PDR2H	0xD1	/* 			    -hi byte	*/
 
/*
Parallel IO offsets for PIO 16 bit transfer mode.
PCR[9] = 1,PCR[1]=1
*/
 
#define		PAR16	0x00	/* PAR					*/
#define		PDR16	0x20	/* PDR					*/
#define	        EMR16	0x40	/* EMR					*/
#define		ESR16	0x60	/* ESR					*/
#define		PCR16	0x70	/* PCR					*/
#define		PIR16	0x80	/* PIR					*/
#define		PARE16	0xB0	/* PARE					*/
#define		PDR216	0xC0	/* PDR2					*/

/*  
PCR bit definitions
*/
 
#define		RUN	0x01	/* Set DSP run flag 			*/
#define		DSPRESET 0x00	/* DSP reset				*/
#define		REGMAP	0x02	/* DSP32C port mapping select		*/
#define		ENI   	0x04	/* DSP interrupt flag enable		*/
#define		DMA	0x08	/* DSP DMA enable flag			*/
#define		AUTO	0x10	/* DSP DMA autoincrement flag		*/
#define		PDF	0x20	/* Parallel Data Flag			*/
#define		PIF	0x40	/* Parallel Interrupt Flag		*/
#define		RES	0x80	/* Reserved				*/
#define		DMA32	0x01	/* 32-bit DMA enable			*/
#define		PIO16	0x02	/* 16-bit PIO enable			*/
#define		FLGEDG	0x04	/* FLG edge (See DSP32C Manual)		*/
 
/************************************************************************
*************************************************************************/ 
#endif /* INCELDRP7 */









