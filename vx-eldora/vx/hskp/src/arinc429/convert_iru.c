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
 * Revision 1.1  1992/09/28  23:03:55  craig
 * Initial revision
 *
 *
 * description:  This routine reads the data from the iru interface
 *               and converts it to the proper units and places it
 *               in the desired IRU data structure.        
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern

#include "hskpAll.h"

int convert_iru(long num_words, long *where_data_is, struct ins_data *goes)
{
/* Define the iru latency in milliseconds, remember that the latency (called
   transport lag by Honeywell) varies from parameter to parameter, since the
   most rapidly changing parameters that affect the radar data the most, are
   the attitude angles (i.e. pitch, roll and heading), the IRU latency is
   define such that these parameter will be aligned in time the most precisly
*/
#define IRU_LATENCY 50

/* Define conversion factors from IRU units to meteorological units, note
   that these factors will also include the scale factors given by
   Honeywell to convert the binary words to standard IRU units */ 

#define PIRAD_TO_DEG          9.54e-7*180.0    /* Pirads to degrees */
#define WKNOTS_TO_MPERSEC     2.44e-4*0.51449  /* knots to m/s (winds) */
#define VKNOTS_TO_MPERSEC     0.0039*0.51449   /* knots to m/s (velocities) */
#define FEET_TO_KM            0.125*0.0003048  /* feet to kilometers */
#define GS_TO_MPERSEC2        1.22e-4*9.7959   /* G's to meter/sec2 */
#define FTPERSEC_TO_MPERSEC   2.44e-4*0.3048   /* feet/sec to meter/sec */
#define FTPERMIN_TO_MPERSEC   0.03125*0.3048/60.   /* feet/min to meter/sec */
#define TRATE_SCALE           3.05e-5     /* Honeywell scale for track rate */ 
#define PRATE_SCALE           0.0039      /* Honeywell scale for pitch rate */ 
#define HEAD_CORR             0.0              /* Correction to heading for box
                                                  alignment */

long i, data, label, msecs_today, msecs_this_data;
char hr,min,sec,today,mon,yr;
short msec,jday,test;

/* Read the time from the time of day clock */

get_time(&hr,&min,&sec,&msec,&jday,&mon,&today,&yr);
goes->msec_longitude[msec_longitude_indx] = msec;
goes->sec_longitude[msec_longitude_indx] = sec;
msec_longitude_indx++;
if(msec_longitude_indx >= 5) msec_longitude_indx = 0;
last_iru_data.seconds = sec;
last_iru_data.msec_longitude = msec;
msecs_today = msec + 1000 * (sec + 60 * (min + 60 * hr));


/* Clear the ARINC errors bit from the status word */
currStatus->iru &= (char)(~ARINC_ERRORS);

/* We need to read num_words, ARINC words and convert them to the proper units
and place them in the ins data structure pointer to by the pointer goes */

for(i=0; i<num_words; i++)
  {
      data = *where_data_is++;
      label = data & 0x000000FF;

/* Now take the ARINC 429 word and shift it to the right the right number
   of places then multiply the result by the proper scale factor.
   Remember that the >> operator will sign extend a signed
   integer, the most significant bit of these words is the sign by virtue of
   how the word is passed by the ARINC 429 interface. */

      if(data & (long)0x600 != 0x600) currStatus->iru |= ARINC_ERRORS;

/*      printf("converting word %d data= %x, label = %x\n",i,data,label); */

      switch(label)
	{
	    case LATITUDE_LAB:
	    last_iru_data.latitude =
	      (float)(data>>11)*(float)PIRAD_TO_DEG;
	    goes->latitude[latitude_indx] = last_iru_data.latitude;
	    latitude_indx++;
	    if(latitude_indx >= 5) latitude_indx = 0;
	    break;

	    case LONGITUDE_LAB:
	    last_iru_data.longitude =
	      (float)(data>>11)*(float)PIRAD_TO_DEG;
	    goes->longitude[longitude_indx] = last_iru_data.longitude;
	    longitude_indx++;
	    if(longitude_indx >= 5) longitude_indx = 0;
	    break;

	    case WIND_SPEED_LAB:
	    last_iru_data.wind_speed =
	      (float)(data>>11)*(float)WKNOTS_TO_MPERSEC;
	    goes->wind_speed[wind_speed_indx] = last_iru_data.wind_speed;
	    wind_speed_indx++;
	    if(wind_speed_indx >= 10) wind_speed_indx = 0;
	    break;

	    case WIND_DIRECTION_LAB:
	    last_iru_data.wind_direction =
	      (float)(data>>11)*(float)PIRAD_TO_DEG;
	    if(last_iru_data.wind_direction < 0)
	      last_iru_data.wind_direction += 360.0;
	    goes->wind_direction[wind_direction_indx] = 
	      last_iru_data.wind_direction;
	    wind_direction_indx++;
	    if(wind_direction_indx >= 10) wind_direction_indx = 0;
	    break;

	    case NS_VELOCITY_LAB:
	    last_iru_data.ns_velocity =
	      (float)(data>>11)*(float)VKNOTS_TO_MPERSEC;
	    goes->ns_velocity[ns_velocity_indx] = last_iru_data.ns_velocity;
	    ns_velocity_indx++;
	    if(ns_velocity_indx >= 10) ns_velocity_indx = 0;
	    break;

	    case EW_VELOCITY_LAB:
	    last_iru_data.ew_velocity =
	      (float)(data>>11)*(float)VKNOTS_TO_MPERSEC;
	    goes->ew_velocity[ew_velocity_indx] = last_iru_data.ew_velocity;
	    ew_velocity_indx++;
	    if(ew_velocity_indx >= 10) ew_velocity_indx = 0;
	    break;

	    case HEADING_LAB:
	    last_iru_data.heading =
	      (float)(data>>11)*(float)PIRAD_TO_DEG + HEAD_CORR;
	    if(last_iru_data.heading < 0)
	      last_iru_data.heading += 360.0;
	    if(last_iru_data.heading > 360.0)
	      last_iru_data.heading -= 360.0;
	    goes->heading[heading_indx] = last_iru_data.heading;
	    heading_indx++;
	    if(heading_indx >= 25) heading_indx = 0;
	    break;

	    case DRIFT_LAB:
	    last_iru_data.drift =
	      (float)(data>>11)*(float)PIRAD_TO_DEG;
	    goes->drift[drift_indx] = last_iru_data.drift;
	    drift_indx++;
	    if(drift_indx >= 25) drift_indx = 0;
	    break;

	    case ALTITUDE_LAB:
	    last_iru_data.altitude =
	      (float)(data>>11)*(float)FEET_TO_KM;
	    goes->altitude[altitude_indx] = last_iru_data.altitude;
	    altitude_indx++;
	    if(altitude_indx >= 25) altitude_indx = 0;
	    break;

	    case INERTIAL_VSPEED_LAB:
	    last_iru_data.inertial_vspeed =
	      (float)(data>>11)*(float)FTPERMIN_TO_MPERSEC;
	    goes->inertial_vspeed[inertial_vspeed_indx]
	      = last_iru_data.inertial_vspeed;
	    inertial_vspeed_indx++;
	    if(inertial_vspeed_indx >= 25) inertial_vspeed_indx = 0;
	    break;

	    case PITCH_LAB:
	    last_iru_data.pitch =
	      (float)(data>>11)*(float)PIRAD_TO_DEG;
	    goes->pitch[pitch_indx] = last_iru_data.pitch;
	    pitch_indx++;
	    if(pitch_indx >= 50) pitch_indx = 0;
	    break;

	    case ROLL_LAB:
	    last_iru_data.roll =
	      (float)(data>>11)*(float)PIRAD_TO_DEG;
	    goes->roll[roll_indx] = last_iru_data.roll;
	    roll_indx++;
	    if(roll_indx >= 50) roll_indx = 0;
	    break;

	    case INTEG_VERT_ACC_LAB:
	    last_iru_data.integ_vert_acc =
	      (float)(data>>11)*(float)FTPERSEC_TO_MPERSEC;
	    goes->integ_vert_acc[integ_vert_acc_indx]
	      = last_iru_data.integ_vert_acc;
	    integ_vert_acc_indx++;
	    if(integ_vert_acc_indx >= 50) integ_vert_acc_indx = 0;
	    break;

	    case VERT_ACC_LAB:
	    last_iru_data.vert_acc =
	      (float)(data>>11)*(float)GS_TO_MPERSEC2;
	    goes->vert_acc[vert_acc_indx] = last_iru_data.vert_acc;

	    /* Vertical acceleration is the last 50 Hertz data value, that is
	       used, that comes out on the ARINC 429 bus.  Therefore we can
	       assume that all of the values in the last_iru_data structure
	       are the lastest and greatest. Also, they are back in time by
	       20 miliseconds times the number of 50 hertz places back from
	       our current time.  For example, if this is the third vertical
	       acceleration label found this interrupt (i.e. vert_acc_indx =
	       2, 12, 22, 32 or 42) then the time of the current data is:
	       msecs_today -(9-(vert_acc_indx modulo 10)) * 20.

	       We need to calculate the time of the current data and then
	       place that data into the correct platform info block */

	    msecs_this_data = msecs_today - (9 - (vert_acc_indx % 10))
	      * 20 - IRU_LATENCY;
	    fill_platform(msecs_this_data);

	    vert_acc_indx++;
	    if(vert_acc_indx >= 50) vert_acc_indx = 0;
	    break;

	    case TRACK_RATE_LAB:
	    last_iru_data.track_rate =
	      (float)(data>>11)*(float)TRATE_SCALE;
	    goes->track_rate[track_rate_indx] = last_iru_data.track_rate;
	    track_rate_indx++;
	    if(track_rate_indx >= 50) track_rate_indx = 0;
	    break;

	    case PITCH_RATE_LAB:
	    last_iru_data.pitch_rate =
	      (float)(data>>11)*(float)PRATE_SCALE;
	    goes->pitch_rate[pitch_rate_indx] = last_iru_data.pitch_rate;
	    pitch_rate_indx++;
	    if(pitch_rate_indx >= 50) pitch_rate_indx = 0;
	    break;

	  default:
	    break;

	} /* Switch on label */
  } /* For loop to num_words */

/* Determine that all like indexes are equal */

test = msec_longitude_indx | status_indx | latitude_indx | longitude_indx;
if(test != msec_longitude_indx) currStatus->iru |= IRU_WORDS_BAD;
else currStatus->iru &= (char)(~IRU_WORDS_BAD);

test = wind_speed_indx | wind_direction_indx | ns_velocity_indx |
  ew_velocity_indx;
if(test != wind_speed_indx) currStatus->iru |= IRU_WORDS_BAD;
else currStatus->iru &= (char)(~IRU_WORDS_BAD);

test = heading_indx | drift_indx | altitude_indx | inertial_vspeed_indx;
if(test != heading_indx) currStatus->iru |= IRU_WORDS_BAD;
else currStatus->iru &= (char)(~IRU_WORDS_BAD);

test = pitch_indx | roll_indx | integ_vert_acc_indx | vert_acc_indx |
  track_rate_indx | pitch_rate_indx;
if(test != pitch_indx) currStatus->iru |= IRU_WORDS_BAD;
else currStatus->iru &= (char)(~IRU_WORDS_BAD);

/* If a buffer is complete, msec_longitude_indx will be zero */

if(msec_longitude_indx == 0)
  return(1);
else
  return(0);
}
