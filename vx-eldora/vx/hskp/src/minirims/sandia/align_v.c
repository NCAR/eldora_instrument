/*+ DEC/CMS REPLACEMENT HISTORY, Element ALIGN_V.C+*/
/*+ *4    28-OCT-1988 14:39:19 ECREEL "eec: changing initialization for saved values of gyro biases"+*/
/*+ *3    18-OCT-1988 16:39:37 ECREEL "eec: adding declare for op_az, old_op_az, and op_az_flag"+*/
/*+ *2    18-OCT-1988 16:06:49 ECREEL "eec: adding saved_tdxgb, ...ygb, ...zgb"+*/
/*+ *1     7-MAR-1988 15:42:37 JRFELLE ""+*/
/*+ DEC/CMS REPLACEMENT HISTORY, Element ALIGN_V.C+*/
/* 10/26/87  11:05
 * NAME:  align_v.c
 *
 * NAV alignment variable declarations.
 *
 * last modification:
 */

#define NN 9


double f[NN][NN];
double kal_gn[NN][2];
double p[NN][NN];
double q[NN][NN];
double r[NN][NN];
double dph_x2, dph_y2, dph_z2;
double dph_x_acc, dph_y_acc, dph_z_acc;
double t_align;
double t_up;
double t_up_1;
double dvn, dve;

int propagate;
int update;
int a_mode;

/* temporary space for Kalman gain routine */
double phi[NN][NN];
double lamqlamt[NN][NN];
double temp_1[NN][NN];
double temp_2[NN][NN];
double temp_3[NN][NN];
double hphr[2][2];
double hhphr[2][2];

double op_az;
