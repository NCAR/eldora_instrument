/*
 *	$Id$
 *
 *	Module:		 ecbShowLast
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1993/09/20  20:25:29  shawn
 * *** empty log message ***
 *
 * Revision 1.2  1992/06/18  19:59:47  shawn
 * uses proper include files (ecbFunc.h and ecbStat.h)
 *
 * Revision 1.1  1992/06/17  22:53:32  shawn
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
/*	ecbShowLast()                                                        */
/*		Read ECB "Last Command" structure in Global memory and       */
/*              print the contents.                                          */
/*      Return values:                                                       */
/*              void function, no value returned.                            */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>

#include "ecbFunc.h"     /* function prototypes for ecb______ */
#include "ecbStat.h"     /* global structures for ecb status */
#include "ecbAdr.h"      /* Slave addresses on ECB bus */

void ecbShowLast()
{
    printf("ecbShowLast: values in ecbLastCmd global status structure:\n");
    printf("ecbShowLast: ecbadr ==> 0x%x, comID ==> 0x%x\n",ecbLastCmd.ecbadr,ecbLastCmd.comID);
    printf("ecbShowLast: this address corresponds to:");
    
    switch(ecbLastCmd.ecbadr)
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
