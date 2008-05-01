/*
 *	$Id$
 *
 *	Module canned_xfer.c 		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	date  $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:  This program copies what the radar processor must do to
 *               the Multi Crate Pipeline (MCPL) and it sends known
 *               dummy data over the link.
 *
 */

#define scope
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include </home/craig/mcpl/include/mcpl_def.h>
#include </ELDORA/vx/include/tape_formats.h>
#include </vx/h/vxWorks.h>
#include </vx/h/stdioLib.h>
#include </vx/h/math.h>

canned_xfer()
{
#define LOG_HEADER_LEN 104  /* length in Bytes of radar logical record header*/

struct TPB  init;       /* Initializing Transfer Parameter Block (TPB) */
struct TPB  tpb_to_proc; /* TPB to send TPB to display processor */
struct TPB  receive_gmad; /* TPB to receive a MAD structure TPB */
struct TPB  gmad_to_proc; /* TPB to send TPB to display processor */
struct TPB  tpb_to_dis; /* TPB to send TPB to display processor */
struct TPB  tpb_to_rec; /* TPB to send TPB to recording processor */
struct TPB  tpb_to_grid; /* TPB to send TPB to data gridding processor */
struct TPB  dis_tpb; /* TPB to send to display processor */
struct TPB  rec_tpb; /* TPB to send to recording processor */
struct TPB  grid_tpb; /* TPB to send to data gridding processor */
struct TPB  bcast_tpb; /* TPB to broadcast data to all processors */


/* Pointers to all defined TPB's */
long *init_pntr, *tpb_to_dis_pntr, *tpb_to_rec_pntr, *tpb_to_grid_pntr;
long *dis_tpb_pntr, *rec_tpb_pntr, *grid_tpb_pntr, *bcast_tpb_pntr;
long *tpb_to_proc_pntr,*gmad_to_proc_pntr,*receive_gmad_pntr;

/* Pointers to other MCPL registers, storage for register values */
short *status_reg, *control_reg, *short_fifo, *start_reg;
long *long_fifo, *long_pntr;
short status, control, start, mailbox;

/* Arrays for address storage to mailboxes and data */

int number_of_mads[3]; /* Number of mads in each processor 0=> no processor */
int current_mad[3];    /* Current mad (Either 0 or 1) in each processor */
short proc_nodes[3];   /* Array of node numbers for each processor */
#define DIS 0          /* All above arrays display processor is entry 0 */
#define REC 1          /* All above arrays recording processor is entry 1 */
#define GRID 2         /* All above arrays gridding processor is entry 2 */

long madinfo[3][2][7];
/* First dimension is processor number display=0, record=1, gridding=2
   Second dimension is for each mad in a processor: 2 maximum, 1 minimum
   Third dimension is for various defined parameters:
   [P] [M] [0] base address of MAD M for processor P
   [P] [M] [1] Number of data areas in MAD M for processor P
   [P] [M] [2] Size of data areas in MAD M for processor P (in Bytes)
   [P] [M] [3] Address of current data area in MAD M for processor P
   [P] [M] [4] Number of current data area in MAD M for processor P
   [P] [M] [6] Maximum Size of memory available for MAD M in processor P */

#define MAD_BASE     0
#define NUM_AREAS    1
#define SIZE_AREAS   2
#define ADD_C_AREA   3
#define NUM_C_AREA   4
#define MAX_MEM      6

/* Reserve one space for entire MAD structure to transfer */
struct MAD gmad;
long *gmad_pntr;

/* General purpose variables and data buffer */
long i, j, timeout;
struct file_header header;
struct logical_radar_data data;
struct radar_descriptor rad_des;
int kill, proc, mad, logical_length;

/* Initialize general purpose variables */

mailbox = 0;
kill = 0;

/* Initalize the arrays that index into the MAD descriptor arrays */
number_of_mads[DIS] = NUM_STRUCT_DIS;
number_of_mads[REC] = NUM_STRUCT_REC;
number_of_mads[GRID] = NUM_STRUCT_GRID;

proc_nodes[DIS] = DISPLAY_NODE;
proc_nodes[REC] = RECORD_NODE;
proc_nodes[GRID] = GRID_NODE;


madinfo[DIS][0][MAD_BASE] = MADBASE_DIS_0;
madinfo[DIS][1][MAD_BASE] = MADBASE_DIS_1;
madinfo[REC][0][MAD_BASE] = MADBASE_REC_0;
madinfo[REC][1][MAD_BASE] = MADBASE_REC_1;
madinfo[GRID][0][MAD_BASE] = MADBASE_GRID_0;
madinfo[GRID][1][MAD_BASE] = MADBASE_GRID_1;

madinfo[DIS][0][MAX_MEM] = DIS_MEM_SIZE_0;
madinfo[DIS][1][MAX_MEM] = DIS_MEM_SIZE_1;
madinfo[REC][0][MAX_MEM] = REC_MEM_SIZE_0;
madinfo[REC][1][MAX_MEM] = REC_MEM_SIZE_1;
madinfo[GRID][0][MAX_MEM] = GRID_MEM_SIZE_0;
madinfo[GRID][1][MAX_MEM] = GRID_MEM_SIZE_1;

/* Set addresses for the MCPL card */

status_reg =  (short *)(MCPL_BASE + STATUS_REG);
control_reg = (short *)(MCPL_BASE + CONTROL_REG);
short_fifo =  (short *)(MCPL_BASE + FIFO_REG);
long_fifo =   (long *)(MCPL_BASE + FIFO_REG);
start_reg =   (short *)(MCPL_BASE + START_REG);

init_pntr =    (long *)&init.source_start_address;
tpb_to_proc_pntr = (long *)&tpb_to_proc.source_start_address;
receive_gmad_pntr = (long *)&receive_gmad.source_start_address;
gmad_to_proc_pntr = (long *)&gmad_to_proc.source_start_address;
tpb_to_dis_pntr = (long *)&tpb_to_dis.source_start_address;
tpb_to_rec_pntr = (long *)&tpb_to_rec.source_start_address;
tpb_to_grid_pntr =   (long *)&tpb_to_grid.source_start_address;
dis_tpb_pntr =   (long *)&dis_tpb.source_start_address;
rec_tpb_pntr =   (long *)&rec_tpb.source_start_address;
grid_tpb_pntr =   (long *)&grid_tpb.source_start_address;
bcast_tpb_pntr =   (long *)&bcast_tpb.source_start_address;
gmad_pntr =   (long *)&gmad.num_before_repeat;

/* Set up the initial TPB */

init.source_start_address = 0;
init.sink_start_address = 0;
init.source_attribute = 0;
init.sink_attribute = 0;
init.transfer_length = 0;
init.completion_status = 0x0002;  /* Disable interrupt Mailbox A24/D16OB */
init.nack_count = 0;
init.link_address = (long)tpb_to_proc_pntr;
init.link_attribute = 0x3933;     /* Normal VMEBus Addressing, VME link after
                                     trigger, address modifier $39, disable
                                     BLT, 32 Xfers/burst, disable pipelineing,
                                     link to VME */
init.mcpl_control_word = 0;
init.mailbox_address = (long)&mailbox;

printf("mailbox address is: %lx\n",&mailbox);

/* Check status register to see if this is a start from power-up */

status = *status_reg;
printf("Initial card status is: %x\n",status);

/* If MCPL has not been initialized since power on status bit 0 will be set */

if((status & 0x0001) != 0)
  {
    printf("First MCPL access since power on\n");
    *control_reg = HOLD;
    *start_reg = 0x0001;

    timeout = 0;
    do
      {
      status = *status_reg;
      timeout++;
      } while (((status & 0x0001) != 0) && (timeout < 300));

    if(timeout >= 300)
      {
      printf("MCPL card still busy after issueing hold command, must exit\n");
      exit(0);
      }
  }

/* if this is not the first time the program has run, we need to send the hold
   command, but the card will respond by a "hold acknowledged" status in the
   mailbox */

else
  {
    printf("MCPL already running, must send hold command to stop\n");

    *control_reg = HOLD;
    *start_reg = 0x0001;

    /* check for acknowledge of hold command */

    timeout = 0;
    mailbox = 0;
    do
    {
    timeout++;
    } while ((mailbox == 0) && (timeout < 3000));

   status = *status_reg;
   if(mailbox != 0x2fff)
     {
     printf("Hold command not acknowledged by MCPL, must exit\n");
     exit(1);
     }
  }

/* Send the init TPB to the MCPL */

printf("Sending Initialization TPB \n");

*control_reg = READ_INIT_TPB;
long_pntr = (long *)&init.source_start_address;
for(i=0; i<8; i++)
    *long_fifo = *(long_pntr + i);

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
  exit(2);
  }

