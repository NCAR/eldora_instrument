/*
 *	$Id$
 *
 *	Module:	gettime.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/14  21:42:58  reif
 * Initial revision
 *
 *
 * description: The purpose of this module is read the time from the
 *              TIME OF DAY board (TOD).
 */


static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern
#include "hskpAll.h"

void get_time(char *hour,char *minute,char *second,short *millisec,short *jday,
              char *month,char *day0,char *year)
{
short i;
char hund, vday, vmon,dummy;

/* Read the time from the tod board */

hund = *hsec_reg; /* Latch clock by first accessing hseconds register */

*millisec = *msec_reg / MS_DIVISOR;
*hour = *hr_reg;
*minute = *min_reg;
*second = *sec_reg;
vmon = *mon_reg;
*month = vmon;
vday = *day_reg;
*day0 = vday;
*year = *yr_reg;
/* Calculate julian day here */
*jday = jday_calc[vmon-1] + vday - 1;
dummy=*int_msk; /* Resets the 1 sec int flag */
}
