/*
|
|       $Id$
|
|       Module: DP_INIT.s
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
|  This code initializes the ELDORA Doppler Processor with the 
|  information required for computing velocity, spectral width, 
|  normalized coherent power, and reflectivity.
*/ 
/* written by E. Loew
 * 23 January 1992
 */

#include "rp7.h"
#include "ELDRP7.h"
#include "dp_ramdefs.h"
#include "dspaddr.h"

/* the key assembler switches in the Doppler Processor Code */

#define ELDORA	/* "define ELDORA" or "define MHR" or "define CBAND" */
#ifdef ELDORA	/* ELDORA assembler switches */
#define no_CHKSUM	/* calculate code checksum ? */
#define PPP_NR_DSPS 8   /* ### should be defined in dspaddr.h */
#define DP_NR_DSPS 4    /* no. of Doppler Processor DSP's */
#define no_ONE_WAIT	/* use one wait state for Amem */

/* default parameters */
#define GATE_SPACING	0.150	/* default gate spacing in km */
#define RADAR_CONST	146.4	/* collator radar const in 0.5 dB units*/
				/* was 88.4 for lo channel only */
#define PNOISE		0.0     /*measured ELDORA noise floor*/
#define LOGP_OFFSET 235.0 /* ### for logP counts on tape, was 190.0 */
#define VSCALE 40.425356      /* Nyquist scaling kv = 127/pi = 40.425356 */
			      /* for one frequency operation only! */
#define WSCALE  57.170086    /* Nyquist scaling ks=57.170086 */
#define LOGP_SCALE  10/.34  /*(26.667)  for 256cts total, eg 10/0.375 db/bit*/
#define INIT_CLR_FIFOS		/* clear input fifos upon dspgo */
#define TIME_SERIES		/* transmit time series to Collator */
#endif ELDORA	/* end of ELDORA assembler switches */

/* Loop index defines */

#define GATE_RATIO	(PPP_NR_DSPS/DP_NR_DSPS) /* will be either 1 or 2 */

