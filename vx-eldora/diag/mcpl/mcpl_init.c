/*
 *	$Id$
 *
 *	Module:	mcpl_init	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1994/05/05  16:26:18  thor
 * Initial revision
 *
 *
 * description: This module sends an initialization TPB to the
 *              Multi-Crate Pipeline (MCPL) node in the processor.
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

mcpl_init()
{
    short i, status, timeout;

/* Set up the initial TPB */

    init->source_start_address = 0;
    init->sink_start_address = 0;
    init->source_attribute = 0;
    init->sink_attribute = 0;
    init->transfer_length = 0;
    init->completion_status = 0x0006;  /* Disable interrupt Mailbox A32/D16 */
    init->nack_count = 0;
    init->link_address = (long)tpb_to_proc - std_base;
    init->link_attribute = 0x0933;     /* Normal VMEBus Addressing, VME link after
                                          trigger, address modifier $09, disable
                                          BLT, 32 Xfers/burst, disable pipelineing,
                                          link to VME */
    init->mcpl_control_word = 0;
    init->mailbox_address = (long)mailbox - std_base;

    printf("Am going to send an initialization TPB to local MCPL node\n");
    printf("mailbox address is: %lx\n",mailbox);

/* Check status register and print its contents */

    status = *status_reg;
    printf("Initial card status is: %x\n",status);

/* Send the init TPB to the MCPL */

    timeout = 0;
    *mailbox = 0;

    *control_reg = READ_INIT_TPB;
    long_pntr = (long *)init;

    for(i=0; i<8; i++)
      *long_fifo = *(long_pntr + i);

    *start_reg = 0x0001;

    do
      {
          taskDelay(1);
      } while ((*mailbox == 0) && (++timeout < 100));

    status = *status_reg;
    if(*mailbox == 0x1fff)
      printf("Init TPB transfer successful status is: %x mailbox:%x\n",
             status,*mailbox);
    else
      printf("Init TPB transfer timed out, unsuccessful mailbox: %x status: %x\n",
             *mailbox,status);
}



































