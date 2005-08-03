/*
|
|       $Id$
|
|       Module: COLL_INIT.s
|       Original Author: E. Loew 
|       Copywrited by the National Center for Atmospheric Research
|       Date:            $Date$
|
| revision history
| ----------------
| $Log$
 * Revision 1.1  1992/11/10  19:32:21  eric
 * Initial revision
 *
|
| calling convention:
|
| description:
|
| 
|  This code initializes the ELDORA Collator with the information required
|  for unsorting and averaging doppler data for up to 3 frequencies
|
|        
|
| 
*/       

/* 
 * Originally Written by E. Loew
 * 14 March 1992
*/

#include "ELDRP7.h"
#include "coll_dpramdefs.h"
#include "dspaddr.h"	

/* the key assembler switch in the collator code */

#define ELDORA	/* "define ELDORA" or "define MHR" or "define CBAND" */
#ifdef ELDORA	/* ELDORA assembler switches */
#define INTERRUPT		        /* to vxworks */
#define no_UNIX_SIMULATOR_TESTS		/* Defines for simulation */
#define No_CONSTANTS		        /* turn on constants for Z,SNR,V,W */
#define no_CHKSUM			/* calculate code checksum */
#define TIME_SERIES		/* read time series from DP */
#define R2TABLE_MAX_GATES 1800 	/*presently 1800 entries in range-squared table*/
#define DP_NR_DSPS 4  		/* Number of doppler processor DSP's */
#define PPP_NR_DSPS 8		/* Number of Pulse Pair Processor DSP's */
#define NUM_PARAM   4		/* Number of doppler parameters produced */
#define TWO_WAITS		/* use one wait state for Amem */
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
*  0xB00-0x4Aff		Sort_buffer for prt#1 and prt#2
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
*  0xA300-0xA333	Pick Gate Buffer
*			(3freq * 4 params + 1) * 4float bytes
*			=0x34 bytes
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

.global start, clrfifos, in_loop, eof
.global gates, samples, tsamples, temp, temp1
.global dp_gates, dp_tgates, tnum_param, num_param
.global scratch, rsamples, fsamples, scale, pick_loop
.global iv, idbZ, incP, isigma, fpzero
.global rzero, dr, prt_flag, f1_flag, f2_flag, f3_flag
.global pick_gate_off, ts_off, f1_sync, f2_sync, f3_sync
.global F1_bogus, F2_bogus, F3_bogus
.global F1_bogus_fac, F2_bogus_fac, F3_bogus_fac, beam_cnt
.global F1_bogus_start, F2_bogus_start, F3_bogus_start, f_flag, pick_indx
.global coll_pick_gate, last_local_var, ledl, lmask
.global V_const, Z_const, SIGMA_const, ncP_const 
.global num_int1, num_int2, num_int3, num_int4
.global tnum_int1, tnum_int2, tnum_int3, tnum_int4
.global chip_avg1, chip_avg2, chip_avg3, chip_avg4, fchip_avg
.global tchip_avg1, tchip_avg2, tchip_avg3, tchip_avg4
.global IQ_norm, iqcf_norm, fpsamples
.extern dwell
/* .global chksum, chkloop, chkdone, endcode */

.rsect ".extA"
#ifdef TWO_WAITS
                r10 = 0x1B              /* 2 Waits for A(DPRAM), 3 for B(i/o)*/
#else
                r10 = 0x0B              /* 1 Wait for A, 3 for B */
#endif 
	  pcw = r10
          ioc = 0x0554
          dauc = 0
          goto start

.=0x0050

gates:    int 64	/* typically 64 for 8 PPP DSP's and 4 DP DSP's */
samples:  int 20	/* typically 16, 20, or 32 */
num_int1: int 509	/* default for 512 gates with no rgate avg */
num_int2: int 1
num_int3: int 1
num_int4: int 1
chip_avg1: int 1	/* default = 1; don't average */
chip_avg2: int 1	/* default = 1; don't average */
chip_avg3: int 1	/* default = 1; don't average */
chip_avg4: int 1	/* default = 1; don't average */
.align 4
.=DSP_BEAM_CNT
beam_cnt:	int24 0

.=DSP_CHKSUM
chksum:		int24 0	/* code checksum from start to endcode */

.=0x00F0

/* Pointers to various things */

