/*

 *	$Id$
 *
 *	Module:	motor.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:
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
unsigned char *act_vel_lsb,*act_vel_msb;
float rpm;
union
  {
      short velocity;
      unsigned char vel[2];
  }actual;

act_vel_lsb=(unsigned char *)ACTUAL_VELOCITY_LSB;
act_vel_msb=(unsigned char *)ACTUAL_VELOCITY_MSB;

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

