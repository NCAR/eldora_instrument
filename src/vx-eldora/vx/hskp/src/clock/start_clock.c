/*
 *	$Id$
 *
 *	Module:	strtclk.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1994/05/20  20:36:43  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  21:42:55  reif
 * Initial revision
 *
 *
 * description: The purpose of this module is to start the clock when called.
 */


static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern
#include "hskpAll.h"

void start_clock()
{

*tod_cmnd=0x1C; /* Normal mode, interrupts, 24hr mode, 32.768kHz crystal */

return;
}
