/*  
|
|       $Id$
|
|       Module: PPP_MAIN.s
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
|   This is the real-time code for the ELDORA Pulse Pair Processor. 
|   This code is located entirely in internal RAM0 to minimize DSP32C 
|   conflict wait states.Values a,b,and p are computed for both single 
|   and staggered PRT's using a lag1 pulse pair algorithm. These values 
|   are then passed on to the Doppler Processor for further computation.
*/

#include "ELDRP7.h" 	
#include "dspaddr.h" /* ### parameter locations */

#define CUT_TRAILER 6	/* to cut off early trailer */

/* Here's the key definitions in the Pulse Pair Processor Code */
#define ELDORA
#define no_SIM  /* sets up code to be compiled for simulation */

#ifdef ELDORA   /* ELDORA assembler switches */
#define PPP_NR_DSPS  4     /* no. of Pulse Pair Processor DSP's */
#define TRAILER_PASSES  1  /* no. of mid-beam passes; if = 1 then */
#define INTERRUPT         /*                            */
#define no_ONE_WAIT      /* use one wait state for Amem */
#define SLAVE             /* invoke slave mode -- wait for beprf */
#define SLAVE_CLR         /* clear allint at end of sweep */
#define No_INPUT_ONES     /* to input 1.0 for i,q in off-line simulation */
#define No_CHKSUM       /* calculate code checksum */
#define No_RAMP           /* places a ramp in i if INPUT_ONES */

/* TIME_SERIES DEFINITIONS !!! */

#define TIME_SERIES       /* build time series buffers and transmit */
#define no_TSGATE_FLY     /* pick time series gate on the fly */

/* END TIME_SERIES DEFINITIONS */

#define VEL
#define DSP_NR_PARAMSm1 (DSP_NR_PARAMS-1)

#endif ELDORA

#ifdef SIM      /* sets up appropriate switches for simulator */
#undef SLAVE
#define INPUT_ONES
#define RAMP
#endif SIM

/*
* Here's a little guide to external memory A:
* .=0000
*  executable code to set up the DSP32C processor control word
* .=0050
* control/status parameters and variables for outside access
* .=0200
*		initialization code
*		subroutines ( abt 300 bytes )
*		temporary variable storage ( abt 100 bytes )
*
* .=1000 to 1DFF
*		ping pong buffer to hold a,b,p (avg) for PRT 1
*		(3 values * NGATES * float bytes) and 
*		I, Q time series data for PRT 1
*		(2 values * SAMPLES * float bytes)
*		(for NGATES = 256 & SAMPLES = 64 buffer = 0xE00 bytes)
* .=1E00 to 2BFF
*		ping pong buffer to hold a,b,p (avg) for PRT 2
*		(3 values * NGATES * float bytes)
*		I, Q time series data for PRT 2
*		(2 values * SAMPLES * float bytes)
*		(for NGATES = 256 & SAMPLES = 64 buffer = 0xE00 bytes)

* .=2C00 to 39FF
*		ping pong buffer to hold a,b,p (avg) for PRT 1
*		(for NGATES = 256, buffer = 0xC00 bytes)
*		I, Q time series data for PRT 1
*		(2 values * SAMPLES * float bytes)
*		(for NGATES = 256 & SAMPLES = 64 buffer = 0xE00 bytes)

* .=3A00 to 47FF
*		ping pong buffer to hold a,b,p (avg) for PRT 2
*		(3 values * NGATES * float bytes)
*		I, Q time series data for PRT 2
*		(2 values * SAMPLES * float bytes)
*		(for NGATES = 256 & SAMPLES = 64 buffer = 0xE00 bytes)
* .=4800 to 4FFF
*		I & Q storage buffer for PRT 1
*		(2 values * NGATES * float bytes)
*		(for NGATES = 256, buffer = 0x800 bytes)
* .=5000 to 57FF
*		I & Q storage buffer for PRT 2
*		(2 values * NGATES * float bytes)
*		(for NGATES = 256, buffer = 0x800 bytes)
* .=FFE000 to FFE3FF
*		inphase lag buffer for PRT 2 (located in RAM 2)
*		(NGATES * float bytes)
*		(for NGATES = 256, buffer=0x400 bytes)
* .=FFE400 to FFE7FF
*		quadrature lag buffer for PRT 2 (located in RAM 2)
*		(NGATES * float bytes)
*		(for NGATES = 256, buffer=0x400 bytes)
* .=FFF000 to FFF7FFF
*		code for time critical main loop (located in RAM 0
*		for speed). approx 2K bytes max.
* .=FFF800 to FFFBFF
*		inphase lag buffer for PRT 1 (located in RAM 1)
*		(NGATES * float bytes)
*		(for NGATES = 256, buffer=0x400 bytes)
* .=FFFC00 to FFFFFF
*		quadrature lag buffer for PRT 1 (located in RAM 1)
*		(NGATES * float bytes)
*		(for NGATES = 256, buffer=0x400 bytes)
*/
/*
*	External memory B: i/o space as identified in include files.
*/
/*
* a guide to register usage:
* r1, r2        pointer to i, q lag buffers for PRT #1
* r3		local storage register
* r4            pointer to data transfer buffer for PRT #1
* r5    	pointer to a, b, p data buffer base for PRT #1
* r6            pointer to a, b, p data buffer base for PRT #2
* r7            pointer to data transfer buffer for PRT #1
* r8, r9	pointers to i, q lag buffers for PRT #2
* r10 		pointer to the i,q  input sample holding buffer
* r11		pointer to the time series storage buffer for PRT #1
* r12		pointer to input-data scaling factor
* r13		input/output port pointer
* r14		pointer for subroutine calls and pointer to the time series 
		storage buffer for PRT #2
* r15		local work register
* r16, r17	sample, gate loop counters 
* r18		data transfer counter
* r19		mid-beam sample counter
* r20		local work register
* r21 		beam sanity counter
* r22		gate averaging counter
*/

