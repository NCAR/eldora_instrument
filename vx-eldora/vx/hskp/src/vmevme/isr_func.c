/*
 *	$Id$
 *
 *	Module:	isr_func	 
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
 * description: This module performs all of the necessary functions 
 *              during the isr run by the interrupt caused by the fore
 *              radar vme to vme interface.  This is the middle of beam
 *              interrupt. This routine handles both the fore and aft
 *              radar processors.
 *              Remember! no printf's in this routine (or any routines called
 *              by this routine)!!!!.
 *
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern
#include "hskpAll.h"

void isr_func()
{

/* Define some general purpose variables */

float position, fore_angle, aft_angle, elapsed_time, temp_position;
float degrees_moved, instant_speed, diff;
static float dumb_position;

long msecs_today;

/* Read the Position of the rotodome */

position = get_pos();

/* Read the time from the time-of-day card */

get_time(&hr,&min,&sec,&msec,&jday,&mon,&day,&yr);
msecs_today = msec + 1000 * (sec + 60 * (min + 60 * hr));

/* Put in a fake parameters if global variable fake_angles is true */

if(fake_angles)
  {
      dumb_position += 1.2;
      if(dumb_position > 360.0) dumb_position = 0;
      position = dumb_position;
  }

/* Clear the polled handshake words */

if(fore_vmehndshk->polled != 1) logMsg("F",0,0,0,0,0,0);
if(aft_vmehndshk->polled != 1 && aft_vmehndshk->polled != 0)
  logMsg("A",0,0,0,0,0,0);
fore_vmehndshk->polled = 0;
aft_vmehndshk->polled = 0;

/* Check to see if a ray long enough into the past to cover any IRU
   time lags is ready to be recorded. */

fore_ray_pntr = (struct DATARAY *)(iru_lag_offset + STANDARD_BASE +
                 FORE_STAND_START);
aft_ray_pntr = (struct DATARAY *)(iru_lag_offset + STANDARD_BASE +
                 AFT_STAND_START);

if(fore_vmehndshk->radar_hndshk[iru_lag_index] == 0)
  {
      fore_vmehndshk->radar_hndshk[iru_lag_index] = 1;
      aft_vmehndshk->radar_hndshk[iru_lag_index] = 1;
  }
else
      logMsg("N",0,0,0,0,0,0);

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
  {
      iru_lag_offset = FIRST_RADAR_OFFSET;
      iru_lag_index = 0;
  }

/* Create a pointer to a data ray structure at the current radar data
   ray location */

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

fore_angle = position - afrad->encoder_antenna_up * afrad->H_plane_angle;
if(fore_angle > 360.0) fore_angle = fore_angle - 360.0;
if(fore_angle < 0.0) fore_angle = fore_angle + 360.0;

aft_angle = position - afrad->encoder_antenna_up * afrad->H_plane_angle;
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

instant_speed = degrees_moved/elapsed_time;
fore_ray_pntr->this_rayi.true_scan_rate = instant_speed;
aft_ray_pntr->this_rayi.true_scan_rate = instant_speed;

last_position = temp_position;

/* Set the velocity command based on an integrated error in velocity */

diff = fraddes->req_rotat_vel - instant_speed;
if((diff < ((-1)*delta_degpersec)) || (diff > delta_degpersec))
  {
      rpm = rpm + diff * integrator_gain * DEGPERSEC_TO_RPM;
      just_set_vel(rpm);
  }

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
