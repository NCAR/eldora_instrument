/*

 *	$Id$
 *
 *	Module:	read_rpm.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/01  20:45:19  craig
 * Initial revision
 *
 *
 * description: The purpose of this module is to read the actual velocity from
 *              the HCTL1100 chip and convert it to an rpm value that is
 *              returned to the calling program.
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

float read_rpm()
{
float rpm;
union
  {
      short velocity;
      unsigned char vel[2];
  }actual;


actual.vel[0] = *act_vel_msb;    /* Read the most significant byte */
taskDelay(1);
actual.vel[1] = *act_vel_lsb;      /* Read the least significant byte */
taskDelay(1);

rpm = actual.velocity / (PERIOD * COUNTS * 0.01667);

return(rpm);
}

