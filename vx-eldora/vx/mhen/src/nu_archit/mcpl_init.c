/*
 *	$Id$
 *
 *	Module:	MCPL_INIT	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1994/07/14  20:32:58  eric
 * Initial revision
 *
 * Revision 1.1  1992/11/09  16:07:27  eric
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

mcpl_init()
{

volatile unsigned long *long_pntr;
long           i, j;

/* Define some general purpose variables */
short timeout, status, stat;


/* Initialize Data Duplication Path */

/* Initalize the arrays that index into the MAD descriptor arrays */
current_mad[DIS1] = 0;
current_mad[DIS2] = 0;
current_mad[REC] = 0;
current_mad[GRID] = 0;

number_of_mads[DIS1] = NUM_STRUCT_DIS1;
number_of_mads[DIS2] = NUM_STRUCT_DIS2;
number_of_mads[REC] = NUM_STRUCT_REC;
number_of_mads[GRID] = NUM_STRUCT_GRID;

proc_nodes[DIS1] = DISPLAY_NODE_1;
proc_nodes[DIS2] = DISPLAY_NODE_2;
proc_nodes[REC] = RECORD_NODE;
proc_nodes[GRID] = GRID_NODE;


madinfo[DIS1][0][MAD_BASE] = MADBASE_DIS1_0;
madinfo[DIS1][1][MAD_BASE] = MADBASE_DIS1_1;
madinfo[DIS2][0][MAD_BASE] = MADBASE_DIS2_0;
madinfo[DIS2][1][MAD_BASE] = MADBASE_DIS2_1;
madinfo[REC][0][MAD_BASE] = MADBASE_REC_0;
madinfo[REC][1][MAD_BASE] = MADBASE_REC_1;
madinfo[GRID][0][MAD_BASE] = MADBASE_GRID_0;
madinfo[GRID][1][MAD_BASE] = MADBASE_GRID_1;

madinfo[DIS1][0][COMPLETE_S] = C_STATUS_DIS1;
madinfo[DIS1][1][COMPLETE_S] = C_STATUS_DIS1;
madinfo[DIS2][0][COMPLETE_S] = C_STATUS_DIS2;
madinfo[DIS2][1][COMPLETE_S] = C_STATUS_DIS2;
madinfo[REC][0][COMPLETE_S] = C_STATUS_REC;
madinfo[REC][1][COMPLETE_S] = C_STATUS_REC;
madinfo[GRID][0][COMPLETE_S] = C_STATUS_GRID;
madinfo[GRID][1][COMPLETE_S] = C_STATUS_GRID;

madinfo[DIS1][0][MAX_MEM] = DIS1_MEM_SIZE_0;
madinfo[DIS1][1][MAX_MEM] = DIS1_MEM_SIZE_1;
madinfo[DIS2][0][MAX_MEM] = DIS2_MEM_SIZE_0;
madinfo[DIS2][1][MAX_MEM] = DIS2_MEM_SIZE_1;
madinfo[REC][0][MAX_MEM] = REC_MEM_SIZE_0;
madinfo[REC][1][MAX_MEM] = REC_MEM_SIZE_1;
madinfo[GRID][0][MAX_MEM] = GRID_MEM_SIZE_0;
madinfo[GRID][1][MAX_MEM] = GRID_MEM_SIZE_1;

madinfo[DIS1][0][NUM_C_AREA] = radar_fore_aft;
madinfo[DIS1][1][NUM_C_AREA] = radar_fore_aft;
madinfo[DIS2][0][NUM_C_AREA] = radar_fore_aft;
madinfo[DIS2][1][NUM_C_AREA] = radar_fore_aft;
madinfo[REC][0][NUM_C_AREA] = radar_fore_aft;
madinfo[REC][1][NUM_C_AREA] = radar_fore_aft;
madinfo[GRID][0][NUM_C_AREA] = radar_fore_aft;
madinfo[GRID][1][NUM_C_AREA] = radar_fore_aft;

/* Set addresses for the MCPL card */

status_reg =  (short *)(MCPL_BASE + STATUS_REG);
control_reg = (short *)(MCPL_BASE + CONTROL_REG);
short_fifo =  (short *)(MCPL_BASE + FIFO_REG);
long_fifo =   (long *)(MCPL_BASE + FIFO_REG);
start_reg =   (short *)(MCPL_BASE + START_REG);

