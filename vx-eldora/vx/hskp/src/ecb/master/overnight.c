/*  Write string to ELDORA UTILITY CARD Non-volatile RAM and
    transfer to EEPROM.  */

#include <vxWorks.h>
#include <stdioLib.h>
#include "ecbMaster.h"

overnight()
     
{
  unsigned char *nvram,*nvxfer,i;
  nvram  = (unsigned char *)(MASTERBASE + MASTERNVRAM);
  nvxfer = (unsigned char *)(MASTERBASE + MASTERNVXFR);
  
  printf("overnight():");
  printf("The following 64-character string will be repeated 8 times\n");
  printf("    in the UTILITY CARD's SRAM array, then it will be\n");
  printf("    transferred into EEPROM.\n");
  printf("The program showee() will show the contents of eeprom\n\n");
  printf("String:\n");
  printf("   \"This string was stored in EEPROM as an overnight integrity test.\"\n\n");

  printf("Copying data to SRAM array...");
  for (i=0; i<8; i++, nvram+=64)
      strcpy(nvram,"This string was stored in EEPROM as an overnight integrity test.");

  printf("\nTransferring data to EEPROM...");
  *nvxfer = 0x00;

  printf("\n...done.\n\n");
}
