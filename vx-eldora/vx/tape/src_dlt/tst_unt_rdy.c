/*
 *	$Id$
 *
 *	Module:	tst_unt_rdy
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1996/11/15  22:06:22  craig
 * *** empty log message ***
 *
 * Revision 1.1  1996/06/18  16:02:54  craig
 * Initial revision
 *
 *
 *
 * description:  Tests to see if the given SCSI unit number is ready 
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <tapeDef.h>
#include <tapeGbl.h>

/********************* TEST UNIT READY **************************/

char tst_unt_rdy(unsigned char drv_num,int control_am)
{
unsigned short *addr_buff;
volatile unsigned short *status;
unsigned short new_stat,old_stat;
int i;
char drv_stat;

union 
  {
     PARMBLK *pb;
     unsigned short pb_addr[2];
  }addr;

addr_buff=(unsigned short *)(CIP_BASE+SHORT_BASE+CIP_ADDR_BUFF);
status=(unsigned short *)(CIP_BASE+SHORT_BASE+CIP_BD_STAT);
old_stat=*status;
new_stat=*status;

/*************** CIPRICO PARAMETER BLOCK ******************/

parmblk[TEST_UNIT_READY]->cmd_id=TEST_UNIT_READY;
parmblk[TEST_UNIT_READY]->resvd=0x00; 
parmblk[TEST_UNIT_READY]->flags=0x00; 
parmblk[TEST_UNIT_READY]->addr_mod= control_am; 
parmblk[TEST_UNIT_READY]->targ_id=drv_num; 
parmblk[TEST_UNIT_READY]->vme_addr=0x00000000;
parmblk[TEST_UNIT_READY]->xfer_count=0x00000000;
parmblk[TEST_UNIT_READY]->scsi_blk[0]=TPUR;
parmblk[TEST_UNIT_READY]->scsi_blk[1]=0x00;
parmblk[TEST_UNIT_READY]->scsi_blk[2]=0x00;
parmblk[TEST_UNIT_READY]->scsi_blk[3]=0x00;
parmblk[TEST_UNIT_READY]->scsi_blk[4]=0x00;
parmblk[TEST_UNIT_READY]->scsi_blk[5]=0x00;

/**********************************************************/

i = parmblk[TEST_UNIT_READY];
i += tape_vme_offset;
addr.pb= (PARMBLK *)i;
*addr_buff = control_am | CONTROL_BYTE; /* Control Byte and Address Modifier */
*addr_buff=addr.pb_addr[0]; /* MSW OF paramblk addr */
*addr_buff=addr.pb_addr[1]; /* LSW of paramblk addr */ 
chan_attn(0); /* Issue channel attention 0 */

i=0;
while(old_stat==new_stat) /* Watch for ENT bit to toggle */
  {                       /* to show if board is ready for */
      new_stat=*status;   /* another command. */             
      i++;
      if(i==500000)
	{
	    puts("CHAN ATTN TIMEOUT");
	    taskDelay(10);
	    printf("STATUS = %X\n",*status);
	    break;
	}
  }
while(parmblk[TEST_UNIT_READY]->stat_flags==0x00);
drv_stat=parmblk[TEST_UNIT_READY]->stat_flags;
parmblk[TEST_UNIT_READY]->stat_flags=0x00; /* Clear status flags */
return(drv_stat);
}



