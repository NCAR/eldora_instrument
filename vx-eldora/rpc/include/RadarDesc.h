/*
 *	$Id$
 *
 *	Module:		 RadarDesc.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1991/10/16  15:33:53  thor
 * Changed variable name.
 *
 * Revision 1.2  1991/10/15  17:56:24  thor
 * Fixed to meet latest version of tape spec.
 *
 * Revision 1.1  1991/08/30  18:39:36  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCRadarDesch
#define INCRadarDesch

#ifdef OK_RPC

#ifdef UNIX
#include <rpc/rpc.h>
#else
#include "rpc/rpc.h"
#endif /* UNIX */

#endif /* OK_RPC */

struct radar_d {
    char  radar_des[4];		/* Identifier for a radar descriptor */
				/* block (ascii characters "RADD"). */
    long radar_des_length;	/* Length of a radar descriptor block */
				/* in bytes. */
    char  radar_name[8];	/* Eight character radar name. */
    float radar_const;		/* Radar constant in ?? */
    float peak_power;		/* Typical peak power of the radar in kw. */
    float noise_power;		/* Typical noise power of the radar in dBm. */
    float receiver_gain;	/* Gain of the receiver in db. */
    float antenna_gain;		/* Gain of the antenna in db. */
    float system_gain;		/* Radar System Gain in db. */
    float horz_beam_width;	/* Horizontal beam width in degrees. */
    float vert_beam_width;	/* Vertical beam width in degrees. */
    short  radar_type;		/* Radar Type (0)Ground, 1)Airborne */
				/* Fore, 2)Airborne Aft, 3)airborne */
				/* Tail, 4)Airborne Lower Fuselage, */
				/* 5)Shipborne. */
    short  scan_mode;		/* Scan Mode (0)Calibration, 1)PPI */
				/* (constant Elevation) 2)Coplane, */
				/* 3)RHI (Constant Azimuth), 4) */
				/* Vertical Pointing, 5)Target */
				/* (Stationary), 6)Manual, 7)Idle (Out */
				/* of Control). */
    float req_rotat_vel;	/* Requested rotational velocity of */
				/* the antenna in degrees/sec. */
    float scan_mode_pram0;	/* Scan mode specific parameter #0 */
				/* (Has different meaning for */
				/* different scan modes). */
    float scan_mode_pram1;	/* Scan mode specific parameter #1. */
    float fixed_angle;		/* Fixed Angle that radar is */
				/* attempting to scan in degrees. */
    short  num_parameter_des;	/* Total number of parameter */
				/* descriptor blocks for this radar. */
    short  total_num_des;	/* Total number of additional */
				/* descriptor block for this radar. */
    short data_compress;	/* Data compression. 0 = none, 1 = HRD */
				/* scheme. */
    short data_reduction;	/* Data Reduction algorithm: 1 = none, */
				/* 2 = between 2 angles, 3 = Between */
				/* concentric circles, 4 = Above/below */
				/* certain altitudes.*/
    float data_red_parm0;	/* 1 = smallest positive angle in */
				/* degrees, 2 = inner circle diameter, */
				/* km, 4 = minimum altitude, km. */
    float data_red_parm1;	/* 1 = largest positve angle, degress, */
				/* 2 = outer cicle diameter, km, 4 = */
				/* maximum altitude, km. */
    float radar_longitude;	/* Longitude of radar in degrees. */
    float radar_latitude;	/* Latitude of radar in degrees. */
    float radar_altitude;	/*  Altitude of radar above msl in m. */
    float eff_unamb_vel;	/* Effective unambiguous velocity, m/s. */
    float eff_unamb_range;	/* Effective unambiguous range, km. */
    short num_freq_trans;	/* Number of frequencies transmitted. */
    short num_ipps_trans;	/* Number of different inter-pulse */
				/* periods transmitted. */
    float freq1;		/* Frequency 1. */
    float freq2;		/* Frequency 2. */
    float freq3;		/* Frequency 3. */
    float freq4;		/* Frequency 4. */
    float freq5;		/* Frequency 5. */
    float interpulse_per1;	/* Interpulse period 1. */
    float interpulse_per2;	/* Interpulse period 2. */
    float interpulse_per3;	/* Interpulse period 3. */
    float interpulse_per4;	/* Interpulse period 4. */
    float interpulse_per5;	/* Interpulse period 5. */
}; /* End of Structure */


#ifdef OK_RPC

typedef struct radar_d radar_d;
typedef struct radar_d RADARDESC;

bool_t xdr_radar_d(XDR *, RADARDESC *);

#endif /* OK_RPC */

#endif /* INCRadarDesch */
