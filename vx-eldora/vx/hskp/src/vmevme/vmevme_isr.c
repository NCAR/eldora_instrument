/*
 *	$Id$
 *
 *	Module:	vmevme_isr	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description: This module answers the interrupt caused by the fore
 *              radar vme to vme interface.  This is the middle of beam
 *              interrupt. This routine handles both the fore and aft
 *              radar processors.
 *
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "vxWorks.h"
#define scope extern
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
#include "HskpStatus.h"

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
void vmevme_isr()
{
/* Define some general purpose variables */
float position, fore_angle, aft_angle, elapsed_time, temp_position;
float degrees_moved;
static float dumb_position;

char hr,min,sec,mon,day,yr;

short msec,jday;

long msecs_today;

/* This services the the middle of beam interrupt caused by the fore radar
   processor but handles both processors, first clear the interrupts. */

*fore_remote_command = 0x40; 
*aft_remote_command = 0x40; 

/* Read the Position of the rotodome */

position = get_pos();

/* Put in a fake angle if global variable fake_angles is true */
if(fake_angles)
  {
      dumb_position += 1.2;
      if(dumb_position > 360.0) dumb_position = 0;
      position = dumb_position;
  }


/* Set global vme to vme interrupt answered flag */
in_vmevme_isr = 1;
get_time(&hr,&min,&sec,&msec,&jday,&mon,&day,&yr);
  printf("A%d",aft_vmehndshk->polled);
fore_vmehndshk->polled = 0;
aft_vmehndshk->polled = 0;

/* Check to see if a ray long enough into the past to cover any IRU
   time lags is ready to be recorded. */

fore_ray_pntr = (struct DATARAY *)(iru_lag_offset + STANDARD_BASE +
                 FORE_STAND_START);
aft_ray_pntr = (struct DATARAY *)(iru_lag_offset + STANDARD_BASE +
                 AFT_STAND_START);

if(fore_vmehndshk->radar_hndshk[iru_lag_index] == 0)
if(++radar_current_index >= NUM_RADAR_HNDSHK)
      aft_vmehndshk->radar_hndshk[iru_lag_index] = 1;
  }
else
      printf("N");

/* Now check and see if the platform info block of the ray we just marked
   to send out had data in it */

if(platform_status[iru_lag_index] == 0)
  {
      currStatus->iru |= ARINC_INT_BAD;
      fore_ray_pntr->this_rayi.ray_status = 1;
      aft_ray_pntr->this_rayi.ray_status = 1;
  }
else
  {
      platform_status[iru_lag_index] = 0;
      currStatus->iru &= (char)(~ARINC_INT_BAD);
      fore_ray_pntr->this_rayi.ray_status = 0;
      aft_ray_pntr->this_rayi.ray_status = 0;
  }


/* Update the iru lag ray pointer */

iru_lag_offset += RADAR_SIZE_INCR;
iru_lag_index++;
if(iru_lag_index >= NUM_RADAR_HNDSHK)
fore_angle = position - ENCODER_ZERO_ANGLE + 180 + FORE_SQUINT_ANGLE;
aft_angle = position - ENCODER_ZERO_ANGLE + AFT_SQUINT_ANGLE;

current_offset += RADAR_SIZE_INCR;
current_index++;
if(current_index >= NUM_RADAR_HNDSHK)
  {
      current_offset = FIRST_RADAR_OFFSET;
      current_index = 0;
  }

fore_ray_pntr = (struct DATARAY *)(current_offset + STANDARD_BASE +
                            FORE_STAND_START); 
aft_ray_pntr = (struct DATARAY *)(current_offset + STANDARD_BASE +
                            AFT_STAND_START); 

/* Place the time read into the proper words of the data ray */

fore_ray_pntr->this_rayi.julian_day = (long)jday;
fore_ray_pntr->this_rayi.hour = (short)hr;
fore_ray_pntr->this_rayi.minute = (short)min;
fore_ray_pntr->this_rayi.second = (short)sec;
fore_ray_pntr->this_rayi.millisecond = (short)msec;
aft_ray_pntr->this_rayi.julian_day = (long)jday;
aft_ray_pntr->this_rayi.hour = (short)hr;
aft_ray_pntr->this_rayi.minute = (short)min;
aft_ray_pntr->this_rayi.second = (short)sec;
aft_ray_pntr->this_rayi.millisecond = (short)msec;

