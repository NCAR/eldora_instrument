/*+************************************************************
  this file contains the following modules:
    ac_slow
    ac_fast

  R.S.Edmunds, 9132

  last modified:
  4/12/89: modified airbear option in function ac_slow to go with bitest.
  4/25/89: modified logic in ac_fast to handle
  mode 4 ( spin_mode >= 2 )
  9/28/89: allow only one axis control torque at a time.
  this is to prevent overdriving the pressure regulators.
  10/1/91: modify comments to add spin_mode=5 case.  modify logic
  to include spin_mode=5 case.
  10/4/91: add rate_check logic to spin_time calculation.  this
  eliminates the possibility of spin_time incrementing immediately
  following separation due to a real (or erroneous) value of w0
  associated with abnormally high tipoff rates.
  10/4/91: modified calc of w0, to notch w0h instead of
  phs.  see spin_b for additional comments.
-------------------------------------------------------------------
  2/3/92: begin mods for kite code.  all comments for modifications
  moved to top of file.
  2/11/92: remove constraint that prevented x & y thrusters to
  fire at same time.  this constraint was imposed when using the
  conax regulator.  this regulator had less flow capability than
  the stearer regulator planned for kite.
  2/11/92: added constraint to inhibit x thrusters if rate_check=1.
  don't want to allow spin-up until tip-off rates are brought under control.
  2/12/92: added adjust_time timer for use with adjust deadband function.
  2/12/92: modify spin_time initialization so that spin_time is held to
  zero for spin_mode= 0,3,5 and not spin_mode=1.
  2/12/92: modify spin_time to start incrementing at W0_NOTCH_MIN= 1.0 rad/s.
  had some problems with 0.5 deg misalignment when spin_time was
  incremented based on:   if(w0 >= w0_spin - rdb_x && rate_check == 0).
  2/12/92: use notch_on variable to turn on notch instead of spin_time.
  return to incrementing spin_time with if(w0 >= w0_spin - rdb_x
  && rate_check == 0).
  2/18/92: comment clean-up.  bracket clean-up.
  4/27/92: added enable_x varible.  this variable is set to zero in
  spin_c.c after spin_up is complete.
  4/29/92: moved calc of enable_x to this file.  set enable_x based
  on spin_time and xoff_delay.  set adjust_time equal to spin_time
  since ILR_SEP is no longer in mission.
  4/30/92: added enable_x,y,z logic based on bitest and disable_x,y,z.
  added enable_y logic based on spin_time and yoff_delay.  may not want
  to use y thrusters after spinup.  also add min_yuse logic to turn
  off y thruster based on rate_check.  minimum use of y thruster is
  for initial tip-off rate reduction.  modify logic on spin_time so that
  it will continue to increment after enable_x set to 0 even if spin
  rate drops out of rate deadband.
***************************************************************/

#include "spin.h"
#include "math.h"
#include "fltseqdat.h"
extern short int snum;  /* from nav data structure */

/*****************************************************************
   function: ac_slow()

   description:
     c module to provide attitude control for spinning spacecraft.

   inputs:
     spin_mode: control system operating mode:
           0: one time initialization
           1: estimation only
           2: spinning control (nominal deadbands)
           3: despun control
           4: relaxed spinning control (large deadbands)
           5: despin without roll angle control
     w0:  estimated value of spin rate, phd (rad/s)
     ton_x_eq: equivalent x thruster on time (sec)

   outputs:
     thc,psc: commanded values of th,ps (rad)
     pdb_x,y,z: position deadband (rad)
     rdb_x,y,z: rate deadband (rad/s)
     krp_x,y,z: rate-to-position gain (sec)
     plim_x,y,z: position limit (rad)
     torq_x,y,z: x,y,z torque levels (ft-lb)
     sa0,sa1,sa2,b1,b2: notch coefficients
     s_wn: nutation frequency (rad/s)
     ixx:
     inn:

   notes:
     thc,psc are commanded Euler angles associated with a
       2-3-1 (Y-Z-X) rotation from
       body to inertial coordinate frame.

   called routines:
     notch_co, deadband, massp

******************************************************************/
#define DBUG_AC_SLOW 0