#define NGATES	256		/* MAX nr of gates for this dsp32c ,,, */
#define LAG_LEN_BYTES	(4*NGATES)	/* ,,, */	
#define AVG_LEN_BYTES	(4*NGATES)	/* ,,, */	
#define DLY_LEN_BYTES	(8*NGATES)	/* ,,, */
/*
* DSP_TS_LEN_OUT  outputted length of time series in samples typically 64
*/
#define TS_LEN_OUT_WORDS (2*DSP_TS_LEN_OUT) /* 2 variables per sample ,,, */
#define TS_LEN_OUT_WORDSm1	(TS_LEN_OUT_WORDS-1)

/* Averager arrays */ 
#define data_buffer1_base 0x1000	/* base of ping pong buffer for PRT 1 */
#define data_buffer2_base 0x1E00	/* base of ping pong buffer for PRT 2 */
#define data_buffer3_base 0x2C00	/* base of ping pong buffer for PRT 1 */
#define data_buffer4_base 0x3A00	/* base of ping pong buffer for PRT 2 */
 
/* PPP lag arrays */ 
#define i_lag1_prt1 0xFFF800 
#define q_lag1_prt1 0xFFFC00
#define i_lag1_prt2 0xFFE000 
#define q_lag1_prt2 0xFFE400
 
/* time series arrays*/
#define MAX_LEN_TIME_SERIES 64 /* NOTE: for zeroext short time series,,,*/ 
#define MAX_LEN_TIME_SERIES_WORDS (2*MAX_LEN_TIME_SERIES)
		/* MAX nr of longwords, at 2 words per time series point ,,, */
/* Time series is stored as Iraw,Qraw for each sample ,,, */
/*  largest size(timser)=2*NSAMPLES*float+DSP_NR_PARAMETERS*float ,,,, */

#define locall	0xFFF7F8  /* reserve two longwords for subroutine scratch*/
			  /* at the end of RAM 0 */
/* Input sample holding array */
/* structured as {I,Q} for each gate ,,, */
/* Thus, size(samstor) = 2*NGATES*float   ,,, */
#define samstor    0x4800	
#define srsamp     0xFFF7F0  /* locate near the end of RAM 0 for speed */

/* masks to use on RFLAG word to identify the mid-beam or end-beam PRI */
#define MASK_MID_BEAM	0x0001		/* rflag0 is mid-beam flag */
#define MASK_END_BEAM	0x0002		/*  rflag1 is end beam flag */

.extern gates, xmtgates, samples, norm, norm_fac, prt_flag
.extern start, midbm_sampl, temp1, temp2
.global beam, wait1, again1, ppp1, gate1, ray, contprt1
/* UNCOMMENT WHEN PRT 2 CODE IS ADDED
.global wait2, again2, ppp2, gate2, contprt2, gtcnt2
*/
.extern end_trailer, beam_cnt
.extern zero,one, eight, ramp 
.extern pick_gate
.extern recip, recip0, ramp_i, ramp_q
.extern rsamples, fsamples
.extern tsamples, tgates, tsoff

#ifdef CHKSUM
.extern chksum, chkdone, chkloop, endcode
#endif CHKSUM 
.extern samoff, last_location /* ,,, */
/* samoff is the pointer offset into the input-sample buffer samstor ,,, */


/************************************************************************
 *  									*
 *                     START MAIN LOOP HERE !!!!                        *
 *									*
 ************************************************************************/

.align 4
/* main loop executes from RAM0 for speed */
.rsect ".ram0"
beam:
/* Toggle PDF (FSTPLS of ADC) */

	pdr = r20
	2*nop
	r20 = pdr

/* reset sample counter before the very first PRT of dwell */
        r16 = *tsamples
/* reset data transfer and mid-beam sample counters */
	r18 = *xmtgates
	r19 = *midbm_sampl

	/* NOTE: this analysis gate select can be moved up */
#ifdef TSGATE_FLY

        r15 = *pick_gate /* calculate "pick_gate" offset into buffers */
        r14e = samoff
        r15 = r15*2     
        r15 = r15*2
        r15 = r15*2
	*r14 = r15

#endif TSGATE_FLY

#ifdef RAMP
	/* initialize test ramp variable to zero ,,, */
	/* normally, for the input_ones option, ramp==1.0 ,,, */
	nop
	r13e = zero
	a0 = *r13
	r11e = ramp
	nop
	*r11 = a0 = a0
