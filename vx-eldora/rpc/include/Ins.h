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
 *
 *
 * description:
 *        
 */
#ifndef INCInsh
#define INCInsh

#ifdef OK_RPC

#ifdef UNIX
#include <rpc/rpc.h>
#else
#include "rpc/rpc.h"
#endif /* UNIX */

#endif /* OK_RPC */

struct ins_data {
    char  ins_data_id[4];	/* Identifier = INSD. */
    long  ins_data_len;		/* Block size in bytes. */
    short msec_latitude;	/* Millesec. into current sec. */
    short status[5];		/* TBD status for each 5 Hz block. */
    float latitude[5];		/* Degrees. */
    float longitude[5];		/* Ditto. */
    float wind_speed[10];	/* m/s. */
    float wind_direction[10];	/* o = north, positive is clockwise. */
    float ns_velocity[10];	/* Toward north is psotive, m/s. */
    float ew_velocity[10];	/* Toward east is positive, m/s. */
    float heading[25];		/* 0 = north, positive clockwise, degrees. */
    float drift[25];		/* Degrees. */
    float altitude[25];		/* Km above msl. */
    float inertial_vspeed[25];	/* Inertial vertical speed, m/s. */
    float pitch[50];		/* Degrees. */
    float roll[50];		/* Ditto. */
    float long_accel[50];	/* Longitudinal acceleration, positive */
				/* forward, m/s/s. */
    float lateral_accel;	/* Right is positive. */
    float normal_accel[50];	/* Up is positive. */
    float track_rate[50];	/* Change rate of track angle, */
				/* clockwise positive, degrees/s. */
    float pitch_rate[50];	/* Change rate of pitch angle, */
				/* positive nose up, degrees/s. */
};

#ifdef OK_RPC

typedef struct ins_data ins_data;
typedef struct ins_data INS_DATA;

extern bool_t xdr_ins_data(XDR *, ins_data *);

#endif /* OK_RPC */
#endif /* INCInsh */

