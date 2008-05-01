/*
 *	$Id$
 *
 *	Module:	cip_cmds
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1996/11/15  22:06:14  craig
 * *** empty log message ***
 *
 * Revision 1.2  1996/09/03  16:39:11  craig
 * cleaned up
 *
 * Revision 1.1  1996/06/18  16:02:19  craig
 * Initial revision
 *
 *
 * description:  This modules sends commands that are specific to the
 *               Ciprico board itself
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <tapeDef.h>
#include <tapeGbl.h>

/****************** CIPRICO COMMANDS ************************/

void cip_cmds(unsigned int cmnd_ident,unsigned char op_cd,unsigned char drv_num,int control_am)
{
unsigned short *addr_buff, cb_am;
volatile unsigned short *status;
unsigned short new_stat,old_stat;
unsigned char resvd,flags,addr_mod,targ_id,vme_addr,xfer_count,test;
int i,stat;
union 
  {
    PARMBLK *pb;
    unsigned short pb_addr[2];
  }addr;
 
addr_buff=(unsigned short *)(CIP_BASE+SHORT_BASE+CIP_ADDR_BUFF);
status=(unsigned short *)(CIP_BASE+SHORT_BASE+CIP_BD_STAT);
old_stat=*status;
new_stat=*status;
cb_am = control_am | CONTROL_BYTE;

/************ LOAD CIPRICO PARAMETER BLOCK *****************/

if(cmnd_ident<0 || cmnd_ident>LAST_CIP_CMD)
  {
      printf("Unknown Ciprico board command given\n");
      return;
  }
switch(cmnd_ident)
  {
    case GEN_OPS:
      resvd=GOP_OP_FLAGS;
      flags=GOP_THROTTLE;
      addr_mod=GOP_OWN_ID;
      targ_id=GOP_TID;
      vme_addr=GOP_RESV;
      xfer_count=GOP_RESV;
      break;
    case UNIT_OPS:
      resvd=UOP_DTO1;
      flags=UOP_DTO2;
      addr_mod=drv_num;
      targ_id=UOP_TID;
      vme_addr=UOP_VME;
      xfer_count=UOP_CNT;
      break;
    default:
      puts("UNKNOWN COMMAND");
      break;
  }

/******************* SETUP CIPRICO PARAMETER BLOCK *****************/

parmblk[cmnd_ident]->cmd_id = cmnd_ident;
parmblk[cmnd_ident]->resvd = resvd;
parmblk[cmnd_ident]->flags = flags;
parmblk[cmnd_ident]->addr_mod = addr_mod;
parmblk[cmnd_ident]->targ_id = targ_id;
parmblk[cmnd_ident]->vme_addr =vme_addr;
parmblk[cmnd_ident]->xfer_count = xfer_count;
parmblk[cmnd_ident]->scsi_blk[0] = op_cd;

/******************* LOAD CIPRICO ADDRESS BUFFER *******************/

i = (int)parmblk[cmnd_ident];
i += tape_vme_offset;
addr.pb= (PARMBLK *)i;
*addr_buff = cb_am; /* Control Byte and Address Modifier */
*addr_buff=addr.pb_addr[0]; /* MSW OF paramblk addr */
*addr_buff=addr.pb_addr[1]; /* LSW of paramblk addr */ 
chan_attn(0); /* Issue channel attention 0 for sending single commands */

/********** IS CIPRICO READY FOR ANOTHER COMMAND?? ****************/

stat = 0;
i=0; /* INIT LOOP COUNTER */
while(old_stat==new_stat) /* Watch for ENT bit to toggle */
  {                       /* to show if board is ready for */
      new_stat=*status;   /* another command. */             
      i++;                     
      if(i==5000)
	{
	    puts("CIP_CMDS: CHAN ATTN TIMEOUT");
	    printf("STATUS = %X\n",*status);
	    stat = 1;
	    break;
	}
  }

/******************* IS COMMAND COMPLETE?? ************************/
/* Watch for command complete bit, if set drop out, if it doesn't
   happen for awhile then timeout */

i=0;
test = 0;
while(test != 0x80)
  {
    test = parmblk[cmnd_ident]->stat_flags & 0x80;
    i++;
    if(i >= 50000)
      {
	    stat=1;                       
	    printf("CIP_CMDS: COMAND TIMEOUT\n");
	    break;
	}
      taskDelay(1);
  }

if(stat == 1) print_stat(stat,cmnd_ident);

parmblk[cmnd_ident]->stat_flags=0x00; /* Clear status flag */
return;
}






