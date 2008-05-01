#include "spin.h"
#include "fltseqdat.h"
/*+**********************************************************************

NAME:           init_acs.c
SYNOPSIS:       void init_acs();
INPUTS:         external acs variables
PROCESSING:     initialize these variables to nominal values
OUTPUTS:        None.
RETURNS:        None.

R.S.Edmunds, 9132

last modified:
   6/8/88: original init_acs.c function by rick fellerhoff.
   4/13/89: original ac_hard and ac_soft by david kozlowski
   9/30/91: added initialization of rate_check.
   10/2/91: added notch_delay.
---------------------------------------------------------------
   2/3/92:  begin mods for kite project.
   2/5/92:  modify inertias and lever arms to heavy configuration for kite
   2/6/91:  add inertias and lever arms for 3 separate configurations
   2/7/92:  modify rdb_x24 to a percentage of w0_spin instead of a fixed
            value.
   2/10/92: change rlfac_y from 0.25 to 0.50.  will use both y & z thrusters
   up until time of bleed down.  bike code used only z thruster after
   w0 was greater than half final spin rate.
   2/10/92: changed s2fac from 0.5 to 0.1;  this will allow faster recovery
   from large initial tip-off rates.  see spin_d.c for more.
   2/11/92: added s0fac and set to 0.25  (was hard coded to 0.5 in spin_d.c
   before).
   2/11/92: changed rlfac_y to 0.5 from 0.6.  0.6 is value used for lace.
   2/11/92: removed s0fac
   2/12/92: added/changed some comments
   2/12/92: removed notch_delay.  added rdbm_delay.
   2/18/92: removed ^Z at end of file.
   3/5/92:  modified a comment
   3/17/92: added puls_7,ptest_7,npbleed,npdivert initialization.
   3/27/92: set w0_spin= 270 deg/s to reduce N2 usage.  want to
   have 33 percent reserve at end of mission.  360 deg/s results in
   22 percent reserve.
   4/27/92: added enable_x initialization.  set w0_spin back to 360 deg/s.
   don't plan to do as ambitious a mission.  changed mass property data
   for RV only.
   4/28/92: remove snum initialization.  Kozlowski will take care of this
   in his c code.
   4/29/92: move all calcs in ac_hard that depend on constants to ac_soft.
   this is done so that if constants are changed (e.g. during air-bearing
   tests, that dependent constants will also be changed.
   4/29/92: added xoff_delay.
   4/30/92: added yoff_delay,disable_x,disable_y,disable_z.
   added enable_y,enable_z.  added min_yuse.  added iptest,pptest.
   set s2fac=.5 from 0.1.  having problems with initial spin-up control
   when y nozzles not used (min_yuse=1).  see spin_d.c for more.
   5/1/92: added euler_seq initialization to ac_soft.  added euler_seq0
   to ac_hard.
*********************************************************************+*/
#define DTOR 0.017453293

void ac_soft()
{
/*  extern short int snum;   nav structure variable */

  puls_x= 0;
  puls_y= 0;
  puls_z= 0;
  puls_1 = 0;
  puls_2 = 0;
  puls_3 = 0;
  puls_4 = 0;
  puls_5 = 0;
  puls_6 = 0;
  puls_7 = 0;
  nppos_x= 0;
  nppos_y= 0;
  nppos_z= 0;
  npneg_x= 0;
  npneg_y= 0;
  npneg_z= 0;
  npbleed= 0;
  npdivert= 0;
  despin_seq= 1;
  rate_check= 1;
  enable_x= 1;
  enable_y= 1;
  enable_z= 1;

  w0=0;
  tta= 0.0;
  tpa= 0.0;
  ton_x_eq= 0.0;

  spin_mode=(short int)INITIALIZE;
  euler_seq= euler_seq0;
  tnum = 1;

  rdbm_y_hi= 3.0*rdbm_y_lo;  /* rate deadband for spin-up  */
  rdb_x24= 0.02*w0_spin;     /* 2 percent of w0_spin */

  /* lever arms and inertias are constant for this mission */
  arm_x1 = arm_x0;  /* middle thruster lever arms */
  arm_y1 = arm_y0;
  arm_z1 = arm_z0;
  arm_x2 = arm_x0;  /* final thruster lever arms */
  arm_y2 = arm_y0;
  arm_z2 = arm_z0;
  ixx1 =    ixx0;  /* middle inertias  */
  inn1 =    inn0;
  ixx2 =    ixx0;  /* final inertias  */
  inn2 =    inn0;

}
/*****************************************************************
   function: ac_hard

   description
     one-time acs initialization, hard power reset

   inputs

   outputs:

   notes:

   called routines:

******************************************************************/

