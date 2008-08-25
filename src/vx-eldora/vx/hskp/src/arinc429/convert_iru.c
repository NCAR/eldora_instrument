/*
 *	$Id$
 *
 *	Module: convert_iru.c		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:  This routine reads the data from the iru interface
 *               and converts it to the proper units and places it
 *               in the last IRU data structure.        
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern

#include "hskpAll.h"

/* Define conversion factors from IRU units to meteorological units, note
   that these factors will also include the scale factors given by
   Honeywell to convert the binary words to standard IRU units. They
   are precomputed to increase runtine speed. */ 

static float PIRAD_TO_DEG =        9.5367e-7*180.0; /* Pirads to degrees */
static float WKNOTS_TO_MPERSEC =   2.44e-4*0.51449; /* knots to m/s (winds) */
static float VKNOTS_TO_MPERSEC =   0.0039*0.51449; /* knots to m/s
                                                      (velocities) */
static float FEET_TO_KM =          0.125*0.0003048; /* feet to kilometers */
static float GS_TO_MPERSEC2 =      1.22e-4*9.7959; /* G's to meter/sec2 */
static float FTPERSEC_TO_MPERSEC = 2.44e-4*0.3048; /* feet/sec to meter/sec */
static float FTPERMIN_TO_MPERSEC = 0.03125*0.3048/60.0; /* feet/min to
                                                          meter/sec */
static float TRATE_SCALE =         3.05e-5; /* Honeywell scale for track
                                               rate */ 
static float PRATE_SCALE =         0.0039; /* Honeywell scale for pitch
                                              rate */ 
static float HEAD_CORR =           0.0; /* Correction to heading for box
                                                  alignment */

