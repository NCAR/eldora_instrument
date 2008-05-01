/*
|
|       $Id$
|
|       Module:COLL_MAIN.s
|       Original Author: E. Loew
|       Copywrited by the National Center for Atmospheric Research
|       Date:            $Date$
|
| revision history
| ----------------
| $Log$
 * Revision 1.2  1992/11/10  19:41:52  eric
 * removed explicit path names from #includes.
 *
 * Revision 1.1  1992/11/10  19:32:27  eric
 * Initial revision
 *
|
| calling convention:
|
| description:
|
|  This is the real time code for the ELDORA Collator Card. It sorts
|  the doppler and time series data from 3 independent frequencies,
|  performs range correction of dbZ, performs range gate averaging, and
|  converts the final results to integers. Results are placed in on board
|  DPRAM for the host 68040 to DMA at its convenience.
*/
/*
 * Written by E. Loew
 * 16 March 1992
*/

#include "ELDRP7.h"
#include "col_dpramdefs.h"
#include "dspaddr.h"	
/* the key assembler switch in the collator code */

#define ELDORA	/* "define ELDORA" or "define MHR" or "define CBAND" */
#ifdef ELDORA	/* ELDORA assembler switches */
#define INTERRUPT		        /* to vxworks */
#define no_UNIX_SIMULATOR_TESTS		/* Defines for simulation */
#define No_CONSTANTS		        /* turn on constants for Z,SNR,V,W */
#define TIME_SERIES		/* read time series from DP */
#define DP_NR_DSPS 4  		/* Number of doppler processor DSP's */
#define PPP_NR_DSPS 8		/* Number of Pulse Pair Processor DSP's */
#define NUM_PARAM   4		/* Number of doppler parameters produced */
#define no_TWO_WAIT		/* use two wait states for Amem */
#define INIT_CLR_FIFOS		/* Clear inout fifos upon dspgo */
#define GATE_SPACING  0.150	/* default gate spacing = 150m */
#endif ELDORA	/* end of ELDORA assembler switches */

/* loop index defines */

#define GATE_RATIO (PPP_NR_DSPS/DP_NR_DSPS)
/* experimental defines used during development on unix simulator ,,, */
#ifdef UNIX_SIMULATOR_TESTS
#define INPUT_BOGUS
#define FAKE_DATA
#endif UNIX_SIMULATOR_TESTS
/*
*  a DPRAM external memory A guide to the collator:
* -base address-
*  0x0000 	initialization code and constants
*  --- make sure additions to code don't pass 0Aff ---
*
*  0xB00-0x4AFF		Sort_buffer for prt#1 and prt#2
*			(4param * 1PRT * 1024gates * 4float bytes)
*			=0x4000 bytes
*  0x4B00-0x76FF  	Ping Pong Data buffer 1 (includes time series)
*			(4param * 2PRT * 512cells * 2int bytes)+
*			(3freq * 2PRT * 2 params * 64samples * 4float bytes)
*			=0x2000 + 0xC00 = 0x2C00 bytes
*  0x7700-0xA2FF	Ping Pong Data buffer 2 (includes time series)
*			(4param * 2PRT * 512cells * 2int bytes)+
*			(3freq * 2PRT * 2 params * 64samples * 4float bytes)
*			=0x2000 + 0xC00 = 0x2C00 bytes
*  0xA300-0xA33F	Pick Gate buffer for prt#1 and prt#2
*			(3freq * 4params + 3) * 4 float bytes
*			=0x3C
*  0xA350-0xA38F	Last Gate buffer for prt#1 and prt#2
*			(3freq * 4params + 3) * 4 float bytes
*			=0x3C
*  0xD000		r2table (range squared correction lookup table)
*  ram0			real time Collator Code
*/

