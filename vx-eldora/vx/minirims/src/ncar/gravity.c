/*+**********************************************************************

      NAME: gravity.c

  SYNOPSIS: Implements Tucker's parametric gravity model thru J2

    INPUTS:

PROCESSING:  	This routine combines the functionality of the old
		navigator routines: earthgeo(), grvty(), and grvsub().
		A set of exact equations for calculating the north and down
		components of gravity replace the approximations formerly
		used to calculate these parameters.
		WGS-72 Earth Model Parameters are initialized in initnav().


   OUTPUTS:  North and down components of gravity.
	     Various earth geographic parameters.
	     Various earth radii parameters.
	

   RETURNS:  None.

	By:  Earl Creel   Sep 23, 1988

**********************************************************************+*/

#define NO_MINIR_SCOPE
#include "minirGbl.h"
#include "math.h"

  double gxc, gzc;
  double gne, gde;

struct earth {
  double req, ome, ome2, flat, eccen2, gm, rj2;
  } erth;

void
gravity()
{
  double gtmp1, gtmp2, gtmp3;
  double rlat, rho, rltc, sltc, cltc;
  double rcpa, sdl, cdl;

  register NAV_VARS *n;
  register struct earth *e;

  n = nav_ptr;
  e = &erth;

/* old earthgeo() calculations */
  n->snlt = sin(n->lg);
  n->cslt = cos(n->lg);

  n->tnlt = n->snlt / n->cslt;
  n->snlt2 = n->snlt * n->snlt;
  n->cslt2 = n->cslt * n->cslt;

  gtmp1 = 1.0/(1.0 - e->eccen2 * n->snlt2);
  n->rlng = e->req * sqrt(gtmp1);

  n->omeclt = e->ome * n->cslt;
  n->omeslt = e->ome * n->snlt;
/* end of old erthgeo() calcs */

/* old grvsub() calculations */
  n->rlgpa = n->rlng + n->alt;

/* end of old grvsub() calcs */


/* old grvty() calculations */
  rlat = (1.0- e->eccen2) * n->rlng * gtmp1;
  n->rltpa = rlat + n->alt;

  rho = 1.0 - e->eccen2 * n->rlng / n->rlgpa;
  rltc = atan(rho * n->tnlt);
  sltc = sin(rltc);
  cltc = cos(rltc);
  rcpa = n->rlgpa * n->cslt / cltc;

  gtmp1 = e->gm/(rcpa*rcpa);
  gtmp2 = (e->req/rcpa)*(e->req/rcpa);
  gtmp3 = e->ome2 * rcpa * cltc;

  gxc = -3.0 * e->rj2 * gtmp1 * gtmp2 * cltc * sltc;
  gzc = gtmp1*(1.0-1.5*e->rj2*gtmp2*(3.0*sltc*sltc-1.0) );

  sdl = sin(n->lg - rltc);
  cdl = cos(n->lg - rltc);

  gne =  cdl * gxc + sdl * gzc;
  gde = -sdl * gxc + cdl* gzc;

  /* North gravitational acceleration component	*/
  n->gnp = (gne - gtmp3 * n->snlt) * n->dtv;
	

  /* Down gravitational acceleration component	*/
  n->gdp = (gde - gtmp3 * n->cslt) * n->dtv;
	
	
/* end of old grvty() calcs */

}
