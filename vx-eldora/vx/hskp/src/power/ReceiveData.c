/*
 *	$Id$
 *
 *	Module:	ReceiveData.c	 
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
 *              Meter Data  after an SRQ is detected.
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope
#include "vxWorks.h"
#include "stdioLib.h"
#include "hskpAll.h"


/******************** MAIN PROGRAM ****************************/

unsigned char * ReceiveData(int channel, int bytecount)
{
  int timeout,i;
  unsigned char status,status1;
  static unsigned char data[15];

  if(channel == 1)
    {
      for(i=0; i<15; i++)  /* Null fill array */
	{
	  data[i] = 0;
	}

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
      for(i=0;i<bytecount;i++)
	{
      /*      status = *g1ism0; */
	  timeout = 0;
	  while(((status = *g1ism0 & 0x20) != 0x20) && timeout < 30)
	    {
	      timeout++;
	      taskDelay(1);
	    }
      /*     taskDelay(5); */
	  data[i] = *g1dido;
	}     
  /*  printf("%s \n",data); */
      *g1acr = 0x0c;        /* ASSERT ATN */
      *g1acr = 0x0b; /* DEASSERT ATN */
    }

  if(channel == 2)
    {
      for(i=0; i<15; i++)  /* Null fill array */
	{
	  data[i] = 0;
	}

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
      for(i=0;i<bytecount;i++)
	{
      /*      status = *g2ism0; */
	  timeout = 0;
	  while(((status = *g2ism0 & 0x20) != 0x20) && timeout < 30)
	    {
	      timeout++;
	      taskDelay(1);
	    }
      /*     taskDelay(5); */
	  data[i] = *g2dido;
	}     
  /*  printf("%s \n",data); */
      *g2acr = 0x0c;        /* ASSERT ATN */
      *g2acr = 0x0b; /* DEASSERT ATN */
    }

  return(data);
}










