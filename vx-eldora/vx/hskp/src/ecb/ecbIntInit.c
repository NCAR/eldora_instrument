/*
 *	$Id$
 *
 *	Module:		 ecbIntInit
 *	Original Author: Shawn B. Copeland
 *      Copyright by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
/*	ecbIntInit()                                                         */
/*		Initialize ecb interrupts -- write to BIM, attach ISR, etc.  */
/*      Return values:                                                       */
/*              void function, no values returned.                           */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>
#include "semLib.h"
#include "intLib.h"
#include "taskLib.h"

#include "ecbMaster.h"   /* general #defines for ecb master offsets */
#include "ecbStat.h"     /* global structures for ecb status */
#include "hskpInt.h"     /* interrupt vector definitions for hskp */
#define MAINPROG
#include "ecbSem.h"      /* semaphore definitions for ecb master */

void ecbIntInit()
{
    unsigned char *ecb_bim_cr0,*ecb_bim_vr0,*ecb_bim_cr1,*ecb_bim_vr1;
    unsigned char *ecb_bim_cr2,*ecb_bim_vr2,*ecb_bim_cr3,*ecb_bim_vr3;
    unsigned char *ecb_vme_ctl;
    unsigned char shitcr0,shitvr0;
    unsigned char *cio1porta;

    ecb_bim_cr0 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMCR0);
    ecb_bim_cr1 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMCR1);
    ecb_bim_cr2 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMCR2);
    ecb_bim_cr3 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMCR3);
    ecb_bim_vr0 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMVR0);
    ecb_bim_vr1 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMVR1);
    ecb_bim_vr2 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMVR2);
    ecb_bim_vr3 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMVR3);

    ecb_vme_ctl = (unsigned char *) (MASTERBASE + MASTERCTL);    

    cio1porta   = (unsigned char *) 0x0d000008;

    /* Make sure interrupt requests lines (INT0* - INT3*) to the BIM on */
    /* the ECB MASTER are cleared */

    *ecb_vme_ctl = 0x00;   /* bring CLRINTn* lines low, leaving hc11 reset */
    *ecb_vme_ctl = 0x0f;   /* bring CLRINTn* lines high, leaving hc11 reset */

    /* Attach the command complete ISR to the appropriate vector */
    if(intConnect((VOIDFUNCPTR *)(ECB_CMPLT_VEC * 4), (VOIDFUNCPTR) ecbCmpltIsr,0) == ERROR)
      {
	  fprintf(stderr,"failed to connect interrupt vector %d. Exiting.\n",ECB_CMPLT_VEC);
	  exit(1);
      }
    
    /* Initialize control and vector registers in ecb master BIM */
    *ecb_bim_cr0 = 0xd9;   /* set command complete interrupt to IRQ1* */
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
printf("ecbIntInit: after writes, reading the BIM:\n");
printf("            cr0 = 0x%2x, vr0 = 0x%2x\n",shitcr0,shitvr0);

    *cio1porta = (unsigned char) 0xfd;  /* enable IRQ*(1) level int. on tp41 */

    /* Create (and Give) the command-not-pending semaphore */
    ecb_cmd_not_pending = semBCreate(SEM_Q_FIFO,SEM_EMPTY);
    if (ecb_cmd_not_pending == NULL)
      {
	  printf("ecbIntInit:  semBCreate returned NULL for ecb_cmd_not_pending.\n");
	  printf("ecbIntInit:  returning without creating or Give-ing semaphore.\n");
	  return;
      }
    if(semGive(ecb_cmd_not_pending) == ERROR)
      {
	  printf("ecbIntInit:  semGive(ecb_cmd_not_pending) returned ERROR,\n");
	  printf("ecbIntInit:  indicating an invalid semaphore ID.\n");
	  printf("ecbIntInit:  Returning without Give-ing the semaphore.\n");
	  return;
      }
    if(semTake(ecb_cmd_not_pending,NO_WAIT) == ERROR)
      {
	  printf("ecbIntInit:  semTake(ecb_cmd_not_pending) returned ERROR,\n");
	  return;
      }
    if(semGive(ecb_cmd_not_pending) == ERROR)
      {
	  printf("ecbIntInit:  2nd semGive(ecb_cmd_not_pending) returned ERROR,\n");
	  printf("ecbIntInit:  indicating an invalid semaphore ID.\n");
	  printf("ecbIntInit:  Returning without Give-ing the semaphore.\n");
	  return;
      }
}





