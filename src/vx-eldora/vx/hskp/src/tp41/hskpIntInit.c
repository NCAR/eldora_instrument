/*
 *	$Id$
 *
 *	Module:		 hskpIntInit
 *	Original Author: Shawn B. Copeland
 *      Copyright by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1993/09/20  20:30:45  shawn
 * *** empty log message ***
 *
 * Revision 1.1  1992/06/19  00:25:57  shawn
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
/*	hskpIntInit()                                                        */
/*		Do HSKP interrupt initialization -- write to 68040 board's   */
/*              CIO1 register to enable VME interrupts by level.             */
/*      Return values:                                                       */
/*              void function, no values returned.                           */
/*****************************************************************************/

#define IRQ1 1       /* set these #defines to 1 to enable */
#define IRQ2 1       /* the VME interrupt level (onto the */
#define IRQ3 1       /* 68040 card), set them to 0 to     */
#define IRQ4 1       /* disable the VME interupt level    */
#define IRQ5 1
#define IRQ6 0
#define IRQ7 0
#define ACFAIL 0

#include <vxWorks.h>
#include <stdioLib.h>

#include "hskpInt.h"     /* interrupt vector definitions for hskp */

void hskpIntInit()
{
    unsigned char *cio1porta,workbyte;

    workbyte = !IRQ7<<7 | !IRQ6<<6| !IRQ5<<5| !IRQ4<<4 | !IRQ3<<3 | !IRQ2<<2 | !IRQ1<<1 | !ACFAIL;

    printf("hskpIntInit: workbyte = 0x%2x\n",workbyte);

    cio1porta   = (unsigned char *) 0x0d000008;

    *cio1porta  = workbyte;  /* enable IRQ* levels on tp41 */

}