init =  (struct TPB *)(VMEMEM_BASE + STD_BASE + INIT_OFFSET);
tpb_to_proc =   (struct TPB *)(VMEMEM_BASE + STD_BASE + TPB_TO_PROC_OFFSET);
receive_gmad =   (struct TPB *)(VMEMEM_BASE + STD_BASE + RECEIVE_GMAD_OFFSET);
receive_data =   (struct TPB *)(VMEMEM_BASE + STD_BASE + RECEIVE_DATA_OFFSET);
gmad_to_proc =   (struct TPB *)(VMEMEM_BASE + STD_BASE + GMAD_TO_PROC_OFFSET);
bcast_tpb =     (struct TPB *)(VMEMEM_BASE + STD_BASE + BCAST_TPB_OFFSET);
gmad =     (struct MAD *)(VMEMEM_BASE + STD_BASE + GMAD_OFFSET);
mailbox =   (short *)(VMEMEM_BASE + STD_BASE + MAILBOX_OFFSET);
ray =   (struct RAY *)(VMEMEM_BASE + STD_BASE + DATA_RAY_BASE);

printf("init address = %x \n",init);
printf("tpb_to_proc address = %x \n",tpb_to_proc);
printf("receive_gmad address = %x \n",receive_gmad);
printf("receive_data address = %x \n",receive_data);
printf("gmad_to_proc address = %x \n",gmad_to_proc);
printf("bcast_tpb address = %x \n",bcast_tpb);
printf("gmad address = %x \n",gmad);

/* for test purposes, clear the memory */
long_pntr = (long *)mailbox;
for(i = 0; i< 262144; i++)
  *(long_pntr+i) = 0;

/* Set up the initial TPB */

init->source_start_address = 0;
init->sink_start_address = 0;
init->source_attribute = 0;
init->sink_attribute = 0;
init->transfer_length = 0;
init->completion_status = 0x0006;  /* Disable interrupt Mailbox A32/D16 */
init->nack_count = 0;
init->link_address = (long)tpb_to_proc - STD_BASE;
init->link_attribute = 0x0933;     /* Normal VMEBus Addressing, VME link after
                                     trigger, address modifier $09, disable
                                     BLT, 32 Xfers/burst, disable pipelineing,
                                     link to VME */
init->mcpl_control_word = 0;
init->mailbox_address = (long)mailbox - STD_BASE;

printf("mailbox address is: %lx\n",mailbox);

/* Check status register and print its contents */

status = *status_reg;
printf("Initial card status is: %x\n",status);
/* Loop infinitely until reset is completed successfully */
do
  {
/* Send a reset command to the mcpl */

      printf("Sending the MCPL node a reset command\n");
      *control_reg = RESET;
      *start_reg = 0x0001;

      timeout = 0;
      do
	{
	    status = *status_reg;
	    timeout++;
	} while (((status & 0x0001) != 0) && (timeout < 300));

      if(timeout >= 300)
	{
	    printf("MCPL card still busy after issueing reset command, must exit\n");
	    stat = 1;
	    /*      return(0); */
	}
      else
	stat = 0;
      taskDelay(5);

/* Send the init TPB to the MCPL */

      printf("Sending Initialization TPB \n");

      *control_reg = READ_INIT_TPB;
      long_pntr = (long *)init;
      
      for(i=0; i<8; i++)
	*long_fifo = *(long_pntr + i);
      
      *start_reg = 0x0001;

      timeout = 0;
      *mailbox = 0;
      do
	{
	    timeout++;
	} while ((*mailbox == 0) && (timeout < 3000));

      status = *status_reg;
      if(*mailbox == 0x1fff)
	{
	    printf("Init TPB transfer successful status is: %x\n",status);
	    stat = 0;
	}
      else
	{
	    printf("Init TPB transfer unsuccessful mailbox: %x status: %x\n",
		   *mailbox,status);
	    currStatus->mcpl |= INIT_MCPL;
	    currStatus->mailbox = *mailbox;
	    currStatus->status = status;
	    stat = 1;
	    /*  return(2); */
	}
  } while(stat == 1);
return(-1);
}



































