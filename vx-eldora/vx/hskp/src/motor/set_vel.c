
/*

 *	$Id$
 *
 *	Module:	vel.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/14  21:34:33  reif
 * Initial revision
 *
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

void set_vel(float rpm)
{
unsigned int vq,n,t;
int i;

union
  {
      unsigned short velocity;
      unsigned char vel[2];
  }cmnd;

*timer=((2000000*PERIOD)-1)/16;
taskDelay(1);
*gain=0.5+(GAIN*((12.8*60)/(PERIOD*COUNTS))); /* FOR ROTODOME */
taskDelay(1);
cmnd.velocity=(COUNTS*rpm*PERIOD*0.01667)*16;
*vel_msb=cmnd.vel[0];
taskDelay(1);
*vel_lsb=cmnd.vel[1];
taskDelay(1);
*flag=0x0B;
return;
}
