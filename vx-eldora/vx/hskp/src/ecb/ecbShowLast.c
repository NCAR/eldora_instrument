/*
 *	$Id$
 *
 *	Module:		 ecbShowLast
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/06/16  22:22:43  shawn
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
/*	ecbShowLast()                                                           */
/*		Write "set IF processor filter" command to ECB MASTER IN FIFO*/
/*      Return values:                                                       */
/*              0 - command placed into IN FIFO and "command-not-pending"    */
/*                  semaphore taken.                                         */
/*              1 - previous command still pending; no action taken.         */
/*              2 - "command-not-pending" semaphore has been given, but IN   */
/*                  FIFO is not empty;  no action taken, semaphore re-given. */
/*              3 - "command-not-pending" semaphore has been given, but OUT  */
/*                  FIFO is not empty;  no action taken, semaphore re-given. */
/*              4 - error when re-Give-ing ecb_cmd_not_pending semaphore.    */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>

#include "ecbStat.h"     /* global structures for ecb status */

void ecbShowLast()
{
printf("ecbShowLast: values in ecbLastCmd global status structure:\n");
printf("ecbShowLast: ecbadr ==> 0x%x, comID ==> 0x%x\n",ecbLastCmd.ecbadr,ecbLastCmd.comID);
}
