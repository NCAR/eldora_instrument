/*
 *	$Id$
 *
 *	Module:	start_clock.c	 
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
 * Revision 1.3  1995/09/25  17:44:28  martin
 * updated from Craig's current code
 *
 * Revision 1.1  1995/07/11  21:04:57  craig
 * Initial revision
 *
 *
 *
 * description: The purpose of this module is to start the clock when called.
 */


static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include "sablIncs.h"

void start_clock(char *time_date)
{

*clk_intstat = 0x08;                 /* Clear the 1 PPS bit */
while(!(*clk_intstat & 0x08));       /* wait for next second to roll around */
send_packet(time_date);

return;
}