/*
*  Here's a guide to external memory A for the Doppler Processor:
*
*
*  0x0000-0x01FF 	variables and constants
*  0x0200-0x0FFF  	initialization code and subroutines
*  0x1000-0x17FF  	A buffer for PRT #1
*			(NGATES * PPP_NR_DSPS/DP_NR_DSPS * float bytes)
*			(for NGATES = 256,PPP_NR_DSPS = 8, DP_NR_DSPS = 4
*			buffer = 0x800 bytes)
*  0x1800-0x1FFF	B buffer for PRT #1
*			(NGATES * PPP_NR_DSPS/DP_NR_DSPS * float bytes)
*			(for NGATES = 256,PPP_NR_DSPS = 8, DP_NR_DSPS = 4
*			buffer = 0x800 bytes)
*  0x2000-0x27FF	P buffer for PRT #1
*			(NGATES * PPP_NR_DSPS/DP_NR_DSPS * float bytes)
*			(for NGATES = 256,PPP_NR_DSPS = 8, DP_NR_DSPS = 4
*			buffer = 0x800 bytes)
*  0x2800-0x2FFF  	A buffer for PRT #2
*			(NGATES * PPP_NR_DSPS/DP_NR_DSPS * float bytes)
*			(for NGATES = 256,PPP_NR_DSPS = 8, DP_NR_DSPS = 4
*			buffer = 0x800 bytes)
*  0x3000-0x37FF	B buffer for PRT #2
*			(NGATES * PPP_NR_DSPS/DP_NR_DSPS * float bytes)
*			(for NGATES = 256,PPP_NR_DSPS = 8, DP_NR_DSPS = 4
*			buffer = 0x800 bytes)
*  0x3800-0x3FFF	P buffer for PRT #2
*			(NGATES * PPP_NR_DSPS/DP_NR_DSPS * float bytes)
*			(for NGATES = 256,PPP_NR_DSPS = 8, DP_NR_DSPS = 4
*			buffer = 0x800 bytes)
*  0x4000-0x63FF	Doppler products and time series ping pong buffer for PRT #1
*			(4 values * NGATES * PPP_NR_DSPS/DP_NR_DSPS * float bytes)
*			(for NGATES = 256,PPP_NR_DSPS = 8, DP_NR_DSPS = 4
*			dp buffer = 0x2000 bytes)
*			(2values * PPP_NR_DSPS/DP_NR_DSPS * SAMPLES * float bytes)
*			(for PPP_NR_DSPS = 8, DP_NR_DSPS = 4 & SAMPLES = 64 
*			ts buffer = 0x400 bytes)
*			buffer = 0x2400 bytes
*  0x6400-0x87FF        Doppler products ping pong buffer for PRT #2
*			(4 values * NGATES * PPP_NR_DSPS/DP_NR_DSPS * float bytes)
*			(for NGATES = 256,PPP_NR_DSPS = 8, DP_NR_DSPS = 4
*			dp buffer = 0x2000 bytes)
*			(2values * PPP_NR_DSPS/DP_NR_DSPS * SAMPLES * float bytes)
*			(for PPP_NR_DSPS = 8, DP_NR_DSPS = 4 & SAMPLES = 64 
*			ts buffer = 0x400 bytes)
*			buffer = 0x2400 bytes
*  0x8800-0xABFF	Doppler products ping pong buffer for PRT #1
*			(4 values * NGATES * PPP_NR_DSPS/DP_NR_DSPS * float bytes)
*			(for NGATES = 256,PPP_NR_DSPS = 8, DP_NR_DSPS = 4
*			dp buffer = 0x2000 bytes)
*			(2values * PPP_NR_DSPS/DP_NR_DSPS * SAMPLES * float bytes)
*			(for PPP_NR_DSPS = 8, DP_NR_DSPS = 4 & SAMPLES = 64 
*			ts buffer = 0x400 bytes)
*			buffer = 0x2400 bytes
*  0xAC00-0xCFFF        Doppler products ping pong buffer for PRT #2
*			(4 values * NGATES * PPP_NR_DSPS/DP_NR_DSPS * float bytes)
*			(for NGATES = 256,PPP_NR_DSPS = 8, DP_NR_DSPS = 4
*			dp buffer = 0x2000 bytes)
*			(2values * PPP_NR_DSPS/DP_NR_DSPS * SAMPLES * float bytes)
*			(for PPP_NR_DSPS = 8, DP_NR_DSPS = 4 & SAMPLES = 64 
*			ts buffer = 0x400 bytes)
*			buffer = 0x2400 bytes
*  
*/

/*

main()
{
Clear FIFO buffers;
Initialize output buffer pointers;

do{
     For(each gate){
       Input A,B,P data; (put in bogus for numeric checks)
	skip gates if so instructed
       Calculate{ 
	  bypass NCP and SIGMA if desired
         rho = (B*B + A*A)^(1/2);
         B/A; 
         v = ATAN(B/A);
	 NCP = rho/P;
	 Ps = P - Pn (if NOISESUB_all);
         LOG10(P) + C; 
         SIGMA = SQRT[-ln(rho/Ps)];
	 Z(dB) = RangeSquared(dB) + RadarConst(dB) + Ps(dB)
       }
       Convert parameters to byte quantities;     
       Combine bytes and store in output buffer;
     }
     Set PIR interrupt flag;
     Read in control-status trailer
     Read in time series data if desired
     Process a few periodogram estimates is desired
Swap output buffers;
  }while(!COL_stop);
}
ATAN(){
.
.
}
DIV(){
.
.
}
LOG10(){
.
.
}
*/
/*
  Variables and Buffers
*/

