/*
 *	$Id$
 *
 *	Module: QUAD_TST		 
 *	Original Author: E. Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date: 3/8/94		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description: This module tests the functionality of the DSPQ Card via
 *              a VME host CPU. It writes and reads from
 *              all ports accessible to the VME Bus and compares the two 
 *              values, if not equal an error is reported, and a status of -1
 *              is returned. If equal a status of 0 is returned. It makes use of 
 *              the auto-increment function in the DSP32C during DMA transfers.
 *
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "math.h"
#include "stdioLib.h"
#include "taskLib.h"
#include "ELDRP7.h"
#include "string.h"
#include <vme.h>
#define  MASK  0x0000ffff   /* used to mask off 16 MSbits */
#define  NMAX  32767.0      /* maximum positive 16 bit number (0x7fff) */
#define  NUM   1            /* number of tests per each memory */


extern int bit_flag;
long temp[65536];
int quad_tst(frq,bd)
unsigned long  bd,frq;
{

long  i, j, seed, w_chksum, r_chksum, temp_r, temp_w, error, toggle;
unsigned char  status;
volatile unsigned char  *dspq_pntr;
char  id[11], result;
volatile char* localAddr;
STATUS stat;
float  random;
volatile short  *dsp_addrh_pntr, *dsp_addrl_pntr, *dsp_dat_pntr,
                *dsp_pntr;
short   addrl;
int  dsp;

float ran2();
#ifdef ELDORA
if(frq == 1 && bd == 1)
  strcpy(id,"Freq1 PPP");
if(frq == 2 && bd == 1)
  strcpy(id,"Freq2 PPP");
if(frq == 3 && bd == 1)
  strcpy(id,"Freq3 PPP");
if(frq == 4 && bd == 1)
  strcpy(id,"Freq4 PPP");
if(frq == 5 && bd == 1)
  strcpy(id,"Freq5 PPP");

if(frq == 1 && bd == 3)
  strcpy(id,"DP #1");
if(frq == 2 && bd == 3)
  strcpy(id,"DP #2");
#endif
#ifdef SPOL
if(frq == 1 && bd == 1)
  strcpy(id,"PP #1");
if(frq == 1 && bd == 2)
  strcpy(id,"PP #2");
if(frq == 1 && bd == 3)
  strcpy(id,"PP #3");
if(frq == 1 && bd == 4)
  strcpy(id,"PP #4");
if(frq == 1 && bd == 5)
  strcpy(id,"PP #5");
if(frq == 2 && bd == 1)
  strcpy(id,"DP");
#endif
result = 0;
frq = (frq << 24);                /* freq # * 10**6 */
bd = bd << 20;                    /* Board # * 10**5 = base addr */
bd += frq;                        /* Full base addr of board */
/* Get local address of board (allows for H/W independence */
stat = sysBusToLocalAdrs(VME_AM_EXT_SUP_DATA, (char *) bd, (char **)&localAddr); 

if(!bit_flag)
  {
      printf("Indicate desired %s DSP32C to test by typing a number \n",id);
      printf("in the range of 0 to 3; or type 4 to test all %s DSP32C's in succession \n",id);
      scanf(" %d",&dsp);
  }
else
  {
      dsp = 4;
  }
switch (dsp)
  {

    case 0:   

/* first toggle test LED on DSPQ Card */

for (i=0; i<10; i++)
  {
      dspq_pntr = (unsigned char *)(localAddr + DLEDON);
      *dspq_pntr = 0x0;   /* write anything only address is important */
      taskDelay(20);       /* short delay */
      dspq_pntr = (unsigned char *)(localAddr + DLEDOFF);
      *dspq_pntr = 0x0;  /* write anything only address is important */
      taskDelay(20);
  }

/* Write to each DSP32C's External Ram (A Memory) and verify contents with 
   checksum sent 
*/

/* Test DSP0's RAM locations (0x0 to 0xffff words) */
/* Address Ram as 16 bit words */

      seed = -1024;  /* any negative integer will seed random number generator */

/* Reset DSP0 in S/W -- just in case */

dspq_pntr = (unsigned char *)(localAddr + DSP0SEL + PCRL);
*dspq_pntr = 0x0;
*dspq_pntr = 0x1;
*dspq_pntr = 0x0;

/* set up PCR and PIO registers for 24 bit addressing, 32 bit DMA xfers,
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
	printf("%s DSP0 PCR Register R/W ERROR, PCR = %x \n",id,temp[0]);

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
	    
	    for (j=0; j<65536; j++)
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

      /* set up starting address (0x0) in RAM for DMA transfers */

	    dsp_addrh_pntr = (short *)(localAddr + DSP0SEL + PARE16);
	    *dsp_addrh_pntr = 0x0;
	    dsp_addrl_pntr = (short *)(localAddr + DSP0SEL + PAR16);
	    addrl = 0x0;
	    *dsp_addrl_pntr = addrl;


	    for (j=0; j<65536; j++)
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
			      printf("%s DSP0 RAM ACCESS ERROR AT LOCATION %x \n",id,j);
			      printf("WROTE %x : READ %x \n",temp[j],temp_r);
			  }
		    }
		  r_chksum += temp_r;

	      }

	    seed--;   /* change seed for random number generator */

/* compare both checksums and if not equal, produce appropriate error msg */

	    if (r_chksum != w_chksum)
	      {
		  printf("%d %s DSP0 RAM ACCESS ERROR(S) IN TEST %d  \n",error,id,i);
		  printf("Read Checksum = %x   \n", r_chksum);
		  printf("Written Checksum = %x   \n", w_chksum);
		  result |= 1;
	      }
	    else 
	      {
		  printf("%s DSP0 RAM TEST %d  PASSED \n",id,i);
		  result |= 0;
	      }  
  
	}
