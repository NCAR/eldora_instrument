/*  Show contents of ECB MASTER Non-volatile RAM in hex dump style. 
    Transfers contents of SRAM to EEPROM before printing. */

#include <vxWorks.h>
#include <stdioLib.h>
#include "ecbMaster.h"

eedump()
     
{
  unsigned char *nvram,*nvxfer,i,j,chartemp[17];
  nvram = (unsigned char *)(MASTERBASE + MASTERNVRAM);
  nvxfer = (unsigned char *)(MASTERBASE + MASTERNVXFR);
  chartemp[17] = '\0';
  
  j = *nvxfer;  /* read nvxfer location to transfer EEPROM to SRAM array */
  printf("Hex dump of EEPROM (32 lines of 16 locations each):\n\n");
  for (j=0;j<20;j++)
    {
      for (i=0;i<16;i++,nvram++)
	{
	  chartemp[i] = *nvram;
	  printf("%2x  ",chartemp[i]);
	  if (chartemp[i]>126 | chartemp[i]<32) chartemp[i] = '.';
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
