/*
 *	$Id$
 *
 *	Module:	INIT_DATA_XFER
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/11/09  16:07:36  eric
 * Initial revision
 *
 *
 * description: This module initializes the Multi-Crate Pipeline (MCPL)
 *              for the ELDORA RP7 RADAR Processor.
 *
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define NO_RP7_SCOPE
#define NO_RDP_SCOPE
#define NO_SCOPE
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
#include "RadarGbls.h"
#include "RDPGlobals.h"
#include "mcpl_def.h"
#include "mcpl_gbl.h"

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

init_data_xfer()
{

/*******************************************************************/
/* Setup the generic TPBs that will do the actual transfer of data */
/*******************************************************************/

/* The tpb_to_proc TPB sends the receive_data_tpb to a processor's dispatch
   table entry zero.  All of the general purpose parmeters in the TPB will be
   set now and processor specific ones will be set later */

tpb_to_proc->source_start_address = (long)receive_data  - STD_BASE;
                                               /* Data to be retrieved from
                                                  receive_data structure */
tpb_to_proc->sink_start_address = 0;           /* Going to an mcpl node so
                                                 no address is necessary */
tpb_to_proc->source_attribute = 0x0963; /* Normal VMEBus Addressing, address
                                          modifier $09, disable pipelining,
                                          disable BLT, 256 transfers per burst,
                                          32 bit transfers,VMEbus */
tpb_to_proc->sink_attribute = 0x0001;     /* Going to a MCPL Node */
tpb_to_proc->transfer_length = 7;         /* Eight Long words in a TPB */
tpb_to_proc->completion_status = 0x0006;  /* Disable interrupt Mailbox A32/D16*/
tpb_to_proc->nack_count = 0x64;              /* Retry 100 times max */
tpb_to_proc->link_address = 0;            /* Will be set later */
tpb_to_proc->link_attribute = 0x0963;/* Normal VMEBus Addressing,VME link after
                                        trigger, address modifier $09, disable
                                        BLT, 256 Xfers/burst, disable
                                        pipelining, link to VME */
tpb_to_proc->mcpl_control_word = 0x0005;  /* Generic MCPL Node address,
                                            dispatch table entry #0, remain
                                            connected, dispatch table load,
                                            burst mode, compelled */
tpb_to_proc->mailbox_address = (long)mailbox - STD_BASE;

/* The bcast_tpb broadcasts the data to aall MCPL node as determined by 
    their dispatch table entries zero.  All of the general purpose parmeters
   in this TPB will be set now and processor specific ones will be set later */

bcast_tpb->source_start_address = 0; /* Will be start of each data ray */
bcast_tpb->sink_start_address = 0;      /* Going to an mcpl node so
                                           no address is necessary */
bcast_tpb->source_attribute = 0x0963; /* Normal VMEBus Addressing, address
                                           modifier $09, disable pipelining,
                                           disable BLT,256 transfers per burst,
                                           32 bit transfers,VMEbus */
bcast_tpb->sink_attribute = 0x0001;     /* Going to a MCPL Node */
bcast_tpb->completion_status = 0x0006; /* Disable interrupt Mailbox A32/D16*/
bcast_tpb->nack_count = 0;             /* Do not retry */
bcast_tpb->link_address = (long)tpb_to_proc - STD_BASE;
                            /* link back to tpb_to_proc structure */ 
bcast_tpb->link_attribute = 0x0963; /* Normal VMEBus Addressing,VME link
                                         after trigger, address modifier $09,
                                         disable BLT, 256 Xfers/burst,
                                         disable pipelining,link to VME */
bcast_tpb->mcpl_control_word = 0x003B; /* Broadcast MCPL Node address,
                                            dispatch table entry #0,BCD
                                            Broadcast data,locked,
                                            noncompelled, remain connected */
bcast_tpb->mailbox_address = 0;        /* will be current mailbox address */

/* The receive_data tpb loads data off of the MCPL and into VMEBus memory
   this TPB is loaded in dispatch table entry one, completion of transfer is
   signaled in mailbox*/

receive_data->source_start_address = 0; /* Data to be retrieved from mcpl */
receive_data->sink_start_address = 0x0000000; /* Generic Start Address of the
                                                data area in MAD Memory */
receive_data->source_attribute = 0x0001;     /* Comming From an MCPL Node */
receive_data->sink_attribute = 0x0963;  /* Normal VMEBus Addressing, address
                                          modifier $09,disable pipelining,
                                          disable BLT, 256 transfers per burst,
                                          32 bit transfers,VMEbus */
receive_data->transfer_length = (logical_length + 4)/4;
                                       /*  Long words in a data ray */
receive_data->completion_status = 0x0006;
                                       /* A32/D16 mailbox completion Status */
receive_data->nack_count = 0;           /* Do not retry */
receive_data->link_address = 0;        /* Linking is not performed by target */
receive_data->link_attribute = 0;      /* Linking is not performed by target */
receive_data->mcpl_control_word = 0;    /* MCPL to VMEBus transfer only */
receive_data->mailbox_address = 0;  /* Generic, mail Box address will change */
return(-1);
}



