/*
Calculate loop indeces, scale factors, and address offsets;
Initialize buffers; 
Clear FIFO buffers;
Set Sense to appropriate FIFO
Initialize Ping Pong Buffer pointers
dwell()
{
Reset all loop indeces;
Swap Ping Pong Buffers;
Input and average all f1,f2,f3 data for PRT 1 from DSP0 of DP's;
Input all f1,f2,f3 Time Series data for PRT 1 from DSP0 of DP's 
and store in Ping Pong buffer(if req'd);
Input and average all f1,f2,f3 data for PRT 1 from DSP1 of DP's;
Input and average all f1,f2,f3 data for PRT 1 from DSP2 of DP's;
Input and average all f1,f2,f3 data for PRT 1 from DSP3 of DP's;
Average range gates and range correct dbZ, for cellspacing 1 of PRT 1;
Convert doppler data to integers and store in Ping Pong Buffer;
Average range gates and range correct dbZ for cellspacing 2 of PRT 1;
Convert doppler data to integers and store in Ping Pong Buffer;
Average range gates and range correct dbZ for cellspacing 3 of PRT 1;
Convert doppler data to integers and store in Ping Pong Buffer;
Average range gates and range correct dbZ for cellspacing 4 of PRT 1;
Convert doppler data to integers and store in Ping Pong Buffer;
Re-scale "picked gate" data and range correct dbZ for PRT 1;
Convert doppler data to integers and store in Ping Pong Buffer;

Reset all loop indeces;
Input and average all f1,f2,f3 data for PRT 2 from DSP0 of DP's;
Input all f1,f2,f3 Time Series data for PRT 2 from DSP0 of DP's 
and store in Ping Pong buffer(if req'd);
Input and average all f1,f2,f3 data for PRT 2 from DSP1 of DP's;
Input and average all f1,f2,f3 data for PRT 2 from DSP2 of DP's;
Input and average all f1,f2,f3 data for PRT 2 from DSP3 of DP's;
Average range gates and range correct dbZ, for cellspacing 1 of PRT 2;
Convert doppler data to integers and store in Ping Pong Buffer;
Average range gates and range correct dbZ for cellspacing 2 of PRT 2;
Convert doppler data to integers and store in Ping Pong Buffer;
Average range gates and range correct dbZ for cellspacing 3 of PRT 2;
Convert doppler data to integers and store in Ping Pong Buffer;
Average range gates and range correct dbZ for cellspacing 4 of PRT 2;
Convert doppler data to integers and store in Ping Pong Buffer;
Re-scale "picked gate" data and range correct dbZ for PRT 2;
Convert doppler data to integers and store in Ping Pong Buffer;
Check that EOF is in correct place and interrupt BIM if it is not;
Interrupt host CPU via BIM to signify valid data;
Toggle LED;
goto dwell;
}

*/
/*
--variables and buffers--
*/
#define R2TABLE 0xD000	
#define sort_buff  0xB00	/* sort buffer base */
#define data_buff_1 0x4B00	/* ping pong data buffer 1 base */
#define data_buff_2 0x7700	/* ping pong data buffer 2 base */
#define pick_buff   0xA300	/* pick gate buffer base */
#define recip_chip_avg1 0xFFE000	/* place in RAM2 for speed */
#define recip_chip_avg2 0xFFE004	/* place in RAM2 for speed */
#define recip_chip_avg3 0xFFE008	/* place in RAM2 for speed */
#define recip_chip_avg4 0xFFE00C	/* place in RAM2 for speed */
#define lst_gt_buff 0xA350		/* last gate buffer base */
.extern start, clrfifos, in_loop, eof, fpzero, timser
.extern gates, samples, tsamples, temp, temp1
.extern dp_gates, dp_tgates, tnum_param, coll_pick_gate
.extern scratch, rsamples, fsamples, scale, pick_loop
.extern iv, idbZ, incP, isigma, ledl, lmask, pick_gate_off
.extern rzero, dr, prt_flag, f1_flag, f2_flag, f3_flag
.extern F1_bogus, F2_bogus, F3_bogus
.extern F1_bogus_fac, F2_bogus_fac, F3_bogus_fac, beam_cnt
.extern F1_bogus_start, F2_bogus_start, F3_bogus_start
.extern last_local_var, f1_sync, f2_sync, f3_sync
.extern V_const, Z_const, SIGMA_const, ncP_const 
.extern num_int1, num_int2, num_int3, num_int4
.extern tnum_int1, tnum_int2, tnum_int3, tnum_int4
.extern chip_avg1, chip_avg2, chip_avg3, chip_avg4, fchip_avg
.extern tchip_avg1, tchip_avg2, tchip_avg3, tchip_avg4, pick_indx
.extern IQ_norm, iqcf_norm, fpsamples
/* .extern chksum, chkloop, chkdone, endcode */
.global dwell, dwcont 
.global d0f1test, d0f2test, d0f3test, d0inf1, d0inf2, d0inf3
.global d1f1test, d1f2test, d1f3test, d1inf1, d1inf2, d1inf3
.global d2f1test, d2f2test, d2f3test, d2inf1, d2inf2, d2inf3
.global d3f1test, d3f2test, d3f3test, d3inf1, d3inf2, d3inf3
.global sort1, lp1, sort2, lp2, sort3, lp3, sort4, lp4
.global avg1, init1, avg2, init2, avg3, init3, avg4, init4

.rsect ".ram0"

dwell:	
						
