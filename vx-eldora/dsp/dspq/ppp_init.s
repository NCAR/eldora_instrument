/*  
|
|       $Id$
|
|       Module: PPP_INIT.s
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
|   This is the initialization code for the ELDORA Pulse Pair Processor.
|   It is located in External Memory A and is run only after a system
|   reset to zero data buffers and compute other key variables required
|   by the main run-time routine.
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

/* TIME SERIES DEFINITIONS !!!! */

#define TIME_SERIES       /* build time series buffers and transmit */
                          /* time series data as part of trailer    */
#define no_TSGATE_FLY     /* pick time series gate on the fly */
#define NOTDEF            /* pick time series gate outside main loop */

/* END TIME SERIES DEFINITIONS */

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
* r7            pointer to data transfer buffer for PRT #2
* r8, r9	pointers to i, q lag buffers for PRT #2
* r10 		pointer to the i,q  input sample holding buffer
* r11		pointer to the time series storage buffer
* r12		pointer to input-data scaling factor
* r13		input/output port pointer
* r14		pointer for subroutine calls and local use
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
#define TS_LEN_OUT_WORDS (5*DSP_TS_LEN_OUT) /* 5 variables per sample ,,, */
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
/* Time series is stored as {Iraw,Qraw} for each sample ,,, */
/*  largest size(timser)=2*NSAMPLES*float+DSP_NR_PARAMETERS*float ,,,, */
/* if NSAMPLES=512, size < 0x3000 bytes ,,, */

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

.global gates, xmtgates, samples, norm, norm_fac
.global start, midbm_sampl, temp1, temp2, prt_flag
.global end_trailer, beam_cnt
.global zero,one, four, eight, ramp 
.global pick_gate
.global recip, recip0, ramp_i, ramp_q
.global rsamples, fsamples, tsoff, tsgates
.global tsamples, tgates, eof
.extern beam
#ifdef CHKSUM
.global chksum, chkdone, chkloop, endcode
#endif CHKSUM 
.global samoff, last_location 
/* samoff is the pointer offset into the input-sample buffer samstor ,,, */

/* .= 0x000 */
.rsect ".extA"
#ifdef ONE_WAITS        /* &&& */
                r10 = 0x0B              /* 1 Wait for A, 3 for B */
#else
                r10 = 0x09              /* 0 Wait for A, 3 for B */
#endif                  /* &&& */
 
                pcw = r10 
                ioc = 0x0554 
                dauc = 0 
                goto start
                nop 
 
/*
*  Control/status parameters: 
*  if run-time changes are made to gates, samples, pick_gate, etc
*  be sure to "allstop" and "allgo" to make the changes effective ,,, 
*  A problem with the assembler keeps us from expilticitly using all
*	the defined constants in the include files.
*/
.=DSP_PARAM_BASE /* this is normally 0x50; new locations follow  ### */
/* the number of 32bitwords of these i/o parameters */
		/* is DSP_NR_PARAMS and must agree in Collator code */
.=DSP_GATES	/* byteoffset=0*/
gates:  int 64  /* typically 64*8processors=512   &&& */
.=DSP_SAMPLES	/* byteoffset=2*/
/* the requested nr of samples: max nr of samples if INDEXED_BEAMS """*/
samples:        int 20	/* typically 16,20, or 32 */
			/* must be > 12 */
.=DSP_BEAM_CNT
beam_cnt:	int24 0
pick_gate:	int24 12
/* .=DSP_GATE_AVG_FAC  should not be less than one */
gate_avg_fac:	int24 1		/* typically 1 */
/*.=DSP_CHKSUM */	/* byteoffset=52*/
chksum:		int24 0	/*###memory checksum between start & endcode */
/* .=DSP_TIME_SERIES_CTL */
tsctl:	int24	TRAILER_PASSES	/* time series output control */
	/* min of 1 to output trailer for 264gates/12processors */
/* .= DSP_CS_SPARE0 */
cs_spare0:	int24 0x1111	/* byteoffset=        */
		int24 0x2222
