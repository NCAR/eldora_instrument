/*
 *	$Id$
 *
 *	Module:	hskpDef.h	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *        
 */

/* Define a data ray structure made out of a group of header structures */

struct DATARAY
  {
  struct ray_i this_rayi;
  struct platform_i this_plat;
  struct field_parameter_data this_fdata;
  }; 

/* Variables associated with pointing angles - These must be updated if the
   transmitted frequency changes! Calculate the SIN and COS of the tilt angles
   here and put them in defines statements to make the calculation of Azimuth
   and Elevation quicker in the code.

       THESE ARE FREQUENCY DEPENDENT !! FOR NOW USE 9.4 GHZ NUMBERS FOR THE
       FORE ANTENNA AND 9.5 GHZ NUMBERS FOR THE AFT ANTENNA 
*/

#define ENCODER_ZERO_ANGLE     0
#define FORE_SQUINT_ANGLE       -0.8
#define AFT_SQUINT_ANGLE        0.05
#define FORE_TILT_ANGLE        18.5
#define AFT_TILT_ANGLE        -17.5
#define COS_FTILT              0.948324
#define COS_ATILT              0.953717


/* Convert from degrees per second to RPM */

#define DEGPERSEC_TO_RPM   0.1666667


/* Definitions to handle the test pulse */
#define TP_ATTEN_STEP  10
#define TP_ATTEN_MAX   120