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
 * Revision 1.3  1996/11/15  22:06:44  craig
 * *** empty log message ***
 *
 * Revision 1.2  1996/09/03  16:40:30  craig
 * added more buffers
 *
 * Revision 1.1  1996/06/18  16:03:28  craig
 * Initial revision
 *
 *
 *
 * description:  Fills the scattter gather blocks with addresses and lengths
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <tapeDef.h>
#include <tapeGbl.h>

int fill_sg(unsigned int *data_add,int length, int sg_buff, int data_am)
{
char stat;
int i;

/* fill current position in the scatter gather structure */

sg[sg_buff][sg_blk_cnt[sg_buff]]->len_add[sg_add_cnt[sg_buff]].am_and_length = 
  data_am * 0x01000000 + length;

sg[sg_buff][sg_blk_cnt[sg_buff]]->len_add[sg_add_cnt[sg_buff]].address =
  (unsigned int)data_add;

/* See if current block is full */

sg_add_cnt[sg_buff] += 1;
if(sg_add_cnt[sg_buff] >= 8)
  {

      sg_add_cnt[sg_buff] = 0;
      
      /* See if number of blocks has reached maximum */
      
      sg_blk_cnt[sg_buff] += 1;
      if(sg_blk_cnt[sg_buff] < MAX_SG_BLK)
	    sg[sg_buff][sg_blk_cnt[sg_buff] - 1]->next_sg_blk_add = 
	      (unsigned int)sg[sg_buff][sg_blk_cnt[sg_buff]];
      else
	return(1);
  }
return(0);
} 