#endif RAMP

	/* swap the data storage buffers for both PRT 1 and PRT 2 */
	
	*temp1 = r4
	r4 = r5
	r5 = *temp1
	*temp2 = r7
	r7 = r6
	r6 = *temp2

#ifdef TIME_SERIES

	/* swap the time series storage buffers for both PRT 1 and PRT 2 */		
	r20 = *tsoff  	
	nop    			/* can replace with useful instr */
	r11e = r5 + r20		/*initialize time series pointer for PRT #1 */
	r14e = r6 + r20		/*initialize time series pointer for PRT #2 */

#endif TIME_SERIES


#ifdef SLAVE	/* ### */
again1:   if(ireq1_hi) goto again1      /* wait for beprf to set allint &&&*/
          nop                           /* &&& */
#else
again1:	  nop
#endif SLAVE

/* Reset pointers to base of buffers */

          r5 = *temp1
	  r6 = *temp2
          r2e = q_lag1_prt1
          r1e = i_lag1_prt1
	  r10e = samstor  /* start pointer to {I,Q} sample buffer ,,, */

/* THIS LOOP USES R4, R12, R13 as scratch registers */
/* Additions to sum i and q ### */
contprt1:  if(syc)goto contprt1       /* Wait for data in FIFO */
	r13e = A_PORT
#ifdef INPUT_ONES
	r20e = *r13
	r13e = ramp_i
#endif
        *r10++=a0=*r13  		/* r10 I -> Q  ,,, */

wait1: if(syc)goto wait1
	r13e = A_PORT
#ifdef INPUT_ONES
	r20e = *r13
	r13e = ramp_q
#endif
        *r10-- = a3 = *r13	/* r10 Q -> I */	
	nop                     /* for latency can sub in useful instr */



/* scale I and Q by 1/(sqrtM * 2048) before pulse pair algorithm */
	r12 = srsamp		/* scale factor */
	a0 = *r10++ * *r12
	a3 = *r10++ * *r12
	2*nop			/* for latency can replace with useful instr */


/* Pulse-Pair Algorithm */
    
ppp1:      
#ifdef VEL
        a1 = *r5++ + a0 * *r1           /* Real averager */
        a1 = a1 + a3 * *r2
 
        a2 = *r5-- + a3 * *r1           /* Imag averager */
	*r5++ = a1 = a1
        *r5++ = a2 = a2 - a0 * *r2
        *r1++ = a0 = a0
 
#else
        5*nop   /* Simulating velocity comps */
#endif
 
	a1 = *r5 + a3 * a3            /* Power averager */     
	*r5++ = a1 = a1 +  a0 * a0  
        *r2++ = a3 = a3

/* 
   Write out one datum of a,b,or p each gate until buffer is empty 
   and pass daisy chain token to next DP DSP32C
*/

write1: r18 = r18 - 1
	if(mi)goto gate1
	r13e = B_SH1		/* write out B_PORT and pass daisy chain */
	*r13 = a2 = *r4
	r12 = zero
	*r4++ = a0 = *r12
 
gate1:	if(r17-->=0)goto contprt1  /* Test gates counter */
	nop

#ifdef TIME_SERIES

                /* transfer 2 float samples to time series storage ,,, */
                /* 2 input (I,Q) */
                r10e = samoff
                r15 = *r10
                r10e = samstor
                r10e = r10 + r15
                do 0, 1       /* do 0+1 instructions 1+1  times */
                *r11++ = a0 = *r10++   /* transfer I, Q, to time series buffer */

#endif TIME_SERIES


                r17 = *tgates   /* Reset gates counter */

#ifdef SLAVE_CLR 	
        	r10e = CLRINT1          /*  Clear all int ,,, */
                r20 = *r10      /* reset allint for next sample &&& */
#endif

#ifdef RAMP	/* increment the ramp variable each pulse */
		r10e = one
		a0 = *r10
		r10e = ramp
		nop
		*r10 = a0 = *r10 + a0
#endif RAMP

/* Test for Mid-Beam and send interrupt to Collator if detected */

	r19 = r19 - 1
	if(ne)goto ray
	nop

#ifdef FORE_INTERRUPT

	pir = r7	/* send mid-beam interrupt to Collator */

#endif FORE_INTERRUPT
ray:   if(r16-->=0)goto again1 /* Test samples counter */
                nop


#ifdef INTERRUPT
          	r7 = pir   /* Clear mid-beam interrupt if host hasn't */
		/* normally dismissed by the host 680x0 reading from PDR */
		/* note psbl latency problem if this instruction is absent*/
#endif
		r5 = eof
		r4 = r4 - 4
		*r4++ = a0 = *r5	/* restore eof to end of buffer */ 
		r5 = *temp1             /* restore r5 to buffer base */
		r4 = r4 - r3            /* restore r4 to buffer base */
        	r21e = r21 + 1		/* bump beam counter */
#ifdef AFT_INTERRUPT

	pir = r7	/* send mid-beam interrupt to Collator */

#endif AFT_INTERRUPT
	        goto beam
                nop             
 
 

/* end of file */

