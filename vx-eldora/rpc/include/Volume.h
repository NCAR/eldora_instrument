/*
 *	$Id$
 *
 *	Module:		 Volume.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1991/08/30  18:39:40  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCVolumeh
#define INCVolumeh

#ifdef OK_RPC

#ifdef UNIX
#include <rpc/rpc.h>
#else
#include "rpc/rpc.h"
#endif /* UNIX */

#endif /* OK_RPC */

struct volume_d {
    char  volume_des[4];	/* Volume descriptor identifier: ASCII */
				/* characters "VOLD" stand for Volume */
				/* Descriptor. */
    short volume_des_length;	/* Volume descriptor length in bytes. */
    short  format_version;	/* ELDORA/ASTRAEA field tape format */
				/* revision number. */
    short volume_num;		/* Volume Number into current tape. */
    short padding1;
    long  maximum_bytes;	/* Maximum number of bytes in any. */
				/* physical record in this volume. */
    long  number_sensor_des;	/* Total Number of sensor descriptors */
				/* that follow. */
    char  proj_name[8];		/* Project number or name. */
    short year;			/* Year data taken in years. */
    short month;		/* Month data taken in months. */
    short day;			/* Day data taken in days. */
    short data_set_hour;	/* hour data taken in hours. */
    short data_set_minute;	/* minute data taken in minutes. */
    short data_set_second;	/* second data taken in seconds. */
    char  flight_num[8];	/* Flight number. */
    short gen_year;		/* year this recording was generated */
				/* in years. */
    short gen_month;		/* month this recording was generated */
				/* in months. */
    short gen_day;		/* day this recording was generated in days. */
    short padding2;
    char  gen_facility[8];	/* identifier of facility that */
				/* generated this recording. */
    short data_compress;	/* Data compression scheme in use. */
    short padding3;
}; /* End of Structure */


#ifdef OK_RPC

typedef struct volume_d volume_d;
typedef struct volume_d VOLUME;

bool_t xdr_volume_d(XDR *, VOLUME *);

#endif /* OK_RPC */

#endif /* INCVolumeh */

