/*+**********************************************************************

NAME:           acs.h

SYNOPSIS:       Header INCLUDE file for external declarations
                  of ACS parameters.

INPUTS:         None.

PROCESSING:     None.


OUTPUTS:        None.

RETURNS:        None.

    David M. Kozlowski  1-5-89
mods:
   9/30/91: rse: added rate_check
   10/2/91: rse: added adj_count,notch_delay
   10/3/91: rse: added spin_time, removed adj_count
   10/3/91: rse: added w0h,w0n
   ---------------------------------------
   2/3/92:  begin kite mods
   2/7/92:  added pdb_xt,pdb_yzt,rdb_xt,rdb_yzt  (see spin_d.c)
   2/11/92: added s0fac
   2/11/92: added kfac_yz
   2/11/92: removed s0fac
   2/12/92: added adjust_time
   2/12/92: removed notch_delay.  added rdbm_delay, notch_on.
   added adjust_delay.  added rdbm_y_lo,rdbm_y_hi.
   3/17/92: added puls_7,ptest_7,npbleed,npdivert.
   4/27/92: added enable_x.
   4/29/92: added xoff_delay.
   4/30/92: added yoff_delay,disable_x,disable_y,disable_z.
   added enable_y,enable_z.  added min_yuse.  added iptest,pptest.
   5/1/92:  added euler_seq0.
**********************************************************************+*/

/* targeting data */
extern double te11, te12, te13;
extern double te21, te22, te23;
extern double te31, te32, te33;

/* Two sets of constants used to initialize */
/* teij */

extern double te11_1con, te12_1con, te13_1con;  /* ECI to Target Inertial  */
extern double te21_1con, te22_1con, te23_1con;   /* Transformation for Cold */
extern double te31_1con, te32_1con, te33_1con;   /* Gas ACS Maneuver #1.    */

extern double te11_2con, te12_2con, te13_2con;   /* ECI to Target Inertial    */
extern double te21_2con, te22_2con, te23_2con;   /* Transformation for Cold   */
extern double te31_2con, te32_2con, te33_2con;   /* Gas ACS Maneuver #2, used */
extern double ph_deploy;

extern double te11_3con, te12_3con, te13_3con;   /* ECI to Target Inertial    */
extern double te21_3con, te22_3con, te23_3con;   /* Transformation for Cold   */
extern double te31_3con, te32_3con, te33_3con;   /* Gas ACS Maneuver #3, used */

/* built-in-test parameters */
extern
short int bitest;

/* nozzle pulse test parameters */
extern
long ptalo;

extern
short int nptest,iptest,pptest
         ,ptest_1,ptest_2,ptest_3,ptest_4,ptest_5,ptest_6,ptest_7;

/* air-bearing test parameters */
extern
short int abtest;

extern
double az_t1, el_t1, rl_t1, tdel1
      ,az_t2, el_t2, rl_t2, tdel2
      ,az_t3, el_t3, rl_t3, tdel3;


/* design parameters */
extern
short int irot,inotch
         ,m2count_max,m2quiet_max
         ,m4count_max,m4quiet_max;

extern
double tqfac,tdelay
      ,rfac_x,rfac_y,pfac_y,kfac_yz
      ,rdbm_x,rdbm_y,rdb_x24,rdbm_y_lo,rdbm_y_hi
      ,pfac_x,pfac_y0
      ,pdb_x0,pdb_y0
      ,rlfac_x,rlfac_y,rlfac_y0
      ,s2fac,m2fac,m4fac,m4tol_x,m4tol_y
      ,arm_x0,arm_y0,arm_z0
      ,arm_x1,arm_y1,arm_z1
      ,arm_x2,arm_y2,arm_z2
      ,thr_lo,thr_hi
      ,w0_spin, spin_time,adjust_time
      ,ixx0,inn0
      ,ixx1,inn1
      ,ixx2,inn2
      ,rdbm_delay,adjust_delay,xoff_delay,yoff_delay;

