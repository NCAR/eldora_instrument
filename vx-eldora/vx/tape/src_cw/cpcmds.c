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
 * Revision 1.1  1994/01/06  21:31:15  craig
 * Initial revision
 *
 * Revision 1.5  1993/09/22  15:26:47  reif
 * changed error checking
 *
 * Revision 1.4  1992/10/25  17:07:51  reif
 * *** empty log message ***
 *
 * Revision 1.3  1992/10/17  21:31:49  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/15  17:52:27  reif
 * *** empty log message ***
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>
#define scope extern

/****************** CIPRICO COMMANDS ************************/

void cip_cmds(unsigned int cmnd_ident,unsigned char op_cd,unsigned char drv_num)
{
unsigned short *addr_buff;
volatile unsigned short *status;
unsigned short new_stat,old_stat;
unsigned char resvd,flags,addr_mod,targ_id,vme_addr,xfer_count;
int i,stat;
union 
  {
    PARMBLK *pb;
    unsigned short pb_addr[2];
  }addr;
 
addr_buff=(unsigned short *)(CIP_BASE+CIP_ADDR_BUFF);
status=(unsigned short *)(CIP_BASE+CIP_BD_STAT);
old_stat=*status;
new_stat=*status;

/************ LOAD CIPRICO PARAMETER BLOCK *****************/

if(cmnd_ident<0 || cmnd_ident>LAST_CIP_CMD)
  {
      printf("WE'Re FUCKED!!\n");
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

parmblk[cmnd_ident].cmd_id = cmnd_ident;
parmblk[cmnd_ident].resvd = resvd;
parmblk[cmnd_ident].flags = flags;
parmblk[cmnd_ident].addr_mod = addr_mod;
parmblk[cmnd_ident].targ_id = targ_id;
parmblk[cmnd_ident].vme_addr =vme_addr;
parmblk[cmnd_ident].xfer_count = xfer_count;
parmblk[cmnd_ident].scsi_blk[0] = op_cd;

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
      if(i==50000)
	{
	    puts("CHAN ATTN TIMEOUT");
	    printf("STATUS = %X\n",*status);
	    break;
	}
  }
/******************* IS COMMAND COMPLETE?? ************************/
i=0;
while(parmblk[cmnd_ident].stat_flags!=0x80) /* Watch for command complete */
  {                                       /* flag showing command was   */
      i++;                                /* successfully completed     */
      if(i==50000)                        /* If not drop out and show   */
	{                                 /* error status.              */
	    stat=1;                       
	    printf("COMAND TIMEOUT\n");
	    print_stat(stat,cmnd_ident);
	    break;
	}
      taskDelay(1);
  }
stat=0;
/*print_stat(stat,cmnd_ident);*/
parmblk[cmnd_ident].stat_flags=0x00; /* Clear status flag */
return;
}
