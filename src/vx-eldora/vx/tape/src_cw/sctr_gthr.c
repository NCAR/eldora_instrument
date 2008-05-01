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
 * Revision 1.1  1995/03/31  22:51:52  craig
 * Initial revision
 *
 * Revision 1.3  1992/10/25  17:08:14  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/17  21:33:07  reif
 * changed to use global array
 *
 * Revision 1.1  1992/08/14  17:50:24  reif
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>

void sctr_gthr(int sg_index,int done)
{
unsigned int *sg_ptr;
unsigned int length;

sg_ptr=(unsigned int *)&sg[sg_index+1];

/***************** LOAD SCATTER/GATHER BLOCK ********************/

      if(sg_index!=9 || done!=1)
	{
	    sg[sg_index].next_sg_addr=(unsigned int)sg_ptr;
	}
      if(sg_index==9 || done==1)
	{
	    sg[sg_index].next_sg_addr=0xFFFFFFFF;
	}
      sg[sg_index].am_and_dat_len1=sg_xfer_len[0];
      sg[sg_index].dat_addr1=sg_xfer_addr[0];
      sg[sg_index].am_and_dat_len2=sg_xfer_len[1];
      sg[sg_index].dat_addr2=sg_xfer_addr[1];
      sg[sg_index].am_and_dat_len3=sg_xfer_len[2];
      sg[sg_index].dat_addr3=sg_xfer_addr[2];
      sg[sg_index].am_and_dat_len4=sg_xfer_len[3];
      sg[sg_index].dat_addr4=sg_xfer_addr[3];
      sg[sg_index].am_and_dat_len5=sg_xfer_len[4];
      sg[sg_index].dat_addr5=sg_xfer_addr[4];
      sg[sg_index].am_and_dat_len6=sg_xfer_len[5];
      sg[sg_index].dat_addr6=sg_xfer_addr[5];
      sg[sg_index].am_and_dat_len7=sg_xfer_len[6];
      sg[sg_index].dat_addr7=sg_xfer_addr[6];
      sg[sg_index].am_and_dat_len8=sg_xfer_len[7];
      sg[sg_index].dat_addr8=sg_xfer_addr[7];
}
