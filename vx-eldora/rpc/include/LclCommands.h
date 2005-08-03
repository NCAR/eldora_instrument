/*
 *	$Id$
 *
 *	Module:		 LclCommands.h
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1994/09/06  16:14:10  thor
 * Many changes.
 *
 * Revision 1.1  1992/06/25  17:39:01  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCLclCommandsh
#define INCLclCommandsh

#define DC_REMOVAL 0x10
#define DC_1       0x20
#define DC_2       0x40
#define DC_3       0x80
#define AD_CHAN_LO 0x100
#define AD_CHAN_HI 0x200
#define FFT        0x400
#define FFT_1      0x800
#define FFT_2      0x1000
#define FFT_3      0x2000
#define SLOW_CAL   0x4000
#define RESYNC     0x8000
#define FAST_CAL   0x10000
#define LOAD       0x20000
#define TSGATE     0x40000
#define DC_NULL    0xFFFFFC0F
#define FFT_NULL   0xFFFFC7FF

/* TASS defines */

#define TS_ON           0x40000     /* Collect time series data */

#define TS_OFF          0x80000     /* Turn off time series data collection */

/* Status Block Defines */

#define DSP_LOAD     0x1
#define END_OF_BEAM  0x2
#define MID_BEAM     0x4
#define DC_BUSY      0x8
#define DC_STAT      0x10
#define DP_SYNC      0x20
#define COLL_SYNC    0x40
#define DC_BUSY_MASK 0xF7
#define DC_STAT_MASK 0xEF
#define INIT_MCPL    0x1
#define MAD_XFER     0x2
#define INIT_TPB     0x4
#define BROADCAST    0x8
#define DP1          0x10
#define DP2          0x20
#define RP           0x40
#define GP           0x80
 
#endif /* INCLclCommandsh */



