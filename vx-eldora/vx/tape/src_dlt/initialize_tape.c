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
 * Revision 1.2  1996/09/03  16:40:06  craig
 * added scatter gather buffers
 *
 * Revision 1.1  1996/06/18  16:07:37  craig
 * Initial revision
 *
 *
 * description:  Calculates size of the tape control area needed.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <tapeDef.h>
#include <tapeGbl.h>

/******************** INITIALIZE TAPE CONTROL AREA **********************/

void initialize_tape(int start_address, int sg_or_blocked)
{
int i, curr_address, j;

curr_address = start_address; 
if(((int)(curr_address / 4) * 4) != curr_address)
  curr_address = ((int)(curr_address / 4) + 1) * 4;

for(i=0; i<LAST_CIP_CMD + 1; i++)
  {
      parmblk[i] = (struct param_block *)(curr_address);
      curr_address += sizeof(struct param_block);
      if(((int)(curr_address / 4) * 4) != curr_address)
	curr_address = ((int)(curr_address / 4) + 1) * 4;
  }

mod_sen = (struct mode_struct *)curr_address;
printf("Starting address of the mode sense structure is: %8x\n",mod_sen);
curr_address += sizeof(struct mode_struct);
if(((int)(curr_address / 4) * 4) != curr_address)
  curr_address = ((int)(curr_address / 4) + 1) * 4;

rqst_sen = (struct request_sense *)curr_address;
curr_address += sizeof(struct request_sense);
if(((int)(curr_address / 4) * 4) != curr_address)
  curr_address = ((int)(curr_address / 4) + 1) * 4;

log_page = (struct log_pg *)curr_address;
curr_address += sizeof(struct log_pg);
if(((int)(curr_address / 4) * 4) != curr_address)
  curr_address = ((int)(curr_address / 4) + 1) * 4;

suprt_pgs = (struct supported_pgs *)curr_address;
curr_address += sizeof(struct supported_pgs);
if(((int)(curr_address / 4) * 4) != curr_address)
  curr_address = ((int)(curr_address / 4) + 1) * 4;

mod_sel = (struct mode_struct *)curr_address;
printf("Starting address of the mode select structure is: %8x\n",mod_sel);
curr_address += sizeof(struct mode_struct);
if(((int)(curr_address / 4) * 4) != curr_address)
  curr_address = ((int)(curr_address / 4) + 1) * 4;

if(sg_or_blocked == SCATTER_GATHER)
  {
      for(j=0; j<NUM_SG_STRUCTS; j++)
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

printf("Ending address of tape control area: %8x\n",curr_address);

/* now initialize every thing */

parm_blk_init();
md_sel_init();
rqst_sns_init();
md_sns_init();

if(sg_or_blocked == SCATTER_GATHER)
  {
    for(i=0; i<NUM_SG_STRUCTS; i++)
      sg_init(i);
  }

cip_cmds(GEN_OPS,GOPS,scsi_id[0],STD_AM);
cip_cmds(UNIT_OPS,UOPS,scsi_id[0],STD_AM);

/* Set write completion condition to "good" so first write does not
   complain */

parmblk[TAPE_WRITE]->stat_flags=0x08;    /* Good write status flag */


return;

}