/*.=DSP_MAGIC */
end_trailer:	int24 0x5AA5	/* byteoffset =      */

.align 4
.=0x112
prt_flag: 	int 0		/* default is no staggered PRT operation */ 

.align 4
.=0x0200
start:

/*TEST an idea to synchronize DSP32Cs use first PRI for init stuff ,,, */
#ifdef SLAVE_CLR	/* ### */
        r10e = CLRINT1          /*  Clear all int &&& ,,, */
        r20 = *r10      /* clear allint before starting &&& */
#endif

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

	r3e = FIFOCLR   /* Set up pointers */
        r13e = A_PORT
        r16 = *samples		/* sample counter """ */ 
        r4 = *r3                /* Clear FIFO   */
	r7 = *samples
	r19 = *gates
	r17 = *gates
	r7 = r7/2
	*midbm_sampl = r7	/* samples/2  */
	r19 = r19*2
	r19 = r19 + r17
	r19 = r19 + 1
	*xmtgates = r19 /* xmtgates = (3 * *gates + 1(for eof)) */
	r16 = r16 - 2
        *tsamples = r16 	/* tsamples = samples - 2 ,was "samples-3" */
	r17 = r17 - 2
        *tgates = r17   	/* tgates = tgates - 2 */
        r21e = 0	/* beam counter */

#ifdef TIME_SERIES

/* Include time series data in transmit gate count (*xmtgates) */
/* and compute time series offsets into data buffers for both PRT's */

	r19 = *xmtgates
	r7 = *samples
	r18 = *xmtgates
	r19 = r19 - 1	/* *xmtgates - 1 */
	r19 = r19 * 2
	r19 = r19 * 2
	*tsoff = r19    /* time series offset = (4*3*gates)=(4*(*xmtgates-1)) */
	r7 = r7 * 2
	*tsgates = r7
	r18 = r18 + r7
	*xmtgates = r18	/* *xmtgates now includes time series data to be transmitted */

#endif TIME_SERIES

/*    
Zero out the integrators    
*/    
	r4 = zero   
        a0 = *r4   
        r15 = *tgates   
	r1e = i_lag1_prt1
	r2e = q_lag1_prt1       
	r8e = i_lag1_prt2       
	r9e = q_lag1_prt2       
 	r5e = data_buffer1_base
        r6e = data_buffer2_base
	r4e = data_buffer3_base
	r7e = data_buffer4_base
   
cloop:  do 3,2          /* do next  3+1 instructions 2+1 times */
        *r5++ = a0 = a0
        *r6++ = a0 = a0
	*r4++ = a0 = a0
	*r7++ = a0 = a0
	*r1++ = a0 = a0
	*r2++ = a0 = a0
	*r8++ = a0 = a0
	*r9++ = a0 = a0
        if(r15-->=0)goto cloop   
        nop   

#ifdef TIME_SERIES 

/* Zero out time series buffers */

	r15 = *tsamples
tloop:  do 3,1          /* do next  3+1 instructions 1+1 times */
        *r5++ = a0 = a0
        *r6++ = a0 = a0
	*r4++ = a0 = a0
	*r7++ = a0 = a0
        if(r15-->=0)goto tloop   
        nop   
#endif TIME_SERIES	


/* Compute I,Q scale factor */

/* Calculate the reciprocal of samples */

recip0:   r1 = samples
recip:    r2 = fsamples
          *r2 = a0 = float(*r1)
          call _invf(r14)
          nop
     int24  fsamples, rsamples
/* results not available in memory for 3 instruction times */
.align 4

/* Take square root */
          call _sqrtf(r14)
          nop
     int24  locall, rsamples, norm_fac	/* ### old srsamp */
.align 4 
 
/* Normalize to 2048 max a/d counts * sqrt(samples)*/
        r1 = norm
        r12 = srsamp  
        r3 = norm_fac			/* ### old srsamp */
	*r12 = a0 = *r3 * *r1
       