/* Return PCR to its reset condition by resetting DSP32C */

dsp_pntr = (short *)(localAddr + DSP0SEL + PCR16);
*dsp_pntr = 0x0;
*dsp_pntr = 0x1;
dspq_pntr = (unsigned char *)(localAddr + DSP0SEL + PCRL);
*dspq_pntr = 0x0;
temp[0] = *dspq_pntr;
dspq_pntr = (unsigned char *)(localAddr + DSP0SEL + PCRH);
*dspq_pntr = 0x0;
temp[1] = *dspq_pntr & 0x7;
if (temp[0] != 0x0 || temp[1] != 0)
  {
      printf("Failed to return %s DSP0 PCR to reset configuration \n",id);
      printf("pcrl = %2x \n",temp[0]);
      printf("pcrh = %2x \n",temp[1]);
  }

    break;

    case 1:

/* first toggle test LED on DSPQ Card */

for (i=0; i<10; i++)
  {
      dspq_pntr = (unsigned char *)(localAddr + DLEDON);
      *dspq_pntr = 0x0;   /* write anything only address is important */
      taskDelay(20);       /* short delay */
      dspq_pntr = (unsigned char *)(localAddr + DLEDOFF);
      *dspq_pntr = 0x0;  /* write anything only address is important */
      taskDelay(20);
  }

/* Reset DSP1 in S/W -- just in case */

dspq_pntr = (unsigned char *)(localAddr + DSP1SEL + PCRL);
*dspq_pntr = 0x0;
*dspq_pntr = 0x1;
*dspq_pntr = 0x0;

/* Test DSP1's RAM locations (0x0 to 0xffff words) */
/* Address Ram as 16 bit words */

      seed = -1943;  /* any negative integer will seed random number generator */

/* set up PCR and PIO registers for 24 bit addressing, 32 bit DMA xfers,
   and a 16 bit Parallel I/O Port (Remember PIO Port defaults to 8 bit 
   I/F after reset). NOTE: MSBYTE on VME is LSBYTE on DSP32C !!!
*/

      dspq_pntr = (unsigned char *)(localAddr + DSP1SEL + PCRL);
      *dspq_pntr = REGMAP + DMA + AUTO;
      dspq_pntr = (unsigned char *)(localAddr + DSP1SEL + PCRH);
      *dspq_pntr = DMA32 + PIO16;

/* verify PCR contains appropriate data */

      dsp_pntr = (short *)(localAddr + DSP1SEL + PCR16);
      temp[0] = *dsp_pntr & 0xf9f; /* mask off PDF and PIF */  
      if (temp[0] != 0x031a)
	printf("%s DSP1 PCR Register R/W ERROR, PCR = %x \n",id,temp[0]);

/* execute memory test NUM times */

      for(i = 0; i < NUM; i++)
	{
	    error = 0;
	    w_chksum = 0;
	    r_chksum = 0;

/* set up starting address (0x0) in RAM for DMA transfers */

	    dsp_addrh_pntr = (short *)(localAddr + DSP1SEL + PARE16);
	    *dsp_addrh_pntr = 0x0;
	    dsp_addrl_pntr = (short *)(localAddr + DSP1SEL + PAR16);
	    addrl = 0x0;
	    *dsp_addrl_pntr = addrl;

	    /* write data out to ram */

	    for (j=0; j<65536; j++)
	      {
		  random = ran2(&seed);
		  toggle = (j & 0x00000001) << 15; /* ensures MSB changes as well */
		  temp_w = ((long)(random * NMAX) + toggle) & 0x0000ffff;
		  temp[j] = temp_w;
		  w_chksum += temp_w;
	    
	    /* write random number to lower 16 bits of ram and toggle msbit */
		  dsp_dat_pntr = (short *)(localAddr + DSP1SEL + PDR216);
		  *dsp_dat_pntr = (short)(random * NMAX) + toggle;   
	    
		  random = ran2(&seed);
		  temp_w = ((long)(random * NMAX) + toggle) & 0x0000ffff;
		  temp_w <<= 16;
		  temp[j] += temp_w;
		  w_chksum += temp_w;

	    /* write out random number to upper 16 bits and toggle msbit */
		  dsp_dat_pntr = (short *)(localAddr + DSP1SEL + PDR16);
		  *dsp_dat_pntr = (short)(random * NMAX) + toggle;

	      }

      /* set up starting address (0x0) in RAM for DMA transfers */

	    dsp_addrh_pntr = (short *)(localAddr + DSP1SEL + PARE16);
	    *dsp_addrh_pntr = 0x0;
	    dsp_addrl_pntr = (short *)(localAddr + DSP1SEL + PAR16);
	    addrl = 0x0;
	    *dsp_addrl_pntr = addrl;


	    for (j=0; j<65536; j++)
	      {
      
		  /* read and verify ram data */

		  dsp_dat_pntr = (short *)(localAddr + DSP1SEL + PDR216);
		  temp_r = *dsp_dat_pntr & 0x0000ffff;
		  dsp_dat_pntr = (short *)(localAddr + DSP1SEL + PDR16);
		  temp_r += ((*dsp_dat_pntr & 0x0000ffff) << 16);
		  if (temp_r != temp[j])
		    {
			error++;
			if(!bit_flag)
			  {
			      printf("%s DSP1 RAM ACCESS ERROR AT LOCATION %x \n",id,j);
			      printf("WROTE %x : READ %x \n",temp[j],temp_r);
			  }
		    }
		  r_chksum += temp_r;

	      }

	    seed--;   /* change seed for random number generator */

/* compare both checksums and if not equal, produce appropriate error msg */

	    if (r_chksum != w_chksum)
	      {
		  printf("%d %s DSP1 RAM ACCESS ERROR(S) IN TEST %d  \n",error,id,i);
		  printf("Read Checksum = %x   \n", r_chksum);
		  printf("Written Checksum = %x   \n", w_chksum);
		  result |= 1;
	      }
	    else 
	      {
		  printf("%s DSP1 RAM TEST %d  PASSED \n",id,i);
		  result |= 0;
	      }  
  
	}
