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
 * Revision 1.2  1992/06/19  01:07:25  shawn
 * removed writes to 68040 board (tp41) CIO1 register.  These were moved to
 * hskpIntInit.
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
/*	ecbIntInit()                                                         */
/*		Initialize ecb interrupts -- write to BIM, attach ISR, etc.  */
/*              Also the place where the global ecb status structures are    */
/*              actually defined (everywhere else is "extern")               */
/*              This function assumes that hskpIntInit has been or will soon */
/*              be called, to enable VME IRQ's to interrupt the tp41 card    */
/*              The ecb parts of the global rpc status structure are         */
/*              initialized here.                                            */
/*              bimtries is number of times to try programming BIM           */
/*      Return values:                                                       */
/*              0 -- BIM written successfully.                               */
/*              1 -- BIM vr0 or cr0 not written sucessfully.                 */
/*              2 -- BIM vr1 or cr1 not written sucessfully.                 */
/*              3 -- Semaphore error.                                        */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>
#include "semLib.h"
#include "intLib.h"

#include "ecbFunc.h"     /* function prototypes for ecb______ */
#include "ecbMaster.h"   /* general #defines for ecb master offsets */
#include "hskpInt.h"     /* interrupt vector definitions for hskp */
#define OK_RPC           /* makes definitions active in HskpStatus.h */
#include "HskpStatus.h"  /* global housekeeping status... rpc to cntl proc */
#define MAINPROG
#include "ecbStat.h"     /* global structures for ecb status */
#include "ecbSem.h"      /* semaphore definitions for ecb master */

unsigned char ecbIntInit(unsigned int bimtries)
{
    unsigned char *ecb_bim_cr0,*ecb_bim_vr0,*ecb_bim_cr1,*ecb_bim_vr1;
    unsigned char *ecb_bim_cr2,*ecb_bim_vr2,*ecb_bim_cr3,*ecb_bim_vr3;
    unsigned char *ecb_vme_ctl;
    unsigned char shitcr0,shitvr0,shitcr1,shitvr1;
    unsigned int bimwrcount;

    ecb_bim_cr0 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMCR0);
    ecb_bim_cr1 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMCR1);
    ecb_bim_cr2 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMCR2);
    ecb_bim_cr3 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMCR3);
    ecb_bim_vr0 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMVR0);
    ecb_bim_vr1 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMVR1);
    ecb_bim_vr2 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMVR2);
    ecb_bim_vr3 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMVR3);

    ecb_vme_ctl = (unsigned char *) (MASTERBASE + MASTERCTL);    

    /* Make sure interrupt requests lines (INT0* - INT3*) to the BIM on */
    /* the ECB MASTER are cleared, clear FIFO's at the same time */

    *ecb_vme_ctl = 0xc0;   /* bring CLRINTn*, VMEINCLR* and VMEOUTCLR* lines
			      low, leaving hc11 reset */
    *ecb_vme_ctl = 0x0f;   /* bring CLRINTn*, VMEINCLR* and VMEOUTCLR* lines
			      high, leaving hc11 reset */

    /* Attach the command complete ISR to the appropriate vector */
    if(intConnect((VOIDFUNCPTR *)(ECB_CMPLT_VEC * 4), (VOIDFUNCPTR) ecbCmpltIsr,0) == ERROR)
      {
	  fprintf(stderr,"failed to connect interrupt vector %d. Exiting.\n",ECB_CMPLT_VEC);
	  exit(1);
      }
    
    /* Attach the command error ISR to the appropriate vector */
    if(intConnect((VOIDFUNCPTR *)(ECB_ERROR_VEC * 4), (VOIDFUNCPTR) ecbErrorIsr,0) == ERROR)
      {
	  fprintf(stderr,"failed to connect interrupt vector %d. Exiting.\n",ECB_ERROR_VEC);
	  exit(1);
      }
    
    /* Initialize control and vector registers in ecb master BIM */
    for (bimwrcount=1; bimwrcount < bimtries; bimwrcount++)
      {
	  *ecb_bim_cr0 = (0xd8 | ECB_CMPLT_IRQ);
	                   /* set command complete interrupt level, */
                           /* enable the interrupt and set auto-clear */
                           /* bit (meaning that the ISR must re-enable */
                           /* the interrupt) */
	  *ecb_bim_vr0 = ECB_CMPLT_VEC;
	  *ecb_bim_cr1 = (0xd8 | ECB_ERROR_IRQ);
	                   /* set command error interrupt level, */
                           /* enable the interrupt and set auto-clear */
                           /* bit (meaning that the ISR must re-enable */
                           /* the interrupt) */
	  *ecb_bim_vr1 = ECB_ERROR_VEC;
	  *ecb_bim_cr2 = 0x00;   /* clear all bits in control register 2 */
	  *ecb_bim_vr2 = 0x0f;   /* set to uninitialized interrupt vector */
	  *ecb_bim_cr3 = 0x00;   /* clear all bits in control register 3 */
	  *ecb_bim_vr3 = 0x0f;   /* set to uninitialized interrupt vector */
	  shitcr0 = *ecb_bim_cr0;
	  shitvr0 = *ecb_bim_vr0;
	  shitcr1 = *ecb_bim_cr1;
	  shitvr1 = *ecb_bim_vr1;
	  if((shitcr0 == (0xd8|ECB_CMPLT_IRQ)) && (shitvr0 == ECB_CMPLT_VEC) && (shitcr1 == (0xd8|ECB_ERROR_IRQ)) && (shitvr1 == ECB_ERROR_VEC))
	      break;
      }

