/*
 *	$Id$
 *
 *	Module:		 ecbGetTsami.c
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description: Write "Send Temperature Sample, scaled int" command
 *              to ECB MASTER IN FIFO
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
/*	ecbGetTsami()                                                        */
/*		Write "Send Temperature Sample, scaled int" command to ECB   */
/*              MASTER IN FIFO                                               */
/*      Return values:                                                       */
/*              0 - command placed into IN FIFO and "command-not-pending"    */
/*                  semaphore taken.                                         */
/*              1 - previous command still pending; no action taken.         */
/*              2 - "command-not-pending" semaphore has been given, but IN   */
/*                  FIFO is not empty;  no action taken, semaphore re-given. */
/*              3 - "command-not-pending" semaphore has been given, but OUT  */
/*                  FIFO is not empty;  no action taken, semaphore re-given. */
/*              4 - error when re-Give-ing ecb_cmd_not_pending semaphore,    */
/*                  things are probably pretty messed up.                    */
/*              5 - ecbadr passed is not valid; no action taken.             */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>
#include "semLib.h"

#include "ecbFunc.h"     /* function prototypes for ecb______ */
#include "ecbMaster.h"   /* general #defines for ecb master offsets */
#include "ecbSem.h"      /* semaphore definitions for ecb master */
#include "ecbAdr.h"      /* Slave addresses on ECB bus */

unsigned char ecbGetTsami(unsigned char ecbadr)
{
    unsigned char *vmestat,*infifo,*outfifo;
    unsigned char readback=0,chksumback=0;

    /* Check for pending Command */
    if (semTake(ecb_cmd_not_pending,1) == ERROR)
      {
	  printf("ecbGetTsami: command still pending...");
	  printf("Returning without issuing command.\n");
	  return(1);
      }

    /* Check for valid ecbadr */
    if (ecbadr!=ECBIFFOR && ecbadr!=ECBIFAFT && ecbadr!=ECBRFFOR && ecbadr!=ECBRFAFT && ecbadr!=ECBATTEN)
      {
	  printf("ecbGetTsami: ecbadr passed [%d] is not the ",ecbadr);
	  printf("ECB bus address\n");
	  printf("ecbGetTsami: for any known slave...");
	  printf("Returning without issuing command.\n");
	  printf("ecbGetTsami: Re-Giving ecb_cmd_not_pending Semaphore.\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbGetTsami: semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbGetTsami: indicating an invalid semaphore ID.\n");
		printf("ecbGetTsami: Returning without Give-ing semaphore.\n");
		return(4);
	    }
	  return(5);
      }

    vmestat = (unsigned char *)(MASTERBASE + MASTERSTAT);
    infifo  = (unsigned char *)(MASTERBASE + MASTERIN);
    outfifo  = (unsigned char *)(MASTERBASE + MASTEROUT);

    if ( (*vmestat & 0x01) )
      {
	  printf("ecbGetTsami: command not pending, but IN FIFO is ");
	  printf("not empty.\n");
	  printf("ecbGetTsami: returning without issuing command, ");
	  printf("re-Giving\n");
	  printf("ecbGetTsami: ecb_cmd_not_pending semaphore...\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbGetTsami: semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbGetTsami: indicating an invalid semaphore ID.\n");
		printf("ecbGetTsami: Returning without Give-ing semaphore.\n");
		return(4);
	    }
	  return(2);
      }

    if ( (*vmestat & 0x04) )
      {
	  printf("ecbGetTsami: command not pending, but OUTFIFO ");
	  printf("is not empty.\n");
	  printf("ecbGetTsami: returning without issuing command, ");
	  printf("re-Giving\n");
	  printf("ecbGetTsami: ecb_cmd_not_pending semaphore...\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbGetTsami: semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbGetTsami: indicating an invalid semaphore ID.\n");
		printf("ecbGetTsami: Returning without Give-ing semaphore.\n");
		return(4);
	    }
	  return(3);
      }

    *infifo = ecbadr;  /* write ecb address into master's IN FIFO */
    *infifo = (unsigned char) 1;  /* write numbytes into IN FIFO */
    *infifo = 0x01;  /* write command ID Code into IN FIFO */
    *infifo = (unsigned char) ECBEOS; /* write end-of-sequence into IN FIFO */

    return(0);
}