#ifdef NOTDEF /* move this calculation inside beam loop, */
/*  so gate can be picked on the fly ,,, */
/* but of gate is picked only before 'allgo' then it can be executed */
/* here to save time in the operational loop */
     /* pointers to the clutter filter inputs {I,Q,AGC} and ,,,*/
	/*   outputs {Icf, Qcf} for the time series  ,,, */
        r15 = *pick_gate /* calculate "pick_gate" offset into buffers */
        r14e = samoff
        r15 = r15*2      
        r15 = r15*2
        r15 = r15*2
	*r14 = r15

#endif NOTDEF

/* TEST another sync mechanism, clear input fifo just before the operational */
/* loop starts */

#ifdef SLAVE_CLR	/* ### */
        r10e = CLRINT1          /*  Clear all int &&& ,,, */
        r20 = *r10      /* clear allint before starting &&& */
#endif

/* Initialize some key pointers used in main routine */

	r17 = *tgates
	r20 = *four
	r3 = *xmtgates  
	r4e = data_buffer1_base
	r5e = data_buffer3_base
	r6e = data_buffer4_base
	r7e = data_buffer2_base	
	r3 = r3*2	/* r3 to store no.(transmitted gates+eof)*4bytes */ 
	r3 = r3*2	/* r3 used to restore transmit buffer to base 	 */
	r20 = r3 - r20  /* r20 is offset to last address in data buffers */

/* Place eof at end of stored data in all data buffers */
/* This position will vary with the number of gates being processed */

	r8 = eof	
	r4 = r4 + r20	/* move to last used address in buffer */
	r5 = r5 + r20	/* move to last used address in buffer */
	r6 = r6 + r20	/* move to last used address in buffer */
	r7 = r7 + r20	/* move to last used address in buffer */
	*r4 = a0 = *r8  /* write eof in data buffer */
	*r5 = a0 = *r8  /* write eof in data buffer */
	*r6 = a0 = *r8  /* write eof in data buffer */
	*r7 = a0 = *r8	/* write eof in data buffer */

/* Reset data buffer pointers to base addresses */

	r4e = data_buffer1_base
	r5e = data_buffer3_base
	r6e = data_buffer4_base
	r7e = data_buffer2_base	

/* Branch to Main Loop */

	goto beam
	nop

/* Storage  of  constants  */   

.align 4
	eof:    float 1024.0 
     	norm: 	float 0.00048828125  
     	zero:   float 0.0
     	one:	float 1.0	/* ### constant added to list */
	two:	float 2.0
	eight:	float 8.0
	two_seven:  float 128.0
	intone:		int24 1
/****************SUBROUTINES***************/ 
 
#define QUADSUBS
#include "colquadsubs.h"

.align 4
endcode:	int24 0x5555
 
 
/* Temporary storage  -  variables  */   
.align 4
     temp1:    int 0
     temp2:    int 0
     xmtgates: int 0
     midbm_sampl:  int 0
     tsoff:   int 0
     tsgates: int 0
     four:    int 4
.align 4
     i_samp:   float 0.0   
     q_samp:   float 0.0   
     rsamples: float 0.0  
     fsamples: float 0.0  
     d3cc:     float 0.0  
     locallAmem:   2*float 0.0  
     norm_fac: float 0.0	/* ### */
     tsamples:       int 0
     tgates: int 0

.align 4
	samoff:		int24 0		/* offset into samstor bufer ,,, */
	ramp_i:		float 1.0	/* for simulation */
	ramp_q:		float 1.0

	f_gate_avg_fac: float 0.0	/* ###  for gate averaging */
	r_gate_avg_fac: float 0.0	/* ### */

	ramp:		float 1.0	/* ,,, ramp for testing in OLD_CODE*/
	ptr_ptot_avgAmem:	int24 0

	garbage_dump:	int24 0

	last_location:	int24 0x5555	/* ,,, */


/* end of file */

