/*
 *	$Id$
 *
 *	Module:	init_gpib.c	 
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
 * description: The purpose of this module is to initialize gpib channels
 *              1 and 2 on the MZ7500 board for talk mode for sending  
 *              command strings to the power meters.
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <vxWorks.h>
#include <stdioLib.h>
#include <math.h>
#include <string.h>
#include <pwrDef.h>
#include <pwrFunc.h>

void init_gpib()
{
/**************** GPIB CHANNEL 1 ****************/

  *g1acr = 0x10; /* CLEAR REN */
  *g1dido = 0x04;/* CLEAR DEVICES ON BUS */ 
  *g1acr = 0x80; /* SET SOFTWARE RESET (swrst)*/
  *g1acr = 0x00; /* CLEAR SOFTWARE RESET */
  *g1acr = 0x8f; /* SEND INTERFACE CLEAR (sic)*/
  taskDelay(1);  /* SET DELAY */
  *g1acr = 0x0f; /* CLEAR sic */
  *g1acr = 0x93;
  *g1ism0 =0x00; /* Clear Int Mask 0 */
  *g1ism1 =0x00; /* Clear Int Mask 1 */
  *g1acr = 0x90; /* SEND REMOTE ENABLE */
  *g1acr = 0x0c; /* TAKE CONTROL ASYNCHRONOUSLY */
                 /* TO MAKE MZ7500 CONTROLER */
  *g1dido = 0x24; /* SEND PPM LISTEN ADDRESS */
  *g1adr = 0x40;  /* SEND TALK ADDRESS OF MZ7500 */
  *g1acr = 0x8a;  /* SET MZ7500 AS TALKER */
  *g1acr = 0x0b;  /* GO TO STANBY TO DEASSERT ATN */

/************** GPIB CHANNEL 2 ******************/

  *g2acr = 0x10; /* CLEAR REN */
  *g2dido = 0x04;/* CLEAR DEVICES ON BUS */ 
  *g2acr = 0x80; /* SET SOFTWARE RESET (swrst)*/
  *g2acr = 0x00; /* CLEAR SOFTWARE RESET */
  *g2acr = 0x8f; /* SEND INTERFACE CLEAR (sic)*/
  taskDelay(1); /* SET DELAY */
  *g2acr = 0x0f; /* CLEAR sic */
  *g2acr = 0x93;
  *g2ism0 =0x00; /* Clear Int Mask 0 */
  *g2ism1 =0x00; /* Clear Int Mask 1 */
  *g2acr = 0x90;  /* SEND REMOTE ENABLE */
  *g2acr = 0x0c;  /* TAKE CONTROL ASYNCHRONOUSLY */
                  /* TO MAKE MZ7500 CONTROLER */
  *g2dido = 0x24; /* SEND PPM LISTEN ADDRESS */
  *g2adr = 0x40;  /* SEND TALK ADDRESS OF MZ7500 */
  *g2acr = 0x8a;  /* SET MZ7500 AS TALKER */
  *g2acr = 0x0b;  /* GO TO STANBY TO DEASSERT ATN */

}