#define A_buff_1 0x1000		/* A ping pong buffer base address for PRT #1 */
#define B_buff_1 0x1800		/* B ping pong buffer base address for PRT #1 */
#define P_buff_1 0x2000		/* P ping pong buffer base address for PRT #1 */
#define A_buff_2 0x2800		/* A ping pong buffer base address for PRT #2 */
#define B_buff_2 0x3000		/* B ping pong buffer base address for PRT #2 */
#define P_buff_2 0x3800		/* P ping pong buffer base address for PRT #2 */
#define dp_buff_1 0x4000	/* doppler products and time series ping pong buffer base address for PRT #1 */
#define dp_buff_2 0x6400	/* doppler products and time series ping pong buffer base address for PRT #2 */
#define dp_buff_3 0x8800	/* doppler products and time series ping pong buffer base address for PRT #1 */
#define dp_buff_4 0xAC00	/* doppler products and time series ping pong buffer base address for PRT #2 */

.global start, clrfifos, xmt_offset, in_tloop
.global temp1, temp2, itable, f_scale, f_flag, ff_flag, sync_flag
.global gates, samples, in_loop, ts_loop, f1_flag, f2_flag, f3_flag
.global in_loop_fp, dp_gates, dp_tgates, cloop, cloop1, eof, eof_rcvd
.global tsoff, xmtgates, tgates, prt_flag, dsp0gates, dsp0_xmt_offset
.global scratch, rho, v, kp, ks, P, kn, kv
.global ks_fx, kn_fx, kv_fx, cradar_fx, kSNR_fx
.global Pnoise, r1_store, r2_store, r15_store, r20_store, r7_store, r8_store
.global buffer, cradar, offset, tsamples
.global logP, logPn, ncP, logncP, dbZ, ncP
.global sigma, nscP, lognscP
.global A_bogus, B_bogus, P_bogus, kSNR
.global A_bogus_fac, B_bogus_fac, P_bogus_fac
.global A_bogus_start, B_bogus_start, P_bogus_start, fptiny
.global last_local_var
.global V_const, P_const, SIGMA_const, ncP_const
.global fpzero, magA, magB, locall, f180d, rho2
.extern isr, dwell
#ifdef CHKSUM
.global chksum, chkloop, chkdone, endcode
#endif CHKSUM

.rsect ".extA"
#ifdef ONE_WAIT
                r10 = 0x0B              /* 1 Wait for A, 3 for B(i/o) */
#else
                r10 = 0x09              /* 0 Wait for A, 3 for B */
#endif ONE_WAIT
	  pcw = r10
          ioc = 0x0554
          dauc = 0
          goto start

.=0x0050

gates:    int 64	/* typically 64 for 8 PPP DSP's */
samples:  int 20	/* typically 16, 20, or 32 */
/* .=BUFFER_PTR */
buffer:   int 0
.align 4
.=DSP_CHKSUM
chksum:		int24 0	/* code checksum from start to endcode */

.=0x00F0

/* Pointers to various things */
/* NOTE: must change include file to match new addresses!!! */

/* .=DP_RADAR_CONST */
/* cradar = 20 log kz + 20 log krcvr  + 2*zoffset ### */
/* the above calculation should be done in VXWORKS and the result poked in */
/* to cradar to give correct weather reflectivities */
/* Example for MHR:   cradar = 2*(70) + 20*(-5.56) + 2*(30) = 89.7  */
/* We assumed the radar constant is 70, the receiver+RP7 square-law slope */
/* about 2.6e-5 mw per normalized power unit, */
/* and the zoffset for output is 30 dB (below 0dB) */
cradar_fx:        float RADAR_CONST /* collator radar constant */
/* .=DP_PNOISE */
Pnoise:         float PNOISE	/* noise power in RP7 p units  */
/* .=DP_LOGP_OFFSET */
offset:         float LOGP_OFFSET /* lid for logP counts on tape */
/* .=DP_VSCALE */
/* for Nyquist scaling kv=127/pi*fmin/fmax; where fmin=minimum freq xmitted */
kv_fx:       float VSCALE  
/* .=DP_WSCALE */
/* for Nyquist scaling ks=(2*127)/(pi*sqrt2) */
/* for fixed scaling ks=(Vmax/Vtop)*(2*127)/(pi*sqrt2) */
ks_fx:       float WSCALE    /* Nyquist scaling ks=57.170086 */

