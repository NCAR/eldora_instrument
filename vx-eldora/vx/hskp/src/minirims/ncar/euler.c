/*+**********************************************************************

      NAME: euler.c

  SYNOPSIS: Computes Pre-launch Euler angles from A matrix and resolver
	      angle.  Computes flight Euler angles using A matrix only.
	    The global variable euler_flag determines which set of angles
	       are calculated.  initial() sets euler_flag to LAUNCH_SET at
               startup.
	
    INPUTS: A matrix values, resolver angle, and launch detect flag.

PROCESSING:  (1) Launch Euler Angles (1-2-3):

		   	      -a22*sin(rmrd) + a23*cos(rmrd)
		psi = arctan( ------------------------------  )
			      -a12*sin(rmrd) + a13*cos(rmrd)

		theta = asin( -a32*sin(rmrd) + a33*cos(rmrd) )

		                 a32*cos(rmrd) + a33*sin(rmrd)  	
		phi = arctan(   ----------------------------- )
				             -a31

	------------------------------------------------------------------
             (2) Flight Euler Angles (3-2-1):

		psi = arctan( a21/a11)

		theta = asin(-a31)

		phi = arctan(a32/a33)
	------------------------------------------------------------------

   OUTPUTS:  Global euler angles ea_psi, ea_theta, ea_phi

   RETURNS:  None.

	By:  Earl Creel   May 26, 1988

**********************************************************************+*/

#define NO_MINIR_SCOPE
#include "minirGbl.h"
#include "math.h"

#define TWO_PI      6.2831853
#define LAUNCH_SET  0
#define FLIGHT_SET  1
#define DTOR        0.017453293

double ea_psi, ea_theta, ea_phi,ea_phi1;
short int euler_flag;

void
euler()
{
  double arctan();
  extern double ea_psi, ea_theta, ea_phi;
  extern short int euler_flag;

  double srmrd, crmrd;
  double sarg, carg;

  register NAV_VARS *n;
  n = nav_ptr;

  if( euler_flag == LAUNCH_SET)	  /* compute Launch Euler angles */
    {
    if(n->mode != 0x0)		/* don't use rmrd during init, hold or align */
      {
      sarg = n->a23;
      carg = n->a13;
      ea_psi = arctan(sarg,carg);

      ea_theta = asin(n->a33);

      ea_phi = arctan(n->a32,-n->a31);
      }
    else	/* in nav mode, platform roll inertial, use resolver */
      {
      srmrd = sin( (double)n->rmrd);
      crmrd = cos( (double)n->rmrd);

      sarg = -n->a22*srmrd + n->a23*crmrd;
      carg = -n->a12*srmrd + n->a13*crmrd;
      ea_psi =  arctan(sarg,carg);

      ea_theta = asin(-n->a32*srmrd + n->a33*crmrd);

      sarg =  n->a32*crmrd + n->a33*srmrd;
      ea_phi = arctan(sarg,-n->a31);
      }
    }
  else if(euler_flag == FLIGHT_SET)  /* Compute flight Euler angles */
    {
    srmrd = sin(n->rmrd);
    crmrd = cos(n->rmrd);
    sarg = n->a32*crmrd + n->a33*srmrd;
    carg = (-n->a32*srmrd + n->a33*crmrd);
    ea_psi = arctan(n->a21, n->a11);
    ea_theta = asin(-n->a31);
    ea_phi = arctan(n->a32, n->a33);
    ea_phi1 = arctan(sarg,carg);    /* Body to geographic roll angle */
    }

  if(ea_psi < 0.0)
    {
      ea_psi += TWO_PI;
    }
} /* end of euler() */

/*+**********************************************************************

      NAME: A_Matrix_Reset()

  SYNOPSIS: Resets the entire A matrix based on the two current platform
	    tilt angles and an azimuth angle input by the user (intended
	    for use with optical alignment initialization).

    INPUTS: euler_flag -- identifies current Euler angle set (Launch or Flight)
   	    op_az      -- azimuth angle (in deg)
	    ea_theta   -- platform pitch angle (in rad)
	    ea_phi     -- platform roll angle (in rad)

PROCESSING: 1. op_az is converted from degrees to radians.
	    2. The sin and cos of each Euler angle is computed
	    3. All 9 elements of the A matrix are computed.  Computations
	         depend on the state of euler_flag.
	    Notes:
		The platform must be caged before calling this routine.


   OUTPUTS:  A Matrix  -- The transformation from IMU platform frame to
			  the Local Level Geographic Frame

   RETURNS:  None.

	By:  Earl Creel   October 8, 1988

**********************************************************************+*/

void
A_Matrix_Reset()
{
  extern double op_az;
  extern double ea_theta, ea_phi;
  extern short int euler_flag;

  register double spsi,cpsi, stheta,ctheta, sphi,cphi;

  register NAV_VARS *n;
  n = nav_ptr;

  spsi = sin(op_az*DTOR);    stheta = sin(ea_theta);    sphi = sin(ea_phi);
  cpsi = cos(op_az*DTOR);    ctheta = cos(ea_theta);    cphi = cos(ea_phi);

  if(euler_flag == LAUNCH_SET)
    {
    n->a11 =  cpsi * stheta * cphi - spsi * sphi;
    n->a12 = -cpsi * stheta * sphi - spsi * cphi;
    n->a13 =  cpsi * ctheta;

    n->a21 =  spsi * stheta * cphi + cpsi * sphi;
    n->a22 = -spsi * stheta * sphi + cpsi * cphi;
    n->a23 =  spsi * ctheta;

    n->a31 = -ctheta * cphi;
    n->a32 =  ctheta * sphi;
    n->a33 =  stheta;
    }

  else if(euler_flag == FLIGHT_SET)
    {
    n->a11 =  cpsi * ctheta;
    n->a12 = -spsi * cphi + cpsi * stheta * sphi;
    n->a13 =  spsi * sphi + cpsi * stheta * cphi;

    n->a21 =  spsi * ctheta;
    n->a22 =  cpsi * cphi + spsi * stheta * sphi;
    n->a23 = -cpsi * sphi + spsi * stheta * cphi;

    n->a31 = -stheta;
    n->a32 =  ctheta * sphi;
    n->a33 =  ctheta * cphi;
    }

} /* End of A_Matrix_Reset() */
