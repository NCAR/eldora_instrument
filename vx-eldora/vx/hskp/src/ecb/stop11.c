/*

Activate HC11 reset* line.
 
*/

#include <vxWorks.h>
#include <stdioLib.h>
#include "ecbMaster.h"

void stop11()
     
{
  unsigned char *vmectl;
  
  vmectl = (unsigned char *)(MASTERBASE + MASTERCTL);
  
  *vmectl = 0x0f;   /* bring HC11 reset* line low, keeping CLRINT*'s
		       inactive (high) */
}
