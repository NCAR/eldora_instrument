/*
 *	$Id$
 *
 *	Module:		 
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1994/01/06  21:31:22  craig
 * Initial revision
 *
 * Revision 1.7  1993/09/22  15:27:05  reif
 * changed error checking
 *
 * Revision 1.6  1992/11/04  04:52:46  reif
 * *** empty log message ***
 *
 * Revision 1.5  1992/10/30  23:54:31  reif
 * *** empty log message ***
 *
 * Revision 1.4  1992/10/25  17:07:53  reif
 * *** empty log message ***
 *
 * Revision 1.3  1992/10/17  21:31:53  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/15  17:52:30  reif
 * *** empty log message ***
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>
#define scope extern

/********************* EXABYTE CONTROL COMMANDS ***************/

void exb_cmds(unsigned int cmnd_ident,unsigned char op_cd,unsigned char drv_num)
{
unsigned short *addr_buff;
volatile unsigned short *status;
unsigned short new_stat,old_stat;
unsigned int xfer_count;
unsigned int vme_addr;
unsigned char op1,op2,op3,op4,op5,op6,op7,op8,op9,flags;
int j,stat;
unsigned long i;
union 
  {
     PARMBLK *pb;
     unsigned short pb_addr[2];
  }addr;

addr_buff=(unsigned short *)(CIP_BASE+CIP_ADDR_BUFF);
status=(unsigned short *)(CIP_BASE+CIP_BD_STAT);
old_stat=*status;
new_stat=*status;
if(cmnd_ident<0 || cmnd_ident>LAST_CMD_ID)
  {
      printf("WE'Re FUCKED!!\n");
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
      vme_addr=(unsigned int)&mod_sel;/* ADDRESS OF MOD_SEL STRUCTURE */
      xfer_count=0x00000000;
      op1=0x10;
      op2=0x00;
      op3=0x00;
      op4=sizeof(mod_sel);
      op5=0x00;
      break;
    case UNLOAD:
      vme_addr=0x00000000;
      xfer_count=0x00000000;
      op1=0x01;
      op2=0x00;
      op3=0x00;
      op4=0x00;
      op5=0x00;
      break;
    case LOAD:
      vme_addr=0x00000000;
      xfer_count=0x00000000;
      op1=0x01;
      op2=0x00;
      op3=0x00;
      op4=0x01;
      op5=0x00;
      break;
    case REWND:
      vme_addr=0x00000000;
      xfer_count=0x00000000;
      op1=0x01;
      op2=0x00;
      op3=0x00;
      op4=0x00;
      op5=0x00;
      break;
    case MODE_SENSE:
      vme_addr=(unsigned int)&mod_sen;/* BEGINNING OF MOD_SEL */
      xfer_count=0x00000000;
      op1=0x00;
      op2=0x10;           /* Device Configuration page only */
      op3=0x00;
      op4=sizeof(mod_sen);
      op5=0x00;
      break;
    case REQUEST_SENSE:
      vme_addr=(unsigned int)&rqst_sen;/* BEGINNING OF rqst_sen */
      xfer_count=0x00000000;
      op1=0x00;
      op2=0x00;
      op3=0x00;
      op4=0x13;
      op5=0x00;
      return;
      break;
    case WRITE_FILEMARK:
      vme_addr=0x00000000;
      xfer_count=0x00000000;
      op1=0x00;
      op2=0x00;
      op3=0x00;
      op4=0x01;
      op5=0x00;
      break;
    case ERASE:
      vme_addr=0x00000000;
      xfer_count=0x00000000;
      op1=0x03;
      op2=0x00;
      op3=0x00;
      op4=0x00;
      op5=0x00;
      break;
    case VERIFY:
      vme_addr=0x00000000;
      xfer_count=0x00000000;
      op1=0x00;
      op2=0x00;
      op3=0x80;
      op4=0x00;
      op5=0x00;
      break;
    case LOG_SELECT:
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
    case LOG_SENSE:
      vme_addr=(unsigned int)&log_page;/* BEGINNING OF log page */
      xfer_count=sizeof(log_page);
      flags = 0x6C;
      op1=0x00;
      op2=0x42;           /* cumulative values, write errors */
      op3=0x00;
      op4=0x00;
      op5=0x00;
      op6=0x00;
      op7=0x00;
      op8=sizeof(log_page);
      op9=0x00;
      break;
  }
/******************* SETUP CIPRICO PARAMETER BLOCK *****************/

parmblk[cmnd_ident].cmd_id = cmnd_ident;
parmblk[cmnd_ident].resvd = 0x00;
parmblk[cmnd_ident].flags = flags;
parmblk[cmnd_ident].addr_mod = AM;
parmblk[cmnd_ident].targ_id = drv_num;
parmblk[cmnd_ident].vme_addr =vme_addr;
parmblk[cmnd_ident].xfer_count = xfer_count;
parmblk[cmnd_ident].scsi_blk[0]=op_cd;
parmblk[cmnd_ident].scsi_blk[1]=op1;
parmblk[cmnd_ident].scsi_blk[2]=op2;
parmblk[cmnd_ident].scsi_blk[3]=op3;
parmblk[cmnd_ident].scsi_blk[4]=op4;
parmblk[cmnd_ident].scsi_blk[5]=op5;
parmblk[cmnd_ident].scsi_blk[5]=op6;
parmblk[cmnd_ident].scsi_blk[5]=op7;
parmblk[cmnd_ident].scsi_blk[5]=op8;
parmblk[cmnd_ident].scsi_blk[5]=op9;

/******************* LOAD CIPRICO ADDRESS BUFFER *******************/

addr.pb=&parmblk[cmnd_ident];
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
	    puts("CHAN ATTN TIMEOUT\n");
	    print_stat(stat,cmnd_ident);
	    break;
	}
  }
