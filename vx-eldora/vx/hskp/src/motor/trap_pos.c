/*
 *
 *	$Id$
 *
 *	Module:	trap_pos.c	 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1992/09/03  15:56:12  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/09/01  20:42:10  craig
 * Initial revision
 *
 *
 * description:  Causes the motor to go to the position specified in the
 *               parameter sent to it. (floating point degrees)
 *             
 *             
 *             
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
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

void trap_pos(float gain,float zero,float pole,float sample_rate,unsigned char max_vel, short accel, int position)
{
unsigned char *mot_pole, *mot_zero, *pos_msb, *pos_lsb, *pos;

union
  {
      int posit;
      unsigned char pos[4];
  }cmnd;


mot_zero = (unsigned char *)(FILTER_ZERO_A);
mot_pole = (unsigned char *)(FILTER_POLE_B);
pos_msb = (unsigned char *)(FINAL_POSITION_MSB);
pos = (unsigned char *)(FINAL_POSITION_MID);
pos_lsb = (unsigned char *)(FINAL_POSITION_LSB);


*mot_timer = ((2000000 * sample_rate) -1) / 16;
taskDelay(1);

/* *mot_gain = 0.5 + (gain*((12.8*60)/(cntrlPeriod*cntrlCounts))); *//* FOR ROTO
DOME */
*mot_gain = gain;
taskDelay(1);
*mot_zero = 0.5 + zero*256;
taskDelay(1);
*mot_pole = 0.5 + pole*256;
taskDelay(1);
cmnd.posit = position;
*pos_msb = cmnd.pos[1];
taskDelay(1);
*pos = cmnd.pos[2];
taskDelay(1);
*pos_lsb = cmnd.pos[3];
taskDelay(1);
*(unsigned char *)(MAXIMUM_VELOCITY) = max_vel;
taskDelay(1);
*(unsigned char *)(ACCELERATION_MSB) = accel >> 8;
taskDelay(1);
*(unsigned char *)(ACCELERATION_LSB) = accel & 0x00FF;
taskDelay(1);
/* Set up Position Command by Clearing F3,F5 and Setting F0 */
*mot_flag = 0x8;
taskDelay(1);
*mot_flag = 0x3;
taskDelay(1);
*mot_flag = 0x5;
taskDelay(1);
}



















