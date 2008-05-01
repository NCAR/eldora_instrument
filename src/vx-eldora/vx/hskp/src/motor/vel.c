
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
unsigned char *timer;
unsigned char *vel_lsb,*vel_msb;
unsigned char *prog_cntr;
unsigned char *status;
unsigned char *flag;
unsigned char *gain;
int i;

union
  {
      unsigned short velocity;
      unsigned char vel[2];
  }cmnd;

gain=(unsigned char *)GAIN_K;
flag=(unsigned char *)FLAG_REGISTER;
status=(unsigned char *)STATUS_REGISTER;
prog_cntr=(unsigned char *)PROGRAM_COUNTER;
vel_lsb=(unsigned char *)CMND_VEL_PROP_LSB;
vel_msb=(unsigned char *)CMND_VEL_PROP_MSB;
printf("RPM= %2.2f\n",rpm);
timer=(unsigned char *)SAMPLE_TIMER;
*timer=((2000000*PERIOD)-1)/16;
taskDelay(1);
printf("TIMER= %X\n",*timer);
taskDelay(1);
*gain=0.5+(GAIN*((12.8*60)/(PERIOD*COUNTS))); /* FOR ROTODOME */
taskDelay(1);
printf("GAIN= %X\n",*gain);
cmnd.velocity=(COUNTS*rpm*PERIOD*0.01667)*16;
printf("COMMAND VELOCITY= %x\n",cmnd.velocity);
*vel_msb=cmnd.vel[0];
taskDelay(1);
printf("VEL MSB= %X\n",*vel_msb);
*vel_lsb=cmnd.vel[1];
taskDelay(1);
printf("VEL LSB= %X\n",*vel_lsb);
taskDelay(1);
*flag=0x0B;
taskDelay(1);
printf("FLAG REG= %X\n",*flag&0x0F);
taskDelay(1);
printf("STATUS= %X\n",*status);
return;
}
