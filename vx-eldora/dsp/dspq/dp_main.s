/*
|
|       $Id$
|
|       Module: DP_MAIN.s
|       Original Author: E. Loew 
|       Copywrited by the National Center for Atmospheric Research
|       Date:            $Date$
|
| revision history
| ----------------
| $Log$
 * Revision 1.1  1993/03/24  20:14:43  eric
 * Initial revision
 *
 * Revision 1.1  1992/11/10  19:58:37  eric
 * Initial revision
 *
|
| calling convention:
|
| description:
|        
|  This is the real time code for the ELDORA Doppler Processor. It 
|  computes velocity, spectral width, normalized coherent power, 
|  and reflectivity for both single and staggered PRT's
*/ 


#include "rp7.h"
#include "ELDRP7.h"
#include "col_dpramdefs.h"
#include "dspaddr.h"		

/* written by E. Loew
 * 11 February 1992
 */

/* the key assembler switches in the Doppler Processor Code */

#define ELDORA	/* "define ELDORA" or "define MHR" or "define CBAND" */
#ifdef ELDORA	/* ELDORA assembler switches */
/* Simulator definition */
#define NO_UNIX_SIMULATOR_TESTS    /* Sets up defines to run code on simulator */
/* try defining these from the preprocessor */
/* #define MASTER */			/* Set for MASTER (DSP0) */
/* #define no_SLAVE  */		/* Set for SLAVE  (DSP1-3) */
#define ERRINT		/* Interrupt Collator if EOF not detected */
#define no_INTERRUPT		/* clear interrupt to vxworks */
#define TIME_SERIES		/* time series switch */
#define PPP_NR_DSPS 8   /* should be defined in dspaddr.h */
#define DP_NR_DSPS 4    /* no. of Doppler Processor DSP's */
#define no_CONSTANTS		/* Input constants for v,sigma,dbZ,ncP */
#endif ELDORA	/* end of ELDORA assembler switches */

/* experimental defines used during development on unix simulator */

#ifdef UNIX_SIMULATOR_TESTS
#define INPUT_BOGUS
#define FAKE_ISR
#endif UNIX_SIMULATOR_TESTS

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

dwell()
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
#define dp_buff_3 0x008800	/* doppler products and time series ping pong buffer base address for PRT #1 */
#define dp_buff_4 0x00AC00	/* doppler products and time series ping pong buffer base address for PRT #2 */
.extern start, clrfifos, prt_flag, xmt_offset, dsp0gates, dsp0_xmt_offset
.extern gates, samples, in_loop, tsamples, in_tloop, sync_flag
.extern in_loop_fp, dp_gates, dp_tgates, cloop, cloop1, eof, eof_rcvd
.extern scratch, rho, v,  kp, ks, P, rho2
.extern kn, kv, temp1, temp2
.extern Pnoise, r1_store, r2_store, r15_store, r20_store, r7_store, r8_store
.extern buffer, cradar, offset
.extern logP, logPn, ncP, logncP, dbZ
.extern sigma, nscP, lognscP
.extern A_bogus, B_bogus, P_bogus, kSNR
.extern A_bogus_fac, B_bogus_fac, P_bogus_fac
.extern A_bogus_start, B_bogus_start, P_bogus_start, fptiny
.extern magA, magB, locall, fpzero, f180d
.global dwell, inA1, inA2, inB1, inB2, inP1, inP2, incnt, ineof
.global calc1, calc2, endgate1, endgate2, proc1, proc2, errint
.global tp1, tp2, tp3, tp4, tp5, tp6, isr
#ifdef FAKE_ISR 
.global fisr
#endif FAKE_ISR
#ifdef MASTER
.global intr, write1, write2
#endif MASTER
#ifdef SLAVE
.global intr, write1, write2
#endif SLAVE

#ifdef TIME_SERIES
.global inQ1, inI1, inQ2, inI2, tscnt  
#endif TIME_SERIES

.extern last_local_var
.extern V_const, P_const, SIGMA_const, ncP_const 
.align 4
.rsect ".ram0"

dwell:

#ifdef INPUT_BOGUS
	r1e = A_bogus		/* initialize bogus inputs each dwell */
	r2e = A_bogus_start
	*r1++ = a0 = *r2++
	*r1++ = a0 = *r2++
	*r1 = a0 = *r2