/******************* IS COMMAND COMPLETE?? ************************/
i=0;
while(parmblk[cmnd_ident].stat_flags!=0x80) /* Watch for command complete */
  {                                         /* flag showing command was   */
      i++;                                  /* successfully completed     */
      if(i==500000000)                      /* If not drop out and show   */
	{                                   /* error status.   */
	    printf("COMMAND #%X TIMEOUT in exbcmds\n",cmnd_ident);
	    stat=1;                       
	    break;
	}
  }
switch(parmblk[cmnd_ident].stat_flags)
  {
    case 0xC2:
      printf("EXB #%d COMMAND COMPLETE WITH SOFT ERROR\n",cmnd_ident);
      stat=1;
      break;
    case 0xC0:
      printf("EXB #%d COMMAND COMPLETE WITH ERROR\n",cmnd_ident);
      printf("ERROR RETURNED FROM EXABYTE= %X\n",parmblk[cmnd_ident].scsi_status);
      printf("CIPRICO ERROR STATUS= %X\n",parmblk[cmnd_ident].error);
      printf("CIPRICO BD STATUS= %X\n",*status);
      taskDelay(100);
      break;
    case 0xD0:
      printf("EXB #%d COMMAND COMPLETE WITH TRUNCATED DATA DRV%d\n",drv_num,cmnd_ident);
      stat=1;
      break;
    case 0xE0:
      printf("EXB #%d COMMAND COMPLETE BUT REQUIRED RETRIES\n",cmnd_ident);
      stat=1;
      break;
    case 0x50:
      printf("EXB #%d COMMAND NOT COMPLETE DUE TO TRUNCATED DATA\n",cmnd_ident);
      stat=1;
      break;
    case 0x60:
      printf("EXB #%d COMMAND NOT COMPLETE DUE TO RETRIES\n",cmnd_ident);
      stat=1;
      break;
    case 0x44:
      printf("EXB #%d COMMAND NOT COMPLETE DUE TO ERROR\n",cmnd_ident);
      stat=1;
      break;
    case 0x80:
      stat=0;
      break;
  }
parmblk[cmnd_ident].stat_flags=0x00; /* Clear status flag */
return;
}


