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
#include "sablIncs.h"


void set_time(int nhour,int nminute,int nsecond,
	      int nmonth,int nday,int nyear, char *string)
{
    int ones,tens;

    *string++ = 'L';    /* The set the real time clock command */

    tens = nyear/10;
    ones = nyear - tens * 10;
    *string++ = tens + 0x30;
    *string++ = ones + 0x30;

    tens = nmonth/10;
    ones = nmonth - tens * 10;
    *string++ = tens + 0x30;
    *string++ = ones + 0x30;

    tens = nday/10;
    ones = nday - tens * 10;
    *string++ = tens + 0x30;
    *string++ = ones + 0x30;

    tens = nhour/10;
    ones = nhour - tens * 10;
    *string++ = tens + 0x30;
    *string++ = ones + 0x30;

    tens = nminute/10;
    ones = nminute - tens * 10;
    *string++ = tens + 0x30;
    *string++ = ones + 0x30;

    tens = nsecond/10;
    ones = nsecond - tens * 10;
    *string++ = tens + 0x30;
    *string++ = ones + 0x30;

return;
}




