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
 * Revision 1.1  1994/01/06  21:31:51  craig
 * Initial revision
 *
 * Revision 1.4  1992/10/25  17:09:25  reif
 * *** empty log message ***
 *
 * Revision 1.3  1992/10/17  21:34:29  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/15  17:52:59  reif
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  17:52:32  reif
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>
/************** CALCULATE REAL LOGICAL LENGTH *********************/

unsigned int nav_length(unsigned int *nav_addr)
{

NAV_INFO *nav_inform;
INS_DATA *ins;
MINIRIMS_DATA *minirims;
GPS_DATA *gps;
NAVDESC *nav_dscr;
unsigned int nav_length;

nav_dscr=GetNavDesc(Hdr);
(unsigned int)nav_inform=*nav_addr;
nav_length=nav_inform->nav_info_len;
if(nav_dscr->ins_flag==1)
  {
      (unsigned int)ins=*nav_addr+sizeof(NAV_INFO);
      nav_length+=ins->ins_data_len;
  }
if(nav_dscr->gps_flag==1)
  {
      (unsigned int)gps=*nav_addr+sizeof(NAV_INFO)+sizeof(INS_DATA);
      nav_length+=gps->gps_data_len;
  }
if(nav_dscr->minirims_flag==1)
  {
      (unsigned int)minirims=*nav_addr+sizeof(NAV_INFO)+sizeof(INS_DATA)+sizeof(GPS_DATA);
      nav_length+=minirims->minirims_data_len;
  }
if(nav_dscr->kalman_flag==1)
  printf("There is no kalman filtering at this time\n");
return(nav_length);
}
