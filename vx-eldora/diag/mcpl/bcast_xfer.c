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
 * Revision 1.1  1994/05/05  16:26:20  thor
 * Initial revision
 *
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
extern float ran2(long *);

bcast_xfer()
{
    short node_array[5], cmplt_array[5], status; 
    int  int_node, pause, cmplt_stat_flg, timeout, proc, num_nodes, dummy;
    long num_xfers, i, j, *start, add_array[5], counter;
    static long seed = -999;
    float afloat;

/*******************************************************************/
/* Setup the generic TPBs that will do the actual transfer of data */
/*******************************************************************/

/* The tpb_to_proc TPB sends the receive_bcast_tpb to a processor's dispatch
   table entry zero.  All of the general purpose parmeters in the TPB will be
   set now and processor specific ones will be set later */

    tpb_to_proc->source_start_address = (long)receive_bcast  - std_base;
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
    tpb_to_proc->mailbox_address = (long)mailbox - std_base;

/* The bcast_tpb broadcasts the data to all MCPL nodes as determined by 
   their dispatch table entry zero.  All of the general purpose parmeters
   in this TPB will be set now and processor specific ones will be set later */

    bcast_tpb->source_start_address = (long)(data_start) - std_base;
                                /* Data will always be in same place */
    bcast_tpb->sink_start_address = 0;      /* Going to an mcpl node so
                                               no address is necessary */
    bcast_tpb->source_attribute = 0x0963; /* Normal VMEBus Addressing, address
                                             modifier $09, disable pipelining,
                                             disable BLT,256 transfers per burst,
                                             32 bit transfers,VMEbus */
    bcast_tpb->sink_attribute = 0x0001;     /* Going to a MCPL Node */
    bcast_tpb->transfer_length = 10000/4;  /* Send 10000 bytes of data each time*/
    bcast_tpb->completion_status = 0x0006; /* Disable interrupt Mailbox A32/D16*/
    bcast_tpb->nack_count = 0;             /* Do not retry */
    bcast_tpb->link_address = (long)tpb_to_proc - std_base;
                                /* link back to tpb_to_proc structure */ 
    bcast_tpb->link_attribute = 0x0963; /* Normal VMEBus Addressing,VME link
                                           after trigger, address modifier $09,
                                           disable BLT, 256 Xfers/burst,
                                           disable pipelining,link to VME */
    bcast_tpb->mcpl_control_word = 0x003B; /* Broadcast MCPL Node address,
                                              dispatch table entry #0,BCD
                                              Broadcast data,locked,
                                              noncompelled, remain connected */
    bcast_tpb->mailbox_address = (long)(mailbox) - std_base;

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
    receive_bcast->transfer_length = 10000/4; /* Send 10000 bytes each time */
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
    if(num_nodes > 5) num_nodes = 5;

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
    printf("\n2) hyper-pause, once per TPB\n> ");
    scanf("%d",&pause);
    getchar();
    counter = 95;

/* Now send as many data pieces over as the user wants */ 

    for(i=0; i<num_xfers; i++)
      {
	  counter++;
	  start = data_start;
	  *start++ = i;
	  if(counter >= 100)
	    {
		printf("%d transfers completed without errors\n",i);
		counter = 0;
		/* Fill some of the data area with this transfer's number */
		for(j=0; j<2000; j++)
		  {
		      afloat = ran2(&seed);
		      *start++ = (afloat - 0.5) * 2147483647;
		  }
	    }


          for (proc=0; proc<num_nodes; proc++)
            {

                /* Fill Generic holes in TPB's to send a tpb structure to the
                   proper processor dispatch table entry zero */

                tpb_to_proc->mcpl_control_word = (tpb_to_proc->mcpl_control_word &
                                                  0x07FF) | (node_array[proc]<<11);

                /* Have to send all processors a tpb then only one
                   broadcast of data */

                tpb_to_proc->link_address = (long)tpb_to_proc - std_base;
                if((proc + 1) >= num_nodes)
                  tpb_to_proc->link_address = (long)bcast_tpb - std_base;

                /* Make remote nodes stick data at proper place */

                receive_bcast->completion_status = cmplt_array[proc];

                receive_bcast->sink_start_address = add_array[proc] + 4;

                receive_bcast->mailbox_address = add_array[proc];

                /* Send the receive_bcast tpb to the proper processor */

		if(pause > 0)
		  printf("Sending receive_bcast TPB to node #%d\n",node_array[proc]);

                timeout = 0;
                *mailbox = 0xAAAA;
                *control_reg = TRIGGER;
                *start_reg = 0x0001;

                do
                  {
                    /*  taskDelay(1); */
                  } while (((unsigned short)*mailbox == 0xAAAA) && (++timeout < 100));

                status = *status_reg;

		if((unsigned short)*mailbox == 0x5FFF && pause > 0)
		  {
		      printf("Successful TPB transfer to node: %d\n",node_array[proc]);
		      printf("status is: %x  mailbox is: %x\n",status,*mailbox);
		  }
                else if(timeout >= 100 && (unsigned short)*mailbox != 0x5FFF)
                  {
                      printf("TPB transfer unsuccessful to node: %d status is: %x\n",
			     node_array[proc],status);
		      printf("Processor timed out waiting for MCPL to write mailbox\n");
		      *panic = 1;
                      return(1); 
                  }
                else if(timeout < 100 && (unsigned short)*mailbox != 0x5FFF)
                  {
                      printf("TPB transfer unsuccessful to node: %d status is: %x",
			     node_array[proc],status);
		      printf(" mailbox is: %x\n",*mailbox);
		      *panic = 1;
                      return(1); 
                  }


                if(pause == 2)
                  {
                      printf("I am pausing now, please enter any number to continue\n");
                      scanf("%d",&dummy);
                      getchar();
                  }

            } /* for loop to get a receive_bcast tpb out to each processor */
 

/* Transfer data ray structure to all remote nodes */

          timeout = 0;
          *mailbox = 0xAAAA;
          *control_reg = TRIGGER;
          *start_reg = 0x0001;
          do
            {
              /*  taskDelay(1); */
            } while (((unsigned short)*mailbox == 0xAAAA) && (++timeout < 100));

          status = *status_reg;

		if((unsigned short)*mailbox == 0xAFFF && pause > 0)
		  {
		      printf("Successful broadcast data transfer\n");
		      printf("status is: %x  mailbox is: %x\n",status,*mailbox);
		  }
                else if(timeout >= 100 && (unsigned short)*mailbox != 0xAFFF)
                  {
                      printf("Broadcast data transfer unsuccessful status is: %x\n",
			     status);
		      printf("Processor timed out waiting for MCPL to write mailbox\n");
		      *panic = 1;
                      return(2); 
                  }
                else if(timeout < 100 && (unsigned short)*mailbox != 0xAFFF)
                  {
                      printf("Broadcast data transfer unsuccessful status is: %x",
			     status);
		      printf(" mailbox is: %x\n",*mailbox);
		      *panic = 1;
                      return(1); 
                  }

          if(pause > 0)
            {
                printf("I am pausing now, please enter any number to continue\n");
                scanf("%d",&dummy);
                getchar();
            }

      } /* Send some more data out */
} /* End of the broadcast_data function */