/* Return PCR to its reset condition by resetting DSP32C */

dsp_pntr = (short *)(localAddr + DSP1SEL + PCR16);
*dsp_pntr = 0x0;
*dsp_pntr = 0x1;
dspq_pntr = (unsigned char *)(localAddr + DSP1SEL + PCRL);
*dspq_pntr = 0x0;
temp[0] = *dspq_pntr;
dspq_pntr = (unsigned char *)(localAddr + DSP1SEL + PCRH);
*dspq_pntr = 0x0;
temp[1] = *dspq_pntr & 0x7;
if (temp[0] != 0x0 || temp[1] != 0)
  {
      printf("Failed to return %s DSP1 PCR to reset configuration \n",id);
      printf("pcrl = %2x \n",temp[0]);
      printf("pcrh = %2x \n",temp[1]);
  }

break;

    case 2:

/* first toggle test LED on DSPQ Card */

for (i=0; i<10; i++)
  {
      dspq_pntr = (unsigned char *)(localAddr + DLEDON);
      *dspq_pntr = 0x0;   /* write anything only address is important */
      taskDelay(20);       /* short delay */
      dspq_pntr = (unsigned char *)(localAddr + DLEDOFF);
      *dspq_pntr = 0x0;  /* write anything only address is important */
      taskDelay(20);
  }

/* Reset DSP2 in S/W -- just in case */

dspq_pntr = (unsigned char *)(localAddr + DSP2SEL + PCRL);
*dspq_pntr = 0x0;
*dspq_pntr = 0x1;
*dspq_pntr = 0x0;

/* Test DSP2's RAM locations (0x0 to 0xffff words) */
/* Address Ram as 16 bit words */

      seed = -984;  /* any negative integer will seed random number generator */

/* set up PCR and PIO registers for 24 bit addressing, 32 bit DMA xfers,
   and a 16 bit Parallel I/O Port (Remember PIO Port defaults to 8 bit 
   I/F after reset). NOTE: MSBYTE on VME is LSBYTE on DSP32C !!!
*/

      dspq_pntr = (unsigned char *)(localAddr + DSP2SEL + PCRL);
      *dspq_pntr = REGMAP + DMA + AUTO;
      dspq_pntr = (unsigned char *)(localAddr + DSP2SEL + PCRH);
      *dspq_pntr = DMA32 + PIO16;

      /* verify PCR contains appropriate data */

      dsp_pntr = (short *)(localAddr + DSP2SEL + PCR16);
      temp[0] = *dsp_pntr & 0xf9f; /* mask off PDF and PIF */  
      if (temp[0] != 0x031a)
	printf("%s DSP2 PCR Register R/W ERROR, PCR = %x \n",id,temp[0]);

      /* execute memory test NUM times */

      for(i = 0; i < NUM; i++)
	{
	    error = 0;
	    w_chksum = 0;
	    r_chksum = 0;

	    /* set up starting address (0x0) in RAM for DMA transfers */

	    dsp_addrh_pntr = (short *)(localAddr + DSP2SEL + PARE16);
	    *dsp_addrh_pntr = 0x0;
	    dsp_addrl_pntr = (short *)(localAddr + DSP2SEL + PAR16);
	    addrl = 0x0;
	    *dsp_addrl_pntr = addrl;

	    /* write data out to ram */

	    for (j=0; j<65536; j++)
	      {
		  random = ran2(&seed);
		  toggle = (j & 0x00000001) << 15; /* ensures MSB changes as well */
		  temp_w = ((long)(random * NMAX) + toggle) & 0x0000ffff;
		  temp[j] = temp_w;
		  w_chksum += temp_w;
	    
	    /* write random number to lower 16 bits of ram and toggle msbit */
		  dsp_dat_pntr = (short *)(localAddr + DSP2SEL + PDR216);
		  *dsp_dat_pntr = (short)(random * NMAX) + toggle;   
	    
		  random = ran2(&seed);
		  temp_w = ((long)(random * NMAX) + toggle) & 0x0000ffff;
		  temp_w <<= 16;
		  temp[j] += temp_w;
		  w_chksum += temp_w;

	    /* write out random number to upper 16 bits and toggle msbit */
		  dsp_dat_pntr = (short *)(localAddr + DSP2SEL + PDR16);
		  *dsp_dat_pntr = (short)(random * NMAX) + toggle;

	      }

      /* set up starting address (0x0) in RAM for DMA transfers */

	    dsp_addrh_pntr = (short *)(localAddr + DSP2SEL + PARE16);
	    *dsp_addrh_pntr = 0x0;
	    dsp_addrl_pntr = (short *)(localAddr + DSP2SEL + PAR16);
	    addrl = 0x0;
	    *dsp_addrl_pntr = addrl;


	    for (j=0; j<65536; j++)
	      {
      
		  /* read and verify ram data */

		  dsp_dat_pntr = (short *)(localAddr + DSP2SEL + PDR216);
		  temp_r = *dsp_dat_pntr & 0x0000ffff;
		  dsp_dat_pntr = (short *)(localAddr + DSP2SEL + PDR16);
		  temp_r += ((*dsp_dat_pntr & 0x0000ffff) << 16);
		  if (temp_r != temp[j])
		    {
			error++;
			if(!bit_flag)
			  {
			      printf("%s DSP2 RAM ACCESS ERROR AT LOCATION %x \n",id,j);
			      printf("WROTE %x : READ %x \n",temp[j],temp_r);
			  }
		    }
		  r_chksum += temp_r;

	      }

	    seed--;   /* change seed for random number generator */

/* compare both checksums and if not equal, produce appropriate error msg */

	    if (r_chksum != w_chksum)
	      {
		  printf("%d %s DSP2 RAM ACCESS ERROR(S) IN TEST %d  \n",error,id,i);
		  printf("Read Checksum = %x   \n", r_chksum);
		  printf("Written Checksum = %x   \n", w_chksum);
		  result |= 1;
	      }
	    else 
	      {
		  printf("%s DSP2 RAM TEST %d  PASSED \n",id,i);
		  result |= 0;
	      }  
  
	}

