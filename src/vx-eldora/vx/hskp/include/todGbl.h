/*
 *	$Id$
 *
 *	Module:	todGbl.h	 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1996/02/21  17:21:25  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/09/01  16:51:08  reif
 * Initial revision
 *
 * Revision 1.1  1992/08/21 21:39:48  craig
 * Initial revision
 *
 *
 * description: These are the global variables associated with the time-
 *              of day (TOD) board.
 */

/***************** Array for calculating julian day *************************/
/** Contains the julian day for the start of each month, not leap year  *****/ 

#ifdef define_globals

short jday_calc[12] = {1,32,60,91,121,152,182,213,244,274,305,335};

#else

extern short jday_calc[12];

#endif

/* All of the pointers to the various clock locations on the TOD board */

scope unsigned char *hsec_reg;
scope unsigned char *sec_reg;
scope unsigned char *min_reg;
scope unsigned char *hr_reg;
scope unsigned char *day_reg;
scope unsigned char *mon_reg;
scope unsigned char *yr_reg;
scope unsigned char *tod_cmnd;
scope unsigned char *int_msk;

/* Pointers to the IRIG-B reading registers on the TOD Board */

scope unsigned char *irigb;
scope unsigned char *strg;
scope unsigned char *isec;
scope unsigned char *imin;
scope unsigned char *ihr;
scope unsigned char *id1;
scope unsigned char *id2;

/* Pointers to the sub-second interrupt handling stuff */

scope short *sel_525;
scope short *reset_525;
scope unsigned short *msec_reg;

/* Pointer to the BIM registers */

scope unsigned char *tod_bim_cr0;
scope unsigned char *tod_bim_cr1;
scope unsigned char *tod_bim_cr2;
scope unsigned char *tod_bim_cr3;
scope unsigned char *tod_bim_vr0;
scope unsigned char *tod_bim_vr1;
scope unsigned char *tod_bim_vr2;
scope unsigned char *tod_bim_vr3;

/* global time holding variables */

scope unsigned short msec;
scope unsigned short jday;
scope unsigned char sec;
scope unsigned char min;
scope unsigned char hr;
scope unsigned char day;
scope unsigned char mon;
scope unsigned char yr;
