/*
 *	$Id$
 *
 *	Module:		 ecbShowBusStat
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description: Read ECB "Slave Status" (ecbSlvStat) structure
 *              in Global memory and print the contents.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
/*	ecbShowBusStat()                                                     */
/*		Read ECB "Slave Status" (ecbSkvStat) structure               */
/*              in Global memory and print the contents.                     */
/*      Return values:                                                       */
/*              void function, no value returned.                            */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>

#include "ecbFunc.h"     /* function prototypes for ecb______ */
#include "ecbStat.h"     /* global structures for ecb status */
#include "ecbAdr.h"      /* Slave addresses on ECB bus */

void ecbShowBusStat()
{
    printf("ecbShowBusStat: values in ecbSlvStat global status structure:\n");
    printf("ecbShowBusStat: ecbadr    ==> 0x%x\n",ecbSlvStat.ecbadr);
    printf("ecbShowBusStat: newdata   ==> %u\n",ecbSlvStat.newdata);
    printf("ecbShowBusStat: syncerrs  ==> %d\n",ecbSlvStat.syncerrs);
    printf("ecbShowBusStat: typeerrs  ==> %d\n",ecbSlvStat.typeerrs);
    printf("ecbShowBusStat: eoserrs   ==> %d\n",ecbSlvStat.eoserrs);
    printf("ecbShowBusStat: feederrs  ==> %d\n",ecbSlvStat.feederrs);
    printf("ecbShowBusStat: cmderrs   ==> %d\n",ecbSlvStat.cmderrs);
    printf("ecbShowBusStat: emptyerrs ==> %d\n",ecbSlvStat.emptyerrs);
    printf("ecbShowBusStat: numberrs  ==> %d\n",ecbSlvStat.numberrs);
    printf("ecbShowBusStat: the address corresponds to:");
    
    switch(ecbSlvStat.ecbadr)
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





