/*
 *	$Id$
 *
 *	Module:		 ecbErrorIsr
 *	Original Author: Shawn B. Copeland
 *      Copyright by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1992/09/17  23:41:50  shawn
 * Writes to rpc global status structure "currStatus" now.  Will set
 * the appropriate "slave dead" bit.
 *
 * Revision 1.2  1992/09/17  21:07:15  shawn
 * debugged; copy prior to adding writes to global hskp rpc status.
 *
 * Revision 1.1  1992/09/17  18:09:55  shawn
 * Initial revision
 *
 * description: ECB "Command Error" Interrupt Service Routine.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
/*	ecbErrorIsr()                                                        */
/*		"ECB Command Error" Interrupt Service Routine.            */
/*              basically will read status out of the OUT FIFO, load         */
/*              appropriate global status structures, and then Give the      */
/*              command-not-pending semaphore.                               */
/*      Return values:                                                       */
/*              void function, no values returned.                           */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>
#include "semLib.h"

#include "ecbFunc.h"     /* function prototypes for ecb______ */
#include "ecbMaster.h"   /* general #defines for ecb master offsets */
#include "ecbStat.h"     /* global structures for ecb status */
#include "ecbSem.h"      /* semaphore definitions for ecb master */
#include "ecbAdr.h"      /* Slave addresses on ECB bus */
#include "hskpInt.h"     /* interrupt vector definitions for hskp */
#define OK_RPC           /* makes definitions active in HskpStatus.h */
#include "HskpStatus.h"  /* global housekeeping status... rpc to cntl proc */

unsigned char errCheckOUT();    /* function to check OUT FIFO for empty */

static unsigned char status[256];  /* returned status info buffer */