#ifdef INPUT_BOGUS	/*  init the bogus inputs each beam ### */
	r1e = F1_bogus
	r2e = F1_bogus_start
	*r1++ = a0 = *r2++
	*r1++ = a0 = *r2++
	*r1 = a0 = *r2
#endif

	/* Toggle LED */
	r1e = *ledl
	r2e = *lmask

/* Reset all loop indeces */

	r17 = *tnum_param
	r16 = *dp_tgates
	r7 = sort_buff
	r8e = pick_buff
	r3 = *coll_pick_gate
	r1 = r1 ^ r2	/* toggle led command (must place here because of */
	*ledl = r1e	/* H/W timing problem on Collator!) */
	a0 = *r1
	r5 = fpzero
	r12e = lst_gt_buff

/* Swap Ping Pong data buffers */

	*temp = r10e
	r10e = r11
	r11e = *temp
	r15 = 0			/* use r15 to increment thru gates */
	*temp1 = r8e		/* save current pick buff offset */

/* Zero accumulators a0, a1, a2 */

	a0 = *r5
	a1 = a0
	a2 = a0
	r5e = FSENSE		/* use r5 to point to sense */
	r20 = *f1_flag

/* Input and Sort V, Sv, ncP, dbZ data from all frequencies for PRT 1 */

/* First input all data for DSP0 of all DP's */

d0f1test: r6 = A_SENSE		
	*r5 = r6 
	r9e = A_PORT
	r20 = r20 - 1
	if(ne) goto d0f2test	/* if f1_flag not set, skip f1 read in */
	r20 = *f2_flag
d0inf1:	if(syc) goto d0inf1
	nop

#ifdef INPUT_BOGUS
	r9e = F1_bogus		/* input bogus value for f1 input */
#endif INPUT_BOGUS

	*r8++ = a0 = *r9	/* input value from A_PORT for f1 */
 
d0f2test: r6 = B_SENSE
	*r5 = r6 
	r9e = B_PORT
	r20 = r20 - 1
	if(ne) goto d0f3test	/* if f2_flag not set, skip f2 read in */
	r20 = *f3_flag
d0inf2:	if(syc) goto d0inf2
	nop

#ifdef INPUT_BOGUS
	r9e = F2_bogus		/* input bogus value for f2 input */
#endif INPUT_BOGUS

	*r8++ = a1 = *r9	/* input value from B_PORT for f2 */

d0f3test: r6 = C_SENSE
	*r5 = r6  
	r9e = C_PORT
	r20 = r20 - 1
	if(ne) goto sort1	/* if f3_flag not set, skip f3 read in */
	r20 = *f1_flag
d0inf3:	if(syc) goto d0inf3
	nop

#ifdef INPUT_BOGUS
	r9e = F3_bogus		/* input bogus value for f3 input */
#endif INPUT_BOGUS

	*r8++ = a2 = *r9	/* input value from C_PORT for f3 */
	nop			/* For DAU Latency */
sort1:	a3 = a0 + a1
	*r7++ = a3 = a3 + a2	/* avg inputs from all frequencies */

/* Test if gate is picked for storage of individual frequencies */
	
	r4 = r3 - r15
	if(ne)goto lp1
	nop
	*temp1 = r8e		/* update pointer if "picked gate" */
lp1:	r8e = *temp1		/* restore pointer */
		
/* Loop on number of parameters */

	if(r17-->=0)goto d0f1test
	nop
	r15 = r15 + 4		/* increment gate count */
	r17 = *tnum_param	/* reset number of parameters */
	r7 = r7 + 0x30		/* increment sort buffer pointer */

#ifdef INPUT_BOGUS
	
	r1e = F1_bogus
	r2e = F1_bogus_fac
	*r1++ = a3 = *r1 * *r2++
	nop
	*r1++ = a3 = *r1 * *r2++
	nop
	*r1 = a3 = *r1 * *r2

#endif INPUT_BOGUS

/* Loop on number of doppler processor gates */
/* This allows us to process all gates for DSP0 of all DP's */

	if(r16-->=0)goto d0f1test
	nop
	r15 = 1			/* reset gate counter */
	r7 = sort_buff	
	*temp1 = r8e		/* save current pick gate offset into ping pong buffer */
	r16 = *dp_tgates
	r7 = r7 + 0x10

/* Input Time Series Data -- from DSP0, PRT1  if desired */
/* NOTE: We only have time series data available from DS0 of each
 *       Doppler Processor. Therefore, the ability to pick the gate
 *       for time series data will be limited to every 8 if running
 *       two Pulse Pair Processors or every 4 if running one Pulse
 *       Pair Processor.
*/

