/*+ DEC/CMS REPLACEMENT HISTORY, Element S_ALIGN.C+*/
/*+ *1     7-MAR-1988 14:40:07 JRFELLE "alignment kalman filter routine"+*/
/*+ DEC/CMS REPLACEMENT HISTORY, Element S_ALIGN.C+*/
/* 10/29/87  12:16
 * Kalman filter routine
 * This routine propagates the covariace matrix from t_up_1 to t_up
 * computes the Kalman gains and then computes the post update value
 * of the covariace matrix. The Joseph formulation of the covariace
 * update is used. The state transition matrix is computed then used
 * to update the covariace matrix.
 * It is assumed that the covariace matrix and the linear system
 * matrix are initialized elsewhere.
 *
 *
 * INPUT:	double a11, a12, a13,
 *		       a21, a22, a23,
 *		       a31, a32, a33
 *		double f[NN][NN]
 *		double kal_gn[NN][2]
 *		double p[NN][NN]
 *		double q[NN][NN]
 *		double r[NN][NN]
 *		double t_up
 *		double t_up_1
 *
 *		int    propagate
 *
 *
 * OUTPUT:	double kal_gn[NN][2]
 *		double p[NN][NN]
 *
 *		int    propagate
 *		int    update
 *
 */

#define NO_MINIR_SCOPE
#include "align_v.h"
#include "minirGbl.h"
#include "vrtxvisi.h"

long int align_mail_box;
long int align_calls;

