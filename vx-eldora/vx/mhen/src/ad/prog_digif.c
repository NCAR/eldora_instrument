/*
 *      $Id$
 *
 *      Module: PROG_DIGIF.c
 *      Original Author: E. Loew
 *      Copywrited by the National Center for Atmospheric Research
 *      Date:            $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * Initial revision
 *
 *
 * description: This module programs the Dual Channel Digital IF Cards
 *              for operation in the ELDORA or SPOL systems.
 *              
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "vxWorks.h"
#include "stdioLib.h"
#include "NU_ELDRP7.h"
#include "math.h"

#ifdef ELDORA
#define PPP_DSPs 4   /* Number of Pulse Pair Processor DSP32C's */
#endif /* ELDORA */

#ifdef SPOL
#define PPP_DSPs 12  /* Number of Pulse Pair Processor DSP32C's */
#endif /* SPOL */

extern int Silent;

void   prog_digif(int num_freq, int num_gates, int gate_sp, int num_samples)

/*
 *  num_freq:    Number of frequencies to program (1-5).
 *  num_gates:   Number of range gates to produce.
 *  gate_sp:     Gate Spacing in meters.
 *  num_samples: Number of samples(hits) per dwell.
*/

   {
   unsigned long   freq,bd;
   unsigned char   *digif_ptr;
   int             gatelength, count, ppp_dsps;

   ppp_dsps = PPP_DSPs;
   
#ifdef ELDORA
   bd = 0x41;   
#endif
#ifdef SPOL
   bd = 0x61;
#endif
   for(freq=1; freq<=num_freq; freq++)
     {
#ifdef ELDORA	
	 digif_ptr = (unsigned char*)((bd << 16) + (freq << 24) + EXTD_BASE);
#endif
#ifdef SPOL
	 digif_ptr = (unsigned char*)((bd << 16) + (1 << 24) + 0x20000 * (freq - 1) + EXTD_BASE);
         if(freq == 3)      /* hv processor only has 4 DSPs */
           ppp_dsps = 4;   
#endif	 
	 /* Program Master Timer Counter 0 with gatelength */
	 
	 gatelength = gate_sp * 0.053333333 + 0.5;  /* Gate Spacing in 8MHz Counts */
	 if(!Silent)
	   printf("gatelength = %X \n",gatelength);
	 *(digif_ptr + MSTR_CWR) = CNTR_0 + RW_LSMSBYTE + MODE_2;
	 *(digif_ptr + MSTR_TM0) = gatelength & 0xFF;
	 *(digif_ptr + MSTR_TM0) = gatelength >> 8;

	 /* Program Master Timer Counter 1 with number of eof's */

	 count = (gatelength * num_gates) + (gatelength * ppp_dsps) - 1;
	 *(digif_ptr + MSTR_CWR) = CNTR_1 + RW_LSMSBYTE + MODE_5;
	 *(digif_ptr + MSTR_TM1) = count & 0xFF;
	 *(digif_ptr + MSTR_TM1) = count >> 8;	 

	 /* Program Master Timer Counter 2 with number of gates */

	 count = gatelength * (num_gates + 1) - 1;
	 *(digif_ptr + MSTR_CWR) = CNTR_2 + RW_LSMSBYTE + MODE_5;
	 *(digif_ptr + MSTR_TM2) = count & 0xFF;
	 *(digif_ptr + MSTR_TM2) = count >> 8;	 
	 
	 /* Program Mid Beam Timer Counter 0 with samples */
	 
	 count = num_samples;
	 *(digif_ptr + MDBM_CWR) = CNTR_0 + RW_LSMSBYTE + MODE_3;
	 *(digif_ptr + MDBM_TM0) = count & 0xFF;
	 *(digif_ptr + MDBM_TM0) = count >> 8;


     }
   }