/* .=DP_LOGP_SCALE */
/* scaling factor for logP: equates to relative slope of cal curve */
kp:       float LOGP_SCALE /* slope of logp counts to tape, eg of */
			/* 256cts total  10/0.375 db/bit ### */
.=0x112
/* .=DP_PRT_FLAG */
prt_flag:	int	0	/* 1=Staggered PRT's; 0=Single PRT */
/* .=DP_F1_FLAG */
f1_flag:	int	1	/* default freq 1 enabled */
/* .=DP_F2_FLAG */
f2_flag:	int	0	/* default freq 2 disabled */
/* .=DP_F3_FLAG */
f3_flag:	int	0	/* default freq 3 disabled */
/* .=DP_SYNC_FLAG */
sync_flag:      int     0       /* if zero; DSP32C in sync */
/* .=DP_EOF_RCVD */
.align 4
eof_rcvd:	float	0.0	/* should be updated to EOF (0x8A) */

.=0x200

start:
/* Setup Interrupt Vector Table */

	r22e = itable

#ifdef CHKSUM
 	r2e = start	/* calculate code checksum */
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

/* Clear out INTR2 in case reset doesn't */

	r2e = CLRINT2
	a1 = *r2

/* Calculate loop indeces for main loop */

	r2 = in_loop		/* in_loop -> PPP_NR_DSPS/DP_NR_DSPS */
	a0 = float(*r2)
	r2 = gates  
	a1 = float(*r2)
	r3 = samples
	r5 = *samples
	a3 = float(*r3)
	r4 = r5 - 2
	*tsamples = r4
	r2e = dp_gates
	a2 = a0 * a1	
	*r2  = a2 = int(a2)  	/* store dp_gates */
	r3e = ts_loop
	a2 = a3 * a0
	*r3 = a2 = int(a2)	/* store ts_loop */
	r3 = *in_loop
	r1 = *dp_gates
	r4 = *gates
	r2 = *ts_loop
	r5 = r1 * 2
	r5 = r5 * 2
	r4 = r4 - 2
	*tgates = r4            /* *tgates = *gates-2 */
	*xmtgates = r5		/* *xmtgates=(*dp_gates*4) */
	r6 = r1 - 2		
	*dp_tgates = r6		/* *dp_tgates=*dp_gates-2 for loop index */
	r2 = r2 * 2	
	r2 = r2 - 2		
	*ts_loop = r2	/* *ts_loop=(*ts_loop * 2) - 2 for loop index */
	r3 = r3 - 2	
	*in_tloop = r3	/* *in_tloop=*in_loop-2 for loop index */

/* Zero out A,B,P ping pong data buffers for PRT 1 and PRT 2 */
	
	r6 = fpzero
	a0 = *r6
	r15 = *dp_tgates
	r1 = A_buff_1
	r2 = B_buff_1
	r3 = P_buff_1
	r4 = A_buff_2
	r5 = B_buff_2
	r6 = P_buff_2
cloop:	*r1++ = a0 = a0
	*r2++ = a0 = a0
	*r3++ = a0 = a0
	*r4++ = a0 = a0
	*r5++ = a0 = a0
	*r6++ = a0 = a0
	if(r15-->=0)goto cloop
	nop

#ifdef TIME_SERIES
/* NOTE: Only DSP0(MASTER) will transmit time series data.
 * Therefore, we can only select every 8th gate for time series.
 */
