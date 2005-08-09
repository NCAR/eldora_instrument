/****************************************************************
* dcmpi.c							*
*****************************************************************

Description:  This routine is used to update the direction cosine
	      matrix which transforms from body to the target inertial
	      frame using the s matrix terms calculated in dircos just
	      before this routine is executed.

inputs:	      s matrix values stored in global memory.
outputs:      updated direction cosine matrix elements tij

notes:	      none.

called routines:  none

*****************************************************************/

/* Last Modified:  4/10/87    BY:  E.J. Nava, 9131		*/

#include "navvars.h"

dcmpi()
{
register struct nav_vars *n;
double t11up,t12up,t13up,t21up,t22up,t23up,t31up,t32up,t33up;
extern double s11,s12,s13,s21,s22,s23,s31,s32,s33;

n = (struct nav_vars *) NAV_start;

t11up = n->t11*s11 + n->t12*s21 + n->t13*s31 + n->t11;
t12up = n->t11*s12 + n->t12*s22 + n->t13*s32 + n->t12;
t13up = n->t11*s13 + n->t12*s23 + n->t13*s33 + n->t13;
t21up = n->t21*s11 + n->t22*s21 + n->t23*s31 + n->t21;
t22up = n->t21*s12 + n->t22*s22 + n->t23*s32 + n->t22;
t23up = n->t21*s13 + n->t22*s23 + n->t23*s33 + n->t23;
t31up = n->t31*s11 + n->t32*s21 + n->t33*s31 + n->t31;
t32up = n->t31*s12 + n->t32*s22 + n->t33*s32 + n->t32;
t33up = n->t31*s13 + n->t32*s23 + n->t33*s33 + n->t33;

n->t11 = t11up;
n->t12 = t12up;
n->t13 = t13up;
n->t21 = t21up;
n->t22 = t22up;
n->t23 = t23up;
n->t31 = t31up;
n->t32 = t32up;
n->t33 = t33up;

}

