/*
 *	$Id$
 *
 *	Module:		 dport.c
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/01/28  15:44:43  thor
 * Initial revision
 *
 *
 * description:
 *        These functions allow better dual porting then the WRS
 * version. They do not assume that one wants to start at the top of
 * memory or install mailboxes.
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "tp41Lib.h"

static STATUS myDualPort(char *,char *);

STATUS dport(FAST char *localAddr, FAST void *vmeAddr, FAST int numSegs)
{
    FAST int i;

    for (i = 0; i < numSegs; i++)
      {
	  if (myDualPort(localAddr,vmeAddr) == ERROR)
	    {
		printf("Dual portting failed.\n");
		return(ERROR);
	    }
	  printf("Local 0x%x, VME 0x%x.\n",localAddr,vmeAddr);
	  localAddr += SEGMENT_SIZE;
	  vmeAddr += SEGMENT_SIZE;
      }
    return(OK);
}

static STATUS myDualPort (FAST char *localAdrs, FAST char *busAdrs)
{
    FAST int adrSpace = VME_AM_EXT_SUP_DATA; /* address space */
    FAST int accessMode = VME_READ_WRITE; /* access mode */
    FAST int lutEntryOff;	/* offset to the LUT */
    FAST UINT32 *lutEntryAdr;	/* entry address in the LUT */
    FAST UINT8 *cio1Ctl = TP41_CIO_1_CNTRL_ADRS;
    FAST UINT8 temp; 
    FAST UINT8 segId;		/* segment ID */
    
    /* check for valid DRAM address */
    
    if ((int)localAdrs > ((int)sysMemTop() + (4 * SEGMENT_SIZE)))
      {
	  return (ERROR);
      }
    
    /* check for valid access mode */

    accessMode &= VME_ACCESS_MASK;

    switch(accessMode)
      {
	case VME_READ_ONLY:
	case VME_NO_RESPOND:
	case VME_READ_WRITE:
	case VME_RW_MBOX:
	  break;

	  default:
	  return(ERROR);
	  break;
      }
    
    /* segment ID 0 is assigned to the top segment of DRAM */
    
    segId = ((UINT)localAdrs - 0x40000000) >> 19;	 /* XXX */
    
    
    *TP41_CIO_1_CNTRL_ADRS = ZCIO_PCD;
    temp = *TP41_CIO_1_CNTRL_ADRS;
    
    *TP41_CIO_1_CNTRL_ADRS = ZCIO_PCD;
    *TP41_CIO_1_CNTRL_ADRS = temp | CIO1_C_VME_LUT_DISABLE;
    
    /* set address source to CPU */
    
    *TP41_CIO_0_CNTRL_ADRS = ZCIO_PAD;
    temp = *TP41_CIO_0_CNTRL_ADRS;
    
    *TP41_CIO_0_CNTRL_ADRS = ZCIO_PAD;
    *TP41_CIO_0_CNTRL_ADRS = temp & (~CIO0_A_ADDR_NONCPU_SRC);
    
    /* XXX should be possible to support both STD & EXT simultaneously */
    
    if (VME_AM_IS_EXT (adrSpace))
      {
	  lutEntryOff = (UINT) busAdrs >> 19;
	  lutEntryAdr = (UINT32 *) ((int)VMELUT_BASE_ADRS + 
				    lutEntryOff * sizeof(UINT32));
	  
	  /* set the look up table */
	  
	  *lutEntryAdr = (accessMode | /* access mode */
			  VME_ASSIGN_EXT | /* assign to ext */
			  (UINT8)segId) << 24; /* set segment id */

	  
	  /* set address source to VME bus */
	  
	  *TP41_CIO_0_CNTRL_ADRS = ZCIO_PAD;
	  temp = *TP41_CIO_0_CNTRL_ADRS;
	  *TP41_CIO_0_CNTRL_ADRS = ZCIO_PAD;
	  *TP41_CIO_0_CNTRL_ADRS = temp | CIO0_A_ADDR_NONCPU_SRC;
	  
	  /* enable LUT output */
	  
	  *cio1Ctl = ZCIO_PCD;
	  *cio1Ctl = CIO1_C_CLEAR_PERR |      /* clear parity error */
	    CIO1_C_NO_WATCHDOG_RESET; /* no watch dog reset */
	  
	  /* dual port on to extended space */
	  
	  *TP41_CIO_0_CNTRL_ADRS = ZCIO_PAD;
	  temp = *TP41_CIO_0_CNTRL_ADRS;
	  *TP41_CIO_0_CNTRL_ADRS = ZCIO_PAD;
	  *TP41_CIO_0_CNTRL_ADRS = temp & (~CIO0_A_DETTACH_VME_EXT);
      }
    else
      {
	  return (ERROR);
      }
    
    return (OK);
}
