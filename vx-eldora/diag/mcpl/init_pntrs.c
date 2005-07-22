/*
 *	$Id$
 *
 *	Module:	init_pntrs.c	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1994/05/05  16:26:15  thor
 * Initial revision
 *
 *
 * description: This module initializes the pointers that handle
 *              the Multi-Crate Pipeline (MCPL).
 *
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "vxWorks.h"
#include "math.h"
#include "stdioLib.h"
#include "intLib.h"
#include "memLib.h"
#include "semLib.h"
#include "taskLib.h"
#include "tyLib.h"
#include "ioLib.h"
#include "in.h"
#include "systime.h"
#include "sysLib.h"

#include "mcplDef.h"
#include "mcplGbl.h"

init_pntrs()
{
int i;

/* Figure out which CPU we are running on then set correct bases */

vmemem_base = VMEMEM_BASE;
std_base = STD_BASE;
mcpl_base = MCPL_BASE;

/* i = whichCpu(); */
i=0;

if(i == 1)
  {
      mcpl_base = 0xFFFFE000;
      vmemem_base = 0x200000;
      std_base = 0;
  }
/* Set addresses for the MCPL card */

status_reg =  (short *)(mcpl_base + STATUS_REG);
control_reg = (short *)(mcpl_base + CONTROL_REG);
short_fifo =  (short *)(mcpl_base + FIFO_REG);
long_fifo =   (long *)(mcpl_base + FIFO_REG);
start_reg =   (short *)(mcpl_base + START_REG);

init =  (struct TPB *)(vmemem_base + std_base + INIT_OFFSET);
tpb_to_proc =   (struct TPB *)(vmemem_base + std_base + TPB_TO_PROC_OFFSET);
receive_stand = (struct TPB *)(vmemem_base + std_base + RECEIVE_STAND_OFFSET);
receive_bcast = (struct TPB *)(vmemem_base + std_base + RECEIVE_BCAST_OFFSET);
stand_to_proc = (struct TPB *)(vmemem_base + std_base + STAND_TO_PROC_OFFSET);
bcast_tpb =     (struct TPB *)(vmemem_base + std_base + BCAST_TPB_OFFSET);
mailbox =   (short *)(vmemem_base + std_base + MAILBOX_OFFSET);
panic =   (long *)(vmemem_base + std_base + PANIC_OFFSET);
data_start = (long *)(vmemem_base + std_base + DATA_OFFSET);


for(i=0; i<500; i++)
  *(data_start + i) = i;

printf("init address = %x \n",init);
printf("tpb_to_proc address = %x \n",tpb_to_proc);
printf("receive_stand address = %x \n",receive_stand);
printf("receive_bcast address = %x \n",receive_bcast);
printf("stand_to_proc address = %x \n",stand_to_proc);
printf("bcast_tpb address = %x \n",bcast_tpb);
printf("data address = %x \n",data_start);
printf("mailbox address = %x \n",mailbox);
printf("panic address = %x \n",panic);
}



































