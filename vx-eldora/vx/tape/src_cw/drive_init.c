/*
 *	$Id$
 *
 *	Module:drive_init.c		 
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1993/09/22  17:03:38  reif
 * Initial revision
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>

int drive_init(char drv_num)
{
int starting_amnt;
unsigned char drv_stat=0;
int error[4];
int number_ints=0;
int i;

drv_stat=tst_unt_rdy(drv_num);  /* SEE IF EXABYTE IS READY */
while(drv_stat!=0x80)
  {
      drv_stat=tst_unt_rdy(drv_num);  /* SEE IF EXABYTE IS READY */
      loggerEvent("ERROR= %X ON ",&drv_stat,1);
      loggerEvent("DRV%d\n",&drv_num,1);
      taskDelay(1000);
  }
exb_cmds(MODE_SELECT,MD_SEL,drv_num);  /* CONFIGURE EXABYTE DRIVEs */
starting_amnt=start_amnt(drv_num);
return(starting_amnt);
}