void ac_slow()
{
   void notch_co(),deadband(),massp();
   void tframe();
   void abframe();

/* initialization for inputs normally computed in ac_fast
*/
   if(spin_mode==0) {
     tframe();
#if(DBUG_AC_SLOW)
     printf("\nac_slow: spin_mode,bitest,abtest %d %d %d",spin_mode,bitest,abtest);
#endif
     if(bitest==1 && abtest==1)
       abframe();
   }

   /* compute commanded Euler angles (thc,psc)
   */
   thc= 0.0;
   psc= 0.0;

   /* compute filter coefficients
      w0= notch center freq (rad/s)
      sa0,sa1,sa2,b1,b2= notch coefficients
   */
   notch_co();

   /* compute mass properties and nutation freq and torques
   */
   massp();

#if(DBUG_AC_SLOW)
  printf("\nac_slow: after massp");
#endif

   /* compute postion deadbands, rate-to-position gain, and position limits
   */
   deadband();

#if(DBUG_AC_SLOW)
  printf("\nac_slow: after deadband");
#endif

}

/*****************************************************************
   function: ac_fast()

   description:
     c module to provide attitude control for spinning spacecraft.

   inputs:
     spin_mode: control system operating mode:
           0: one time initialization
           1: estimation only
           2: spinning control
           3: despun control
           4: relaxed spinning control
           5: despin without roll angle control
     irot: rotation flag
           0: disable rotation
           1: enable rotation
     inotch: notch flag
           0: no notch
           1: notch
     eb11, ..., eb33: elements of body-nav to earth-centered-inertial
                      transformation matrix
     thc,psc: commanded values of th,ps from guidance (rad)
     pdb_x,y,z: position deadbands (rad)
     rdb_x,y,z: rate deadbands (rad/s)
     krp_x,y,z: rate-to-position gain (sec)
     plim_x,y,z: position limit (rad)
     sa0,sa1,sa2,b1,b2: notch coefficients
     torq_x,y,z: torque levels (ft-lb)
     s_wn: nutation frequency (rad/s)
     ixx:
     inn:

   outputs:
     ths,pss,phs: th,ps,ph Euler angles computed in euler (rad)
     thn,psn: ths,pss after notching (rad)
     thh,thdh: estimated values of th (rad), thd (rad/s)
     psh,psdh: estimated values of ps (rad), psd (rad/s)
     phh: estimated value of Euler angle ph (rad).
     w0:  estimated value of spin rate, phd (rad/s)
     s_alf: rotation angle (rad)
     enabl_th,ps,x: torque enable flag
          0: no torque
          1: torque enable
     puls_x,y,z: x,y,z pulse flag
          0: no pulse
          1: pulse
     tta,tpa: applied th, ps torques (no units)
     npuls_x,y,z: x,y,z cumulative thruster pulses (pulses)
     ton_x_eq: eqivalent x thruster on-time (sec)
     dthcen: computed change in th circle center (rad)
     dpscen: computed change in ps circle center (rad)

   notes:
     th,ps,ph are Euler angles associated with a 2-3-1 (Y-Z-X) rotation from
       body to inertial coordinate frame.  thd,psd,phd are the associated
       Euler angle rates.

   called routines:
     notch_th,notch_ps,notch_ph, pulse_gen
     estim_th_ps,estim_ph
     monitor,euler,rotate

******************************************************************/

#define DBUG_AC_FAST 0
#define W0_NOTCH_MIN 1.0

