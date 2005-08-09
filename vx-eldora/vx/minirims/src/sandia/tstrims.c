/*+ DEC/CMS REPLACEMENT HISTORY, Element TSTRIMS.C+*/
/*+ *1     7-MAR-1988 14:37:46 JRFELLE "test rims routine"+*/
/*+ DEC/CMS REPLACEMENT HISTORY, Element TSTRIMS.C+*/
/****************************************************************
* tstrims.c							*
*****************************************************************

<<<<<<<<<<<<<<<<<<<<<<<<<< N O T E >>>>>>>>>>>>>>>>>>>>>>>>>>>>

This file is under configuration control.  Any changes to this routine
must be noted in the configuration control log.

<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


Description:  This routine is used to determine the x gyro, xr gyro,
	      and the x accelerometer scale factors through use of a
	      measured rotation and the known 1g field.  The
	      measured x and xr scale factors are then used to predict
	      the y and z gyro scale factors using a linear curve fit
	      whose coeficients have been determined using cal data.

	      The y and z accelerometer scale factors are calculated from
	      estimates of the x accel csle factor.

	      The newly calculated values can then be
	      used if a switch is made to the init mode.  Otherwise,
	      the last values are used.

inputs:	      raw sensor counts and the resolver position.
outputs:      gyro and accelerometer scale factors.

notes:	      The linear prediction coefficients are based stored in
	      the file [ejnava.esie]rims.h are are a function of the
	      sensor calibration data.  If the hardware is changed,
	      this routine must be recompiled using the new rims.h.

	      With Mini RIMS C, a ccw rotation as viewed from the top
	      of the sensors causes the resolver angle to go from
	      zero to +pi, -pi to zero.  A CCW rotation gives negative
	      x and xr gyro counts.

called routines:  none.

*****************************************************************/

/* Last Modified:  12/18/87    BY:  E.E. Creel, 9131		*/

/* 18/DEC/87 Modified to include positive and negative gyro counts -- for   */
/*           compatibility with the new sensread.                           */
/* Modified to remove the linear prediction coefs and read them from rims.h */
/* Modified to use the MINI Rims C resolver angle w/o polarity swithching  */
/* Modified to calculate accel scale factors instead of biases		*/

#include "navvars.h"
#include "rims.h"

#define M3DEG	-5.235988e-2	/* minus three degrees in radians	*/
#define P3DEG	 5.235988e-2	/* plus three degrees in radians	*/

#define PT2	6.283185308	/* two pi - amount of test revolution	*/
#define MPT2	-6.283185308

long sum;

