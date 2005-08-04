/*
 *	$Id$
 *
 *	Module:		 ecbSetAtten
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1996/12/09  20:05:41  craig
 * *** empty log message ***
 *
 * Revision 1.2  1993/09/20  20:25:20  shawn
 * *** empty log message ***
 *
 * Revision 1.1  1992/06/23  19:44:45  shawn
 * Initial revision
 *
 * description: Write "Set Attenuator value" command into ECB MASTER IN FIFO
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
/*	ecbSetAtten()                                                        */
/*		Write "set Attenuator" command to ECB MASTER IN FIFO         */
/*              Note:  foraft = 1 means forward, = 0 means aft.              */
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
/*              6 - foraft passed is not 1 or 0; no action taken.            */
/*              7 - attenval out of range; no action taken.                  */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>
#include "semLib.h"

#include "ecbFunc.h"     /* function prototypes for ecb______ */
#include "ecbMaster.h"   /* general #defines for ecb master offsets */
#include "ecbSem.h"      /* semaphore definitions for ecb master */
#include "ecbAdr.h"      /* Slave addresses on ECB bus */
#include "ecbErrBound.h" /* various #defines for error bounds */

unsigned char ecbSetAtten(unsigned char ecbadr,unsigned char foraft,unsigned char attenval)
{
    unsigned char *vmestat,*infifo,*outfifo;

    /* Check for pending command */
    if (semTake(ecb_cmd_not_pending,1) == ERROR)
      {
/*	  printf("ecbSetAtten: command still pending...");
	  printf("Returning without issuing command.\n");
*/	  return(1);
      }
    
    /* Check for valid ecbadr */
    if (ecbadr!=ECBATTEN)
      {
	  printf("ecbSetAtten: ecbadr passed [%d] is not the ",ecbadr);
	  printf("ECB bus address\n");
	  printf("ecbSetAtten: for the ATTENUATOR chassis...");
	  printf("Returning without issuing command.\n");
	  printf("ecbSetAtten: Re-Giving ecb_cmd_not_pending Semaphore.\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetAtten: semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetAtten: indicating an invalid semaphore ID.\n");
		printf("ecbSetAtten: Returning without Give-ing semaphore.\n");
		return(4);
	    }
	  return(5);
      }
    
    /* Check for valid foraft */
    if (foraft != 0 && foraft != 1)
      {
	  printf("ecbSetAtten: foraft passed [%d] is not ",foraft);
	  printf("a valid value (0 or 1).\n");
	  printf("ecbSetAtten: Returning without issuing command.\n");
	  printf("ecbSetAtten: Re-Giving ecb_cmd_not_pending Semaphore.\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetAtten: semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetAtten: indicating an invalid semaphore ID.\n");
		printf("ecbSetAtten: Returning without Give-ing semaphore.\n");
		return(4);
	    }
	  return(6);
      }

    /* Check for valid attenval */
    if ((attenval < ECBATLOWATTEN) || (attenval > ECBATHIGHATTEN))
      {
	  printf("ecbSetAtten: attenuator value passed [%u] is out ",attenval);
	  printf("of range (0..121).\n");
	  printf("ecbSetAtten: Returning without issuing command.\n");
	  printf("ecbSetAtten: Re-Giving ecb_cmd_not_pending Semaphore.\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetAtten: semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetAtten: indicating an invalid semaphore ID.\n");
		printf("ecbSetAtten: Returning without Give-ing semaphore.\n");
		return(4);
	    }
	  return(7);
      }

    vmestat = (unsigned char *)(MASTERBASE + MASTERSTAT);
    infifo  = (unsigned char *)(MASTERBASE + MASTERIN);
    outfifo  = (unsigned char *)(MASTERBASE + MASTEROUT);

    if ( (*vmestat & 0x01) )
      {
	  printf("ecbSetAtten: command not pending, but ");
	  printf("IN FIFO is not empty.\n");
	  printf("ecbSetAtten: returning without issuing command...\n");
	  printf("ecbSetAtten: Re-Giving ecb_cmd_not_pending semaphore...\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetAtten: semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetAtten: indicating an invalid semaphore ID.\n");
		printf("ecbSetAtten: Returning without Give-ing semaphore.\n");
		return(4);
	    }
	  return(2);
      }

    if ( (*vmestat & 0x04) )
      {
	  printf("ecbSetAtten: command not pending, but ");
	  printf("OUTFIFO is not empty.\n");
	  printf("ecbSetAtten: returning without issuing command\n");
	  printf("ecbSetAtten: Re-Giving ecb_cmd_not_pending semaphore...\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetAtten: semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetAtten: indicating an invalid semaphore ID.\n");
		printf("ecbSetAtten: Returning without Give-ing semaphore.\n");
		return(4);
	    }
	  return(3);
      }

    *infifo = ecbadr;   /* write ecb address into master's IN FIFO */
    *infifo = (unsigned char) 2;  /* write numbytes into IN FIFO */

    if(foraft==1)       /* check for forward */
      *infifo = 0x10;   /* write command ID Code into IN FIFO */
    else if (!foraft)   /* check for aft */
      *infifo = 0x11;   /* write command ID Code into IN FIFO */

    *infifo = attenval; /* write attenuator value into IN FIFO */
    *infifo = (unsigned char) ECBEOS; /* write end-of-sequence into IN FIFO */

    return(0);
}
