/****************************************************************
* dircos.c							*
*****************************************************************

Description:  This routine is used to update the direction cosine
	      matrix which transforms from body to the local level
	      frame based only on the rotations measured by the
	      gyros.

inputs:	      dph_ incremental angles calculated in gcomp stored
	      in the nav data structure.
outputs:      updated direction cosine matrix elements aij

notes:	      implements the following transformation:

		 L(n)     L(n)    B(m)
		C      = C      C
		 B(m+1)   B(m)    B(m+1)

called routines:  none

*****************************************************************/

/* Last Modified:  3/23/87    BY:  E.J. Nava, 9131		*/

#include "navvars.h"
#include "math.h"

#define K6 0.16666667		/* constant = 1/6    = 1/3!	*/
#define K24 0.0416666667	/* constant = 1/24   = 1/4!	*/
#define K120 0.0083333333	/* constant = 1/120  = 1/5!	*/
#define K720 0.0013888889	/* constant = 1/720  = 1/6!	*/

double s11,s12,s13,s21,s22,s23,s31,s32,s33;

dircos()
{
register struct nav_vars *n;
double dphx2,dphy2,dphz2;
double drwx,drwy,drwz,dxdy,dxdz,dydz;
double dph2,dph4,alf,bet;
double a11up,a12up,a13up,a21up,a22up,a23up,a31up,a32up,a33up;

n = (struct nav_vars *) NAV_start;

/* based on compensated rotations calculated in gcomp()		*/

dphx2 = n->dphx * n->dphx;
dphy2 = n->dphy * n->dphy;
dphz2 = n->dphz * n->dphz;
dph2 = dphx2 + dphy2 + dphz2;
dph4 = dph2 * dph2;

alf = -dph2 * K6 + dph4 * K120;
bet = 0.5 - dph2 * K24 + dph4 * K720;

drwx = alf * n->dphx + n->dphx;
drwy = alf * n->dphy + n->dphy;
drwz = alf * n->dphz + n->dphz;

dxdy = n->dphx * n->dphy * bet;
dxdz = n->dphx * n->dphz * bet;
dydz = n->dphy * n->dphz * bet;

s11 = -bet * (dphz2 + dphy2);
s22 = -bet * (dphz2 + dphx2);
s33 = -bet * (dphy2 + dphx2);
s12 = -drwz + dxdy;
s13 = drwy + dxdz;
s21 = drwz + dxdy;
s23 = -drwx + dydz;
s31 = -drwy + dxdz;
s32 = drwx + dydz;

a11up = n->a11*s11 + n->a12*s21 + n->a13*s31 + n->a11;
a12up = n->a11*s12 + n->a12*s22 + n->a13*s32 + n->a12;
a13up = n->a11*s13 + n->a12*s23 + n->a13*s33 + n->a13;
a21up = n->a21*s11 + n->a22*s21 + n->a23*s31 + n->a21;
a22up = n->a21*s12 + n->a22*s22 + n->a23*s32 + n->a22;
a23up = n->a21*s13 + n->a22*s23 + n->a23*s33 + n->a23;
a31up = n->a31*s11 + n->a32*s21 + n->a33*s31 + n->a31;
a32up = n->a31*s12 + n->a32*s22 + n->a33*s32 + n->a32;
a33up = n->a31*s13 + n->a32*s23 + n->a33*s33 + n->a33;

n->a11 = a11up;
n->a12 = a12up;
n->a13 = a13up;
n->a21 = a21up;
n->a22 = a22up;
n->a23 = a23up;
n->a31 = a31up;
n->a32 = a32up;
n->a33 = a33up;

}

