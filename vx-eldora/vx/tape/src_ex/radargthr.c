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
 * Revision 1.1  1994/01/06  21:31:37  craig
 * Initial revision
 *
 * Revision 1.1  1992/08/14  17:50:24  reif
 * Initial revision
 *
 *
 * description:  Copies the temporary length and address arrays into the
 *               radar scatter-gather structure.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>

void rad_sctr_gthr(int rad_sg_index,int done)
{
unsigned int *rad_sg_ptr;
unsigned int length;

rad_sg_ptr=(unsigned int *)&rad_sg[rad_sg_index+1];

/***************** LOAD RAD SCATTER/GATHER BLOCK ********************/

      if(rad_sg_index < MAX_RAD_DSCR_BLK && !done)
	{
	    rad_sg[rad_sg_index].next_rad_sg_addr=(unsigned int)rad_sg_ptr;
	}
      else
	{
	    rad_sg[rad_sg_index].next_rad_sg_addr=0xFFFFFFFF;
	}
      rad_sg[rad_sg_index].am_and_dat_len1=rad_xfer_len[0];
      rad_sg[rad_sg_index].dat_addr1=rad_xfer_addr[0];
      rad_sg[rad_sg_index].am_and_dat_len2=rad_xfer_len[1];
      rad_sg[rad_sg_index].dat_addr2=rad_xfer_addr[1];
      rad_sg[rad_sg_index].am_and_dat_len3=rad_xfer_len[2];
      rad_sg[rad_sg_index].dat_addr3=rad_xfer_addr[2];
      rad_sg[rad_sg_index].am_and_dat_len4=rad_xfer_len[3];
      rad_sg[rad_sg_index].dat_addr4=rad_xfer_addr[3];
      rad_sg[rad_sg_index].am_and_dat_len5=rad_xfer_len[4];
      rad_sg[rad_sg_index].dat_addr5=rad_xfer_addr[4];
      rad_sg[rad_sg_index].am_and_dat_len6=rad_xfer_len[5];
      rad_sg[rad_sg_index].dat_addr6=rad_xfer_addr[5];
      rad_sg[rad_sg_index].am_and_dat_len7=rad_xfer_len[6];
      rad_sg[rad_sg_index].dat_addr7=rad_xfer_addr[6];
      rad_sg[rad_sg_index].am_and_dat_len8=rad_xfer_len[7];
      rad_sg[rad_sg_index].dat_addr8=rad_xfer_addr[7];
}
