/*
 *	$Id$
 *
 *	Module:	mcpl_recover	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description: This module resets the Multi-Crate Pipeline (MCPL)
 *              node over the VME bus.
 *
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define NO_RP7_SCOPE
#define NO_RDP_SCOPE
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

#include "Parameter.h"
#include "RadarDesc.h"
#include "Header.h"
#include "CellSpacing.h"
#include "Ray.h"
#include "Platform.h"
#include "FieldParam.h"
#include "Waveform.h"
#include "mcpl_def.h"
#include "mcpl_gbl.h"
#include "RadarGbls.h"
#include "RDPGlobals.h"
#include "LclCommands.h"
#include "ELDRP7.h"
#include "coll_dpramdefs.h"
#include "colquadsubs.h"
#include "dspdefs.h"
#include "dspaddr.h"
#include "rp7.h"

extern CELLSPACING *cs;
extern RADARDESC *rdsc;
extern WAVEFORM *wvfm;
extern PARAMETER *prm;
mcpl_recover()
{

/* Define some general purpose variables */
volatile short status, timeout;
int i;


/* Stay in here until I recover */
do{

    /* Send a reset command to the mcpl */

    do{
	printf("r");
	*control_reg = RESET;
	*start_reg = 0x0001;

	status = *status_reg;

	timeout = 0;
	do
	  {
	      timeout++;
	  } while ((status & 0x0001) != 0 && (timeout < 1000));

    } while ((status & 0x0001) != 0);

    /* Card reset correctly, now initialize the reset MCPL board */

    /* Set up the initial TPB */

    printf("i");
    init->source_start_address = 0;
    init->sink_start_address = 0;
    init->source_attribute = 0;
    init->sink_attribute = 0;
    init->transfer_length = 0;
    init->completion_status = 0x0006;  /* Disable interrupt Mailbox A32/D16 */
    init->nack_count = 0;
    init->link_address = (long)tpb_to_proc - STD_BASE;
    init->link_attribute = 0x0933;     /* Normal VMEBus Addressing, VME link
				     after trigger, address modifier $09,
				     disable BLT, 32 Xfers/burst,
				     disable pipelineing,link to VME */
    init->mcpl_control_word = 0;
    init->mailbox_address = (long)mailbox - STD_BASE;


    /* Pre-read the status register */

    status = *status_reg;

    /* Send the init TPB to the MCPL */

    *control_reg = READ_INIT_TPB;
    long_pntr = (long *)init;

    for(i=0; i<8; i++)
      *long_fifo = *(long_pntr + i);

    *start_reg = 0x0001;

    *mailbox = 0;
    timeout = 0;
    do
      {
	  timeout++;
      } while ((*mailbox == 0) && (timeout < 9000));
} while (*mailbox != 0x1fff);

}





































