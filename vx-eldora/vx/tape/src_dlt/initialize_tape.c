/*
 *	$Id$
 *
 *	Module: initialize_tape		 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:  Calculates size of the tape control area needed.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <tapeDef.h>
#include <tapeGbl.h>

/******************** CHANNEL ATTENTION **********************/

void initialize_tape(int start_address, int sg_or_blocked)
{
int i, curr_address, j;

curr_address = start_address; 
if(((int)(curr_address / 4) * 4) != curr_address)
  curr_address = ((int)(curr_address / 4) + 1) * 4;

for(i=0; i<LAST_PARAM_BLOCK + 1; i++)
  {
      parmblk[i] = (struct param_block *)(curr_address);
      curr_address += sizeof(struct param_block);
      if(((int)(curr_address / 4) * 4) != curr_address)
	curr_address = ((int)(curr_address / 4) + 1) * 4;
  }
mod_sel = (struct mode_select *)curr_address;
curr_address += sizeof(struct mode_select);
if(((int)(curr_address / 4) * 4) != curr_address)
  curr_address = ((int)(curr_address / 4) + 1) * 4;

mod_sen = (struct mode_sense *)curr_address;
curr_address += sizeof(struct mode_sense);
if(((int)(curr_address / 4) * 4) != curr_address)
  curr_address = ((int)(curr_address / 4) + 1) * 4;

rqst_sen = (struct request_sense *)curr_address;
curr_address += sizeof(struct request_sense);
if(((int)(curr_address / 4) * 4) != curr_address)
  curr_address = ((int)(curr_address / 4) + 1) * 4;


if(sg_or_blocked == SCATTER_GATHER)
  {
      for(j=0; j<2; j++)
	{
	    for(i=0; i<MAX_SG_BLK + 2; i++)
	      {
		  sg[j][i] = (struct scatter_gather *)curr_address;
		  curr_address += sizeof(struct scatter_gather);
		  if(((int)(curr_address / 4) * 4) != curr_address)
		    curr_address = ((int)(curr_address / 4) + 1) * 4;
	      }
	}
  }

/* now initialize every thing */

parm_blk_init();
md_sel_init();
rqst_sns_init();
md_sns_init();
if(sg_or_blocked == SCATTER_GATHER)
  {
      sg_init(0);
      sg_init(1);
  }

cip_cmds(GEN_OPS,GOPS,scsi_id[0]);
cip_cmds(UNIT_OPS,UOPS,scsi_id[0]);

/* Set write completion condition to "good" so first write does not
   complain */

parmblk[TAPE_WRITE]->stat_flags=0x08;    /* Good write status flag */


return;

}
