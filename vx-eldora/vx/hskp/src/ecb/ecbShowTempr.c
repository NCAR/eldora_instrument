/*
 *	$Id$
 *
 *	Module:		 ecbShowTempr
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1993/09/20  20:25:36  shawn
 * *** empty log message ***
 *
 * Revision 1.1  1992/06/23  19:39:12  shawn
 * Initial revision
 *
 *
 * description: Read ECB "Temperature Status, raw counts" (ecbTempr) structure
 *              in Global memory and print the contents.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
/*	ecbShowTempr()                                                       */
/*		Read ECB "Temperature Status, raw counts" (ecbTempr)         */
/*              structure in Global memory and print the contents.           */
/*      Return values:                                                       */
/*              void function, no value returned.                            */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>

#include "ecbFunc.h"     /* function prototypes for ecb______ */
#include "ecbStat.h"     /* global structures for ecb status */
#include "ecbAdr.h"      /* Slave addresses on ECB bus */

void ecbShowTempr()
{
    printf("ecbShowTempr: values in ecbTempr global status structure:\n");
    printf("ecbShowTempr: ecbadr   ==> 0x%x\n",ecbTempr.ecbadr);
    printf("ecbShowTempr: newdata  ==> %u\n",ecbTempr.newdata);
    printf("ecbShowTempr: onbtemp  ==> %u\n",ecbTempr.onbtemp);
    printf("ecbShowTempr: offbtemp ==> %u\n",ecbTempr.offbtemp);
    printf("ecbShowTempr: the address corresponds to:");
    
    switch(ecbTempr.ecbadr)
      {
	case ECBRFFOR:
 	  printf(" FORWARD RCVR/XCTR SLAVE.\n");
	  break;
	case ECBRFAFT:
 	  printf(" AFT RCVR/XCTR SLAVE.\n");
	  break;
	case ECBIFFOR:
 	  printf(" FORWARD IF PROCESSOR SLAVE.\n");
	  break;
	case ECBIFAFT:
 	  printf(" AFT IF PROCESSOR SLAVE.\n");
	  break;
	case ECBATTEN:
 	  printf(" ATTENUATOR CHASSIS SLAVE.\n");
	  break;
	default:
	  printf(" NO KNOWN ADDRESS!!!!\n");
	  break;
      }
}
