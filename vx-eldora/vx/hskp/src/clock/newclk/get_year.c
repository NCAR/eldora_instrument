/*
 *	$Id$
 *
 *	Module:	get_year.c	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  2003/09/23  16:42:36  kapoor
 * Initial revision
 *
 * Revision 2.0  2001/06/12 19:32:42  thor
 * *** empty log message ***
 *
 * Revision 1.2  2001/05/31 20:45:36  thor
 * Syncing source tree to ACE-ASIA
 *
 * Revision 1.3  1995/09/25  17:44:22  martin
 * updated from Craig's current code
 *
 * Revision 1.1  1995/07/11  21:04:58  craig
 * Initial revision
 *
 *
 * description: The purpose of this module is to read the year from the 
 *              the Bancomm clock board.
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include "hskpAll.h"

void get_year()
{
    short tens, ones, dumb;

    *clk_ack = 0x04;           /* clear bit 2 of the ACK register */
    send_packet("O4");         /* send request year packet */
    while(!(*clk_ack & 0x04)); /* wait to receive packet */

    dumb = *clk_fifo;          /*read back the packet */
    dumb = *clk_fifo;
    dumb = *clk_fifo;
    tens = *clk_fifo & 0x00FF;
    ones = *clk_fifo & 0x00FF;
    dumb = *clk_fifo;

    year = (tens - 0x30) * 10 + (ones - 0x30);

}
