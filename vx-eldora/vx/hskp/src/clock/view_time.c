/*
 *	$Id$
 *
 *	Module:	view.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/14  21:42:49  reif
 * Initial revision
 *
 *
 * description: The purpose of this module is to test the various functions
 *              of the TIME OF DAY board (TOD).  These include setting the
 *              Real Time Clock (RTC), reading time of power failure, writing
 *              and reading the various register on the RTC, setting up the 
 *              TOD for interrupts using the Bus Interrupter Module (BIM), the 
 *              RTC and the Power Monitor (PM) chip.  The software also
 *              allows the TOD to be set to IRIGB and reading time from the
 *              RTC.
 */


static char rcsid[] = "$Date$ $RCSfile$ $Revision$";


#define OK_RPC
#define scope extern
#include "hskpAll.h"


/*************** FUNCTION view_time() *********************/

void view_time() 
{
char hsecond;
short milsec;

      hsecond=*hsec_reg; /* Latch clock data first */
      while(!*int_msk&0x08)
	{
	    taskDelay(20);
	}
      milsec=(short)((float)*msec_reg/MS_DIVISOR);
      printf("TIME %02d:%02d:%02d:%02d:%03d  DATE %02d/%02d/%02d\n",
	     *hr_reg,*min_reg,*sec_reg,hsecond,milsec,*mon_reg,
	     *day_reg,*yr_reg);

}
