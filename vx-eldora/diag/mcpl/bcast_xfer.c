/*
 *	$Id$
 *
 * 	Module:	bcast_xfer	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description: This function broadcasts a block of data to all
 *              nodes that the user specifies
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

bcast_xfer()
{
short node_array[5], cmplt_array[5], status; 
int  int_node, pause, cmplt_stat_flg, timeout, proc, num_nodes;
long num_xfers, i, j, *start, add_array[5];

/*******************************************************************/
/* Setup the generic TPBs that will do the actual transfer of data */
/*******************************************************************/

/* The tpb_to_proc TPB sends the receive_bcast_tpb to a processor's dispatch
   table entry zero.  All of the general purpose parmeters in the TPB will be
   set now and processor specific ones will be set later */

tpb_to_proc->source_start_address = (long)receive_bcast  - STD_BASE;
                                               /* Data to be retrieved from
                                                  receive_bcast structure */
tpb_to_proc->sink_start_address = 0;           /* Going to an mcpl node so
                                                 no address is necessary */
tpb_to_proc->source_attribute = 0x0963; /* Normal VMEBus Addressing, address
                                          modifier $09, disable pipelining,
                                          disable BLT, 256 transfers per burst,
                                          32 bit transfers,VMEbus */
tpb_to_proc->sink_attribute = 0x0001;     /* Going to a MCPL Node */
tpb_to_proc->transfer_length = 7;         /* Eight Long words in a TPB */
tpb_to_proc->completion_status = 0x0006; /* Disable interrupt Mailbox A32/D16*/
tpb_to_proc->nack_count = 0;              /* Do not retry */
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

/* The bcast_tpb broadcasts the data to all MCPL nodes as determined by 
    their dispatch table entries zero.  All of the general purpose parmeters
   in this TPB will be set now and processor specific ones will be set later */

bcast_tpb->source_start_address = (long)(data_start - STD_BASE);
                                        /* Data will always be in same place */
bcast_tpb->sink_start_address = 0;      /* Going to an mcpl node so
                                           no address is necessary */
bcast_tpb->source_attribute = 0x0963; /* Normal VMEBus Addressing, address
                                           modifier $09, disable pipelining,
                                           disable BLT,256 transfers per burst,
                                           32 bit transfers,VMEbus */
bcast_tpb->sink_attribute = 0x0001;     /* Going to a MCPL Node */
bcast_tpb->transfer_length = 1000/4;  /* Send 1000 bytes of data each time*/
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
bcast_tpb->mailbox_address = (long)(mailbox - STD_BASE);

/* The receive_bcast tpb loads data off of the MCPL and into VMEBus memory
   this TPB is loaded in dispatch table entry zero, completion of transfer is
   signaled in mailbox and with interrupt if user desires it */

receive_bcast->source_start_address = 0; /* Data to be retrieved from mcpl */
receive_bcast->sink_start_address = 0x0000000; /* Generic Start Address */
receive_bcast->source_attribute = 0x0001;     /* Comming From an MCPL Node */
receive_bcast->sink_attribute = 0x0963;  /* Normal VMEBus Addressing, address
                                          modifier $09,disable pipelining,
                                          disable BLT, 256 transfers per burst,
                                          32 bit transfers,VMEbus */
receive_bcast->transfer_length = 1000/4; /* Send 1000 bytes each time */
receive_bcast->completion_status = 0x0000; /* Generic completion Status */
receive_bcast->nack_count = 0;           /* Do not retry */
receive_bcast->link_address = 0;       /* Linking is not performed by target */
receive_bcast->link_attribute = 0;     /* Linking is not performed by target */
receive_bcast->mcpl_control_word = 0;    /* MCPL to VMEBus transfer only */
receive_bcast->mailbox_address = 0; /* Generic, mail Box address will change */


/***************************************************/
/* Get the particulars from the user for this test */
/***************************************************/

printf("\nEnter the number of nodes to send data to: ");
scanf("%d",&num_nodes);
getchar();

