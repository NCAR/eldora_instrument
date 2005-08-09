/*+**********************************************************************

      NAME: ecinav_att.c

  SYNOPSIS: Computes the transformation matrix from the missile body frame
	      to the ECI frame.

    INPUTS: T matrix and resolver angle from the nav data structure.

PROCESSING:  This routine which is invoked in NAV mode by navtask()
             at the bottom of the attitude branch calculations, effects the
             following calculations:
						     _              _
		  ECI     ECI     p  		    | 1    0    0    |
		 T    =  T    * T           ( = T * | 0  cphi -sphi  | )
		  body    p      body		    |_0  sphi  cphi _|

		where cphi and sphi denote cos and sin of resolver angle.
		
   OUTPUTS:  ebij matrix defined in ecinav_vars.c

   RETURNS:  None.

	By:  Earl Creel   May 17, 1988

**********************************************************************+*/

#include "navvars.h"
#include "math.h"
#include "ecinav_v.h"

void
ecinav_att()
{
  double cphi, sphi;
  register struct nav_vars *n;
  register struct ecinav_vars *p_eci;

  n = (struct nav_vars *) NAV_start;
  p_eci = &eci;

  /* compute sin and cos of platform resolver angle */
  sphi = sin(n->rmrd);
  cphi = cos(n->rmrd);

  p_eci->eb11 = n->t11;
  p_eci->eb12 = n->t12*cphi + n->t13*sphi;
  p_eci->eb13 = n->t13*cphi - n->t12*sphi;

  p_eci->eb21 = n->t21;
  p_eci->eb22 = n->t22*cphi + n->t23*sphi;
  p_eci->eb23 = n->t23*cphi - n->t22*sphi;

  p_eci->eb31 = n->t31;
  p_eci->eb32 = n->t32*cphi + n->t33*sphi;
  p_eci->eb33 = n->t33*cphi - n->t32*sphi;

} /* end of routine */
