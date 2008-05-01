/*
 *	$Id$
 *
 *	Module:	vmevme_aisr	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/19  20:44:35  craig
 * Initial revision
 *
 *
 * description: This module answers the interrupt caused by the aft
 *              radar vme to vme interface.  This is the middle of beam
 *              interrupt. 
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

/* include fifty million .h files to deal with the header formats */
#include "Volume.h"
#include "Waveform.h"
#include "RadarDesc.h"
#include "FieldRadar.h"
#include "CellSpacing.h"
#include "Parameter.h"
#include "NavDesc.h"
#include "InSitu.h"
#include "Ray.h"
#include "Platform.h"
#include "FieldParam.h"
#include "IndFreq.h"
#include "TimeSeries.h"
#include "NavInfo.h"
#include "Ins.h"
#include "MiniRIMS.h"
#include "Gps.h"
#include "InSituData.h"

#include "Header.h"
extern HeaderPtr inHeader;

/* include the .h files that are housekeeper code specific */

#include "cntrlDef.h"
#include "cntrlFunc.h"
#include "hskpDef.h"
#include "hskpInt.h"
#include "hskpGbl.h"
#include "todDef.h"
#include "todFunc.h"
#include "ecbAdr.h"
#include "ecbErrBound.h"
#include "ecbFunc.h"
#include "ecbMaster.h"
#include "ecbSem.h"
#include "ecbStat.h"
#include "pwrDef.h"
#include "pwrGbl.h"
#include "pwrFunc.h"
#include "gpsDef.h"
#include "gpsFunc.h"
#include "minDef.h"
#include "minFunc.h"
#include "tp41vAdr.h"
#include "vmevmeDef.h"
#include "vme_hndshk.h"
#include "vmevmeAdr.h"
#include "vmevmeFunc.h"
#include "vmevmeGbl.h"

void vmevme_aisr()
{
/* Define some general purpose variables */
float position, aft_angle, elapsed_time, temp_position;
float degrees_moved;

char hr,min,sec,mon,day,yr;

short msec,jday;

long msecs_today;

/* This services the the middle of beam interrupt caused by the aft radar
   processor, first clear that interrupt. */

*aft_remote_command = 0x40; 

/* Read the Position of the rotodome */

position = get_pos();

/* Read the time from the time-of-day card */

get_time(&hr,&min,&sec,&msec,&jday,&mon,&day,&yr);

/* Clear the polled handshake words */
aft_vmehndshk->polled = 0;

/* For the time being, check to see if the last ray is ready to be recorded,
   it better be or something is wrong, then send it over */

if(aft_vmehndshk->radar_hndshk[aft_current_index] == 0)
  aft_vmehndshk->radar_hndshk[aft_current_index] = 1;

/* Create a pointer to a data ray structure at the current radar data
   ray location */

aft_current_offset += RADAR_SIZE_INCR;
aft_current_index++;
if(aft_current_index >= NUM_RADAR_HNDSHK)
  {
      aft_current_offset = FIRST_RADAR_OFFSET;
      aft_current_index = 0;
  }

aft_ray_pntr = (struct DATARAY *)(aft_current_offset + STANDARD_BASE +
                            AFT_STAND_START); 

/* Place the time read into the proper words of the data ray */

aft_ray_pntr->this_rayi.julian_day = (long)jday;
aft_ray_pntr->this_rayi.hour = (short)hr;
aft_ray_pntr->this_rayi.minute = (short)min;
aft_ray_pntr->this_rayi.second = (short)sec;
aft_ray_pntr->this_rayi.millisecond = (short)msec;
msecs_today = msec + 1000 * (sec + 60 * (min + 60 * hr));


/* Calculate the rotational position of both radar beams and place them
   into the proper word in the data ray */

aft_angle = position - ENCODER_ZERO_AFT_UP + AFT_SQUINT_ANGLE;
if(aft_angle > 360.0) aft_angle = aft_angle - 360.0;
if(aft_angle < 0.0) aft_angle = aft_angle + 360.0;

aft_ray_pntr->this_plat.rotation_angle = aft_angle;

/* Calculate the current velocity of the radar */

elapsed_time = (float)(msecs_today - last_msecs_today) / 1000.;
if(last_day != day)
  elapsed_time = (float)(msecs_today + 86400000 - last_msecs_today) / 1000.;

last_aft_msecs_today = msecs_today;
last_aft_day = day;


temp_position = position;
if(degrees_moved = position - last_position < 0.)
  degrees_moved = -1. * degrees_moved;

if(raddes->req_rotat_vel > 0.)   /* Trying to spin in a positive direction */
      if(degrees_moved > 10.) position = position + 360.;

else                            /* Trying to spin in negative direction */
  if(degrees_moved > 10.) position = position - 360.;

degrees_moved = position - last_position; 

aft_ray_pntr->this_rayi.true_scan_rate = degrees_moved/elapsed_time;

last_aft_position = temp_position;

/* Now we need to update the sweep number for
   each of the radars */

if(degrees_moved = aft_angle - last_aft_angle < 0.)
  degrees_moved = -1. * degrees_moved;
last_aft_angle = aft_angle;

if(degrees_moved > 10.) aft_sweep_num++;
aft_ray_pntr->this_rayi.sweep_num = aft_sweep_num;

}