.=COL_RZERO
rzero:         float 0.0	/* ### range to first gate in km */
/*.=COL_GATE_SPACE */
dr:            float GATE_SPACING /* gate spacing in km */
/* .=COL_RADAR_CONST */
/* cradar = 20 log kz + 20 log krcvr  + 2*zoffset ### */
/* the above calculation should be done in VXWORKS and the result poked in */
/* to cradar to give correct weather reflectivities */
/* Example for MHR:   cradar = 2*(70) + 20*(-5.56) + 2*(30) = 89.7  */
/* We assumed the radar constant is 70, the receiver+RP7 square-law slope */
/* about 2.6e-5 mw per normalized power unit, */
/* and the zoffset for output is 30 dB (below 0dB) */
/* COL_TS_RAW_FACT */
IQ_norm:	float 1.0/2048.0 /* norm fac for raw quad samples,,,*/
/* COL_TS_FILT_FACT */
iqcf_norm:	float 1.0 /*norm factor for icf,qcf from quads, default=1.0*/
/* .=COL_PICK_GATE */
coll_pick_gate:  int 32   /* default for gate to transmit data of all freq */
			    /* NOTE: 0=< coll_pick_gate <=dp_gates */ 
/* .=COL_PRT_FLAG */
prt_flag:  int 0      /* default = 0 i.e. no staggered PRT operation */
/* .=COL_F1_FLAG */
f1_flag:  int 1	/* default: freq 1 enabled */
/* .=COL_F2_FLAG */
f2_flag:  int 0	/* default: freq 2 disabled */
/* .=COL_F3_FLAG */
f3_flag:  int 0	/* default: freq 3 disabled */
.align 4
/* .=COL_F1_SYNC */
f1_sync: float 0.0
/* .=COL_F2_SYNC */
f2_sync: float 0.0
/* .=COL_F3_SYNC */
f3_sync: float 0.0

.=0x200

start:

#ifdef CHKSUM
 	r2e = start	/* ### calculate code checksum */
	r18e = endcode + 4
	r18e = r18 - r2
	r18e = r18/2
	r18e = r18/2	/* get 32bitword distance */
	r3e = 0
chkloop: r4e = *r2++
	nop
	r3e = r3 + r4
        if(r18-->=0)goto chkloop   
	nop
chkdone: r2e = chksum
	 *r2 = r3	/* put checksum into trailer */
#endif CHKSUM

	r1e = FIFOCLR        /* FIFO clear loc */

        r2e = CLEDON     /* Get collator LED location */
        *ledl = r2e      /* Set up led condition location */

#ifdef INIT_CLR_FIFOS
clrfifos: a0 = *r1            /* Clear the FIFO's */
#else
clrfifos: nop           /* Dont Clear the FIFO's upon software initialization*/
#endif INIT_CLR_FIFOS

		r1e = FSENSE		/* Set sync sense to B_PORT */
		r2 = B_SENSE
		*r1 = r2    
        	r8 = range
        	r9 = rzero
        	r18 = R2TABLE_MAX_GATES /* ### was   *gates  */
        	*r8 = a0 = *r9          /* Initialize r to r0 */
        	r13e = R2TABLE
        	r18 = r18 - 2
r2loop:
               	call _log10(r14)        /* Compute log10 of r */
               	nop
       int24     locall, range, r2corr
.align 4
               	r6 = r2corr
              	r7 = dr
               	r8 = range
               	r9 = f40		/* 2/16/90 grg; was f20 */
		/* because Ze-counts are 0.5 dB/count */

               	r10 = f1
               	*r13++ = a0 = *r9 * *r6 /* 20 log10(r) --> table */
               	*r8 = a1 = *r8 + *r7    /* r = r + dr */
r2end:          if(r18 -->=0)goto r2loop
               	nop
      
	 	r21e = 0	/* collator beam counter */

/* Calculate loop indeces for main loop */

		r2 = in_loop	/* in_loop -> PPP_NR_DSPS/DP_NR_DSPS */
		a0 = float(*r2)
		r2 = gates
		a1 = float(*r2)
		r8 = *samples
		r2e = dp_gates
		a2 = a0 * a1
		*r2 = a2 = int(a2)	/* store dp_gates */
		r1 = *num_int1
		r2 = *num_int2
		r3 = *num_int3
		r4 = *num_int4
		r1 = r1 - 2
		*tnum_int1 = r1		/* *tnum_int1 = *num_int1 - 2 */
		r2 = r2 - 2
		*tnum_int2 = r2		/* *tnum_int2 = *num_int2 - 2 */
		r3 = r3 - 2
		*tnum_int3 = r3		/* *tnum_int3 = *num_int3 - 2 */
		r4 = r4 - 2
		*tnum_int4 = r4		/* *tnum_int4 = *num_int4 - 2 */
		r5 = *dp_gates
		r6 = *num_param
		r1 = *chip_avg1
		r2 = *chip_avg2
		r3 = *chip_avg3
		r4 = *chip_avg4
		r5 = r5 - 2
		*dp_tgates = r5		/* *dp_tgates = *dp_gates - 2 */
		r6 = r6 - 2
		*tnum_param = r6	/* *tnum_param = *tnum_param - 2 */
		r1 = r1 - 2
		*tchip_avg1 = r1	/* *tchip_avg1 = *chip_avg1 - 2 */
		r2 = r2 - 2
		*tchip_avg2 = r2	/* *tchip_avg2 = *chip_avg2 - 2 */
		r3 = r3 - 2
		*tchip_avg3 = r3	/* *tchip_avg3 = *chip_avg3 - 2 */
		r4 = r4 - 2
		*tchip_avg4 = r4	/* *tchip_avg4 = *chip_avg4 - 2 */
		r8 = r8 - 2
		*tsamples = r8		/* *tsamples = *samples - 2 */
