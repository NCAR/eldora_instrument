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
 * Revision 1.1  1992/08/14  17:44:54  reif
 * Initial revision
 *
 *
 * description:  Copies the temporary insitu address and length arrays into
 *               the insitu scatter-gather structure.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>

void ads_sctr_gthr(int ads_sg_index,int done)
{
unsigned int *ads_sg_ptr;
unsigned int length;

ads_sg_ptr=(unsigned int *)&ads_sg[ads_sg_index+1];

/***************** LOAD ADS SCATTER/GATHER BLOCK ********************/

      if(ads_sg_index < MAX_ADS_DSCR_BLK && !done)
	{
	    ads_sg[ads_sg_index].next_ads_sg_addr=(unsigned int)ads_sg_ptr;
	}
      else
	{
	    ads_sg[ads_sg_index].next_ads_sg_addr=0xFFFFFFFF;
	}
      ads_sg[ads_sg_index].am_and_dat_len1=ads_xfer_len[0];
      ads_sg[ads_sg_index].dat_addr1=ads_xfer_addr[0];
      ads_sg[ads_sg_index].am_and_dat_len2=ads_xfer_len[1];
      ads_sg[ads_sg_index].dat_addr2=ads_xfer_addr[1];
      ads_sg[ads_sg_index].am_and_dat_len3=ads_xfer_len[2];
      ads_sg[ads_sg_index].dat_addr3=ads_xfer_addr[2];
      ads_sg[ads_sg_index].am_and_dat_len4=ads_xfer_len[3];
      ads_sg[ads_sg_index].dat_addr4=ads_xfer_addr[3];
      ads_sg[ads_sg_index].am_and_dat_len5=ads_xfer_len[4];
      ads_sg[ads_sg_index].dat_addr5=ads_xfer_addr[4];
      ads_sg[ads_sg_index].am_and_dat_len6=ads_xfer_len[5];
      ads_sg[ads_sg_index].dat_addr6=ads_xfer_addr[5];
      ads_sg[ads_sg_index].am_and_dat_len7=ads_xfer_len[6];
      ads_sg[ads_sg_index].dat_addr7=ads_xfer_addr[6];
      ads_sg[ads_sg_index].am_and_dat_len8=ads_xfer_len[7];
      ads_sg[ads_sg_index].dat_addr8=ads_xfer_addr[7];
}
