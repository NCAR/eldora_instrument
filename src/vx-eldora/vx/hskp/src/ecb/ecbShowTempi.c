/*
 *	$Id$
 *
 *	Module:		 ecbShowTempi
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1993/09/20  20:25:34  shawn
 * *** empty log message ***
 *
 * Revision 1.1  1992/06/23  19:38:40  shawn
 * Initial revision
 *
 *
 * description: Read ECB "Temperature Status, scaled int" (ecbTempi) structure
 *              in Global memory and print the contents.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
/*	ecbShowTempi()                                                       */
/*		Read ECB "Temperature Status, scaled int" (ecbTempi)         */
/*              structure in Global memory and print the contents.           */
/*      Return values:                                                       */
/*              void function, no value returned.                            */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>

#include "ecbFunc.h"     /* function prototypes for ecb______ */
#include "ecbStat.h"     /* global structures for ecb status */
#include "ecbAdr.h"      /* Slave addresses on ECB bus */

void ecbShowTempi()
{
    printf("ecbShowTempi: values in ecbTempi global status structure:\n");
    printf("ecbShowTempi: ecbadr   ==> 0x%x\n",ecbTempi.ecbadr);
    printf("ecbShowTempi: newdata  ==> %u\n",ecbTempi.newdata);
    printf("ecbShowTempi: onbtemp  ==> %d\n",ecbTempi.onbtemp);
    printf("ecbShowTempi: offbtemp ==> %d\n",ecbTempi.offbtemp);
    printf("ecbShowTempi: the address corresponds to:");
    
    switch(ecbTempi.ecbadr)
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





