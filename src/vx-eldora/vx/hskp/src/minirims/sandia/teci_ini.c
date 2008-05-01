/*+**************************************************************
* teci_init.c							*
*****************************************************************

Description:  This routine is used to initialize the Target-to-ECI
	      transformation matrix (T).  It is only called proir to
              launch by navtask().

inputs:	      A matrix from the nav data structure; and the TNI matrix
	      which is a 3x3 constant transformation matrix relating the
	      geographic frame at the missile launcher to the ECI frame,
              defined as a set of 9 global double floating point variables.

outputs:      initialized T matrix

notes:	      This routine implements the following transformation:

		 ECI    ECI  G
		T    = T    T     (= TNI * A)
		 P      G    P

called routines:  none

****************************************************************+*/

/* Last Modified:  5/12/88    By:  E. E. Creel 9131		      */
/* Revamp of STRYPI XI tinit() to accommodate new T matrix definition */

#include "navvars.h"
#include "ecinav_v.h"

teci_init()
{
  register struct nav_vars *n;
  register struct ecinav_vars *p_eci;

  n = (struct nav_vars *)NAV_start;
  p_eci = &eci;

  /* compute T = TNI * A */

  n->t11 = p_eci->tni11*n->a11 + p_eci->tni12*n->a21 + p_eci->tni13*n->a31;
  n->t12 = p_eci->tni11*n->a12 + p_eci->tni12*n->a22 + p_eci->tni13*n->a32;
  n->t13 = p_eci->tni11*n->a13 + p_eci->tni12*n->a23 + p_eci->tni13*n->a33;

  n->t21 = p_eci->tni21*n->a11 + p_eci->tni22*n->a21 + p_eci->tni23*n->a31;
  n->t22 = p_eci->tni21*n->a12 + p_eci->tni22*n->a22 + p_eci->tni23*n->a32;
  n->t23 = p_eci->tni21*n->a13 + p_eci->tni22*n->a23 + p_eci->tni23*n->a33;

  n->t31 = p_eci->tni31*n->a11 + p_eci->tni32*n->a21 + p_eci->tni33*n->a31;
  n->t32 = p_eci->tni31*n->a12 + p_eci->tni32*n->a22 + p_eci->tni33*n->a32;
  n->t33 = p_eci->tni31*n->a13 + p_eci->tni32*n->a23 + p_eci->tni33*n->a33;

}
