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
 * Revision 1.1  1992/06/26  00:23:54  shawn
 * Initial revision
 *
 * description: Clear ECB MASTER IN FIFO AND OUT FIFO.  Useful for debug.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
/*	ecbClrFifos()                                                        */
/*		Clear the IN FIFO and OUT FIFO on the ECB MASTER card.       */
/*              Useful for debugging. Assumes HC11 is running, leaves it     */
/*              running. IF THE HC11 IS STOPPED, THIS ROUTINE WILL START IT! */
/*      Return values:                                                       */
/*              none, void fuction (it's a tool for debug)                   */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>
#include "semLib.h"

#include "ecbFunc.h"     /* function prototypes for ecb______ */
#include "ecbMaster.h"   /* general #defines for ecb master offsets */

void ecbClrFifos()
{
unsigned char *ecb_vme_ctl;

ecb_vme_ctl = (unsigned char *) (MASTERBASE + MASTERCTL);

/* Make sure interrupt requests lines (INT0* - INT3*) to the BIM on */
/* the ECB MASTER are cleared, clear FIFO's at the same time */

*ecb_vme_ctl = 0xdf;   /* bring VMEINCLR* and VMEOUTCLR* lines low, leaving
			  hc11 running */
*ecb_vme_ctl = 0x1f;   /* bring VMEINCLR* and VMEOUTCLR* lines high, leaving
			  hc11 running */

}
