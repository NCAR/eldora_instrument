/*
 *	$Id$
 *
 *	Module:		 ecbSetDDS
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/06/19  01:05:00  shawn
 * Initial revision
 *
 * description: Writes "set DDS frequency" command to ECB MASTER IN FIFO
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
/*	ecbSetDDS()                                                          */
/*		Write "set DDS frequency" command to ECB MASTER IN FIFO      */
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
/*              5 - ecbadr passed is not for a RCVR/XCTR processor; no       */
/*                  action taken.                                            */
/*              6 - unitnum passed is out of range; no action taken.         */
/*              7 - requested frequency is out of range; no action taken.    */
/*              8 - nr value passed is out of range; no action taken.        */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>
#include "semLib.h"

#include "ecbFunc.h"     /* function prototypes for ecb______ */
#include "ecbMaster.h"   /* general #defines for ecb master offsets */
#include "ecbSem.h"      /* semaphore definitions for ecb master */
#include "ecbAdr.h"      /* Slave addresses on ECB bus */
#include "ecbErrBound.h" /* various #defines for error bounds */

unsigned char ecbSetDDS(unsigned char ecbadr,unsigned char unitnum,unsigned char B,unsigned long T,unsigned char nr)
{
    unsigned char *vmestat,*infifo,*outfifo;
    unsigned char onebyte=0;
    double actfreq,temp;

    /* Check for pending command */
    if (semTake(ecb_cmd_not_pending,1) == ERROR)
      {
	  printf("ecbSetDDS: command still pending...");
	  printf("Returning without issuing command.\n");
	  return(1);
      }

    /* Check for valid address */
    if (ecbadr!=ECBRFFOR && ecbadr!=ECBRFAFT)
      {
	  printf("ecbSetDDS: ecbadr passed [%d] is not the ",ecbadr);
	  printf("ECB bus address\n");
	  printf("ecbSetDDS: for a Rcvt/Xctr slave...");
	  printf("Returning without issuing command.\n");
	  printf("ecbSetDDS: Re-Giving ecb_cmd_not_pending Semaphore.\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetDDS:  semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetDDS:  indicating an invalid semaphore ID.\n");
		printf("ecbSetDDS:  Returning without Give-ing semaphore.\n");
		return(4);
	    }
	  return(5);
      }

    /* Check for valid unitnum */
    if (unitnum < ECBDDSLOWUNIT  ||  unitnum > ECBDDSHIGHUNIT)
      {
	  printf("ecbSetDDS: unitnum passed [%d] is out ",unitnum);
	  printf("of range (1..6)\n");
	  printf("ecbSetDDS: Returning without issuing command.\n");
	  printf("ecbSetDDS: Re-Giving ecb_cmd_not_pending Semaphore.\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetDDS:  semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetDDS:  indicating an invalid semaphore ID.\n");
		printf("ecbSetDDS:  Returning without Give-ing semaphore.\n");
		return(4);
	    }
	  return(6);
      }

    /* Check for valid requested frequency */
    temp = T * (double) (B+1) / 1431655765.333333333333;
    actfreq = 6.0e7 * (181.0 - temp);

#ifdef ECBDEBUG
    printf("ecbSetDDS: actual frequency requested was %lf\n",actfreq);
#endif

    if (actfreq < ECBDDSLOWFREQ  ||  actfreq > ECBDDSHIGHFREQ)
      {
	  printf("ecbSetDDS: frequency passed [%lf] is out ",actfreq);
	  printf("of range.\n");
	  printf("ecbSetDDS: Returning without issuing command.\n");
	  printf("ecbSetDDS: Re-Giving ecb_cmd_not_pending Semaphore.\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetDDS:  semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetDDS:  indicating an invalid semaphore ID.\n");
		printf("ecbSetDDS:  Returning without Give-ing semaphore.\n");
		return(4);
	    }
	  return(7);
      }

    if (nr != 0 && nr != 1)
      {
	  printf("ecbSetDDS: nr value passed [%u] is out ",nr);
	  printf("of range.\n");
	  printf("ecbSetDDS: Returning without issuing command.\n");
	  printf("ecbSetDDS: Re-Giving ecb_cmd_not_pending Semaphore.\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetDDS:  semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetDDS:  indicating an invalid semaphore ID.\n");
		printf("ecbSetDDS:  Returning without Give-ing semaphore.\n");
		return(4);
	    }
	  return(8);
      }

    vmestat = (unsigned char *)(MASTERBASE + MASTERSTAT);
    infifo  = (unsigned char *)(MASTERBASE + MASTERIN);
    outfifo  = (unsigned char *)(MASTERBASE + MASTEROUT);

    if ( (*vmestat & 0x01) )
      {
	  printf("ecbSetDDS: command not pending, but ");
	  printf("IN FIFO is not empty.\n");
	  printf("ecbSetDDS: returning without issuing command, re-Giving\n");
	  printf("ecbSetDDS: ecb_cmd_not_pending semaphore...\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetDDS:  semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetDDS:  indicating an invalid semaphore ID.\n");
		printf("ecbSetDDS:  Returning without Give-ing semaphore.\n");
		return(4);
	    }
	  return(2);
      }

    if ( (*vmestat & 0x04) )
      {
	  printf("ecbSetDDS: command not pending, but ");
	  printf("OUTFIFO is not empty.\n");
	  printf("ecbSetDDS: returning without issuing command, re-Giving\n");
	  printf("ecbSetDDS: ecb_cmd_not_pending semaphore...\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetDDS:  semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetDDS:  indicating an invalid semaphore ID.\n");
		printf("ecbSetDDS:  Returning without Give-ing semaphore.\n");
		return(4);
	    }
	  return(3);
      }

    *infifo = ecbadr;  /* write ecb address into master's IN FIFO */
    *infifo = (unsigned char) 8;  /* write numbytes into IN FIFO */
    *infifo = 0x10;  /* write command ID Code into IN FIFO */
    *infifo = unitnum;  /* write IF unit number into IN FIFO */
    *infifo = B;  /* write B value into IN FIFO */
    onebyte = (unsigned char) ((T & 0xFF000000) >> 24);
    *infifo = onebyte; /* write MSB of T to IN FIFO */
    onebyte = (unsigned char) ((T & 0x00FF0000) >> 16);
    *infifo = onebyte; /* write next-to-MSB of T to IN FIFO */
    onebyte = (unsigned char) ((T & 0x0000FF00) >> 8);
    *infifo = onebyte; /* write next-to-LSB of T to IN FIFO */
    onebyte = (unsigned char) (T & 0x000000FF);
    *infifo = onebyte; /* write LSB of T to IN FIFO */
    *infifo = nr;      /* write nr value to IN FIFO */
    *infifo = (unsigned char) ECBEOS; /* write end-of-sequence into IN FIFO */

    return(0);
}