/* Return PCR to its reset condition by resetting DSP32C */

dsp_pntr = (short *)(localAddr + DSP2SEL + PCR16);
*dsp_pntr = 0x0;
*dsp_pntr = 0x1;
dspq_pntr = (unsigned char *)(localAddr + DSP2SEL + PCRL);
*dspq_pntr = 0x0;
temp[0] = *dspq_pntr;
dspq_pntr = (unsigned char *)(localAddr + DSP2SEL + PCRH);
*dspq_pntr = 0x0;
temp[1] = *dspq_pntr & 0x7;
if (temp[0] != 0x0 || temp[1] != 0)
  {
      printf("Failed to return %s DSP2 PCR to reset configuration \n",id);
      printf("pcrl = %2x \n",temp[0]);
      printf("pcrh = %2x \n",temp[1]);
  }

break;

    case 3:

/* first toggle test LED on DSPQ Card */

for (i=0; i<10; i++)
  {
      dspq_pntr = (unsigned char *)(localAddr + DLEDON);
      *dspq_pntr = 0x0;   /* write anything only address is important */
      taskDelay(20);       /* short delay */
      dspq_pntr = (unsigned char *)(localAddr + DLEDOFF);
      *dspq_pntr = 0x0;  /* write anything only address is important */
      taskDelay(20);
  }

/* Reset DSP3 in S/W -- just in case */

dspq_pntr = (unsigned char *)(localAddr + DSP3SEL + PCRL);
*dspq_pntr = 0x0;
*dspq_pntr = 0x1;
*dspq_pntr = 0x0;

/* Test DSP3's RAM locations (0x0 to 0xffff words) */
/* Address Ram as 16 bit words */

      seed = -987;  /* any negative integer will seed random number generator */



/* set up PCR and PIO registers for 24 bit addressing, 32 bit DMA xfers,
   and a 16 bit Parallel I/O Port (Remember PIO Port defaults to 8 bit 
   I/F after reset). NOTE: MSBYTE on VME is LSBYTE on DSP32C !!!
*/
      
      dspq_pntr = (unsigned char *)(localAddr + DSP3SEL + PCRL);
      *dspq_pntr = REGMAP + DMA + AUTO;
      dspq_pntr = (unsigned char *)(localAddr + DSP3SEL + PCRH);
      *dspq_pntr = DMA32 + PIO16;

      /* verify PCR contains appropriate data */

      dsp_pntr = (short *)(localAddr + DSP3SEL + PCR16);
      temp[0] = *dsp_pntr & 0xf9f; /* mask off PDF and PIF */  
      if (temp[0] != 0x031a)
	printf("%s DSP3 PCR Register R/W ERROR, PCR = %x \n",id,temp[0]);
      
      /* execute memory test NUM times */

      for(i = 0; i < NUM; i++)
	{
	    error = 0;
	    w_chksum = 0;
	    r_chksum = 0;

	    /* set up starting address (0x0) in RAM for DMA transfers */

	    dsp_addrh_pntr = (short *)(localAddr + DSP3SEL + PARE16);
	    *dsp_addrh_pntr = 0x0;
	    dsp_addrl_pntr = (short *)(localAddr + DSP3SEL + PAR16);
	    addrl = 0x0;
	    *dsp_addrl_pntr = addrl;

	    /* write data out to ram */

	    for (j=0; j<65536; j++)
	      {
		  random = ran2(&seed);
		  toggle = (j & 0x00000001) << 15; /* ensures MSB changes as well */
		  temp_w = ((long)(random * NMAX) + toggle) & 0x0000ffff;
		  temp[j] = temp_w;
		  w_chksum += temp_w;
	    
	    /* write random number to lower 16 bits of ram and toggle msbit */
		  dsp_dat_pntr = (short *)(localAddr + DSP3SEL + PDR216);
		  *dsp_dat_pntr = (short)(random * NMAX) + toggle;   
		  
		  random = ran2(&seed);
		  temp_w = ((long)(random * NMAX) + toggle) & 0x0000ffff;
		  temp_w <<= 16;
		  temp[j] += temp_w;
		  w_chksum += temp_w;

	    /* write out random number to upper 16 bits and toggle msbit */
		  dsp_dat_pntr = (short *)(localAddr + DSP3SEL + PDR16);
		  *dsp_dat_pntr = (short)(random * NMAX) + toggle;

	      }

      /* set up starting address (0x0) in RAM for DMA transfers */

	    dsp_addrh_pntr = (short *)(localAddr + DSP3SEL + PARE16);
	    *dsp_addrh_pntr = 0x0;
	    dsp_addrl_pntr = (short *)(localAddr + DSP3SEL + PAR16);
	    addrl = 0x0;
	    *dsp_addrl_pntr = addrl;


	    for (j=0; j<65536; j++)
	      {
      
		  /* read and verify ram data */

		  dsp_dat_pntr = (short *)(localAddr + DSP3SEL + PDR216);
		  temp_r = *dsp_dat_pntr & 0x0000ffff;
		  dsp_dat_pntr = (short *)(localAddr + DSP3SEL + PDR16);
		  temp_r += ((*dsp_dat_pntr & 0x0000ffff) << 16);
		  if (temp_r != temp[j])
		    {
			error++;
			if(!bit_flag)
			  {
			      printf("%s DSP3 RAM ACCESS ERROR AT LOCATION %x \n",id,j);
			      printf("WROTE %x : READ %x \n",temp[j],temp_r);
			  }
		    }
		  r_chksum += temp_r;

	      }

	    seed--;   /* change seed for random number generator */

/* compare both checksums and if not equal, produce appropriate error msg */

	    if (r_chksum != w_chksum)
	      {
		  printf("%d %s DSP3 RAM ACCESS ERROR(S) IN TEST %d  \n",error,id,i);
		  printf("Read Checksum = %x   \n", r_chksum);
		  printf("Written Checksum = %x   \n", w_chksum);
		  result |= 1;
	      }
	    else 
	      {
		  printf("%s DSP3 RAM TEST %d  PASSED \n",id,i);
		  result |= 0;
	      }  
	    
	}