#ifdef TIME_SERIES

	goto timser
	nop
	
#endif TIME_SERIES

/* Input all data for DSP1 of all DP's */

dwcont:	r8e = *temp1 		/* restore previous pointer value */

#ifdef INPUT_BOGUS	/*  init the bogus inputs each beam ### */
	r1e = F1_bogus
	r2e = F1_bogus_start
	*r1++ = a0 = *r2++
	*r1++ = a0 = *r2++
	*r1 = a0 = *r2
#endif

d1f1test: r6 = A_SENSE 
	*r5 = r6
	r9e = A_PORT
	r20 = r20 - 1
	if(ne) goto d1f2test	/* if f1_flag not set, skip f1 read in */
	r20 = *f2_flag
d1inf1:	if(syc) goto d1inf1
	nop

#ifdef INPUT_BOGUS
	r9e = F1_bogus		/* input bogus value for f1 input */
#endif INPUT_BOGUS

	*r8++ = a0 = *r9	/* input value from A_PORT for f1 */

d1f2test: r6 = B_SENSE
	*r5 = r6
	r9e = B_PORT
	r20 = r20 - 1
	if(ne) goto d1f3test	/* if f2_flag not set, skip f2 read in */
	r20 = *f3_flag
d1inf2:	if(syc) goto d1inf2
	nop

#ifdef INPUT_BOGUS
	r9e = F2_bogus		/* input bogus value for f2 input */
#endif INPUT_BOGUS

	*r8++ = a1 = *r9	/* input value from B_PORT for f2 */

d1f3test: r6 = C_SENSE
	*r5 = r6  
	r9e = C_PORT
	r20 = r20 - 1
	if(ne) goto sort2	/* if f3_flag not set, skip f3 read in */
	r20 = *f1_flag
d1inf3:	if(syc) goto d1inf3
	nop

#ifdef INPUT_BOGUS
	r9e = F3_bogus		/* input bogus value for f3 input */
#endif INPUT_BOGUS

	*r8++ = a2 = *r9	/* input value from C_PORT for f3 */
	nop			/* For DAU Latency */	
sort2:	a3 = a0 + a1
	*r7++ = a3 = a3 + a2	/* avg inputs from all frequencies */

/* Test if gate is picked for storage of individual frequencies */
	
	r4 = r3 - r15
	if(ne)goto lp2
	nop
	*temp1 = r8e		/* update pointer if "picked gate" */
lp2:	r8e = *temp1		/* restore pointer */

/* Loop on number of parameters */

	if(r17-->=0)goto d1f1test
	nop
	r15 = r15 + 4		/* increment gate count */
	r17 = *tnum_param	/* reset number of parameters */
	r7 = r7 + 0x30		/* increment sort buffer pointer */

#ifdef INPUT_BOGUS
	
	r1e = F1_bogus
	r2e = F1_bogus_fac
	*r1++ = a3 = *r1 * *r2++
	nop
	*r1++ = a3 = *r1 * *r2++
	nop
	*r1 = a3 = *r1 * *r2

#endif INPUT_BOGUS

/* Loop on number of doppler processor gates */
/* This allows us to process all gates for DSP1 of all DP's */

	if(r16-->=0)goto d1f1test
	nop
	r15 = 2			/* reset gate counter */
	r7 = sort_buff	
	*temp1 = r8e		/* save current pick gate offset into ping pong buffer */
	r16 = *dp_tgates
	r7 = r7 + 0x20

/* Input all data for DSP2 of all DP's */

#ifdef INPUT_BOGUS	/*  init the bogus inputs each beam ### */
	r1e = F1_bogus
	r2e = F1_bogus_start
	*r1++ = a0 = *r2++
	*r1++ = a0 = *r2++
	*r1 = a0 = *r2
#endif

d2f1test: r6 = A_SENSE
	*r5 = r6
	r9e = A_PORT
	r20 = r20 - 1
	if(ne) goto d2f2test	/* if f1_flag not set, skip f1 read in */
	r20 = *f2_flag
d2inf1:	if(syc) goto d2inf1
	nop

#ifdef INPUT_BOGUS
	r9e = F1_bogus		/* input bogus value for f1 input */
#endif INPUT_BOGUS

	*r8++ = a0 = *r9	/* input value from A_PORT for f1 */

d2f2test: r6 = B_SENSE
	*r5 = r6
	r9e = B_PORT
	r20 = r20 - 1
	if(ne) goto d2f3test	/* if f2_flag not set, skip f2 read in */
	r20 = *f3_flag
d2inf2:	if(syc) goto d2inf2
	nop

