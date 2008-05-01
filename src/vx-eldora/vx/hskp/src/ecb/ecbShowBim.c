/*
 *	$Id$
 *
 *	Module:		 ecbShowBim
 *	Original Author: Shawn B. Copeland
 *      Copyright by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/06/26  00:29:49  shawn
 * Initial revision
 *
 *
 * description: Read registers of BIM on ECB MASTER card and print results.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
/*	ecbShowBim()                                                         */
/*              Read registers of BIM on ECB MASTER card and print results.  */
/*      Return values:                                                       */
/*              void function, no values returned.                           */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>
#include "semLib.h"
#include "intLib.h"

#include "ecbFunc.h"     /* function prototypes for ecb______ */
#include "ecbMaster.h"   /* general #defines for ecb master offsets */
#include "hskpInt.h"     /* interrupt vector definitions for hskp */

void ecbShowBim()
{
    unsigned char *ecb_bim_cr0,*ecb_bim_vr0,*ecb_bim_cr1,*ecb_bim_vr1;
    unsigned char *ecb_bim_cr2,*ecb_bim_vr2,*ecb_bim_cr3,*ecb_bim_vr3;
    unsigned char shitcr,shitvr;

    ecb_bim_cr0 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMCR0);
    ecb_bim_cr1 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMCR1);
    ecb_bim_cr2 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMCR2);
    ecb_bim_cr3 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMCR3);
    ecb_bim_vr0 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMVR0);
    ecb_bim_vr1 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMVR1);
    ecb_bim_vr2 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMVR2);
    ecb_bim_vr3 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMVR3);

    shitcr = *ecb_bim_cr0;
    shitvr = *ecb_bim_vr0;

    if (shitcr != (0xd8 | ECB_CMPLT_IRQ))
      printf("ecbShowBim:  BIM CONTROL REG 0 DOES NOT HOLD CORRECT VALUE!\n");
    if (shitvr != ECB_CMPLT_VEC)
      printf("ecbShowBim:  BIM VECTOR REG 0 DOES NOT HOLD CORRECT VALUE!\n");
    
    printf("ecbShowBim: values read from the BIM:\n");
    printf("            cr0 = 0x%2x, vr0 = 0x%2x\n",shitcr,shitvr);
    shitcr = *ecb_bim_cr1;
    shitvr = *ecb_bim_vr1;
    printf("            cr1 = 0x%2x, vr1 = 0x%2x\n",shitcr,shitvr);
    shitcr = *ecb_bim_cr2;
    shitvr = *ecb_bim_vr2;
    printf("            cr2 = 0x%2x, vr2 = 0x%2x\n",shitcr,shitvr);
    shitcr = *ecb_bim_cr3;
    shitvr = *ecb_bim_vr3;
    printf("            cr3 = 0x%2x, vr3 = 0x%2x\n",shitcr,shitvr);
}
