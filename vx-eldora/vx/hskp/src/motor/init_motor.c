/*
 *
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
 * Revision 1.1  1992/08/14  21:34:28  reif
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


void init_motor()
{
gain=(unsigned char *)GAIN_K;
flag=(unsigned char *)FLAG_REGISTER;
status=(unsigned char *)STATUS_REGISTER;
prog_cntr=(unsigned char *)PROGRAM_COUNTER;
vel_lsb=(unsigned char *)CMND_VEL_PROP_LSB;
vel_msb=(unsigned char *)CMND_VEL_PROP_MSB;
timer=(unsigned char *)SAMPLE_TIMER;
cmnd=(unsigned char *)EIGHT_BIT_MOTOR_COMMAND;
reset=(unsigned char *)RESET_1100;
ltch=(unsigned char *)CONTROL_LATCH;
pin_stat=(unsigned char *)STATUS_1100;
rd2016=(unsigned char *)READ_2016;
act_vel_lsb=(unsigned char *)ACTUAL_VELOCITY_LSB;
act_vel_msb=(unsigned char *)ACTUAL_VELOCITY_MSB;

*ltch=0x06; /* Latch 1100 reset */
taskDelay(1);
*ltch=0x0E; /* Release 1100 reset */

return;
}
