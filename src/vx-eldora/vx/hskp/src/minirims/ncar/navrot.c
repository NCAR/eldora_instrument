/****************************************************************
* navrot.c							*
*****************************************************************

Description:  This routine is used to update the direction cosine
	      matrix which transforms from IMU to local level
	      coordinates as a function of earth and craft rates as
	      calculated in erthnav.c

inputs:	      local level roll rates stored in the nav data structure.
outputs:      updated aij elements of the DCM

notes:	      This routine implements the following function:

		 L(n+1)   L(n+1)  L(n)
		C      = C       C
		 B(m)     L(n)    B(m)

called routines:  none

*****************************************************************/

/* Last Modified:  1/5/88    BY:  J.R. Fellerhoff, 9131		*/

#define NO_MINIR_SCOPE
#include "minirGbl.h"
#include "math.h"

#define K6 0.16666667		/* constant = 1/6   = 1/3!	*/
#define K24 0.0416666667	/* constant = 1/24  = 1/4!	*/
#define K120 0.0083333333	/* constant = 1/120 = 1/5!	*/
#define K720 0.0013888889	/* constant = 1/720 = 1/6!	*/

#define ANG_SWITCH 0.01         /* 0.1 radians squared          */

navrot()
{
double wn,we,wd;
double dnde,dndd,dedd;
double dphn2,dphe2,dphd2;
double dph,dph2,dph4,alf,bet;
double s11,s12,s13,s21,s22,s23,s31,s32,s33;
double a11up,a12up,a13up,a21up,a22up,a23up,a31up,a32up,a33up;

register NAV_VARS *n;

n = nav_ptr;

/* coordinate system rotations due to craft and earth rate - calculated	*/
/* in the erthnav() routine.						*/

dphn2 = n->dthn * n->dthn;
dphe2 = n->dthe * n->dthe;
dphd2 = n->dthd * n->dthd;

dph2 = dphn2 + dphe2 + dphd2;
dph4 = dph2 * dph2;

if ( dph2 >= ANG_SWITCH )
    {
    dph = sqrt( dph2);
    alf = ( sin(dph) / dph ) -1.0;
    bet = ( 1.0 - cos(dph) ) / dph2;
    }
else
    {
    alf = -dph2 * K6 + dph4 * K120;
    bet = 0.5 - dph2 * K24 + dph4 * K720;
    }
wn = alf * n->dthn + n->dthn;
we = alf * n->dthe + n->dthe;
wd = alf * n->dthd + n->dthd;

dnde = n->dthn * n->dthe * bet;
dndd = n->dthn * n->dthd * bet;
dedd = n->dthe * n->dthd * bet;

s11 = -bet * (dphd2 + dphe2);
s22 = -bet * (dphd2 + dphn2);
s33 = -bet * (dphe2 + dphn2);

s12 = -wd + dnde;
s13 = we + dndd;
s21 = wd + dnde;
s23 = -wn + dedd;
s31 = -we + dndd;
s32 = wn + dedd;

a11up = s11*n->a11 + s12*n->a21 + s13*n->a31 + n->a11;
a12up = s11*n->a12 + s12*n->a22 + s13*n->a32 + n->a12;
a13up = s11*n->a13 + s12*n->a23 + s13*n->a33 + n->a13;
a21up = s21*n->a11 + s22*n->a21 + s23*n->a31 + n->a21;
a22up = s21*n->a12 + s22*n->a22 + s23*n->a32 + n->a22;
a23up = s21*n->a13 + s22*n->a23 + s23*n->a33 + n->a23;
a31up = s31*n->a11 + s32*n->a21 + s33*n->a31 + n->a31;
a32up = s31*n->a12 + s32*n->a22 + s33*n->a32 + n->a32;
a33up = s31*n->a13 + s32*n->a23 + s33*n->a33 + n->a33;

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
