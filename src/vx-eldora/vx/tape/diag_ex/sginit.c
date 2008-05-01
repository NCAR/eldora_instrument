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
 * Revision 1.3  1992/10/25  17:08:12  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/17  21:32:58  reif
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  17:50:21  reif
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <cipincl.h>

/**************** INITIALIZE SCATTER/GATHER BLOCKS **************/

void sg_init()
{
register int i;
 for(i=0; i<10; i++)
   {
       sg[i].next_sg_addr=0x00000000;
       sg[i].am_and_dat_len1=0x00000000;
       sg[i].dat_addr1=0x00000000;
       sg[i].am_and_dat_len2=0x00000000;
       sg[i].dat_addr2=0x00000000;
       sg[i].am_and_dat_len3=0x00000000;
       sg[i].dat_addr3=0x00000000;
       sg[i].am_and_dat_len4=0x00000000;
       sg[i].dat_addr4=0x00000000;
       sg[i].am_and_dat_len5=0x00000000;
       sg[i].dat_addr5=0x00000000;
       sg[i].am_and_dat_len6=0x00000000;
       sg[i].dat_addr6=0x00000000;
       sg[i].am_and_dat_len7=0x00000000;
       sg[i].dat_addr7=0x00000000;
       sg[i].am_and_dat_len8=0x00000000;
       sg[i].dat_addr8=0x00000000;
   }
}
