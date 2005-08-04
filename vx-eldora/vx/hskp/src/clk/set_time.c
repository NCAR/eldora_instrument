 /*
 *	$Id$
 *
 *	Module:	set_time.c	 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  2003/10/01  20:53:12  kapoor
 * Initial revision
 *
 * Revision 1.2  2003/09/23  22:58:15  kapoor
 * changed include to "hskpAll.h"
 *
 * Revision 1.1  2003/09/23  16:43:29  kapoor
 * Initial revision
 *
 * Revision 2.0  2001/06/12 19:32:43  thor
 * *** empty log message ***
 *
 * Revision 1.2  2001/05/31 20:45:37  thor
 * Syncing source tree to ACE-ASIA
 *
 * Revision 1.3  1995/09/25  17:44:27  martin
 * updated from Craig's current code
 *
 * Revision 1.1  1995/07/11  21:04:56  craig
 * Initial revision
 *
 *
 * description: The purpose of this module is to encode the requested time into
 *              the proper format for the Bancomm clock board. Note: you
 *              must call start_clock with the encoded string before the
 *              clock hardware will actually get loaded with the time. 
 */


static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include "hskpAll.h"


void set_time(short nhour,short nminute,short nsecond,
	      short nmonth,short nday,short nyear)
{
    int ones,tens;

    *time_date++ = 'L';    /* The set the real time clock command */

    tens = nyear/10;
    ones = nyear - tens * 10;
    *time_date++ = tens + 0x30;
    *time_date++ = ones + 0x30;

    tens = nmonth/10;
    ones = nmonth - tens * 10;
    *time_date++ = tens + 0x30;
    *time_date++ = ones + 0x30;

    tens = nday/10;
    ones = nday - tens * 10;
    *time_date++ = tens + 0x30;
    *time_date++ = ones + 0x30;

    tens = nhour/10;
    ones = nhour - tens * 10;
    *time_date++ = tens + 0x30;
    *time_date++ = ones + 0x30;

    tens = nminute/10;
    ones = nminute - tens * 10;
    *time_date++ = tens + 0x30;
    *time_date++ = ones + 0x30;

    tens = nsecond/10;
    ones = nsecond - tens * 10;
    *time_date++ = tens + 0x30;
    *time_date++ = ones + 0x30;

    clkstart = 0;
    start_clock();

return;
}




