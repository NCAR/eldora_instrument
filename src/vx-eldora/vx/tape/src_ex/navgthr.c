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
 * Revision 1.1  1994/01/06  21:31:38  craig
 * Initial revision
 *
 * Revision 1.1  1992/08/14  17:50:25  reif
 * Initial revision
 *
 *
 * description:  Copies the temporary navigational data address and length
 *               arrays into the navigational data scatter-gather structure.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>

void nav_sctr_gthr(int nav_sg_index,int done)
{
unsigned int *nav_sg_ptr;
unsigned int length;

nav_sg_ptr=(unsigned int *)&nav_sg[nav_sg_index+1];

/***************** LOAD NAV SCATTER/GATHER BLOCK ********************/

      if(nav_sg_index < MAX_NAV_DSCR_BLK && !done)
	{
	    nav_sg[nav_sg_index].next_nav_sg_addr=(unsigned int)nav_sg_ptr;
	}
      else
	{
	    nav_sg[nav_sg_index].next_nav_sg_addr=0xFFFFFFFF;
	}
      nav_sg[nav_sg_index].am_and_dat_len1=nav_xfer_len[0];
      nav_sg[nav_sg_index].dat_addr1=nav_xfer_addr[0];
      nav_sg[nav_sg_index].am_and_dat_len2=nav_xfer_len[1];
      nav_sg[nav_sg_index].dat_addr2=nav_xfer_addr[1];
      nav_sg[nav_sg_index].am_and_dat_len3=nav_xfer_len[2];
      nav_sg[nav_sg_index].dat_addr3=nav_xfer_addr[2];
      nav_sg[nav_sg_index].am_and_dat_len4=nav_xfer_len[3];
      nav_sg[nav_sg_index].dat_addr4=nav_xfer_addr[3];
      nav_sg[nav_sg_index].am_and_dat_len5=nav_xfer_len[4];
      nav_sg[nav_sg_index].dat_addr5=nav_xfer_addr[4];
      nav_sg[nav_sg_index].am_and_dat_len6=nav_xfer_len[5];
      nav_sg[nav_sg_index].dat_addr6=nav_xfer_addr[5];
      nav_sg[nav_sg_index].am_and_dat_len7=nav_xfer_len[6];
      nav_sg[nav_sg_index].dat_addr7=nav_xfer_addr[6];
      nav_sg[nav_sg_index].am_and_dat_len8=nav_xfer_len[7];
      nav_sg[nav_sg_index].dat_addr8=nav_xfer_addr[7];
}