/* zero out sort buffer */

		a0 = a0 - a0
		r15 = *dp_tgates
		r1 = sort_buff
cloop:		do 0,3
		*r1++ = a0 = a0
		if (r15-->=0)goto cloop
		nop

/* 
   zero out entire Ping Pong Buffers since it's easier than calculating 
   the number of cells computed plus any time series
*/

		a0 = a0 - a0
		r1 = data_buff_1
		r2e = data_buff_2
		do 1,2047
		*r1++ = a0 = a0
		*r2++ = a0 = a0
		do 1,767
		*r1++ = a0 = a0
		*r2++ = a0 = a0
		
/* calculate the reciprocal of range gate averaging factors */

		r1 = chip_avg1
		r2 = fchip_avg
		a0 = float(*r1)
		*r2 = a0 = a0
		3*nop
		call _invf (r14)
		nop
	int24 fchip_avg, recip_chip_avg1
.align 4		
		r1 = chip_avg2
		r2 = fchip_avg
		a0 = float(*r1)
		*r2 = a0 = a0
		3*nop
		call _invf (r14)
		nop
	int24 fchip_avg, recip_chip_avg2
.align 4		
		r1 = chip_avg3
		r2 = fchip_avg
		a0 = float(*r1)
		*r2 = a0 = a0
		3*nop
		call _invf (r14)
		nop
	int24 fchip_avg, recip_chip_avg3
.align 4		
		r1 = chip_avg4
		r2 = fchip_avg
		a0 = float(*r1)
		*r2 = a0 = a0
		3*nop
		call _invf (r14)
		nop
	int24 fchip_avg, recip_chip_avg4
.align 4		

/* Calculate pick gate loop counter and scale factor */

	r1 = *f1_flag
	r2 = *f2_flag
	r3 = *f3_flag
	r4 = pick_loop
	r5 = scale
	r6 = r1 + r2
	r6 = r6 + r3	
	*pick_loop = r6	/* *pick_loop = *f1_flag + *f2_flag + *f3_flag */
	r6 = *pick_loop
	a0 = float(*r4)
	*r5 = a0 = a0	/* *scale = float(*f1_flag + *f2_flag + *f3_flag) */
	r6 = r6 - 2	
	*pick_loop = r6	/* *pick_loop = *pick_loop - 2 */

/* Calculate pick_gate offset into Ping Pong Buffers */
/* pick_gate_off = no. PRT's * num_param * 2bytes (*num_int1 + *num_int2 + *num_int3 + *num_int4 + *pick_loop + 2) */

	r5 = num_int1
	a0 = float(*r5)
	r5 = num_int2	
	a1 = float(*r5)
	a1 = a1 + a0
	r5 = num_int3	
	a0 = float(*r5)
	2*nop			/* for DAU latency */
	a1 = a1 + a0
	r5 = num_int4	
	a0 = float(*r5)
	2*nop			/* for DAU latency */
	a1 = a1 + a0
	r1 = fptwo
	r2 = num_param
	r6 = pick_loop
	a0 = float(*r2)
	a3 = float(*r6)
	2*nop			/* for DAU latency */
	a3 = a3 + *r1
	a1 = a3 + a1
	a2 = a0 * *r1
	r13 = *prt_flag
	r5 = pick_gate_off
	a1 = a1 * a2
	r13 = r13 - 1
	if(ne)goto nomult	/* for staggered PRT's double offset */
	nop
	a1 = a1 * *r1
nomult:	*r5 = a1 = int(a1)

