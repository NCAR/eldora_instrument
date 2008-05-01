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
 * Revision 1.4  1992/07/28  17:33:03  thor
 * Added ray_status.
 *
 * Revision 1.3  1992/04/20  17:18:31  thor
 * Latest Eldora/Asterea revisions included.
 *
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
    long  ray_status;		/* 0 = normal, 1 = transition, 2 = bad. */
}; /* End of Structure */

typedef struct ray_i ray_i;
typedef struct ray_i RAY;

#endif /* INCRayh */