/* Return PCR to its reset condition by resetting DSP32C */

dsp_pntr = (short *)(localAddr + DSP3SEL + PCR16);
*dsp_pntr = 0x0;
*dsp_pntr = 0x1;
dspq_pntr = (unsigned char *)(localAddr + DSP3SEL + PCRL);
*dspq_pntr = 0x0;
temp[0] = *dspq_pntr;
dspq_pntr = (unsigned char *)(localAddr + DSP3SEL + PCRH);
*dspq_pntr = 0x0;
temp[1] = *dspq_pntr & 0x7;
if (temp[0] != 0x0 || temp[1] != 0)
  {
      printf("Failed to return %s DSP3 PCR to reset configuration \n",id);
      printf("pcrl = %2x \n",temp[0]);
      printf("pcrh = %2x \n",temp[1]);
  }

break;
  
    case 4:

/* first toggle test LED on DSPQ Card */

for (i=0; i<10; i++)
  {
      dspq_pntr = (unsigned char *)(localAddr + DLEDON);
      *dspq_pntr = 0x0;   /* write anything only address is important */
      taskDelay(20);       /* short delay */
      dspq_pntr = (unsigned char *)(localAddr + DLEDOFF);
      *dspq_pntr = 0x0;  /* write anything only address is important */
      taskDelay(20);
  }

/* Reset DSP0 in S/W -- just in case */

dspq_pntr = (unsigned char *)(localAddr + DSP0SEL + PCRL);
*dspq_pntr = 0x0;
*dspq_pntr = 0x1;
*dspq_pntr = 0x0;

/* Write to each DSP32C's External Ram (A Memory) and verify contents with 
   checksum sent 
*/

/* Test DSP0's RAM locations (0x0 to 0xffff words) */
/* Address Ram as 16 bit words */

seed = -1024;  /* any negative integer will seed random number generator */

/* set up PCR and PIO registers for 24 bit addressing, 32 bit DMA xfers,
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
printf("%s DSP0 PCR Register R/W ERROR, PCR = %x \n",id,temp[0]);

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

      for (j=0; j<65536; j++)
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

      /* set up starting address (0x0) in RAM for DMA transfers */

      dsp_addrh_pntr = (short *)(localAddr + DSP0SEL + PARE16);
      *dsp_addrh_pntr = 0x0;
      dsp_addrl_pntr = (short *)(localAddr + DSP0SEL + PAR16);
      addrl = 0x0;
      *dsp_addrl_pntr = addrl;


      for (j=0; j<65536; j++)
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
			printf("%s DSP0 RAM ACCESS ERROR AT LOCATION %x \n",id,j);
			printf("WROTE %x : READ %x \n",temp[j],temp_r);
		    }
	      }
	    r_chksum += temp_r;

	}

      seed--;   /* change seed for random number generator */

/* compare both checksums and if not equal, produce appropriate error msg */

      if (r_chksum != w_chksum)
	{
	    printf("%d %s DSP0 RAM ACCESS ERROR(S) IN TEST %d  \n",error,id,i);
	    printf("Read Checksum = %x   \n", r_chksum);
	    printf("Written Checksum = %x   \n", w_chksum);
	    result |= 1;
	}
      else 
	{
	    printf("%s DSP0 RAM TEST %d  PASSED \n",id,i);
	    result |= 0;
	}  
  
  }

/* Return PCR to its reset condition by resetting DSP32C */

dsp_pntr = (short *)(localAddr + DSP0SEL + PCR16);
*dsp_pntr = 0x0;
*dsp_pntr = 0x1;
dspq_pntr = (unsigned char *)(localAddr + DSP0SEL + PCRL);
*dspq_pntr = 0x0;
temp[0] = *dspq_pntr;
dspq_pntr = (unsigned char *)(localAddr + DSP0SEL + PCRH);
*dspq_pntr = 0x0;
temp[1] = *dspq_pntr & 0x7;
if (temp[0] != 0x0 || temp[1] != 0)
  {
      printf("Failed to return %s DSP0 PCR to reset configuration \n",id);
      printf("pcrl = %2x \n",temp[0]);
      printf("pcrh = %2x \n",temp[1]);
  }

