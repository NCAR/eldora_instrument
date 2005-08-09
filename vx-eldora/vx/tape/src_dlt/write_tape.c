/*
 *	$Id$
 *
 *	Module: write_tape
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1996/11/15  22:06:41  craig
 * *** empty log message ***
 *
 * Revision 1.1  1996/06/18  16:03:25  craig
 * Initial revision
 *
 *
 * description: Writes a record to the tape drive, data can be either in
 *              blocked mode or in a scatter-gather configuration
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <tapeDef.h>
#include <tapeGbl.h>

/************ WRITE TAPE FUNCTION *************************/

char write_tape(unsigned int *data_addr,unsigned int data_len,unsigned char sgflg,unsigned char drv_num, int control_am, int data_am)
{
unsigned short *addr_buff;
volatile unsigned short *status;
unsigned short new_stat,old_stat;
unsigned char op1,op2,op3,op4,op5;
unsigned int cmnd_ident;
int j;
char stat;
long i;

union 
  {
     PARMBLK *pb;
     unsigned short pb_addr[2];
  }addr;

union
  {
     unsigned long dat_length;
     unsigned char dat_len[4];
  }len;

addr_buff=(unsigned short *)(CIP_BASE+SHORT_BASE+CIP_ADDR_BUFF);
status=(unsigned short *)(CIP_BASE+SHORT_BASE+CIP_BD_STAT);
cmnd_ident=TAPE_WRITE; 
len.dat_length=data_len;
op1=0x00; /* Variable length data */


/******************* IS PREVIOUS COMMAND COMPLETE?? ************************/

i=0;
while(parmblk[cmnd_ident]->stat_flags==0x00) /* Watch for command complete */
  {                                         /* flag showing command was   */
      i++;                                  /* successfully completed     */
      if(i>=500000000)                      /* If not drop out and show   */
	{                                   /* error status.              */
	    printf("TIME EXCEEDED\n");
	    taskDelay(200);
	    break;
	}
  }

switch(parmblk[cmnd_ident]->stat_flags)
  {
    case 0xC2:
      printf("WRITE COMMAND COMPLETE WITH SOFT ERROR\n");
      stat=0xC2;
      return(stat);
      break;
    case 0xC0:
      printf("WRITE COMMAND COMPLETE WITH ERROR\n");
      stat=0xC0;
      return(stat);
      break;
    case 0x90:
/*      printf("WRITE COMMAND COMPLETE WITH TRUNCATED DATA DRV%d\n",drv_num);
      stat=0x90; */
      stat = 0;
      break;
    case 0xE0:
      printf("WRITE COMMAND COMPLETE BUT REQUIRED RETRIES\n");
      stat=0xE0;
      return(stat);
      break;
    case 0x60:
      printf("WRITE COMMAND NOT COMPLETE DUE TO RETRIES\n");
      stat=0x60;
      return(stat);
      break;
    case 0x44:
      printf("WRITE COMMAND NOT COMPLETE DUE TO ERROR\n");
      stat=0x44;
      return(stat);
      break;
    case 0x80:   /* 0x08 is the good completion status */
      stat=0;
      break;
    case 0x00:
      stat=1;
      printf("COMMAND NEVER COMPLETED\n");
      return(stat);
      break;
    default:
      stat=parmblk[cmnd_ident]->stat_flags;
      break;
  }

parmblk[cmnd_ident]->stat_flags=0x00; /* Clear status flag */

/* Set old status and new status to current status */

old_stat=*status;
new_stat=*status;

/* Break data into 4 bytes and use the last 3 */
op2=len.dat_len[1]; /* MSB of data transfer length */
op3=len.dat_len[2]; /* MID of data transfer length */
op4=len.dat_len[3]; /* LSB of data transfer length */ 
op5=0x00;

/******************* SETUP CIPRICO PARAMETER BLOCK *****************/

parmblk[cmnd_ident]->cmd_id = cmnd_ident;
parmblk[cmnd_ident]->resvd = 0x00;
parmblk[cmnd_ident]->flags = sgflg; /*Set for scatter/gather or write buffer */
if(sgflg == BLOCKED)
  parmblk[cmnd_ident]->addr_mod = data_am;
else
  parmblk[cmnd_ident]->addr_mod = control_am;
parmblk[cmnd_ident]->targ_id = drv_num;
parmblk[cmnd_ident]->vme_addr = (unsigned int)data_addr;
parmblk[cmnd_ident]->xfer_count = 0x00008000;
parmblk[cmnd_ident]->scsi_blk[0]=TP_WRT;
parmblk[cmnd_ident]->scsi_blk[1]=op1;
parmblk[cmnd_ident]->scsi_blk[2]=op2;
parmblk[cmnd_ident]->scsi_blk[3]=op3;
parmblk[cmnd_ident]->scsi_blk[4]=op4;
parmblk[cmnd_ident]->scsi_blk[5]=op5;


/******************* LOAD CIPRICO ADDRESS BUFFER *******************/

i = (int)parmblk[cmnd_ident];
i += tape_vme_offset;
addr.pb= (PARMBLK *)i;
*addr_buff = control_am | CONTROL_BYTE;
*addr_buff=addr.pb_addr[0]; /* MSW OF paramblk addr */
*addr_buff=addr.pb_addr[1]; /* LSW of paramblk addr */ 
chan_attn(0); /* Issue channel attention 0 for sending single commands */

/********** IS CIPRICO READY FOR ANOTHER COMMAND?? ****************/

i=0; /* INIT LOOP COUNTER */
while(old_stat==new_stat) /* Watch for ENT bit to toggle */
  {                       /* to show if board is ready for */
      new_stat=*status;   /* another command. */             
      i++;                     
      if(i>=5000)
	{
	    printf("WRITE CHAN ATTN TIMEOUT\n");
	    printf("STATUS = %X\n",*status);
	    taskDelay(200);
	    break;
	}
  }

return(stat);
}