#endif INPUT_BOGUS

/* reset all loop indeces */

	r18 = *dp_tgates	/* reset gate processing counter */
	r17 = *tsamples		/* reset time series data counter */
	r16 = *tgates		/* reset input loop counter */
	r19 = *in_tloop		/* reset individual input loop counter */
/* reset pointers to base of A, B, P buffers for both PRT 1 and PRT 2 */

	r4 = A_buff_1
	r5 = B_buff_1
	r6 = P_buff_1
	r7 = A_buff_2
	r8 = B_buff_2
	r9 = P_buff_2

/* Swap doppler product and time series buffers for both PRT 1 and PRT 2 */
	
	*temp1 = r10e
	r10e = r11
	r11e = *temp1
	*temp2 = r12e
	r12e = r13
	r13e = *temp2

/* Un-mask INTR2 */

	r3 = pcw
	r20 = *prt_flag
	r3 = r3 | 0x400
	pcw = r3

#ifdef MASTER

/* 
  if MASTER write out data from previous dwell first then interrupt DSP 1
*/
 
	r1e = B_PORT 	/* write out B_PORT for F1,F2,F3 */
	r2e = INTOUT2  
	r15 = *dsp0gates
	r21 = *dsp0_xmt_offset
write1:	*r1 = a0 = *r10++
	if(r15-->= 0)goto write1
	nop
	r20 = r20 - 1
	if(ne)goto intr
	r20 = *prt_flag
	r15 = *dsp0gates
write2:	*r1 = a0 = *r12++
	if(r15-->= 0)goto write2
	nop
	r12e = r12 - r21	/* restore data buffer to base */
intr:	a0 = *r2		/* interrupt DSP 1 */
	r10e = r10 - r21	/* restore data buffer to base */
	
#endif MASTER

#ifdef FAKE_ISR 
/* Branch to isr for unix simulation */
	goto isr
fisr:	nop
#endif FAKE_ISR
	r1e = A_PORT         	/* point to input FIFO (A_PORT) */

/* Input Loop;  inputs A,B, and P (PRT 1 and PRT 2) for each gate */
/* 
   NOTE: can straight line following code for speed once 
   functionality is verified using 4 PPP DSP's, because
   under normal operation *in_loop will be fixed at 2!!!
*/

inA1:	if(syc)goto inA1	/* Wait for data    */
	nop
#ifdef INPUT_BOGUS
	r1e = A_bogus
#endif
	*r4++ = a0 = *r1       /* Input A for PRT 1 */
	if(r19-->=0)goto inA1
	nop
	r19 = *in_tloop
	r20 = r20 - 1
	if(ne)goto inB1
	r20 = *prt_flag

inA2:	if(syc)goto inA2     	/* Wait for data    */
	nop
#ifdef INPUT_BOGUS
	r1e = A_bogus
#endif
	*r7++ = a0 = *r1       /* Input A for PRT 2 */
	if(r19-->=0)goto inA2
	nop
	r19 = *in_tloop

inB1:   if(syc)goto inB1     
	nop
#ifdef INPUT_BOGUS
	r1e = B_bogus
#endif
        *r5++ = a1 = *r1	/* Input B for PRT 1 */
	if(r19-->=0)goto inB1
	nop
	r19 = *in_tloop
	r20 = r20 - 1
	if(ne)goto inP1
	r20 = *prt_flag

inB2:	if(syc)goto inB2     	/* Wait for data    */
	nop
#ifdef INPUT_BOGUS
	r1e = B_bogus
#endif
	*r8++ = a0 = *r1       /* Input B for PRT 2 */
	if(r19-->=0)goto inB2
	nop
	r19 = *in_tloop

inP1:   if(syc)goto inP1     
	nop
#ifdef INPUT_BOGUS
	r1e = P_bogus
#endif
        *r6++ = a2 = *r1       /* Input P for PRT 1 */ 
	if(r19-->=0)goto inP1
	nop
	r19 = *in_tloop
	r20 = r20 - 1
	if(ne)goto incnt 
	r20 = *prt_flag

inP2:	if(syc)goto inP2     	/* Wait for data    */
	nop
#ifdef INPUT_BOGUS
	r1e = P_bogus
#endif
	*r9++ = a0 = *r1       /* Input P for PRT 2 */
	if(r19-->=0)goto inP2
	nop
	r19 = *in_tloop