#ifdef INPUT_BOGUS
	r9e = F2_bogus		/* input bogus value for f2 input */
#endif INPUT_BOGUS

	*r8++ = a1 = *r9	/* input value from B_PORT for f2 */

d2f3test: r6 = C_SENSE
	*r5 = r6
	r9e = C_PORT
	r20 = r20 - 1
	if(ne) goto sort3	/* if f3_flag not set, skip f3 read in */
	r20 = *f1_flag
d2inf3:	if(syc) goto d2inf3
	nop

#ifdef INPUT_BOGUS
	r9e = F3_bogus		/* input bogus value for f3 input */
#endif INPUT_BOGUS

	*r8++ = a2 = *r9	/* input value from C_PORT for f3 */
	nop			/* For DAU Latency */	
sort3:	a3 = a0 + a1
	*r7++ = a3 = a3 + a2	/* avg inputs from all frequencies */

/* Test if gate is picked for storage of individual frequencies */
	
	r4 = r3 - r15
	if(ne)goto lp3
	nop
	*temp1 = r8e		/* update pointer if "picked gate" */
lp3:	r8e = *temp1		/* restore pointer */

/* Loop on number of parameters */

	if(r17-->=0)goto d2f1test
	nop
	r15 = r15 + 4		/* increment gate count */
	r17 = *tnum_param	/* reset number of parameters */
	r7 = r7 + 0x30		/* increment sort buffer pointer */

#ifdef INPUT_BOGUS
	
	r1e = F1_bogus
	r2e = F1_bogus_fac
	*r1++ = a3 = *r1 * *r2++
	nop
	*r1++ = a3 = *r1 * *r2++
	nop
	*r1 = a3 = *r1 * *r2

#endif INPUT_BOGUS

/* Loop on number of doppler processor gates */
/* This allows us to process all gates for DSP2 of all DP's */

	if(r16-->=0)goto d2f1test
	nop
	r15 = 3			/* reset gate counter */
	r7 = sort_buff	
	*temp1 = r8e		/* save current pick gate offset into ping pong buffer */
	r16 = *dp_tgates
	r7 = r7 + 0x30


/* Input all data for DSP3 of all DP's */

#ifdef INPUT_BOGUS	/*  init the bogus inputs each beam ### */
	r1e = F1_bogus
	r2e = F1_bogus_start
	*r1++ = a0 = *r2++
	*r1++ = a0 = *r2++
	*r1 = a0 = *r2
#endif

d3f1test: r6 = A_SENSE
	*r5 = r6
	r9e = A_PORT
	r20 = r20 - 1
	if(ne) goto d3f2test	/* if f1_flag not set, skip f1 read in */
	r20 = *f2_flag
d3inf1:	if(syc) goto d3inf1
	nop

#ifdef INPUT_BOGUS
	r9e = F1_bogus		/* input bogus value for f1 input */
#endif INPUT_BOGUS

	*r8++ = a0 = *r9	/* input value from A_PORT for f1 */
d3f2test: r6 = B_SENSE
	*r5 = r6
	r9e = B_PORT
	r20 = r20 - 1
	if(ne) goto d3f3test	/* if f2_flag not set, skip f2 read in */
	r20 = *f3_flag
d3inf2:	if(syc) goto d3inf2
	nop

#ifdef INPUT_BOGUS
	r9e = F2_bogus		/* input bogus value for f2 input */
#endif INPUT_BOGUS

	*r8++ = a1 = *r9	/* input value from B_PORT for f2 */
d3f3test: r6 = C_SENSE
	*r5 = r6
	r9e = C_PORT
	r20 = r20 - 1
	if(ne) goto sort4	/* if f3_flag not set, skip f3 read in */
	r20 = *f1_flag
d3inf3:	if(syc) goto d3inf3
	nop

#ifdef INPUT_BOGUS
	r9e = F3_bogus		/* input bogus value for f3 input */
#endif INPUT_BOGUS

	*r8++ = a2 = *r9	/* input value from C_PORT for f3 */
	nop			/* For DAU Latency */	
sort4:	a3 = a0 + a1
	*r7++ = a3 = a3 + a2	/* avg inputs from all frequencies */

	r20 = r20 - 1
	if(ne)goto lg2
	r20 = *f2_flag
	*r12++ = a0 = a0
lg2:	r20 = r20 - 1
	if(ne) goto lg3
	r20 = *f3_flag
	*r12++ = a1 = a1
lg3:	r20 = r20 - 1
	if(ne)goto skplg
	r20 = *f1_flag
	*r12++ = a2 = a2

/* Test if gate is picked for storage of individual frequencies */
	