/* Test DSP1's RAM locations (0x0 to 0xffff words) */
/* Address Ram as 16 bit words */

/* Reset DSP1 in S/W -- just in case */

dspq_pntr = (unsigned char *)(localAddr + DSP1SEL + PCRL);
*dspq_pntr = 0x0;
*dspq_pntr = 0x1;
*dspq_pntr = 0x0;

seed = -1943;  /* any negative integer will seed random number generator */

/* set up PCR and PIO registers for 24 bit addressing, 32 bit DMA xfers,
   and a 16 bit Parallel I/O Port (Remember PIO Port defaults to 8 bit 
   I/F after reset). NOTE: MSBYTE on VME is LSBYTE on DSP32C !!!
*/

dspq_pntr = (unsigned char *)(localAddr + DSP1SEL + PCRL);
*dspq_pntr = REGMAP + DMA + AUTO;
dspq_pntr = (unsigned char *)(localAddr + DSP1SEL + PCRH);
*dspq_pntr = DMA32 + PIO16;

/* verify PCR contains appropriate data */

dsp_pntr = (short *)(localAddr + DSP1SEL + PCR16);
temp[0] = *dsp_pntr & 0xf9f; /* mask off PDF and PIF */  
if (temp[0] != 0x031a)
printf("%s DSP1 PCR Register R/W ERROR, PCR = %x \n",id,temp[0]);

/* execute memory test NUM times */

for(i = 0; i < NUM; i++)
  {
      error = 0;
      w_chksum = 0;
      r_chksum = 0;

/* set up starting address (0x0) in RAM for DMA transfers */

      dsp_addrh_pntr = (short *)(localAddr + DSP1SEL + PARE16);
      *dsp_addrh_pntr = 0x0;
      dsp_addrl_pntr = (short *)(localAddr + DSP1SEL + PAR16);
      addrl = 0x0;
      *dsp_addrl_pntr = addrl;

/* write data out to ram */

      for (j=0; j<65536; j++)
	{
	    random = ran2(&seed);
	    toggle = (j & 0x00000001) << 15; /* ensures MSB changes as well */
	    temp_w = ((long)(random * NMAX) + toggle) & 0x0000ffff;
	    temp[j] = temp_w;
	    w_chksum += temp_w;
	    
	    /* write random number to lower 16 bits of ram and toggle msbit */
	    dsp_dat_pntr = (short *)(localAddr + DSP1SEL + PDR216);
	    *dsp_dat_pntr = (short)(random * NMAX) + toggle;   
	    
	    random = ran2(&seed);
	    temp_w = ((long)(random * NMAX) + toggle) & 0x0000ffff;
            temp_w <<= 16;
            temp[j] += temp_w;
	    w_chksum += temp_w;

	    /* write out random number to upper 16 bits and toggle msbit */
	    dsp_dat_pntr = (short *)(localAddr + DSP1SEL + PDR16);
	    *dsp_dat_pntr = (short)(random * NMAX) + toggle;

	}

      /* set up starting address (0x0) in RAM for DMA transfers */

      dsp_addrh_pntr = (short *)(localAddr + DSP1SEL + PARE16);
      *dsp_addrh_pntr = 0x0;
      dsp_addrl_pntr = (short *)(localAddr + DSP1SEL + PAR16);
      addrl = 0x0;
      *dsp_addrl_pntr = addrl;


      for (j=0; j<65536; j++)
	{
      
            /* read and verify ram data */

	    dsp_dat_pntr = (short *)(localAddr + DSP1SEL + PDR216);
	    temp_r = *dsp_dat_pntr & 0x0000ffff;
	    dsp_dat_pntr = (short *)(localAddr + DSP1SEL + PDR16);
	    temp_r += ((*dsp_dat_pntr & 0x0000ffff) << 16);
	    if (temp_r != temp[j])
	      {
		  error++;
		  if(!bit_flag)
		    {
			printf("%s DSP1 RAM ACCESS ERROR AT LOCATION %x \n",id,j);
			printf("WROTE %x : READ %x \n",temp[j],temp_r);
		    }
	      }
	    r_chksum += temp_r;

	}

      seed--;   /* change seed for random number generator */

/* compare both checksums and if not equal, produce appropriate error msg */

      if (r_chksum != w_chksum)
	{
	    printf("%d %s DSP1 RAM ACCESS ERROR(S) IN TEST %d  \n",error,id,i);
	    printf("Read Checksum = %x   \n", r_chksum);
	    printf("Written Checksum = %x   \n", w_chksum);
	    result |= 1;
	}
      else 
	{
	    printf("%s DSP1 RAM TEST %d  PASSED \n",id,i);
	    result |= 0;
	}  
  
  }

/* Return PCR to its reset condition by resetting DSP32C */

dsp_pntr = (short *)(localAddr + DSP1SEL + PCR16);
*dsp_pntr = 0x0;
*dsp_pntr = 0x1;
dspq_pntr = (unsigned char *)(localAddr + DSP1SEL + PCRL);
*dspq_pntr = 0x0;
temp[0] = *dspq_pntr;
dspq_pntr = (unsigned char *)(localAddr + DSP1SEL + PCRH);
*dspq_pntr = 0x0;
temp[1] = *dspq_pntr & 0x7;
if (temp[0] != 0x0 || temp[1] != 0)
  {
      printf("Failed to return %s DSP1 PCR to reset configuration \n",id);
      printf("pcrl = %2x \n",temp[0]);
      printf("pcrh = %2x \n",temp[1]);
  }

