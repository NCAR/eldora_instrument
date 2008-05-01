/*

Release HC11 reset* line.
 
*/

#include <vxWorks.h>
#include <stdioLib.h>
#include "ecbMaster.h"

void go11()
     
{
  unsigned char *vmectl;
  
  vmectl = (unsigned char *)(MASTERBASE + MASTERCTL);
  
  *vmectl = 0x1f;  /* Release HC11 from reset, keeping the CLRINTn* signals
		      high and FIFO Clear signals inactive (high) */
}