incnt:
#ifdef INPUT_BOGUS
	r1e = A_bogus		/* multiply bogus inputs by factor each gate */
	r2e = A_bogus_fac
	*r1++ = a0 = -*r1 * *r2++
	nop
	*r1++ = a0 = -*r1 * *r2++
	nop
	*r1 = a0 = *r1 * *r2
#endif INPUT_BOGUS

	if(r16-->=0)goto inA1
	r2 = eof

#ifdef TIME_SERIES

	r3 = *tsoff
	nop			/* for CA Latency */
	r11e = r11 + r3		/* add in offset for time series */
	r13e = r13 + r3		/* add in offset for time series */
inI1:   if(syc)goto inI1     	/* Wait for data */
	nop
        *r11++ = a2 = *r1       /* Input I for PRT 1 */ 
	if(r19-->=0)goto inI1
	nop
	r19 = *in_tloop
	r20 = r20 - 1
	if(ne)goto inQ1 
	r20 = *prt_flag

inI2:	if(syc)goto inI2     	/* Wait for data    */
	nop
	*r13++ = a0 = *r1       /* Input I for PRT 2 */
	if(r19-->=0)goto inI2
	nop
	r19 = *in_tloop

inQ1:   if(syc)goto inQ1     	/* Wait for data */
	nop
        *r11++ = a2 = *r1       /* Input Q for PRT 1 */ 
	if(r19-->=0)goto inQ1
	nop
	r19 = *in_tloop
	r20 = r20 - 1
	if(ne)goto tscnt 
	r20 = *prt_flag

inQ2:	if(syc)goto inQ2     	/* Wait for data */
	nop
	*r13++ = a0 = *r1       /* Input Q for PRT 2 */
	if(r19-->=0)goto inQ2
	nop
	r19 = *in_tloop
tscnt:	if(r17-->=0)goto inI1	
	nop
	r11e = *temp1		/* return buffer pointer to base */
	r13e = *temp2		/* return buffer pointer to base */
#endif TIME_SERIES

/* 
   Read in EOF:
*/
ineof:	if(syc)goto ineof
	r3 = eof_rcvd
	a0 = *r1       		/* Input EOF  */
	if(r19-->=0)goto ineof
	nop
	r19 = *in_tloop
	*r3 = a0 = a0
	a1 = a0 - *r2		/* eof recvd - eof (better be 0!) */

#ifdef INTERRUPT
	 /* clear the pif, the DP interrupt to the host 680x0 ### */
	r1 = pir 
#endif

/* 
   Processing Loop; Doppler processing is done on all gates for PRT 1 
   and then for PRT 2 if desired. Presently four parameters are 
   computed: Velocity (v), Spectral Width(sigma), Refectivity(dbZ), 
   and Normalized Coherent Power(ncP)
*/


/* reset pointers to base of A, B, P buffers for PRT 1 */

proc1:	r7 = A_buff_1
	r8 = B_buff_1
	r9 = P_buff_1

#ifdef no_UNIX_SIMULATOR_TESTS
#ifdef ERRINT
/* 
   Check for EOF:  (placed here for DAU latency)
   If not received interrupt Collator's BIM 
*/
	if(ane)goto errint	/* if result not 0 interrupt Collator */
	nop
#endif ERRINT
#endif no_UNIX_SIMULATOR_TESTS

/* Calculate magnitude of A and B */
calc1:  r1 = magA
        r2 = magB 
        a0 = *r7		/* bring A into accumulator */
        a1 = -a0
        *r1 = a1 = ifalt(a0)
        a2 = *r8		/* bring B into accumulator */
        a3 = -a2
        *r2 = a3 = ifalt(a2)

/* Calculate rho2 = AxA + BxB */
	
	r1 = rho2
        a3 = a0 * a0
        *r1 = a3 = a3 + a2 * a2  	/* store A**2 + B**2 in rho2 */

/* Calculate ATAN(B/A) */

          call _atan2(r14)
          nop
     int24  magB, magA, v
