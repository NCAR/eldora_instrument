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
 * Revision 1.2  1997/11/12  19:49:55  eric
 * Added code to handle channels independently.
 *
 * Revision 1.1  1997/08/25  17:58:29  eric
 * Initial revision
 *
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
#include "taskLib.h"

void talk(int channel)
{
  int timeout;
/**************  TURN MZ7500 INTO TALKER *************/
  
  /* TURN BUS AROUND TO TALK */
  /* TAKE CONTROL BY ASSERTING ATN */

/*********** CHANNEL 1 **************/

if(channel == 1)
  {
    *g1acr = 0x0c; /* TAKE CONTROL ASYNCHRONOUSLY */	  
    *g1dido = 0x5f; /* UNTALK PPM */
    timeout = 0;
    while(*g1ism0 != 0x10 && timeout < 30)
      {
	timeout++;
	taskDelay(1);
      }
    *g1acr = 0x09; /* UNLISTEN MZ7500*/
    *g1dido = 0x24; /* LISTEN ADDRESS PPM */
    timeout = 0;
    while(*g1ism0 != 0x10 && timeout < 30)
      {
	timeout++;
	taskDelay(1);
      }
    *g1adr = 0x40; /* TALK ADDRESS MZ7500 */
    *g1acr = 0x8a; /* SET MZ7500 TO TALK */
    *g1acr = 0x0b; /* DEASSERT ATN */
  }

/************ CHANNEL 2 ******************/

if(channel == 2)
  {
    *g2acr = 0x0c; /* TAKE CONTROL ASYNCHRONOUSLY */	  
    *g2dido = 0x5f; /* UNTALK PPM */
    timeout = 0;
    while(*g2ism0 != 0x10 && timeout < 30)
      {
	timeout++;
	taskDelay(1);
      }
    *g2acr = 0x09; /* UNLISTEN MZ7500*/
    *g2dido = 0x24; /* LISTEN ADDRESS PPM */
    timeout = 0;
    while(*g2ism0 != 0x10 && timeout < 30)
      {
	timeout++;
	taskDelay(1);
      }
    *g2adr = 0x40; /* TALK ADDRESS MZ7500 */
    *g2acr = 0x8a; /* SET MZ7500 TO TALK */
    *g2acr = 0x0b; /* DEASSERT ATN */
  }
}