printf("ecbIntInit: after %d writes, reading the BIM:\n",bimwrcount);
printf("    cr0 = 0x%2x, vr0 = 0x%2x\n",*ecb_bim_cr0,*ecb_bim_vr0);
printf("    cr1 = 0x%2x, vr1 = 0x%2x\n",*ecb_bim_cr1,*ecb_bim_vr1);
printf("    cr2 = 0x%2x, vr2 = 0x%2x\n",*ecb_bim_cr2,*ecb_bim_vr2);
printf("    cr3 = 0x%2x, vr3 = 0x%2x\n",*ecb_bim_cr3,*ecb_bim_vr3);

shitcr0 = *ecb_bim_cr0;
shitvr0 = *ecb_bim_vr0;
shitcr1 = *ecb_bim_cr1;
shitvr1 = *ecb_bim_vr1;

if (shitcr0 != (0xd8 | ECB_CMPLT_IRQ))
  {
      printf("ecbIntInit:  BIM CONTROL REGISTER 0 DOES NOT HOLD ");
      printf("CORRECT VALUE!\n");
      return(1);
  }
if (shitvr0 != ECB_CMPLT_VEC)
  {
      printf("ecbIntInit:  BIM VECTOR REGISTER 0 DOES NOT ");
      printf("HOLD CORRECT VALUE!\n");
      return(1);
  }
if (shitcr1 != (0xd8 | ECB_ERROR_IRQ))
  {
      printf("ecbIntInit:  BIM CONTROL REGISTER 1 DOES NOT HOLD ");
      printf("CORRECT VALUE!\n");
      return(2);
  }
if (shitvr1 != ECB_ERROR_VEC)
  {
      printf("ecbIntInit:  BIM VECTOR REGISTER 1 DOES NOT ");
      printf("HOLD CORRECT VALUE!\n");
      return(2);
  }
    printf("ecbIntInit: Creating semaphore\n");
    /* Create the command-not-pending semaphore */
    ecb_cmd_not_pending = semBCreate(SEM_Q_FIFO,SEM_EMPTY);
    if (ecb_cmd_not_pending == NULL)
      {
	  printf("ecbIntInit:  semBCreate returned NULL for ecb_cmd_not_pending.\n");
	  printf("ecbIntInit:  returning without creating or Give-ing semaphore.\n");
	  return(3);
      }

    /* Give the command-not-pending semaphore */
    printf("ecbIntInit: Sending semaphore\n");
    if(semGive(ecb_cmd_not_pending) == ERROR)
      {
	  printf("ecbIntInit:  semGive(ecb_cmd_not_pending) returned ERROR,\n");
	  printf("ecbIntInit:  indicating an invalid semaphore ID.\n");
	  printf("ecbIntInit:  Returning without Give-ing the semaphore.\n");
	  return(3);
      }

    /* Initialize ecb variables in the global rpc status structure */
    currStatus->ecbgen = currStatus->slvdead = currStatus->onBoverT = currStatus->offBoverT = currStatus->forDDSool = currStatus->aftDDSool = 0x00;

return(0);
}

