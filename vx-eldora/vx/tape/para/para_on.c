/*
 *	$Id$
 *
 *	Module:	para_on	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.4  1995/09/25  17:45:53  martin
 * updated from Craig's current code
 *
 * Revision 1.2  1995/07/11  21:09:06  craig
 * *** empty log message ***
 *
 * Revision 1.1  1995/06/09  20:45:07  craig
 * Initial revision
 *
 * Revision 1.1  1995/06/09  17:08:39  craig
 * Initial revision
 *
 *
 *
 * description:  This function sets to one all of the requested bits in the 
 *                 parallel I/O port that is requested.  Possible ports are:
 *                'D' and 'E' each is byte wide.
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include "tapeDef.h"
#include "paraDef.h"
#include "paraGbl.h"


void para_on(port,which_one)
char port, which_one;
   {
   char value;


   /* parallel I/O card has base address and two byte wide output ports  */
   switch (port)
      {
      case 'd':
      case 'D':
         value = curr_pio_d | which_one;
         *para_portd = value;
         curr_pio_d = value;
         break;

      case 'e':
      case 'E':
         value = curr_pio_e | which_one;
         *para_porte = value;
         curr_pio_e = value;
         break;

      default :
         return;
      }
   return;
   }
