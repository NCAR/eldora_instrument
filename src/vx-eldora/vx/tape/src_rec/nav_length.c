/*
 *	$Id$
 *
 *	Module:	nav_length	 
 *	Original Author: craig walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1996/06/24 23:00:04  craig
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <recordIncs.h>
#include <tapeDef.h>
#include <tapeGbl.h>

/************** CALCULATE LENGTH OF NAVIGATION RECORD *********************/

unsigned int nav_length(unsigned int *nav_addr)
{

NAV_INFO *nav_inform;
INS_DATA *ins;
MINIRIMS_DATA *minirims;
GPS_DATA *gps;
NAVDESC *nav_dscr;
unsigned int nav_len;

nav_dscr=GetNavDesc(inHeader);
(unsigned int)nav_inform = *nav_addr;

nav_len = nav_inform->nav_info_len;

if(nav_dscr->ins_flag==1)
  {
      (unsigned int)ins=*nav_addr+sizeof(NAV_INFO);
      nav_len+=ins->ins_data_len;
  }

if(nav_dscr->gps_flag==1)
  {
      (unsigned int)gps=*nav_addr+sizeof(NAV_INFO)+sizeof(INS_DATA);
      nav_len+=gps->gps_data_len;
  }

if(nav_dscr->minirims_flag==1)
  {
      (unsigned int)minirims=*nav_addr+sizeof(NAV_INFO)+sizeof(INS_DATA)+sizeof(GPS_DATA);
      nav_len+=minirims->minirims_data_len;
  }

if(nav_dscr->kalman_flag==1)
  printf("There is no kalman filtering at this time\n");
return(nav_len);

}