/* Calculate time series offset into Doppler Products and Time Series Buffers */

	r15 = *xmtgates
	r16 = *xmtgates
	r2 = *ts_loop
	r15 = r15 - 1		/* *xmtgates - 1 */
	r15 = r15 * 2
	r15 = r15 * 2
	*tsoff = r15		/* time series offset = (4*(*xmtgates - 1) */
	r16 = r16 + r2
	r16 = r16 + 2
	*dsp0gates = r16	/* *dsp0gates now includes time series data */
				/* to be transmitted by dsp0 only */

/* Compute offset to end of DSP0's Doppler Products and Time Series ping pong buffers */

	r15 = *dsp0gates
	nop
	r15 = r15 * 2
	r15 = r15 * 2
	*dsp0_xmt_offset = r15

/* Convert dsp0gates to a loop index i.e. *dsp0gates=*dsp0gates - 2 */	

	r15 = *dsp0gates
	nop
	r15 = r15 - 2
	*dsp0gates = r15      /* *dsp0gates-2 */
	
#endif TIME_SERIES
	
/* Compute offset to end of Doppler Products and Time Series ping pong buffers */

	r15 = *xmtgates
	nop
	r15 = r15 * 2
	r15 = r15 * 2
	*xmt_offset = r15


/* Convert xmtgates to a loop index i.e. *xmtgates=*xmtgates - 2 */	

	r15 = *xmtgates
	nop
	r15 = r15 - 2
	*xmtgates = r15      /* *xmtgates-2 */


#ifndef TIME_SERIES	/* if not sending time series, dsp0 transmits same */
			/* number of gates as other dsp's */

	r15 = *xmtgates
	r16 = *xmt_offset
	*dsp0gates = r15
	*dsp0_xmt_offset = r16

#endif TIME_SERIES
/*
   Zero out Doppler Products and Time Series ping pong buffers 
   for both PRT 1 and PRT 2 
*/
	r15 = *dsp0gates	/* All DSP's input time series data if req'd */
	r1 = dp_buff_1	
	r2 = dp_buff_2	
	r3e = dp_buff_3	
	r4e = dp_buff_4	
cloop1:	*r1++ = a0 = a0
	*r2++ = a0 = a0	
	*r3++ = a0 = a0
	*r4++ = a0 = a0
	if(r15-->=0)goto cloop1
	nop
	
#ifdef INIT_CLR_FIFOS
clrfifos: r1e = FIFOCLR        /* FIFO clear loc */
	  a0 = *r1            /* Clear the FIFO's */
#else
clrfifos: nop           /* Dont Clear the FIFO's upon software initialization */
#endif INIT_CLR_FIFOS


/* Compute log10 of noise power, Pnoise */
 
                call _log10(r14)
                nop
                int24     locall, Pnoise, logPn
.align 4

/* Compute frequency scale factor */
/* Each doppler product will be multiplied by f_scale prior to transmition */
/* to the Collator. In this way the Collator averages the data by simple */
/* summation. */

	r1 = *f1_flag
	r2 = *f2_flag
	r3 = *f3_flag
	r1 = r1 + r2
	r3 = r3 + r1		/* sum all frequency flags */
	*f_flag = r3
	r1 = f_flag
	r2 = ff_flag
	*r2 = a0 = float(*r1)	/* convert to float */
	3*nop
	call _invf (r14)
	nop
int24 ff_flag, f_scale
.align 4
	
/* Modify scale factor kv, ks, kn, kSNR, cradar to reflect frequency scaling */

	r1 = kv_fx
	r5 = f_scale
	r2 = ks_fx
	r3 = kn_fx
	r4 = kSNR_fx
	r6 = cradar_fx
	r7 = kn
	r8 = kSNR
	r9 = cradar
	r10 = kv
	r11 = ks
	*r10 = a0 = *r5 * *r1
	*r11 = a0 = *r5 * *r2
	*r7 = a0 = *r5 * *r3
	*r8 = a0 = *r5 * *r4
	*r9 = a0 = *r5 * *r6