void ac_hard()
{

/* targeting from previous program */

  te11_1con =   0.45641636;
  te12_1con =   0.72579190;
  te13_1con =  -0.51469430;
  te21_1con =   0.79232923;
  te22_1con =  -0.06833381;
  te23_1con =   0.60625481;
  te31_1con =   0.40484380;
  te32_1con =  -0.68451195;
  te33_1con =  -0.60625481;

  te11_2con = 0.0;
  te12_2con = 0.0;
  te13_2con = 0.0;
  te21_2con = 0.0;
  te22_2con = 0.0;
  te23_2con = 0.0;
  te31_2con = 0.0;
  te32_2con = 0.0;
  te33_2con = 0.0;
  ph_deploy = 0.0;

  te11_3con = 0.0;
  te12_3con = 0.0;
  te13_3con = 0.0;
  te21_3con = 0.0;
  te22_3con = 0.0;
  te23_3con = 0.0;
  te31_3con = 0.0;
  te32_3con = 0.0;
  te33_3con = 0.0;

/* built-in-test
*/
  bitest= 0;

/* built-in-test, nozzle pulsing
*/
  ptalo= 0;
  nptest=0;
  iptest=0;
  pptest=1;
  ptest_1=0;
  ptest_2=0;
  ptest_3=0;
  ptest_4=0;
  ptest_5=0;
  ptest_6=0;
  ptest_7=0;

/* thruster enable/disable variables
*/
  min_yuse= 0;
  disable_x= 0;
  disable_y= 0;
  disable_z= 0;


/* built-in-test, airbearing
*/
  abtest=0;
  az_t1= 0.0;
  el_t1= 0.0;
  rl_t1= 0.0;
  tdel1= 0.0;
  az_t2= 0.0;
  el_t2= 0.0;
  rl_t2= 0.0;
  tdel2= 0.0;
  az_t3= 0.0;
  el_t3= 0.0;
  rl_t3= 0.0;
  tdel3= 0.0;

/* controller parameters
*/
  irot= 1;
  inotch= 1;
  euler_seq0= 1;   /* initial euler seq */

  adjust_delay= 30.0; /* time after spinup that deadbands are adjusted */
  rdbm_delay=   10.0; /* time after spinup that deadbands are reduced */
  xoff_delay=    5.0; /* time after spinup that x axis control is disabled */
  yoff_delay=    5.0; /* time after spinup that y axis control is disabled */
  m2count_max = 12;
  m2quiet_max =  4;
  m4count_max = 12;
  m4quiet_max =  4;

  tqfac= 1.0;
  tdelay= 0.05;

  w0_spin = 6.28;   /* 360 deg/s */

  rfac_x= 2.0;
  rfac_y= 2.0;
  pfac_y= 2.0;
  rdbm_x= 0.00175;
  rdbm_y_lo= 0.00175;        /* rate deadband at spin rate */
  pfac_x = 1.0;
  pfac_y0= 1.0;
  pdb_x0=  0.0087;
  pdb_y0=  0.0087;

  kfac_yz= 1.0;      /* scale factor on krp_yz */

  rlfac_x=  1.0;
  rlfac_y=  0.60;   /* this value is for using both y & z thrusters */
  rlfac_y0= 0.50;

  s2fac = 0.50;     /* used to select rate limit (rlim_yz) */
  m2fac = 1.732;    /* m2fac*m4fac = 3.0 */
  m4fac = 1.732;
  m4tol_x = 0.08727;  /* 5.0 deg; max y thr duty cycle is 2.28 % */
  m4tol_y = 0.08727;  /* 5.0 deg; max y thr duty cycle is 5.55 % */

/* values before mission changed to RV only
  arm_x0 = 0.960;
  arm_y0 = 2.75;
  arm_z0 = 2.75;
  arm_x1 = 0.960;
  arm_y1 = 3.17;
  arm_z1 = 3.17;
  arm_x2 = 0.960;
  arm_y2 = 3.12;
  arm_z2 = 3.12;
*/
  arm_x0 = 0.960;  /* initial thruster lever arms */
  arm_y0 = 3.12;
  arm_z0 = 3.12;

/* thr_lo adjusted for gbi/eris flights 1 & 2 test results - initial spin-up;
   the despin & 2nd spin-up had lower numbers by about 13 % ;
   however, this may have been because of plume impingement??
   the 0.68 number used below is very close to results documented
   in R.Mayes memo 3/21/91  (0.66 lb flowing, 145 psi regulator).
   thr_hi is lab test result as documented in R.Mayes memo 3/21/90
*/
  thr_lo=  0.68;
  thr_hi=  1.23;

/*  values before mission changed to RV only
  ixx0 =    8.11;
  inn0 =    85.60;
  ixx1 =    7.42;
  inn1 =    61.64;
  ixx2 =    6.97;
  inn2 =    57.20;
*/
  ixx0 =    8.11;  /* initial inertias */
  inn0 =    85.60;

  therrl_r = 0.0;
  pserrl_r = 0.0;
  pherrl = 0.0;

  thd_r = 0.0;
  psd_r = 0.0;

  thdel_r = 0.0;
  psdel_r = 0.0;
  phdel = 0.0;

  ps_err_pz = 0.0;
  th_err_pz = 0.0;
}