/***************************************************************************/
/* Setup Generic parts of TPB's to send a MAD structures to the processors */
/***************************************************************************/

/* The following three lines are for tests only!! remove for real thing */

rad_des.cell_total = 512;
header.param_per_cell = 3;
header.binary_format = 1;

/* The tpb_to_proc TPB sends the receive_gmad_tpb to a processor's dispatch
   table entry one.  All of the general purpose parmeters in the TPB will be
   set now and processor specific ones will be set later */

tpb_to_proc.source_start_address = (long)receive_gmad_pntr;
                                               /* Data to be retrieved from
                                                  receive_gmad structure */
tpb_to_proc.sink_start_address = 0;           /* Going to an mcpl node so
                                                 no address is necessary */
tpb_to_proc.source_attribute = 0x3963; /* Normal VMEBus Addressing, address
                                          modifier $39, disable pipelining,
                                          disable BLT, 256 transfers per burst,
                                          32 bit transfers,VMEbus */
tpb_to_proc.sink_attribute = 0x0001;     /* Going to a MCPL Node */
tpb_to_proc.transfer_length = 7;         /* Eight Long words in a TPB */
tpb_to_proc.completion_status = 0x0002;  /* Disable interrupt Mailbox A24/D16*/
tpb_to_proc.nack_count = 0;              /* Do not retry */
tpb_to_proc.link_address = (long)gmad_to_proc_pntr;
                                     /* link to gmad_to_proc TPB structure */
