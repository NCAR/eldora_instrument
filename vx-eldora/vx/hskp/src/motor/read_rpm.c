/*

 *	$Id$
 *
 *	Module:	rpm.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/14  21:34:32  reif
 * Initial revision
 *
 *
 * description: The purpose of this module is to read the actual velocity from
 *              the HCTL1100 chip and convert it to an rpm value and display it.
 *             
 *             
 *              
 *              
 *             
 *             
 *             
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <cntrlIncl.h>

void read_rpm()
{
float rpm;
union
  {
      short velocity;
      unsigned char vel[2];
  }actual;


for(;;)
  {
actual.vel[0]=*act_vel_msb;
taskDelay(1);
actual.vel[1]=*act_vel_lsb;
taskDelay(1);
rpm=actual.velocity/(PERIOD*COUNTS*0.01667);
printf("ACTUAL VELOCITY = %4.2f RPM  %4d COUNTS\r",rpm,actual.velocity);
taskDelay(10);
}
return;
}