void convert_iru()
{
/* Define the iru latency in milliseconds, remember that the latency (called
   transport lag by Honeywell) varies from parameter to parameter, since the
   most rapidly changing parameters that affect the radar data the most, are
   the attitude angles (i.e. pitch, roll and heading), the IRU latency is
   define such that these parameter will be aligned in time the most precisly
*/
#define IRU_LATENCY 50

long i, msecs_today, msecs_this_data;
short julday,test;
union{
  short sarray[2];
  long input;
}swp0;
union{
  short sarray[2];
  long data;
}swp1;

msecs_today = msec + 1000 * (sec + 60 * (minute + 60 * hr));

/* If main buffer pointer is different, recalculate pointers */

if(rxa_buffer_offset != iru_pntr->rxa0_vmeptr)
  {
    printf("B");
    calc_iru_pointers();
  }

/* Clear the ARINC errors bit from the status word */
currStatus->iru &= (char)(~ARINC_ERRORS);

/* Now take each ARINC 429 word and shift it to the right the right number
   of places then multiply the result by the proper scale factor.
   Remember that the >> operator will sign extend a signed
   integer, the most significant bit of these words is the sign by virtue of
   how the word is passed by the ARINC 429 interface. */

swp0.input = *latitude_pntr;
if(iru_new_data_test[IRU_LATITUDE] != swp0.input)
  {
    iru_new_data_test[IRU_LATITUDE] = swp0.input;
    swp1.sarray[0] = swp0.sarray[1]; 
    swp1.sarray[1] = swp0.sarray[0]; 
    last_iru_data.latitude =  (float)(swp1.data>>11)*(float)PIRAD_TO_DEG;
  }

swp0.input = *longitude_pntr;
if(iru_new_data_test[IRU_LONGITUDE] != swp0.input)
  {
    /* Log the time of new longitude data */

    last_iru_data.msec_longitude = msec;
    last_iru_data.sec_longitude = sec;
    swp1.sarray[0] = swp0.sarray[1]; 
    swp1.sarray[1] = swp0.sarray[0]; 
    iru_new_data_test[IRU_LONGITUDE] = swp0.input;
    last_iru_data.longitude = (float)(swp1.data>>11)*(float)PIRAD_TO_DEG;
  }

swp0.input = *wind_speed_pntr;
if(iru_new_data_test[IRU_WIND_SPEED] != swp0.input)
  {
    iru_new_data_test[IRU_WIND_SPEED] = swp0.input;
    swp1.sarray[0] = swp0.sarray[1]; 
    swp1.sarray[1] = swp0.sarray[0]; 
    last_iru_data.wind_speed = (float)(swp1.data>>11)*(float)WKNOTS_TO_MPERSEC;
  }

swp0.input = *wind_direction_pntr;
if(iru_new_data_test[IRU_WIND_DIRECTION] != swp0.input)
  {
    iru_new_data_test[IRU_WIND_DIRECTION] = swp0.input;
    swp1.sarray[0] = swp0.sarray[1]; 
    swp1.sarray[1] = swp0.sarray[0]; 
    last_iru_data.wind_direction = (float)(swp1.data>>11)*(float)PIRAD_TO_DEG;
    if(last_iru_data.wind_direction < 0.0) 
      last_iru_data.wind_direction += 360.0;
  }

swp0.input = *ns_velocity_pntr;
if(iru_new_data_test[IRU_NS_VELOCITY] != swp0.input)
  {
    iru_new_data_test[IRU_NS_VELOCITY] = swp0.input;
    swp1.sarray[0] = swp0.sarray[1]; 
    swp1.sarray[1] = swp0.sarray[0]; 
    last_iru_data.ns_velocity =
      (float)(swp1.data>>11)*(float)VKNOTS_TO_MPERSEC;
  }

swp0.input = *ew_velocity_pntr;
if(iru_new_data_test[IRU_EW_VELOCITY] != swp0.input)
  {
    iru_new_data_test[IRU_EW_VELOCITY] = swp0.input;
    swp1.sarray[0] = swp0.sarray[1]; 
    swp1.sarray[1] = swp0.sarray[0]; 
    last_iru_data.ew_velocity =
      (float)(swp1.data>>11)*(float)VKNOTS_TO_MPERSEC;
  }

swp0.input = *heading_pntr;
if(iru_new_data_test[IRU_HEADING] != swp0.input)
  {
    iru_new_data_test[IRU_HEADING] = swp0.input;
    swp1.sarray[0] = swp0.sarray[1]; 
    swp1.sarray[1] = swp0.sarray[0]; 
    last_iru_data.heading =
      (float)(swp1.data>>11)*(float)PIRAD_TO_DEG + HEAD_CORR;
    if(last_iru_data.heading < 0.0)
      last_iru_data.heading += 360.0;
    if(last_iru_data.heading > 360.0)
      last_iru_data.heading -= 360.0;
  }

swp0.input = *drift_pntr;
if(iru_new_data_test[IRU_DRIFT] != swp0.input)
  {
    iru_new_data_test[IRU_DRIFT] = swp0.input;
    swp1.sarray[0] = swp0.sarray[1]; 
    swp1.sarray[1] = swp0.sarray[0]; 
    last_iru_data.drift =
      (float)(swp1.data>>11)*(float)PIRAD_TO_DEG;
  }

swp0.input = *altitude_pntr;
if(iru_new_data_test[IRU_ALTITUDE] != swp0.input)
  {
    iru_new_data_test[IRU_ALTITUDE] = swp0.input;
    swp1.sarray[0] = swp0.sarray[1]; 
    swp1.sarray[1] = swp0.sarray[0]; 
    last_iru_data.altitude =
      (float)(swp1.data>>11)*(float)FEET_TO_KM;
  }

swp0.input = *inertial_vspeed_pntr;
if(iru_new_data_test[IRU_INERTIAL_VSPEED] != swp0.input)
  {
    iru_new_data_test[IRU_INERTIAL_VSPEED] = swp0.input;
    swp1.sarray[0] = swp0.sarray[1]; 
    swp1.sarray[1] = swp0.sarray[0]; 
    last_iru_data.inertial_vspeed =
      (float)(swp1.data>>11)*(float)FTPERMIN_TO_MPERSEC;
  }

swp0.input = *pitch_pntr;
if(iru_new_data_test[IRU_PITCH] != swp0.input)
  {
    iru_new_data_test[IRU_PITCH] = swp0.input;
    swp1.sarray[0] = swp0.sarray[1]; 
    swp1.sarray[1] = swp0.sarray[0]; 
    last_iru_data.pitch =
      (float)(swp1.data>>11)*(float)PIRAD_TO_DEG;
  }

swp0.input = *roll_pntr;
if(iru_new_data_test[IRU_ROLL] != swp0.input)
  {
    iru_new_data_test[IRU_ROLL] = swp0.input;
    swp1.sarray[0] = swp0.sarray[1]; 
    swp1.sarray[1] = swp0.sarray[0]; 
    last_iru_data.roll =
      (float)(swp1.data>>11)*(float)PIRAD_TO_DEG;
  }

swp0.input = *integ_vert_acc_pntr;
if(iru_new_data_test[IRU_INTEG_VERT_ACC] != swp0.input)
  {
    iru_new_data_test[IRU_INTEG_VERT_ACC] = swp0.input;
    swp1.sarray[0] = swp0.sarray[1]; 
    swp1.sarray[1] = swp0.sarray[0]; 
    last_iru_data.integ_vert_acc =
      (float)(swp1.data>>11)*(float)FTPERSEC_TO_MPERSEC;
  }

swp0.input = *vert_acc_pntr;
if(iru_new_data_test[IRU_VERT_ACC] != swp0.input)
  {
    iru_new_data_test[IRU_VERT_ACC] = swp0.input;
    swp1.sarray[0] = swp0.sarray[1]; 
    swp1.sarray[1] = swp0.sarray[0]; 
    last_iru_data.vert_acc =
      (float)(swp1.data>>11)*(float)GS_TO_MPERSEC2;
  }

swp0.input = *track_rate_pntr;
if(iru_new_data_test[IRU_TRACK_RATE] != swp0.input)
  {
    iru_new_data_test[IRU_TRACK_RATE] = swp0.input;
    swp1.sarray[0] = swp0.sarray[1]; 
    swp1.sarray[1] = swp0.sarray[0]; 
    last_iru_data.track_rate =
      (float)(swp1.data>>11)*(float)TRATE_SCALE;
  }

swp0.input = *pitch_rate_pntr;
if(iru_new_data_test[IRU_PITCH_RATE] != swp0.input)
  {
    iru_new_data_test[IRU_PITCH_RATE] = swp0.input;
    swp1.sarray[0] = swp0.sarray[1]; 
    swp1.sarray[1] = swp0.sarray[0]; 
    last_iru_data.pitch_rate =
      (float)(swp1.data>>11)*(float)PRATE_SCALE;
  }


/* We need to calculate the time of the current data and then
   place that data into the correct platform info block */

msecs_this_data = msecs_today - IRU_LATENCY;
fill_platform(msecs_this_data);

}