tpb_to_proc.link_attribute = 0x3963; /* Normal VMEBus Addressing,VME link after
                                        trigger, address modifier $39, disable
                                        BLT, 256 Xfers/burst, disable
                                        pipelining, link to VME */
tpb_to_proc.mcpl_control_word = 0x0045;  /* Generic MCPL Node address,
                                            dispatch table entry #1, remain
                                            connected, dispatch table load,
                                            burst mode, compelled */
tpb_to_proc.mailbox_address = (long)&mailbox;

/* The gmad_to_proc TPB sends the a MAD to an MCPL node as determined by 
    its dispatch table entry one.  All of the general purpose parmeters in this
    TPB will be set now and processor specific ones will be set later */

gmad_to_proc.source_start_address = (long)gmad_pntr;
                               /* Data to be retrieved from gmad structure */
gmad_to_proc.sink_start_address = 0;      /* Going to an mcpl node so
                                           no address is necessary */
gmad_to_proc.source_attribute = 0x3963; /* Normal VMEBus Addressing, address
                                           modifier $39, disable pipelining,
                                           disable BLT,256 transfers per burst,
                                           32 bit transfers,VMEbus */
gmad_to_proc.sink_attribute = 0x0001;     /* Going to a MCPL Node */
gmad_to_proc.transfer_length = (sizeof(struct MAD) + 4)/4;
                                         /* Size of MAD in Long words */
gmad_to_proc.completion_status = 0x0002; /* Disable interrupt Mailbox A24/D16*/
gmad_to_proc.nack_count = 0;             /* Do not retry */
gmad_to_proc.link_address = (long)tpb_to_proc_pntr;
                            /* link back to tpb_to_proc structure */ 
gmad_to_proc.link_attribute = 0x3963; /* Normal VMEBus Addressing,VME link
                                         after trigger, address modifier $39,
                                         disable BLT, 256 Xfers/burst,
                                         disable pipelining,link to VME */
gmad_to_proc.mcpl_control_word = 0x004B; /* Generic MCPL Node address,
                                            dispatch table entry #1,DTX
                                            Dispatch Table execute,locked,
                                            noncompelled, remain connected */
gmad_to_proc.mailbox_address = (long)&mailbox;

/* The receive_gmad tpb loads data off of the MCPL and into VMEBus memory
   this TPB is loaded in dispatch table entry one, completion of transfer is
   not signaled */

receive_gmad.source_start_address = 0; /* Data to be retrieved from mcpl */
receive_gmad.sink_start_address = 0x0000000; /* Generic Start Address of the
                                                MAD Memory */
