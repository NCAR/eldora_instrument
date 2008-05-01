/*+ DEC/CMS REPLACEMENT HISTORY, Element RMRDCALC.C+*/
/*+ *2    29-OCT-1988 16:18:04 JRFELLE "added r_sin and r_cos for diagnostics"+*/
/*+ *1     7-MAR-1988 14:41:33 JRFELLE "resolver angle calculate routine"+*/
/*+ DEC/CMS REPLACEMENT HISTORY, Element RMRDCALC.C+*/
/****************************************************************
* rmrdcalc.c							*
*****************************************************************

<<<<<<<<<<<<<<<<<<<<<<<<<< N O T E >>>>>>>>>>>>>>>>>>>>>>>>>>>>

This file is under configuration control.  Any changes to this routine
must be noted in the configuration control log.

<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


Description:  This routine is used to determine the resolver position
	      in radians based on sine and cosine data read from the
	      resolver by the A/D.

inputs:	      sine and cosine of the resolver from the nav data structure.
outputs:      resolver angle in radians.

notes:	      Input data is from a 12 bit A/D converter with the data in
	      complimentary binary form so that an input of 0x0000
	      represents +10v and 0x0FFF represents -10v.
	      The upper four bits are masked to insure proper
	      operation with 16 bit arithmetic.

called routines:  arctan()

*****************************************************************/

/* Last Modified:  11/18/87    BY:  B.A. Sims, 7525		*/

/* Modified to solve for angle instead of voltage               */
/* Modified to change RMRD_SF to 1/204.8                        */
/* Modified n->rmrd to make negative for rims-a-3 on carco table */
/* Modified to remove hardware specific calibration of a/d's	*/
/* Modified to convert int number to -1 to +1 range		*/
/* Modified to switch angle sign - now looks like a cw rotation	*/
/* Modified to clean up parameter definitions with arctan	*/
/* Modified to switch back to original rotation polarity for acs*/

#include "navvars.h"
#include "math.h"
#include "rims.h"

#define RMRD_SF	  4.8828125e-3	/* A/D Scale factor - 1/204.8		*/
#define AND_MASK	0x0fff	/* forces MS nibble to zero - plus case	*/
double r_sin,r_cos;

rmrdcalc()
{
register struct nav_vars *n;

double arctan();

double sin,cos;		        /* values converted to single prec float */
short int sinint,cosint;	/* values converted to signed 16 bit int */

n = (struct nav_vars *)NAV_start;

sinint = n->rmad[7] & AND_MASK;		/* force MS bits to zero	*/
sin = ((RMRD_SF * (double)(2048-sinint))-(R_SIN_BIAS))/R_SIN_NOM;
                                	/* convert to -1 to +1 range 	*/
					/* from compl binary form.	*/

cosint = n->rmad[8] & AND_MASK;		/* force MS bits to zero	*/
cos = ((RMRD_SF * (double)(2048-cosint))-(R_COS_BIAS))/R_COS_NOM;	
                                        /* convert to -1 to +1 range	*/
					/* from compl binary form.	*/

n->rmrd = (float) arctan(sin,cos) - R_SIN_OFF;
					/* calculate resolver angle	*/
					/* ---change sign---            */
r_sin = sin;
r_cos = cos;

}
