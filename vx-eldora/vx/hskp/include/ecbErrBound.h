/*
 *	$Id$
 *
 *	Module:	ecbErrBound.h	 
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/06/18  20:39:16  shawn
 * Initial revision
 *
 * description: Contains various ECB error bounds.
 *        
 */

/* DDS Bounds... */

/* Lower and Upper valid DDS frequencies */
#define ECBDDSLOWFREQ   9300000000.0 	/* Lower valid DDS frequency */
#define ECBDDSHIGHFREQ  9800000000.0	/* Upper valid DDS frequency */

/* Lower and Upper valid DDS units */
#define ECBDDSLOWUNIT   1               /* Lower valid DDS unit */
#define ECBDDSHIGHUNIT  6               /* Upper valid DDS unit */


/* IF Bounds... */

/* Lower and Upper valid filter numbers */
#define ECBIFLOWFILT    1               /* Lower valid IF filter number */
#define ECBIFHIGHFILT   5               /* Upper valid IF filter number */

/* Lower and UPper valid IF units */
#define ECBIFLOWUNIT    1               /* Lower valid IF unit */
#define ECBIFHIGHUNIT   3               /* Upper valid IF unit */

/* Attenuator Bounds... */

/* Lower and Upper valid attenuator settings (in dB) */
#define ECBATLOWATTEN   0               /* Lower valid attenuation setting */
#define ECBATHIGHATTEN  121             /* Upper valid attenuation setting */

/* Lower and Upper valid Divide-by-N values */
#define ECBATLOWN       2
#define ECBATHIGHN      255

/* Lower and Upper valid trigger mux values */
#define ECBATLOWMUX     0
#define ECBATHIGHMUX    14
#define ECBATBADMUX     7

/* Valid Attenuator Mux values */
#define ECBATMUXFORF1   0               /* Mux value for forward freq 1 */
#define ECBATMUXFORF2   1               /* Mux value for forward freq 2 */
#define ECBATMUXFORF3   2               /* Mux value for forward freq 3 */
#define ECBATMUXFORF4   3               /* Mux value for forward freq 4 */
#define ECBATMUXFORF5   4               /* Mux value for forward freq 5 */
#define ECBATMUXFORPK   5               /* Mux value for forward preknock */
#define ECBATMUXFORTP   6               /* Mux value for forward testpulse */

#define ECBATMUXAFTF1   8               /* Mux value for aft freq 1 */
#define ECBATMUXAFTF2   9               /* Mux value for aft freq 2 */
#define ECBATMUXAFTF3   10              /* Mux value for aft freq 3 */
#define ECBATMUXAFTF4   11              /* Mux value for aft freq 4 */
#define ECBATMUXAFTF5   12              /* Mux value for aft freq 5 */
#define ECBATMUXAFTPK   13              /* Mux value for aft preknock */
#define ECBATMUXAFTTP   14              /* Mux value for aft testpulse */

/* Temperature Bounds... */
#define ECBTEMPLO       15              /* lower temperature bound [C] */
#define ECBTEMPHI       40              /* upper temperature bound [C] */




