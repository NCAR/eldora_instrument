/*
 *	$Id$
 *
 *	Module:	para_init	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1996/09/03  16:42:04  craig
 * Initial revision
 *
 * Revision 1.5  1996/01/25  22:41:25  craig
 * Added the  piezo screw control code
 *
 * Revision 1.4  1995/09/25  17:45:49  martin
 * updated from Craig's current code
 *
 * Revision 1.2  1995/07/11  21:09:03  craig
 * *** empty log message ***
 *
 * Revision 1.1  1995/06/09  20:45:05  craig
 * Initial revision
 *
 * Revision 1.1  1995/06/09  17:08:39  craig
 * Initial revision
 *
 *
 *
 * description:  This function initializes the parallel input
 *               output card.
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include "tapeDef.h"
#include "paraDef.h"
#include "paraGbl.h"

void para_init()
   {
   char *port_add, value;

   /* Clear all the current setup variables */
   curr_pio_d = 0;
   curr_pio_e = 0;

   /* Ports A, B, C & F are input ports (Bits 0,1,2,5 = 0) and
           Ports D, E are output ports (Bits 3,4 =1) */

   port_add = (char *)(SHORT_BASE + BASE_PARA + PIO_CONTRL);
   *port_add = 0x18;

   para_porta = (unsigned char *)(SHORT_BASE + BASE_PARA + PIO_IN_A);
   para_portb = (unsigned char *)(SHORT_BASE + BASE_PARA + PIO_IN_B);
   para_portc = (unsigned char *)(SHORT_BASE + BASE_PARA + PIO_IN_C);
   para_portf = (unsigned char *)(SHORT_BASE + BASE_PARA + PIO_IN_F);
   para_portd= (char *)(SHORT_BASE + BASE_PARA + PIO_OUT_D);
   para_porte= (char *)(SHORT_BASE + BASE_PARA + PIO_OUT_E);

   value = 0xFF;
   para_off('d',value);
   para_off('e',value);

   para_on('d',SCREW_DIR);
   para_on('d',SCREW_PULSE);

   }
