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
#include "hskpDef.h"
#include "hskpInt.h"
#include "hskpGbl.h"
#include "pwrDef.h"
#include "pwrGbl.h"
#include "pwrFunc.h"
#include "gpsDef.h"
#include "gpsGbl.h"
#include "gpsFunc.h"
#include "iruDef.h"
#include "iruGbl.h"
#include "iruFunc.h"
#include "minDef.h"
#include "minFunc.h"
#include "tp41vAdr.h"
#include "vmevmeDef.h"
#include "vme_hndshk.h"
#include "vmevmeAdr.h"
#include "vmevmeFunc.h"
#include "vmevmeGbl.h"

void fill_platform(long msecs)
{
/* Note: this routine will change as the various navigation data sources
   become available so the very best known values are used */

long next_index, failsafe;
struct DATARAY *ray_pntr;
float ns_wind, ew_wind;

#define DEGS_TO_RADS 0.0174533     /* Degrees to radians */
#define RADS_TO_DEGS 57.2957      /* Radians to Degrees */

float corr_rot_angle, sin_crot_angle, cos_crot_angle;
float sin_heading, cos_heading, sin_pitch, cos_pitch;
double x, y, z;

 
next_index = current_platform_index + 1;
if(next_index >= NUM_RADAR_HNDSHK) next_index = 0;

/* check for time rolling over midnight */

if((msecs_ray[next_index] - msecs) < -8000000 ) 
  msecs = msecs + 86400000;

/* Is the time of the current data greater than that pointed to by
   current platform index? */

failsafe = 0;
while((msecs > msecs_ray[next_index]) && (failsafe < 3))
  {


      /* Fill the fore radar's platform info block first */

      ray_pntr = (struct DATARAY *)(current_platform_offset + STANDARD_BASE
				    + FORE_STAND_START);
      ray_pntr->this_plat.longitude = last_iru_data.longitude;
      ray_pntr->this_plat.latitude = last_iru_data.latitude;
      ray_pntr->this_plat.altitude_msl = last_iru_data.altitude;
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

      if((last_iru_data.wind_direction > 0.0) &&
	 (last_iru_data.wind_direction <= 90.0))
	 {
	     ew_wind =
	       sin((double)(last_iru_data.wind_direction * DEGS_TO_RADS)) *
		 last_iru_data.wind_speed;
	     ns_wind =
	       cos((double)(last_iru_data.wind_direction * DEGS_TO_RADS)) *
		 last_iru_data.wind_speed;
	 }
      if((last_iru_data.wind_direction > 90.0) &&
	 (last_iru_data.wind_direction <= 180.0))
	 {
	     last_iru_data.wind_direction -= 90.0;
	     ew_wind =
	       cos((double)(last_iru_data.wind_direction * DEGS_TO_RADS)) *
		 last_iru_data.wind_speed;
	     ns_wind =
	       (-1.) * sin((double)(last_iru_data.wind_direction *
				    DEGS_TO_RADS)) * last_iru_data.wind_speed;
	 }
      if((last_iru_data.wind_direction > 180.0) &&
	 (last_iru_data.wind_direction <= 270.0))
	 {
	     last_iru_data.wind_direction -= 180.0;
	     ew_wind =
	       (-1.) * sin((double)(last_iru_data.wind_direction *
				    DEGS_TO_RADS)) * last_iru_data.wind_speed;
	     ns_wind =
	       (-1.) * (last_iru_data.wind_direction * DEGS_TO_RADS) *
		 last_iru_data.wind_speed;
	 }
      if((last_iru_data.wind_direction > 270.0) &&
	 (last_iru_data.wind_direction <= 360.0))
	 {
	     last_iru_data.wind_direction -= 270.0;
	     ew_wind =
	       (-1.) * cos((double)(last_iru_data.wind_direction *
				    DEGS_TO_RADS)) * last_iru_data.wind_speed;
	     ns_wind =
	       sin((double)(last_iru_data.wind_direction * DEGS_TO_RADS)) *
		 last_iru_data.wind_speed;
	 }
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

      x = -1.0 * cos_crot_angle * sin_heading * COS_FTILT * sin_pitch +
	cos_heading * sin_crot_angle * COS_FTILT +
	  sin_heading * cos_pitch * SIN_FTILT;
      y = -1.0 * cos_crot_angle * cos_heading * COS_FTILT * sin_pitch -
	sin_heading * sin_crot_angle * COS_FTILT +
	  cos_pitch * cos_heading * SIN_FTILT;
      z = cos_pitch * COS_FTILT * cos_crot_angle +
	sin_pitch * SIN_FTILT;

      if((x != (double)0.0) && (y != (double)0.0))
	{
	    ray_pntr->this_rayi.azimuth = RADS_TO_DEGS * (float)atan2(x,y);
	    if(ray_pntr->this_rayi.azimuth< 0.0)
	      ray_pntr->this_rayi.azimuth += 360.;
	}

      if((z >= (double)-1.0) && (z <= (double)1.0))
	 ray_pntr->this_rayi.elevation = RADS_TO_DEGS * (float)asin(z);

      /* Now Repeat everything for the aft radar */

      ray_pntr = (struct DATARAY *)(current_platform_offset + STANDARD_BASE
				    + AFT_STAND_START);
      ray_pntr->this_plat.longitude = last_iru_data.longitude;
      ray_pntr->this_plat.latitude = last_iru_data.latitude;
      ray_pntr->this_plat.altitude_msl = last_iru_data.altitude;
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

      x = -1.0 * cos_crot_angle * sin_heading * COS_ATILT * sin_pitch +
	cos_heading * sin_crot_angle * COS_ATILT +
	  sin_heading * cos_pitch * SIN_ATILT;
      y = -1.0 * cos_crot_angle * cos_heading * COS_ATILT * sin_pitch -
	sin_heading * sin_crot_angle * COS_ATILT +
	  cos_pitch * cos_heading * SIN_ATILT;
      z = cos_pitch * COS_ATILT * cos_crot_angle +
	sin_pitch * SIN_ATILT;

      if((x != (double)0.0) && (y != (double)0.0))
	{
	    ray_pntr->this_rayi.azimuth = RADS_TO_DEGS * (float)atan2(x,y);
	    if(ray_pntr->this_rayi.azimuth< 0.0)
	      ray_pntr->this_rayi.azimuth += 360.;
	}

      if((z >= (double)-1.0) && (z <= (double)1.0))
	 ray_pntr->this_rayi.elevation = RADS_TO_DEGS * (float)asin(z);

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
