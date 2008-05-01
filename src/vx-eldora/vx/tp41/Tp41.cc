//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          Tp41.cc
//      Original Author: Rich Neitzel
//      Copywrited by the National Center for Atmospheric Research
//      Date:            $Date$
//
// revision history
// ----------------
// $Log$
//
//
// description:
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#pragma implementation

#include "Tp41.hh"
#include "vme.h"
#include "tp41.h"
#include "intLib.h"

#define VME_EXT 0
#define VME_STD 1

void Tp41::setcio(FAST int select, FAST char *port, FAST unsigned char mask)
{
    FAST unsigned char *cio;
    FAST unsigned char tmp;

    if (select == 0)
      cio = (unsigned char *)Tp41::cio0;
    else if (select == 1)
      cio = (unsigned char *)Tp41::cio1;
    else if (select == 2)
      cio = (unsigned char *)Tp41::cio2;
    else if (select == 3)
      cio = (unsigned char *)Tp41::cio3;

    if (*port == 'A' || *port == 'a')
      cio += Tp41::a_off;
    else if (*port == 'B' || *port == 'b')
      cio += Tp41::b_off;
    else if (*port == 'C' || *port == 'c')
      cio += Tp41::c_off;

    tmp = *cio;

    tmp ^= mask;

    *cio = tmp;
}

STATUS Tp41::wcio(FAST int select, FAST char *port, FAST unsigned char value)
{
    FAST unsigned char *cio;

    if (select == 0)
      cio = (unsigned char *)Tp41::cio0;
    else if (select == 1)
      cio = (unsigned char *)Tp41::cio1;
    else if (select == 2)
      cio = (unsigned char *)Tp41::cio2;
    else if (select == 3)
      cio = (unsigned char *)Tp41::cio3;
    else
      return(ERROR);

    if (*port == 'A' || *port == 'a')
      cio += Tp41::a_off;
    else if (*port == 'B' || *port == 'b')
      cio += Tp41::b_off;
    else if (*port == 'C' || *port == 'c')
      cio += Tp41::c_off;
    else
      return(ERROR);

    *cio = value;

    return(OK);
}

int Tp41::rcio(FAST int select, FAST char *port)
{
    FAST unsigned char *cio;

    if (select == 0)
      cio = (unsigned char *)Tp41::cio0;
    else if (select == 1)
      cio = (unsigned char *)Tp41::cio1;
    else if (select == 2)
      cio = (unsigned char *)Tp41::cio2;
    else if (select == 3)
      cio = (unsigned char *)Tp41::cio3;
    else
      return(ERROR);


    if (*port == 'A' || *port == 'a')
      cio += Tp41::a_off;
    else if (*port == 'B' || *port == 'b')
      cio += Tp41::b_off;
    else if (*port == 'C' || *port == 'c')
      cio += Tp41::c_off;
    else
      return(ERROR);

    return((int)(*cio & 0xff));
}

STATUS Tp41::mailboxConnect(FAST FUNCPTR routine, FAST int arg,
                            FAST int mailbox)
{
#ifndef MAILBOX_16
    if (mailbox >= 8)
      return(ERROR);
#else
    if (mailbox >= 16)
      return(ERROR);
#endif /* MAILBOX_16 */

    return(intConnect((VOIDFUNCPTR *)((INT_VEC_MAILBOX0 + (2 * mailbox)) * 4),
                      (VOIDFUNCPTR)routine,arg));
}

STATUS Tp41::mailboxEnable(char *mailboxAdrs, void *vmeAdrs)
{
    FAST int i;

    enableMbox();

    for (i = 0; i < 3; i++)     /* The first three sections do not get */
                                /* mailboxes. */
      {
          if (myDualPort(mailboxAdrs,vmeAdrs,VME_EXT,0) == ERROR)
            return(ERROR);
          mailboxAdrs += SEGMENT_SIZE;
          vmeAdrs += SEGMENT_SIZE;
      }

    return(myDualPort(mailboxAdrs,vmeAdrs,VME_EXT,1));
}

STATUS Tp41::stdMailboxEnable(FAST char *localAddr, FAST void *vmeAddr)
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

