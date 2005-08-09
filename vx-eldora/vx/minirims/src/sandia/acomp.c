/****************************************************************
* acomp.c							*
*****************************************************************

Description:  used to calculate the velocity increments using data
	      from the accelerometers compensated for bias, size
	      effect (offset) and misalignments.

inputs:	      accelerometer data stored in the nav data structure.
outputs:      differential velocity terms dvxi,dvyi,dvzi.

notes:	      This routine must be changed to reflect the calibration
	      data of the mini-rims platform being used.

called routines:  none

*****************************************************************/

/* Last Modified:  8/18/87    BY:  E.J. Nava, 9131		*/

#include "navvars.h"
#include "rims.h"

#define CONST 1.8403328

acomp()
{
  register struct nav_vars *n;
  float dvxis,dvyis,dvzis;
  float omgx,omgy,omgz,omgx2,omgy2,omgz2;

  n = (struct nav_vars *) NAV_start;

/* accelerometer bias and scale factor calculation */

  dvxis = n->rtxak * (n->ixan + n->ixan1) - n->xab;
  dvyis = n->rtyak * (n->iyan + n->iyan1) - n->yab;
  dvzis = n->rtzak * (n->izan + n->izan1) - n->zab;

/* accelerometer misalignment */

  n->dvxi = dvxis - XAZ * dvyis + XAY * dvzis;
  n->dvyi = dvyis + YAZ * dvxis - YAX * dvzis;
  n->dvzi = dvzis - ZAY * dvxis + ZAX * dvyis;

/* size effect corrections 					*/
/* Compensate for accelerometer offsets from the platform CG	*/

  omgx = n->dphx;
  omgy = n->dphy;
  omgz = n->dphz;

  omgx2 = omgx * omgx;
  omgy2 = omgy * omgy;
  omgz2 = omgz * omgz;

/* units - ft/sec per delta time */

  n->dvxi += (n->rxx * (omgy2 + omgz2) - n->rxy * omgx * omgy
	  - n->rxz * omgx * omgz - n->rxz * n->alfy + n->rxy * n->alfz);
  n->dvyi += (n->ryy * (omgx2 + omgz2) - n->ryz * omgy * omgz
	  - n->ryx * omgx * omgy - n->ryx * n->alfz + n->ryz * n->alfx);
  n->dvzi += (n->rzz * (omgx2 + omgy2) - n->rzy * omgy * omgz
	  - n->rzx * omgx * omgz - n->rzy * n->alfx + n->rzx * n->alfy);
/* Additional rotation compensation used on Hoe and Swerve flights	*/
/* 		-------- Disabled -----------				*/
/*
  n->dvxi += n->alfy * CONST;
  n->dvyi -= n->alfx * CONST;
  n->dvzi -= n->alfx * CONST;
*/

}