.align 4
tp1:    r3 = v
	r1 = P
	r2 = f180d     
	*r1 = a2 = *r9++		/* store R(0) in P */
        a1 = -*r3 + *r2      		/* a1 = 180 - v */
        a0 = *r3              		/* a0 = v */
        a2 = *r7++			/* Test A and increment pointer */
        *r3 = a0 = ifalt(a1) 		/* If A<0 then v = 180 - v */
        a1 = -a0
        a2 = *r8++			/* Test B and increment pointer */
        *r3 = a0 = ifalt(a1)  		/* If B < 0 then v = - v */

/* Store r7,r8 pointers before Subroutine */

	*r7_store = r7
	*r8_store = r8

/* Calculate SIGMA = SQRT(-ln(rho/P)) and Calculate NCP */

/* rho = sqrt(rho2) */

tp2:	call _sqrtf(r14)	
	nop
     	int24  scratch
     int24  rho2, rho
.align 4

/* use total received power for ncP */
	call _divf(r14)          	/* rho/P --> ncP */
        nop
     int24  rho, P, ncP
.align 4


/* in this case, the system noise estimate Pn corrects Ze, Sigma */
        r2 = P 
        r3 = Pnoise
	r1e = fptiny		/* smallest likely value of a,b,p */
        a1 = *r2 - *r3	/* Noise subtracted from P */
	*r2 = a1 = ifalt(*r1)	   /* Don't allow very small or negatives */
/* P becomes an estimate of "signal pwr" (R(0)-Pn) */ 

	call _log10(r14)
	nop
    int24  locall, P, logP
.align 4

/* calculate the coherent signal power from Psig and rho */
	call _divf(r14)          /* rho/Psig --> nscP */
        nop
     int24  rho, P, nscP
.align 4
        call _loge(r14)          /* ln(rho/Psig)  */
        nop				/* a0 also returns result ### */
     int24  locall, nscP, lognscP
.align 4  

tp3:    r1 = lognscP  
	r2e = fpzero  
        *r1 = a0 = -a0           /* negate ln(rho/psig) */
	*r1 = a0 = ifalt(*r2)   /* Don't allow negatives */
        call _sqrtf(r14)      /* sqrt(-ln(rho/P)) --> sigma */
        nop
     int24  scratch
     int24  lognscP, sigma    

.align 4

/* Scale processor floating point representations of v, sigma, ncP, */
/* dbZ so that they occupy one byte when converted to int values. */
/* Scaling also accounts for number of frequencies processed */
/* Power is expressed in normalized units from the Pulse Pair Processors;*/
/* normalization is performed in the Pulse Pair code to */
/* ADC one-sided full scale. For example the system noise might average */
/* 5 counts on each i and q, so that Pnoise from the processor would be */
/* P = 2*[(5/2048)^2] = 1.2e-5 in normalized units  */
/* For Ze, the conversion from p to signal power is assumed */
/* to be linear, with a slope expressed */
/* in milliwatts per normalized power unit */

/* Scale velocity (v) and store in buffer */

        r1 = v              		/* Preload r1-->v  */
#ifdef CONSTANTS
	/* insert constant for debugging */
	r1e = V_const
#endif CONSTANTS
	r2 = kv             		/* Preload r2-->kv */
        *r11++ = a1 = *r2 * *r1		/* Scale v and store in dp buffer  */

/* Scale spectral width (sigma) and store in buffer */
        r1 = sigma            		/* Preload r1-->sigma */
#ifdef CONSTANTS
	/* insert constant for debugging */
	r1e = SIGMA_const
#endif CONSTANTS

        r2 = ks          		/* Preload r2-->ks */
        *r11++ = a1 = *r2 * *r1      	/* Scale sigma and store in dp buffer */

/* Scale normalized coherent power (ncP) and store in buffer */
        r1 = ncP             		/* Preload ncP stuff*/
#ifdef CONSTANTS
	/* insert constant for debugging */
	r1e = ncP_const
#endif CONSTANTS

        r2 = kn            
        *r11++ = a1 = *r2 * *r1	/* Scale ncP and store in dp buffer  */

/* Calculate and scale Ze counts with noise correction if NOISESUB */
/* NOTE: range correction will be added in the Collator!!! */
l1:     r1 = logP
#ifdef CONSTANTS
	/* insert constant for debugging */
	r1e = P_const
#endif CONSTANTS
	r14 = kSNR	/* *kSNR = 20.0 (i.e. 0.5 db/count) */ 
	a0 = *r1
        r3 = dbZ
        r2 = cradar
