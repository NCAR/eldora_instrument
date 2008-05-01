
/*

 *	$Id$
 *
 *	Module:	just_set_vel.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1993/08/10  19:58:32  craig
 * Initial revision
 *
 * Revision 1.1  1992/09/01  20:47:20  craig
 * Initial revision
 *
 * description:  This module sets the velocity in the motor control card
 *               as given by the calling routine.  This routine does not
 *               stop or start the motor.  This routine should be used
 *               only if the motor has been started and is running an 
 *               already requested velocity.
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

void just_set_vel(float cmrpm)
{
union
  {
      unsigned short velocity;
      unsigned char vel[2];
  }cmnd;

cmnd.velocity = (cntrlCounts*cmrpm*cntrlPeriod*0.01667)*16;

*vel_msb=cmnd.vel[0];

*vel_lsb=cmnd.vel[1];

return;
}
