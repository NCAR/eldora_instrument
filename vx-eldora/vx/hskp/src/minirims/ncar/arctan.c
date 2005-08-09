/*+ DEC/CMS REPLACEMENT HISTORY, Element ARCTAN.C+*/
/*+ *1     7-MAR-1988 15:42:24 JRFELLE ""+*/
/*+ DEC/CMS REPLACEMENT HISTORY, Element ARCTAN.C+*/
/****************************************************************
* arctan.c							*
*****************************************************************

<<<<<<<<<<<<<<<<<<<<<<<<<< N O T E >>>>>>>>>>>>>>>>>>>>>>>>>>>>

This file is under configuration control.  Any changes to this routine
must be noted in the configuration control log.

<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


Description:  this routine is used to calculate the arctangent
	      of an angle based on the sine and cosine of the
	      angle.  The routine returns a value from -pi to
	      +pi.  This routine checks for a zero value of
	      cosine and responds accordingly.

inputs:	      sine and cosine of the angle (-1 to +1).
outputs:      4 quadrant arctangent of the angle. (-pi to +pi)

notes:	      none

called routines:  atan()

*****************************************************************/

/* Last Modified:  8/31/87    BY:  E.J. Nava, 9131		*/

/* Modified to correct result to be +/- Pi verses +3/2Pi -Pi/2	*/
/* Modified to correct varible type declarations		*/

#include "math.h"

#define PID2 1.570796
#define PI 3.141593

double arctan (sinx, cosx)
double sinx,cosx;
{
double result, argmnt;
double atan();

if(cosx == 0.0)
    {
    if (sinx > 0.0)
        result = PID2;
    if (sinx < 0.0)
        result = -PID2;
    if (sinx == 0.0)
        result = 0.0;
    }
else
    {
    argmnt = sinx / cosx;

    if (((sinx >= 0.0) && (cosx > 0.0)) || ((sinx < 0.0) && (cosx > 0.0)))
        result = atan (argmnt);     /* 1st and 4th quadrants */
    else
	{
	if (argmnt < 0.0)		/* angle in range pi/2 to pi	*/
	    result = PI + atan(argmnt);	/* negative angle from atan	*/
	else
	    result = atan(argmnt) - PI;	/* angle in range pi to 3/2pi	*/
	}				/* Positive angle from atan	*/
    }
return(result);
}