printf("\nEnter the number of transfers to attempt: ");
scanf("%d",&num_xfers);
getchar();

for(i=0; i<num_nodes; i++)
  {
      printf("\nEnter the node number of processor %d: ",i);
      scanf("%d",&int_node);
      getchar();
      node_array[i] = int_node;

      printf("\nEnter address where to put mailbox in processor %d",i);
      printf("\n Remember to type in hexidecimal: ");
      scanf("%lx",&add_array[i]);
      getchar();

      printf("\nEnter the completion status type for processor %d",i);
      printf("\n0=mailbox only, 1=mailbox and interrupt: ");
      scanf("%d",&cmplt_stat_flg);
      getchar();

      if(cmplt_stat_flg == 0)
	cmplt_array[i] = MAILBOX_ONLY;
      else
	cmplt_array[i] = MAILBOX_INTERRUPT;

  }

printf("\nEnter the mode to operate in");
printf("\n0) no pause, full out");
printf("\n1) pause, once per transfer");
printf("\n2) hyper-pause, once per TPB: ");
scanf("%d",&pause);
getchar();


/* Now send as many data pieces over as the user wants */ 

for(i=0; i<num_xfers; i++)
  {

      /* Fill the data area with this transfer's number */
      for(j=0; j<250; j++)
	start = data_start;
	*start++ = i;


      for (proc=0; proc<num_nodes; proc++)
	{

	    /* Fill Generic holes in TPB's to send a tpb structure to the
	       proper processor dispatch table entry zero */

	    tpb_to_proc->mcpl_control_word = (tpb_to_proc->mcpl_control_word &
                                            0x07FF) | (node_array[proc]<<11);

	    /* Have to send all processors a tpb then only one
               broadcast of data */

	    tpb_to_proc->link_address = (long)tpb_to_proc - STD_BASE;
	    if((proc + 1) >= num_nodes)
	      tpb_to_proc->link_address = (long)bcast_tpb - STD_BASE;

	    /* Make remote nodes stick data at proper place */

	    receive_bcast->completion_status = cmplt_array[proc];

	    receive_bcast->sink_start_address = add_array[proc] + 4;

	    receive_bcast->mailbox_address = add_array[proc];

	    /* Send the receive_bcast tpb to the proper processor */

	    printf("Sending receive_bcast TPB to node #%d\n",node_array[proc]);

	    timeout = 0;
	    *mailbox = 0;
	    *control_reg = TRIGGER;
	    *start_reg = 0x0001;
	    do
	      {
		  timeout++;
	      } while ((*mailbox == 0) && (timeout < 3000));

	    status = *status_reg;
	    if(*mailbox != 0x5FFF)
	      {
		  printf("TPB transfer unsuccessful node: %d mailbox: %x
                        status: %x\n",node_array[proc],*mailbox,status);
		  return(1); 
	      }
	    else
	      printf("Successful TPB transfer to node: %d\n",node_array[proc]);

	    if(pause == 2)
	     {
	     printf("I am pausing now, please enter any number to continue\n");
	     scanf("%d",&i);
	     getchar();
	     }

 } /* for loop to get a receive_bcast tpb out to each processor */
 

/* Transfer data ray structure to all remote nodes */

      timeout = 0;
      *mailbox = 0;
      *control_reg = TRIGGER;
      *start_reg = 0x0001;
      do
	{
	  timeout++;
	} while (((unsigned short)(*mailbox) != 0xAFFF) && (timeout < 3000));

      status = *status_reg;
      if(timeout >= 3000)
	{
	printf("Unsuccessful broadcast xfer mailbox: %x status: %x\n",
                *mailbox,status);
        return(2);
        }
      else
	printf("Successful broadcast xfer number: %d",i);

      if(pause > 0)
	{
	    printf("I am pausing now, please enter any number to continue\n");
	    scanf("%d",&i);
	    getchar();
	}

  } /* Send some more data out */
} /* End of the broadcast_data function */
