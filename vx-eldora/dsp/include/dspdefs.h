/*
 *	$Id$
 *
 *	Module:	DSPDEFS	 
 *	Original Author: Grant Gray
 *      Copywrited by the National Center for Atmospheric Research
 *	Date: 	       $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:
 *
 *	General RP7 DSP/Collator board definitions
 *	8/5/89 
 *	Added:
 *		ISIM and QSIM; which simulate I and Q outputs, respectively,
 *		when addressed.
 *        
 */
#ifndef INCdspdefs
#define INCdspdefs

/*
Parallel IO offsets for PIO mode 1 -- 8 bit transfers, DSP32 emulation.
PCR[9]=0,PCR[1]=0
*/
#define		CPARL	0x01	/* Parallel Address Register -lo byte 	 */
#define		CPARH	0x11 /*					    -hi byte 	 */ 	
#define		CPDRL	0x21	/* Parallel Data Register    -lo byte 	 */
#define		CPDRH	0x31	/*			    		    -hi byte 	 */
#define		CEMRL	0x41	/* Error Mask Register	    -lo byte	 */
#define		CEMRH	0x51	/* 			    		    -hi byte 	 */	
#define		CESR		0x61	/* Error Status Register				 */
#define		CPCRL	0x71	/* Parallel Control Register -lo byte	 */
#define		CPIRL	0x81	/* Parallel Interrupt Reg    -hi byte  	 */	
 
/*
Parallel IO offsets for PIO mode 2 -- 8 bit transfers.
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
Parallel IO offsets for PIO mode 3 -- 16 bit transfers.
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
#define		DPRAM	0x00000 /* Dual-ported RAM on Collator board	*/
#define		BIMSEL	0x14000	/* Bus Interrupter Module on Collator	*/
#define		LTCHSEL 0x18000	/* Parameter latch select 		*/
 
/*
A/D and MTG board definitions
*/
/* A/D board */ 
#define		I_LO	0x00000 /* I channel low range			*/
#define		I_HI	0x04000 /* I channel hi  range			*/
#define		Q_LO	0x08000 /* Q	     low			*/
#define		Q_HI	0x0C000 /* Q         hi				*/
#define		ISIM	0x39000 /* I simulated from VME			*/
#define		QSIM	0x39800 /* Q simulated from VME			*/
#define		ZSIM	0x3A000 /* Z simulated from VME			*/
#define		CHOICE	0x10000 /* Choice RAM				*/
#define		THRESH	0X20000 /* Threshold latch for choice RAM 	*/

/* MTG board */ 
#define		GCNT	0x10000 /* Gate count latch			*/
#define		DR	0x00002 /* Start of gate spacing counts memory	*/
#define		R0	0x00001 /* Range to first gate (counts)		*/
#define		PRF	0x18000 /* PRF simulation when on hold		*/
/* #define		HOLD	0x14000 Holdoff latch			*/
 
/*
DSPQ and Collator board definitions
*/
 
#define		DAISY	0x00000	/* Daisy-chain enable latch		*/
 

#endif /* INCdspdefs */
