/*
 *	$Id$
 *
 *	Module:	dlt_cmds	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:  Sends the requested SCSI command to the requested device 
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <tapeDef.h>
#include <tapeGbl.h>

/********************* DLT CONTROL COMMANDS ***************/

void dlt_cmds(unsigned int cmnd_ident,unsigned char drv_num)
{

unsigned short *addr_buff;
volatile unsigned short *status;
unsigned short new_stat,old_stat;
unsigned int xfer_count;
unsigned int vme_addr;
unsigned char op1,op2,op3,op4,op5,op_cd;
int j,stat;
unsigned long i;
union 
  {
     PARMBLK *pb;
     unsigned short pb_addr[2];
  }addr;

addr_buff=(unsigned short *)(CIP_BASE+SHORT_BASE+CIP_ADDR_BUFF);
status=(unsigned short *)(CIP_BASE+SHORT_BASE+CIP_BD_STAT);

old_stat=*status;
new_stat=*status;
if(cmnd_ident<0 || cmnd_ident>LAST_CMD_ID)
  {
      printf("Unknown command identification given\n");
      return;
  }

switch(cmnd_ident)
  {
    case MODE_SELECT:
      op_cd = MD_SEL;
      /* ADDRESS OF MOD_SEL STRUCTURE */
      vme_addr=(unsigned int)((int)mod_sel + tape_vme_offset);
      xfer_count=0x00000000;
      op1=0x00;
      op2=0x00;
      op3=0x00;
      op4=0x11;
      op5=0x00;
      break;
    case UNLOAD:
      op_cd = ULD;
      vme_addr=0x00000000;
      xfer_count=0x00000000;
      op1=0x01;
      op2=0x00;
      op3=0x00;
      op4=0x00;
      op5=0x00;
      break;
    case REWND:
      op_cd = RWND;
      vme_addr=0x00000000;
      xfer_count=0x00000000;
      op1=0x01;
      op2=0x00;
      op3=0x00;
      op4=0x00;
      op5=0x00;
      break;
    case MODE_SENSE:
      op_cd = MD_SNS;
      /* BEGINNING OF MOD_SEL */
      vme_addr=(unsigned int)((int)mod_sen + tape_vme_offset);
      xfer_count=0x00000000;
      op1=0x00;
      op2=0x00;
      op3=0x00;
      op4=0x11;
      op5=0x00;
      break;
    case REQUEST_SENSE:
      op_cd = RQST_SNS;
      /* BEGINNING OF request sense */
      vme_addr=(unsigned int)((int)rqst_sen + tape_vme_offset);
      xfer_count=0x00000000;
      op1=0x00;
      op2=0x00;
      op3=0x00;
      op4=0x1D;
      op5=0x00;
      break;
    case WRITE_FILEMARK:
      op_cd = WRT_FLMK;
      vme_addr=0x00000000;
      xfer_count=0x00000000;
      op1=0x00;
      op2=0x00;
      op3=0x00;
      op4=0x01;
      op5=0x00;
      break;
    case ERASE:
      op_cd = ERS;
      vme_addr=0x00000000;
      xfer_count=0x00000000;
      op1=0x03;
      op2=0x00;
      op3=0x00;
      op4=0x00;
      op5=0x00;
      break;
    case VERIFY:
      op_cd = VFY;
      vme_addr=0x00000000;
      xfer_count=0x00000000;
      op1=0x00;
      op2=0x00;
      op3=0x80;
      op4=0x00;
      op5=0x00;
      break;
  }
/******************* SETUP CIPRICO PARAMETER BLOCK *****************/

parmblk[cmnd_ident]->cmd_id = cmnd_ident;
parmblk[cmnd_ident]->resvd = 0x00;
parmblk[cmnd_ident]->flags = 0x68;
parmblk[cmnd_ident]->addr_mod = AM;
parmblk[cmnd_ident]->targ_id = drv_num;
parmblk[cmnd_ident]->vme_addr =vme_addr;
parmblk[cmnd_ident]->xfer_count = xfer_count;
parmblk[cmnd_ident]->scsi_blk[0]=op_cd;
parmblk[cmnd_ident]->scsi_blk[1]=op1;
parmblk[cmnd_ident]->scsi_blk[2]=op2;
parmblk[cmnd_ident]->scsi_blk[3]=op3;
parmblk[cmnd_ident]->scsi_blk[4]=op4;
parmblk[cmnd_ident]->scsi_blk[5]=op5;

/******************* LOAD CIPRICO ADDRESS BUFFER *******************/

i = (int)parmblk[cmnd_ident];
i += tape_vme_offset;
addr.pb=(PARMBLK *)i;
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
      if(i==500000000)
	{
	    stat=1;
	    puts("CHAN ATTN TIMEOUT");
	    print_stat(stat,cmnd_ident);
	    break;
	}
  }
/******************* IS COMMAND COMPLETE?? ************************/
i=0;
while(parmblk[cmnd_ident]->stat_flags!=0x80) /* Watch for command complete */
  {                                       /* flag showing command was   */
      i++;                                /* successfully completed     */
      if(i==500000000)                      /* If not drop out and show   */
	{ 
	    printf("COMMAND #%X\n TIMEOUT",cmnd_ident);/* error status.   */
	    stat=1;                       
	    break;
	}
  }
switch(parmblk[cmnd_ident]->stat_flags)
  {
    case 0xC2:
      printf("DLT #%d COMMAND COMPLETE WITH SOFT ERROR\n",cmnd_ident);
      stat=1;
      break;
    case 0xC0:
      printf("DLT #%d COMMAND COMPLETE WITH ERROR\n",cmnd_ident);
      printf("ERROR RETURNED FROM DLT= %X\n",parmblk[cmnd_ident]->scsi_status);
      printf("CIPRICO ERROR STATUS= %X\n",parmblk[cmnd_ident]->error);
      printf("CIPRICO BD STATUS= %X\n",*status);
      taskDelay(100);
      break;
    case 0xD0:
      printf("DLT #%d COMMAND COMPLETE WITH TRUNCATED DATA DRV%d\n",
	     drv_num,cmnd_ident);
      stat=1;
      break;
    case 0xE0:
      printf("DLT #%d COMMAND COMPLETE BUT REQUIRED RETRIES\n",cmnd_ident);
      stat=1;
      break;
    case 0x50:
      printf("DLT #%d COMMAND NOT COMPLETE DUE TO TRUNCATED DATA\n",
	     cmnd_ident);
      stat=1;
      break;
    case 0x60:
      printf("DLT #%d COMMAND NOT COMPLETE DUE TO RETRIES\n",cmnd_ident);
      stat=1;
      break;
    case 0x44:
      printf("DLT #%d COMMAND NOT COMPLETE DUE TO ERROR\n",cmnd_ident);
      stat=1;
      break;
    case 0x80:
      stat=0;
      break;
  }

parmblk[cmnd_ident]->stat_flags=0x00; /* Clear status flag */

return;
}


