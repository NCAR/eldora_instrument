/******************************************************************************
*                                                                             *
* Copyright (c) 1990,							      *
* 	National Center for Atmospheric Research                              *
*									      *
*******************************************************************************
/*
*		Module:	coll_dpramdefs.h
*		Author:	E. Loew
*		Date:	3/24/92
*		
*
*	Collator board definitions of dual-port RAM locations
*
*/
#include "dspaddr.h"	/* for DSP_NR_PARAMS */


#ifndef INCcoll_dpramdefs
#define  INCcoll_dpramdefs
 
/*
Special Collator DPRAM memory locations and definitions
*/
#define COL_GATES 0x50
#define COL_SAMPLES 0x52
#define COL_NUM_INT1 0x54
#define COL_NUM_INT2 0x56
#define COL_NUM_INT3 0x58
#define COL_NUM_INT4 0x5a
#define COL_CHIP_AVG1 0x5c
#define COL_CHIP_AVG2 0x5e
#define COL_CHIP_AVG3 0x60
#define COL_CHIP_AVG4 0x62
/* #define DSP_BEAM_CNT 0x68 */


/* DSP32 floating point variables */

#define COL_RZERO	0x100	/* range to gate 0 in km */
#define COL_GATE_SPACE 0x104   /*  gate spacing in km*/

/* locations of floating point values to convert time series values
 * into normalized RP7 units 
 * ADC counts/(2048 * sqrt(NumSamples))
 * (same as A,B,P) 
 */

#define COL_TS_RAW_FACT 0x108
#define COL_TS_FILT_FACT 0x10c

/* run time "pick gate" */

#define COL_PICK_GATE 0x110

/* run time frequency and staggered PRT flags */

#define COL_PRT_FLAG 0x112
#define COL_F1_FLAG 0x114
#define COL_F2_FLAG 0x116
#define COL_F3_FLAG 0x118
#define COL_F4_FLAG 0x120
#define COL_F5_FLAG 0x122
#define COL_F1_SYNC 0x11c
#define COL_F2_SYNC 0x120
#define COL_F3_SYNC 0x124

/* SPOL Specific Parameters */

#define DATA_OFF 0x130
#define POLAR_FLAG 0x134

/* Collator Data Buffer Locations */

#define DATA_BUFF_1  0x5B00 /* 0x4B00 when try TOGA COARE code */
#define DATA_BUFF_2  0x8700 /* 0x7700 when try TOGA COARE code */

/* New Architecture Collator Data Buffer Locations */

#define DB_1 0x4B00
#define DB_2 0x9AA0
#endif INCcoll_dpramdefs
