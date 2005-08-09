/****************************************************************
* gcomp.c							*
*****************************************************************

Description:  used to calculate the rotation increments using data
	      from the gyros compensated for bias and
	      misalignments.

inputs:	      gyro data stored in the nav data structure.
outputs:      differential rotational terms dphx,dphy,dphz

notes:	      calibration data for the mini-rims platform being used
	      is stored in the file rims.h

called routines:  none

*****************************************************************/

/* Last Modified:  9/JAN/88 BY:  J.R. Fellerhoff, 9131		*/

/* 22/DEC/87 The rotation calculations implemented for RDT have been removed */
/*           Original version saved in:                                      */
/*	        [ecreel.esie.nav.diagnostics]gcomp_rdt.c                     */
/* 19/DEC/87 Modified to define new external separate counters for pos &
             negative gyro counts                                          */
/* 18/DEC/87 Scale factors are now applied independently for the positive  */
/*           and negative instrument counts. The deltang() subroutine has  */
/*           been removed.                                                 */
/* Modified to compute platform rotations during a countdown interval */
/* Modified to add facility for accumulating gyro counts */
/* Modified to use gyro bias correction terms from align	*/
/* Modified to correct the scale factor bug  by: Rick Fellerhoff */

#define NO_MINIR_SCOPE
#include "minirGbl.h"
#include "rims.h"

#define K1 0.083333		/* 1/12 - Bortz algorithm constants	*/
#define K2 0.583333		/* 7/12					*/

/* this routine is used to adjust the raw gyro data for biases */
/* misalignments and g sensitivities. */
/* ----- xr gryo values for all x direction parameters ------- */


/* External variables for computing platform rotation angles */
int start_flag = 0L;

double as11,as12,as13,as21,as22,as23,as31,as32,as33;  /* a matrix @ start
                                                         of accumulation */
double ae11,ae12,ae13,ae21,ae22,ae23,ae31,ae32,ae33;  /* a matrix @ end of
                                                         accumulation */

