/*
 *	$Id$
 *
 *	Module:		 ecbSetDivN
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/06/23  19:44:45  shawn
 * Initial revision
 *
 * description: Write "Set Trigger Divide-by-N value" command into ECB 
 *              MASTER IN FIFO
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
/*	ecbSetDivN()                                                         */
/*		Write "Set Trigger Divide-by-N Value" command to ECB         */
/*              MASTER IN FIFO                                               */
/*              Note:  xmittp = 1 means xmit power meter trigger.            */
/*                     xmittp = 0 means testpulse power meter trigger.       */
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
/*              5 - ecbadr passed is not for the ATTENUATOR chassis; no      */
/*                  action taken.                                            */
/*              6 - xmittp passed is not 1 or 0; no action taken.            */
/*              7 - N out of range; no action taken.                         */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>
#include "semLib.h"

#include "ecbFunc.h"     /* function prototypes for ecb______ */
#include "ecbMaster.h"   /* general #defines for ecb master offsets */
#include "ecbSem.h"      /* semaphore definitions for ecb master */
#include "ecbAdr.h"      /* Slave addresses on ECB bus */
#include "ecbErrBound.h" /* various #defines for error bounds */

unsigned char ecbSetDivN(unsigned char ecbadr,unsigned char xmittp,unsigned char N)
{
    unsigned char *vmestat,*infifo,*outfifo;

    /* Check for pending command */
    if (semTake(ecb_cmd_not_pending,1) == ERROR)
      {
	  printf("ecbSetDivN: command still pending...");
	  printf("Returning without issuing command.\n");
	  return(1);
      }
    
    /* Check for valid ecbadr */
    if (ecbadr!=ECBATTEN)
      {
	  printf("ecbSetDivN: ecbadr passed [%d] is not the ",ecbadr);
	  printf("ECB bus address\n");
	  printf("ecbSetDivN: for the ATTENUATOR chassis...");
	  printf("Returning without issuing command.\n");
	  printf("ecbSetDivN: Re-Giving ecb_cmd_not_pending Semaphore.\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetDivN: semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetDivN: indicating an invalid semaphore ID.\n");
		printf("ecbSetDivN: Returning without Give-ing semaphore.\n");
		return(4);
	    }
	  return(5);
      }
    
    /* Check for valid xmittp */
    if (xmittp != 0 && xmittp != 1)
      {
	  printf("ecbSetDivN: xmittp passed [%d] is not ",xmittp);
	  printf("a valid value (0 or 1).\n");
	  printf("ecbSetDivN: Returning without issuing command.\n");
	  printf("ecbSetDivN: Re-Giving ecb_cmd_not_pending Semaphore.\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetDivN: semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetDivN: indicating an invalid semaphore ID.\n");
		printf("ecbSetDivN: Returning without Give-ing semaphore.\n");
		return(4);
	    }
	  return(6);
      }

    /* Check for valid N */
    if ((N < ECBATLOWN) || (N > ECBATHIGHN))
      {
	  printf("ecbSetDivN: N value passed [%u] is out ",N);
	  printf("of range (2..255).\n");
	  printf("ecbSetDivN: Returning without issuing command.\n");
	  printf("ecbSetDivN: Re-Giving ecb_cmd_not_pending Semaphore.\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetDivN: semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetDivN: indicating an invalid semaphore ID.\n");
		printf("ecbSetDivN: Returning without Give-ing semaphore.\n");
		return(4);
	    }
	  return(7);
      }

    vmestat = (unsigned char *)(MASTERBASE + MASTERSTAT);
    infifo  = (unsigned char *)(MASTERBASE + MASTERIN);
    outfifo  = (unsigned char *)(MASTERBASE + MASTEROUT);

    if ( (*vmestat & 0x01) )
      {
	  printf("ecbSetDivN: command not pending, but ");
	  printf("IN FIFO is not empty.\n");
	  printf("ecbSetDivN: returning without issuing command...\n");
	  printf("ecbSetDivN: Re-Giving ecb_cmd_not_pending semaphore...\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetDivN: semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetDivN: indicating an invalid semaphore ID.\n");
		printf("ecbSetDivN: Returning without Give-ing semaphore.\n");
		return(4);
	    }
	  return(2);
      }

    if ( (*vmestat & 0x04) )
      {
	  printf("ecbSetDivN: command not pending, but ");
	  printf("OUTFIFO is not empty.\n");
	  printf("ecbSetDivN: returning without issuing command\n");
	  printf("ecbSetDivN: Re-Giving ecb_cmd_not_pending semaphore...\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetDivN: semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetDivN: indicating an invalid semaphore ID.\n");
		printf("ecbSetDivN: Returning without Give-ing semaphore.\n");
		return(4);
	    }
	  return(3);
      }

    *infifo = ecbadr;   /* write ecb address into master's IN FIFO */
    *infifo = (unsigned char) 2;  /* write numbytes into IN FIFO */

    if(xmittp==1)       /* check for xmit */
      *infifo = 0x14;   /* write command ID Code into IN FIFO */
    else if (!xmittp)   /* check for testpulse */
      *infifo = 0x15;   /* write command ID Code into IN FIFO */

    *infifo = N;        /* write N into IN FIFO */
    *infifo = (unsigned char) ECBEOS; /* write end-of-sequence into IN FIFO */

    return(0);
}
