/*
 *	$Id$
 *
 *	Module: command_gps.c		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/19  17:23:56  craig
 * Initial revision
 *
 *
 * description:  Sends the desired command to the GPS interface.
 *        
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern
#include "hskpAll.h"

void command_gps(short cmd)
{
/* cmd = 1  Start looking for GPS data
   cmd = 2  Send initialization sequence to GPS receiver
   cmd = 3  Stop looking for GPS data */

/* Clear handshake area and send the command */
*gps_hndshk = (short)0;
*gps_command = cmd;

/* Set the gps status byte to indicate no errors */

currStatus->gps = 0;

return;
}