skplg:	r4 = r3 - r15
	if(ne)goto lp4
	nop
	*temp1 = r8e		/* update pointer if "picked gate" */
lp4:	r8e = *temp1		/* restore pointer */

/* Loop on number of parameters */

	if(r17-->=0)goto d3f1test
	nop
	r15 = r15 + 4		/* increment gate count */
	r17 = *tnum_param	/* reset number of parameters */
	r7 = r7 + 0x30		/* increment sort buffer pointer */
	r12e = lst_gt_buff	/* reset last gate pointer */

#ifdef INPUT_BOGUS
	
	r1e = F1_bogus
	r2e = F1_bogus_fac
	*r1++ = a3 = *r1 * *r2++
	nop
	*r1++ = a3 = *r1 * *r2++
	nop
	*r1 = a3 = *r1 * *r2

#endif INPUT_BOGUS

/* Loop on number of doppler processor gates */
/* This allows us to process all gates for DSP3 of all DP's */

	if(r16-->=0)goto d3f1test
	nop
	r15 = 0			/* reset gate counter */
	r7 = sort_buff	
	*temp1 = r8e		/* save current pick gate offset into ping pong buffer */
	r16 = *dp_tgates
	r1 = pir	/* clear collint if host hasn't */

/* Range Average data for PRT 1 and range correct dbZ */

	r14e = recip_chip_avg1
	r11e = *temp		/* restore ping pong buffer to base */
	r1 = iv
	r2 = isigma
	r3 = incP
	r4 = idbZ
	r6e = R2TABLE
	r16 = *tnum_int1

	
/* FOR CELL SPACING 1 */
/* Zero accumulators */

init1:	r17 = *tchip_avg1
	r5 = fpzero	
	a0 = *r5
	a1 = a0
	a2 = a0
	a3 = *r6	/* range correction for dbZ */
	
avg1:	a0 = a0 + *r7++ * *r14		/* velocity */
	a1 = a1 + *r7++ * *r14		/* spectral width */
	a2 = a2 + *r7++ * *r14		/* normalized coherent power */
	a3 = a3 + *r7++ * *r14		/* refectivity */

	if(r17-->=0)goto avg1
	r6e = r6 + 4		/* increment through r2table */
	a3 = ifalt(*r5)		/* clamp dbZ at 0 counts (-30 db ) */
	
/* Convert to integers */
	
	*r1 = a0 = int(a0)
	*r2 = a1 = int(a1)
	*r4 = a3 = int(a3)
	*r3 = a2 = int(a2)

/* Store in data in ping pong buffer */
	
	r8 = *iv
	r9 = *isigma
	r12 = *idbZ
	r13 = *incP
	r5e = r11
	r8 = r8 - -127
	*r5 = r8
	r5e = r11 + 0x2
	*r5 = r9
	r5e = r11 + 0x4
	*r5 = r12
	r5e = r11 + 0x6
	*r5 = r13
	r11e = r11 + 0x8

	if(r16-->=0)goto init1	/* test if all req'd gates have been averaged */
	nop

/* FOR CELL SPACING 2 */

	r14e = recip_chip_avg2
	r16 = *tnum_int2


/* Zero accumulators */

init2:	r17 = *tchip_avg2
	r5 = fpzero
	a0 = *r5
	a1 = a0
	a2 = a0
	a3 = *r6	/* range correction for dbZ */
	
avg2:	a0 = a0 + *r7++ * *r14		/* velocity */
	a1 = a1 + *r7++ * *r14		/* spectral width */
	a2 = a2 + *r7++ * *r14		/* normalized coherent power */
	a3 = a3 + *r7++ * *r14		/* refectivity */

	if(r17-->=0)goto avg2
	r6e = r6 + 4		/* increment through r2table */
	a3 = ifalt(*r5)		/* clamp dbZ at 0 counts (-30 db ) */
	
/* Convert to integers */

	*r1 = a0 = int(a0)
	*r2 = a1 = int(a1)
	*r4 = a3 = int(a3)
	*r3 = a2 = int(a2)

/* Store in data in ping pong buffer */

	r8 = *iv
	r9 = *isigma
	r12 = *idbZ
	r13 = *incP
	r5e = r11
	r8 = r8 - -127
	*r5 = r8
	r5e = r11 + 0x2
	*r5 = r9
	r5e = r11 + 0x4
	*r5 = r12
	r5e = r11 + 0x6
	*r5 = r13
	r11e = r11 + 0x8

	if(r16-->=0)goto init2	/* test if all req'd gates have been averaged */
	nop

/* FOR CELL SPACING 3 */

	r14e = recip_chip_avg3
	r16 = *tnum_int3


