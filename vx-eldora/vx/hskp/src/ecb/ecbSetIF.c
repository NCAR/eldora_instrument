/*
 *	$Id$
 *
 *	Module:		 ecbSetIF
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1992/06/19  01:03:24  shawn
 * Does error checking for appropriate ecbadr.  Checks for empty IN and
 * OUT FIFO's and returns an error code if non-empty.
 *
 * Revision 1.1  1992/06/16  22:22:43  shawn
 * Initial revision
 *
 * description: Write "set IF processor filter" command to ECB MASTER IN FIFO
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
/*	ecbSetIF()                                                           */
/*		Write "set IF processor filter" command to ECB MASTER IN FIFO*/
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
/*              5 - ecbadr passed is not for an IF processor; no action      */
/*                  taken.                                                   */
/*              6 - unitnum passed is out of range; no action taken.         */
/*              7 - filtnum passed is out of range; no action taken.         */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>
#include "semLib.h"

#include "ecbFunc.h"     /* function prototypes for ecb______ */
#include "ecbMaster.h"   /* general #defines for ecb master offsets */
#include "ecbSem.h"      /* semaphore definitions for ecb master */
#include "ecbAdr.h"      /* Slave addresses on ECB bus */
#include "ecbErrBound.h" /* various #defines for error bounds */

unsigned char ecbSetIF(unsigned char ecbadr,unsigned char unitnum,unsigned char filtnum)
{
    unsigned char *vmestat,*infifo,*outfifo;
    unsigned char readback=0,chksumback=0;

    /* Check for pending Command */
    if (semTake(ecb_cmd_not_pending,1) == ERROR)
      {
	  printf("ecbSetIF: command still pending...");
	  printf("Returning without issuing command.\n");
	  return(1);
      }

    /* Check for valid ecbadr */
    if (ecbadr!=ECBIFFOR && ecbadr!=ECBIFAFT)
      {
	  printf("ecbSetIF: ecbadr passed [%d] is not the ",ecbadr);
	  printf("ECB bus address\n");
	  printf("ecbSetIF: for an IF Processor slave...");
	  printf("Returning without issuing command.\n");
	  printf("ecbSetIF: Re-Giving ecb_cmd_not_pending Semaphore.\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetIF:  semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetIF:  indicating an invalid semaphore ID.\n");
		printf("ecbSetIF:  Returning without Give-ing semaphore.\n");
		return(4);
	    }
	  return(5);
      }

    /* Check for valid unitnum */
    if (unitnum < ECBIFLOWUNIT  ||  unitnum > ECBIFHIGHUNIT)
      {
	  printf("ecbSetIF: unitnum passed [%d] is out of ",unitnum);
	  printf("range (0..4).\n");
	  printf("ecbSetIF: Returning without issuing command.\n");
	  printf("ecbSetIF: Re-Giving ecb_cmd_not_pending Semaphore.\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetIF:  semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetIF:  indicating an invalid semaphore ID.\n");
		printf("ecbSetIF:  Returning without Give-ing semaphore.\n");
		return(4);
	    }
	  return(6);
      }

    /* Check for valid filtnum */
    if (filtnum < ECBIFLOWFILT  ||  filtnum > ECBIFHIGHFILT)
      {
	  printf("ecbSetIF: filtnum passed [%d] is out of ",filtnum);
	  printf("range (1..5).\n");
	  printf("ecbSetIF: Returning without issuing command.\n");
	  printf("ecbSetIF: Re-Giving ecb_cmd_not_pending Semaphore.\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetIF:  semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetIF:  indicating an invalid semaphore ID.\n");
		printf("ecbSetIF:  Returning without Give-ing semaphore.\n");
		return(4);
	    }
	  return(7);
      }

    vmestat = (unsigned char *)(MASTERBASE + MASTERSTAT);
    infifo  = (unsigned char *)(MASTERBASE + MASTERIN);
    outfifo  = (unsigned char *)(MASTERBASE + MASTEROUT);

    if ( (*vmestat & 0x01) )
      {
	  printf("ecbSetIF: command not pending, but IN FIFO is not empty.\n");
	  printf("ecbSetIF: returning without issuing command, re-Giving\n");
	  printf("ecbSetIF: ecb_cmd_not_pending semaphore...\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetIF:  semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetIF:  indicating an invalid semaphore ID.\n");
		printf("ecbSetIF:  Returning without Give-ing semaphore.\n");
		return(4);
	    }
	  return(2);
      }

    if ( (*vmestat & 0x04) )
      {
	  printf("ecbSetIF: command not pending, but OUTFIFO is not empty.\n");
	  printf("ecbSetIF: returning without issuing command, re-Giving\n");
	  printf("ecbSetIF: ecb_cmd_not_pending semaphore...\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetIF:  semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetIF:  indicating an invalid semaphore ID.\n");
		printf("ecbSetIF:  Returning without Give-ing semaphore.\n");
		return(4);
	    }
	  return(3);
      }

    *infifo = ecbadr;  /* write ecb address into master's IN FIFO */
    *infifo = (unsigned char) 3;  /* write numbytes into IN FIFO */
    *infifo = 0x10;  /* write command ID Code into IN FIFO */
    *infifo = unitnum;  /* write IF unit number into IN FIFO */
    *infifo = filtnum;  /* write IF filter number into IN FIFO */
    *infifo = (unsigned char) ECBEOS; /* write end-of-sequence into IN FIFO */

    return(0);
}
