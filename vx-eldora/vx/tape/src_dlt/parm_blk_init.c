/*
 *	$Id$
 *
 *	Module: parm_blk_init		 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1996/06/18  16:03:07  craig
 * Initial revision
 *
 *
 * description: Initializes all of the general purpose areas of the
 *              parameter blocks;
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <tapeDef.h>
#include <tapeGbl.h>

/*************** INITIALIZE PARAMETER BLOCK STRUCTURES *************/

void parm_blk_init()
{
register int i,j;

for(i=0; i<LAST_CIP_CMD+1; i++)
  {
      parmblk[i]->cmd_id=0x00000000;
      parmblk[i]->resvd=0x00;
      parmblk[i]->flags=0x00;
      parmblk[i]->addr_mod=0x00;
      parmblk[i]->targ_id=0x00;
      parmblk[i]->vme_addr=0x00000000;
      parmblk[i]->xfer_count=0x00000000;
      for(j=0; j<12; j++)
	parmblk[i]->scsi_blk[j]=0x00;
      parmblk[i]->res1=0x0000;
      parmblk[i]->int_level=INT_LVL;
      parmblk[i]->user_id=USER_ID;
      parmblk[i]->res2=0x00000000;
      parmblk[i]->status_cmd_id=0x00000000;
      parmblk[i]->res3=0x00;
      parmblk[i]->scsi_status=0x00;
      parmblk[i]->error=0x00;
      parmblk[i]->stat_flags=0x00;
      parmblk[i]->class_code=0x00;
      parmblk[i]->segment=0x00;
      parmblk[i]->scsi_flags=0x00;
      parmblk[i]->info3=0x00;
      parmblk[i]->info4=0x00;
      parmblk[i]->info5=0x00;
      parmblk[i]->info6=0x00;
      parmblk[i]->ex_length=0x00;
  }      
}
