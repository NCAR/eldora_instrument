/******************************************************************************
*                                                                             *
* Copyright (c) 1990,							      *
* 	National Center for Atmospheric Research                              *
*									      *
*******************************************************************************
/*
*		Module:	dp_ramdefs.h
*		Author:	E. Loew
*		Date:	2/19/92
*		
*
*	Collator board definitions of dual-port RAM locations
*
*/
#include "dspaddr.h"	/* for DSP_NR_PARAMS */


#ifndef INCdp_ramdefs
#define  INCdp_ramdefs
/*
Special Doppler Processor RAM memory locations and definitions
*/
#define BUFFER_PTR 0x54
/* DSP32 floating point variables */
#define DP_RADAR_CONST 0xf0  /* Rp-7 radar constant for reflectivity factor */
#define DP_PNOISE	0xf4   /* processor noise in normalized RP-7 units */
#define DP_LOGP_OFFSET 0xf8   /* "full-scale level" for log P byte */
/*
 * next two paramaters must change with PRF if absolute scale factor
 * in housekeeping is to stay constant
 */
#define DP_VSCALE	0xfc	/* scaling factor for velocity byte */
#define DP_WSCALE	0x100	/* scaling factor for width byte */
#define DP_LOGP_SCALE 0x104	/* scaling factor for log power */
#define DP_PRT_FLAG   0x112     /* Staggered PRT flag */
#define DP_F1_FLAG    0x114     /* frequency 1 flag */
#define DP_F2_FLAG    0x116     /* frequency 2 flag */
#define DP_F3_FLAG    0x118     /* frequency 3 flag */
#define DP_F4_FLAG    0x120     /* frequency 4 flag */
#define DP_F5_FLAG    0x122     /* frequency 5 flag */
#define DP_SYNC_FLAG  0x11a     /* dp out of sync flag */
#define DP_EOF_RCVD   0x11c     /* eof value received */
#define DP_PICK_GATE  0x124     /* dp pick gate */
#define DP_INT_STATUS 0x126     /* dp interrupt status */
#define PPP_SYNC_FLAG 0x116     /* ppp sync flag */
#define PPP_EOF_RCVD  0x11c     /* ppp EOF received */
#define PPP_INT_STATUS 0x120    /* ppp interrupt status */
#endif INCdp_ramdefs
