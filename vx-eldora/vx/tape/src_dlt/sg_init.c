/*
 *	$Id$
 *
 *	Module:	sg_init
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description: Initializes the scatter - gather command blocks
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <tapeDef.h>
#include <tapeGbl.h>

/**************** INITIALIZE SCATTER/GATHER BLOCKS **************/

void sg_init(int sg_buff)
{
register int i, j;

sg_blk_cnt = 0;
sg_add_cnt = 0;

for(i=0; i<MAX_SG_BLK; i++)
  {
      sg[sg_buff][i]->next_sg_blk_add=0xFFFFFFFF;
      for(j=0; j<8; j++)
	{
	    sg[sg_buff][i]->len_add[j].am_and_length = 0x00000000;
	    sg[sg_buff][i]->len_add[j].address = 0x00000000;
	}
  }
}
