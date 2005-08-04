/*
 *	$Id$
 *
 *	Module: command_mini.c		 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
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
#include "numinDef.h"

extern struct RIMS *rims_d;      /* Minirims structure */

void command_mini(short cmd)
{
/* 
   cmd = 1  Start looking for Minirims data (enable Mailbox interrupt)
   cmd = 3  Stop looking for Minirims data  (disable Mailbox interrupt)
*/

if(cmd == 1)
  rims_d->hndshk.intr_flag = 1;    /* enable Mailbox interrupts */

if(cmd == 3)
  rims_d->hndshk.intr_flag = 0;    /* disable Mailbox interrupts */

return;
}

