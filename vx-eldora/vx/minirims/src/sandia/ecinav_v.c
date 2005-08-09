/*+**********************************************************************

NAME:		ecinav_vars.c

SYNOPSIS: 	Global Structure Variable declarations
                for ECI frame Navigation parameters.

INPUTS: 	None.

PROCESSING: 	None.


OUTPUTS: 	None.

RETURNS: 	None.

    David M. Kozlowski 1-5-89

**********************************************************************+*/

struct ecinav_vars {
  double tni11, tni12, tni13;	/* Transformation from launcher Geographic */
  double tni21, tni22, tni23;	/* to ECI frame. */
  double tni31, tni32, tni33;

  double eb11, eb12, eb13;	/* Body to ECI transformation Matrix */
  double eb21, eb22, eb23;
  double eb31, eb32, eb33;

  double pxeci, pyeci, pzeci;	/* x,y,z position and velocity, ECI frame */
  double vxeci, vyeci, vzeci;

  double dv_x, dv_y, dv_z;	/* Accumulated 1st integral of specific force.*/
				/* ECI frame, reset to 0 at startup and */
				/* again at launch. */
  } eci;
