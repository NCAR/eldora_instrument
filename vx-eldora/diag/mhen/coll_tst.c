/*
 *	$Id$
 *
 *	Module: COLL_TST		 
 *	Original Author: E. Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date: 3/7/94		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description: This module tests the functionality of the Collator Card via
 *              the VME host CPU. It writes and reads from
 *              all ports accessible to the VME Bus and compares the two 
 *              values, if not equal an error is reported and status of -1
 *              is returned.
 * 
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "math.h"
#include "stdioLib.h"
#include "taskLib.h"
#include "ELDRP7.h"
#include <vme.h>

#define  MASK  0x0000ffff   /* used to mask off 16 MSbits */
#define  NMAX  32767.0      /* maximum positive 16 bit number (0x7fff) */
#define  NUM   1           /* number of tests per each memory */

extern int bit_flag;

long temp[16384];
int coll_tst()
{

long  i, j, seed, w_chksum, r_chksum, temp_r, temp_w, error, toggle;
volatile long  *vme_pntr;
unsigned char  status;
volatile unsigned char *dspq_pntr;
float  random;
volatile short  *dsp_addrh_pntr, *dsp_addrl_pntr, *dsp_dat_pntr,
                *dsp_pntr;
short addrl;
volatile char*  localAddr;
STATUS stat;
int result=0;

float ran2();

/* Make Collator base address H/W independent */

dspq_pntr = (unsigned char *)(COL0BASE - EXTD_BASE);
stat = sysBusToLocalAdrs(VME_AM_EXT_SUP_DATA, dspq_pntr, (char **)(&localAddr));

/* Reset DSP0 in software -- just in case */

dspq_pntr = (unsigned char *)(localAddr + DSP0SEL + PCRL);
*dspq_pntr = 0x0;
*dspq_pntr = 0x1;
*dspq_pntr = 0x0;
  
/* Write to DSP32C's Dual Port Ram (A Memory) via PIO and read back data via
   the VME Bus. Verify contents with checksum sent. 
*/

/* Test DSP0's DPRAM locations (0x0 to 0xffff bytes) */
/* Address Ram as 16 bit words */

seed = -1024;  /* any negative integer will seed random number generator */

/* 
   set up PCR and PIO registers for 24 bit addressing, 32 bit DMA xfers,
   and a 16 bit Parallel I/O Port (Remember PIO Port defaults to 8 bit 
   I/F after reset). NOTE: MSBYTE on VME is LSBYTE on DSP32C !!!
*/

dspq_pntr = (unsigned char *)(localAddr + DSP0SEL + PCRL);
*dspq_pntr = REGMAP + DMA + AUTO;
dspq_pntr = (unsigned char *)(localAddr + DSP0SEL + PCRH);
*dspq_pntr = DMA32 + PIO16;

/* verify PCR contains appropriate data */

dsp_pntr = (short *)(localAddr + DSP0SEL + PCR16);
temp[0] = *dsp_pntr & 0xf9f; /* mask off PDF and PIF */  
if (temp[0] != 0x031a)
printf("Collator DSP0 PCR Register R/W ERROR, PCR = %x \n",temp[0]);

/* execute memory test NUM times */

for(i = 0; i < NUM; i++)
  {
      error = 0;
      w_chksum = 0;
      r_chksum = 0;

/* set up starting address (0x0) in RAM for DMA transfers */

      dsp_addrh_pntr = (short *)(localAddr + DSP0SEL + PARE16);
      *dsp_addrh_pntr = 0x0;
      dsp_addrl_pntr = (short *)(localAddr + DSP0SEL + PAR16);
      addrl = 0x0;
      *dsp_addrl_pntr = addrl;

/* write data out to ram */

      for (j=0; j<16384; j++)
	{
	    random = ran2(&seed);
	    toggle = (j & 0x00000001) << 15; /* ensures MSB changes as well */
	    temp_w = ((long)(random * NMAX) + toggle) & 0x0000ffff;
	    temp[j] = temp_w;
	    w_chksum += temp_w;
	    
	    /* write random number to lower 16 bits of ram and toggle msbit */
	    dsp_dat_pntr = (short *)(localAddr + DSP0SEL + PDR216);
	    *dsp_dat_pntr = (short)(random * NMAX) + toggle;   
	    
	    random = ran2(&seed);
	    temp_w = ((long)(random * NMAX) + toggle) & 0x0000ffff;
            temp_w <<= 16;
            temp[j] += temp_w;
	    w_chksum += temp_w;

	    /* write out random number to upper 16 bits and toggle msbit */
	    dsp_dat_pntr = (short *)(localAddr + DSP0SEL + PDR16);
	    *dsp_dat_pntr = (short)(random * NMAX) + toggle;

	}
      /* Read DPRAM data back directly from VME BUS */

      vme_pntr = (long *)(localAddr + DPRAM);
      for (j=0; j<16384; j++)
	{
	    temp_r = *vme_pntr++;
	    if (temp_r != temp[j])
	      {
		  error++;
		  if(!bit_flag)
		    {
			printf("COLLATOR DSP0 DPRAM ACCESS ERROR IN DPRAM TEST 1 AT LOCATION %x \n",j);
			printf("WROTE %x : READ %x \n",temp[j],temp_r);
		    }
	      }
	    r_chksum += temp_r;

	}
      seed--;   /* change seed for random number generator */

/* compare both checksums and if not equal, produce appropriate error msg */

      if (r_chksum != w_chksum)
	{
	    printf("%d COLLATOR DSP0 DPRAM ACCESS ERROR(S) IN TEST 1 TRIAL %d  \n",error,i);
	    printf("Read Checksum = %x   \n", r_chksum);
	    printf("Written Checksum = %x   \n", w_chksum);
	    result |= 1;
	}
      else 
	{
	    printf("COLLATOR DSP0 DPRAM TEST 1 TRIAL %d  PASSED \n",i);
	    result |= 0;
	}  
  
  }


/* 
   Now fill DPRAM directly via VME interface and read back contents via
   PIO on the DSP32C
*/

/* execute memory test NUM times */

for(i = 0; i < NUM; i++)
  {
      error = 0;
      w_chksum = 0;
      r_chksum = 0;
      vme_pntr = (long *)(localAddr + DPRAM);

/* write data out to DPRAM */

      for (j=0; j<16384; j++)
	{

	    random = ran2(&seed);
	    toggle = (j & 0x00000001) << 15; /* ensures MSB changes as well */
	    temp_w = ((long)(random * NMAX) + toggle) & 0x0000ffff;
	    temp[j] = temp_w;
	    w_chksum += temp_w;
	    
	    random = ran2(&seed);
	    temp_w = ((long)(random * NMAX) + toggle) & 0x0000ffff;
            temp_w <<= 16;
            temp[j] += temp_w;
	    w_chksum += temp_w;
	    
	    /* Write 32 bit data out to DPRAM */

	    *vme_pntr++ = temp[j];
	}

      /* set up starting address (0x0) in RAM for DMA transfers */

      dsp_addrh_pntr = (short *)(localAddr + DSP0SEL + PARE16);
      *dsp_addrh_pntr = 0x0;
      dsp_addrl_pntr = (short *)(localAddr + DSP0SEL + PAR16);
      addrl = 0x0;
      *dsp_addrl_pntr = addrl;

      for (j=0; j<16384; j++)
	{
      
            /* read and verify ram data */

	    dsp_dat_pntr = (short *)(localAddr + DSP0SEL + PDR216);
	    temp_r = *dsp_dat_pntr & 0x0000ffff;
	    dsp_dat_pntr = (short *)(localAddr + DSP0SEL + PDR16);
	    temp_r += ((*dsp_dat_pntr & 0x0000ffff) << 16);
	    if (temp_r != temp[j])
	      {
		  error++;
		  if(!bit_flag)
		    {
			printf("COLLATOR DSP0 DPRAM ACCESS ERROR IN DPRAM TEST2 AT LOCATION %x \n",j);
			printf("WROTE %x : READ %x \n",temp[j],temp_r);
		    }
	      }
	    r_chksum += temp_r;

	}

      seed--;   /* change seed for random number generator */

/* compare both checksums and if not equal, produce appropriate error msg */

      if (r_chksum != w_chksum)
	{
	    printf("%d COLLATOR DSP0 DPRAM ACCESS ERROR(S) IN TEST 2 TRIAL %d  \n",error,i);
	    printf("Read Checksum = %x   \n", r_chksum);
	    printf("Written Checksum = %x   \n", w_chksum);
	    result |= 1;
	}
      else 
	{
	    printf("COLLATOR DSP0 DPRAM TEST 2 TRIAL %d  PASSED \n",i);
	    result |= 0;
	}  
  
  }


/* 
   Test one way VME Bus Latch by writing random 32 bit data out to the
   latch via the VME Bus and reading it back via the PIO on the DSP32C.
*/

/* 
   Write to latch directly via VME interface and read back contents via
   PIO on the DSP32C
*/

/* 
   Set up PCR to disable auto incrementing
*/

dsp_pntr = (short *)(localAddr + DSP0SEL + PCR16);
*dsp_pntr = ((DMA32 + PIO16) << 8) + REGMAP + DMA;

/*
   Verify PCR contains appropriate data
*/

temp[0] = *dsp_pntr & 0xf9f; /* mask off PDF and PIF */
if (temp[0] != 0x030a)
printf ("DSP0 PCR Register R/W ERROR, PCR = %x \n",temp[0]);

/* execute latch test NUM times */

for(i = 0; i < NUM; i++)
  {
      error = 0;
      w_chksum = 0;
      r_chksum = 0;
      vme_pntr = (long *)(localAddr + LTCHSEL);

/* write data out to latch */

      for (j=0; j<16384; j++)
	{

	    random = ran2(&seed);
	    toggle = (j & 0x00000001) << 15; /* ensures MSB changes as well */
	    temp_w = ((long)(random * NMAX) + toggle) & 0x0000ffff;
	    temp[j] = temp_w;
	    w_chksum += temp_w;
	    
	    random = ran2(&seed);
	    temp_w = ((long)(random * NMAX) + toggle) & 0x0000ffff;
            temp_w <<= 16;
            temp[j] += temp_w;
	    w_chksum += temp_w;
	    
	    /* Write 32 bit data out to DPRAM */

	    *vme_pntr = temp[j];
	

      /* set up starting address (0x0) in RAM for DMA transfers */

	    dsp_addrh_pntr = (short *)(localAddr + DSP0SEL + PARE16);
	    *dsp_addrh_pntr = 0x00ff;
	    dsp_addrl_pntr = (short *)(localAddr + DSP0SEL + PAR16);
	    addrl = 0xde00;
	    *dsp_addrl_pntr = addrl;

      
            /* read and verify latch data */

	    dsp_dat_pntr = (short *)(localAddr + DSP0SEL + PDR216);
	    temp_r = *dsp_dat_pntr & 0x0000ffff;
	    dsp_dat_pntr = (short *)(localAddr + DSP0SEL + PDR16);
	    temp_r += ((*dsp_dat_pntr & 0x0000ffff) << 16);
	    if (temp_r != temp[j])
	      {
		  error++;
		  if(!bit_flag)
		    {
			printf("COLLATOR LATCH ACCESS ERROR IN VME LATCH TEST  \n");
			printf("WROTE %x : READ %x \n",temp[j],temp_r);
		    }
	      }
	    r_chksum += temp_r;

	}

      seed--;   /* change seed for random number generator */

/* compare both checksums and if not equal, produce appropriate error msg */

      if (r_chksum != w_chksum)
	{
	    printf("%d COLLATOR LATCH ACCESS ERROR(S) IN VME LATCH TEST %d  \n",error,i);
	    printf("Read Checksum = %x   \n", r_chksum);
	    printf("Written Checksum = %x   \n", w_chksum);
	    result |= 1;
	}
      else 
	{
	    printf("COLLATOR-VME LATCH TEST %d  PASSED \n",i);
	    result |= 0;
	}  
  
  }

/*
   Toggle led on Collator Card from VME via PIO on Collator 
*/

for (i=0; i<10; i++)
  {
     /* Turn on LED */

      dsp_addrh_pntr = (short *)(localAddr + DSP0SEL + PARE16);
      *dsp_addrh_pntr = 0x00ff;
      dsp_addrl_pntr = (short *)(localAddr + DSP0SEL + PAR16);
      *dsp_addrl_pntr = 0xde60;
      dsp_dat_pntr = (short *)(localAddr + DSP0SEL + PDR216);
      temp_r = *dsp_dat_pntr;
      taskDelay(20);  /* a short delay */

    /* Turn off LED */

      dsp_addrh_pntr = (short *)(localAddr + DSP0SEL + PARE16);
      *dsp_addrh_pntr = 0x00ff;
      dsp_addrl_pntr = (short *)(localAddr + DSP0SEL + PAR16);
      *dsp_addrl_pntr = 0xde70;
      dsp_dat_pntr = (short *)(localAddr + DSP0SEL + PDR216);
      temp_r = *dsp_dat_pntr;
      taskDelay(20);  /* a short delay */
  }
/* Return PCR to its reset configuration by reseting DSP32C */

dsp_pntr = (short *)(localAddr + DSP0SEL + PCR16);
*dsp_pntr = 0x0;
*dsp_pntr = 0x1;
dspq_pntr = (unsigned char *)(localAddr + DSP0SEL + PCRL);
*dspq_pntr = 0x0;
temp[0] = *dspq_pntr & 0x9f; /* mask off PDF and PIF */  
dspq_pntr = (unsigned char *)(localAddr + DSP0SEL + PCRH);
*dspq_pntr = 0x0;
temp[1] = *dspq_pntr & 0xf;
if (temp[0] != 0x0 || temp[1] != 0x0)
  {
      printf("Failed to return Collator's DSP0 PCR to reset state \n");
      printf("pcrl = %4x \n",temp[0]);
      printf("pcrh = %4x \n",temp[1]);
  }
if(result)
  return(-1);
else
  return(0);
}