/* Zero accumulators */

init3:	r17 = *tchip_avg3
	r5 = fpzero
	a0 = *r5
	a1 = a0
	a2 = a0
	a3 = *r6	/* range correction for dbZ */
	
avg3:	a0 = a0 + *r7++ * *r14		/* velocity */
	a1 = a1 + *r7++ * *r14		/* spectral width */
	a2 = a2 + *r7++ * *r14		/* normalized coherent power */
	a3 = a3 + *r7++ * *r14		/* refectivity */

	if(r17-->=0)goto avg3
	r6e = r6 + 4		/* increment through r2table */
	a3 = ifalt(*r5)		/* clamp dbZ at 0 counts (-30 db ) */
	
/* Convert to integers */

	*r1 = a0 = int(a0)
	*r2 = a1 = int(a1)
	*r4 = a3 = int(a3)
	*r3 = a2 = int(a2)


/* Store in data in ping pong buffer */

	r8 = *iv
	r9 = *isigma
	r12 = *idbZ
	r13 = *incP
	r5e = r11
	r8 = r8 - -127
	*r5 = r8
	r5e = r11 + 0x2
	*r5 = r9
	r5e = r11 + 0x4
	*r5 = r12
	r5e = r11 + 0x6
	*r5 = r13
	r11e = r11 + 0x8

	if(r16-->=0)goto init3	/* test if all req'd gates have been averaged */
	nop

/* FOR CELL SPACING 4 */

	r14e = recip_chip_avg4
	r16 = *tnum_int4


/* Zero accumulators */

init4:  r17 = *tchip_avg4
	r5 = fpzero
	a0 = *r5
	a1 = a0
	a2 = a0
	a3 = *r6	/* range correction for dbZ */
	
avg4:	a0 = a0 + *r7++ * *r14		/* velocity */
	a1 = a1 + *r7++ * *r14		/* spectral width */
	a2 = a2 + *r7++ * *r14		/* normalized coherent power */
	a3 = a3 + *r7++ * *r14		/* refectivity */

	if(r17-->=0)goto avg4
	r6e = r6 + 4		/* increment through r2table */
	a3 = ifalt(*r5)		/* clamp dbZ at 0 counts (-30 db ) */
	
/* Convert to integers */

	*r1 = a0 = int(a0)
	*r2 = a1 = int(a1)
	*r4 = a3 = int(a3)
	*r3 = a2 = int(a2)


/* Store data in ping pong buffer */

	r8 = *iv
	r9 = *isigma
	r12 = *idbZ
	r13 = *incP
	r5e = r11
	r8 = r8 - -127
	*r5 = r8
	r5e = r11 + 0x2
	*r5 = r9
	r5e = r11 + 0x4
	*r5 = r12
	r5e = r11 + 0x6
	*r5 = r13
	r11e = r11 + 0x8

	if(r16-->=0)goto init4	/* test if all req'd gates have been averaged */
	nop

/* Re-scale "last gate" data, correct dbZ for range**2, and convert to int */

	r16 = *pick_loop	/* r16 is loop counter for picked gate params */
	r7e = lst_gt_buff
	r19 = *pick_indx
	r6e = r6 - 4		/* re-use correction for last cell */	
	r14 = scale
	r21 = 0
last:	r5 = fpzero
	a0 = *r7 * *r14		/* re-scale velocity */
	r7e = r7 + r19		/* index thru last gate buff */
	a1 = *r7 * *r14		/* re-scale sigma */
	r7e = r7 + r19		/* index thru last gate buff */
	a2 = *r7 * *r14		/* re-scale ncP */
	r7e = r7 + r19		/* index thru last gate buff */
	a3 = *r7 * *r14		/* re-scale dbZ */
	r7e = lst_gt_buff	/* return to last gate buff base */
	a3 = a3	+ *r6		/* range correct dbZ */
	a3 = ifalt(*r5)		/* clamp dbZ at 0 counts (-30 db ) */
	nop			/* for DAU Latency */

/* Convert to integers */

	*r1 = a0 = int(a0)
	*r2 = a1 = int(a1)
	*r4 = a3 = int(a3)
	*r3 = a2 = int(a2)


/* Store data in ping pong buffer */

	r8 = *iv
	r9 = *isigma
	r12 = *idbZ
	r13 = *incP
	r5e = r11
	r8 = r8 - -127
	*r5 = r8
	r5e = r11 + 0x2
	*r5 = r9
	r5e = r11 + 0x4
	*r5 = r12
	r5e = r11 + 0x6
	*r5 = r13
	r11e = r11 + 0x8
	r21 = r21 + 4
	if(r16-->=0)goto last
	r7e = r7 + r21 	/* point to start of next frequency in last gate buff */
