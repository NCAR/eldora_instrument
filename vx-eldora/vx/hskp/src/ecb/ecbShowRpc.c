/*
 *	$Id$
 *
 *	Module:		 ecbShowRpc
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/24  22:31:43  shawn
 * Initial revision
 *
 * description: Read ECB "Slave Status" (ecbSlvStat) structure
 *              in Global memory and print the contents.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
/*	ecbShowRpc()                                                         */
/*		Read ECB parts of the rpc status structure "currStatus"      */
/*              in Global memory and print the contents.                     */
/*      Return values:                                                       */
/*              void function, no value returned.                            */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>

#include "ecbFunc.h"     /* function prototypes for ecb______ */
#define OK_RPC           /* makes definitions active in HskpStatus.h */
#include "HskpStatus.h"  /* global housekeeping status... rpc to cntl proc */

void ecbShowRpc()
{
    printf("ecbShowRpc: values in currStatus global status structure:\n");
    printf("ecbShowRpc: ecbgen    ==> 0x%2x\n",currStatus->ecbgen);
    printf("ecbShowRpc: slvdead   ==> 0x%2x\n",currStatus->slvdead);
    printf("ecbShowRpc: onBoverT  ==> 0x%2x\n",currStatus->onBoverT);
    printf("ecbShowRpc: offBoverT ==> 0x%2x\n",currStatus->offBoverT);
    printf("ecbShowRpc: forDDSool ==> 0x%2x\n",currStatus->forDDSool);
    printf("ecbShowRpc: aftDDSool ==> 0x%2x\n\n",currStatus->aftDDSool);
}
