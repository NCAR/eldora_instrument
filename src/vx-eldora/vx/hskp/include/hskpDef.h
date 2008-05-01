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
 * Revision 1.9  1999/09/27  15:57:00  eric
 * changed error limits on slow cal.
 *
 * Revision 1.8  1997/08/20 20:05:30  eric
 * set new limits on test pulse power and velocity based on
 * statistics from fastex.
 *
 * Revision 1.7  1996/10/29  23:28:18  craig
 * *** empty log message ***
 *
 * Revision 1.6  1996/10/11  23:27:24  craig
 * *** empty log message ***
 *
 * Revision 1.5  1996/02/21  17:21:12  craig
 * *** empty log message ***
 *
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
#define TP_ATTEN_MAX   70                  /* was 40 before tp amps added */
#define TP_ATTEN_ST    0
#define MAX_TP_VEL_DIFF 0.25               /* In m/s */
#define MAX_TP_PWR_DIFF 1.25               /* In dbm */

/*Definitions of the pattern that point the waveguide switch at each antenna*/
#define WG_SW_FORE 0x00
#define WG_SW_AFT  0xFF
#define WG_SW_BASE 0xA000









