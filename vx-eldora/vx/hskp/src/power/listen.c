/*
 *	$Id$
 *
 *	Module:	listen.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1999/07/13  16:49:51  thor
 * *** empty log message ***
 *
 * Revision 1.2  1997/11/12 19:44:43  eric
 * modified to support Graph Mode for xmit pulse.
 *
 * Revision 1.1  1993/09/20  17:36:00  reif
 * Initial revision
 *
 * Revision 1.1  1992/09/01  16:48:23  craig
 * Initial revision
 *
 *
 * description: The purpose of this module is to turn MZ7500 into listener
 *              and Power meters into talkers
 * 
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <vxWorks.h>
#include <stdioLib.h>
#include <math.h>
#include <string.h>
#include <pwrDef.h>
#include <pwrFunc.h>

void listener(int channel)
{
  int timeout;
/**************  TURN MZ7500 INTO LISTENER *************/
  
  /* TURN BUS AROUND TO LISTEN */
  /* TAKE CONTROL BY ASSERTING ATN */


if(channel == 1)
  {
/*********** CHANNEL 1 **************/

    *g1acr = 0x0c;	  
    *g1acr = 0x0a; /* UNTALK MZ7500*/
    *g1dido = 0x3f; /* UNLISTEN PPM */
    timeout = 0;
    while(*g1ism0 != 0x10 && timeout < 30)
      {
	timeout++;
	taskDelay(1);
      }
    *g1dido = 0x44; /* TALK ADDRESS PPM */
    timeout = 0;
    while(*g1ism0 != 0x10 && timeout < 30)
      {
	timeout++;
	taskDelay(1);
      }
    *g1adr = 0x20; /* LISTEN ADDRESS MZ7500 */
    *g1acr = 0x89; /* SET MZ7500 TO LISTEN */
    *g1acr = 0x0b; /* DEASSERT ATN */
  }

if(channel == 2)
  {
/************ CHANNEL 2 ******************/

    *g2acr = 0x0c;	  
    *g2acr = 0x0a; /* UNTALK MZ7500*/
    *g2dido = 0x3f; /* UNLISTEN PPM */
    timeout = 0;
    while(*g2ism0 != 0x10 && timeout < 30)
      {
	timeout++;
	taskDelay(1);
      }
    *g2dido = 0x44; /* TALK ADDRESS PPM */
    timeout = 0;
    while(*g2ism0 != 0x10 && timeout < 30)
      {
	timeout++;
	taskDelay(1);
      }
    *g2adr = 0x20; /* LISTEN ADDRESS MZ7500 */
    *g2acr = 0x89; /* SET MZ7500 TO LISTEN */
    *g2acr = 0x0b; /* DEASSERT ATN */
  }
}
