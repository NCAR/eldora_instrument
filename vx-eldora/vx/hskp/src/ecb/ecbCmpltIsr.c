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
 * stuff ecbLastCmd if numbytes is wrong.
 *
 * Revision 1.3  1992/06/25  23:22:22  shawn
 * Posterity check-in, just prior to implementing "bus status" commands.
 *
 * Revision 1.2  1992/06/19  00:39:39  shawn
 * Intermediate version, loads ecbLastCmd, works with ecbSetIF and ecbSetDDS
 *
 * Revision 1.1  1992/06/16  22:25:18  shawn
 * Initial revision
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
 *
 * description: ECB "Command Complete" Interrupt Service Routine.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
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
void procTsami(unsigned char ecbadr); /* process temp sample status data */
void procTsamr(unsigned char ecbadr); /* process temp sample status data */
void procBstat(unsigned char ecbadr); /* process Bus status data */

unsigned char checkOUT();    /* function to check OUT FIFO for empty */
void procTsamr(unsigned char ecbadr,unsigned char numbytes); /* process temp sample status data */
void procDDSool(unsigned char ecbadr,unsigned char numbytes); /* process DDS lock status data */
void procBstat(unsigned char ecbadr,unsigned char numbytes); /* process Bus status data */

static unsigned char status[256];  /* returned status info buffer */
unsigned char cmdfail;             /* command failure flag */
    unsigned char ecbadr,numbytes,ecbcomID,chksumback=0,chksum=0,bcount=0;
{
    unsigned char *ecb_bim_cr0;  /* pointer to BIM control reg 0 */
    unsigned char *ecb_vme_ctl;  /* pointer to ECB master vme control reg */
    unsigned char *ecb_out_fifo; /* pointer to ECB out fifo */
    unsigned char chksumback=0,chksum=0,bcount=0;

    ecb_bim_cr0   = (unsigned char *) (MASTERBASE + MASTERBIM + BIMCR0);
    ecb_vme_ctl   = (unsigned char *) (MASTERBASE + MASTERCTL);
    ecb_out_fifo  = (unsigned char *) (MASTERBASE + MASTEROUT);
    cmdfail = 0;                 /* clear command fail flag */

    /* clear interrupt */
    *ecb_vme_ctl = 0x1e;         /* bring CLRINT0* line low, leaving HC11
                                    running (VMEHC11RST* = 1) */
    *ecb_vme_ctl = 0x1f;         /* bring CLRINT0* line high, leaving HC11
				    running (VMEHC11RST* = 1) */

    /* do status retrieval */
    if (checkOUT()) return;  /* check for OUT FIFO EMPTY */

    ecbadr = *ecb_out_fifo;  /* get address of slave returning status */
    chksumback += ecbadr;

    if (checkOUT()) return;  /* check for OUT FIFO EMPTY */
    numbytes = *ecb_out_fifo; /* get numbytes (of status) */
logMsg("ecbadr, numbytes, ecbcomID ==> 0x%x,%d,0x%x\n",ecbadr,numbytes,ecbcomID);
    chksumback += ecbcomID;

    /* load global status structure to be rpc'd back to control processor */
    /* (Reset appropriate slave-dead bit) */

    /* load any returned status bytes into status buffer */
      {
	  status[bcount] = *ecb_out_fifo;
	  chksumback += status[bcount];
#ifdef ECBDEBUG
	  logMsg(" bcount=%d, status[bcount] = %d\n.",bcount,status[bcount]);
#endif
      }

    if (checkOUT()) return;  /* check for OUT FIFO EMPTY */
    chksum = *ecb_out_fifo; /* get checksum byte */
    if (chksum != chksumback)
    /* load "last command" global status structure */
    ecbLastCmd.ecbadr = ecbadr;
    ecbLastCmd.comID = ecbcomID;

      {
	  logMsg("\necbCmpltIsr: ERROR! RETURNED AND COMPUTED CHECKSUM DON'T AGREE.\necbCmpltIsr: computed = %d, returned = %d\necbCmpltIsr: ABORTING WITHOUT RE-GIVING ecb_cmd_not_pending SEMAPHORE,\necbCmpltIsr OR INTERPRETING STATUS\n",chksumback,chksum);
	  return;
      }

    /* Based on ecbadr and comID, interpret status bytes in status buffer */
    switch(ecbadr)
      {
	  /* RCVR/XCTRS: */
		logMsg("\nProcessing returned status from RCVR/XCTR slave\ncomID=0x01 (Send Temp Sample, scaled int)\n");
		procTsami(ecbadr);  /* process Temp sample status */
	case ECBRFAFT:
	  switch(ecbcomID)
		logMsg("\nProcessing returned status from RCVR/XCTR slave\ncomID=0x02 (Send Temp Sample, raw counts)\n");
		procTsamr(ecbadr);  /* process Temp sample status */
	      case 0x01:
		procTsami(ecbadr,numbytes);  /* process Temp sample status */
		logMsg("\nProcessing returned status from RCVR/XCTR slave\ncomID=0x05 (Send Bus Status)\n");
		procBstat(ecbadr);  /* process Bus status info */
	      case 0x02:
		procTsamr(ecbadr,numbytes);  /* process Temp sample status */
	      case 0x05:
		break;
	      case 0x06:
		logMsg("\nProcessing returned status from RCVR/XCTR slave\ncomID=0x10 (Set generated frequency for a DDS Unit)\n");
		procDDSool(ecbadr,numbytes); /* process out-of-lock status */
		break;
	      case 0x10:
		break;
	      default:
		logMsg("\nERROR PROCESSING RETURNED STATUS FROM RCVR/XCTR SLAVE!!!\nUndefined COMID returned in OUT FIFO:  ecbadr = %d, comID = 0x%2x.\n",ecbadr,ecbcomID);
		break;
	    }
	  break;
	  
	  /* IF PROCESSORS: */
	case ECBIFFOR:
	case ECBIFAFT:
		logMsg("\nProcessing returned status from IF PROCESSOR slave\ncomID=0x01 (Send Temp Sample, scaled int)\n");
		procTsami(ecbadr);  /* process Temp sample status */
	    {
	      case 0x01:
		logMsg("\nProcessing returned status from IF PROCESSOR slave\ncomID=0x02 (Send Temp Sample, raw counts)\n");
		procTsamr(ecbadr);  /* process Temp sample status */
		break;
	      case 0x02:
		logMsg("\nProcessing returned status from IF PROCESSOR slave\ncomID=0x05 (Send Bus Status)\n");
		procBstat(ecbadr);  /* process Bus status info */
		break;
	      case 0x10:
		logMsg("\nProcessing returned status from IF PROCESSOR slave\ncomID=0x10 (Set filter number for an IF processor unit)\n");
		procBstat(ecbadr,numbytes);  /* process Bus status info */
		break;
	      case 0x10:                     /* Set IF Filter command */
		break;
	      default:
		logMsg("\nERROR PROCESSING RETURNED STATUS FROM IF PROCESSOR SLAVE!!!\nUndefined COMID returned in OUT FIFO:  ecbadr = %d, comID = 0x%2x.\n",ecbadr,ecbcomID);
		break;
	    }
	  break;
	  
	  /* ATTENUATOR CHASSIS: */
	case ECBATTEN:
	    {
		procTsami(ecbadr);  /* process Temp sample status */	
		break;
	      case 0x02:
		break;
		procTsamr(ecbadr);  /* process Temp sample status */
		break;
	      case 0x10:
	      case 0x11:
		procBstat(ecbadr);  /* process Bus status info */
		break;
	      case 0x13:
	      case 0x14:
	      case 0x15:
		break;
		logMsg("\nERROR PROCESSING RETURNED STATUS FROM ATTENUATOR CHASSIS SLAVE!!!\nUndefined COMID returned in OUT FIFO:  ecbadr = %d, comID = 0x%2x.\n",ecbadr,ecbcomID);
	    }
	  break;
	  /* UNKNOWN ecbadr */
	  logMsg("\necbCmpltIsr: ERROR!!! ecbadr RETURNED IN ECB MASTER OUT FIFO DOES NOT\necbCmpltIsr: CORRESPOND TO ANY VALID SLAVE ADDRESS. Re-Enabling interrupt\necbCmpltIsr: and Give-ing ecb_cmd_not_pending semaphore anyway.\n");
	break;

      { /* load "last command" global status structure */
	  ecbLastCmd.ecbadr = ecbadr;
      }
    /* Re-enable interrupt in ECB MASTER BIM */
    *ecb_bim_cr0 = (0xd8 | ECB_CMPLT_IRQ);
                           /* enable the interrupt and set auto-clear */
                           /* the interrupt) */
    semGive(ecb_cmd_not_pending); /* give the command-not-pending semaphore */;
}