/* Re-scale "picked gate" data, correct dbZ for range**2, and convert to int */

	r16 = *pick_loop	/* r16 is loop counter for picked gate params */
	r11e = *temp		/* restore buffer to base */
	r7e = pick_buff
	r19 = *pick_gate_off
	r17 = *coll_pick_gate
	r11e = r11 + r19	/* point to correct place in ping pong buffer */
	r19 = *pick_indx
	r6e = R2TABLE	
	r17 = r17 * 2		/* this instruction and the following can be */
	r17 = r17 * 2		/* moved to coll_init.s if req'd for speed */
	r14 = scale
	r6e = r6 + r17	
	r21 = 0
rescl:	r5 = fpzero
	a0 = *r7 * *r14		/* re-scale velocity */
	r7e = r7 + r19		/* index thru pick buff */
	a1 = *r7 * *r14		/* re-scale sigma */
	r7e = r7 + r19		/* index thru pick buff */
	a2 = *r7 * *r14		/* re-scale ncP */
	r7e = r7 + r19		/* index thru pick buff */
	a3 = *r7 * *r14		/* re-scale dbZ */
	r7e = pick_buff		/* return to pick buff base */
	a3 = a3	+ *r6		/* range correct dbZ */
	a3 = ifalt(*r5)		/* clamp dbZ at 0 counts (-30 db ) */
	nop			/* for DAU Latency */

/* Convert to integers */

	*r1 = a0 = int(a0)
	*r2 = a1 = int(a1)
	*r4 = a3 = int(a3)
	*r3 = a2 = int(a2)


/* Store data in ping pong buffer */

	r8 = *iv
	r9 = *isigma
	r12 = *idbZ
	r13 = *incP
	r5e = r11
	r8 = r8 - -127
	*r5 = r8
	r5e = r11 + 0x2
	*r5 = r9
	r5e = r11 + 0x4
	*r5 = r12
	r5e = r11 + 0x6
	*r5 = r13
	r11e = r11 + 0x8
	r21 = r21 + 4
	if(r16-->=0)goto rescl
	r7e = r7 + r21 	/* point to start of next frequency in pick_buff */

/* Input and Sort V, Sv, ncP, dbZ data from all frequencies for PRT 2, if req'd */

/* Range Average data for PRT 2 if req'd and range correct dbZ */

/* Read in EOF's from Doppler processors */
	
	r5 = fpzero
	a0 = *r5	/* zero accumulators */
	a1 = a0
	a2 = a0
	r2 = eof
	r5e = FSENSE
	r6 = A_SENSE
eoff1:	*r5 = r6
	r20 = *f1_flag
	r9e = A_PORT
	r20 = r20 - 1
	if(ne) goto eoff2	/* if f1_flag not set, skip f1 read in */
	r20 = *f2_flag
ineof1:	if(syc)goto ineof1
	nop
	a0 = *r9
	a0 = a0 - *r2
eoff2: 	r6 = B_SENSE
	*r5 = r6
	r9e = B_PORT
	r20 = r20 - 1
	if(ne) goto eoff3	/* if f2_flag not set, skip f2 read in */
	r20 = *f3_flag
ineof2:	if(syc)goto ineof2	
	nop
	a1 = *r9
	a1 = a1 - *r2

eoff3: 	r6 = C_SENSE
	*r5 = r6
	r9e = C_PORT
	r20 = r20 - 1
	if(ne) goto eoftst	/* if f3_flag not set, skip f3 read in */
	r20 = *f1_flag
ineof3:	if(syc)goto ineof3	/* should change sense before here in future! */
	nop
	a2 = *r9
	a2 = a2 - *r2
	nop			/* For DAU Latency */
eoftst:	a3 = a0 + a1
	a3 = a3 + a2

/* Interrupt Host CPU */

	r11e = *temp	/* reset ping pong buffer to base */
	pir = r1	/* interrupt host via BIM */
	nop		/* For DAU latency */	
/* Check for EOF: (placed here for DAU latency) */
/* if not received interrupt Collator's BIM */

	if(aeq)goto skpint
	nop
	pdr = r8	/* interrupt BIM via pdr */
	r1 = f1_sync	/* store EOF test results for each freq */
	*r1 = a0 = a0
	r1 = f2_sync
	*r1 = a1 = a1
	r1 = f3_sync
	*r1 = a2 = a2
/*
errint: goto errint    if Collator out of sync sit in infinite loop
	nop
*/
/* Begin new dwell */

skpint:	goto dwell
	nop
/*  end of file  */



