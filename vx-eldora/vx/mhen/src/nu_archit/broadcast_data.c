/*
 *	$Id$
 *
 * 	Module:	broadcast_data	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1994/07/14  20:33:14  eric
 * Initial revision
 *
 * Revision 1.1  1992/11/09  16:07:38  eric
 * Initial revision
 *
 *
 * description: This function broadcasts a block of data to all
 *              processors that are alive on the data duplication 
 *              path current_ray_add and current_mailbox_add should be
 *              the actual address (not pointers to) of the data to be
 *              sent and the mailbox to return acknowledgment in
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define NO_SCOPE
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
#include "Header.h"
#include "Ray.h"
#include "Platform.h"
#include "FieldParam.h"
#include "RadarGbls.h"
#include "RDPGlobals.h"
#include "LclCommands.h"
#include "mcpl_def.h"
#include "mcpl_gbl.h"

extern int err_cnt;
long err_stat, max_time = 0;

void broadcast_data(current_ray_add,current_mailbox_add,length)
long current_ray_add,current_mailbox_add,length;

{
volatile short proc, proc_count, status, *mailbox_test, mad; 
volatile int timeout;
proc_count = 0;
for (proc=0; proc<4; proc++)
  {
    if(number_of_mads[proc] > 0)   /* Does the processor exist? */
      {


/* Fill Generic holes in TPB's to send a tpb structure to the
   proper processor dispatch table entry zero */

   tpb_to_proc->mcpl_control_word = (tpb_to_proc->mcpl_control_word & 0x07FF)
                                   | (proc_nodes[proc]<<11);

/* Have to send all processors a tpb then only one broadcast of data */
   tpb_to_proc->link_address = (long)tpb_to_proc - STD_BASE;
   if(++proc_count >= number_procs)
     tpb_to_proc->link_address = (long)bcast_tpb - STD_BASE;

/* Make remote nodes stick data at proper place */

   receive_data->completion_status =
                     madinfo[proc][current_mad[proc]][COMPLETE_S];

   receive_data->sink_start_address = 
       madinfo[proc][current_mad[proc]][MAD_BASE] + STARTOF_MAD_DATA +
         madinfo[proc][current_mad[proc]][SIZE_AREAS] * 
           madinfo[proc][current_mad[proc]][NUM_C_AREA];
   receive_data->mailbox_address =
       madinfo[proc][current_mad[proc]][MAD_BASE] + 8 +
         2 * madinfo[proc][current_mad[proc]][NUM_C_AREA];


/* Send the receieve_data tpb to the proper processor */

      timeout = 0;
      *mailbox = 0;
      *control_reg = TRIGGER;
      *start_reg = 0x0001;
      do
	{
	  timeout++; 
	} while ((*mailbox == 0));

      status = *status_reg;
      if(*mailbox != 0x5FFF)
	{
	    printf("#3 TPB transfer unsuccessful proc: %d mailbox: %x status: %x\n",proc,*mailbox,status);
	    err_cnt++;
	    if(err_cnt == 1)
	      err_stat = *mailbox;
	    currStatus->mcpl |= INIT_TPB;
	    switch(proc)
	      {
		case 0:
		  currStatus->mcpl |= DP1;
		  break;
		case 1:
		  currStatus->mcpl |= DP2;
		  break;
		case 2:
		  currStatus->mcpl |= RP;
		  break;
		case 3:
		  currStatus->mcpl |= GP;
		  break;
	      }
	    currStatus->mailbox = *mailbox;
	    currStatus->status = status;
	    mcpl_recover();
	    return; 
	}

 } /* Test to see if processor exists */
 } /* for loop to get a receive_data tpb out to each processor */

for (proc=0; proc<4; proc++)
  {
      madinfo[proc][current_mad[proc]][NUM_C_AREA] += 2;

      if(madinfo[proc][current_mad[proc]][NUM_C_AREA] >=
	 madinfo[proc][current_mad[proc]][NUM_AREAS])
	{
	    madinfo[proc][current_mad[proc]][NUM_C_AREA] = radar_fore_aft;
	    if(++current_mad[proc] >= number_of_mads[proc])
	      current_mad[proc] = 0;
	}
  }

/* Transfer data ray structure to all remote nodes */

      bcast_tpb->transfer_length = (length + 4)/4;
      bcast_tpb->source_start_address = current_ray_add - STD_BASE;
      bcast_tpb->mailbox_address = current_mailbox_add - STD_BASE;

      timeout = 0;
      mailbox_test = (short *)current_mailbox_add;
      *mailbox_test = 0x5555;
      *control_reg = TRIGGER;
      *start_reg = 0x0001;
      do
	{
	  timeout++;
	} while (((unsigned short)(*mailbox_test) == 0x5555) && (timeout < 1000));
      if(max_time < timeout)
	max_time = timeout;
      status = *status_reg;
      if(timeout >= 1000)
	{
	    printf("Unsuccessful data xfer mailbox: %x status: %x\n",*mailbox_test,status);
	    err_cnt++;
	    if(err_cnt == 1)
	      err_stat = *mailbox_test;
	    currStatus->mcpl |= BROADCAST;
	    currStatus->mailbox = *mailbox;
	    currStatus->status = status;
	    mcpl_recover();
	    return;
        }
return;
} /* End of the broadcast_data function */
