unsigned char checkOUT()
{
    unsigned char *ecb_vme_stat; /* pointer to ECB master vme status reg */
    ecb_vme_stat  = (unsigned char *) (MASTERBASE + MASTERSTAT);

    if ( !(*ecb_vme_stat & 0x04) )   /* check for OUT FIFO EMPTY */
      {
	  logMsg("\necbCmpltIsr: ERROR! OUT FIFO EMPTY AFTER COMMAND COMPLETION.\necbCmpltIsr: ABORTING WITHOUT READING/LOADING ECB STATUS, OR RE-GIVING\necbCmpltIsr: ecb_cmd_not_pending SEMAPHORE.\n");
	  return(1);
void procTsami(unsigned char ecbadr,unsigned char numbytes)
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
}

void procTsamr(unsigned char ecbadr,unsigned char numbytes)
{
if (numbytes == 2)  /* check for correct number of status bytes */
  {   /* load "Temperature status, raw counts" global status structure */
      ecbTempr.ecbadr = ecbadr;      /* load ecb address */
      ecbTempr.onbtemp = status[0];  /* load onboard temperature */
void procTsami(unsigned char ecbadr)
      ecbTempr.newdata = 1;          /* set newdata flag */
/* load "Temperature status, scaled int" global status structure */
ecbTempi.ecbadr = ecbadr;                      /* load ecb address */
ecbTempi.onbtemp = status[0]*256 + status[1];  /* load onboard temperature */
ecbTempi.offbtemp = status[2]*256 + status[3]; /* load offboard temperature */
ecbTempi.newdata = 1;                          /* set newdata flag */
	  logMsg("\necbCmpltIsr: WARNING! ADDRESS OF SLAVE RETURNING DDS OOL STATUS NOT RCVR/XCTR.\necbCmpltIsr: RETURNING WITHOUT LOADING RPC STATUS.\n");
  }
void procTsamr(unsigned char ecbadr)
  cmdfail = 1;  /* flag command failure */
/* load "Temperature status, raw counts" global status structure */
ecbTempr.ecbadr = ecbadr;      /* load ecb address */
ecbTempr.onbtemp = status[0];  /* load onboard temperature */
ecbTempr.offbtemp = status[1];  /* load offboard temperature */
ecbTempr.newdata = 1;          /* set newdata flag */
      ecbSlvStat.eoserrs   = status[4]*256+status[5];   /* load eos  errors */
      ecbSlvStat.feederrs  = status[6]*256+status[7];   /* load feed errors */
void procBstat(unsigned char ecbadr)
      ecbSlvStat.emptyerrs = status[10]*256+status[11]; /* load sync errors */
logMsg("\nprocBstat entered.\n");
