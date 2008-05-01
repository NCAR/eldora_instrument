/*
 *	$Id$
 *
 *	Module:	stand_xfer	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1994/05/05  16:26:19  thor
 * Initial revision
 *
 *
 * description: This module transfers data over the Multi-Crate Pipeline (MCPL)
 *              by using the standard one host to one target transfer
 *              method.
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
extern float ran2(long *);

stand_xfer()
{

/* Define some general purpose variables */
    long i, j, taddress, num_xfers, *start;
    short status, node, cmplt_stat;
    int int_node, timeout, cmplt_stat_flg, pause, counter;
    float afloat;
    static long seed = -777;

/***************************************************************************/
/* Setup Generic parts of TPB's to send standard data block to a processor */
/***************************************************************************/

/* The tpb_to_proc TPB sends the receive_stand_tpb to a processor's dispatch
   table entry one.  All of the general purpose parmeters in the TPB will be
   set now and processor specific ones will be set later */

    tpb_to_proc->source_start_address = (long)receive_stand - std_base;
                                /* Data to be retrieved from
                                   receive_stand TPB structure */
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
    tpb_to_proc->link_address = (long)stand_to_proc - std_base;
                                /* link to stand_to_proc TPB structure */
    tpb_to_proc->link_attribute = 0x0963;/* Normal VMEBus Addressing,VME link after
                                            trigger, address modifier $09, disable
                                            BLT, 256 Xfers/burst, disable
                                            pipelining, link to VME */
    tpb_to_proc->mcpl_control_word = 0x0045;  /* Generic MCPL Node address,
                                                 dispatch table entry #1, remain
                                                 connected, dispatch table load,
                                                 burst mode, compelled */
    tpb_to_proc->mailbox_address = (long)mailbox - std_base;

/* The stand_to_proc TPB sends a data block to an MCPL node as determined by 
   its dispatch table entry one.  All of the general purpose parmeters in this
   TPB will be set now and processor specific ones will be set later */

    stand_to_proc->source_start_address = (long)data_start - std_base;
    /* Data to be retrieved from data_start */
    stand_to_proc->sink_start_address = 0;      /* Going to an mcpl node so
                                                   no address is necessary */
    stand_to_proc->source_attribute = 0x0963; /* Normal VMEBus Addressing, address
                                                 modifier $09, disable pipelining,
                                                 disable BLT,256 transfers per burst,
                                                 32 bit transfers,VMEbus */
    stand_to_proc->sink_attribute = 0x0001;     /* Going to a MCPL Node */
    stand_to_proc->transfer_length = (10000/4);
                                /* Send 10000 bytes on each xfer */
    stand_to_proc->completion_status = 0x0006;/* Disable interrupt
                                                 Mailbox A32/D16*/
    stand_to_proc->nack_count = 0;             /* Do not retry */
    stand_to_proc->link_address = (long)tpb_to_proc - std_base;
    /* link back to tpb_to_proc structure */ 
    stand_to_proc->link_attribute = 0x0963; /* Normal VMEBus Addressing,VME link
                                               after trigger, address modifier $09,
                                               disable BLT, 256 Xfers/burst,
                                               disable pipelining,link to VME */
    stand_to_proc->mcpl_control_word = 0x004B; /* Generic MCPL Node address,
                                                  dispatch table entry #1,DTX
                                                  Dispatch Table execute,locked,
                                                  noncompelled, remain connected */
    stand_to_proc->mailbox_address = (long)mailbox - std_base;

/* The receive_stand tpb loads data off of the MCPL and into VMEBus memory
   this TPB is loaded in dispatch table entry one, completion of transfer is
   not signaled */

    receive_stand->source_start_address = 0; /* Data to be retrieved from mcpl */
    receive_stand->sink_start_address = 0x0000000; /* Generic Start Address of the
                                                      Processor's Memory */
    receive_stand->source_attribute = 0x0001;     /* Comming From an MCPL Node */
    receive_stand->sink_attribute = 0x0963;  /* Normal VMEBus Addressing, address
                                                modifier $09,disable pipelining,
                                                disable BLT, 256 transfers per burst,
                                                32 bit transfers,VMEbus */
    receive_stand->transfer_length = 10000/4;
                                             /* Send 10000 bytes each time */
    receive_stand->completion_status = 0;    /* Generic completion Status */
    receive_stand->nack_count = 0;           /* Do not retry */
    receive_stand->link_address = 0;         /* Linking is not performed by target*/
    receive_stand->link_attribute = 0;       /* Linking is not performed by target*/
    receive_stand->mcpl_control_word = 0;    /* MCPL to VMEBus transfer only */
    receive_stand->mailbox_address = 0;      /* Generic mail Box Address */