receive_gmad.source_attribute = 0x0001;     /* Comming From an MCPL Node */
receive_gmad.sink_attribute = 0x0963;  /* Normal VMEBus Addressing, address
                                          modifier $09,disable pipelining,
                                          disable BLT, 256 transfers per burst,
                                          32 bit transfers,VMEbus */
receive_gmad.transfer_length = (sizeof(struct MAD) + 4)/4;
                                       /*  Long words in a MAD structure */
receive_gmad.completion_status = 0;    /*  No completion Status */
receive_gmad.nack_count = 0;           /* Do not retry */
receive_gmad.link_address = 0;         /* Linking is not performed by target */
receive_gmad.link_attribute = 0;       /* Linking is not performed by target */
receive_gmad.mcpl_control_word = 0;    /* MCPL to VMEBus transfer only */
receive_gmad.mailbox_address = 0;      /* No mail Box Being used */

/***********************************************************************/
/* Calculate the position and size of the data areas in all processors */
/***********************************************************************/

logical_length = LOG_HEADER_LEN + rad_des.cell_total * header.param_per_cell *
                 header.binary_format;

for (proc=0; proc<3; proc++)
  {
    for (mad=0; mad<number_of_mads[proc]; mad++)
      {
      i = floor((double)((logical_length + 0x400)/0x400));
      madinfo[proc][mad][SIZE_AREAS] = i * 0x400;
      if((madinfo[proc][mad][NUM_AREAS] = (madinfo[proc][mad][MAX_MEM]-0x1800)/
                              madinfo[proc][mad][SIZE_AREAS])>1000)
                              madinfo[proc][mad][NUM_AREAS] = 1000;

/* fill the general MAD structure with the proper information */

      gmad.num_before_repeat = madinfo[proc][mad][NUM_AREAS];
      for(i=0; i<madinfo[proc][mad][NUM_AREAS]; i++)
	{
         gmad.mailboxes[i] = 0;
	 gmad.addresses[i] = madinfo[proc][mad][MAD_BASE] + 0x1800 +
                             i * madinfo[proc][mad][SIZE_AREAS];
        }

/* Fill Generic holes in TPB's to send this MAD structure to the
   proper processor */
      tpb_to_proc.mcpl_control_word = (tpb_to_proc.mcpl_control_word & 0x07FF)
                                       | (proc_nodes[proc]<<11);
      gmad_to_proc.mcpl_control_word= (gmad_to_proc.mcpl_control_word & 0x07FF)
                                      | (proc_nodes[proc]<<11);
      receive_gmad.sink_start_address = madinfo[proc][mad][MAD_BASE];

/* Send the  receieve_gmad tpb to the proper processor */

      printf("triggering the mcpl node to transfer TPB to processor: %d\n"
              ,proc);

      timeout = 0;
      mailbox = 0;
      *control_reg = TRIGGER;
      *start_reg = 0x0001;
      do
	{
	  timeout++;
	} while ((mailbox == 0) && (timeout < 3000));

      status = *status_reg;
      if(mailbox == 0x5FFF)
	printf("TPB transfer to processor %d successful status is: %x\n",
                proc,status);
      else
	{
	  printf("TPB transfer unsuccessful proc: %d mailbox: %x status: %x\n",
          proc,mailbox,status);
	  exit(2);
	}

/* Transfer MAD structure to remote node */

      printf("\nTransferring MAD: %d to processor: %d\n",mad,proc);

      timeout = 0;
      mailbox = 0;
      *control_reg = TRIGGER;
      *start_reg = 0x0001;
      do
	{
	  timeout++;
	} while ((mailbox == 0) && (timeout < 3000));

      status = *status_reg;
      if(timeout < 3000)
      printf("Successful Xfer of MAD: %d to proc: %d status is: %x\n",
              mad,proc,status);
      else
	{
	printf("Unsuccessful xfer MAD: %d, proc: %d mailbox: %x status: %x\n",
                mad,proc,mailbox,status);
        exit(5);
        }
 
       } /* For loop for each MAD */
   } /* For loop for each processor */

/* Let the user select the various options */

printf("\nEnter time to sleep between transfers of data in milliseconds\n"); 
scanf("%d"&sleep_time);
printf("Do you want sinusoidal(1), random(2) or ramp(3) data?\n");
scanf("%ld"i);
switch(i)
  {
    case 1:   /* sinusoidal data */
/*******************************************************************/
/* Setup the generic TPBs that will do the actual transfer of data */
/*******************************************************************/

}
