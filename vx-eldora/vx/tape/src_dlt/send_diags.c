/*
 *	$Id$
 *
 *	Module: send_diags
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:  Sends the "Send Diagnostics" SCSI command to 
 *               an DLT tape drive 
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <tapeDef.h>
#include <tapeGbl.h>

/************ SEND DIAGNOSTIC FUNCTION *************************/

void send_diags(char test,short list_length,char mem_dump,char drv_num)
{
unsigned short *addr_buff;
volatile unsigned short *status;
unsigned short new_stat,old_stat;
int i,stat,j;
union 
  {
     PARMBLK *pb;
     unsigned short pb_addr[2];
  }addr;

union
  {
     short param_length;
     char param[2];
  }list;


addr_buff=(unsigned short *)(CIP_BASE+SHORT_BASE+CIP_ADDR_BUFF);
status=(unsigned short *)(CIP_BASE+SHORT_BASE+CIP_BD_STAT);
old_stat=*status;
new_stat=*status;
list.param_length=list_length;

/******************* SETUP CIPRICO PARAMETER BLOCK *****************/

parmblk[SEND_DIAGNOSTICS]->cmd_id = SEND_DIAGNOSTICS;
parmblk[SEND_DIAGNOSTICS]->resvd = RSVD;
parmblk[SEND_DIAGNOSTICS]->flags = 0x68;
parmblk[SEND_DIAGNOSTICS]->addr_mod = AM;
parmblk[SEND_DIAGNOSTICS]->targ_id = drv_num;
parmblk[SEND_DIAGNOSTICS]->vme_addr = 0;
parmblk[SEND_DIAGNOSTICS]->xfer_count = 0;
parmblk[SEND_DIAGNOSTICS]->scsi_blk[0]=SND_DIAGS;
parmblk[SEND_DIAGNOSTICS]->scsi_blk[1]=test;
parmblk[SEND_DIAGNOSTICS]->scsi_blk[2]=RSVD;
parmblk[SEND_DIAGNOSTICS]->scsi_blk[3]=list.param[0];
parmblk[SEND_DIAGNOSTICS]->scsi_blk[4]=list.param[1];
parmblk[SEND_DIAGNOSTICS]->scsi_blk[5]=mem_dump;

/******************* LOAD CIPRICO ADDRESS BUFFER *******************/

i = (int)parmblk[SEND_DIAGNOSTICS];
i += tape_vme_offset;
addr.pb = (PARMBLK *)i;
*addr_buff=CB_AM; /* Control Byte and Address Modifier */
*addr_buff=addr.pb_addr[0]; /* MSW OF paramblk addr */
*addr_buff=addr.pb_addr[1]; /* LSW of paramblk addr */ 
chan_attn(0); /* Issue channel attention 0 for sending single commands */

/********** IS CIPRICO READY FOR ANOTHER COMMAND?? ****************/

i=0; /* INIT LOOP COUNTER */
while(old_stat==new_stat) /* Watch for ENT bit to toggle */
  {                       /* to show if board is ready for */
      new_stat=*status;   /* another command. */             
      i++;                     
      if(i>=50000)
	{
	    puts("SEND_DIAGS CHAN ATTN TIMEOUT");
	    printf("STATUS = %X\n",*status);
	    exit(0);
	}
  }
/******************* IS COMMAND COMPLETE?? ************************/


i=0;
while(parmblk[SEND_DIAGNOSTICS]->stat_flags==0x00)  /* Watch for command complete */
  {                                       /* flag showing command was   */
      i++;                                /* successfully completed     */
      if(i>=50000000)                       /* If not drop out and show   */
	{                                 /* error status.              */
	    puts("TIME EXCEEDED");    /* error status.              */
	    break;
	}
      taskDelay(1); /* Give us a little delay */
  }
switch(parmblk[SEND_DIAGNOSTICS]->stat_flags)
  {
    case 0xC2:
      printf("SEND DIAGS COMMAND COMPLETE WITH SOFT ERROR\n");
      stat=1;
      break;
    case 0xC0:
      printf("SEND_DIAGS COMMAND COMPLETE WITH ERROR\n");
      printf("ERROR RETURNED FROM DLT = %X\n",
	     parmblk[SEND_DIAGNOSTICS]->scsi_status);
      printf("CIPRICO ERROR STATUS= %X\n",parmblk[SEND_DIAGNOSTICS]->error);
      printf("CIPRICO BD STATUS= %X\n",*status);
      taskDelay(100);
      break;
    case 0xD0:
      printf("SEND_DIAGS COMMAND COMPLETE WITH TRUNCATED DATA DRV%d\n",
	     drv_num);
      stat=1;
      break;
    case 0xE0:
      printf("SEND_DIAGS COMMAND COMPLETE BUT REQUIRED RETRIES\n");
      stat=1;
      break;
    case 0x50:
      printf("SEND_DIAGS COMMAND NOT COMPLETE DUE TO TRUNCATED DATA\n");
      stat=1;
      break;
    case 0x60:
      printf("SEND_DIAGS COMMAND NOT COMPLETE DUE TO RETRIES\n");
      stat=1;
      break;
    case 0x44:
      printf("SEND_DIAGS COMMAND NOT COMPLETE DUE TO ERROR\n");
      stat=1;
      break;
    case 0x80:
      stat=0;
      break;
  }

parmblk[SEND_DIAGNOSTICS]->stat_flags=0x00; /* Clear status flag */
return;
}
