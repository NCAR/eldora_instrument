/*
|
|       $Id$
|
|       Module: COLL_TS.s
|       Original Author: E. Loew 
|       Copywrited by the National Center for Atmospheric Research
|       Date:            $Date$
|
| revision history
| ----------------
| $Log$
|
| calling convention:
|
| description:
|
|  This is the real time time series code for the ELDORA Collator Card. 
|  It is mapped to internal ram2. Time series data from all 3 frequencies
|  and both PRT's will be acquired in this module.
 
*/
/*
 * Written by E. Loew
 * 27 May 1992
*/

#include "/ELDORA/vx/mhen/tadpole/include/ELDRP7.h"
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
*  0xA300-0xA333	Pick Gate buffer for prt#1 and prt#2
*			(3freq * 4params + 1) * 4 float bytes
*			=0x34
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

.extern start, clrfifos, in_loop, eof, fpzero, dwcont
.extern gates, samples, tsamples, temp, temp1
.extern dp_gates, dp_tgates, tnum_param, coll_pick_gate
.extern scratch, rsamples, fsamples, scale, pick_loop
.extern iv, idbZ, incP, isigma, ledl, lmask
.extern rzero, dr, prt_flag, f1_flag, f2_flag, f3_flag
.extern F1_bogus, F2_bogus, F3_bogus
.extern F1_bogus_fac, F2_bogus_fac, F3_bogus_fac, beam_cnt
.extern F1_bogus_start, F2_bogus_start, F3_bogus_start
.extern last_local_var
.extern V_const, Z_const, SIGMA_const, ncP_const 
.extern num_int1, num_int2, num_int3, num_int4
.extern tnum_int1, tnum_int2, tnum_int3, tnum_int4
.extern chip_avg1, chip_avg2, chip_avg3, chip_avg4, fchip_avg
.extern tchip_avg1, tchip_avg2, tchip_avg3, tchip_avg4, pick_indx
.extern IQ_norm, iqcf_norm, fpsamples
/* .extern chksum, chkloop, chkdone, endcode */
.global timser 

.rsect ".ram2"
.=0x0020
timser:	
						

/* Input Time Series Data -- from DSP0, PRT1  if desired */
/* NOTE: We only have time series data available from DS0 of each
 *       Doppler Processor. Therefore, the ability to pick the gate
 *       for time series data will be limited to every 8 if running
 *       two Pulse Pair Processors or every 4 if running one Pulse
 *       Pair Processor.
*/

#ifdef TIME_SERIES
	
	r3 = *ts_off
	r19 = *in_loop
	r11e = r11 + r3 /* add in time series offset */
	r18 = *tsamples

d0tsf1:	r6 = A_SENSE
	*r5 = r6
	r9e = A_PORT
	r20 = r20 - 1
	if(ne) goto d0tsf2	/* if f1_flag not set, skip f1 read in */
	r20 = *f2_flag
d0inf1I1: if(syc)goto d0inf1I1
	nop
	*r11++ = a3 = ieee(*r9)	/* Read time series data in IEEE format into ping pong buffer */
	r19 = r19 - 1
	if(eq)goto d0inf1Q1
	r19 = *in_loop
d0inf1I2: if(syc)goto d0inf1I2
	nop
	a3 = *r9		/* remove data from FIFO but don't store */
d0inf1Q1: if(syc)goto d0inf1Q1
	nop
	*r11++ = a3 = ieee(*r9)	/* Read time series data in IEEE format into ping pong buffer */
	r19 = r19 - 1
	if(eq)goto d0tsf2
	r19 = *in_loop
d0inf1Q2: if(syc)goto d0inf1Q2	
	nop
	a3 = *r9		/* remove data from FIFO but don't store */

d0tsf2:	r6 = B_SENSE
	*r5 = r6
	r9e = B_PORT
	r20 = r20 - 1
	if(ne) goto d0tsf3	/* if f2_flag not set, skip f2 read in */
	r20 = *f3_flag
d0inf2I1: if(syc)goto d0inf2I1
	nop
	*r11++ = a3 = ieee(*r9)	/* Read time series data in IEEE format into ping pong buffer */
	r19 = r19 - 1
	if(eq)goto d0inf2Q1
	r19 = *in_loop
d0inf2I2: if(syc)goto d0inf2I2
	nop
	a3 = *r9		/* remove data from FIFO but don't store */
d0inf2Q1: if(syc)goto d0inf2Q1
	nop
	*r11++ = a3 = ieee(*r9)	/* Read time series data in IEEE format into ping pong buffer */
	r19 = r19 - 1
	if(eq)goto d0tsf3
	r19 = *in_loop
d0inf2Q2: if(syc)goto d0inf2Q2	
	nop
	a3 = *r9		/* remove data from FIFO but don't store */

d0tsf3:	r6 = C_SENSE
	*r5 = r6
	r9e = C_PORT
	r20 = r20 - 1
	if(ne) goto smpt	/* if f3_flag not set, skip f3 read in */
	r20 = *f1_flag
d0inf3I1: if(syc)goto d0inf3I1
	nop
	*r11++ = a3 = ieee(*r9)	/* Read time series data in IEEE format into ping pong buffer */
	r19 = r19 - 1
	if(eq)goto d0inf3Q1
	r19 = *in_loop
d0inf3I2: if(syc)goto d0inf3I2
	nop
	a3 = *r9		/* remove data from FIFO but don't store */
d0inf3Q1: if(syc)goto d0inf3Q1
	nop
	*r11++ = a3 = ieee(*r9)	/* Read time series data in IEEE format into ping pong buffer */
	r19 = r19 - 1
	if(eq)goto smpt
	r19 = *in_loop
d0inf3Q2: if(syc)goto d0inf3Q2	
	nop
	a3 = *r9		/* remove data from FIFO but don't store */

smpt:	if(r18-->=0)goto d0tsf1
	nop
#endif TIME_SERIES

	goto dwcont
	nop

/*  end of file  */