void Tp41::enableMbox(void)
{
    *TP41_VME_MBOX_CLEAR = 0;   /* Clear all mailbox interrupts. */

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

STATUS Tp41::dualPort(FAST void *localAddr, FAST void *vmeAddr,
                      FAST int numSegs)
{
    FAST int i;

    for (i = 0; i < numSegs; i++)
      {
          if (myDualPort(localAddr,vmeAddr,VME_EXT,0) == ERROR)
            return(ERROR);

          localAddr += SEGMENT_SIZE;
          vmeAddr += SEGMENT_SIZE;
      }
    return(OK);
}

STATUS Tp41::dualPortStd(FAST void *localAddr, FAST void *vmeAddr,
                         FAST int numSegs)
{
    FAST int i;

    for (i = 0; i < numSegs; i++)
      {
          if (myDualPort(localAddr,vmeAddr,VME_STD,0) == ERROR)
            return(ERROR);

          localAddr += SEGMENT_SIZE;
          vmeAddr += SEGMENT_SIZE;
      }
    return(OK);
}

STATUS Tp41::myDualPort(FAST void *localAdrs, FAST void *busAdrs,
                        FAST int adrSpace, FAST int mbox)
{
    FAST int accessMode = VME_READ_WRITE; // access mode
    FAST int lutEntryOff;       // offset to the LUT
    FAST UINT8 space = VME_ASSIGN_EXT;
    FAST UINT32 *lutEntryAdr;   // Entry address in the LUT. Note that
                                // it is a long because the TP$1
                                // decodes data bits 31:24 for the
                                // SRAM.
    FAST UINT8 *cio1Ctl = TP41_CIO_1_CNTRL_ADRS;
    FAST UINT8 *cio0Ctl = TP41_CIO_0_CNTRL_ADRS;
    FAST UINT8 temp; 
    FAST UINT8 segId;           // segment ID
    
    // Check for valid DRAM address. You may have to change this to
    // suit your configuration. We lie to VxWorks and tell about only
    // the first 2 Mb of memory so we can use the other 2Mb for
    // private allocation.
    
    if ((int)localAdrs > ((int)sysMemTop() + (4 * SEGMENT_SIZE)))
      {
          return (ERROR);
      }
    
    if (mbox)
      accessMode = VME_RW_MBOX;

    // check for valid access mode

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

    // Segment ID 7 is assigned to the top segment of DRAM.
    
    segId = ((UINT)localAdrs - 0x40000000) >> 19;
    
    *cio1Ctl = ZCIO_PCD;
    temp = *cio1Ctl;
    
    *cio1Ctl = ZCIO_PCD;
    *cio1Ctl = temp | CIO1_C_VME_LUT_DISABLE;
    
    // set address source to CPU
    
    *cio0Ctl = ZCIO_PAD;
    temp = *cio0Ctl;
    
    *cio0Ctl = ZCIO_PAD;
    *cio0Ctl = temp & (~CIO0_A_ADDR_NONCPU_SRC);
    
    if (adrSpace == VME_STD)
      space = VME_ASSIGN_STD;

    lutEntryOff = (UINT) busAdrs >> 19;

    lutEntryAdr = (UINT32 *) ((int)VMELUT_BASE_ADRS + 
                              lutEntryOff * sizeof(UINT32));

    // For std space one must fill in all 256 possible entries
    // corresponding to the upper 8 address bits, since they could be
    // floating at nay value.
    if (adrSpace == VME_STD)
      {
          FAST int i;
          FAST int j = 256;
          FAST char entry = accessMode | space | (unsigned char)segId;

          // 8192 / 256 = 32.
          for (i = 0; i < j; i++, lutEntryAdr += 32)
            *lutEntryAdr = entry << 24;
      }
    else
      *lutEntryAdr =  (accessMode | space | (unsigned char)segId) << 24;

    // set address source to VME bus
          
    *cio0Ctl = ZCIO_PAD;
    temp = *cio0Ctl;
    *cio0Ctl = ZCIO_PAD;
    *cio0Ctl = temp | CIO0_A_ADDR_NONCPU_SRC;
          
    // enable LUT output
          
    *cio1Ctl = ZCIO_PCD;
    *cio1Ctl = CIO1_C_CLEAR_PERR | // clear parity error
        CIO1_C_NO_WATCHDOG_RESET; // no watch dog reset
          
    *cio0Ctl = ZCIO_PAD;
    temp = *cio0Ctl;
    *cio0Ctl = ZCIO_PAD;

    if (adrSpace == VME_EXT)
      // dual port on to extended space
      *cio0Ctl = temp & (~CIO0_A_DETTACH_VME_EXT);
    else
      *cio0Ctl = temp & (~CIO0_A_DETTACH_VME_STD);
    return (OK);
}

STATUS Tp41::setVmePage(FAST int page)
{
    page &= 7;

    if (!page)
      return(ERROR);

    FAST unsigned char mask = (unsigned char)page;

    setcio(0,"b",mask);

    return(OK);
}

int Tp41::getVmePage()
{
    FAST unsigned char *cio = (unsigned char *)Tp41::cio0 + Tp41::b_off;

    return((int)(*cio & (Tp41::VmePage0 | Tp41::VmePage1 | Tp41::VmePage2)));
}

STATUS Tp41::busRwd()
{
    if ((rcio(1,"b") & Tp41::Ror))
        {
            FAST unsigned char mask = Tp41::Ror;

            setcio(1,"b",mask);
        }
    return(OK);
}

STATUS Tp41::busRor()
{
    if (!(rcio(1,"b") & Tp41::Ror))
        {
            FAST unsigned char mask = Tp41::Ror;

            setcio(1,"b",mask);
        }
    return(OK);
}

STATUS Tp41::busTmoEnable()
{
    if (!(rcio(1,"b") & Tp41::BusTmo))
        {
            FAST unsigned char mask = Tp41::BusTmo;

            setcio(1,"b",mask);
        }
    return(OK);
}

STATUS Tp41::busTmoDisable()
{
    if ((rcio(1,"b") & Tp41::BusTmo))
        {
            FAST unsigned char mask = Tp41::BusTmo;

            setcio(1,"b",mask);
        }
    return(OK);
}

STATUS Tp41::busPriorityMode()
{
    if ((rcio(1,"b") & Tp41::Rrs))
        {
            FAST unsigned char mask = Tp41::Rrs;

            setcio(1,"b",mask);
        }
    return(OK);
}

STATUS Tp41::busRoundRobin()
{
    if (!(rcio(1,"b") & Tp41::Rrs))
        {
            FAST unsigned char mask = Tp41::Rrs;

            setcio(1,"b",mask);
        }
    return(OK);
}
    
