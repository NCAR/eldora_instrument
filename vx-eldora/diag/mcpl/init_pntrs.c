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

/* Set addresses for the MCPL card */

status_reg =  (short *)(MCPL_BASE + STATUS_REG);
control_reg = (short *)(MCPL_BASE + CONTROL_REG);
short_fifo =  (short *)(MCPL_BASE + FIFO_REG);
long_fifo =   (long *)(MCPL_BASE + FIFO_REG);
start_reg =   (short *)(MCPL_BASE + START_REG);

init =  (struct TPB *)(VMEMEM_BASE + STD_BASE + INIT_OFFSET);
tpb_to_proc =   (struct TPB *)(VMEMEM_BASE + STD_BASE + TPB_TO_PROC_OFFSET);
receive_stand = (struct TPB *)(VMEMEM_BASE + STD_BASE + RECEIVE_STAND_OFFSET);
receive_bcast = (struct TPB *)(VMEMEM_BASE + STD_BASE + RECEIVE_BCAST_OFFSET);
stand_to_proc = (struct TPB *)(VMEMEM_BASE + STD_BASE + STAND_TO_PROC_OFFSET);
bcast_tpb =     (struct TPB *)(VMEMEM_BASE + STD_BASE + BCAST_TPB_OFFSET);
mailbox =   (short *)(VMEMEM_BASE + STD_BASE + MAILBOX_OFFSET);
data_start = (long *)(VMEMEM_BASE + STD_BASE + DATA_OFFSET);

printf("init address = %x \n",init);
printf("tpb_to_proc address = %x \n",tpb_to_proc);
printf("receive_stand address = %x \n",receive_stand);
printf("receive_bcast address = %x \n",receive_bcast);
printf("stand_to_proc address = %x \n",stand_to_proc);
printf("bcast_tpb address = %x \n",bcast_tpb);
printf("data address = %x \n",data_start);
}



































