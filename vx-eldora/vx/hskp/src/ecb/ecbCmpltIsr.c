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
 * Posterity check-in, just prior to implementing "bus status" commands.
 *
 * Revision 1.2  1992/06/19  00:39:39  shawn
 * Intermediate version, loads ecbLastCmd, works with ecbSetIF and ecbSetDDS
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
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>
#include "semLib.h"
#include "ecbMaster.h"   /* general #defines for ecb master offsets */
#include "ecbAdr.h"      /* Slave addresses on ECB bus */
#include "ecbErrBound.h" /* Various #defines for error bounds */
void procTsamr(unsigned char ecbadr,unsigned char numbytes); /* process temp sample status data */
void procDDSool(unsigned char ecbadr,unsigned char numbytes); /* process DDS lock status data */
void procBstat(unsigned char ecbadr,unsigned char numbytes); /* process Bus status data */

static unsigned char status[256];  /* returned status info buffer */
unsigned char cmdfail;             /* command failure flag */
    unsigned char ecbadr,numbytes,ecbcomID,chksumback=0,chksum;
{
    unsigned char *ecb_bim_cr0;  /* pointer to BIM control reg 0 */
    unsigned char *ecb_vme_ctl;  /* pointer to ECB master vme control reg */
    unsigned char *ecb_out_fifo; /* pointer to ECB out fifo */
    unsigned char chksumback=0,chksum=0,bcount=0;

    ecb_bim_cr0   = (unsigned char *) (MASTERBASE + MASTERBIM + BIMCR0);
    ecb_vme_ctl   = (unsigned char *) (MASTERBASE + MASTERCTL);
    ecb_out_fifo  = (unsigned char *) (MASTERBASE + MASTEROUT);
    cmdfail = 0;                 /* clear command fail flag */

    /* do status retrieval and filling of global status structures here */
                                    running (VMEHC11RST* = 1) */
    if (checkOUT()) 
      {
	  logMsg("\necbCmpltIsr: checkOUT returned nonzero!\n");
	  return;  /* check for OUT FIFO EMPTY */
      }
    *ecb_vme_ctl = 0x1f;         /* bring CLRINT0* line high, leaving HC11
				    running (VMEHC11RST* = 1) */

    /* do status retrieval */
    if (checkOUT()) return;  /* check for OUT FIFO EMPTY */

    ecbadr = *ecb_out_fifo;  /* get address of slave returning status */
    chksumback += ecbadr;

    if (checkOUT()) return;  /* check for OUT FIFO EMPTY */
    numbytes = *ecb_out_fifo; /* get numbytes (of status) */
    /* load "last command" global status structure */
    ecbLastCmd.ecbadr = ecbadr;
    ecbLastCmd.comID = ecbcomID;
	  chksumback += status[bcount];
#ifdef ECBDEBUG
	  logMsg(" bcount=%d, status[bcount] = %d\n.",bcount,status[bcount]);
#endif
      }
	  logMsg("\necbCmpltIsr: ERROR! RETURNED AND COMPUTED CHECKSUM DON'T AGREE.\n");
	  logMsg("ecbCmpltIsr: computed = %d, returned = %d\n",chksumback,chksum);
	  logMsg("ecbCmpltIsr: ABORTING WITHOUT RE-GIVING ecb_cmd_not_pending SEMAPHORE.\n");
    if (checkOUT()) return;  /* check for OUT FIFO EMPTY */
    chksum = *ecb_out_fifo; /* get checksum byte */
    if (chksum != chksumback)
{
if (numbytes == 4)  /* check for correct number of status bytes */
  {   /* load "Temperature status, scaled int" global status structure */
      ecbTempi.ecbadr = ecbadr;                      /* load ecb address */
      ecbTempi.onbtemp = status[0]*256 + status[1];  /* load onboard temp */
      ecbTempi.offbtemp = status[2]*256 + status[3]; /* load offboard temp */

      if (status[0]*2.56 > ECBTEMPHI)                /* set/clear overtemp */
      else
	currStatus->onBoverT = currStatus->onBoverT & ~(0x01<<ecbadr);
      if (status[2]*2.56 > ECBTEMPHI)                /* set/clear overtemp */
	currStatus->offBoverT = currStatus->offBoverT | (0x01<<ecbadr);
      else
	currStatus->offBoverT = currStatus->offBoverT & ~(0x01<<ecbadr);
  }
else 
  cmdfail = 1;  /* flag command failure */
	  logMsg("\necbCmpltIsr: ERROR! OUT FIFO EMPTY AFTER COMMAND COMPLETION.\n");
	  logMsg("ecbCmpltIsr: ABORTING WITHOUT READING/LOADING ECB STATUS,\n");
	  logMsg("ecbCmpltIsr: OR RE-GIVING ecb_cmd_not_pending SEMAPHORE.\n");

void procTsamr(unsigned char ecbadr,unsigned char numbytes)
{
if (numbytes == 2)  /* check for correct number of status bytes */
  {   /* load "Temperature status, raw counts" global status structure */
