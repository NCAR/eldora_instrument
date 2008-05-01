/*
 *	$Id$
 *
 *	Module:		 ecbTestBim
 *	Original Author: Shawn B. Copeland
 *      Copyright by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * Revision 1.1  1992/06/16  22:24:11  shawn
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
/*	ecbTestBim()                                                         */
/*		Diagnostic routine to repeatedly write to the BIM            */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>
#include "semLib.h"
#include "intLib.h"

#include "ecbFunc.h"     /* function prototypes for ecb______ */
#include "ecbMaster.h"   /* general #defines for ecb master offsets */
#include "hskpInt.h"     /* interrupt vector definitions for hskp */

unsigned char ecbTestBim(unsigned int bimtries)
{
    unsigned char *ecb_bim_cr0,*ecb_bim_vr0,*ecb_bim_cr1,*ecb_bim_vr1;
    unsigned char *ecb_bim_cr2,*ecb_bim_vr2,*ecb_bim_cr3,*ecb_bim_vr3;
    unsigned char shitcr0,shitvr0;
    unsigned int bimwrcount;

    ecb_bim_cr0 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMCR0);
    ecb_bim_cr1 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMCR1);
    ecb_bim_cr2 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMCR2);
    ecb_bim_cr3 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMCR3);
    ecb_bim_vr0 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMVR0);
    ecb_bim_vr1 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMVR1);
    ecb_bim_vr2 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMVR2);
    ecb_bim_vr3 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMVR3);

    /* Initialize control and vector registers in ecb master BIM */
    for (bimwrcount=1; bimwrcount < bimtries; bimwrcount++)
      {
	  *ecb_bim_cr0 = (0xd8 | ECB_CMPLT_IRQ);
	                   /* set command complete interrupt level, */
                           /* enable the interrupt and set auto-clear */
                           /* bit (meaning that the ISR must re-enable */
                           /* the interrupt) */
	  *ecb_bim_vr0 = ECB_CMPLT_VEC;
	  *ecb_bim_cr1 = 0x00;   /* clear all bits in control register 1 */
	  *ecb_bim_vr1 = 0x0f;   /* set to uninitialized interrupt vector */
	  *ecb_bim_cr2 = 0x00;   /* clear all bits in control register 2 */
	  *ecb_bim_vr2 = 0x0f;   /* set to uninitialized interrupt vector */
	  *ecb_bim_cr3 = 0x00;   /* clear all bits in control register 3 */
	  *ecb_bim_vr3 = 0x0f;   /* set to uninitialized interrupt vector */
	  shitcr0 = *ecb_bim_cr0;
	  shitvr0 = *ecb_bim_vr0;
	  if((shitcr0 == (0xd8|ECB_CMPLT_IRQ)) && (shitvr0 == ECB_CMPLT_VEC))
	      break;
      }

printf("ecbTestBim: after %d writes, reading the BIM:\n",bimwrcount);
printf("    cr0 = 0x%2x, vr0 = 0x%2x\n",*ecb_bim_cr0,*ecb_bim_vr0);
printf("    cr1 = 0x%2x, vr1 = 0x%2x\n",*ecb_bim_cr1,*ecb_bim_vr1);
printf("    cr2 = 0x%2x, vr2 = 0x%2x\n",*ecb_bim_cr2,*ecb_bim_vr2);
printf("    cr3 = 0x%2x, vr3 = 0x%2x\n",*ecb_bim_cr3,*ecb_bim_vr3);

shitcr0 = *ecb_bim_cr0;
shitvr0 = *ecb_bim_vr0;
if (shitcr0 != (0xd8 | ECB_CMPLT_IRQ))
  {
      printf("ecbTestBim:  BIM CONTROL REGISTER 0 DOES NOT HOLD ");
      printf("CORRECT VALUE!\n");
      return(1);
  }
if (shitvr0 != ECB_CMPLT_VEC)
  {
      printf("ecbTestBim:  BIM VECTOR REGISTER 0 DOES NOT ");
      printf("HOLD CORRECT VALUE!\n");
      return(1);
  }   
return(0);
}