void
s_align()
{
  register NAV_VARS *n;
  long int i,j,k,err,msg;

  extern double phi[NN][NN],
                lamqlamt[NN][NN],
                temp_1[NN][NN],
                temp_2[NN][NN],
                temp_3[NN][NN],
                hphr[2][2],
                hhphr[2][2];

  double  det_inv,
          del_t,
          del_t_2,
          del_t_4 ;

  double  q_x_2,
          q_y_2,
          q_z_2;

  gatarout(1,"ALIGN_TASK is alive!!!\n",23);
  n = nav_ptr;

 while(1)
 {
  msg = sc_pend(&align_mail_box,0L,&err);
  if(err != RET_OK)
    gatarout(1,"ALIGN_TASK pend error\n",22);

  align_calls++;

  if( update > 0 )
  {
    /* compute post update covariance */
    /* P(+) = (I-KH)P(-)(I-KH)' + KRK'  */

    /* compute (I-KH) */

    for (i=0;i<NN;i++)
    {
      for (j=0;j<NN;j++)
      {
         if(i == j)
            temp_1[i][j] = 1. ;
         else
            temp_1[i][j] = 0. ;
      }
      temp_1[i][0] -= kal_gn[i][0] ;
      temp_1[i][1] -= kal_gn[i][1] ;
    }

   /* compute (I_KH)P(-)(I-KH)' */

    for (i=0;i<NN;i++)
    {
      for (j=0;j<NN;j++)
      {
        temp_2[i][j] = 0. ;
        for (k=0;k<NN;k++)
          temp_2[i][j] += p[i][k]*temp_1[j][k] ;
      }
    }
    for (i=0;i<NN;i++)
    {
       for (j=0;j<NN;j++)
       {
         temp_3[i][j] = 0. ;
         for (k=0;k<NN;k++)
           temp_3[i][j] += temp_1[i][k]*temp_2[k][j] ;
        }
    }

    for (j=0;j<NN;j++)
    {
      temp_1[0][j] = r[0][0]*kal_gn[j][0]+r[0][1]*kal_gn[j][1] ;
      temp_1[1][j] = r[1][0]*kal_gn[j][0]+r[1][1]*kal_gn[j][1] ;
    }


    for (i=0;i<NN;i++)
    {
      for (j=0;j<NN;j++)
        temp_2[i][j] = kal_gn[i][0]*temp_1[0][j]+kal_gn[i][1]*temp_1[1][j] ;
    }

    for (i=0;i<NN;i++)
    {
      for (j=0;j<NN;j++)
        p[i][j] = temp_2[i][j] + temp_3[i][j] ;
    }
    update = 0;

  }  /* END OF UPDATE IF STATEMENT */

  if( propagate > 0 )
  {
    del_t = t_up - t_up_1 ;

    if(del_t > 0.)
    {
      del_t_2 = 0.5*del_t*del_t ;
      del_t_4 = 0.5*del_t_2 ;

      /* compute approximation for the state transition matrix */

      /* include direction cosine elements for instrument sensitivity */

      f[2][6] =  n->a11;
      f[2][7] =  n->a12;
      f[2][8] =  n->a13;
      f[3][6] =  n->a21;
      f[3][7] =  n->a22;
      f[3][8] =  n->a23;
      f[4][6] =  n->a31;
      f[4][7] =  n->a32;
      f[4][8] =  n->a33;


      for (i=0;i<NN;i++)
      {
         for (j=0;j<NN;j++)
         {
            phi[i][j] = 0. ;
            for (k=0;k<NN;k++)
              phi[i][j] += f[i][k]*f[k][j]*del_t_2 ;
            phi[i][j] += f[i][j]*del_t ;
         }
         phi[i][i] += 1.0 ;
      }

      /* compute approximation for discrete driving noise matrix */


      /* include driving noise based on rotations sensed in last
         second                                                  */

      q_x_2 = D_XGK_2*dph_x2 + D_XGY_2*dph_y2 + D_XGZ_2*dph_z2;
      q_y_2 = D_YGX_2*dph_x2 + D_YGK_2*dph_y2 + D_YGZ_2*dph_z2;
      q_z_2 = D_ZGX_2*dph_x2 + D_ZGY_2*dph_y2 + D_ZGK_2*dph_z2;

      q[2][2] = n->a11*q_x_2*n->a11 + n->a12*q_y_2*n->a12 + n->a13*q_z_2*n->a13
                + Q_EN ;
      q[2][3] = n->a11*q_x_2*n->a21 + n->a12*q_y_2*n->a22 + n->a13*q_z_2*n->a23;
      q[2][4] = n->a11*q_x_2*n->a31 + n->a12*q_y_2*n->a32 + n->a13*q_z_2*n->a33;
      q[3][2] = n->a21*q_x_2*n->a11 + n->a22*q_y_2*n->a12 + n->a23*q_z_2*n->a13;
      q[3][3] = n->a21*q_x_2*n->a21 + n->a22*q_y_2*n->a22 + n->a23*q_z_2*n->a23
                + Q_EE ;
      q[3][4] = n->a21*q_x_2*n->a31 + n->a22*q_y_2*n->a32 + n->a23*q_z_2*n->a33;
      q[4][2] = n->a31*q_x_2*n->a11 + n->a32*q_y_2*n->a12 + n->a33*q_z_2*n->a13;
      q[4][3] = n->a31*q_x_2*n->a21 + n->a32*q_y_2*n->a22 + n->a33*q_z_2*n->a23;
      q[4][4] = n->a31*q_x_2*n->a31 + n->a32*q_y_2*n->a32 + n->a33*q_z_2*n->a33
                + Q_ED ;

      for (i=0;i<NN;i++)
      {
         for (j=0;j<NN;j++)
         {
            lamqlamt[i][j] = 0. ;
            for (k=0;k<NN;k++)
               lamqlamt[i][j] += (f[i][k]*q[k][j]+q[i][k]*f[j][k])*del_t_4 ;
            lamqlamt[i][j] += q[i][j]*del_t ;
         }
      }

      /* propagate the covariace matrix */
      for (i=0;i<NN;i++)
      {
         for (j=0;j<NN;j++)
         {
            temp_1[i][j] = 0. ;
            for (k=0;k<NN;k++)
               temp_1[i][j] += p[i][k]*phi[j][k] ;
         }
      }

      for (i=0;i<NN;i++)
      {
         for (j=0;j<NN;j++)
         {
            p[i][j] = 0. ;
            for (k=0;k<NN;k++)
               p[i][j] += phi[i][k]*temp_1[k][j] ;
            p[i][j] += lamqlamt[i][j] ;
         }
      }

      /* compute H' / [HPH' + R] */
      /* compute (R+HPH') */
      hphr[0][0] = p[0][0]+r[0][0] ;
      hphr[0][1] = p[0][1]+r[0][1] ;
      hphr[1][0] = p[1][0]+r[1][0] ;
      hphr[1][1] = p[1][1]+r[1][1] ;

      det_inv = 1.0/(hphr[0][0]*hphr[1][1]-hphr[1][0]*hphr[0][1])  ;

      hhphr[0][0]= hphr[1][1]*det_inv ;
      hhphr[0][1]= -(hphr[0][1]*det_inv) ;
      hhphr[1][0]= -(hphr[1][0]*det_inv) ;
      hhphr[1][1]= hphr[0][0]*det_inv ;
      /* compute optimal gains */
      for (i=0;i<NN;i++)
      {
        kal_gn[i][0] = p[i][0]*hhphr[0][0] + p[i][1]*hhphr[1][0] ;
        kal_gn[i][1] = p[i][0]*hhphr[0][1] + p[i][1]*hhphr[1][1] ;
      }
      propagate = 0 ;
    }  /* END OF DEL_T IF STATEMENT */

  }  /* END OF PROPAGATE IF STATEMENT */
 } /* END OF WHILE STATEMENT */
}
