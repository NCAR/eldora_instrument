/*
 *	$Id$
 *
 *	Module:	para_in	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.4  1995/09/25  17:45:47  martin
 * updated from Craig's current code
 *
 * Revision 1.2  1995/07/11  21:09:00  craig
 * *** empty log message ***
 *
 * Revision 1.1  1995/06/09  20:45:03  craig
 * Initial revision
 *
 * Revision 1.1  1995/06/09  17:08:39  craig
 * Initial revision
 *
 *
 *
 * description:   This function reads the parallel input card and returns
 *                the value that is read.
 *
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include "tapeDef.h"
#include "paraDef.h"
#include "paraGbl.h"

unsigned char para_in(port)
char port;
     {
     unsigned char value;

/* parallel input card has base address and four byte wide input ports  */

   switch (port)
      {
      case 'a':
      case 'A':
         value = *para_porta;
         break;

      case 'b':
      case 'B':
         value = *para_portb;
         break;

      case 'c':
      case 'C':
         value = *para_portc;
         break;

      case 'f':
      case 'F':
         value = *para_portf;
         break;

      default :
               value = 0xFF;
      }

   return(value);
   }
