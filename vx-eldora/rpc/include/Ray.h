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
    short ray_info_length;	/* length of a data ray info block in */
				/* bytes. */
    short padding1;
    long  sweep_num;		/* sweep number for this radar. */
    short hour;			/* Hour in hours. */
    short minute;		/* Minute in minutes. */
    short second;		/* Second in seconds. */
    short millisecond;		/* Millisecond in milliseconds. */
    float azimuth;		/* Azimuth in degrees. */
    float elevation;		/* Elevation in degrees. */
    float peak_power;		/* Last measured peak transmitted */
				/* power in kw. */
}; /* End of Structure */


#ifdef OK_RPC

typedef struct ray_i ray_i;
typedef struct ray_i RAY;

bool_t xdr_ray_i(XDR *, RAY *);

#endif /* OK_RPC */

#endif /* INCRayh */

