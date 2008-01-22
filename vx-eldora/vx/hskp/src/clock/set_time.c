 /*
 *	$Id$
 *
 *	Module:	settime.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.4  2005/08/04 22:27:38  granger
 * commit vx/hskp as copied from /net/eldora/eldora, except a few obsolete
 * (afaik) directories were removed, like hskp/src/clock/newclk and
 * hskp/src/arinc_NCAR
 *
 * Revision 1.3  1994/05/20  20:36:42  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  21:42:54  reif
 * Initial revision
 *
 *
 * description: The purpose of this module is to set the time in the clock
 *              chip on the time-of-day (tod) board.
 */


static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern
#include "hskpAll.h"


void set_time(short hour, short minute, short second, short month, short day0,
              short year)
{
/***** Stop clock. Leave in 24 hr mode **********/

*tod_cmnd=0x04; 

/*********** LOAD CLOCK REGISTERS ***************/

*msec_reg = 0x0000; /* Init milliseconds to zero */

*hsec_reg = 0x00;
*sec_reg = second;
*min_reg = minute;
*hr_reg = hour;
*mon_reg = month;
*day_reg = day0;
*yr_reg = year;

return;
}