/* Initialize doppler product and time series buffers */

	r10e = dp_buff_1	
	r11e = dp_buff_3	
	r12e = dp_buff_2	
	r13e = dp_buff_4	
		
/* Branch to Main Routine */

	goto dwell
	nop

/* Interrupt Vector Table */
.=0x420
.align 4
itable:	2*nop
	2*nop
	2*nop
	2*nop
	2*nop
	goto isr
	nop
	4*nop
/*-------------------Local Constants --------------------*/

.align 4
eof:	  float 1024.0
fpone:    float 1.0
fptwo:    float 2.0
fpfive:	  float 5.0
fhalf:    float 0.5
kSNR_fx:  float 20.0	/* constant multiplier for log quantities ### */
kn_fx:    float 100.0         /* ncP scale factor - put out 0 -100 counts */
kdbZ:     float 1.0               /* dbZ scale factor */
cdbZ:     float 0.0               /* dbZ offset */
f180d:    float 3.14159265    /* pi */
f90d:     float 1.570796327   /* pi/2 */
f20:      float 20.0
f40:	  float 40.0	/* 2/16/90 grg; added f40 for 0.5 db/cnt*/
f1:       float 1.0
fpzero:		float 0.0
fptiny:		float 1.0e-12 	/* smallest possible A, B, or P */
		/* we picked a number representing less than 0.1 ADC bit */
P_const:	float 180.
ncP_const:	float 100.
SIGMA_const:	float 0.35	/* should give 4 m/s at 1000hz sband */
V_const:	float 1.57
kn:	float 0.0
ks:	float 0.0
kv:	float 0.0
kSNR:	float 0.0
cradar:	float 0.0

/*---------------------Pointers---------------------*/

.align 4
dp_gates:	int 0
dp_tgates:	int 0
xmt_offset:	int 0
dsp0_xmt_offset: int 0
in_loop:	int GATE_RATIO
in_tloop:	int 0
tgates:		int 0
ts_loop:	int 0
tsoff:		int 0
xmtgates:	int 0
dsp0gates:	int 0
r7_store:	int 0
r8_store:	int 0
r15_store:	int 0
r20_store:	int 0
f_flag:		int 0
tsamples:	int 0
.align 4
temp1:		int24 0
temp2:		int24 0
r1_store:	int24 0
r2_store:	int24 0

.page

/*===================SUBROUTINES=====================*/

.align 4
#define QUADSUBS
#define COLSUBS
#include "/ELDORA/dsp/include/colquadsubs.h"

/* endcode:	int24	0x5555 is in colquadsubs.h */
/* atan is not in checksum because of local storage */
/* in the atan function */

/*================END=SUBROUTINES=====================*/

/*-------------------Local Variables --------------------*/

.align 4
in_loop_fp: float 0.0
locall:   float 0.0
scratch:  4*float 0.0
magA:     float 0.0
magB:     float 0.0
RP:       float 0.0
rho:      float 0.0
rho2:     float 0.0		
ratio:    float 0.0
P:	  float 0.0
v:        float 0.0
logP:     float 0.0
logPn:	  float 0.0
logPs:	  float 0.0	
ncP:      float 0.0
logncP:   float 0.0
sigma:    float 0.0
dbZ:      float 0.0
.align 4
f_scale:	float 1.0	/* frequency scale factor */
ff_flag:	float 0.0
A_bogus_start:	float .120	/* 200.0 */
B_bogus_start:	float .001	/* 200.0 */
P_bogus_start:	float .125	/* 100.0 */
A_bogus:	float 0.0	/* ###  inserted values for test pattern */
B_bogus:	float 0.0
P_bogus:	float 0.0	/* ### */
A_bogus_fac:	float 10.0	/*0.93*/	/* ### */
B_bogus_fac:	float 8.0	/*0.93*/
P_bogus_fac:	float 10.0	/* 0.95*/
nscP:			float 0.0
lognscP:		float 0.0
last_local_var:		int24 0x5555

/*  end of file  */
