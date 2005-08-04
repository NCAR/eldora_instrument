/*
 *	$Id$
 *
 *	Module: init_mini.c		 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:  Initializes the minirims handling pointers etc.
 *        
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern
#define VME_DPORT_BASE 0x82000000
#define MAILBOX 0x3FF100
#include "hskpAll.h"
#include "numinDef.h"

struct RIMS *rims_d;      /* Minirims structure */
int in_mini_isr;
void init_mini()
{

void mini_isr(void);

/* Set the pointers to the minirims data areas */

rims_d = (struct RIMS *)(VME_DPORT_BASE);

/* Clear the global GPS interrupt detected flag */

in_mini_isr = 0;

/* Connect the interrupt handler to the first mailbox interrupt */

connectMailbox((VOIDFUNCPTR)mini_isr,1,0);

/* Initialize mailbox location to 0 */

rims_d->hndshk.mailbox = (unsigned int *)(DRAM_BASE + MAILBOX);
*(rims_d->hndshk.mailbox) = 0;

return;
}