/***************************************************/
/* Get the particulars from the user for this test */
/***************************************************/

    printf("Enter the node number to send data to: ");
    scanf("%d",&int_node);
    getchar();
    node = int_node;

    printf("\nEnter address where to put mailbox (data will be right after)\n");
    printf("on target node (Remember to type in hexidecimal): ");
    scanf("%lx",&taddress);
    getchar();

    printf("\nEnter the number of transfers to attempt: ");
    scanf("%d",&num_xfers);
    getchar();

    printf("\nEnter the completion status type:\n");
    printf("0=mailbox only,1=mailbox and interrupt: ");
    scanf("%d",&cmplt_stat_flg);
    getchar();

    printf("\nEnter the mode to operate in");
    printf("\n0) no pause, full out");
    printf("\n1) pause, once per transfer");
    printf("\n2) hyper-pause, once per TPB\n>  ");
    scanf("%d",&pause);
    getchar();

    if(cmplt_stat_flg == 0)
      cmplt_stat = MAILBOX_ONLY;
    else
      cmplt_stat = MAILBOX_INTERRUPT;

    printf("node selected = %d\n",node);
    printf("mail box address = %lx\n",taddress);
    printf("Number of transfers to attempt = %d\n",num_xfers);
    counter = 900;

/* Fill Generic holes in TPB's based on user inputs */

    tpb_to_proc->mcpl_control_word = (tpb_to_proc->mcpl_control_word & 0x07FF)
        | (node << 11);
    stand_to_proc->mcpl_control_word= (stand_to_proc->mcpl_control_word &
                                       0x07FF) | (node <<11);
    receive_stand->sink_start_address = taddress + 4;
    receive_stand->mailbox_address = taddress;
    receive_stand->completion_status = cmplt_stat;

    printf("\n triggering the mcpl node to transfer TPB to node: %d\n",node);

    timeout = 0;
    *mailbox = 0xAAAA;
    *control_reg = TRIGGER;
    *start_reg = 0x0001;
    do
      {
          taskDelay(1);
      } while (((unsigned short)*mailbox == 0xAAAA) && (++timeout < 100));

    status = *status_reg;

    if((unsigned short)*mailbox == 0x5FFF)
      printf("TPB transfer to node %d successful status is: %x mailbox is: %x\n",
             node,status,*mailbox);
    else if(timeout >= 100 && (unsigned short)*mailbox != 0x5FFF)
      {
          printf("TPB transfer unsuccessful to node: %d  status: %x\n",
                 node,status);
	  printf("Processor timed out before MCPL wrote mail box");
	  *panic = 1;
          return(1);
      }
    else if(timeout < 100 && (unsigned short)*mailbox != 0x5FFF)
      {
	  printf("Unsuccessful Xfer of TPB to node: %d status is: %x",
		 node,status);
	  printf(" mailbox: %x\n",*mailbox);
	  *panic = 1;
	  return(1); 
      }

    if(pause == 2)
      {
          printf("I am pausing now, please enter any number to continue\n");
          scanf("%d",&i);
          getchar();
      }

/* Now send as many data pieces over as the user wants */ 

    printf("Will repeat data transfer %d times.\n",num_xfers);
    
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

          /* Transfer standard data to the remote node */

	  if(pause > 0)
	    printf("\nTransferring Data Block # %d to node: %d\n",i,node);

          timeout = 0;
          *mailbox = 0xAAAA;
          *control_reg = TRIGGER;
          *start_reg = 0x0001;
          do
            {
                taskDelay(1);
            } while (((unsigned short)*mailbox == 0xAAAA) && (++timeout < 100));

          status = *status_reg;
          
	  if((unsigned short)*mailbox == 0x5FFF && pause > 0)
	    {
		printf("Successful Xfer of block # %d to node: %d status is: %x",
		       i,node,status);
		printf(" mailbox: %x\n",*mailbox);
	    }
          else if(timeout >= 100 && (unsigned short)*mailbox != 0x5FFF)
            {
                printf("Unsuccessful Xfer of block # %d to node: %d status is: %x",
		       i,node,status);
		printf("\nProcessor timed out before MCPL wrote mailbox\n");
		*panic = 1;
                return(2); 
            }
          else if(timeout < 100 && (unsigned short)*mailbox != 0x5FFF)
            {
                printf("Unsuccessful Xfer of block # %d to node: %d status is: %x",
		       i,node,status);
		printf(" mailbox: %x\n",*mailbox);
		*panic = 1;
                return(2); 
            }

          if(pause > 0)
            {
                int dummy;
                printf("I am pausing now, please enter any number to continue\n");
                scanf("%d",&dummy);
                getchar();
            }

      } /* Loop to send each data block */

    return(0);
}
