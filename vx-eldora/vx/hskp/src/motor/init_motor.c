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
 * description:  This routine initializes the motor controller pointers 
 *               and sets the gain and sample interval.             
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern

/* Include fifty million vx-works .h files */

#include "vxWorks.h"
#include "math.h"
#include "stdioLib.h"
#include "intLib.h"
#include "memLib.h"
#include "semLib.h"
#include "taskLib.h"
#include "tyLib.h"
#include "ioLib.h"
#include "in.h"
#include "systime.h"
#include "sysLib.h"

/* include the .h files that are housekeeper code specific */

#include "tp41vAdr.h"
#include "cntrlDef.h"
#include "cntrlGbl.h"
#include "cntrlFunc.h"

void init_motor()
{
mot_gain=(unsigned char *)GAIN_K;
mot_flag=(unsigned char *)FLAG_REGISTER;
mot_status=(unsigned char *)STATUS_REGISTER;
mot_prog_cntr=(unsigned char *)PROGRAM_COUNTER;
vel_lsb=(unsigned char *)CMND_VEL_PROP_LSB;
vel_msb=(unsigned char *)CMND_VEL_PROP_MSB;
mot_timer=(unsigned char *)SAMPLE_TIMER;
mot_cmnd=(unsigned char *)EIGHT_BIT_MOTOR_COMMAND;
mot_reset=(unsigned char *)RESET_1100;
mot_ltch=(unsigned char *)CONTROL_LATCH;
pin_stat=(unsigned char *)STATUS_1100;
rd2016=(unsigned char *)READ_2016;
act_vel_lsb=(unsigned char *)ACTUAL_VELOCITY_LSB;
act_vel_msb=(unsigned char *)ACTUAL_VELOCITY_MSB;

/* Setup the parameters associated with the integrator */
integrator_gain = INTEGRATOR_GAIN;
delta_degpersec = DELTA_DEGPERSEC;

*mot_ltch=0x06; /* Latch 1100 reset */
taskDelay(1);
*mot_ltch=0x0E; /* Release 1100 reset */

return;
}

