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
 * Revision 1.2  1996/09/03  16:39:25  craig
 * cleaned up
 *
 * Revision 1.1  1996/06/18  16:02:34  craig
 * Initial revision
 *
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

void dlt_cmds(unsigned int cmnd_ident,unsigned char drv_num, int control_am)
{

unsigned short *addr_buff;
volatile unsigned short *status;
unsigned short new_stat,old_stat;
unsigned int xfer_count;
unsigned int vme_addr;
unsigned char op1,op2,op3,op4,op5,op6,op7,op8,op9,flags,op_cd,test;
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


op6 = 0;
op7 = 0;
op8 = 0;
op9 = 0;
flags = 0x68;

switch(cmnd_ident)
  {
    case MODE_SELECT:
      op_cd = MD_SEL;
      /* ADDRESS OF MOD_SEL STRUCTURE */
      vme_addr=(unsigned int)((int)mod_sel + tape_vme_offset);
      xfer_count=0x00000000;
      op1=0x10;
      op2=0x00;
      op3=0x00;
      op4=sizeof(struct mode_struct);
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
    case LOAD:
      op_cd = LD_ULD;
      vme_addr=0x00000000;
      xfer_count=0x00000000;
      op1=0x01;
      op2=0x00;
      op3=0x00;
      op4=0x01;
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
      /* BEGINNING OF THE MODE SENCE STRUCTURE */
      vme_addr=(unsigned int)((int)mod_sen + tape_vme_offset);
      xfer_count=0x00000000;
      op1=0x00;
      op2=0x10;           /* Device Configuration page only */
      op3=0x00;
      op4=sizeof(struct mode_struct);
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
      op4=0x13;
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
    case LOG_SELECT:  /* This resets all the logs maintained in the DLTs */
      op_cd = LOG_SEL;
      vme_addr=0x00000000;
      xfer_count=0x00000000;
      op1=0x02;
      op2=0x00;
      op3=0x00;
      op4=0x00;
      op5=0x00;
      op6=0x00;
      op7=0x00;
      op8=0x00;
      op9=0x00;
      break;
    case LOG_SENSE: /* Asks for cumlative values of support write errors */
      op_cd = LOG_SEN;
      /* BEGINNING OF log page */
      vme_addr=(unsigned int)((int)log_page + tape_vme_offset);
      xfer_count=sizeof(struct log_pg);
      flags = 0x6C;
      op1=0x00;
      op2=0x42;           /* cumulative values, write errors */
      op3=0x00;
      op4=0x00;
      op5=0x00;
      op6=0x00;
      op7=0x00;
      op8=sizeof(struct log_pg);
      op9=0x00;
      break;
    case LOG_SENSE_SPGS:  /* Request list of supported log sence pages */ 
      op_cd = LOG_SEN;
      /* BEGINNING OF suprt_pgs */
      vme_addr=(unsigned int)((int)suprt_pgs + tape_vme_offset);
      xfer_count=sizeof(struct supported_pgs);
      flags = 0x6C;
      op1=0x00;
      op2=0x00;           /* cumulative values, write errors */
      op3=0x00;
      op4=0x00;
      op5=0x00;
      op6=0x00;
      op7=0x00;
      op8=sizeof(struct supported_pgs);
      op9=0x00;
      break;

  }
/******************* SETUP CIPRICO PARAMETER BLOCK *****************/

parmblk[cmnd_ident]->cmd_id = cmnd_ident;
parmblk[cmnd_ident]->resvd = 0x00;
parmblk[cmnd_ident]->addr_mod = control_am;
parmblk[cmnd_ident]->targ_id = drv_num;
parmblk[cmnd_ident]->vme_addr =vme_addr;
parmblk[cmnd_ident]->xfer_count = xfer_count;
parmblk[cmnd_ident]->scsi_blk[0]=op_cd;
parmblk[cmnd_ident]->scsi_blk[1]=op1;
parmblk[cmnd_ident]->scsi_blk[2]=op2;
parmblk[cmnd_ident]->scsi_blk[3]=op3;
parmblk[cmnd_ident]->scsi_blk[4]=op4;
parmblk[cmnd_ident]->scsi_blk[5]=op5;
parmblk[cmnd_ident]->flags = flags;
parmblk[cmnd_ident]->scsi_blk[6]=op6;
parmblk[cmnd_ident]->scsi_blk[7]=op7;
parmblk[cmnd_ident]->scsi_blk[8]=op8;
parmblk[cmnd_ident]->scsi_blk[9]=op9;

/******************* LOAD CIPRICO ADDRESS BUFFER *******************/

i = (int)parmblk[cmnd_ident];
i += tape_vme_offset;
addr.pb=(PARMBLK *)i;
*addr_buff = control_am | CONTROL_BYTE; /* Control Byte and Address Modifier */
*addr_buff=addr.pb_addr[0]; /* MSW OF paramblk addr */
*addr_buff=addr.pb_addr[1]; /* LSW of paramblk addr */ 
chan_attn(0); /* Issue channel attention 0 for sending single commands */

/********** IS CIPRICO READY FOR ANOTHER COMMAND?? ****************/

i=0; /* INIT LOOP COUNTER */
while(old_stat==new_stat) /* Watch for ENT bit to toggle */
  {                       /* to show if board is ready for */
      new_stat=*status;   /* another command. */             
      i++;                     
      if(i==5000)
	{
	    stat=1;
	    puts("CHAN ATTN TIMEOUT");
	    print_stat(stat,cmnd_ident);
	    break;
	}
  }
/******************* IS COMMAND COMPLETE?? ************************/
 /* Watch for command complete bit, timeout if it does not happen */
i=0;
test = 0;
while(test != 0x80)
  {
      test = parmblk[cmnd_ident]->stat_flags & 0x80;
      i++;
      if(i==5000000)
	{
	    printf("COMMAND #%X\n TIMEOUT",cmnd_ident);
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

if(stat == 1) print_stat(stat,cmnd_ident);
parmblk[cmnd_ident]->stat_flags=0x00; /* Clear status flag */

return;
}


