/*
 *	$Id$
 *
 *	Module:		 ecbSetDDSfloat
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
 *              This version takes a double for frequency (useful for debug)
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
/*	ecbSetDDSfloat()                                                     */
/*		Write "set DDS frequency" command to ECB MASTER IN FIFO      */
/*              Takes a double as argument for frequency (useful for debug)  */
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
/*              7 - desfreq passed is not in valid range (LOWFREQ..HIGHFREQ) */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>
#include "semLib.h"

#include "ecbFunc.h"     /* function prototypes for ecb______ */
#include "ecbMaster.h"   /* general #defines for ecb master offsets */
#include "ecbSem.h"      /* semaphore definitions for ecb master */
#include "ecbAdr.h"      /* Slave addresses on ECB bus */
#include "ecbErrBound.h" /* various #defines for error bounds */

unsigned char ecbSetDDSfloat(unsigned char ecbadr,unsigned char unitnum,double
desfreq)
{
    unsigned char *vmestat,*infifo,*outfifo;
    unsigned char B=0,onebyte=0;
    unsigned long T=0;
    double actfreq=0.0,temp=0.0;

    /* Check for pending command */
    if (semTake(ecb_cmd_not_pending,1) == ERROR)
      {
	  printf("ecbSetDDSfloat: command still pending...");
	  printf("Returning without issuing command.\n");
	  return(1);
      }

    /* Check for valid address */
    if (ecbadr!=ECBRFFOR && ecbadr!=ECBRFAFT)
      {
	  printf("ecbSetDDSfloat: ecbadr passed [%d] is not the ",ecbadr);
	  printf("ECB bus address\n");
	  printf("ecbSetDDSfloat: for a Rcvt/Xctr slave...");
	  printf("Returning without issuing command.\n");
	  printf("ecbSetDDSfloat: Re-Giving ecb_cmd_not_pending Semaphore.\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetDDSfloat: semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetDDSfloat: indicating an invalid semaphore ID\n");
		printf("ecbSetDDSfloat: Returning without Give-ing ");
		printf("semaphore.\n");
		return(4);
	    }
	  return(5);
      }

    /* Check for valid unitnum */
    if (unitnum < ECBDDSLOWUNIT  ||  unitnum > ECBDDSHIGHUNIT)
      {
	  printf("ecbSetDDSfloat: unitnum passed [%d] is out ",unitnum);
	  printf("of range (1..6)\n");
	  printf("ecbSetDDSfloat: Returning without issuing command.\n");
	  printf("ecbSetDDSfloat: Re-Giving ecb_cmd_not_pending Semaphore.\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetDDSfloat: semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetDDSfloat: indicating an invalid semaphore ID\n");
		printf("ecbSetDDSfloat: Returning without Give-ing ");
		printf("semaphore.\n");
		return(4);
	    }
	  return(6);
      }

    /* Check for valid requested frequency */
    if (desfreq < ECBDDSLOWFREQ  ||  desfreq > ECBDDSHIGHFREQ)
      {
	  printf("ecbSetDDSfloat: desfreq passed [%lf] is out ",desfreq);
	  printf("of range.\n");
	  printf("ecbSetDDSfloat: Returning without issuing command.\n");
	  printf("ecbSetDDSfloat: Re-Giving ecb_cmd_not_pending Semaphore.\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetDDSfloat: semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetDDSfloat: indicating an invalid semaphore ID\n");
		printf("ecbSetDDSfloat: Returning without Give-ing ");
		printf("semaphore.\n");
		return(4);
	    }
	  return(7);
      }

    vmestat = (unsigned char *)(MASTERBASE + MASTERSTAT);
    infifo  = (unsigned char *)(MASTERBASE + MASTERIN);
    outfifo  = (unsigned char *)(MASTERBASE + MASTEROUT);

    if ( (*vmestat & 0x01) )
      {
	  printf("ecbSetDDSfloat: command not pending, but ");
	  printf("IN FIFO is not empty.\n");
	  printf("ecbSetDDSfloat: returning without issuing command, ");
	  printf("re-Giving\n");
	  printf("ecbSetDDSfloat: ecb_cmd_not_pending semaphore...\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetDDSfloat: semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetDDSfloat: indicating an invalid semaphore ID\n");
		printf("ecbSetDDSfloat: Returning without Give-ing ");
		printf("semaphore.\n");
		return(4);
	    }
	  return(2);
      }

    if ( (*vmestat & 0x04) )
      {
	  printf("ecbSetDDSfloat: command not pending, but ");
	  printf("OUTFIFO is not empty.\n");
	  printf("ecbSetDDSfloat: returning without issuing command, ");
	  printf("re-Giving\n");
	  printf("ecbSetDDSfloat: ecb_cmd_not_pending semaphore...\n");
	  if(semGive(ecb_cmd_not_pending) == ERROR)
	    {
		printf("ecbSetDDSfloat: semGive(ecb_cmd_not_pending) ");
		printf("returned ERROR,\n");
		printf("ecbSetDDSfloat: indicating an invalid semaphore ID\n");
		printf("ecbSetDDSfloat: Returning without Give-ing ");
		printf("semaphore.\n");
		return(4);
	    }
	  return(3);
      }

    /****************************************************************/
    /* do the calculations for B, and T here                        */
    /****************************************************************/

    B = (unsigned char) (361 - (int)(desfreq / 30.0e6));
    temp = 15445.333333333333333 - (desfreq / 703125.0);
    temp = (16777216.0 / (B+1)) * temp;
    T = (unsigned long) (temp + 0.5);

    temp = T * (double) (B+1) / 1431655765.333333333333;
    actfreq = 6.0e7 * (181.0 - temp);

#ifdef ECBDEBUG
    printf("ecbSetDDSfloat: requested frequency was %lf\n",desfreq);
    printf("ecbSetDDSfloat: quantized frequency programmed was %lf\n",actfreq);
#endif

    *infifo = ecbadr;  /* write ecb address into master's IN FIFO */
    *infifo = (unsigned char) 7;  /* write numbytes into IN FIFO */
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
    *infifo = (unsigned char) ECBEOS; /* write end-of-sequence into IN FIFO */

    return(0);
}

