/****************************************************************
* align.c							*
*****************************************************************

<<<<<<<<<<<<<<<<<<<<<<<<<< N O T E >>>>>>>>>>>>>>>>>>>>>>>>>>>>

This file is under configuration control.  Any changes to this routine
must be noted in the configuration control log.

<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


Description:  This routine is used to determing the initial alignment
	      of the RIMS hardware with respect to the local level
	      coordinate frame.  This is done through the use of a
	      Kalman fitler with precomputed gains.  The measurement
	      function uses the horizontal velocities to drive the
	      estimate of azimuth (yaw) and gyro biases.

inputs:	      local level horizontal velocities from the nav data structure.
outputs:      estimates of yaw and gyro biases.

notes:	      none

called routines:  arctan()

*****************************************************************/

#define NO_MINIR_SCOPE
#include "minirGbl.h"
#include "align_v.h"
#include "vrtxvisi.h"

#define ivn 0
#define ive 1
#define ien 2
#define iee 3
#define ied 4
#define isind 4
#define icosd 5
#define ixgb 6
#define iygb 7
#define izgb 8

align()
{
extern long align_mail_box;
extern unsigned short vrtx_errf;
long int err;

register NAV_VARS *n;

double arctan();

n = nav_ptr;

dph_x_acc += n->dphx;
dph_y_acc += n->dphy;
dph_z_acc += n->dphz;

t_align += n->dtv;

  if ( t_align >= t_up ) {

    n->nsec++;
    n->asec++;
    t_up_1 = t_up;
    t_up += DT_UPDT;

    /* compute driving noise based on rotations in last alignment period */
    /* for use in the kalman filter routine and then reset the           */
    /* accumulators                                                      */

    dph_x2 = dph_x_acc * dph_x_acc;
    dph_x_acc = 0.;
    dph_y2 = dph_y_acc * dph_y_acc;
    dph_y_acc = 0.;
    dph_z2 = dph_z_acc * dph_z_acc;
    dph_z_acc = 0.;

    n->dthn += n->vn * kal_gn[ien][ivn] + n->ve * kal_gn[ien][ive];
    n->dthe += n->vn * kal_gn[iee][ivn] + n->ve * kal_gn[iee][ive];

    if (a_mode == COURSE)
    {
       n->calf += (n->vn * kal_gn[icosd][ivn] + n->ve * kal_gn[icosd][ive]);
       n->salf += (n->vn * kal_gn[isind][ivn] + n->ve * kal_gn[isind][ive]);
       if (t_up > T_ALIGN_SW)
       {
       a_mode=FINE;
       n->dthd += arctan(n->salf,n->calf);
       f[4][1] = -(n->tnlt/n->rlgpa);
       f[4][3] = - n->omeclt;
       f[2][5] = 0.;
       }
    }
    else
       n->dthd += n->vn * kal_gn[ied][ivn] + n->ve * kal_gn[ied][ive];



    n->tdxgb -= ( n->vn * kal_gn[ixgb][ivn] + n->ve * kal_gn[ixgb][ive] )*n->dtv;
    n->tdygb -= ( n->vn * kal_gn[iygb][ivn] + n->ve * kal_gn[iygb][ive] )*n->dtv;
    n->tdzgb -= ( n->vn * kal_gn[izgb][ivn] + n->ve * kal_gn[izgb][ive] )*n->dtv;

/*	adjust velocity error terms		*/

    dvn = n->vn * kal_gn[ivn][ivn] + n->ve * kal_gn[ivn][ive];
    dve = n->vn * kal_gn[ive][ivn] + n->ve * kal_gn[ive][ive];
    n->vn -= dvn;
    n->ve -= dve;
    n->vd = 0.0;

    propagate = 1;
    update = 1;

    sc_post(&align_mail_box,100L,&err);
    if (err != RET_OK)
      vrtx_errf++;

  }   /* end of part executed for each update */
}
