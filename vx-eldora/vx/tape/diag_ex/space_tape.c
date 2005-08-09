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
 * Revision 1.5  1992/10/30  23:54:37  reif
 * *** empty log message ***
 *
 * Revision 1.4  1992/10/25  17:07:56  reif
 * *** empty log message ***
 *
 * Revision 1.3  1992/10/17  21:32:03  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/15  17:52:33  reif
 * *** empty log message ***
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <cipincl.h>
/********************* SPACE **************************/

void space_tape(long blocks,char code,char eod,char drv_num)
{
unsigned short *addr_buff;
volatile unsigned short *status;
unsigned short new_stat,old_stat;
int i,stat,j;

union
  {
      long count;
      char cnt_array[4];
  }cnt;

union 
  {
     PARMBLK *pb;
     unsigned short pb_addr[2];
  }addr;
 
addr_buff=(unsigned short *)(CIP_BASE+CIP_ADDR_BUFF);
status=(unsigned short *)(CIP_BASE+CIP_BD_STAT);
old_stat=*status;
new_stat=*status;
cnt.count=blocks;

/*************** CIPRICO PARAMETER BLOCK ******************/

parmblk[SPACE].cmd_id=SPACE;
parmblk[SPACE].resvd=0x00; 
parmblk[SPACE].flags=0x00; 
parmblk[SPACE].addr_mod=AM; 
parmblk[SPACE].targ_id=drv_num; 
parmblk[SPACE].vme_addr=0x00000000;
parmblk[SPACE].xfer_count=0x00000000;
parmblk[SPACE].scsi_blk[0]=SP;
parmblk[SPACE].scsi_blk[1]=code;
parmblk[SPACE].scsi_blk[2]=cnt.cnt_array[1];
parmblk[SPACE].scsi_blk[3]=cnt.cnt_array[2];
parmblk[SPACE].scsi_blk[4]=cnt.cnt_array[3];
parmblk[SPACE].scsi_blk[5]=eod;

/******************* LOAD CIPRICO ADDRESS BUFFER *******************/

addr.pb=&parmblk[SPACE];
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
	    puts("SPACE CHAN ATTN TIMEOUT");
	    printf("STATUS = %X\n",*status);
	    exit(0);
	}
  }
/******************* IS COMMAND COMPLETE?? ************************/
i=0;
while((parmblk[SPACE].stat_flags&0x80)!=0x80)  /* Watch for command complete */
  {                                       /* flag showing command was   */
      i++;                                /* successfully completed     */
      if(i>=50000000)                       /* If not drop out and show   */
	{                                 /* error status.              */
	    puts("TIME EXCEEDED");    /* error status.              */
	    break;
	}
     for(j=0; j<10; j++); /* Give us a little delay */
  }
switch(parmblk[SPACE].stat_flags&0xFE)
  {
    case 0xC2:
      printf("SPACE COMMAND COMPLETE WITH SOFT ERROR\n");
      stat=1;
      break;
    case 0xC0:
      printf("SPACE COMMAND COMPLETE WITH ERROR\n");
      stat=1;
      break;
    case 0xD0:
      printf("SPACE COMMAND COMPLETE WITH TRUNCATED DATA DRV%d\n",drv_num);
      stat=1;
      break;
    case 0xE0:
      printf("SPACE COMMAND COMPLETE BUT REQUIRED RETRIES\n");
      stat=1;
      break;
    case 0x50:
      printf("SPACE COMMAND NOT COMPLETE DUE TO TRUNCATED DATA\n");
      stat=1;
      break;
    case 0x60:
      printf("SPACE COMMAND NOT COMPLETE DUE TO RETRIES\n");
      stat=1;
      break;
    case 0x44:
      printf("SPACE COMMAND NOT COMPLETE DUE TO ERROR\n");
      stat=1;
      break;
    case 0x80:
      stat=0;
      break;
  }
parmblk[SPACE].stat_flags=0x00; /* Clear status flag */
return;
}
