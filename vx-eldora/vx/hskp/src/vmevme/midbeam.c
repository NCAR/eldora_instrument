/*
 *	$Id$
 *
 *	Module:	midbeam	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.5  1999/09/27 15:45:02  eric
 * added test for missed or extra processor interrupts.
 *
 * Revision 1.4  1999/07/22  17:47:06  eric
 * made ls byte of tpulse_freq_num track test pulse and
 * ms byte track xmit pulse frequency numbers.
 *
 * Revision 1.3  1999/07/20  17:42:56  eric
 * *** empty log message ***
 *
 * Revision 1.2  1997/08/27  16:50:04  craig
 * *** empty log message ***
 *
 * Revision 1.1  1996/02/09  18:29:29  craig
 * Initial revision
 *
 * description: This module performs all of the necessary functions 
 *              during the isr run by the interrupt caused by the fore
 *              radar vme to vme interface.  This is the middle of beam
 *              interrupt. This routine handles both the fore and aft
 *              radar processors.
 *
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern
#include "hskpAll.h"

int ERR_CHK; 
static int proc_offset_f, proc_offset_a;
void midbeam()
{
/* Define some general purpose variables */

float position, fore_angle, aft_angle, elapsed_time, temp_position;
float degrees_moved, instant_speed, diff;
static float dumb_position;
float dumb_stepr, dumb_stepp, anglesin, dumb_rads;
int dumb_start, dumb_index, test, delta_f, delta_a;

long msecs_today;
ERR_CHK = 1;
dumb_start = 0;

