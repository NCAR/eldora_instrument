
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

//#include <vxWorks.h>
#include <stdLoc.h>
#include <stdlib.h>
#include <tickLib.h>
#include <sysLib.h>
#include <vme.h>
//#include <iostream.h>


#include "ELDRP7.h"
#include "QuadDSP32C.hh"
#include "CollDSP32C.hh"

QuadDSP32C::QuadDSP32C(unsigned short freq, unsigned short bd_num) :
	 frequency_(freq), board_num_(bd_num) 
{
    volatile char *addr = (char *)((bd_num <<20)  + (freq << 24));
    unsigned char *localAddr;
    int stat;


    // Get local address of board (allows for H/W independence 

    stat = sysBusToLocalAdrs(VME_AM_EXT_SUP_DATA, (char *) addr, 
	(char **)&localAddr);
    if (stat != OK) {
	fprintf(stderr, "%s: ERROR in mapping address %x\n", FN, addr);
    }
    dsp[0] = new DSP32C (localAddr + DSP0SEL);
    dsp[1] = new DSP32C (localAddr + DSP1SEL);
    dsp[2] = new DSP32C (localAddr + DSP2SEL);
    dsp[3] = new DSP32C (localAddr + DSP3SEL);

}
QuadDSP32C::~QuadDSP32C()
{
    int i;

   //  fprintf(stderr, "destructor called for 0x%x\n", this);
    for (i = 0; i < 4; ++i)
    {
	delete dsp[i];
    }
}
QuadDSP32C & QuadDSP32C::operator=(const QuadDSP32C & q)
{
    QuadDSP32C *qnew = new QuadDSP32C(frequency_, board_num_);
    return *qnew;
    
}


// write data out the HSDBB of a specific quad processor
extern "C"   float ran2(long *);
#define NMAX 32767.0

int QuadDSP32C::hsdbb_wr(int proc, long seed)

{
    if (proc < 0 || proc > 3) {
	fprintf(stderr, "testing all quad procs not supported yet!\n");
	return -1;
    }

    DSP32C *quad_dsp = dsp[proc];

    long  j,  w_chksum, r_chksum, temp_r, temp_w,toggle, error;

    const int Fifo_sz = 8192;
    
// set up DSPQ PCR and PIO registers for 24 bit addressing,
// 32 bit DMA xfers, and a 16 bit Parallel I/O Port (Remember PIO Port
// defaults to 8 bit I/F after reset). NOTE: MSBYTE on VME is LSBYTE on
// DSP32C !!!
//

    quad_dsp->pcrl(REGMAP + DMA);
    quad_dsp->pcrh(DMA32 + PIO16);

    unsigned short pcr = (quad_dsp->pcr16() & 0xfcf); 
    if (pcr != 0x030a)
    {
	fprintf(stderr, "%s: quad PCR Register R/W ERROR, PCR = %x\n", FN, pcr);
	return 1;
    }
    long *temp = new long[Fifo_sz];

// set up DSPQ  B Port address (0xFFDF08) for DMA transfers 

    quad_dsp->setDMAaddr(0xffdf08);

    error = 0;
    
    for (j = 0; j < Fifo_sz; j++)
    {

      // don't need to swap bytes for HSDBB test, but
      //   must swap bytes if downloading DSP32C code to ensure
      //  proper operation
      //
	toggle = (j & 0x00000001) << 15; // ensures MSB changes
        temp_w = ((long)(ran2(&seed) * NMAX) + toggle) & 0x0000ffff;
        w_chksum += temp_w;
	temp[j] = temp_w;
	quad_dsp->pdr216(temp_w);

	// generate a new random number for high 16 bits 

        temp_w = ((long)(ran2(&seed) * NMAX) + toggle) & 0x0000ffff;        
	quad_dsp->pdr16(temp_w);
	temp_w <<= 16;
	w_chksum += temp_w;
        temp[j] += temp_w;
    }
    delete  [] temp;
    return (error);
}

// Read data in from the HSDBA of a specific quad processor

extern "C"   float ran2(long *);
#define NMAX 32767.0
#define NMAX8 127.0
int No_Print;

