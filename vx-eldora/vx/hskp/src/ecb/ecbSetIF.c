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
 * Revision 1.1  1992/05/29  19:24:07  shawn
 * Initial revision
 *
 *
 * description:
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
/*                  FIFO is not empty;  no action taken.                     */
/*              3 - "command-not-pending" semaphore has been given, but OUT  */
/*                  FIFO is not empty;  no action taken.                     */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>
#include "semLib.h"
#include "intLib.h"
#include "taskLib.h"

#include "ecbMaster.h"   /* general #defines for ecb master offsets */
#include "ecbStat.h"     /* global structures for ecb status */
#include "ecbSem.h"      /* semaphore definitions for ecb master */
#include "hskpInt.h"     /* interrupt vector definitions for hskp */

unsigned char ecbSetIF(unsigned char ecbadr,unsigned char unitnum,unsigned char filtnum)
{
    unsigned char *vmestat,*infifo,*outfifo;
    unsigned char readback=0,chksumback=0;

    if (semTake(ecb_cmd_not_pending,1) == ERROR)
      {
	  printf("ecbSetIF: command still pending... returning without issuing command.\n");
	  return(1);
      }
    
    vmestat = (unsigned char *)(MASTERBASE + MASTERSTAT);
    infifo  = (unsigned char *)(MASTERBASE + MASTERIN);
    outfifo  = (unsigned char *)(MASTERBASE + MASTEROUT);

    *infifo = ecbadr;  /* write ecb address into master's IN FIFO */
    *infifo = (unsigned char) 3;  /* write numbytes into IN FIFO */
    *infifo = 0x10;  /* write command ID Code into IN FIFO */
    *infifo = unitnum;  /* write IF unit number into IN FIFO */
    *infifo = filtnum;  /* write IF filter number into IN FIFO */
    *infifo = (unsigned char) ECBEOS; /* write end-of-sequence into IN FIFO */

    return(0);
}