/* Ze(0.5dB) = 40 log r + 20 log p + cradar */
/* cradar = 20 log kz + 20 log krcvr  + 2*zoffset */
		/* scale by kSNR then add the composite radar constant cradar */

	*r11++ = a0 = *r2 + a0 * *r14
	nop
/* Restore pointers used in Subroutines */

	r7 = *r7_store		/* restore r7 pointer value (r7 used in subr.) */
	r8 = *r8_store		/* restore r8 pointer value (r8 used in subr.) */
	
        
endgate1:  if(r18-->=0)goto calc1	/* test number of gates processed */
	   nop
/* Restore dp buffer to base */

	r11e = *temp1
/*
   Check status of prt flag: if 1 calculate doppler product for PRT 2
			     if not 1 begin next dwell               
*/
	r20 = r20 - 1
	if(ne)goto dwell 
	r20 = *prt_flag
	r18 = *dp_tgates	/* re-initialize to number of gates */

/* reset pointers to base of A, B, P buffers for PRT 2 */

proc2:	r7 = A_buff_2
	r8 = B_buff_2
	r9 = P_buff_2

/* Calculate magnitude of A and B */
calc2:  r1 = magA
        r2 = magB 
        a0 = *r7		/* bring A into accumulator */
        a1 = -a0
        *r1 = a1 = ifalt(a0)
        a2 = *r8		/* bring B into accumulator */
        a3 = -a2
        *r2 = a3 = ifalt(a2)

/* Calculate rho2 = AxA + BxB */
	
	r1 = rho2
        a3 = a0 * a0
        *r1 = a3 = a3 + a2 * a2  	/* store A**2 + B**2 in rho2 */

/* Calculate ATAN(B/A) */

          call _atan2(r14)
          nop
     int24  magB, magA, v
.align 4
tp4:    r3 = v
	r1 = P
	r2 = f180d     
	*r1 = a2 = *r9++		/* store R(0) in P */
        a1 = -*r3 + *r2      		/* a1 = 180 - v */
        a0 = *r3              		/* a0 = v */
        a2 = *r7++			/* Test A and increment pointer */
        *r3 = a0 = ifalt(a1) 		/* If A<0 then v = 180 - v */
        a1 = -a0
        a2 = *r8++			/* Test B and increment pointer */
        *r3 = a0 = ifalt(a1)  		/* If B < 0 then v = - v */

/* Store r7,r8 pointers before Subroutine */

	*r7_store = r7
	*r8_store = r8

/* Calculate SIGMA = SQRT(-ln(rho/P)) and Calculate NCP */

/* rho = sqrt(rho2) */

tp5:	call _sqrtf(r14)	
	nop
     	int24  scratch
     int24  rho2, rho
.align 4

/* use total received power for ncP */
	call _divf(r14)          	/* rho/P --> ncP */
        nop
     int24  rho, P, ncP
.align 4


/* in this case, the system noise estimate Pn corrects Ze, Sigma */
        r2 = P 
        r3 = Pnoise
	r1e = fptiny		/* smallest likely value of a,b,p */
        a1 = *r2 - *r3	/* Noise subtracted from P */
	*r2 = a1 = ifalt(*r1)	   /* Don't allow very small or negatives */
/* P becomes an estimate of "signal pwr" (R(0)-Pn) */ 

	call _log10(r14)
	nop
    int24  locall, P, logP
.align 4

/* calculate the coherent signal power from Psig and rho */
	call _divf(r14)          /* rho/Psig --> nscP */
        nop
     int24  rho, P, nscP
.align 4
        call _loge(r14)          /* ln(rho/Psig)  */
        nop				/* a0 also returns result ### */
     int24  locall, nscP, lognscP
.align 4  

tp6:    r1 = lognscP  
	r2e = fpzero  
        *r1 = a0 = -a0           /* negate ln(rho/psig) */
	*r1 = a0 = ifalt(*r2)   /* Don't allow negatives */
        call _sqrtf(r14)      /* sqrt(-ln(rho/P)) --> sigma */
        nop
     int24  scratch
     int24  lognscP, sigma    

.align 4

