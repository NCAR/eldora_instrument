/*  
  Write a single byte of unsigned char to requested location, to see if
  more than one byte gets written
       */

#include <vxWorks.h>
#include <stdioLib.h>
#include "ecbMaster.h"

short testalign(loc)
     
     unsigned char *loc;
{
	  *loc = (unsigned char) 0xab;
	  printf("address %8x written with 0xab\n",loc);
}











