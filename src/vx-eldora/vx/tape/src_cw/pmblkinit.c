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
 * Revision 1.2  1995/03/31  22:49:31  craig
 * Modified for more blcoks
 *
 * Revision 1.1  1994/01/06  21:31:31  craig
 * Initial revision
 *
 * Revision 1.4  1993/09/22  15:27:34  reif
 * changed number of parameters to include send diags space etc.
 *
 * Revision 1.3  1992/10/25  17:08:05  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/17  21:32:45  reif
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  17:50:14  reif
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>

/*************** INITIALIZE PARAMETER BLOCK STRUCTURES *************/

void parm_blk_init()
{
register int i,j;

for(i=0; i<(LAST_CIP_CMD+1); i++)
  {
      parmblk[i].cmd_id=0x00000000;
      parmblk[i].resvd=0x00;
      parmblk[i].flags=0x00;
      parmblk[i].addr_mod=0x00;
      parmblk[i].targ_id=0x00;
      parmblk[i].vme_addr=0x00000000;
      parmblk[i].xfer_count=0x00000000;
      for(j=0; j<12; j++)
	parmblk[i].scsi_blk[j]=0x00;
      parmblk[i].res1=0x0000;
      parmblk[i].int_level=INT_LVL;
      parmblk[i].user_id=USER_ID;
      parmblk[i].res2=0x00000000;
      parmblk[i].status_cmd_id=0x00000000;
      parmblk[i].res3=0x00;
      parmblk[i].scsi_status=0x00;
      parmblk[i].error=0x00;
      parmblk[i].stat_flags=0x00;
      parmblk[i].class_code=0x00;
      parmblk[i].segment=0x00;
      parmblk[i].scsi_flags=0x00;
      parmblk[i].info3=0x00;
      parmblk[i].info4=0x00;
      parmblk[i].info5=0x00;
      parmblk[i].info6=0x00;
      parmblk[i].ex_length=0x00;
  }      
}
