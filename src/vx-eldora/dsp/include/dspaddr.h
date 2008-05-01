/******************************************************************************
*                                                                             *
* Copyright (c) 1990,							      *
* 	National Center for Atmospheric Research                              *
*									      *
*******************************************************************************
                                                                               
$Header$

*******************************************************************************
* Revision history:
* $Log$
 * Revision 1.2  1994/07/18  20:11:02  eric
 * *** empty log message ***
 *
 * Revision 1.18  91/03/19  07:17:08  vanandel
 * added offset for time series data
 * 
 * Revision 1.17  90/12/06  11:07:04  vanandel
 * constants for DSP index beam control
 * 
 * Revision 1.16  90/11/30  11:42:56  vanandel
 * change to use gin_NumOfQuads, since this is
 * defined on all procesors
 * 
 * Revision 1.15  90/11/26  13:05:33  vanandel
 * added various constants for support of full control,
 * indexed beam code
 * 
 * Revision 1.14  90/11/19  14:19:16  vanandel
 * add  globals to indicate how many samples were actually integrated,
 * and to control operation of indexing
 * 
 * Revision 1.13  90/11/13  16:27:31  gagnon
 * *** empty log message ***
 * 
 * Revision 1.12  90/10/01  07:53:10  vanandel
 * added offsets for GATES, SAMPLES, FILTER, for debug use
 * 
 * Revision 1.11  90/08/06  10:04:10  vanandel
 * added DSP_TS_LEN_OUT for timeseries
 * 
 * Revision 1.10  90/05/21  12:53:29  vanandel
 * added COL_STAT_ADDR macro
 * 
 * Revision 1.9  90/05/10  17:18:49  vanandel
 * added checksum location 
 * 
 * Revision 1.8  90/05/09  14:57:07  vanandel
 * added gate averaging control word
 * 
 * Revision 1.7  90/05/04  17:55:49  vanandel
 * fixed comments and several typos
 * 
 * Revision 1.6  90/05/03  15:52:16  vanandel
 * add gate selection parameter
 * 
 * Revision 1.5  90/05/01  12:31:56  vanandel
 * added averages for A & B
 * 
 * Revision 1.4  90/04/26  13:01:05  vanandel
 * added power counts and 'MAGIC' (sanity bit pattern) 
 * 
 * Revision 1.3  90/04/23  18:27:43  vanandel
 * added defines for I and Q offset stats
 * 
 * Revision 1.2  90/02/24  15:27:35  vanandel
 * malformed comment
 * 
 * Revision 1.1  90/02/24  15:20:43  vanandel
 * Initial revision
 * 
* 
*********************************END OF RCS INFO******************************/

#ifndef INCdspaddr
#define INCdspaddr

/*
 * define locations in quad/collator code used for parameters set by host
 *
 * (these are not defined in dspdefs.h, since they are conventions and
 *  not hardwired addresses)
 * 
 * WARNING- these values are also assumed in various DSP32C routines! 
 */

#define DSP_PARAM_BASE  0x50

#define DSP_GATES_OFF 0
#define DSP_GATES (DSP_PARAM_BASE + DSP_GATES_OFF)

#define DSP_SAMPLES_OFF 0x2
#define DSP_SAMPLES (DSP_PARAM_BASE + DSP_SAMPLES_OFF)

#define DSP_FILTER_OFF 0x4
#define DSP_FILTER (DSP_PARAM_BASE + DSP_FILTER_OFF)

#define DSP_SPARE0_OFF   0x8

#define DSP_FID_OFF 0xC	/* offset from base to filter id */
#define DSP_FID (DSP_PARAM_BASE + DSP_FID_OFF)

#define DSP_I_SUM_OFF 0x10	/* offset from base to sum of I */
#define DSP_I_SUM (DSP_PARAM_BASE + DSP_I_SUM_OFF)

#define DSP_Q_SUM_OFF 0x14	/* offset from base to sum of Q */
#define DSP_Q_SUM (DSP_PARAM_BASE + DSP_Q_SUM_OFF)

#define DSP_BEAM_CNT_OFF 0x18	/* offset from base to beam counter */
#define DSP_BEAM_CNT (DSP_PARAM_BASE +DSP_BEAM_CNT_OFF)


#define DSP_AVG_A_CNT_OFF (0x1c)	/* average A counts */
#define DSP_AVG_A_CNT (DSP_PARAM_BASE + DSP_AVG_A_CNT_OFF)

#define DSP_AVG_B_CNT_OFF (0x20)	/* average B counts */
#define DSP_AVG_B_CNT (DSP_PARAM_BASE + DSP_AVG_B_CNT_OFF)

#define DSP_AVG_PWR_CNT_OFF (0x24)	/* average filtered power counts */
#define DSP_AVG_PWR_CNT (DSP_PARAM_BASE + DSP_AVG_PWR_CNT_OFF)

#define DSP_AVG_PTOT_CNT_OFF (0x28)	/* average raw power counts */
#define DSP_AVG_PTOTWR_CNT (DSP_PARAM_BASE + DSP_AVG_PWR_CNT_OFF)

#define DSP_GATE_SEL_OFF (0x2C)	/* select which gate to dump A,B,P */
#define DSP_GATE_SEL (DSP_PARAM_BASE + DSP_GATE_SEL_OFF)

/* gate averaging doesn't work until we get batched processing working */
#define DSP_GATE_AVG_FAC_OFF (0x30)
#define DSP_GATE_AVG_FAC (DSP_PARAM_BASE + DSP_GATE_AVG_FAC_OFF)

#define DSP_CHKSUM_OFF (0x34)
#define DSP_CHKSUM  (DSP_PARAM_BASE + DSP_CHKSUM_OFF)

/* this location holds the number of samples actually processed
 * which may be less than commanded, if running indexed beams
 */
#define DSP_SAMPLES_INTEGRATED_OFF (0x38)
#define DSP_SAMPLES_INTEGRATED (DSP_PARAM_BASE + DSP_SAMP_INT_OFF)

/* this is a mask set to 0x03 to turn indexing ON */
#define DSP_INDEX_ON 0x03
#define DSP_INDEX_OFF 0

#define DSP_INDEX_CTL_OFF (0x3C)
#define DSP_INDEX_CTL (DSP_PARAM_BASE + DSP_INDEX_CTL_OFF)

#define DSP_TS_CTL_OFF   0x40
#define DSP_TS_CTL (DSP_PARAM_BASE + DSP_TS_CTL_OFF)

#define DSP_SPARE1_OFF	0x44

#define DSP_SPARE2_OFF	0x48

#define DSP_MAGIC_OFF (0x4C)
#define DSP_MAGIC	(DSP_PARAM_BASE + DSP_MAGIC_OFF)

/* # of 4 byte parameters to copy from quad to collator */
#define DSP_NR_PARAMS ((DSP_MAGIC_OFF)/4 + 1)

/* time series data follows the other trailer info */
#define DSP_TS_OFF (0x50)
#define DSP_TS	(DSP_PARAM_BASE + DSP_TS_OFF)

/* length of time series sequence */

#define DSP_TS_LEN_OUT (64)
#define DSP_TS_LEN_OUTm1 (63)


#define DSP_LOG_PWR_OFF	(0x3b4)

/* this macro gets the address of a quad processor's statistic, from
 * collator's dual port ram
 */
#ifdef VX
extern int gin_NumOfQuads;
#endif
#define COL_STAT_ADDR(varOff,i) ((ULONG *)(CollBases[0] + COL_STAT_BASE +\
	varOff * gin_NumOfQuads*4 ) +i)

#endif
