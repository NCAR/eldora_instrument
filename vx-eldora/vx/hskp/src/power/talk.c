/*
 *	$Id$
 *
 *	Module:	talk.c	 
 *	Original Author: Eric Loew
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * 
 * Initial revision
 *
 * 
 *
 * description: The purpose of this module is to configure the MZ7500 as a
 *              "talker" from a "listner" state.
 * 
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <vxWorks.h>
#include <stdioLib.h>
#include <math.h>
#include <string.h>
#include <pwrDef.h>
#include <pwrFunc.h>

void talk()
{
/**************  TURN MZ7500 INTO TALKER *************/
  
  /* TURN BUS AROUND TO TALK */
  /* TAKE CONTROL BY ASSERTING ATN */

/*********** CHANNEL 1 **************/

  *g1acr = 0x0c; /* TAKE CONTROL ASYNCHRONOUSLY */	  
  *g1acr = 0x09; /* UNLISTEN MZ7500*/
  *g1dido = 0x5f; /* UNTALK PPM */
  *g1dido = 0x24; /* LISTEN ADDRESS PPM */
  *g1adr = 0x40; /* TALK ADDRESS MZ7500 */
  *g1acr = 0x8a; /* SET MZ7500 TO TALK */
  *g1acr = 0x0b; /* DEASSERT ATN */

/************ CHANNEL 2 ******************/

  *g2acr = 0x0c; /* TAKE CONTROL ASYNCHRONOUSLY */	  
  *g2acr = 0x09; /* UNLISTEN MZ7500*/
  *g2dido = 0x5f; /* UNTALK PPM */
  *g2dido = 0x24; /* LISTEN ADDRESS PPM */
  *g2adr = 0x40; /* TALK ADDRESS MZ7500 */
  *g2acr = 0x8a; /* SET MZ7500 TO TALK */
  *g2acr = 0x0b; /* DEASSERT ATN */

}
