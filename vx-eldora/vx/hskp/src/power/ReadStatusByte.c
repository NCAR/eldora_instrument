/*
 *	$Id$
 *
 *	Module:	ReadStatusByte.c	 
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
 * description: The purpose of this module is to read and return Power 
 *              Meter Status after an SRQ is detected.
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope
#include "vxWorks.h"
#include "stdioLib.h"
#include "hskpAll.h"


/******************** MAIN PROGRAM ****************************/

unsigned char ReadStatusByte(int channel)
{
  int timeout;
  unsigned char status,status1;

  if(channel == 1)
    {
  /* Serial Poll for status */
	
      *g1acr = 0x0c;  /* ASSERT ATN */      
      *g1acr = 0x0a;  /* UNTALK MZ7500*/
      *g1dido = 0x3f; /* UNLISTEN PPM */
      timeout = 0;
      while(*g1ism0 != 0x10 && timeout < 30)
	{
	  timeout++; 
	  taskDelay(1);
	}
  /*  printf("Timeout %d\n",timeout); */
      *g1adr = 0x20; /* LISTEN ADDRESS MZ7500 */
      *g1dido = 0x18;
      timeout = 0;
      while(*g1ism0 != 0x10 && timeout < 30)
	{
	  timeout++; 
	  taskDelay(1);
	}
  /*  printf("Timeout %d\n",timeout); */
      timeout = 0;
      *g1dido = 0x44; /* TALK ADDRESS PPM */
      while(*g1ism0 != 0x10 && timeout < 30)
	{
	  timeout++; 
	  taskDelay(1);
	}
  /*  printf("Timeout %d\n",timeout); */
      *g1acr = 0x89; /* SET MZ7500 TO LISTEN */
      *g1acr = 0x0b; /* DEASSERT ATN */
      timeout = 0;
      status = *g1ism0;
      while((status & 0x20 != 0x20) && timeout < 30)
	{
	  timeout++;
	  taskDelay(1);
	  status = *g1ism0;
	}
  /*  printf("Timeout %d\n",timeout); */
      status = *g1dido;     
  /*  printf("Status = %2X\n",status); */
      *g1acr = 0x0c;        /* ASSERT ATN */
      *g1dido = 0x19;
      timeout = 0;
      while((*g1ism0 & 0x10 != 0x10) && timeout < 30)
	{
	  timeout++; 
	  taskDelay(1);
	}
  /*  printf("Timeout %d\n",timeout); */
      *g1acr = 0x0b; /* DEASSERT ATN */
      talk(1);
    }
  if(channel == 2)
    {
  /* Serial Poll for status */
	
      *g2acr = 0x0c;  /* ASSERT ATN */      
      *g2acr = 0x0a;  /* UNTALK MZ7500*/
      *g2dido = 0x3f; /* UNLISTEN PPM */
      timeout = 0;
      while(*g2ism0 != 0x10 && timeout < 30)
	{
	  timeout++; 
	  taskDelay(1);
	}
  /*  printf("Timeout %d\n",timeout); */
      *g2adr = 0x20; /* LISTEN ADDRESS MZ7500 */
      *g2dido = 0x18;
      timeout = 0;
      while(*g2ism0 != 0x10 && timeout < 30)
	{
	  timeout++; 
	  taskDelay(1);
	}
  /*  printf("Timeout %d\n",timeout); */
      timeout = 0;
      *g2dido = 0x44; /* TALK ADDRESS PPM */
      while(*g2ism0 != 0x10 && timeout < 30)
	{
	  timeout++; 
	  taskDelay(1);
	}
  /*  printf("Timeout %d\n",timeout); */
      *g2acr = 0x89; /* SET MZ7500 TO LISTEN */
      *g2acr = 0x0b; /* DEASSERT ATN */
      timeout = 0;
      status = *g2ism0;
      while((status & 0x20 != 0x20) && timeout < 30)
	{
	  timeout++;
	  taskDelay(1);
	  status = *g2ism0;
	}
  /*  printf("Timeout %d\n",timeout); */
      status = *g2dido;     
  /*  printf("Status = %2X\n",status); */
      *g2acr = 0x0c;        /* ASSERT ATN */
      *g2dido = 0x19;
      timeout = 0;
      while((*g2ism0 & 0x10 != 0x10) && timeout < 30)
	{
	  timeout++; 
	  taskDelay(1);
	}
  /*  printf("Timeout %d\n",timeout); */
      *g2acr = 0x0b; /* DEASSERT ATN */
      talk(2);
    }
      return(status);
}







