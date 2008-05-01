/*
 *	$Id$
 *
 *	Module:		 Ins.h
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.5  1992/10/02  18:15:52  craig
 * made time base on longitude (as this is last 5 hertz variable across)
 *
 * Revision 1.4  1992/09/25  15:19:21  craig
 * added sec_latitude so block does not have to line up perfectly
 *
 * Revision 1.3  1992/09/23  21:50:22  craig
 * Added integrated vertical accel and vertical accel, removed body accelerations
 *
 * Revision 1.2  1992/08/11  13:47:43  thor
 * Fixed incorrect declaration of msec_latitude & lateral_accel.
 *
 * Revision 1.1  1992/07/28  17:23:46  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCInsh
#define INCInsh

struct ins_data {
    char  ins_data_id[4];	/* Identifier = INSD. */
    long  ins_data_len;		/* Block size in bytes. */
    short sec_longitude;        /* Second in which longitude was found */
    short msec_longitude;	/* Millesec. into current sec. */
    short status;		/* TBD status for each 5 Hz block. */
    float latitude;		/* Degrees. */
    float longitude;		/* Ditto. */
    float wind_speed;	        /* m/s. */
    float wind_direction;	/* o = north, positive is clockwise. */
    float ns_velocity;	        /* Toward north is psotive, m/s. */
    float ew_velocity;	        /* Toward east is positive, m/s. */
    float heading;		/* 0 = north, positive clockwise, degrees. */
    float drift;		/* Degrees. */
    float altitude;		/* Km above msl. */
    float inertial_vspeed;	/* Inertial vertical speed, m/s. */
    float pitch;		/* Degrees. */
    float roll;		        /* Ditto. */
    float integ_vert_acc;	/* Integrated vertical Acceleration (m/s) */
    float vert_acc;	        /* Veritcal Acceleration (m/s/s) */
    float track_rate;	        /* Change rate of track angle, */
				/* clockwise positive, degrees/s. */
    float pitch_rate;	        /* Change rate of pitch angle, */
				/* positive nose up, degrees/s. */
};

typedef struct ins_data ins_data;
typedef struct ins_data INS_DATA;

#endif /* INCInsh */

