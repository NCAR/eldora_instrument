/*
 *	$Id$
 *
 *	Module:		 ecbCmpltIsr
 *	Original Author: Shawn B. Copeland
 *      Copyright by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1992/06/19  00:39:39  shawn
 * description:
 * Revision 1.1  1992/06/16  22:25:18  shawn
 * Initial revision
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

 *
 * description: ECB "Command Complete" Interrupt Service Routine.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*              command-not-pending semaphore.
/*	ecbCmpltIsr()                                                        */
/*		"ECB Command Complete" Interrupt Service Routine.            */
/*              basically will read status out of the OUT FIFO, load         */
/*              appropriate global status structures, and then Give the      */
/*              command-not-pending semaphore.                               */
/*      Return values:                                                       */
/*              void function, no values returned.                           */
#include "intLib.h"
#include "taskLib.h"
/*****************************************************************************/
#include <vxWorks.h>
#include <stdioLib.h>
#include "semLib.h"
#include "ecbMaster.h"   /* general #defines for ecb master offsets */
#include "ecbAdr.h"      /* Slave addresses on ECB bus */
void procDDSool(unsigned char ecbadr,unsigned char numbytes); /* process DDS lock status data */
void procBstat(unsigned char ecbadr,unsigned char numbytes); /* process Bus status data */
    unsigned char *ecb_vme_ctl;  /* pointer to ecb master vme control reg */


    ecb_vme_ctl = (unsigned char *) (MASTERBASE + MASTERCTL);
    ecb_bim_cr0 = (unsigned char *) (MASTERBASE + MASTERBIM + BIMCR0);

    logMsg("\necbCmpltIsr: ISR entered.\n");
    unsigned char chksumback=0,chksum=0,bcount=0;

    ecb_bim_cr0   = (unsigned char *) (MASTERBASE + MASTERBIM + BIMCR0);
    ecb_vme_ctl   = (unsigned char *) (MASTERBASE + MASTERCTL);
    ecb_out_fifo  = (unsigned char *) (MASTERBASE + MASTEROUT);
    cmdfail = 0;                 /* clear command fail flag */
    logMsg("\necbCmpltIsr: Command Complete interrupt (to BIM) cleared.\n");

    /* do status retrieval and filling of global status structures here */
                                    running (VMEHC11RST* = 1) */
    semGive(ecb_cmd_not_pending); /* give the command-not-pending semaphore */;
    logMsg("\necbCmpltIsr: Command-not-pending semaphore given.\n");
    if (chksum != chksumback)
    *ecb_bim_cr0 = 0xd9;   /* set command complete interrupt to IRQ1* */
  {   /* load "Temperature status, scaled int" global status structure */
      ecbTempi.ecbadr = ecbadr;                      /* load ecb address */
      ecbTempi.onbtemp = status[0]*256 + status[1];  /* load onboard temp */
    logMsg("\necbCmpltIsr: Command Complete interrupt reenabled in BIM.\n");
  }
  {   /* load "Temperature status, raw counts" global status structure */
