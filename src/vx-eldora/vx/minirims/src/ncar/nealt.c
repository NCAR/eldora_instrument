/*+ DEC/CMS REPLACEMENT HISTORY, Element NEALT.C+*/
/*+ *2    25-OCT-1988 15:07:15 ECREEL "eec: upgrading for compatiblility with new gravity model"+*/
/*+ *1     7-MAR-1988 14:45:16 JRFELLE "velocity calculation routine"+*/
/*+ DEC/CMS REPLACEMENT HISTORY, Element NEALT.C+*/
/****************************************************************
* nealt.c							*
*****************************************************************

<<<<<<<<<<<<<<<<<<<<<<<<<< N O T E >>>>>>>>>>>>>>>>>>>>>>>>>>>>

This file is under configuration control.  Any changes to this routine
must be noted in the configuration control log.

<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


Description:  This routine calculates the changes in both local level
	      velocities (North, East, and Down) and geographic
	      coordinates (Latitude, Longitude, and Altitude).

inputs:	      compensated delta velocity information, attitude information
	      from the direction cosine matrix, and gravity info which
	      is stored in the nav data structure.
outputs:      updated velocities and geographic coordinates.

notes:	      none

called routines:  none

*****************************************************************/

/* Last Modified:  11/24/87    BY:  B.A. Sims, 7525		*/
/* Modified to add velocities back in                           */
/* Modified to null velocities for air bearing tests            */
/* Modified to remove vertical channel diagnostics		*/

#define NO_MINIR_SCOPE
#include "minirGbl.h"

#define PI	3.1415926536
#define TWOPI	6.2831853072

nealt()
{
register NAV_VARS *n;

n = nav_ptr;

/* update velocity terms - delta v's transformed to local level - includes */
/* coriolis and plumb bob gravity terms (with centripital accel effects)   */
/* based on calculations in erthnav().					   */

n->vn += n->a11 * n->dvxi + n->a12 * n->dvyi + n->a13 * n->dvzi + n->dvnn
	 + n->gnp;

n->ve += n->a21 * n->dvxi + n->a22 * n->dvyi + n->a23 * n->dvzi + n->dven;

n->vd += n->a31 * n->dvxi + n->a32 * n->dvyi + n->a33 * n->dvzi
	+ n->dvdn + n->gdp;


/* 		update geographic coordinates				*/

n->lg += n->vn / n->rltpa * n->dtv;

n->lng += n->ve / n->rlgpa / n->cslt * n->dtv;

n->alt -= n->vd * n->dtv;

/*   correct longitude to be within +- pi/2				*/

if(n->lng > PI)
    n->lng -= TWOPI;
else
    if (n->lng <= -PI)
	n->lng += TWOPI;

}
