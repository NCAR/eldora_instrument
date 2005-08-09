/*+**************************************************************
* initnav.c							*
*****************************************************************


Description:  This routine is used to force the navigational variables
	      to initial values before starting a gyrocompass and
	      navigation.

inputs:	      none - parameters stored internally.
outputs:      variables in the nav data structure as shown below.

notes:	      This routine must be changed to reflect the initial
	      starting position.

called routines:

***************************************************************+*/

#include "navvars.h"
#include "ecinav_v.h"
#include "diag.h"

init_nav()
{
register struct nav_vars *n;
register struct ecinav_vars *p_eci;
struct diag_vars *d;

n = (struct nav_vars *)NAV_start;
d = (struct diag_vars *)DIAG_start;
p_eci = &eci;

/* reset launch and acs related times and flags		*/

n->talo = 0;

/* initialize geographic position coordinates	*/

n->lg = n->lgin;
n->lng = n->lngin;
n->alt = n->altin;

/* initialize gravity terms */

gravity();

/* initialize the local level velocity variables	*/

n->vn = 0.0;
n->ve = 0.0;
n->vd = 0.0;

n->dvnn = 0.0;
n->dven = 0.0;
n->dvdn = 0.0;

/* reset output discretes	*/

n->out1dis = 0;
n->out2dis = 0;

/* terms added from other routines based on condensed HOE software	*/
/* may be transferred to other routines as required.			*/

n->dvxi = 0.0;
n->dvyi = 0.0;
n->dvzi = 0.0;
n->dphxl = 0.0;
n->dphyl = 0.0;
n->dphzl = 0.0;
n->dthx2 = 0.0;
n->dthy2 = 0.0;
n->dthz2 = 0.0;


/* reset max time through loop monitor variables			*/

d->maxeven = 0;
d->maxodd = 0;

p_eci->dv_x=0.0;
p_eci->dv_y=0.0;
p_eci->dv_z=0.0;

}