int QuadDSP32C::hsdba_rd(int proc, int digif_flag, long seed)
{
    if (proc < 0 || proc > 3) {
	fprintf(stderr, "testing all quad procs not supported yet!\n");
	return -1;
    }

    DSP32C *quad_dsp = dsp[proc];

    long  j,  w_chksum, r_chksum, temp_r, temp_w,toggle, error;

    const int Fifo_sz = 4096;
    
// set up DSPQ PCR and PIO registers for 24 bit addressing,
// 32 bit DMA xfers, and a 16 bit Parallel I/O Port (Remember PIO Port
// defaults to 8 bit I/F after reset). NOTE: MSBYTE on VME is LSBYTE on
// DSP32C !!!
//


    quad_dsp->pcrl(REGMAP + DMA);
    quad_dsp->pcrh(DMA32 + PIO16);

    unsigned short pcr = (quad_dsp->pcr16() & 0xfcf); 
    if (pcr != 0x030a)
    {
	fprintf(stderr, "%s: quad PCR Register R/W ERROR, PCR = %x\n", FN, pcr);
	return(-1);
    }
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

        if(digif_flag)
          temp_w = ((long)(ran2(&seed) * NMAX8) + toggle) & 0x0000ff;
        else
          temp_w = ((long)(ran2(&seed) * NMAX) + toggle) & 0x0000ffff;
	temp_w <<= 16;
	w_chksum += temp_w;
        temp[j] += temp_w;
    }
// set up DSPQ A Port address (0xFFDF04) for DMA transfers 

    quad_dsp->setDMAaddr(0xffdf04);

    // now, read back and check against what we've written

    for (j = 0; j < Fifo_sz; j++)
    {

        temp_r = quad_dsp->pdr216();
        temp_r += (quad_dsp->pdr16() << 16);
        r_chksum += temp_r;
        if (temp_r != temp[j])
        {
            error++;
	    if(!No_Print)
	      {
		fprintf(stderr, "HSDBA TRANSMISSION ERROR IN DSP%d\n",proc);
		fprintf(stderr, "WROTE  %x       READ  %x \n",temp[j],temp_r);
	      }
        }
    }

    delete  [] temp;
    return (error);
}

// write data out HSDBA of a specific quad processor

extern "C"   float ran2(long *);
#define NMAX 32767.0
#define NMAX8 127.0

int QuadDSP32C::hsdba_wr(int proc, long seed)
{
    if (proc < 0 || proc > 3) {
	fprintf(stderr, "testing all quad procs not supported yet!\n");
	return -1;
    }

    DSP32C *quad_dsp = dsp[proc];

    long  j,  w_chksum, r_chksum, temp_r, temp_w,toggle, error;

    const int Fifo_sz = 4096;
    
// set up DSPQ PCR and PIO registers for 24 bit addressing,
// 32 bit DMA xfers, and a 16 bit Parallel I/O Port (Remember PIO Port
// defaults to 8 bit I/F after reset). NOTE: MSBYTE on VME is LSBYTE on
// DSP32C !!!
//

    quad_dsp->pcrl(REGMAP + DMA);
    quad_dsp->pcrh(DMA32 + PIO16);

    unsigned short pcr = (quad_dsp->pcr16() & 0xfcf); 
    if (pcr != 0x030a)
    {
	fprintf(stderr, "%s: quad PCR Register R/W ERROR, PCR = %x\n", FN, pcr);
	return 1;
    }
    long *temp = new long[Fifo_sz];

// set up DSPQ A Port address (0xFFDF04) for DMA transfers 

    quad_dsp->setDMAaddr(0xffdf04);

    error = 0;
    
    for (j = 0; j < Fifo_sz; j++)
    {

      // don't need to swap bytes for HSDBA test, but
      //   must swap bytes if downloading DSP32C code to ensure
      //  proper operation
      //
	toggle = (j & 0x00000001) << 15; // ensures MSB changes
        temp_w = ((long)(ran2(&seed) * NMAX) + toggle) & 0x0000ffff;
        w_chksum += temp_w;
	temp[j] = temp_w;
	quad_dsp->pdr216(temp_w);

	// generate a new random number for high 8  bits 

        temp_w = ((long)(ran2(&seed) * NMAX8) + toggle) & 0x0000ff;        
	quad_dsp->pdr16(temp_w);
	temp_w <<= 16;
	w_chksum += temp_w;
        temp[j] += temp_w;
    }
    delete  [] temp;
    return (error);
}
// Toggles PDF Flag on DSP0

int QuadDSP32C::toggle_pdr(void)
{
    unsigned short temp;
    
    DSP32C *quad_dsp = dsp[0];
    
// set up DSPQ PCR and PIO registers for 24 bit addressing,
// 32 bit DMA xfers, and a 16 bit Parallel I/O Port (Remember PIO Port
// defaults to 8 bit I/F after reset). NOTE: MSBYTE on VME is LSBYTE on
// DSP32C !!!
//
    quad_dsp->pcrl(REGMAP + DMA);
    quad_dsp->pcrh(DMA32 + PIO16);

    unsigned short pcr = (quad_dsp->pcr16() & 0xfcf); 
    if (pcr != 0x030a)
        {
            fprintf(stderr, "%s: quad PCR Register R/W ERROR, PCR = %x\n", FN, pcr);
            return 1;
        }

    // toggle PDR

    temp = quad_dsp->pdr16();  // read PDR
    quad_dsp->pdr16(1);        // write PDR
    temp = quad_dsp->pdr16();  // read PDR
    
}



