/*
 *	$Id$
 *
 *	Module:	gettime.c	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  2003/09/23  22:58:15  kapoor
 * changed include to "hskpAll.h"
 *
 * Revision 1.1  2003/09/23  16:41:14  kapoor
 * Initial revision
 *
 * Revision 2.0  2001/06/12 19:32:41  thor
 * *** empty log message ***
 *
 * Revision 1.2  2001/05/31 20:45:35  thor
 * Syncing source tree to ACE-ASIA
 *
 * Revision 1.4  1995/09/25  17:44:20  martin
 * updated from Craig's current code
 *
 * Revision 1.2  1995/08/14  14:43:52  craig
 * *** empty log message ***
 *
 * Revision 1.1  1995/07/11  21:04:52  craig
 * Initial revision
 *
 *
 * description: The purpose of this module is read the time from the
 *              Bancomm clock board.
 */


static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include "hskpAll.h"

void get_time(void)
{
    int i, dummy;
    short a[5];

/* latch current time by reading the time request register */
    a[0] = *timereq;


/* read in the time */
    time = (short *)(SHORT_BASE + BASE_CLOCK + TIME);
    for(i=0; i<5; i++) a[i] = *time++;

/* place into binary global variables */
    julian_day = 100 * (a[0] & 0xF);
    julian_day += ((a[1] & 0xF000) >> 12) * 10;
    julian_day += (a[1] & 0x0F00) >> 8;

    hour = ((a[1] & 0x00F0) >> 4) * 10;
    hour += a[1] & 0x000f;

    minute = ((a[2] & 0xF000) >> 12) * 10;
    minute += (a[2] & 0x0F00) >> 8;

    second = ((a[2] & 0x00F0) >> 4) * 10;
    second += a[2] & 0x000f;

    millisecond = ((a[3] & 0xF000) >> 12) * 100; 
    millisecond += ((a[3] & 0x0F00) >> 8) * 10; 
    millisecond += ((a[3] & 0x00F0) >> 4); 

/* Calculate day and month here here */

    dummy = 0;
    for(i=0; i<13; i++)
      {
	  if(julian_day <= dummy)
	    break;
	  else
	    dummy += day_mon_calc[i];
      }

    month = i;
    day = julian_day - dummy + day_mon_calc[i-1];

}