void ac_fast()
{
   double notch_th(double ths)
         ,notch_ps(double pss)
         ,notch_ph(double phs)
         ;

   void pulse_gen(), estim_th_ps(), estim_ph(), s_euler(), monitor();
   void rotate(), control_yz(), control_x();
   static int euler_last;

   /* compute Euler angles (ths,pss,phs) relating body-nav to
        (nominal) target-inertial frame
   */
   s_euler();

   /* check for change in euler angle rotation sequence
   */
   if(spin_mode == 0)
     euler_last = euler_seq;

   if(euler_seq == euler_last)
     euler_init = 0;
   else
     euler_init = 1;
   euler_last = euler_seq;


/* spin timer, used for selecting minimum rate deadband.
   inhibit timer if controller is in initial rate reduction mode.
*/
   if(spin_mode == 0 || spin_mode == 3 || spin_mode == 5)
     spin_time= 0.0;
   if(w0 >= w0_spin - rdb_x && rate_check == 0
   || enable_x == 0)
     spin_time += DT;

/* set notch_on variable */
   if(fabs(w0) > W0_NOTCH_MIN && rate_check == 0)
     notch_on= 1;
   else
     notch_on= 0;

/* adjust timer, used for allowing deadband adjust.
   keep small deadbands until after ILR separation.
   if(snum < ILR_SEP)
     adjust_time= 0.0;
   else
     adjust_time += DT;
*/
   adjust_time= spin_time;

   /* notch out spin freq from ths, pss, phs
   */
   if(inotch) {
     thn= notch_th(ths);
     psn= notch_ps(pss);
   }
   else {
     thn= ths;
     psn= pss;
   }

#if DBUG_AC_FAST
   printf("\nac_fast: spin_mode,ths,pss %d %lg %lg",spin_mode,ths,pss);
   printf("\nac_fast: thn,thh %lg %lg",ths,thh);
#endif

   /* estimate Euler angles and rates thh,thhd, psh,pshd, phh,w0
   */
   estim_th_ps();
   estim_ph();

   /* compute spin rate
   the "exact" exquations are these:
     if(euler_seq == 1)
       w0h=  thdh*sin(psh) + phdh;
     else if(euler_seq == 2)
       w0h= -psdh*sin(thh) + phdh;
   however, thdh,psdh,thh,psh are not "exact", so w0h computed
   this way isn't greatly improved over what is used below.
   in any case, once steady state conditions are achieved for
   x axis pointing, the spin rate computed below is ok, since
   the error terms are 2nd order.
   */
   w0h=  phdh;

   if(inotch)
      w0n= notch_ph(w0h);
   else
      w0n= w0h;

   if(notch_on == 1)
      w0= w0n;
   else
      w0= w0h;

   /* calculate Euler angle errors
   */
   therr= thh - thc;
   pserr= psh - psc;

   /* test for 180 to -180 deg switch point
      this logic allows th to swing through 180 point
      abs(ps) must remain less than pi/2
   */
   if(therr > PI)
     therr= therr - TPI;
   else if(therr < -PI)
     therr= therr + TPI;

   if(pserr > PI)
     pserr= pserr - TPI;
   else if(pserr < -PI)
     pserr= pserr + TPI;

   /* compute rotation angle and rotated position errors and rates
   */
   if(irot==1)
     rotate();
   else {
     s_alf= 0.0;
     therr_r= therr;
     pserr_r= pserr;
     thd_r= thdh;
     psd_r= psdh;
   }

   /* thruster enable/disable logic
   */
   if(bitest == 1 && disable_x == 1
   || spin_time >= xoff_delay )
     enable_x= 0;
   else
     enable_x= 1;

   if(bitest == 1 && disable_y == 1
   || spin_time >= yoff_delay
   || min_yuse == 1 && rate_check == 0
   || snum >= (short int)BLEED_DOWN )
     enable_y= 0;
   else
     enable_y= 1;

   if(bitest == 1 && disable_z == 1)
     enable_z= 0;
   else
     enable_z= 1;



   /* compute applied control torques
      use phs instead of phh to avoid additional phase shift introduced
        by estimator.
   */

   if(spin_mode >= 2 && enable_x==1)
     control_x();
   else
     puls_x= 0;

   if(spin_mode >= 2)
     control_yz();
   else {
     enabl_th= 0;
     enabl_ps= 0;
     puls_y= 0;
     puls_z= 0;
     tta= 0.0;
     tpa= 0.0;
   }

   /* resolve conflicts in pulse commands.
      x & z cannot both be on at same time.
      give priority to z thruster.  even for rather large
      theta/psi errors this takes little time away from x
      thruster.  on the other hand, if x is given priority
      the spin_up, spin_down is so long that y/z control
      can get pretty sloppy.

      want to inhibit spin-up if rate_check=1.  better to
      bring tip-off rates under control before spin-up.
   */
   if( ABS(puls_z) > 0 || rate_check==1)
     puls_x = 0;

   /* compute thruster pulses and on times
      puls_1,2,3,4,5,6
      nppos_x,nppos_y,nppos_z,npneg_x,npneg_y,npneg_z,ton_x_eq
   */
   pulse_gen();

   /* monitor circle-center change (dthcen,dpscen)
   */
   monitor();

}