void ecbErrorIsr()
{
    unsigned char *ecb_bim_cr1;  /* pointer to BIM control reg 1 */
    unsigned char *ecb_vme_ctl;  /* pointer to ECB master vme control reg */
    unsigned char *ecb_out_fifo; /* pointer to ECB out fifo */
    unsigned char ecbadr=0,numbytes=0,ecbcomID=0;
    unsigned char chksumback=0,chksum=0,bcount=0;

    ecb_bim_cr1   = (unsigned char *) (MASTERBASE + MASTERBIM + BIMCR1);
    ecb_vme_ctl   = (unsigned char *) (MASTERBASE + MASTERCTL);
    ecb_out_fifo  = (unsigned char *) (MASTERBASE + MASTEROUT);

    /* clear interrupt */
    *ecb_vme_ctl = 0x1d;         /* bring CLRINT1* line low, leaving HC11
                                    running (VMEHC11RST* = 1) */
    *ecb_vme_ctl = 0x1f;         /* bring CLRINT1* line high, leaving HC11
				    running (VMEHC11RST* = 1) */

logMsg("\necbErrorIsr:  LAST ECB COMMAND ISSUED ENDED IN ERROR!!!\n");

    /* do status retrieval */
    if (errCheckOUT())   /* check for OUT FIFO EMPTY */
      {
	  logMsg("ecbErrorIsr:  but OUT FIFO IS EMPTY!!!  No ecbadr or ComID\n              information is available.\n\n");
	  return;
      }

    ecbadr = *ecb_out_fifo;  /* get address of slave returning status */
    chksumback += ecbadr;
    switch(ecbadr)
      {
	case ECBRFFOR:
 	  logMsg("ecbErrorIsr:  ecbadr = %d.\n                         This corresponds to FORWARD RCVR/XCTR SLAVE.\n",ecbadr);
	  break;
	case ECBRFAFT:
 	  logMsg("ecbErrorIsr:  ecbadr = %d.\n                         This corresponds to AFT RCVR/XCTR SLAVE.\n",ecbadr);
	  break;
	case ECBIFFOR:
 	  logMsg("ecbErrorIsr:  ecbadr = %d.\n                         This corresponds to FORWARD IF PROCESSOR SLAVE.\n",ecbadr);
	  break;
	case ECBIFAFT:
 	  logMsg("ecbErrorIsr:  ecbadr = %d.\n                         This corresponds to AFT IF PROCESSOR SLAVE.\n",ecbadr);
	  break;
	case ECBATTEN:
 	  logMsg("ecbErrorIsr:  ecbadr = %d.\n                         This corresponds to ATTENUATOR CHASSIS SLAVE.\n",ecbadr);
	  break;
	default:
 	  logMsg("ecbErrorIsr:  ecbadr = %d.\n                         This corresponds to NO KNOWN ADDRESS!!!!\n",ecbadr);
	  break;
      }

    /* load global status structure to be rpc'd back to control processor */
    /* (Set appropriate slave-dead bit) */
    currStatus->slvdead = currStatus->slvdead | (0x01<<ecbadr);

    if (errCheckOUT())   /* check for OUT FIFO EMPTY */
      {
	  logMsg("ecbErrorIsr:  but OUT FIFO IS NOW EMPTY!!!  No numbytes or ComID\n              information is available.\n\n");
	  return;
      }
    numbytes = *ecb_out_fifo; /* get numbytes (of status) */
    chksumback += numbytes;

    if (errCheckOUT())   /* check for OUT FIFO EMPTY */
      {
	  logMsg("ecbErrorIsr:  but OUT FIFO IS NOW EMPTY!!!  No ComID\n              information is available.\n\n");
	  return;
      }
    ecbcomID = *ecb_out_fifo; /* get Command ID status is associated with */
    chksumback += ecbcomID;
    logMsg("ecbErrorIsr:  ComID of bad command is 0x%2x.\n",ecbcomID);

    /* load any returned status bytes into status buffer */
    for (bcount = 0; bcount<numbytes; bcount++)
      {
	  if (errCheckOUT()) return;  /* check for OUT FIFO EMPTY */
	  status[bcount] = *ecb_out_fifo;
	  chksumback += status[bcount];
#ifdef ECBDEBUG
	  logMsg(" bcount=%d, status[bcount] = %d\n.",bcount,status[bcount]);
#endif
      }

    if (errCheckOUT()) return;  /* check for OUT FIFO EMPTY */
    chksum = *ecb_out_fifo; /* get checksum byte */
    if (chksum != chksumback)
      {
	  logMsg("\necbErrorIsr: ERROR! RETURNED AND COMPUTED CHECKSUM DON'T AGREE.\necbErrorIsr: computed = %d, returned = %d\necbErrorIsr: ABORTING WITHOUT RE-GIVING ecb_cmd_not_pending SEMAPHORE,\necbErrorIsr OR INTERPRETING STATUS\n",chksumback,chksum);
	  return;
      }

    /* Based on ecbadr and comID, interpret status bytes in status buffer */
    switch(ecbadr)
      {
	  /* RCVR/XCTRS: */
	case ECBRFFOR:
	case ECBRFAFT:
	  switch(ecbcomID)
	    {
	      case 0x01:
	      case 0x02:
	      case 0x05:
	      case 0x06:
	      case 0x10:
		break;
	      default:
		logMsg("\necbErrorIsr: ERROR PROCESSING RETURNED STATUS FROM RCVR/XCTR SLAVE!!!\nUndefined COMID returned in OUT FIFO:  ecbadr = %d, comID = 0x%2x.\n",ecbadr,ecbcomID);
		break;
	    }
	  break;
	  
	  /* IF PROCESSORS: */
	case ECBIFFOR:
	case ECBIFAFT:
	  switch(ecbcomID)
	    {
	      case 0x01:
	      case 0x02:
	      case 0x05:
	      case 0x10:                     /* Set IF Filter command */
		break;
	      default:
		logMsg("\necbErrorIsr: ERROR PROCESSING RETURNED STATUS FROM IF PROCESSOR SLAVE!!!\nUndefined COMID returned in OUT FIFO:  ecbadr = %d, comID = 0x%2x.\n",ecbadr,ecbcomID);
		break;
	    }
	  break;
	  
	  /* ATTENUATOR CHASSIS: */
	case ECBATTEN:
	  switch(ecbcomID)
	    {
	      case 0x01:
	      case 0x02:
	      case 0x05:
	      case 0x10:
	      case 0x11:
	      case 0x12:
	      case 0x13:
	      case 0x14:
	      case 0x15:
		break;
	      default:
		logMsg("\nERROR PROCESSING RETURNED STATUS FROM ATTENUATOR CHASSIS SLAVE!!!\nUndefined COMID returned in OUT FIFO:  ecbadr = %d, comID = 0x%2x.\n",ecbadr,ecbcomID);
		break;
	    }
	  break;

	  /* UNKNOWN ecbadr */
	default:
	  logMsg("\necbErrorIsr: ERROR!!! ecbadr RETURNED IN ECB MASTER OUT FIFO DOES NOT\necbErrorIsr: CORRESPOND TO ANY VALID SLAVE ADDRESS. Re-Enabling interrupt\necbErrorIsr: and Give-ing ecb_cmd_not_pending semaphore anyway.\n");
	break;
      }

    /* Re-enable interrupt in ECB MASTER BIM */
    *ecb_bim_cr1 = (0xd8 | ECB_ERROR_IRQ);
                           /* set command error interrupt level, */
                           /* enable the interrupt and set auto-clear */
                           /* bit (meaning that the ISR must re-enable */
                           /* the interrupt) */
    semGive(ecb_cmd_not_pending); /* give the command-not-pending semaphore */;
#ifdef ECBDEBUG
    logMsg("\necbErrorIsr: int. cleared and reenabled, semaphore Given.\n");
#endif
}

unsigned char errCheckOUT()
{
    unsigned char *ecb_vme_stat; /* pointer to ECB master vme status reg */
    ecb_vme_stat  = (unsigned char *) (MASTERBASE + MASTERSTAT);

    if ( !(*ecb_vme_stat & 0x04) )   /* check for OUT FIFO EMPTY */
      {
	  logMsg("\necbErrorIsr: ERROR! OUT FIFO EMPTY.\necbErrorIsr: ABORTING WITHOUT READING/LOADING ECB STATUS, OR RE-GIVING\necbErrorIsr: ecb_cmd_not_pending SEMAPHORE.\n");
	  return(1);
      }
    else
      return(0);
}







