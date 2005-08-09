/****************************************************************
* erthal.c							*
*****************************************************************

Description:  This routine is used to initialize parameters before
              the start of the second position gyrocompass.  Earth
	      rate is coupled into the nav equations based on the
	      estimate of the heading at that point in time.

inputs:	      estimated heading angle parameters salf and calf from
	      the nav data structure.
outputs:      Reset velocities and geographic coordinates and earth
	      rate terms dthn, dthe, and dthd.

notes:	      none

called routines:  gravity();

*****************************************************************/

/* Last Modified:  3/18/87    BY:  E.J. Nava, 9131		*/

#define NO_MINIR_SCOPE
#include "minirGbl.h"
#include "math.h"

erthal()
{
register NAV_VARS *n;
void gravity();

n = nav_ptr;

n->lg = n->lgin;
n->lng = n->lngin;
n->alt = n->altin;

gravity();

n->dvnn = 0.0;
n->dven = 0.0;
n->dvdn = 0.0;

n->dthd = n->omeslt * n->dtv;
n->dthn = -n->calf * n->omeclt * n->dtv;
n->dthe = n->salf * n->omeclt * n->dtv;

}