/* Calculate time series offset into Ping Pong Buffers */
/* ts_off = *pick_gate_off + no. PRT's * num_param * 2bytes *(*f1_flag + *f2_flag + *f3_flag) */

	r2 = fptwo
	r6 = ts_off
	r3 = *f1_flag
	r4 = *f2_flag
	r5 = *f3_flag
	r3 = r3 + r4
	r3 = r3 + r5
	*f_flag = r3
	r3 = f_flag
	a0 = float(*r3)
	2*nop			/* for DAU latency */
	a1 = a0 * *r2
	r1 = num_param
	a0 = float(*r1)
	nop			/* for DAU latency */
	r13 = *prt_flag
	a2 = a1 * a0	
	r13 = r13 - 1
	if(ne)goto nomult1	/* for staggered PRT's double offset */
	nop
	a2 = a2 * *r2
nomult1:r5 = *pick_gate_off		
	*r6 = a2 = int(a2)
	3*nop			/* for DAU latency */
	r6 = *ts_off
	nop
	r6 = r6 + r5
	*ts_off = r6
	
/* Calcultate index into pick buffer to sort parameters properly */

	r3 = *f_flag
	nop
	r3 = r3 * 2
	r3 = r3 * 2
	*pick_indx = r3   /* *pick_indx = 4*(*f1_flag + *f2_flag + *f3_flag) */

/* Initialize data buffers to base addresses */

	r7 = sort_buff
	r10e = data_buff_1
	r11e = data_buff_2

/* Go to main routine */

	goto dwell
	nop
						
/*-------------------Local Constants --------------------*/

.align 4
fpone:    float 1.0
fptwo:    float 2.0
fpfive:	float 5.0
fhalf:    float 0.5
f20:      float 20.0
f40:	  float 40.0	/* 2/16/90 grg; added f40 for 0.5 db/cnt*/
f1:       float 1.0
fpzero:		float 0.0
Z_const:	float 180.
ncP_const:	float 100.
SIGMA_const:	float 0.35	/* should give 4 m/s at 1000hz sband */
V_const:	float 1.57



/*===================SUBROUTINES=====================*/

.align 4
#define QUADSUBS
#define COLSUBS
#include "colquadsubs.h"

/* endcode:	int24	0x5555 is in colquadsubs.h */
/* atan is not in checksum because of local storage */
/* in the atan function */

/*================END=SUBROUTINES=====================*/

/*-------------------Local Variables --------------------*/

.align 4
locall:   float 0.0
scratch:  4*float 0.0
fsamples: float 0.0
rsamples: float 0.0
range:	  float 1.0
r2corr:   float 0.0
fchip_avg: float 1.0
iv:       int 0
incP:     int 0
isigma:   int 0
idbZ:     int 0
in_loop:  int GATE_RATIO
tnum_int1: int 0
tnum_int2: int 0
tnum_int3: int 0
tnum_int4: int 0
tchip_avg1: int 0
tchip_avg2: int 0
tchip_avg3: int 0
tchip_avg4: int 0
dp_gates:   int 0
dp_tgates:  int 0
tsamples:  int 0
pick_gate_off:	int 0
ts_off:	    int 0
f_flag:	 int 0
tnum_param: int 0
num_param:  int NUM_PARAM
pick_loop:  int 0
pick_indx:  int 0

.align 4
ledl:     int24 CLEDON
lmask:    int24 0x000010
temp:     int24 0
temp1:	  int24 0
.align 4
eof:	float 1024.0
F1_bogus_start:	float 1.0
F2_bogus_start:	float 1.5
F3_bogus_start:	float 2.0
F1_bogus:	float 0.0	
F2_bogus:	float 0.0
F3_bogus:	float 0.0	
F1_bogus_fac:	float 2.0	
F2_bogus_fac:	float 1.5	
F3_bogus_fac:	float 0.75
scale:		float 1.0
fpsamples:	float 0.0
last_local_var:		int24 0x5555

#ifdef FAKE_DATA
.=0xF000
i_fft2:	16*float 0.0, 1.0, 0.0, -1.0
q_fft2: 16*float 1.0, 0.0, -1.0, 0.0	/* cw phasor of one count */
/* if you set *gates=1, *samples=4 in the simulator, then */
/* Iraw, Qraw = 1.0 */
/* and noting that AGC=2.0 in the FAKE_DATA code */
/* Icf, Qcf = 2.0 */
/* after processing periodogram: */
/* Iraw, Qraw = 1.0/IQ_norm = 1.0/2048.0 = .000488 */
/* Icf, Qcf = 2.0/iqcf_norm = 2.0/2.0 */
/* Raw periodogram has an estimate at pi/2  (.000488*64*2)^2 = 0.0039 */
/* Filtered periodogram at pi/2 has	((2.0/2.0)*64*2)^2 = 16384.0 */
#endif FAKE_DATA

/*  end of file  */


