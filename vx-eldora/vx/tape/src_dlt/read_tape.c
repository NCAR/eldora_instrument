/*
 *	$Id$
 *
 *	Module:read_tape.c		 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1996/06/18  16:03:34  craig
 * Initial revision
 *
 *
 * description:  Read a record from the specified tape drive
 *        
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <tapeDef.h>
#include <tapeGbl.h>

int read_tape(unsigned int *data_addr,unsigned int data_length,unsigned char sgflg,unsigned char drv_num)
{
unsigned short *addr_buff;
volatile unsigned short *status;
unsigned short new_stat,old_stat;
unsigned char op1,op2,op3,op4,op5;
unsigned int cmnd_ident;
int i,stat,j;
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
old_stat=*status;
new_stat=*status;
cmnd_ident=TAPE_READ; 
len.dat_length=data_length;
op1=0x00; /* Variable length data/ILLEGAL LENGTH REPORTED */
/* Break data into 4 bytes and use the last 3 */
op2=len.dat_len[1]; /* MSB of data transfer length */
op3=len.dat_len[2]; /* MID of data transfer length */
op4=len.dat_len[3]; /* LSB of data transfer length */ 
op5=0x00;

/******************* SETUP CIPRICO PARAMETER BLOCK *****************/

parmblk[cmnd_ident]->cmd_id = cmnd_ident;
parmblk[cmnd_ident]->resvd = 0x00;
parmblk[cmnd_ident]->flags = sgflg; /*Set for scatter/gather or single block*/
parmblk[cmnd_ident]->addr_mod = 0x0D;
parmblk[cmnd_ident]->targ_id = drv_num;
parmblk[cmnd_ident]->vme_addr = (unsigned int)data_addr;
parmblk[cmnd_ident]->xfer_count = 0;
parmblk[cmnd_ident]->scsi_blk[0]=TP_RD;
parmblk[cmnd_ident]->scsi_blk[1]=op1;
parmblk[cmnd_ident]->scsi_blk[2]=op2;
parmblk[cmnd_ident]->scsi_blk[3]=op3;
parmblk[cmnd_ident]->scsi_blk[4]=op4;
parmblk[cmnd_ident]->scsi_blk[5]=op5;

/******************* LOAD CIPRICO ADDRESS BUFFER *******************/

i = (int)parmblk[cmnd_ident];
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
	    puts("READ CHAN ATTN TIMEOUT");
	    printf("STATUS = %X\n",*status);
	    exit(0);
	}
  }
/******************* IS COMMAND COMPLETE?? ************************/
i=0;
while((parmblk[cmnd_ident]->stat_flags&0x80)!=0x80)  /* Watch for command complete */
  {                                       /* flag showing command was   */
      i++;                                /* successfully completed     */
      if(i>=5000)                       /* If not drop out and show   */
	{ 
	    puts("TIME EXCEEDED FOR READ");    /* error status.              */
	    break;
	}
     taskDelay(2);                      /* Give us a little delay */
  }
switch(parmblk[cmnd_ident]->stat_flags&0xFE)
  {
    case 0xC2:
      printf("READ COMMAND COMPLETE WITH SOFT ERROR\n");
      stat=1;
      break;
    case 0xC0:
      /*printf("READ COMMAND COMPLETE WITH ERROR\n");*/
      stat=1;
      break;
    case 0xD0:
      printf("READ COMMAND COMPLETE WITH TRUNCATED DATA\n");
      stat=1;
      break;
    case 0xE0:
      printf("READ COMMAND COMPLETE BUT REQUIRED RETRIES\n");
      stat=1;
      break;
    case 0x50:
      printf("READ COMMAND NOT COMPLETE DUE TO TRUNCATED DATA\n");
      stat=1;
      break;
    case 0x60:
      printf("READ COMMAND NOT COMPLETE DUE TO RETRIES\n");
      stat=1;
      break;
    case 0x44:
      printf("READ COMMAND NOT COMPLETE DUE TO ERROR\n");
      stat=1;
      break;
    case 0x80:
      stat=0;
      break;
  }
parmblk[cmnd_ident]->stat_flags=0x00; /* Clear status flag */
return(stat);
}