for (;;)
  {
/* Wait for the semaphore. */
semTake(vmeSem,WAIT_FOREVER);


/* Read the Position of the rotodome */

    position = get_pos();

/* Read the time from the time-of-day card */

    get_time(&hr,&min,&sec,&msec,&jday,&mon,&day,&yr);
    msecs_today = msec + 1000 * (sec + 60 * (min + 60 * hr));

/* Put in a fake rotational position if global variable fake_angles is true */

    if(fake_angles)
      {
	dumb_position += 1.2;
	if(dumb_position > 360.0) dumb_position = 0;
	if(dumb_position < 0.0) dumb_position = 0;
	position = dumb_position;

/* Put in a fake parameters if global variable fake_angles is true */

	if(dumb_start == 0)
	  {
	    dumb_start = 1;  
	    dumb_stepr = 6.2831 / (60000.0 / (float)dwelltime_msec);
	    /* 2 pi radians per second */
	    dumb_stepp = 0.000757 / (float)dwelltime_msec;
	    /* Is about 120 m/s if lat step = long step */ 
	    dumb_rads = 0;
	    last_iru_data.longitude = -105.;
	    last_iru_data.latitude = 40.;
	  }

      /* Dummy up the INS parameters */
	dumb_rads += dumb_stepr;
	if(dumb_rads > 6.28318) dumb_rads = 0;
	anglesin = sin(dumb_rads);
	last_iru_data.longitude += dumb_stepp;
	last_iru_data.latitude += dumb_stepp;
/* 	last_iru_data.pitch = 2.0 * anglesin; */
/* 	last_iru_data.roll = 15.0 * anglesin; */
	last_iru_data.pitch = 0.0;
	last_iru_data.roll = 1.0;
	last_iru_data.heading = 45.0 + 0.5 * anglesin;
	last_iru_data.altitude = 4.572 + 0.3048 * anglesin;
	last_iru_data.sec_longitude = sec;
	last_iru_data.msec_longitude = msec;
	dumb_index++;
	if(dumb_index >= NUM_RADAR_HNDSHK) dumb_index=0;
	fill_platform(msecs_ray[dumb_index]);
	
      }


/* Clear the polled handshake words */

    mcpl_error[0] = fore_vmehndshk->polled;
    mcpl_error[1] = aft_vmehndshk->polled;
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
      mcpl_error[2] = 1;
    
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

/* 
Determine delay between Radar Processor data interrupt and Hskp "midbeam" interrupt and set current_index accordingly 
*/

if(Proc_dly_sync)
  {
    Intr_cnt += 1;
    if(Intr_cnt == 10)
      {
/*	delta_f = Intr_cnt - fore_vmehndshk->radar_proc_idx; */
	delta_f = current_index - fore_vmehndshk->radar_proc_idx;
	delta_a = current_index - aft_vmehndshk->radar_proc_idx;
	proc_offset_f = delta_f;
	proc_offset_a = delta_a;
	printf("fore proc_offset = %d\n",proc_offset_f);
	printf("aft proc_offset = %d\n",proc_offset_a);
	Proc_dly_sync = 0;
      }
  }
if(ERR_CHK)
  {
    if(!Proc_dly_sync)
      {
	delta_f = current_index - fore_vmehndshk->radar_proc_idx;
	delta_a = current_index - aft_vmehndshk->radar_proc_idx;
	if(delta_f < 0)
	  delta_f += NUM_RADAR_HNDSHK;
	if(delta_a < 0)
	  delta_a += NUM_RADAR_HNDSHK;
	if(delta_f > proc_offset_f) 
      printf("ERROR: Missed FORE INTERRUPT!;%d\n",delta_f);
	if(delta_a > proc_offset_a) 
	  printf("ERROR: Missed AFT INTERRUPT!;%d\n",delta_a);
	if(delta_f < (proc_offset_f - 1) || delta_a < (proc_offset_a - 1))
	  printf("ERROR: Got Extra INTERRUPT!;%d %d\n",delta_f,delta_a);
      }
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
    fore_ray_pntr->this_rayi.peak_power = fore_xmit_pwr;
    
    aft_ray_pntr->this_rayi.julian_day = (long)jday;
    aft_ray_pntr->this_rayi.hour = (short)hr;
    aft_ray_pntr->this_rayi.minute = (short)min;
    aft_ray_pntr->this_rayi.second = (short)sec;
    aft_ray_pntr->this_rayi.millisecond = (short)msec;
    aft_ray_pntr->this_rayi.peak_power = aft_xmit_pwr;
    
    msecs_ray[current_index] = msecs_today;

/* Put the test pulse info into the correct spot in the current ray */

    tp_dwell_count += 1;
    fore_ray_pntr->this_fdata.test_pulse_level = fore_testp_pwr;
    fore_ray_pntr->this_fdata.test_pulse_dist = fore_vmehndshk->tpulse_dist;
    fore_ray_pntr->this_fdata.test_pulse_width = fore_vmehndshk->tpulse_width;
    fore_ray_pntr->this_fdata.test_pulse_freq = fore_vmehndshk->tpulse_freq;
    fore_ray_pntr->this_fdata.test_pulse_atten = fore_vmehndshk->tpulse_atten;
    fore_ray_pntr->this_fdata.test_pulse_fnum = fore_vmehndshk->tpulse.combined_freq_num;
    
    aft_ray_pntr->this_fdata.test_pulse_level = aft_testp_pwr;
    aft_ray_pntr->this_fdata.test_pulse_dist = aft_vmehndshk->tpulse_dist;
    aft_ray_pntr->this_fdata.test_pulse_width = aft_vmehndshk->tpulse_width;
    aft_ray_pntr->this_fdata.test_pulse_freq = aft_vmehndshk->tpulse_freq;
    aft_ray_pntr->this_fdata.test_pulse_atten = aft_vmehndshk->tpulse_atten;
    aft_ray_pntr->this_fdata.test_pulse_fnum = aft_vmehndshk->tpulse.combined_freq_num;

    if((tp_dwell_count > tp_sum_start) && (tp_dwell_count < tp_sum_end))
      sum_testpulse();
    
    /* Calculate the rotational position of both radar beams and place them
       into the proper word in the data ray */
    
    fore_angle = position - ffrad->encoder_antenna_up + ffrad->H_plane_angle;
    if(fore_angle > 360.0) fore_angle = fore_angle - 360.0;
    if(fore_angle < 0.0) fore_angle = fore_angle + 360.0;
    
    aft_angle = position - afrad->encoder_antenna_up + afrad->H_plane_angle;
    if(aft_angle > 360.0) aft_angle = aft_angle - 360.0;
    if(aft_angle < 0.0) aft_angle = aft_angle + 360.0;
    
    fore_ray_pntr->this_plat.rotation_angle = fore_angle;
    aft_ray_pntr->this_plat.rotation_angle = aft_angle;
    
    /* Calculate the current velocity of the radar */

    elapsed_time = (float)(msecs_today - last_msecs_today) / 1000.;
    if(last_day != day)
      elapsed_time = (float)(msecs_today + 86400000 - last_msecs_today) / 1000.;
    if(elapsed_time <= 0.005) elapsed_time = 0.02;
    
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
    
    /*
      diff = fraddes->req_rotat_vel - instant_speed;
      if((diff < ((-1)*delta_degpersec)) || (diff > delta_degpersec))
      {
      rpm = rpm + diff * integrator_gain * DEGPERSEC_TO_RPM;
      just_set_vel(rpm);
      }
*/

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
    
/* Handle the data from the IRU here */
    convert_iru();
    
/* If necessary handle the waveguide switch here */

    if (wg_sw_flag != 0)
      {

      /* If we are really transmiting out the aft antenna, change the FORE
	 data ray's rotation angle and tilt angle to reflect this */

	if(wg_sw_current_set == WG_SW_AFT)
	  {
	    fore_ray_pntr->this_plat.tilt = afrad->E_plane_angle;
	    fore_ray_pntr->this_plat.rotation_angle = aft_angle;
	  }
	else
	  fore_ray_pntr-> this_plat.tilt = ffrad->E_plane_angle;

      /* wg_sw_counter makes sure we only switch
	 once each switch over point */

	wg_sw_counter +=1;
	if(wg_sw_counter > 10)
	  {

	    /* Now test to see if we are at a switch over point */

	    test = 0;
	    if (wg_sw_big_angle > 357.0)
	      {
		if(fore_angle > wg_sw_big_angle || 
		   fore_angle < wg_sw_small_angle) test = 1;
	      }
	    else
	      {
		if(fore_angle < wg_sw_big_angle &&
		   fore_angle > wg_sw_small_angle) test = 1;
	      }

	    if(test == 1)
	      {
		wg_sw_counter = 0;

		  /* We are at a switch over point, handle each type of
		     scanning in it own unique manner */

		switch(wg_sw_flag)
		  {
		  case 1:      /* Complete rotations */
		    
		    if(wg_sw_current_set == WG_SW_FORE)
		      wg_sw_current_set = WG_SW_AFT;
		    else 
		      wg_sw_current_set = WG_SW_FORE;
		    
		    break;
		    
		  case 2:     /* Right side only */
		    if(wg_sw_current_set == WG_SW_FORE)
		      {
			wg_sw_current_set = WG_SW_AFT;
			wg_sw_big_angle = 358.0;
			wg_sw_small_angle = 2.0;
		      }
		    else
		      {
			wg_sw_current_set = WG_SW_FORE;
			wg_sw_big_angle = 182.0;
			wg_sw_small_angle = 178.0;
		      }

		    break;
		    
		  case 3:     /* left side only */
		    if(wg_sw_current_set == WG_SW_FORE)
		      {
			wg_sw_current_set = WG_SW_AFT;
			wg_sw_big_angle = 182.0;
			wg_sw_small_angle = 178.0;
		      }
		    else
		      {
			wg_sw_current_set = WG_SW_FORE;
			wg_sw_big_angle = 358.0;
			wg_sw_small_angle = 2.0;
		      }

		    break;
		  default:
		    break;

		  }    /* switch on wg_sw_flag */

		/* Now switch the switch */

	      *wg_sw_pntr = wg_sw_current_set;

	      } /* test for switch over point (test == 1) */

	  } /* Test of wg_sw_counter > 10 */

      } /* Test of wg_sw_flag != 0 */


} /* Infinite for loop */
}

