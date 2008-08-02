/*
 * $Id$
 *
 * Module: start_vmevme  
 * Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 * Date:   $Date$
 * 
 * description: This module starts the radar processors over the
 *              vme to vme interfaces.
 *              
 */

static char rcsid[] = "$Date$ $RCSfile: start_vmevme.c,v $ $Revision$";

#define OK_RPC
#define scope extern
#include "hskpAll.h"

void start_vmevme()
{
/* Define some general purpose variables */
long i, iru_lags, msecs_today;
long radar_offset;
struct DATARAY *ray_pntr;

/* Set each of the handshake areas to the "good data broadcast" status word
   of 0xAFFF */

for(i=0; i<31; i++)
  fore_vmehndshk->radar_hndshk[i] = 0xAFFF;

for(i=0; i<31; i++)
  aft_vmehndshk->radar_hndshk[i] = 0xAFFF;

/* The Radar processors will be 4 dwell times behind so set each of the
 indecies to two (2) (starts at 0, increments once before it is used) */
/*
current_index = 2;
*/

/* Attempt Fix of ground tilt by changing pipeline delay to 2 from 4 above */
/* Test Measurements support 2 delays */
/* The Radar processors will be 2 dwell times behind so set each of the
 indecies to 1 (0) (starts at 0, increments once before it is used) */

current_index = 0;
ads_current_index = 0;

/* Setup the offsets for all of the data types */

current_offset = FIRST_RADAR_OFFSET + current_index * RADAR_SIZE_INCR;
ads_current_offset = FIRST_ADS_OFFSET;

/* Make an index that will follow behind the navigational data and make the
   radar processors send it out over the mcpl.   
Begin by calculating the number of lags the iru data will lag behind */

iru_lags = 20/dwelltime_msec + 4;  /* +4 because this should lag
           at a safe distance */

iru_lag_index = current_index - iru_lags;
if(iru_lag_index < 0) iru_lag_index += 27;

iru_lag_offset = FIRST_RADAR_OFFSET + iru_lag_index * RADAR_SIZE_INCR;

printf("MSECS in a dwell = %d   iru_lag_index = %d\n",dwelltime_msec,
       iru_lag_index);

/* Setup the platform data index and offset values */

iru_lags = 20/dwelltime_msec;  /* 50 because we get roll,pitch,yaw at 50 Hz */

current_platform_index = 2 - iru_lags;
if(current_platform_index < 0) current_platform_index += 27;
current_platform_offset = FIRST_RADAR_OFFSET +
  current_platform_index * RADAR_SIZE_INCR;


/* Get current milliseconds since midnite and place the value
   into all entries of msecs_ray */

get_time(&hr,&minute,&sec,&msec,&jday,&mon,&day,&yr);
msecs_today = msec + 1000 * (sec + 60 * (minute + 60 * hr));

radar_offset = FIRST_RADAR_OFFSET;
for(i=0; i<NUM_RADAR_HNDSHK; i++)
  {
      platform_status[i] = 0;
      msecs_ray[i] = msecs_today;
      /* Put things in each record that only change with a header */
      ray_pntr = (struct DATARAY *)(radar_offset + STANDARD_BASE +
        FORE_STAND_START);
      ray_pntr->this_rayi.peak_power = -999.0;
      ray_pntr->this_rayi.ray_status = 0;
      ray_pntr->this_plat.altitude_agl = -999.0;
      ray_pntr->this_plat.tilt = ffrad->E_plane_angle;
      ray_pntr->this_fdata.data_sys_status = 0;

      ray_pntr = (struct DATARAY *)(radar_offset + STANDARD_BASE +
        AFT_STAND_START);
      ray_pntr->this_rayi.peak_power = -999.0;
      ray_pntr->this_rayi.ray_status = 0;
      ray_pntr->this_plat.altitude_agl = -999.0;
      ray_pntr->this_plat.tilt = afrad->E_plane_angle;
      ray_pntr->this_fdata.data_sys_status = 0;

      radar_offset += RADAR_SIZE_INCR;
  }


/* Put ASCII characters into both nav structures */

current_nav_pntr = (struct nav_data *)(SEC_NAV_OFFSET + 
                    STANDARD_BASE + FORE_STAND_START);
fill_nav_info((int)1);
current_nav_pntr = (struct nav_data *)(FIRST_NAV_OFFSET + 
                    STANDARD_BASE + FORE_STAND_START);
fill_nav_info((int)1);

fore_vmehndshk->nav_length = sizeof(struct nav_data);

/* Clear the polled handshake words */
fore_vmehndshk->polled = 0;
aft_vmehndshk->polled = 0;

/* Initialize global variables */

Intr_cnt = 0;
Proc_dly_sync = 1;

/* Wait for the radar processors to get ready */
/* Tom 3/20/08
do{
    taskDelay(2);
}while((fore_vmehndshk->start_hndshk == 0) ||
       (aft_vmehndshk->start_hndshk == 0)); 
       
// Now write "Hi Radar" to the radars to get them going 


aft_vmehndshk->salute[0] = 'H';
aft_vmehndshk->salute[1] = 'I';
aft_vmehndshk->salute[2] = ' ';
aft_vmehndshk->salute[3] = 'R';
aft_vmehndshk->salute[4] = 'A';
aft_vmehndshk->salute[5] = 'D';
aft_vmehndshk->salute[6] = 'A';
aft_vmehndshk->salute[7] = 'R';

fore_vmehndshk->salute[0] = 'H';
fore_vmehndshk->salute[1] = 'I';
fore_vmehndshk->salute[2] = ' ';
fore_vmehndshk->salute[3] = 'R';
fore_vmehndshk->salute[4] = 'A';
fore_vmehndshk->salute[5] = 'D';
fore_vmehndshk->salute[6] = 'A';
fore_vmehndshk->salute[7] = 'R';
*/

// Now we can enable the interrupt
sysIntEnable(VME_VME_IRQ);
}







