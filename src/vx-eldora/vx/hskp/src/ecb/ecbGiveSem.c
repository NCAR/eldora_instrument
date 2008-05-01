/*
 *	$Id$
 *
 *	Module:		 ecbClrSem
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1993/09/20  20:20:59  shawn
 * *** empty log message ***
 *
 * Revision 1.1  1992/06/23  19:46:26  shawn
 * Initial revision
 *
 *
 *
 * description: Write "set IF processof filter" command to ECB MASTER IN FIFO
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
/*	ecbGiveSem()                                                         */
/*		"Give" the ecb_cmd_not_pending semaphore.                    */
/*      Return values:                                                       */
/*              none, void fuction (it's a tool for debug)                   */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>
#include "semLib.h"

#include "ecbFunc.h"     /* function prototypes for ecb______ */
#include "ecbSem.h"      /* semaphore definitions for ecb master */

void ecbGiveSem()
{
    /* Check for pending Command */
    if (semTake(ecb_cmd_not_pending,1) == ERROR)
      {
	  printf("ecbGiveSem: note: ecb_cmd_not_pending not Given...\n");
	  printf("ecbGiveSem: preparing to Give it...\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbGiveSem: semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbGiveSem: indicating an invalid semaphore ID.\n");
		printf("ecbGiveSem: Returning without Give-ing semaphore.\n");
	    } /* end if (semGive... */
      } /* end if (SemTake... */
    else
      {
	  printf("ecbGiveSem: note: ecb_cmd_not_pending already Given!\n");
	  printf("ecbGiveSem: Preparing to re-Give it...\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbGiveSem: semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbGiveSem: indicating an invalid semaphore ID.\n");
		printf("ecbGiveSem: Returning without Give-ing semaphore.\n");
	    } /* end if (semGive... */
      } /* end else */
}