msecs_today = msec + 1000 * (sec + 60 * (min + 60 * hr));
msecs_ray[current_index] = msecs_today;

/* Put the test pulse info into the correct spot in the current ray */
fore_ray_pntr->this_fdata.test_pulse_level = fore_vmehndshk->tpulse_level;
fore_ray_pntr->this_fdata.test_pulse_dist = fore_vmehndshk->tpulse_dist;
fore_ray_pntr->this_fdata.test_pulse_width = fore_vmehndshk->tpulse_width;
fore_ray_pntr->this_fdata.test_pulse_atten = fore_vmehndshk->tpulse_atten;
fore_ray_pntr->this_fdata.test_pulse_fnum = fore_vmehndshk->tpulse_freq_num;

aft_ray_pntr->this_fdata.test_pulse_level = aft_vmehndshk->tpulse_level;
aft_ray_pntr->this_fdata.test_pulse_dist = aft_vmehndshk->tpulse_dist;
aft_ray_pntr->this_fdata.test_pulse_width = aft_vmehndshk->tpulse_width;
aft_ray_pntr->this_fdata.test_pulse_atten = aft_vmehndshk->tpulse_atten;
aft_ray_pntr->this_fdata.test_pulse_fnum = aft_vmehndshk->tpulse_freq_num;

/* Calculate the rotational position of both radar beams and place them
   into the proper word in the data ray */

fore_angle = position - ENCODER_ZERO_FORE_UP + FORE_SQUINT_ANGLE;
if(fore_angle > 360.0) fore_angle = fore_angle - 360.0;
if(fore_angle < 0.0) fore_angle = fore_angle + 360.0;

aft_angle = position - ENCODER_ZERO_AFT_UP + AFT_SQUINT_ANGLE;
if(aft_angle > 360.0) aft_angle = aft_angle - 360.0;
if(aft_angle < 0.0) aft_angle = aft_angle + 360.0;

fore_ray_pntr->this_plat.rotation_angle = fore_angle;
aft_ray_pntr->this_plat.rotation_angle = aft_angle;

/* Calculate the current velocity of the radar */

elapsed_time = (float)(msecs_today - last_msecs_today) / 1000.;
if(last_day != day)
  elapsed_time = (float)(msecs_today + 86400000 - last_msecs_today) / 1000.;

last_msecs_today = msecs_today;
last_day = day;

temp_position = position;
degrees_moved = position - last_position;
if(degrees_moved < 0.)
  degrees_moved = -1. * degrees_moved;

if(fraddes->req_rotat_vel > 0.)   /* Trying to spin in a positive direction */
      if(degrees_moved > 300.) position = position + 360.;

else                            /* Trying to spin in negative direction */
  if(degrees_moved > 300.) position = position - 360.;

degrees_moved = position - last_position; 

fore_ray_pntr->this_rayi.true_scan_rate = degrees_moved/elapsed_time;
aft_ray_pntr->this_rayi.true_scan_rate = degrees_moved/elapsed_time;

last_position = temp_position;

/* Now we need to update the sweep number for
   each of the radars */

degrees_moved = fore_angle - last_fore_angle;
if(degrees_moved  < 0.)
  degrees_moved = -1. * degrees_moved;
last_fore_angle = fore_angle;

if(degrees_moved > 300.) fore_sweep_num++;
fore_ray_pntr->this_rayi.sweep_num = fore_sweep_num;

degrees_moved = aft_angle - last_aft_angle;
if(degrees_moved  < 0.)
  degrees_moved = -1. * degrees_moved;
last_aft_angle = aft_angle;

if(degrees_moved > 300.) aft_sweep_num++;
aft_ray_pntr->this_rayi.sweep_num = aft_sweep_num;

}
