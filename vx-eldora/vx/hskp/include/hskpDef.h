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
 * Revision 1.4  1993/09/01  20:07:30  craig
 * *** empty log message ***
 *
 * Revision 1.3  1992/11/03  22:53:56  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/19  19:33:48  craig
 * Initial revision
 *
 *        
 */

/* Define a data ray structure made out of a group of header structures */

struct DATARAY
  {
  struct ray_i this_rayi;
  struct platform_i this_plat;
  struct field_parameter_data this_fdata;
  }; 

/* Convert from degrees per second to RPM */

#define DEGPERSEC_TO_RPM   0.1666667


/* Definitions to handle the test pulse */
#define TP_ATTEN_STEP  10
#define TP_ATTEN_MAX   120


/*Definitions of the pattern that point the waveguide switch at each antenna*/
#define WG_SW_FORE 0x01
#define WG_SW_AFT  0x00