tstrims()
{
register struct nav_vars *n;
register struct rims *r;

extern long sum;

short int *pc;			/* ptr to control word in rims interface */
struct plat_control *rc;	/* pointers to rims bit field structures */

n = (struct nav_vars *)NAV_start;
r = (struct rims *)RIMS_START;	/* initialize data structure pointers	*/

pc = (short int *)&r->control;
rc = (struct plat_control *)&n->rimscmd;

if (n->nsec < 10)
    {
    n->cntflg = 0;
    n->skpflg = 0;
    n->xcwtot = 0;
    n->xccwtot = 0;
    n->xrcwtot = 0;
    n->xrccwtot = 0;
    n->xsstot = 0;
    n->xrsstot = 0;
    n->axcaltot = 0;
    }
else
    {
    if (n->nsec < 80)
	{
	datacw();
	if (n->skpflg == 0)
	    {
	    rc->rims_cont = SLEW_CW;
	    rc->rims_oper = RUN_CAGE_SLEW;
	    *pc = n->rimscmd;			/* copy command to rims	*/
	    if ((n->rmrd < M3DEG))	/* test if 357-360 deg	*/
		n->cntflg = 1;
	    else
		{
		if (n->cntflg == 1)
		    {
		    if (n->asec == 0)
			{
			rc->rims_cont = CAGE_0;
			rc->rims_oper = RUN_CAGE_SLEW;
			*pc = n->rimscmd;	/* copy command to rims	*/
			n->skpflg = 0xff;
			n->asec++;
	                }
        	    else
                	{
	                n->cntflg = 0;
        	        n->asec++;
                	}
	            }
		}
	    }
	}
    else
	{
	if (n->nsec < 90)
	    {
	    n->cntflg = 0;
	    n->skpflg = 0;
	    datacw();
	    }
	}
    }
if (n->nsec >= 90)
    {
    if (n->nsec < 160)
	{
	dataccw();
	if (n->skpflg == 0)
	    {
	    rc->rims_cont = SLEW_CCW;
	    rc->rims_oper = RUN_CAGE_SLEW;
	    *pc = n->rimscmd;		/* copy command to rims	*/

	    if ((n->rmrd > P3DEG))		 /* test if 0-3 deg	*/
		n->cntflg = 1;
	    else
		{
		if (n->cntflg == 1)
		    {
		    if (n->asec == 1)
			{
			rc->rims_cont = CAGE_0;
			rc->rims_oper = RUN_CAGE_SLEW;
			*pc = n->rimscmd;	/* copy command to rims	*/
			n->skpflg = 0xff;
			n->asec--;
			}
		    else
			{
			n->cntflg = 0;
			n->asec--;
			}
		    }
		}
	    }
	}
    else
	{
	if (n->nsec < 170)
	    {
	    dataccw();
	    n->cntflg = 0;
	    n->calcnt = 0;
	    }
	else
	    if (n->nsec < 250)
		datass();
	}
    }

if (n->nsec == 250)
    {
    switch (n->calcnt)
        {
        case 0:
            sum = n->xccwtot - n->xsstot;
            if (sum != 0)
                n->prtxgkp = PT2 / (float)sum;
            else
                n->prtxgkp = n->rtxgkp;

            sum = n->xcwtot - n->xsstot;
            if (sum != 0)
                n->prtxgkn = MPT2 / (float)sum;
            else
                n->prtxgkn = n->rtxgkn;

            break;
        case 1:
            sum = n->xrccwtot - n->xrsstot;
            if (sum != 0)
                n->prtxrgkp = PT2 / (float)sum;
            else
                n->prtxrgkp = n->rtxrgkp;

            sum = n->xrcwtot - n->xrsstot;
            if (sum != 0)
                n->prtxrgkn = MPT2 / (float)sum;
            else
                n->prtxrgkn = n->rtxrgkn;

            break;
        case 2:
            n->prtygkp = SC1 * n->prtxgkp + SC2;
            n->prtygkn = SC3 * n->prtxgkn + SC4;
            break;
        case 3:
            n->prtzgkp = SC5 * n->prtxrgkp + SC6;
            n->prtzgkn = SC7 * n->prtxrgkn + SC8;
            break;
        case 4:
	    if (n->axcaltot != 0)
		n->prtxak = ((n->gdp + n->xab) * (n->onesec * 240))
			    / (float)n->axcaltot;
	    else
		n->prtxak = n->rtxak;
            break;
        case 5:
            n->prtyak = SC9 * n->prtxak + SC10;
            n->prtzak = SC11 * n->prtxak + SC12;
            break;
        default:
            break;
        }
    n->calcnt++;
    }
}

datass()
{
    register struct nav_vars *n;

    n = (struct nav_vars *)NAV_start;

    n->xsstot += n->ixgn + n->ixgn1 + n->ixgp + n->ixgp1;
    n->xrsstot += n->ixrgn + n->ixrgn1 + n->ixrgp + n->ixrgp1;
    n->axcaltot += n->ixan + n->ixan1;
}

datacw()
{
    register struct nav_vars *n;

    n = (struct nav_vars *)NAV_start;

    n->xcwtot += n->ixgn + n->ixgn1 + n->ixgp + n->ixgp1;
    n->xrcwtot += n->ixrgn + n->ixrgn1 + n->ixrgp + n->ixrgp1;
    n->axcaltot += n->ixan + n->ixan1;
}

dataccw()
{
    register struct nav_vars *n;

    n = (struct nav_vars *)NAV_start;

    n->xccwtot += n->ixgn + n->ixgn1 + n->ixgp + n->ixgp1;
    n->xrccwtot += n->ixrgn + n->ixrgn1 + n->ixrgp + n->ixrgp1;
    n->axcaltot += n->ixan + n->ixan1;
}

