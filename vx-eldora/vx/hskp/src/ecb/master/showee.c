/*  Show contents of ECB MASTER Non-volatile RAM.  */

#include <vxWorks.h>
#include <stdioLib.h>
#include "ecbMaster.h"

showee()
     
{
  unsigned char *nvram,*nvxfer,i,j;
  nvram = (unsigned char *)(MASTERBASE + MASTERNVRAM);
  nvxfer = (unsigned char *)(MASTERBASE + MASTERNVXFR);
  
  j = *nvxfer;  /* read nvxfer location to transfer EEPROM to SRAM array */
  printf("CONTENTS of EEPROM (8 lines of 64 characters each):\n\n");
  for (j=0;j<8;j++)
    {
      for (i=0;i<64;i++,nvram++)
        printf("%c",*nvram);
      printf("\n");
    }
  printf("\n\n");
}
