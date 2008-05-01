/*    Module:     para_init.c
      Author:     Craig Walther
      Copyright:  National Center For Atmospheric Research
      Date:       July 10, 1990
      Purpose:    This function initializes the parallel input
                  output card.
*/


#define scope extern
#include <HPAI_VAR.H>
#include <HPAI_ADD.H>
#include <HPAI_GBL.H>

void para_init()
   {
   char *port_add;

   /* Clear all the current setup variables */
   curr_pio_d = 0;
   curr_pio_e = 0;

   /* Ports A, B, C & F are input ports (Bits 0,1,2,5 = 0) and
           Ports D, E are output ports (Bits 3,4 =1) */

   port_add = (char *)(BASE_PARA + PIO_CONTRL);
   *port_add = 0x18;

   }