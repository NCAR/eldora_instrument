/*
 *	$Id$
 *
 *	Module: drive_init.c		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:  Waits for drive to come ready, then sets it mode select
 *               parameters.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <tapeDef.h>
#include <tapeGbl.h>

void drive_init(unsigned char drv_num)
{

unsigned char drv_stat;

drv_stat = tst_unt_rdy(drv_num);  /* SEE IF DLT IS READY */

while(drv_stat != 0x80)
  {
      drv_stat=tst_unt_rdy(drv_num);  /* SEE IF DLT IS READY */
      printf("Waiting for SCSI Drive %2d to come ready\n",drv_num);
      taskDelay(100);
  }

dlt_cmds(MODE_SELECT,drv_num);  /* CONFIGURE DLT DRIVEs */

return;
}








