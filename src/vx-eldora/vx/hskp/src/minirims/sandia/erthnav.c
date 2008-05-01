/****************************************************************
* erthnav.c							*
*****************************************************************

Description:  This routine calculates the local level rotations
	      as a function of craft position, velocity, and earth
	      rate.

inputs:	      vehicle position and velocity data stored in the
	      nav data structure.
outputs:      angular changes referred to local level axes: dthn,
	      dthe, and dthd.

notes:	      call gravity before this routine

called routines:

*****************************************************************/

/* Last Modified:  10/30/87    BY:  E.J. Nava, 9131		*/
/* Modified to support navrot execution at 64 hz		*/

#include "navvars.h"

/* this routine is used to calculate the earth and craft */
/* rate angular conrtibutions */

erthnav()
{
register struct nav_vars *n;
double vedr,dthnn,dthen,dthdn;
extern void gravity();

n = (struct nav_vars *)NAV_start;

gravity();

vedr = n->ve / n->rlgpa;

dthnn = vedr + 2.0 * n->omeclt;
dthen = -n->vn / n->rltpa;
dthdn = -(vedr * n->tnlt + 2.0 * n->omeslt);

/* Coriolis compensation terms for velocity equations in nealt()	*/

n->dvnn = (-n->vd * dthen + n->ve * dthdn ) * n->dtv;
n->dven = (-n->vn * dthdn + n->vd * dthnn ) * n->dtv;
n->dvdn = (-n->ve * dthnn + n->vn * dthen ) * n->dtv;

/* earth and craft rate terms for use in navrot() to adjust the DCM	*/
/* used for body to local level transformations - a matrix		*/

n->dthn = -(dthnn - n->omeclt) * n->dtv;
n->dthe = -(dthen) * n->dtv;
n->dthd = -(dthdn + n->omeslt) * n->dtv;

}
