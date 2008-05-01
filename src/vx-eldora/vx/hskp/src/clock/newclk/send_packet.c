/*
 *	$Id$
 *
 *	Module:	send_packet.c	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  2003/09/23  16:43:15  kapoor
 * Initial revision
 *
 * Revision 2.0  2001/06/12 19:32:42  thor
 * *** empty log message ***
 *
 * Revision 1.2  2001/05/31 20:45:36  thor
 * Syncing source tree to ACE-ASIA
 *
 * Revision 1.3  1995/09/25  17:44:25  martin
 * updated from Craig's current code
 *
 * Revision 1.1  1995/07/11  21:04:57  craig
 * Initial revision
 *
 *
 * description: The purpose of this module is to send a control packet 
 *              to the Bancomm clock board.
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include "hskpAll.h"

#define SOH  0x01
#define ETB  0x17

void send_packet(char *charptr)
{
    *clk_fifo = SOH;
    while(*charptr) *clk_fifo = *charptr++;
    *clk_fifo = ETB;
    *clk_ack = 0x81;
    while(!(*clk_ack & 0x01));
}
