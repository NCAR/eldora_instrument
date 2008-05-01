/*
 *	$Id$
 *
 *	Module: mini_isr.c		 
 *	Original Author: Eric Loew
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * Initial revision
 *
 *
 * description:  This routine handles an interrupt from the Minirims interface.
 *               Also, it will handle being called if the polled handshake
 *               method of using the Minirims interface is enabled.
 *        
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern
#include "hskpAll.h"
#include "numinDef.h"
#include "tp41.h"

extern struct RIMS *rims_d;      /* Minirims structure */
extern int in_mini_isr;
static int nav_buff = 0;
void mini_isr(void)
{
register long *data_pntr, i;
register long *nav_pntr;
int data_size, curr_buff;

/* Perform the necessary tasks to clear the interrupt */

*(rims_d->hndshk.mailbox) = 0x0;

/* Set the global flag */

in_mini_isr = 1;

if(nav_buff)
  current_nav_pntr = (struct nav_data *)(FIRST_NAV_OFFSET + STANDARD_BASE + FORE_STAND_START);
else
  current_nav_pntr = (struct nav_data *)(SEC_NAV_OFFSET + STANDARD_BASE + FORE_STAND_START);

if(rims_d->hndshk.data_flag[0] == 1)
  curr_buff = 0;
if(rims_d->hndshk.data_flag[1] == 1)
  curr_buff = 1;

/* Update Record with Nav info */

fill_nav_info(0);

/* Update the Nav Record with IRU data */

data_pntr = (long *)(&last_iru_data.ins_data_id[0]);
data_size = sizeof(ins_data)/4 + 1;
nav_pntr = (long *)(&current_nav_pntr->s_ins_data.ins_data_id[0]);
for(i=0; i<data_size; i++)
  *nav_pntr++ = *data_pntr++;

/* Update the Nav Record with GPS data */

data_pntr = (long *)(&last_gps_data.gps_data_id[0]);
data_size = sizeof(gps_data)/4 + 1;
nav_pntr = (long *)(&current_nav_pntr->s_gps_data.gps_data_id[0]);
for(i=0; i<data_size; i++)
  *nav_pntr++ = *data_pntr++;

/* Update the Nav Record with Minirims data */

data_pntr = (long *)(&rims_d->RIMS_data[curr_buff]);
data_size = sizeof(minirims_data)/4;
nav_pntr = (long *)(&current_nav_pntr->s_minirims_data.minirims_data_id[0]);
for(i=0; i<data_size; i++)
  *nav_pntr++ = *data_pntr++;

rims_d->hndshk.data_flag[curr_buff] = 0;

/* Handshake with mcpl to send Nav record */

fore_vmehndshk->nav_hndshk[nav_buff] = 1;

nav_buff = !nav_buff;   /* toggle nav buffer */
in_mini_isr = 0;
return;
}
