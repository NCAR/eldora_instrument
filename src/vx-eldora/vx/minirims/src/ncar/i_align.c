/* 10/29/87  13:07
 * NAME:  i_align.c
 *
 * Alignment initialization routine.
 * This routine initializes the permanent variables associated with
 * the alignment routines.
 *
 * INPUT:	double	lat
 *
 * OUTPUT:	double	f[NN][NN]
 *		double	p[NN][NN]
 *		double	q[NN][NN]
 *		double	r[NN][NN]
 *		double	t_align
 *		double	t_up
 *		double	t_up_1
 *		double	t_align_sw
 *
 *		int	propagate, update
 */

#define NO_MINIR_SCOPE
#include "align_v.h"
#include "minirGbl.h"

void i_align()
{
  register NAV_VARS *n;
  extern short int modein;
  extern int align_calls;

  int i,j;

   n = nav_ptr;

   n->calf = 0.0;
   n->salf = 0.0;

   align_calls = 0;

   for(i = 0; i < NN; i++)
   {
      for(j = 0; j < NN; j++)
      {
         f[i][j] = 0.0;
         p[i][j] = 0.0;
         q[i][j] = 0.0;
      }
   }

   r[0][0] = 0.0;
   r[0][1] = 0.0;
   r[1][0] = 0.0;
   r[1][1] = 0.0;

   t_up_1  = 0.0;
   t_up    = DT_UPDT;
   t_align = 0.0;

   dph_x_acc = 0.;
   dph_y_acc = 0.;
   dph_z_acc = 0.;

   dph_x2 = 0.;
   dph_y2 = 0.;
   dph_z2 = 0.;

/* Initialize INS error matrix */
   f[0][1] = -2.0 * n->omeslt;
   f[0][3] = n->gdp/n->dtv;
   f[1][0] = 2.0 * n->omeslt;
   f[1][2] = -n->gdp/n->dtv;
   f[2][1] = 1.0/n->rlgpa;
   f[2][3] = - n->omeslt;
   f[3][0] = -1.0/n->rlgpa;
   f[3][2] = n->omeslt;
   f[3][4] = n->omeclt;

/* Initialize covariance matrix */
   p[0][0] = 0.01;
   p[1][1] = 0.01;
   p[2][2] = 0.01;
   p[3][3] = 0.01;
   p[6][6] = S_GBX2;
   p[7][7] = S_GBY2;
   p[8][8] = S_GBZ2;

/* Initialize driving noise matrix */
   q[0][0] = Q_VN;
   q[1][1] = Q_VE;

   /* attitude driving noise is inserted in s_align */

   q[6][6] = Q_GBX;
   q[7][7] = Q_GBY;
   q[8][8] = Q_GBZ;

/* Initialize measurement noise matrix */
   r[0][0] = M_NOISEN2;
   r[1][1] = M_NOISEE2;

/* Mode dependent initial parameters */
   switch(modein)
   {
   case 4:   /* values for optical align */

     /* heading -- 0.005 deg */
     p[4][4] = 7.61544e-9;
     p[5][5] = 7.61544e-9;

     /* gyro biases -- 0.025 deg/hr */
     p[6][6] = 1.47e-14;
     p[7][7] = 1.47e-14;
     p[8][8] = 1.47e-14;

     f[4][1] = -(n->tnlt/n->rlgpa);
     f[4][3] = -n->omeclt;
     a_mode = FINE;
     break;

   case 6:     /* use standard values for QUICK align */

     p[4][4] = 0.01;
     p[5][5] = 0.01;

     f[4][1] = -(n->tnlt/n->rlgpa);
     f[4][3] = -n->omeclt;
     a_mode = FINE ;
     break;

   case 8:
   case 9:
   default:
     p[4][4] = 0.5;
     p[5][5] = 0.5;
     f[2][5] = - n->omeclt;
     a_mode = COURSE ;
     break;
   }
}