/* Scale processor floating point representations of v, sigma, ncP, */
/* dbZ so that they occupy one byte when converted to int values. */
/* Power is expressed in normalized units from the Pulse Pair Processors;*/
/* normalization is performed in the Pulse Pair code to */
/* ADC one-sided full scale. For example the system noise might average */
/* 5 counts on each i and q, so that Pnoise from the processor would be */
/* P = 2*[(5/2048)^2] = 1.2e-5 in normalized units  */
/* For Ze, the conversion from p to signal power is assumed */
/* to be linear, with a slope expressed */
/* in milliwatts per normalized power unit */

/* Scale velocity (v) and store in buffer */

        r1 = v              		/* Preload r11-->v  */
#ifdef CONSTANTS
	/* insert constant for debugging */
	r1e = V_const
#endif CONSTANTS

        r2 = kv             		/* Preload r12-->kv */
        *r13++ = a1 = *r2 * *r1		/* Scale v and store in dp buffer  */

/* Scale spectral width (sigma) and store in buffer */
        r1 = sigma            		/* Preload r11-->sigma */
#ifdef CONSTANTS
	/* insert constant for debugging */
	r1e = SIGMA_const
#endif CONSTANTS

        r2 = ks          		/* Preload r12-->ks */
        *r13++ = a1 = *r2 * *r1      	/* Scale sigma and store in dp buffer */

/* Scale normalized coherent power (ncP) and store in buffer */
        r1 = ncP             		/* Preload ncP stuff*/
#ifdef CONSTANTS
	/* insert constant for debugging */
	r1e = ncP_const
#endif CONSTANTS

        r2 = kn            
        *r13++ = a1 = *r2 * *r1	/* Scale ncP and store in dp buffer  */

/* Calculate and scale Ze counts with noise correction if NOISESUB */
/* NOTE: range correction will be added in the Collator!!! */
l2:     r1 = logP
#ifdef CONSTANTS
	/* insert constant for debugging */
	r1e = P_const
#endif CONSTANTS
	r14 = kSNR	/* *kSNR = 20.0 (i.e. 0.5 db/count) */ 
	a0 = *r1
        r3 = dbZ
        r2 = cradar
/* Ze(0.5dB) = 40 log r + 20 log p + cradar */
/* cradar = 20 log kz + 20 log krcvr  + 2*zoffset */
		/* scale by kSNR then add the composite radar constant cradar */
	*r13++ = a0 = *r2 + a0 * *r14
	nop

endgate2:  if(r18-->=0)goto calc2	/* test number of gates processed */
	   nop
	   goto dwell		/* return to process new beam */
	   nop

errint:		r1 = sync_flag
		pir = r1	/* interrupt BIM to signal out of sync error */
		r1 = r1 + 1	/* set sync flag = 1 if dsp out of sync */
		*sync_flag = r1
infloop:	goto infloop	/* sit in infinite loop until host */
		nop		/* resets entire system */
/*  end of file  */

/* Interrupt Service Routine */
/* locate at end of main loop */
/* NOTE: Interrupt is acknowledged in hardware on DSPQC */
.=0x520
/* Save the user state of the machine */
isr:	*r1_store = r1e
	*r2_store = r2e
	*r15_store = r15
	*r20_store = r20
/* Mask off INTR2 */
	r2 = pcw
	r1e = B_PORT	/* Write out B_PORT for F1,F2,F3 */
	r2 = r2 - 0x400
	pcw = r2

#ifdef MASTER
	r2 = eof
	*r1 = a0 = *r2		/* Write EOF out to Collator */
#endif MASTER
#ifdef SLAVE
/* 
  if SLAVE write out data from previous dwell first then interrupt next DSP 
*/
	r2e = INTOUT2  
	r15 = *xmtgates
	r21 = *xmt_offset
write1:	*r1 = a0 = *r10++
	if(r15-->= 0)goto write1
	nop
	r20 = r20 - 1
	if(ne)goto intr
	r20 = *prt_flag
	r15 = *xmtgates
write2:	*r1 = a0 = *r12++
	if(r15-->= 0)goto write2
	nop
	r12e = r12 - r21	/* restore data buffer to base */
intr:	a0 = *r2		/* interrupt DSP 1 */
	r10e = r10 - r21	/* restore data buffer to base */
#endif SLAVE
/* Restore the user state of the machine */
	r1e = *r1_store
	r2e = *r2_store
	r15 = *r15_store
	r20 = *r20_store
#ifdef FAKE_ISR
	goto fisr
	nop
#endif FAKE_ISR
	ireturn

















