
// <!-- file: ../doc/DSP32C.html >
// <TITLE>RDS: DSP32C </TITLE>
// <H1>Class Name:  DSP32C</H1>
// <H1>
// Class Description:
// </H1>
//  This class manages all the registers associated with a DSP32C
// processor
// <P>

static char FN[] = __FILE__;


#include <stdLoc.h>
#include <sysLib.h>
#include <vme.h>


#include "ELDRP7.h"
#include "CollDSP32C.hh"

CollDSP32C::CollDSP32C()
{
//    printf("I'm at %8X\n",this);
    volatile char *addr = (char *)((0 <<20)  + (4 << 24));
    unsigned char *localAddr;
    int stat;


    // Get local address of board (allows for H/W independence 

     stat = sysBusToLocalAdrs(VME_AM_EXT_SUP_DATA, (char *) addr, 
 	(char **)&localAddr);
     if (stat != OK) {
 	fprintf(stderr, "%s: ERROR in mapping address %x\n", FN, addr);
     }
     dsp = new DSP32C (localAddr + DSP0SEL);
//     printf("local address = %8X \n",localAddr); 
}
CollDSP32C::~CollDSP32C()
{
    delete dsp;
}
CollDSP32C & CollDSP32C::operator=(const CollDSP32C & q)
{
    CollDSP32C *qnew = new CollDSP32C();
    return *qnew;
    
}

extern "C" float ran2(long *);
#define NMAX 32767.0

// Read data in from the HSDBB of the Collator

int CollDSP32C::hsdbb_rd(long seed)
{

    long  j,  w_chksum, r_chksum, temp_r, temp_w,toggle, error;

    const int Fifo_sz = 8192;

// set up Coll PCR and PIO registers for 24 bit addressing,
// 32 bit DMA xfers, and a 16 bit Parallel I/O Port (Remember PIO Port
// defaults to 8 bit I/F after reset). NOTE: MSBYTE on VME is LSBYTE on
// DSP32C !!!
//

//    printf("this = %8X \n",this);
//    printf("made it to pcr \n");

    DSP32C *coll_dsp = dsp;
    
    coll_dsp->pcrl(REGMAP + DMA);
//    printf("set up pcrl \n");
    coll_dsp->pcrh(DMA32 + PIO16);
//    printf("set up pcrh \n");
    unsigned short pcr = (coll_dsp->pcr16() & 0xfcf);    
    if (pcr != 0x030a)
    {
	fprintf(stderr, "%s: Collator PCR Register R/W ERROR, PCR = %x\n", FN, pcr);
	return(-1);
    }
//    printf("made it past pcr \n");
    long *temp = new long[Fifo_sz];

    error = 0;
    
    for (j = 0; j < Fifo_sz; j++)
    {

        // generate  array of data that should have been
        // received given seed that was transmitted.  
        // don't need to swap bytes for HSDBA test, but
        //   must swap bytes if downloading DSP32C code to ensure
        //  proper operation
    
	toggle = (j & 0x00000001) << 15; // ensures MSB changes
        temp_w = ((long)(ran2(&seed) * NMAX) + toggle) & 0x0000ffff;
        w_chksum += temp_w;
	temp[j] = temp_w;

	// generate a new random number for high 16 bits 

        temp_w = ((long)(ran2(&seed) * NMAX) + toggle) & 0x0000ffff;        
	temp_w <<= 16;
	w_chksum += temp_w;
        temp[j] += temp_w;
    }

// set up Collator's DSP0's B Port address (0xFFDF04) for DMA transfers 

    coll_dsp->setDMAaddr(0xffdf08);
//    printf("set up DMA address \n");
    
    // now, read back and check against what we've written

    for (j = 0; j < Fifo_sz; j++)
        {

            temp_r = coll_dsp->pdr216();
            temp_r += (coll_dsp->pdr16() << 16);
            r_chksum += temp_r;
            if (temp_r != temp[j])
                {
                    error++;
                    fprintf(stderr, "HSDBB TRANSMISSION ERROR \n");
                    fprintf(stderr, "WROTE  %x       READ  %x \n",temp[j],temp_r);
                }
        }

    delete  [] temp;
    return (error);
}








