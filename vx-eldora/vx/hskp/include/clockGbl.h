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
 
/* Define global time words for any task to use */ 
 
scope short year;
scope short month;
scope short day;
scope int julian_day;
scope short hour;
scope short minute;
scope short second;
scope short millisecond;

/* pointers to the clock registers */

scope short *timereq;            /* Time request register (latches time) */
scope short *time;               /* Starting address where time is stored */
scope short *clk_ack;            /* Acknowledge register */
scope short *clk_cmd;            /* Command register */
scope short *clk_intstat;        /* Interrupt status register */
scope short *clk_fifo;           /* clock command fifo */

/* array used to calculate month and day */
scope int day_mon_calc[13];



