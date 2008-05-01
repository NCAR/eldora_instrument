/*
 *	$Id$
 *
 *	Module:		 Aircraft.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:
 *        
 */
#ifndef INCAircrafth
#define INCAircrafth

#ifdef OK_RPC

#ifdef UNIX
#include <rpc/rpc.h>
#else
#include "rpc/rpc.h"
#endif /* UNIX */

#endif /* OK_RPC */

struct aircraft_i {
    char  aircraft_info[4];	/* Identifier for the aircraft/ship */
				/* parameters block (ascii characters ASIB) */
    short aircraft_info_length; /* Length in Bytes of the */
				/* aircraft/ship arameters block */
    short padding1;
    float longitude;		/* Antenna Longitude (Eastern */
				/* Hemisphere is positive, West */
				/* negative) in degrees */
    float latitude;		/* Antenna Latitude (Northern */
				/* Hemisphere is positive, South */
				/* Negative) in degrees */
    float altitude_msl;		/* Antenna Altitude above mean sea */
				/* level (MSL) in m */
    float altitude_agl;		/* Antenna Altitude above ground level */
				/* (AGL) in m */
    float ew_velocity;		/* Antenna east-west ground speed */
				/* (towards East is positive) in m/sec */
    float ns_velocity;		/* Antenna north-south ground speed */
				/* (towards North is positive) in m/sec */
    float vert_velocity;	/* Antenna vertical velocity (Up is */
				/* positive) in m/sec */
    float heading;		/* Antenna heading (angle between */
				/* rotodome rotational axis and true */
				/* North, clockwise (looking down) */
				/* positive) in degrees */
    float roll;			/* Roll angle of aircraft tail section
				/* (Horizontal zero, Positive left wing up) */
				/* in degrees */
    float pitch;		/* Pitch angle of rotodome (Horizontal */
				/* is zero positive front up) in degrees*/
    float drift_angle;		/* Antenna drift Angle. (angle between
				/* aircraft true velocity and heading, */
				/* positive is drift more clockwise */
				/* looking down) in degrees */
    float rotation_angle;	/* Angle of the radar beam with */
				/* respect to the airframe (zero is */
				/* along vertical stabilizer, positive */
				/* is clockwise) in deg */
    float tilt;			/* Angle of radar beam and line normal */
				/* to longitudinal axis of aircraft, */
				/* positive is towards nose of */
				/* aircraft) in degrees */
    float ew_horiz_wind;	/* east - west wind velocity at the */
				/* aircraft (towards East is positive) */
				/* in m/sec */
    float ns_horiz_wind;	/* North - South wind velocity at the */
				/* aircraft (towards North is */
				/* positive) in m/sec */
    float vert_wind;		/* Vertical wind velocity at the */
				/* aircraft (up is positive) in m/sec */
    float rotation_vel;		/* Instantaneous antenna rotational */
				/* velocity with respect to airframe */
				/* (clockwise is positive) in deg/sec */
}; /* End of Structure */

#ifdef OK_RPC

typedef struct aircraft_i aircraft_i;
typedef struct aircraft_i AIRCRAFT;

bool_t xdr_aircraft_i(XDR *, AIRCRAFT *);

#endif /* OK_RPC */

#endif /* INCAircrafth */

