/*
 *	$Id$
 *
 *	Module:	Return_to_Local.c	 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Initial revision
 *
 *
 * description: The purpose of this module is to return the Power 
 *              Meter to Local Control.
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope
#include "vxWorks.h"
#include "stdioLib.h"
#include "hskpAll.h"


/******************** MAIN PROGRAM ****************************/

void Return_to_Local(int channel)
{
  int timeout;
  unsigned char status,status1;

  if(channel == 1)
    {
/* Return to Local Control */

      *g1acr = 0x0c; /* TAKE CONTROL ASYNCHRONOUSLY */
      *g1adr = 0x40;  /* SEND TALK ADDRESS OF MZ7500 */
      *g1dido = 0x3f; /* UNLISTEN PPM */
      timeout = 0;
      while((status = *g1ism0 & 0x10) != 0x10 && timeout < 30)
	{
	  timeout++;
	  taskDelay(1);
	}
      *g1dido = 0x24; /* SEND PPM LISTEN ADDRESS */
      timeout = 0;
      while(((status = *g1ism0 & 0x10) != 0x10) && timeout < 30)
	{
	  timeout++;
	  taskDelay(1);
	}
      *g1acr = 0x10; /* CLEAR REN */
#ifdef TEST
      *g1dido = 0x04;/* CLEAR DEVICES ON BUS */ 
      while((status = *g1ism0 & 0x10) != 0x10)
	{
	  taskDelay(1);
	}
#endif
      *g1dido = 0x1;  /* GTL */ 

    }
  if(channel == 2)
    {
/* Return to Local Control */

      *g2acr = 0x0c; /* TAKE CONTROL ASYNCHRONOUSLY */
      *g2adr = 0x40;  /* SEND TALK ADDRESS OF MZ7500 */
      *g2dido = 0x3f; /* UNLISTEN PPM */
      timeout = 0;
      while(((status = *g2ism0 & 0x10) != 0x10) && timeout < 30)
	{
	  timeout++;
	  taskDelay(1);
	}
      *g2dido = 0x24; /* SEND PPM LISTEN ADDRESS */ 
      timeout = 0;
      while(((status = *g2ism0 & 0x10) != 0x10) && timeout < 30)
	{
	  timeout++;
	  taskDelay(1);
	}
      *g2acr = 0x10; /* CLEAR REN */
#ifdef TEST
      *g2dido = 0x04; /* CLEAR DEVICES ON BUS */ 
      status = *g2ism0;
      while((status = *g2ism0 & 0x10) != 0x10)
	{
	  taskDelay(1);
	}
#endif
      *g2dido = 0x1;  /* GTL */ 
    }
}







