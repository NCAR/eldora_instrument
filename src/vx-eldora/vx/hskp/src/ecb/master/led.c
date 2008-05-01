/*
 
Play around with the ECB MASTER LED.
 
*/

#include <vxWorks.h>
#include <stdioLib.h>
#include "ecbMaster.h"

led(timval)
     
     short timval;
     
{
  unsigned char *vmectl;
  long icount,jcount;
  
  vmectl = (unsigned char *)(MASTERBASE + 0xF200);
  
  while(1)
    {
      for (icount = 100; icount<20*timval; icount = icount+timval)
	{
	  for (jcount = 0; jcount<icount; jcount++);
	  *vmectl = 0x20;
	  for (jcount = 0; jcount<icount; jcount++);
	  *vmectl = 0x00;
	}
      for (; icount>100; icount = icount-timval)
	{
	  for (jcount = 0; jcount<icount; jcount++);
	  *vmectl = 0x20;
	  for (jcount = 0; jcount<icount; jcount++);
	  *vmectl = 0x00;
	}
    }
}
