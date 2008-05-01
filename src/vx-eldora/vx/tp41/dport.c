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
 * Revision 2.0  1992/11/02  20:42:08  thor
 * First offical ELDORA release!
 *
 * Revision 1.6  1992/09/28  21:03:49  thor
 * Added missing shift to extended mode lut write.
 *
 * Revision 1.5  1992/08/27  13:31:21  thor
 * Major changes. Fixed code to correctly set up STD addressing in LUT.
 * Added support for mailboxes.
 *
 * Revision 1.4  1992/08/24  18:10:00  thor
 * Added support to myDualPort to allow mailboxes.
 *
 * Revision 1.3  1992/03/04  17:10:43  thor
 * Added new code to dual port in standard i/o space.
 *
 * Revision 1.2  1992/03/03  18:02:06  thor
 * Changed local address check to allow dual porting of all of memory.
 *
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

STATUS dport(FAST char *localAddr, FAST void *vmeAddr, FAST int numSegs)
{
    FAST int i;

    for (i = 0; i < numSegs; i++)
      {
	  if (myDualPort(localAddr,vmeAddr,VME_EXT,0) == ERROR)
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

STATUS std_dport(FAST char *localAddr, FAST void *vmeAddr, FAST int numSegs)
{
    FAST int i;

    for (i = 0; i < numSegs; i++)
      {
	  if (myDualPort(localAddr,vmeAddr,VME_STD,0) == ERROR)
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

STATUS myDualPort (FAST char *localAdrs, FAST char *busAdrs,
		   FAST int adrSpace, FAST int mbox)
{
    FAST int accessMode = VME_READ_WRITE; /* access mode */
    FAST int lutEntryOff;	/* offset to the LUT */
    FAST UINT8 space = VME_ASSIGN_EXT;
    FAST UINT32 *lutEntryAdr;	/* Entry address in the LUT. Note that */
				/* it is a long because the TP$1 */
				/* decodes data bits 31:24 for the */
				/* SRAM. */
    FAST UINT8 *cio1Ctl = TP41_CIO_1_CNTRL_ADRS;
    FAST UINT8 *cio0Ctl = TP41_CIO_0_CNTRL_ADRS;
    FAST UINT8 temp; 
    FAST UINT8 segId;		/* segment ID */
    
    /* Check for valid DRAM address. You may have to change this to */
    /* suit your configuration. We lie to VxWorks and tell about only */
    /* the first 2 Mb of memory so we can use the other 2Mb for */
    /* private allocation. */
    
    if ((int)localAdrs > ((int)sysMemTop() + (4 * SEGMENT_SIZE)))
      {
	  return (ERROR);
      }
    
    if (mbox)
      accessMode = VME_RW_MBOX;

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

    /* Segment ID 7 is assigned to the top segment of DRAM. */
    
    segId = ((UINT)localAdrs - 0x40000000) >> 19;
    
    *cio1Ctl = ZCIO_PCD;
    temp = *cio1Ctl;
    
    *cio1Ctl = ZCIO_PCD;
    *cio1Ctl = temp | CIO1_C_VME_LUT_DISABLE;
    
    /* set address source to CPU */
    
    *cio0Ctl = ZCIO_PAD;
    temp = *cio0Ctl;
    
    *cio0Ctl = ZCIO_PAD;
    *cio0Ctl = temp & (~CIO0_A_ADDR_NONCPU_SRC);
    
    if (adrSpace == VME_STD)
      space = VME_ASSIGN_STD;

    lutEntryOff = (UINT) busAdrs >> 19;

    lutEntryAdr = (UINT32 *) ((int)VMELUT_BASE_ADRS + 
			      lutEntryOff * sizeof(UINT32));

    /* For std space one must fill in all 256 possible entries */
    /* corresponding to the upper 8 address bits, since they could be */
    /* floating at nay value. */
    if (adrSpace == VME_STD)
      {
	  FAST int i;
	  FAST int j = 256;
	  FAST char entry = accessMode | space | (unsigned char)segId;

	  /* 8192 / 256 = 32. */
	  for (i = 0; i < j; i++, lutEntryAdr += 32)
	    *lutEntryAdr = entry << 24;
      }
    else
      *lutEntryAdr =  (accessMode | space | (unsigned char)segId) << 24;

    /* set address source to VME bus */
	  
    *cio0Ctl = ZCIO_PAD;
    temp = *cio0Ctl;
    *cio0Ctl = ZCIO_PAD;
    *cio0Ctl = temp | CIO0_A_ADDR_NONCPU_SRC;
	  
    /* enable LUT output */
	  
    *cio1Ctl = ZCIO_PCD;
    *cio1Ctl = CIO1_C_CLEAR_PERR |      /* clear parity error */
      CIO1_C_NO_WATCHDOG_RESET; /* no watch dog reset */
	  
    *cio0Ctl = ZCIO_PAD;
    temp = *cio0Ctl;
    *cio0Ctl = ZCIO_PAD;

    if (adrSpace == VME_EXT)
      /* dual port on to extended space */
      *cio0Ctl = temp & (~CIO0_A_DETTACH_VME_EXT);
    else
      *cio0Ctl = temp & (~CIO0_A_DETTACH_VME_STD);
    return (OK);
}
