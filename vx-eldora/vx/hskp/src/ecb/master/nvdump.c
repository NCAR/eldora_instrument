/*  Show contents of ECB MASTER Non-volatile RAM in hex dump style. 
    SHOWS SRAM, DOES NOT TRANSFER SRAM ARRAY TO EEPROM */

#include <vxWorks.h>
#include <stdioLib.h>
#include "ecbMaster.h"

nvdump()
     
{
  unsigned char *nvram,i,j,chartemp[17];
  nvram = (unsigned char *)(MASTERBASE + MASTERNVRAM);
  chartemp[17] = '\0';
  
  printf("Hex dump of NVRAM (32 lines of 16 locations each):\n\n");
  for (j=0;j<20;j++)
    {
       for (i=0;i<16;i++,nvram++)
	{
	  chartemp[i] = *nvram;
	  printf("%2x  ",chartemp[i]);
	  if (chartemp[i]>127 | chartemp[i] < 32) chartemp[i] = '.';
	}
      printf("%s\n",chartemp);
    }
  printf("Press return.....");
  getchar();
  printf("\r");
  for (j=0;j<12;j++)
    {
      for (i=0;i<16;i++,nvram++)
	{
	  chartemp[i] = *nvram;
	  printf("%2x  ",chartemp[i]);
	}
      printf("%s\n",chartemp);
    }
  printf("\n");
}
