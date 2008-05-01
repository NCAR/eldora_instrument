/*
 *	$Id$
 *
 *	Module:		 ecbShowMstat
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1993/09/20  20:25:32  shawn
 * *** empty log message ***
 *
 * Revision 1.1  1992/06/18  20:01:57  shawn
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
/*	ecbShowMstat()                                                       */
/*		Read VME Status Register on ECB MASTER card and print result */
/*      Return values:                                                       */
/*              void function, no value returned.                            */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>

#include "ecbFunc.h"     /* function prototypes for ecb______ */
#include "ecbMaster.h"   /* general #defines for ecb master offsets */

void ecbShowMstat()
{
    unsigned char *ecb_vme_stat; /* pointer to ECB master vme status reg */
    ecb_vme_stat  = (unsigned char *) (MASTERBASE + MASTERSTAT);
    
    printf("ecbShowMstat: ECB Master VME Status Register = 0x%2x\n",*ecb_vme_stat);
    printf("ecbShowMstat: For you stupid bastards, this means:\n");
    printf("              The IN FIFO is ");
    if (*ecb_vme_stat & 0x01)
      printf("not empty.\n");
    else
      printf("empty.\n");
    printf("              The IN FIFO is ");
    if (*ecb_vme_stat & 0x02)
      printf("not half full.\n");
    else
      printf("half full.\n");
    printf("              The OUT FIFO is ");
    if (*ecb_vme_stat & 0x04)
      printf("not empty.\n");
    else
      printf("empty.\n");
    printf("              The OUT FIFO is ");
    if (*ecb_vme_stat & 0x08)
      printf("not half full.\n");
    else
      printf("half full.\n");
    printf("              The Latched Cmd Complete IRQ line is ");
    if (*ecb_vme_stat & 0x10)
      printf("not asserted.\n");
    else
      printf("asserted.\n");
    printf("              The Latched Command Error IRQ line is ");
    if (*ecb_vme_stat & 0x20)
      printf("not asserted.\n");
    else
      printf("asserted.\n");
    printf("              The Latched Spare IRQ line is ");
    if (*ecb_vme_stat & 0x40)
      printf("not asserted.\n");
    else
      printf("asserted.\n");
    printf("              The Latched DP Contention IRQ line is ");
    if (*ecb_vme_stat & 0x80)
      printf("not asserted.\n");
    else
      printf("asserted.\n");
}

