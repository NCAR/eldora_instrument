/*
 *	$Id$
 *
 *	Module:drive_init.c		 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1994/11/14  17:52:23  craig
 * Modified timeout
 *
 * Revision 1.1  1994/01/06  21:31:55  craig
 * Initial revision
 *
 * Revision 1.1  1993/09/22  17:03:38  reif
 * Initial revision
 *
 * description:  Waits for drive to come ready, then sets it mode select
 *               parameters.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>

void drive_init(unsigned char drv_num)
{

unsigned char drv_stat=0;
int error[4];
int number_ints=0;

drv_stat=tst_unt_rdy(drv_num);  /* SEE IF EXABYTE IS READY */

while(drv_stat!=0x80)
  {
      drv_stat=tst_unt_rdy(drv_num);  /* SEE IF EXABYTE IS READY */
      printf("Waiting for SCSI Drive %2d to come ready\n",drv_num);
      taskDelay(100);
  }
exb_cmds(MODE_SELECT,MD_SEL,drv_num);  /* CONFIGURE EXABYTE DRIVEs */

return;
}








