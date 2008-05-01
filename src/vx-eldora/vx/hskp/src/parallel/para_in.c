/*    Module:     para_in.c
      Author:     Craig Walther
      Copyright:  National Center For Atmospheric Research
      Date:       July 10, 1990
      Purpose:    This function reads the parallel input card and returns
                  the value that is read.
*/


#define scope extern
#include <HPAI_VAR.H>
#include <HPAI_ADD.H>
#include <HPAI_GBL.H>

unsigned char para_in(port)
char port;
     {
     unsigned char *port_add;
     unsigned char value;

/* parallel input card has base address and four byte wide input ports  */

   switch (port)
      {
      case 'a':
      case 'A':
         port_add = (unsigned char *)(BASE_PARA + PIO_IN_A);
         value = *port_add;
         break;

      case 'b':
      case 'B':
         port_add = (unsigned char *)(BASE_PARA + PIO_IN_B);
         value = *port_add;
         break;

      case 'c':
      case 'C':
         port_add = (unsigned char *)(BASE_PARA + PIO_IN_C);
         value = *port_add;
         break;

      case 'f':
      case 'F':
         port_add = (unsigned char *)(BASE_PARA + PIO_IN_F);
         value = *port_add;
         break;

      default :
               value = 0xFF;
      }

   return(value);
   }