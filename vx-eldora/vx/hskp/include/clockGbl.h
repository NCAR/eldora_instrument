/*
 *	$Id$
 *
 *	Module:	clockGbl.h	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 *
 * Description: This include module contains all of the global 
 *              variables that are used with the Bancomm clock board.
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.4  2003/09/25  18:30:31  kapoor
 * Needed to add in old Gbl vars - trying to get housekeeper to run.  May
 * delete these after testing and discovering where old variables are being
 * used.
 *
 * Revision 1.3  2003/09/25  17:33:09  kapoor
 * changed time variable to time_reg to avoid declaration conflicts
 *
 * Revision 1.2  2003/09/24  20:15:54  kapoor
 * Added global var declarations, to satisfy convert_iru.c
 *
 * Revision 1.1  2003/09/24  20:14:30  kapoor
 * Initial revision
 *
 * Revision 2.0  2001/06/12 19:33:57  thor
 * *** empty log message ***
 *
 * Revision 1.2  2001/05/31 20:45:44  thor
 * Syncing source tree to ACE-ASIA
 *
 * Revision 1.4  1995/09/25  17:44:54  martin
 * updated from Craig's current code
 *
 * Revision 1.1  1995/08/14  14:49:48  craig
 * Initial revision
 *
 *
 *        
 */
 
/* Define global time words for any task to use  

************** USED in SABL, RENAMED BELOW FOR ELDORA *********** 

scope short year;
scope short month;
scope short day;
scope int julian_day;
scope short hour;
scope short minute;
scope short second;
scope short millisecond;

*/

/* pointers to the clock registers */

scope short *timereq;            /* Time request register (latches time) */
scope short *time_reg;           /* Starting address where time is stored */
scope short *clk_ack;            /* Acknowledge register */
scope short *clk_cmd;            /* Command register */
scope short *clk_intstat;        /* Interrupt status register */
scope short *clk_fifo;           /* clock command fifo */

/* array used to calculate month and day */
scope int day_mon_calc[13];

/* global time holding variables */

scope unsigned short msec;
scope unsigned short jday;
scope unsigned char sec;
scope unsigned char min;
scope unsigned char hr;
scope unsigned char day;
scope unsigned char mon;
scope unsigned char yr;

/******** THESE Addl Gbl vars need to be here to make housekeeper happy **/
/****************Testing*****************/


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