gcomp()
{
register NAV_VARS *n;

double dattx,datty,dattz,xgbs,ygbs,zgbs,dphxs,dphys,dphzs;

/* Variables required for implementation of RDT's rotation calculations */
/* Declared in navtask.c */
extern int start_flag;

/* Counters for raw accelerometer and gyro counts */
extern long int xg_cnts, yg_cnts, zg_cnts, xrg_cnts;
extern long int xg_pcnts, yg_pcnts, zg_pcnts, xrg_pcnts;
extern long int xg_ncnts, yg_ncnts, zg_ncnts, xrg_ncnts;
extern long int xa_cnts, ya_cnts, za_cnts;
extern double dph_x_tot, dph_y_tot, dph_z_tot;
extern long int countdown;

n = nav_ptr;

n->dthx = (double)n->ixrgp * (double)n->rtxrgkp -
          (double)n->ixrgn * (double)n->rtxrgkn;
n->dthy = (double)n->iygp * (double)n->rtygkp -
          (double)n->iygn * (double)n->rtygkn;
n->dthz = (double)n->izgp * (double)n->rtzgkp -
          (double)n->izgn * (double)n->rtzgkn;

n->dthx1 = (double)n->ixrgp1 * (double)n->rtxrgkp -
          (double)n->ixrgn1 * (double)n->rtxrgkn;
n->dthy1 = (double)n->iygp1 * (double)n->rtygkp -
          (double)n->iygn1 * (double)n->rtygkn;
n->dthz1 = (double)n->izgp1 * (double)n->rtzgkp -
          (double)n->izgn1 * (double)n->rtzgkn;

/* bortz calculations */

dattx = K1 * (n->dthy2 * n->dthz1 - n->dthz2 * n->dthy1) + K2 *
	(n->dthy1 * n->dthz - n->dthz1 * n->dthy);
datty = K1 * (n->dthx2 * n->dthz1 - n->dthx1 * n->dthz2) + K2 *
	(n->dthx1 * n->dthz - n->dthx * n->dthz1);
dattz = K1 * (n->dthx2 * n->dthy1 - n->dthx1 * n->dthy2) + K2 *
	(n->dthx1 * n->dthy - n->dthx * n->dthy1);

/* Accumulate attitude changes for telemetry	*/

n->adatx += dattx;
n->adaty += datty;
n->adatz += dattz;

/* calculate gyro biases and g sensitivity terms */

xgbs = n->xgb + n->tdxgb + XGAX * n->dvxi + XGAY * n->dvyi + XGAZ * n->dvzi;
ygbs = n->ygb + n->tdygb + YGAX * n->dvxi + YGAY * n->dvyi + YGAZ * n->dvzi;
zgbs = n->zgb + n->tdzgb + ZGAX * n->dvxi + ZGAY * n->dvyi + ZGAZ * n->dvzi;

dphxs = (n->dthx + n->dthx1) + dattx - xgbs;
dphys = (n->dthy + n->dthy1) - datty - ygbs;
dphzs = (n->dthz + n->dthz1) + dattz - zgbs;

/* correct for gyro misalignments */

n->dphx1 = dphxs - XGZ * dphys + XGY * dphzs;
n->dphy1 = dphys - YGX * dphzs + YGZ * dphxs;
n->dphz1 = dphzs + ZGX * dphys - ZGY * dphxs;

/* calculate alpha correction terms - angle change per sample time */

n->alfx = n->dphx1 - n->dphxl;
n->alfy = n->dphy1 - n->dphyl;
n->alfz = n->dphz1 - n->dphzl;

n->dphx = n->dphx1;
n->dphy = n->dphy1;
n->dphz = n->dphz1;

/* save last sample for next iteration */

n->dphxl = n->dphx;
n->dphyl = n->dphy;
n->dphzl = n->dphz;

n->dthx2 = n->dthx;
n->dthy2 = n->dthy;
n->dthz2 = n->dthz;
			
	switch(countdown)	/* accumulate gyro and accelerometer counts */
				/* compute platform rotation during countdown */
	{
	  case -1L:	/* zero out counter totals and external copies */
	    xg_cnts = xg_pcnts = xg_ncnts =  0;
	    yg_cnts = yg_pcnts = yg_ncnts = 0;
	    zg_cnts = zg_pcnts = zg_ncnts = 0;
	    xrg_cnts = xrg_pcnts = xrg_ncnts = 0;
	    xa_cnts = 0;
	    ya_cnts = 0;
	    za_cnts = 0;
            dph_x_tot = 0;
            dph_y_tot = 0;
            dph_z_tot = 0;

	    start_flag = 1;
	  break;

          case -2L:
	  break;

	  case 0L:
	    countdown = -2L;

	    /* take a snapshot of A matrix at the end of accumulation time */
            ae11 = n->a11;
	    ae12 = n->a12;
	    ae13 = n->a13;
	    ae21 = n->a21;
	    ae22 = n->a22;
	    ae23 = n->a23;
	    ae31 = n->a31;
	    ae32 = n->a32;
	    ae33 = n->a33;
	  break;

	  default:	/* decrement the countdown value */
	    countdown -= 1;
	    if(start_flag == 1L)
	      {
	      start_flag = 0L;
	      /* take a snapshot of the A matrix at start of accumulation */
              as11 = n->a11;
	      as12 = n->a12;
	      as13 = n->a13;
	      as21 = n->a21;
	      as22 = n->a22;
	      as23 = n->a23;
	      as31 = n->a31;
	      as32 = n->a32;
              as33 = n->a33;
	      }
	    xg_pcnts += n->ixgp + n->ixgp1;
	    xg_ncnts += n->ixgn + n->ixgn1;
	    xg_cnts += n->ixgp + n->ixgp1 - n->ixgn - n->ixgn1;

	    yg_pcnts += n->iygp + n->iygp1;
	    yg_ncnts += n->iygn + n->iygn1;
	    yg_cnts += n->iygp + n->iygp1 - n->iygn - n->iygn1;

	    zg_pcnts += n->izgp + n->izgp1;
	    zg_ncnts += n->izgn + n->izgn1;
	    zg_cnts += n->izgp + n->izgp1 - n->izgn - n->izgn1;

	    xrg_pcnts += n->ixrgp + n->ixrgp1;
	    xrg_ncnts += n->ixrgn + n->ixrgn1;
	    xrg_cnts += n->ixrgp + n->ixrgp1 - n->ixrgn - n->ixrgn1;

	    xa_cnts += n->ixan + n->ixan2;
	    ya_cnts += n->iyan + n->iyan2;
	    za_cnts += n->izan + n->izan2;

            dph_x_tot += n->dphx;
            dph_y_tot += n->dphy;
            dph_z_tot += n->dphz;

    	  break;
	} /* end of countdown switch */

}