/* Test DSP2's RAM locations (0x0 to 0xffff words) */
/* Address Ram as 16 bit words */

seed = -984;  /* any negative integer will seed random number generator */

/* Reset DSP2 in S/W -- just in case */

dspq_pntr = (unsigned char *)(localAddr + DSP2SEL + PCRL);
*dspq_pntr = 0x0;
*dspq_pntr = 0x1;
*dspq_pntr = 0x0;

/* set up PCR and PIO registers for 24 bit addressing, 32 bit DMA xfers,
   and a 16 bit Parallel I/O Port (Remember PIO Port defaults to 8 bit 
   I/F after reset). NOTE: MSBYTE on VME is LSBYTE on DSP32C !!!
*/

dspq_pntr = (unsigned char *)(localAddr + DSP2SEL + PCRL);
*dspq_pntr = REGMAP + DMA + AUTO;
dspq_pntr = (unsigned char *)(localAddr + DSP2SEL + PCRH);
*dspq_pntr = DMA32 + PIO16;

/* verify PCR contains appropriate data */

dsp_pntr = (short *)(localAddr + DSP2SEL + PCR16);
temp[0] = *dsp_pntr & 0xf9f; /* mask off PDF and PIF */  
if (temp[0] != 0x031a)
printf("%s DSP2 PCR Register R/W ERROR, PCR = %x \n",id,temp[0]);

/* execute memory test NUM times */

for(i = 0; i < NUM; i++)
  {
      error = 0;
      w_chksum = 0;
      r_chksum = 0;

/* set up starting address (0x0) in RAM for DMA transfers */

      dsp_addrh_pntr = (short *)(localAddr + DSP2SEL + PARE16);
      *dsp_addrh_pntr = 0x0;
      dsp_addrl_pntr = (short *)(localAddr + DSP2SEL + PAR16);
      addrl = 0x0;
      *dsp_addrl_pntr = addrl;

/* write data out to ram */

      for (j=0; j<65536; j++)
	{
	    random = ran2(&seed);
	    toggle = (j & 0x00000001) << 15; /* ensures MSB changes as well */
	    temp_w = ((long)(random * NMAX) + toggle) & 0x0000ffff;
	    temp[j] = temp_w;
	    w_chksum += temp_w;
	    
	    /* write random number to lower 16 bits of ram and toggle msbit */
	    dsp_dat_pntr = (short *)(localAddr + DSP2SEL + PDR216);
	    *dsp_dat_pntr = (short)(random * NMAX) + toggle;   
	    
	    random = ran2(&seed);
	    temp_w = ((long)(random * NMAX) + toggle) & 0x0000ffff;
            temp_w <<= 16;
            temp[j] += temp_w;
	    w_chksum += temp_w;

	    /* write out random number to upper 16 bits and toggle msbit */
	    dsp_dat_pntr = (short *)(localAddr + DSP2SEL + PDR16);
	    *dsp_dat_pntr = (short)(random * NMAX) + toggle;

	}

      /* set up starting address (0x0) in RAM for DMA transfers */

      dsp_addrh_pntr = (short *)(localAddr + DSP2SEL + PARE16);
      *dsp_addrh_pntr = 0x0;
      dsp_addrl_pntr = (short *)(localAddr + DSP2SEL + PAR16);
      addrl = 0x0;
      *dsp_addrl_pntr = addrl;


      for (j=0; j<65536; j++)
	{
      
            /* read and verify ram data */

	    dsp_dat_pntr = (short *)(localAddr + DSP2SEL + PDR216);
	    temp_r = *dsp_dat_pntr & 0x0000ffff;
	    dsp_dat_pntr = (short *)(localAddr + DSP2SEL + PDR16);
	    temp_r += ((*dsp_dat_pntr & 0x0000ffff) << 16);
	    if (temp_r != temp[j])
	      {
		  error++;
		  if(!bit_flag)
		    {
			printf("%s DSP2 RAM ACCESS ERROR AT LOCATION %x \n",id,j);
			printf("WROTE %x : READ %x \n",temp[j],temp_r);
		    }
	      }
	    r_chksum += temp_r;

	}

      seed--;   /* change seed for random number generator */

/* compare both checksums and if not equal, produce appropriate error msg */

      if (r_chksum != w_chksum)
	{
	    printf("%d %s DSP2 RAM ACCESS ERROR(S) IN TEST %d  \n",error,id,i);
	    printf("Read Checksum = %x   \n", r_chksum);
	    printf("Written Checksum = %x   \n", w_chksum);
	    result |= 1;
	}
      else 
	{
	    printf("%s DSP2 RAM TEST %d  PASSED \n",id,i);
	    result |= 0;
	}  
  
  }


/* Return PCR to its reset condition by resetting DSP32C */

dsp_pntr = (short *)(localAddr + DSP2SEL + PCR16);
*dsp_pntr = 0x0;
*dsp_pntr = 0x1;
dspq_pntr = (unsigned char *)(localAddr + DSP2SEL + PCRL);
*dspq_pntr = 0x0;
temp[0] = *dspq_pntr;
dspq_pntr = (unsigned char *)(localAddr + DSP2SEL + PCRH);
*dspq_pntr = 0x0;
temp[1] = *dspq_pntr & 0x7;
if (temp[0] != 0x0 || temp[1] != 0)
  {
      printf("Failed to return %s DSP2 PCR to reset configuration \n",id);
      printf("pcrl = %2x \n",temp[0]);
      printf("pcrh = %2x \n",temp[1]);
  }

