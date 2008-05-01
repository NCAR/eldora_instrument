/*
 *	$Id$
 *
 *	Module:	COL_DPRAMDEFS
 *	Original Author: Joe VanAndel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date: 		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:
 *
 *	Collator board definitions of dual-port RAM locations
 *
 *        
 */

#include "/ELDORA/dsp/include/dspaddr.h"    /* for DSP_NR_PARAMS */


#ifndef INCcol_dpramdefs
#define  INCcol_dpramdefs
#define HSKP_SIZE 200	/* bytes */
 
/*
Special Collator DPRAM memory locations and definitions
*/
#define BUFFER_PTR 0x54
#define COL_GATE_SKIP_FACT 0x5c	/* 0 -> process all, 1-> do every other */

/* DSP32 floating point variables */
#define COL_RZERO	0x100	/* range to gate 0 in km */
#define COL_GATE_SPACE 0x104   /*  gate spacing in km*/
#define COL_RADAR_CONST 0x108  /* Rp-7 radar constant for reflectivity factor */
#define COL_PNOISE	0x10c   /* processor noise in normalized RP-7 units */
#define COL_LOGP_OFFSET 0x110   /* "full-scale level" for log P byte */

/*
 * next two paramaters must change with PRF if absolute scale factor
 * in housekeeping is to stay constant
 */
#define COL_VSCALE	0x114	/* scaling factor for velocity byte */
#define COL_WSCALE	0x118	/* scaling factor for width byte */

#define COL_LOGP_SCALE 0x11C	/* scaling factor for log power */

/* locations of floating point values to convert time series values
 * into normalized RP7 units 
 * ADC counts/(2048 * sqrt(NumSamples))
 * (same as A,B,P) 
 */

#define COL_TS_RAW_FACT 0x120
#define COL_TS_FILT_FACT 0x124
/*
 * since the collator only has enough processor power to do 3 FFTs,
 * we need to be able to tell it which quad processor's data (0-7)
 * to process as the first
 *
 * this lets us slide the computed 3 ffts within the 8 selected gates
 */ 

#define COL_START_PER_PROC 0x128	/* starting processor, default to 0*/

/* # number of gates to compute periodigrams for 
 * (practical maximum is 3 right now) 
 */
#define COL_NR_PER	   0x12c	

/* switch to enable processing 2nd moment (NCP and width) 
 * non-zero means process 2nd moment 
 *
 * this needs to be switchable to save ~40% when running a large number of
 * gates
 */
#define COL_PROCESS_2ND_MOMENT	0x130	

/*
 * # of samples, from the trailer
 */
#define COL_RCVD_SAMPLES 0x134

/*
 * non-zero means read time series from quads 
 */
#define COL_TIME_SERIES_CTL 0x138

/* the # of quad processors in the system (typically, 8 or 12) */
#define COL_QUAD_PROC_CNT	0x13C

#define BUF_OFFSET 0x3000

#define HSKP_BASE  0xF38
#define	HSKP1	   HSKP_BASE	/* Start of Buffer 1 hskp header 	*/
#define HSKP2	   (HSKP_BASE + BUF_OFFSET) /* Start of Buffer 2 hskp header	*/

#define DATA1	   (HSKP1 + HSKP_SIZE) /* start of Buffer 1 data area		*/
#define DATA2	   (HSKP2 + HSKP_SIZE) /* Start of Buffer 2 data area		*/

#define COL_STAT_BASE 0xA000


#define COL_TS_SZ_SAMPLE_PT (5)	/* # of floats per sample */
/*
 * time series format:
 *
 * Iraw[p0][sam0] Iraw[p1][sam0] . . . Iraw[p11][sam0]
 * Qraw[p0][sam0] Qraw[p1][sam0] . . . Qraw[p11][sam0]
 * AGC[p0][sam0] AGC[p1][sam0]   . . . AGC[p11][sam0]
 * Ifilt[p0][sam0] Ifilt[p1][sam0] . . .  Ifilt[p11][sam0]
 * Qfilt[p0][sam0] Qfilt[p1][sam0] . . .  Qfilt[p11][sam0]
 *
 * Iraw[p0][sam1] Iraw[p1][sam1] . . . Iraw[p11][sam1]
 * Qraw[p0][sam1] Qraw[p1][sam1] . . . Qraw[p11][sam1]
 * AGC[p0][sam1] AGC[p1][sam1]   . . . AGC[p11][sam1]
 * Ifilt[p0][sam1] Ifilt[p1][sam1] . . .  Ifilt[p11][sam1]
 * Qfilt[p0][sam1] Qfilt[p1][sam1] . . .  Qfilt[p11][sam1]
 *
 * Note: the trailer parameter 'DSP_SAMPLES_INTEGRATED' indicates how
 * many valid timeseries samples are present
 *
 *
 */


/*
 * supply a base address so we can copy all the data into a sep. buffer, and
 * then reformat it.
 * Since these quantities are IEEE floats, setup the default access as such
 *
 * We rely on C pointer math to address the correct location,
 * since 'C' multiplies all offsets by the size of the base type (e.g. float)
 */


#define COL_TS_IRAW(base, numProcs, proc, sam) ( (float *)(base) + \
	((sam) * COL_TS_SZ_SAMPLE_PT * (numProcs)) +  (proc))

#define COL_TS_QRAW(base, numProcs, proc, sam) ((float *)(base) +\
	((((sam) * COL_TS_SZ_SAMPLE_PT) + 1) * (numProcs)) +  (proc))

#define COL_TS_AGC(base, numProcs, proc, sam) ((float *)(base) + \
	((((sam) * COL_TS_SZ_SAMPLE_PT) + 2) * (numProcs)) +  (proc))

#define COL_TS_IFILT(base, numProcs, proc, sam)((float *)(base) + \
	((((sam) * COL_TS_SZ_SAMPLE_PT) + 3) * (numProcs)) +  (proc))

#define COL_TS_QFILT(base, numProcs, proc, sam)((float *)base + \
	((((sam) * COL_TS_SZ_SAMPLE_PT) + 4) * (numProcs)) +  (proc))


/* each quad DSP processor can have this much stats
 * (upper bound)
 */
#define COL_STAT_SIZE 0x80

/* each quad processors parameters, starting at DSP_PARAM_BASE (0x50)
 * are copied to the collators dual ported ram,
 * into its own buffer
 */

/* periodigram buffer  */
#define COL_PER_BASE	0x7000

#define COL_NFFT_WORDS 64	/* how many points */
#define COL_MFFT_TWO 6		/* corresponding power of two */

#define COL_PER_OFF ((COL_NFFT_WORDS/2)*4)
#define COL_PER_SIZE ((COL_NFFT_WORDS *2)*4)

/* these are the ffts for the picked gate, and the two following */


#define COL_NUM_PER 3    /* # of FFTs computed by the collator code  */

#define COL_RAW_PER(n)	(COL_PER_BASE + COL_PER_OFF + 2*(n)*COL_PER_SIZE)
#define COL_FILT_PER(n)	(COL_PER_BASE + COL_PER_OFF + (2*(n)+1)*COL_PER_SIZE)




#endif INCcol_dpramdefs
