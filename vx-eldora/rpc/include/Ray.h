/*
 *	$Id$
 *
 *	Module:		 Ray.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1991/10/15  17:56:43  thor
 * Fixed to meet latest version of tape spec.
 *
 * Revision 1.1  1991/08/30  18:39:38  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCRayh
#define INCRayh

#ifdef OK_RPC

#ifdef UNIX
#include <rpc/rpc.h>
#else
#include "rpc/rpc.h"
#endif /* UNIX */

#endif /* OK_RPC */

struct ray_i {
    char  ray_info[4];		/* Identifier for a data ray info. */
				/* block (ascii characters "RYIB"). */
    long ray_info_length;	/* length of a data ray info block in */
				/* bytes. */
    long  sweep_num;		/* sweep number for this radar. */
    long  julian_day;		/* Guess. */
    short hour;			/* Hour in hours. */
    short minute;		/* Minute in minutes. */
    short second;		/* Second in seconds. */
    short millisecond;		/* Millisecond in milliseconds. */
    float azimuth;		/* Azimuth in degrees. */
    float elevation;		/* Elevation in degrees. */
    float peak_power;		/* Last measured peak transmitted */
				/* power in kw. */
    float true_scan_rate;	/* Actual scan rate in degrees/second. */
}; /* End of Structure */


#ifdef OK_RPC

typedef struct ray_i ray_i;
typedef struct ray_i RAY;

bool_t xdr_ray_i(XDR *, RAY *);

#endif /* OK_RPC */

#endif /* INCRayh */

