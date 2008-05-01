 /*
 *	$Id$
 *
 *	Module:	fill_platform	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.5  1996/02/09  18:29:27  craig
 * *** empty log message ***
 *
 * Revision 1.4  1994/05/20  20:37:08  craig
 * *** empty log message ***
 *
 * Revision 1.3  1993/09/20  16:56:18  craig
 * *** empty log message ***
 *
 * Revision 1.2  1993/05/12  16:08:38  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/11/03  22:51:43  craig
 * Initial revision
 *
 * Revision 1.1  1992/09/25  20:43:04  craig
 * Initial revision
 *
 * description: This module fills the platform info block pointed to by
 *              the variable current_platform_offset, used in conjunction with
 *              the current platform index. 
 *
 *
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern
#include "hskpAll.h"
double cwtan();

void fill_platform(long msecs)
{
/* Note: this routine will change as the various navigation data sources
   become available so the very best known values are used */

long next_index, failsafe;
struct DATARAY *ray_pntr;
float ns_wind, ew_wind, aft_azimuth, aft_elevation;

#define DEGS_TO_RADS 0.0174533     /* Degrees to radians */
#define RADS_TO_DEGS 57.2957      /* Radians to Degrees */

float corr_rot_angle, sin_crot_angle, cos_crot_angle;
float sin_heading, cos_heading, sin_pitch, cos_pitch;
double x, y, z;

if((current_platform_index < 0) || (current_platform_index >= NUM_RADAR_HNDSHK))
  printf("cpi=%d",current_platform_index);
 
next_index = current_platform_index + 1;
if(next_index >= NUM_RADAR_HNDSHK) next_index = 0;

/* check for time rolling over midnight */

if((msecs_ray[next_index] - msecs) < -8000000 ) 
  msecs = msecs + 86400000;

/* Is the time of the current data greater than that pointed to by
   current platform index? */

