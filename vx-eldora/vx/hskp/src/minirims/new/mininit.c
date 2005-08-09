/*
 *	$Id$
 *
 *	Module: mininit.c		 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1994/05/20  20:37:16  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/19  19:07:18  reif
 * Initial revision
 *
 *
 * description:
 *        
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include "numinDef.h"

void mini_init()
{
unsigned short *reset;

reset=(unsigned short *)(MIN_BASE+COUNT_RESET); 
*reset=0xff;    /* Select all counters */
reset=(unsigned short *)(MIN_BASE+MIN_COMMAND);
*reset=0x200;   /* Program Timer */
*reset=0x0;     /* Start Timer */
return;
}