/* controller outputs */
extern
double ths,pss,phs
      ,thc,psc,thn,psn
      ,thh,thdh,psh,psdh,phh,phdh
      ,w0h,w0n,w0
      ,s_alf
      ,therr,pserr,therr_r,pserr_r,thd_r,psd_r
      ,ps_ang_pz,th_ang_pz,ps_err_pz,th_err_pz
      ,therrl_r,pserrl_r,thdel_r,psdel_r
      ,tta,tpa
      ,pherr,pherrl,phdel
      ,dthcen,dpscen
      ,ton_x_eq
      ,sa0,sa1,sa2,b1,b2
      ,pdb_x,pdb_yz,rdb_x,rdb_yz
      ,pdb_xt,pdb_yzt,rdb_xt,rdb_yzt
      ,mfac_x,mfac_yz
      ,krp_x,krp_yz,plim_x,plim_yz
      ,rlim_x,rlim_yz,rlim_yz0,rlim_yz1
      ,ixx,inn,arm_x,arm_y,arm_z,s_wn
      ,torq_x,torq_y,torq_z,torq_yz;

extern
short int enabl_th,enabl_ps
         ,puls_x, puls_y, puls_z
         ,nppos_x, npneg_x, nppos_y, npneg_y, nppos_z, npneg_z
         ,npbleed, npdivert
         ,puls_1, puls_2, puls_3, puls_4, puls_5, puls_6, puls_7
         ,despin_seq, euler_seq, euler_seq0, euler_init, rate_check
         ,min_yuse, notch_on
         ,enable_x,enable_y,enable_z, disable_x,disable_y,disable_z
         ;

/* flight sequencer outputs */
extern
short int spin_mode, tnum;

/* parameters
   PI:     pi
   TPI:     2 pi
   HPI:    1/2 pi
   DTR:    degrees to radians conversion factor (rad/deg)
   DT:     sample period (sec)
*/
#define PI   3.141593
#define TPI  6.283185
#define HPI  1.570796
#define DTR .0174533
#define DT (1.0/64.0)

/* acs mode defines */
#define INITIALIZE     0
#define ESTIMATE_ONLY  1
#define SPIN           2
#define DESPIN         3
#define ACS_SLEEP      4

/* utility macros
*/
#define ABS(x) (((x)< 0)?(-(x)):(x))
#define SIGN(x,y) (((y)< 0)?(-ABS(x)):(ABS(x)))
#define MIN(x,y) (((x)< (y))?(x):(y ))
#define MAX(x,y) (((x)>=(y))?(x):(y ))
#define BOUND(xlo,xhi,x) MIN(MAX(x,xlo),xhi)

/*****************************************************************
   macro: LOW_INDEX()

   description:
     macro to find index (ilo) such that:
     vtab[ilo] <= v <= vtab[ilo+1]
     subject to contraints:
        0 <= ilo <= npts-2
     npts is number of elements in vtab[].

   inputs:
     vtab,v,ilo,npts

   outputs:
     ilo

   notes:

   last modified:

******************************************************************/

#define LOW_INDEX(vtab,v,ilo,npts) \
      while (v < vtab[ilo] && ilo > 0) --ilo; \
      while (v > vtab[ilo+1] && ilo+2 < npts) ++ilo


/*****************************************************************
   macro: INTERP

   description:
     interpolation macro

   inputs:
     x is independent variable.
     xlo and xhi are table values for indep var;
     ylo(xlo) and yhi(xhi) are table values for dep var.

   outputs:
    interpolated value of y

   notes:
     this macro will extrapolate if independent var x
     is not constrained.

   last modified:

******************************************************************/

#define INTERP(xlo,xhi,x,ylo,yhi) \
        ( (ylo) + ((yhi)-(ylo)) * ((x)-(xlo)) / ((xhi)-(xlo)) )

