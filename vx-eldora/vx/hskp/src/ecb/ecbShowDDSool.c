/*
 *	$Id$
 *
 *	Module:		 ecbShowDDSool
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/06/23  19:38:40  shawn
 * Initial revision
 *
 *
 * description: Read ECB "DDS out-of-lock Status" (ecbOol) structure
 *              in Global memory and print the contents.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
/*	ecbShowDDSool()                                                      */
/*		Read ECB "DDS out-of-lock Status" (ecbOol)                   */
/*              structure in Global memory and print the contents.           */
/*      Return values:                                                       */
/*              void function, no value returned.                            */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>

#include "ecbFunc.h"     /* function prototypes for ecb______ */
#include "ecbStat.h"     /* global structures for ecb status */
#include "ecbAdr.h"      /* Slave addresses on ECB bus */

void ecbShowDDSool()
{
    printf("ecbShowDDSool: values in ecbOol global status structure:\n");
    printf("ecbShowDDSool: ecbadr   ==> 0x%x\n",ecbOol.ecbadr);
    printf("ecbShowDDSool: newdata  ==> %u\n",ecbOol.newdata);
    printf("ecbShowDDSool: oolbits  ==> 0x%x\n",ecbOol.oolbits);
    printf("ecbShowDDSool: the address corresponds to:");
    
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
	  printf("ecbShowDDSool: WARNING!  THIS IS NOT THE ADDRESS OF A RCVR/XCTR.\n");
	  break;
	case ECBIFAFT:
 	  printf(" AFT IF PROCESSOR SLAVE.\n");
	  printf("ecbShowDDSool: WARNING!  THIS IS NOT THE ADDRESS OF A RCVR/XCTR.\n");
	  break;
	case ECBATTEN:
 	  printf(" ATTENUATOR CHASSIS SLAVE.\n");
	  printf("ecbShowDDSool: WARNING!  THIS IS NOT THE ADDRESS OF A RCVR/XCTR.\n");
	  break;
	default:
	  printf(" NO KNOWN ADDRESS!!!!\n");
	  break;
      }
}





