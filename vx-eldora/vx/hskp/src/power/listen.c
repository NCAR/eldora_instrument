/*
 *	$Id$
 *
 *	Module:	start_ieee.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/01  16:48:23  craig
 * Initial revision
 *
 *
 * description: The purpose of this module is to 
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <vxWorks.h>
#include <stdioLib.h>
#include <math.h>
#include <string.h>
#include <pwrDef.h>
#include <pwrFunc.h>

void listen()
{
/**************  TURN MZ7500 INTO LISTENER *************/
  
  /* TURN BUS AROUND TO LISTEN */
  /* TAKE CONTROL BY ASSERTING ATN */

/*********** CHANNEL 1 **************/

  *g1acr = 0x0c;	  
  *g1acr = 0x0a; /* UNTALK MZ7500*/
  *g1dido = 0x3f; /* UNLISTEN PPM */
  *g1dido = 0x44; /* TALK ADDRESS PPM */
  *g1adr = 0x20; /* LISTEN ADDRESS MZ7500 */
  *g1acr = 0x89; /* SET MZ7500 TO LISTEN */
  *g1acr = 0x0b; /* DEASSERT ATN */

/************ CHANNEL 2 ******************/

  *g2acr = 0x0c;	  
  *g2acr = 0x0a; /* UNTALK MZ7500*/
  *g2dido = 0x3f; /* UNLISTEN PPM */
  *g2dido = 0x44; /* TALK ADDRESS PPM */
  *g2adr = 0x20; /* LISTEN ADDRESS MZ7500 */
  *g2acr = 0x89; /* SET MZ7500 TO LISTEN */
  *g2acr = 0x0b; /* DEASSERT ATN */

}
