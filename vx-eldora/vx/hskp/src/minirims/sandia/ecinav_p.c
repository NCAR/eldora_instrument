/*+**********************************************************************

      NAME: ecinav_pos.c

  SYNOPSIS: Calculates ECI frame position branch navigation parameters.

    INPUTS: T matrix from nav data structure.
	    Geographic latitude, terrestrial longitude, geographic altitude,
	      from the nav data structure.
	    Platform velocity w.r.t. earth coordinitized in the
	      geographic frame, from the nav data structure.
	    Time after lift off, from the nav data structure.
	    rlgpa (ref. grvsub.c), omeclt (ref. erthgeo.c), and rltpai
	      (ref erthnav.c); all from the nav data structure.
		
PROCESSING:  ECI velocity calculations:
                                             _              _
	 ECI     ECI    G            G      |       0        |    G
	V    =  T    * V       ;    V    =  | omeclt * rlgpa | + V      ;
	 i,p     G      i,p          i,p    |_      0       _|    E,p
    		  _			                              -
 	 ECI     |  -sin(Lg)cos(Lam)    -sin(Lam)     -cos(Lg)cos(Lam) |
	T  =     |  -sin(Lg)sin(Lam)     cos(Lam)     -cos(Lg)sin(Lam) |
	 G       |_   cos(Lg)                0         -sin(Lg)       _|

	where:
		Lg   =  geographic latitude
		Lam  =  terrestrial longitude + (earth rate)(talo) =
			  celestial longitude
		  G
		V    = platform velocity w.r.t. earth, coordinitized in
		 E,p     the geographic frame (from nav data structure).

      ------------------------------------------------------------------
         ECI Position calculations:
			   _                           _
		  ECI	  |  rlgpa * cos(Lg) * cos(Lam) |
		 P  =     |  rlgpa * cos(Lg) * sin(Lam) |
 		          |_  (1/rltpai) * sin(Lg)      _|

      ------------------------------------------------------------------
         First Integral of Specific force calculations:
				         _       _        _      _
 		  ECI           ECI     |   dvxi  |      |  dv_x  |
            FI( sf   ) = SUM(  T     *  |   dvyi  | ) =  |  dv_y  |
			        P       |_  dvzi _|      |_ dv_z _|

		where:
			dvxi, dvyi, dvzi are incremental velocities
			  calculated in acomp.c.
			FI denotes first integral.
			
      ------------------------------------------------------------------
   OUTPUTS:  ECI position.
  	     ECI velocity.
	     First integral of specific force from launch, ECI frame.

   RETURNS:  None.

	By:  Earl Creel   May 18, 1988

**********************************************************************+*/

#include "navvars.h"
#include "math.h"
#include "ecinav_v.h"

extern struct earth
      {
        double req, ome, ome2, flat, eccen2, gm, rj2;
      } erth;

void
ecinav_pos()
{
  double Lam, cLam, sLam;
  double sLat, cLat;
  double dvx1;
  double dvy1;
  double dvz1;
  double temp;
  double rg;

  register struct nav_vars *n;
  register struct ecinav_vars *p_eci;
  register struct earth *e;

  n = (struct nav_vars *) NAV_start;
  p_eci = &eci;
  e = &erth;

  /* Computations for ECI Velocity and ECI Position Vectors */

  /* First compute celestial Longitude using terrestrial Longitude and OMEGA */
  Lam = n->lng + e->ome * ( ( (double)n->talo )/( (double)n->onesec) );

  cLam = cos(Lam);	  sLam = sin(Lam);

  sLat = sin(n->lg);	  cLat = cos(n->lg);

  /* 3 components of ECI velocity */
  temp = (e->ome*cLat*n->rlgpa + n->ve);

  p_eci->vxeci = -sLat*cLam*n->vn - sLam*temp - cLat*cLam*n->vd;

  p_eci->vyeci = -sLam*sLat*n->vn + cLam*temp - cLat*sLam*n->vd;

  p_eci->vzeci = cLat*n->vn - sLat*n->vd;


  /* 3 components of ECI position */

  p_eci->pxeci = n->rlgpa*cLat*cLam;

  p_eci->pyeci = n->rlgpa*cLat*sLam;

  rg = e->req / sqrt(1.0 - e->eccen2 * sLat * sLat);
  p_eci->pzeci = ((1.0 - e->eccen2) * rg + n->alt ) * sLat;


  /* ======================================= */
  /* specific force calculations (ECI frame) */
  /* ======================================= */

  /* Calculate new incremental velocities */
  dvx1 = n->t11*n->dvxi + n->t12*n->dvyi + n->t13*n->dvzi;
  dvy1 = n->t21*n->dvxi + n->t22*n->dvyi + n->t23*n->dvzi;
  dvz1 = n->t31*n->dvxi + n->t32*n->dvyi + n->t33*n->dvzi;

  /* Accumulate incremental velocities to form First Integral */
  p_eci->dv_x += dvx1;
  p_eci->dv_y += dvy1;
  p_eci->dv_z += dvz1;

} /* End of function */
