/*
 *	$ix: fill_sg.c,v 1.1 1992/08/14 17:50:08 craig Exp craig $
 *
 *	Module:fill_sg.c		 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:  Fills the scattter gather blocks with addresses and lengths
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <tapeDef.h>
#include <tapeGbl.h>

int fill_sg(unsigned int *data_add,int length, int sg_buff)
{
char stat;
int i;

/* fill current position in the scatter gather structure */

sg[sg_buff][sg_blk_cnt]->len_add[sg_add_cnt].am_and_length = 
  AM * 0x01000000 + length;

sg[sg_buff][sg_blk_cnt]->len_add[sg_add_cnt].address = (unsigned int)data_add;

/* See if current block is full */

sg_add_cnt += 1;
if(sg_add_cnt >= 8)
  {

      sg_add_cnt = 0;
      
      /* See if number of blocks has reached maximum */
      
      sg_blk_cnt += 1;
      if(sg_blk_cnt < MAX_SG_BLK)
	    sg[sg_buff][sg_blk_cnt - 1]->next_sg_blk_add = 
	      (unsigned int)sg[sg_buff][sg_blk_cnt];
      else
	return(1);
  }
return(0);
} 









