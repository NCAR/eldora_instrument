
/*

 *	$Id$
 *
 *	Module:	set_vel.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1992/09/03  15:56:22  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/09/01  20:47:20  craig
 * Initial revision
 *
 * description:  This module sets the velocity in the motor control card
 *               as given by the calling routine.  This routine does not
 *               stop or start the motor.  If stopped the requested velocity
 *               will be attempted when the motor is started, if running
 *               this module will change the velocity at which the motor
 *               is running.
 *             
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

void set_vel(float cmrpm)
{
unsigned int vq,n,t;
int i;

union
  {
      unsigned short velocity;
      unsigned char vel[2];
  }cmnd;

*mot_timer = ((2000000 * PERIOD) -1) / 16;
taskDelay(1);

*mot_gain = 0.5 + (GAIN*((12.8*60)/(PERIOD*COUNTS))); /* FOR ROTODOME */
taskDelay(1);

cmnd.velocity = (COUNTS*cmrpm*PERIOD*0.01667)*16;

*vel_msb=cmnd.vel[0];
taskDelay(1);

*vel_lsb=cmnd.vel[1];
taskDelay(1);

*mot_flag=0x0B;
return;
}
