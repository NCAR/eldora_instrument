/*
 *	$Id$
 *
 *	Module: initiator.c 		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	date  $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:  This program is a test initiator for the Multi Crate
 *               Pipeline (MCPL)
 *
 */

#define scope
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include </home/craig/mcpl/include/mcpl_def.h>
#include <vxWorks.h>
#include <stdioLib.h>
#include <math.h>

initiator()
{
struct TPB  init, source1, source2, target1, target2, data1, data2;
long *init_pntr, *target2_pntr, *target1_pntr, *data2_pntr, *data1_pntr;
long *source1_pntr, *source2_pntr;
short *status_reg, *control_reg, *short_fifo, *start_reg;
long *long_fifo, *long_pntr, i, j, timeout,buffer1[2001],buffer2[2001];
short status, control, start, mailbox;
int kill;

mailbox = 0;
kill = 0;

status_reg =  (short *)(MCPL_BASE + STATUS_REG);
control_reg = (short *)(MCPL_BASE + CONTROL_REG);
short_fifo =  (short *)(MCPL_BASE + FIFO_REG);
long_fifo =   (long *)(MCPL_BASE + FIFO_REG);
start_reg =   (short *)(MCPL_BASE + START_REG);

init_pntr =    (long *)&init.source_start_address;
target2_pntr = (long *)&target2.source_start_address;
target1_pntr = (long *)&target1.source_start_address;
data2_pntr =   (long *)&data2.source_start_address;
data1_pntr =   (long *)&data1.source_start_address;
source2_pntr =   (long *)&source2.source_start_address;
source1_pntr =   (long *)&source1.source_start_address;

/* Set up the initial TPB */

init.source_start_address = 0;
init.sink_start_address = 0;
init.source_attribute = 0;
init.sink_attribute = 0;
init.transfer_length = 0;
init.completion_status = 0x0002;  /* Disable interrupt Mailbox A24/D16 */
init.nack_count = 0;
init.link_address = (long)source1_pntr;
init.link_attribute = 0x3933;     /* Normal VMEBus Addressing, VME link after
                                     trigger, address modifier $39, disable
                                     BLT, 32 Xfers/burst, disable pipelineing,
                                     link to VME */
init.mcpl_control_word = 0;
init.mailbox_address = (long)&mailbox;

printf("mailbox address is: %lx\n",&mailbox);

status = *status_reg;
printf("Initial card status is: %x\n",status);
if((status & 0x0001) != 0)
  {
    *control_reg = HOLD;
    *start_reg = 0x0001;

    timeout = 0;
    do
      {
      status = *status_reg;
      timeout++;
      } while (((status & 0x0001) != 0) && (timeout < 300));
  }

else
  {
    printf("Status register busy bit not set, can not send init TPB\n");
    exit(0);
  }

*control_reg = READ_INIT_TPB;

printf("Sending Initialization TPB \n");
long_pntr = (long *)&init.source_start_address;
for(i=0; i<8; i++)
  {
    *long_fifo = *(long_pntr + i);
    timeout = *(long_pntr + i);
  }

*start_reg = 0x0001;
timeout = 0;
mailbox = 0;
do
   {
   timeout++;
   } while ((mailbox == 0) && (timeout < 3000));

status = *status_reg;
if(mailbox == 0x1fff)
  printf("Init TPB transfer successful status is: %x\n",status);
else
  {
  printf("Init TPB transfer unsuccessful mailbox: %x status: %x\n",
          mailbox,status);
  exit(1);
  }

 
/* Set up the source1 TPB to load the target1 TPB into mcpl node 3 (target) */

source1.source_start_address = target1_pntr;  /* Data to be retrieved from
                                                 Target 1 structure */
source1.sink_start_address = 0;      /* Going to mcpl node 3 no address
                                        necessary */
source1.source_attribute = 0x3963;   /* Normal VMEBus Addressing, address
                                        modifier $39,disable pipelining,
                                        disable BLT, 256 transfers per burst,
                                        32 bit transfers,VMEbus */
source1.sink_attribute = 0x0001;     /* Going to a MCPL Node */
source1.transfer_length = 7;         /* Eight Long words in a TPB */
source1.completion_status = 0x0002;  /* Disable interrupt Mailbox A24/D16 */
source1.nack_count = 0;              /* Do not retry */
source1.link_address = source2_pntr;
source1.link_attribute = 0x3933;    /* Normal VMEBus Addressing, VME link after
                                     trigger, address modifier $39, disable
                                     BLT, 32 Xfers/burst, disable pipelining,
                                     link to VME */
source1.mcpl_control_word = 0x1845;  /* MCPL Node address 3, dispatch table
                                        entry #1, remain connected, dispatch
                                        table load, burst mode */
source1.mailbox_address = &mailbox;

 
/* Set up the target1 TPB to store data @ 0x200000 */

target1.source_start_address = 0;  /* Data to be retrieved from mcpl itself */
target1.sink_start_address = 0x2000000; /*Start Address of the VME/VSB Memory*/
target1.source_attribute = 0x0001;     /* Comming From an MCPL Node */
target1.sink_attribute = 0x0963;       /* Normal VMEBus Addressing, address
                                          modifier $09,disable pipelining,
                                          disable BLT, 256 transfers per burst,
                                          32 bit transfers,VMEbus */
target1.transfer_length = 1999;      /*  2000 Long words in buffer */
target1.completion_status = 0;       /* Disable No completion Status */
target1.nack_count = 0;              /* Do not retry */
target1.link_address = 0;            /* Linking is not performed by target */
target1.link_attribute = 0;          /* Linking is not performed by target */
target1.mcpl_control_word = 0;       /* MCPL to VMEBus transfer only */
target1.mailbox_address = 0;         /* No mail Box Being used */


/* Transfer TPB to remote node */

printf("triggering the mcpl node to transfer TPB to remote node \n");
*control_reg = TRIGGER;
*start_reg = 0x0001;
timeout = 0;
mailbox = 0;
do
   {
   timeout++;
   } while ((mailbox == 0) && (timeout < 3000));

status = *status_reg;
if(mailbox == 0x5FFF)
  printf("TPB transfer successful status is: %x mailbox: %x\n",
          status,mailbox);
else
  {
  printf("TPB transfer unsuccessful mailbox: %x status: %x\n",
          mailbox,status);

  exit(2);
  }


/* Sit in loop and send data to the remote node */
do
  {

/* Set up the TPB that will actually do the VMEBus to MCPL transfer */

    source2.source_start_address = &buffer1; /* Data to be retrieved
                                                from buffer1*/
    source2.sink_start_address = 0;      /* Going to mcpl node 3 no address
                                            necessary */
    source2.source_attribute = 0x3963;   /* Normal VMEBus Addressing, address
                                            modifier $39,disable pipelining,
                                            disable BLT, 256 transfers per
                                            burst, 32 bit transfers,VMEbus */
    source2.sink_attribute = 0x0001;     /* Going to a MCPL Node */
    source2.transfer_length = 1999;      /* 2000 Long words to transfer */
    source2.completion_status = 0x0002;  /* Disable interrupt Mailbox A24/D16*/
    source2.nack_count = 0;              /* Do not retry */
    source2.link_address = source1_pntr;
    source2.link_attribute = 0x3963;   /* Normal VMEBus Addressing, VME link
                                          after trigger, address modifier $39,
                                          disable BLT, 256 Xfers/burst,
                                          disable pipelining, link to VME */
    source2.mcpl_control_word = 0x184F;  /* MCPL Node address 3, dispatch table
                                            entry #1, remain connected,
                                            dispatch table execute, burst mode
                                            noncompelled */
    source2.mailbox_address = &mailbox;

/* Get start value for putting something known into the buffer */
    printf("Enter start value for data #1");
    scanf("%ld",&j);
    for(i=0; i<2000; i++)
      buffer1[i] = i+j;

/* Transfer Data to remote node */

    printf("\nTriggering the mcpl node to receive data #1\n");
    *control_reg = TRIGGER;
    *start_reg = 0x0001;
    timeout = 0;
    mailbox = 0;
    do
      {
	timeout++;
      } while ((mailbox == 0) && (timeout < 3000));

    status = *status_reg;
    if(timeout < 3000)
      printf("Data transfer (buffer1) successful status is: %x mailbox: %x\n",
              status,mailbox);
    else
      printf("Data (buffer1) transfer unsuccessful mailbox: %x status: %x\n",
              mailbox,status);

/* Set up the second TPB that will actually do the VMEBus to MCPL transfer */

    source1.source_start_address = &buffer2; /* Data to be retrieved
                                                from buffer2*/
    source1.sink_start_address = 0;      /* Going to mcpl node 3 no address
                                            necessary */
    source1.source_attribute = 0x3963;   /* Normal VMEBus Addressing, address
                                            modifier $39,disable pipelining,
                                            disable BLT, 256 transfers per
                                            burst, 32 bit transfers,VMEbus */
    source1.sink_attribute = 0x0001;     /* Going to a MCPL Node */
    source1.transfer_length = 1999;      /* 2000 Long words to transfer */
    source1.completion_status = 0x0002;  /* Disable interrupt Mailbox A24/D16*/
    source1.nack_count = 0;              /* Do not retry */
    source1.link_address = source2_pntr;
    source1.link_attribute = 0x3963;   /* Normal VMEBus Addressing, VME link
                                          after trigger, address modifier $39,
                                          disable BLT, 256 Xfers/burst,
                                          disable pipelining, link to VME */
    source1.mcpl_control_word = 0x184F;  /* MCPL Node address 3, dispatch table
                                            entry #1, remain connected,
                                            dispatch table execute, burst mode
                                            noncompelled */
    source1.mailbox_address = &mailbox;

/* Get start value for putting something known into the buffer */
    printf("Enter start value for data #2 ");
    scanf("%d",&j);
    for(i=0; i<2000; i++)
      buffer2[i] = i+j;


/* Transfer Data to remote node */

    printf("\nTriggering the mcpl node to receive data #2\n");
    *control_reg = TRIGGER;
    *start_reg = 0x0001;
    timeout = 0;
    mailbox = 0;
    do
      {
	timeout++;
      } while ((mailbox == 0) && (timeout < 3000));

    status = *status_reg;
    if(timeout < 3000)
      printf("Data transfer (buffer2) successful status is: %x mailbox: %x\n",
	      status,mailbox);
    else
      printf("Data (buffer2) transfer unsuccessful mailbox: %x status: %x\n",
              mailbox,status);
    printf("Continue Sending More data Blocks? 1=no 0=yes ");
    scanf("%d",&kill);

  }while(kill == 0);
}

