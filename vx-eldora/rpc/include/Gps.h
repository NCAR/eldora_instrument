/*
 *	$Id$
 *
 *	Module:		 Gps.h
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/07/28  17:23:46  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCGpsh
#define INCGpsh

#ifdef OK_RPC

#ifdef UNIX
#include <rpc/rpc.h>
#else
#include "rpc/rpc.h"
#endif /* UNIX */

#endif /* OK_RPC */

struct gps_data {
    char  gps_data_id[4];	/* Identifier = GPSD. */
    long  gps_data_len;		/* Block size in bytes. */
    short msec_fix;		/* Millesecnds of last fix. */
    short week_number;		/* GPS week number. */
    long  secs_of_week;		/* Seconds into week. */
    long  utc_offset;		/* Offset from UTC. */
    long  gps_mode;		/* Defined by Trimbal. */
    short satellite[4];		/* Satellites being used. */
    long  pdop;
    long  hdop;
    long  vdop;
    long  tdop;
    short software_version;	/* Trimbal software version. */
    short month;
    short day;
    short year;
    short health;		/* Defined by Trimbal. */
    short machine_id;		/* Ditto. */
    float latitude;		/* + = north, - = south. */
    float longitude;		/* + = east, - = west */
    float altitude;		/* Km above msl. */
    float clock_bias;		/* m. */
    float time_of_fix;		/* Time of fix in GPS seconds. */
};

#ifdef OK_RPC

typedef struct gps_data gps_data;
typedef struct gps_data GPS_DATA;

extern bool_t xdr_gps_data(XDR *, gps_data *);

#endif /* OK_RPC */
#endif /* INCGpsh */

