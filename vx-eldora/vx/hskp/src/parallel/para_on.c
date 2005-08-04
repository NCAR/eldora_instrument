/*    Module:     para_on.c
      Author:     Craig Walther
      Copyright:  National Center For Atmospheric Research
      Date:       July 10, 1990
      Purpose:    This function sets to one all of the requested bits in the 
                  parallel I/O port that is requested.  Possible ports are:
                  'D'and 'E' each is byte wide.
*/

#define scope extern
#include <HPAI_VAR.H>
#include <HPAI_ADD.H>
#include <HPAI_GBL.H>

void para_on(port,which_one)
char port, which_one;
   {
   char *port_add;
   char value;

   /* parallel I/O card has base address and two byte wide output ports  */
   switch (port)
      {
      case 'd':
      case 'D':
         port_add = BASE_PARA + PIO_OUT_D;
         value = curr_pio_d | which_one;
         *port_add = value;
         curr_pio_d = value;
         break;

      case 'e':
      case 'E':
         port_add = BASE_PARA + PIO_OUT_E;
         value = curr_pio_e | which_one;
         *port_add = value;
         curr_pio_e = value;
         break;

      default :
         return;
      }
   return;
   }