/*
 *	$Id$
 *
 *	Module:	fill_nav_info	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.5  2003/10/01  19:31:05  kapoor
 * *** empty log message ***
 *
 * Revision 1.3  1994/05/20  20:37:04  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/25  20:43:04  craig
 * Initial revision
 *
 * description: This module fills the nav info block pointed to by
 *              the variable current_nav_pntr.  If the parameter sent
 *              to it is non-zero (true) this routine will also fill
 *              in the ascii at the start of the block.
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern
#include "hskpAll.h"

void fill_nav_info(int ascii)
{

/* Define some general purpose variables */
char hour,min,sec,day,yr,mon;
short msec,jday;

/* If ascii is true write the ascii out to the current nav area */

if(ascii)
  {
      current_nav_pntr->s_nav_info.nav_info_id[0] = 'N';
      current_nav_pntr->s_nav_info.nav_info_id[1] = 'A';
      current_nav_pntr->s_nav_info.nav_info_id[2] = 'V';
      current_nav_pntr->s_nav_info.nav_info_id[3] = 'D';
      current_nav_pntr->s_nav_info.nav_info_len = sizeof(struct nav_info);
  }

/* Get the current time from the time of day board */
get_time(&hour,&min,&sec,&msec,&jday,&mon,&day,&yr);

current_nav_pntr->s_nav_info.julian_day = jday;
current_nav_pntr->s_nav_info.hours = hour;
current_nav_pntr->s_nav_info.minutes = min;
current_nav_pntr->s_nav_info.seconds = sec;

return;
}