/* Test DSP3's RAM locations (0x0 to 0xffff words) */
/* Address Ram as 16 bit words */

seed = -987;  /* any negative integer will seed random number generator */

/* Reset DSP3 in S/W -- just in case */

dspq_pntr = (unsigned char *)(localAddr + DSP3SEL + PCRL);
*dspq_pntr = 0x0;
*dspq_pntr = 0x1;
*dspq_pntr = 0x0;

/* set up PCR and PIO registers for 24 bit addressing, 32 bit DMA xfers,
   and a 16 bit Parallel I/O Port (Remember PIO Port defaults to 8 bit 
   I/F after reset). NOTE: MSBYTE on VME is LSBYTE on DSP32C !!!
*/

dspq_pntr = (unsigned char *)(localAddr + DSP3SEL + PCRL);
*dspq_pntr = REGMAP + DMA + AUTO;
dspq_pntr = (unsigned char *)(localAddr + DSP3SEL + PCRH);
*dspq_pntr = DMA32 + PIO16;

/* verify PCR contains appropriate data */

dsp_pntr = (short *)(localAddr + DSP3SEL + PCR16);
temp[0] = *dsp_pntr & 0xf9f; /* mask off PDF and PIF */  
if (temp[0] != 0x031a)
printf("%s DSP3 PCR Register R/W ERROR, PCR = %x \n",id,temp[0]);

/* execute memory test NUM times */

for(i = 0; i < NUM; i++)
  {
      error = 0;
      w_chksum = 0;
      r_chksum = 0;

/* set up starting address (0x0) in RAM for DMA transfers */

      dsp_addrh_pntr = (short *)(localAddr + DSP3SEL + PARE16);
      *dsp_addrh_pntr = 0x0;
      dsp_addrl_pntr = (short *)(localAddr + DSP3SEL + PAR16);
      addrl = 0x0;
      *dsp_addrl_pntr = addrl;

/* write data out to ram */

      for (j=0; j<65536; j++)
	{
	    random = ran2(&seed);
	    toggle = (j & 0x00000001) << 15; /* ensures MSB changes as well */
	    temp_w = ((long)(random * NMAX) + toggle) & 0x0000ffff;
	    temp[j] = temp_w;
	    w_chksum += temp_w;
	    
	    /* write random number to lower 16 bits of ram and toggle msbit */
	    dsp_dat_pntr = (short *)(localAddr + DSP3SEL + PDR216);
	    *dsp_dat_pntr = (short)(random * NMAX) + toggle;   
	    
	    random = ran2(&seed);
	    temp_w = ((long)(random * NMAX) + toggle) & 0x0000ffff;
            temp_w <<= 16;
            temp[j] += temp_w;
	    w_chksum += temp_w;

	    /* write out random number to upper 16 bits and toggle msbit */
	    dsp_dat_pntr = (short *)(localAddr + DSP3SEL + PDR16);
	    *dsp_dat_pntr = (short)(random * NMAX) + toggle;

	}

      /* set up starting address (0x0) in RAM for DMA transfers */

      dsp_addrh_pntr = (short *)(localAddr + DSP3SEL + PARE16);
      *dsp_addrh_pntr = 0x0;
      dsp_addrl_pntr = (short *)(localAddr + DSP3SEL + PAR16);
      addrl = 0x0;
      *dsp_addrl_pntr = addrl;


      for (j=0; j<65536; j++)
	{
      
            /* read and verify ram data */

	    dsp_dat_pntr = (short *)(localAddr + DSP3SEL + PDR216);
	    temp_r = *dsp_dat_pntr & 0x0000ffff;
	    dsp_dat_pntr = (short *)(localAddr + DSP3SEL + PDR16);
	    temp_r += ((*dsp_dat_pntr & 0x0000ffff) << 16);
	    if (temp_r != temp[j])
	      {
		  error++;
		  if(!bit_flag)
		    {
			printf("%s DSP3 RAM ACCESS ERROR AT LOCATION %x \n",id,j);
			printf("WROTE %x : READ %x \n",temp[j],temp_r);
		    }
	      }
	    r_chksum += temp_r;

	}

      seed--;   /* change seed for random number generator */

/* compare both checksums and if not equal, produce appropriate error msg */

      if (r_chksum != w_chksum)
	{
	    printf("%d %s DSP3 RAM ACCESS ERROR(S) IN TEST %d  \n",error,id,i);
	    printf("Read Checksum = %x   \n", r_chksum);
	    printf("Written Checksum = %x   \n", w_chksum);
	    result |= 1;
	}
      else 
	{
	    printf("%s DSP3 RAM TEST %d  PASSED \n",id,i);
	    result |= 0;
	}  
  
  }

/* Return PCR to its reset condition by resetting DSP32C */

dsp_pntr = (short *)(localAddr + DSP3SEL + PCR16);
*dsp_pntr = 0x0;
*dsp_pntr = 0x1;
dspq_pntr = (unsigned char *)(localAddr + DSP3SEL + PCRL);
*dspq_pntr = 0x0;
temp[0] = *dspq_pntr;
dspq_pntr = (unsigned char *)(localAddr + DSP3SEL + PCRH);
*dspq_pntr = 0x0;
temp[1] = *dspq_pntr & 0x7;
if (temp[0] != 0x0 || temp[1] != 0)
  {
      printf("Failed to return %s DSP3 PCR to reset configuration \n",id);
      printf("pcrl = %2x \n",temp[0]);
      printf("pcrh = %2x \n",temp[1]);
  }

break;
  }
if(result)
  return(-1);
else 
  return(0);
}
