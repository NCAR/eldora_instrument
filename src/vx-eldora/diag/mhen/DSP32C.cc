
// <!-- file: ../doc/DSP32C.html >
// <TITLE>RDS: DSP32C </TITLE>
// <H1>Class Name:  DSP32C</H1>
// <H1>
// Class Description:
// </H1>
//  This class manages all the registers associated with a DSP32C
// processor
// <P>

#include "ELDRP7.h"
#include "DSP32C.hh"

DSP32C::DSP32C(register unsigned char *base) 
{
    pcrl_ = base + PCRL;
    pcrh_ = base + PCRH;

    pcr16_ = (unsigned short *)(base + PCR16);
    pare16_ = (unsigned short *)(base + PARE16);
    par16_ =  (unsigned short *)(base + PAR16);

    pdr16_ = (unsigned short *)(base + PDR16);
    pdr216_ =  (unsigned short *)(base + PDR216);
    pir16_ = (unsigned short *)(base + PIR16);
}
// setup the DMA port address
void DSP32C::setDMAaddr(unsigned long addr)
{
    *pare16_ = (addr >>16);
    *par16_ = (addr & 0xffff);
}