failsafe = 0;
while((msecs > (msecs_ray[next_index]-half_dwelltime_msec)) && (failsafe < 4))
  {

      /* Fill the aft radar's platform info block first */

      ray_pntr = (struct DATARAY *)(current_platform_offset + STANDARD_BASE
				    + AFT_STAND_START);
      ray_pntr->this_plat.longitude = last_iru_data.longitude;
      ray_pntr->this_plat.latitude = last_iru_data.latitude;
      ray_pntr->this_plat.altitude_msl = last_iru_data.altitude;
      ray_pntr->this_plat.altitude_agl = last_gps_data.altitude;
      ray_pntr->this_plat.ew_velocity = last_iru_data.ew_velocity;
      ray_pntr->this_plat.ns_velocity = last_iru_data.ns_velocity;
      ray_pntr->this_plat.vert_velocity = last_iru_data.inertial_vspeed;
      ray_pntr->this_plat.heading = last_iru_data.heading;
      ray_pntr->this_plat.roll = last_iru_data.roll;
      ray_pntr->this_plat.pitch = last_iru_data.pitch;
      ray_pntr->this_plat.drift_angle = last_iru_data.drift;
      ray_pntr->this_plat.vert_wind = -999;
      ray_pntr->this_plat.heading_change = last_iru_data.track_rate;
      ray_pntr->this_plat.pitch_change = last_iru_data.pitch_rate;
 
/* Calculate the East-West and North South horizontal winds */

      ew_wind = sin((double)(last_iru_data.wind_direction * DEGS_TO_RADS)) *
		 (-1) * last_iru_data.wind_speed;
      ns_wind = cos((double)(last_iru_data.wind_direction * DEGS_TO_RADS)) *
		 (-1) * last_iru_data.wind_speed;
      ray_pntr->this_plat.ns_horiz_wind = ns_wind;
      ray_pntr->this_plat.ew_horiz_wind = ew_wind;

/* Calculate the azimuth and the elevation use equations as defined by:
   Wen-Chau Lee, Frank Marks and Peter Dodge */

      corr_rot_angle = ray_pntr->this_plat.rotation_angle + last_iru_data.roll;
      sin_crot_angle = sin((double)(corr_rot_angle * DEGS_TO_RADS));
      cos_crot_angle = cos((double)(corr_rot_angle * DEGS_TO_RADS));
      sin_heading = sin((double)(last_iru_data.heading * DEGS_TO_RADS));
      cos_heading = cos((double)(last_iru_data.heading * DEGS_TO_RADS));
      sin_pitch = sin((double)(last_iru_data.pitch * DEGS_TO_RADS));
      cos_pitch = cos((double)(last_iru_data.pitch * DEGS_TO_RADS));

      x = -1.0 * cos_crot_angle * sin_heading * cos_atilt * sin_pitch +
	cos_heading * sin_crot_angle * cos_atilt +
	  sin_heading * cos_pitch * sin_atilt;
      y = -1.0 * cos_crot_angle * cos_heading * cos_atilt * sin_pitch -
	sin_heading * sin_crot_angle * cos_atilt +
	  cos_pitch * cos_heading * sin_atilt;
      z = cos_pitch * cos_atilt * cos_crot_angle +
	sin_pitch * sin_atilt;

      if((x != (double)0.0) && (y != (double)0.0))
	{
	    ray_pntr->this_rayi.azimuth = RADS_TO_DEGS * (float)cwtan(x,y);
	    if(ray_pntr->this_rayi.azimuth< 0.0)
	      ray_pntr->this_rayi.azimuth += 360.;
	}

      if((z >= (double)-1.0) && (z <= (double)1.0))
	 ray_pntr->this_rayi.elevation = RADS_TO_DEGS * (float)asin(z);

      aft_azimuth = ray_pntr->this_rayi.azimuth;
      aft_elevation = ray_pntr->this_rayi.elevation;

      /* Now repeat everything for the FORE radar */

      ray_pntr = (struct DATARAY *)(current_platform_offset + STANDARD_BASE
				    + FORE_STAND_START);
      ray_pntr->this_plat.longitude = last_iru_data.longitude;
      ray_pntr->this_plat.latitude = last_iru_data.latitude;
      ray_pntr->this_plat.altitude_msl = last_iru_data.altitude;
      ray_pntr->this_plat.altitude_agl = last_gps_data.altitude;
      ray_pntr->this_plat.ew_velocity = last_iru_data.ew_velocity;
      ray_pntr->this_plat.ns_velocity = last_iru_data.ns_velocity;
      ray_pntr->this_plat.vert_velocity = last_iru_data.inertial_vspeed;
      ray_pntr->this_plat.heading = last_iru_data.heading;
      ray_pntr->this_plat.roll = last_iru_data.roll;
      ray_pntr->this_plat.pitch = last_iru_data.pitch;
      ray_pntr->this_plat.drift_angle = last_iru_data.drift;
      ray_pntr->this_plat.vert_wind = -999;
      ray_pntr->this_plat.heading_change = last_iru_data.track_rate;
      ray_pntr->this_plat.pitch_change = last_iru_data.pitch_rate;
 
/* East-West and North South horizontal winds */

      ray_pntr->this_plat.ns_horiz_wind = ns_wind;
      ray_pntr->this_plat.ew_horiz_wind = ew_wind;

/* Calculate the azimuth and the elevation use equations as defined by:
   Wen-Chau Lee, Frank Marks and Peter Dodge */
      corr_rot_angle = ray_pntr->this_plat.rotation_angle + last_iru_data.roll;
      sin_crot_angle = sin((double)(corr_rot_angle * DEGS_TO_RADS));
      cos_crot_angle = cos((double)(corr_rot_angle * DEGS_TO_RADS));

      x = -1.0 * cos_crot_angle * sin_heading * cos_ftilt * sin_pitch +
	cos_heading * sin_crot_angle * cos_ftilt +
	  sin_heading * cos_pitch * sin_ftilt;
      y = -1.0 * cos_crot_angle * cos_heading * cos_ftilt * sin_pitch -
	sin_heading * sin_crot_angle * cos_ftilt +
	  cos_pitch * cos_heading * sin_ftilt;
      z = cos_pitch * cos_ftilt * cos_crot_angle +
	sin_pitch * sin_ftilt;

      if((x != (double)0.0) && (y != (double)0.0))
	{
	    ray_pntr->this_rayi.azimuth = RADS_TO_DEGS * (float)cwtan(x,y);
	    if(ray_pntr->this_rayi.azimuth< 0.0)
	      ray_pntr->this_rayi.azimuth += 360.;
	}

      if((z >= (double)-1.0) && (z <= (double)1.0))
	 ray_pntr->this_rayi.elevation = RADS_TO_DEGS * (float)asin(z);

      /* If waveguide switch was pointing at the aft radar, put the aft
	 radar's azimuth and elevation into the fore data record */

      if(ray_pntr->this_plat.tilt < 0 && wg_sw_flag > 0)
	{
	    ray_pntr->this_rayi.azimuth = aft_azimuth;
	    ray_pntr->this_rayi.elevation = aft_elevation;
	}


/* Update the current_platform_index and current_platform_offset */

      platform_status[current_platform_index] = 1;
      current_platform_offset += RADAR_SIZE_INCR;
      current_platform_index ++;

      if(current_platform_index >= NUM_RADAR_HNDSHK)
	{
	    current_platform_index = 0;
	    current_platform_offset = FIRST_RADAR_OFFSET;
	}

      next_index = current_platform_index + 1;
      if(next_index >= NUM_RADAR_HNDSHK) next_index = 0;
      failsafe++;
     }

return;
}
double cwtan(double y, double x)
{
/* same function as the atan2 function, calculates the 360 degree tangent of
   y/x */
double ratio, result;

ratio = 0;
if(x == (double)0.0) return(ratio);

ratio = y/x;
if(ratio < (double)0.0) ratio *= -1;
result = atan(ratio);

if((y <= (double)0.0) && (x > (double)0.0))
  result = 2*3.14159 - result;

if((y >= (double)0.0) && (x < (double)0.0))
  result = 3.14159 - result;

if((y <= (double)0.0) && (x < (double)0.0))
  result = 3.14159 + result;

return(result);
}
