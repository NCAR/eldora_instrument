/*
 *	$Id$
 *
 *	Module:		 mbox.c
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:
 *        These routines allow one to create a memory region that is
 *  dual ported and has mailboxes. One must dual port 2 Mb of memory
 *  for the mailboxes to work! This could be done in scattered
 * chunks, but to us it makes more sense to do it units of 2 Mb.
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "tp41Lib.h"

static void enableMbox(void);

STATUS createMailbox(FAST void *localAddr, FAST void *vmeAddr)
{
    FAST int i;

    enableMbox();

    for (i = 0; i < 3; i++)	/* The first three sections do not get */
				/* mailboxes. */
      {
	  if (myDualPort(localAddr,vmeAddr,VME_EXT,0) == ERROR)
	    return(ERROR);
	  localAddr += SEGMENT_SIZE;
	  vmeAddr += SEGMENT_SIZE;
      }

    return(myDualPort(localAddr,vmeAddr,VME_EXT,1));
}

STATUS createStdMailbox(FAST void *localAddr, FAST void *vmeAddr)
{
    FAST int i;

    enableMbox();

    for (i = 0; i < 3; i++)
      {
	  if (myDualPort(localAddr,vmeAddr,VME_STD,0) == ERROR)
	    return(ERROR);
	  localAddr += SEGMENT_SIZE;
	  vmeAddr += SEGMENT_SIZE;
      }

    return(myDualPort(localAddr,vmeAddr,VME_STD,1));
}

STATUS connectMailbox(FAST VOIDFUNCPTR function, FAST int arg,
		      FAST int mailbox)
{
#ifndef MAILBOX_16
    if (mailbox >= 8)
      return(ERROR);
#else
    if (mailbox >= 16)
      return(ERROR);
#endif /* MAILBOX_16 */

    return(intConnect(((INT_VEC_MAILBOX0 + (2 * mailbox)) * 4),function,
		      arg));
}

static void enableMbox(void)
{
    *TP41_VME_MBOX_CLEAR = 0;	/* Clear all mailbox interrupts. */

    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PBDPP;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_DPP_NONINV;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PBDD;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_DD_IN;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PBMS;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PMS_PMS_PEV;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PBPT;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PT_ANY;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PBPM;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PM_ONE;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PBPP;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PP_ONE;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PBCS;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_CS_CLIPIUS;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PBCS;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_CS_CLIE;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PBCS;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_CS_SIE;
     
#ifdef MAILBOX_16
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PADPP;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_DPP_NONINV;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PADD;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_DD_IN;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PAMS;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PMS_PMS_PEV;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PAPT;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PT_ANY;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PAPM;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PM_ONE;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PAPP;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PP_ONE;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PACS;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_CS_CLIPIUS;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PACS;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_CS_CLIE;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_PACS;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_CS_SIE;
#endif /* MAILBOX_16 */
     
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_MCC;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_MCC_PBE;

#ifdef MAILBOX_16
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_MCC;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_MCC_PAE;
#endif /* MAILBOX_16 */

    *TP41_CIO_2_CNTRL_ADRS = ZCIO_MIC;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_MIC_MIE | ZCIO_MIC_PBVIS;

#ifdef MAILBOX_16
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_MIC;
    *TP41_CIO_2_CNTRL_ADRS = ZCIO_MIC_MIE | ZCIO_MIC_PAVIS;
#endif /* MAILBOX_16 */
} 
