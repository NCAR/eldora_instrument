/*
 *	$Id$
 *
 *	Module:	LD_TIMMOD	 
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.6  2002/03/12  18:33:38  eric
 * *** empty log message ***
 *
 * Revision 1.5  1994/04/29  17:08:14  eric
 * ensured proper rounding for float to int calculations by
 * casting ints as floats.
 *
 * Revision 1.4  1994/03/23  21:40:39  eric
 * fixed bug with preknock in blanking ram.
 *
 * Revision 1.3  1994/02/01  15:19:51  craig
 * Change calculation of duty cycle, PRF, etc and some error checking
 *
 * Revision 1.2  1994/01/31  18:19:47  eric
 * changed type of repeat_seq to float from short.
 *
 * Revision 1.1  1993/07/22  17:31:28  eric
 * Initial revision
 *
 *
 * Revision
 *
 * Initial revision
 *
 *
 description:
 
 Purpose:       Program ELDORA Timing Module from the Header.


 Date Written:  3/28/93
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define NO_RP7_SCOPE
#define NO_RDP_SCOPE
#include "vxWorks.h"
#include "math.h"
#include "stdioLib.h"
#include "intLib.h"
#include "memLib.h"
#include "semLib.h"
#include "taskLib.h"
#include "tyLib.h"
#include "ioLib.h"
#include "in.h"
#include "systime.h"
#include "sysLib.h"

#include "HeaderRpc.h"
#include "Parameter.h"
#include "Header.h"
#include "CellSpacing.h"
#include "Ray.h"
#include "Platform.h"
#include "FieldParam.h"
#include "Waveform.h"
#include "RadarDesc.h"
#include "RadarGbls.h"
#include "RDPGlobals.h"
#include "mcpl_def.h"
#include "mcpl_gbl.h"
#include "ELDRP7.h"
#include "coll_dpramdefs.h"
#include "colquadsubs.h"
#include "dspdefs.h"
#include "dspaddr.h"
#include "rp7.h"

extern int Silent;
extern CELLSPACING *cs;
extern RADARDESC *rdsc;
extern WAVEFORM *wvfm;
extern PARAMETER *prm;
extern FIELDRADAR *fldrdr;


void ld_timmod(int site,int module)

{
    unsigned short *blankloc,*nloc,*chiploc;
    unsigned short pkstart,f0start,f1start,f2start,f3start,f4start;
    unsigned short pkend,f0end,f1end,f2end,f3end,f4end;
    unsigned short chipramvalue,blankingvalue;
    unsigned short chipindex,maxindex,i,pcpn;
    short chip_width[6],chip_offset[6], total_pcp;
    short num_gates[6],gate_dist1[2],gate_dist2[2],gate_dist3[2];
    short gate_dist4[2],gate_dist5[2];
    unsigned short *timgate0,*timgate1,*timgate2,*timgate3,*timgate4;
    unsigned short gateindex,gatevalue;
    int            sampl,gates,gate_sp,first_gate,rpt_seq,j,shortcnt;
    int            pcpcnt[10];
    float          duty_cycle, prf, repeat_seq, dwell_time, gate_tm; 
    float          pcp, short_prt;

if(!module) /* module = 0 if New timing module; module = 1 if old timing module */
  {
      nloc        = (unsigned short *)(TIMBASE + TIMPCPN);
      blankloc    = (unsigned short *)(TIMBASE + TIMBLANK);
      chiploc     = (unsigned short *)(TIMBASE + TIMCHIP);
    
    /* Turn Off Timing Module -- just in case */

    timoffb();
  }
else
  {
      nloc        = (unsigned short *)(TIMBASE - STD_BASE + EXTD_BASE+ TIMPCPN);
      blankloc    = (unsigned short *)(TIMBASE - STD_BASE + EXTD_BASE + TIMBLANK);
      chiploc     = (unsigned short *)(TIMBASE - STD_BASE + EXTD_BASE + TIMCHIP);
    
    /* Turn Off Timing Module -- just in case */

    timoff();
  }
    /* Parse Header for Required Chip Information */

    wvfm = GetWaveform(inHeader);
    for(i=0; i<6; i++)
      {
	  chip_offset[i] = (unsigned short)(wvfm -> chip_offset[i]);
	  chip_width[i] = (unsigned short)(wvfm -> chip_width[i]);
      }
    
    repeat_seq = wvfm -> repeat_seq;
    total_pcp = (unsigned short)(wvfm -> total_pcp);

    /* Compute PRF, Duty Cycle, etc. from Header and PRINT it */

    gates = wvfm -> num_gates[0]; /* Assumes equal numbers of gates for all
                                     frequencies */
    gate_sp = wvfm -> gate_dist1[1]; /* Assumes uniform gate spacing for all 
                                        frequencies */
    gate_sp = (float)(gate_sp) * 2.5 + 0.5; /* convert 60 mhz counts to m and r
ound */
    first_gate = wvfm -> gate_dist1[0]; /* Assumes first gate spacing for 
                                           all frequencies */
    first_gate = (float)(first_gate) * 2.5 + 0.5; /* convert 60 mhz counts to m and r
ound */

    /* Compute dwell time in milliseconds */
    dwell_time = wvfm -> repeat_seq * wvfm -> repeat_seq_dwel;
    /* Compute the duty cycle in percent */
    duty_cycle = 100.0 * wvfm -> num_chips[5] * wvfm -> chip_width[5] / (wvfm -> repeat_seq * 1.0e-3 * 60.0e6);
    /* Compute the number of samples for frequency #1 */
    sampl = wvfm -> repeat_seq_dwel;
    /* Compute the shortest PRT for freqeuncy #1 in seconds */
    pcp = repeat_seq * 1.0E-3 / total_pcp;
    j = 0;
    pcpcnt[j] = 0;
    for(i=0; i<total_pcp; i++)
      {
	  if((wvfm -> blank_chip[i] & 0x01) == 0x01)
	    {
		j++;
		if(j > 9 ) j = 9;
		pcpcnt[j] = 1;
	    }
	  else
	    pcpcnt[j]++;
      }
    pcpcnt[j] = pcpcnt[j] + pcpcnt[0];
    shortcnt = pcpcnt[1];
    if(j >= 2)
      {
	  for(i=2; i<(j+1); i++)
	    if(pcpcnt[i] < shortcnt) shortcnt = pcpcnt[i];
      }
    short_prt = pcp * shortcnt;
    prf = 1.0 / short_prt;

  /* check for duty cycle > 1% */
  if (duty_cycle > 1.0)
    {
	if(!Silent)
	  {
	      printf("ld_timmod: duty cycle must not be more than 1%%.\n");
	      printf("ld_timmod: requested duty cycle = %f%%.\n",duty_cycle);
	      printf("ld_timmod: aborting without programming timingb module...\n\n");
	  }
      return;
    } 

    printf("Samples = %d; PRF = %f; GATES = %d; GATE SPACING = %d; \n",sampl,prf,gates,gate_sp); 
    printf("First Gate = %d; Duty Cycle = %f; \n",first_gate,duty_cycle);

    /* load N for desired PCP */
    pcpn = (repeat_seq * 15000.0 / (float)(total_pcp)) + 0.5;
    pcpn = pcpn - 1;
    *nloc = pcpn;

    /* DO CHIPS FOR 5 FREQUENCIES & PREKNOCK */

    pkstart = chip_offset[5];
    pkend   = chip_offset[5] + chip_width[5];
    f0start = chip_offset[0];
    f0end   = chip_offset[0] + chip_width[0];
    f1start = chip_offset[1];
    f1end   = chip_offset[1] + chip_width[1];
    f2start = chip_offset[2];
    f2end   = chip_offset[2] + chip_width[2];
    f3start = chip_offset[3];
    f3end   = chip_offset[3] + chip_width[3];
    f4start = chip_offset[4];
    f4end   = chip_offset[4] + chip_width[4];
    
    /* find end of sequence */
    maxindex = pkend;
    if (f0end > maxindex) maxindex = f0end;
    if (f1end > maxindex) maxindex = f1end;
    if (f2end > maxindex) maxindex = f2end;
    if (f3end > maxindex) maxindex = f3end;
    if (f4end > maxindex) maxindex = f4end;
    
    /* error checking */
    if (maxindex > 4090)
      {
	  if(!Silent)
	    {
		printf("ld_timmod: index computation error.  CHIP RAM bounds exceeded.\n");
		printf("ld_timmod: aborting without fully programming timing module.\n\n");
	    }
	  return;
      }
    
    /* Load chip RAM */
    *chiploc = 0x8000;  /* Load initial, special value */
    chiploc++;
    for (chipindex = 1; chipindex<maxindex; chipindex++)
      {
	  chipramvalue = 0x8000; /* set STOPCT* bit */
	  if ( (chipindex >= pkstart) & (chipindex < pkend) )
	    chipramvalue = chipramvalue | 0x0400;
	  if ( (chipindex >= f0start) & (chipindex < f0end) )
	    chipramvalue = chipramvalue | 0x0001;
	  if ( (chipindex >= f1start) & (chipindex < f1end) )
	    chipramvalue = chipramvalue | 0x0002;
	  if ( (chipindex >= f2start) & (chipindex < f2end) )
	    chipramvalue = chipramvalue | 0x0004;
	  if ( (chipindex >= f3start) & (chipindex < f3end) )
	    chipramvalue = chipramvalue | 0x0008;
	  if ( (chipindex >= f4start) & (chipindex < f4end) )
	    chipramvalue = chipramvalue | 0x0010;
	  *chiploc = chipramvalue;
/*	  printf("writing chipram value of %4x to location %8x\n",chipramvalue,chiploc);
*/
	  chiploc++;
	  if(!module) /* module = 0 if new timing module; 0 for old timing module */
	    {
		if (chiploc > (unsigned short *) TIMBASE+TIMCHIP+4090)
		  {
		      if(!Silent)
			{
			    printf("ld_timmod: CHIP RAM index computation error.\n");
			    printf("ld_timmod: aborting without fully programming timing module.\n\n");
			}
		      return;
		  }
	    }
	  else
	    {
		if (chiploc > (unsigned short *) TIMBASE-STD_BASE+EXTD_BASE+TIMCHIP+4090)
		  {
		      if(!Silent)
			{
			    printf("ld_timmod: CHIP RAM index computation error.\n");
			    printf("ld_timmod: aborting without fully programming timing module.\n\n");
			}
		      return;
		  }
	    }
      }
    /* Write terminal value (twice, as required by timing module) */
    *chiploc = 0x0000;
    if(!Silent)
      printf("wrote chipram terminal value of 0000 to location %8x\n",chiploc);
    chiploc++;
    *chiploc = 0x0000;
    if(!Silent)
      printf("wrote chipram terminal value of 0000 to location %8x\n",chiploc);

    /* GENERATE BLANKING VALUES */
    if (total_pcp == 1)
      {
	  blankingvalue = wvfm -> blank_chip[0] & 0x001f;
	  if (wvfm -> blank_chip[0] & 0x0020)
	    blankingvalue=blankingvalue|0x0C00; /*shift pk*/
	  *blankloc = blankingvalue;
/*	  printf("writing blanking value of %4x to location %8x\n",blankingvalue,blankloc); */

	  blankloc++;
      }
    else
      {
	  for (i=0; i<(total_pcp)-1; i++)  /* step through blanking values */
	    {
		/* get lower 5 blanking bits and set STOPCT* bit */
		blankingvalue = 0x8000 | (wvfm -> blank_chip[i] & 0x001f);
		if (wvfm -> blank_chip[i] & 0x0020)
		  blankingvalue=blankingvalue|0x0C00; /*shift pk*/
		*blankloc = blankingvalue;
/*		printf("writing blanking value of %4x to location %8x\n",blankingvalue,blankloc); */ 

		blankloc++;
	    }
	  blankingvalue = wvfm -> blank_chip[i] & 0x001f; /* load lower five bits, with
						     STOPCT* = 0; */
	  if (wvfm -> blank_chip[i] & 0x0020)
	    blankingvalue=blankingvalue|0xC00;    /* shift PK bit */
	  *blankloc = blankingvalue;
/*	  printf("writing blanking value of %4x to location %8x\n",blankingvalue,blankloc); */

      }

/* NOW PROGRAM TIMING MODULE WITH APPROPRIATE GATE PARAMETERS */
if(!module) /* module = 0 for new timing module; 1 for old timing module */
  {
    timgate0   = (unsigned short *)(TIMBASE + TIMGATE0); /* Gate RAM 0 */
    timgate1   = (unsigned short *)(TIMBASE + TIMGATE1); /* Gate RAM 1 */
    timgate2   = (unsigned short *)(TIMBASE + TIMGATE2); /* Gate RAM 2 */
    timgate3   = (unsigned short *)(TIMBASE + TIMGATE3); /* Gate RAM 3 */
    timgate4   = (unsigned short *)(TIMBASE + TIMGATE4); /* Gate RAM 4 */
   }
else
   { 
    timgate0   = (unsigned short *)(TIMBASE - STD_BASE + EXTD_BASE + TIMGATE0); /* Gate RAM 0 */
    timgate1   = (unsigned short *)(TIMBASE - STD_BASE + EXTD_BASE + TIMGATE1); /* Gate RAM 1 */
    timgate2   = (unsigned short *)(TIMBASE - STD_BASE + EXTD_BASE + TIMGATE2); /* Gate RAM 2 */
    timgate3   = (unsigned short *)(TIMBASE - STD_BASE + EXTD_BASE + TIMGATE3); /* Gate RAM 3 */
    timgate4   = (unsigned short *)(TIMBASE - STD_BASE + EXTD_BASE + TIMGATE4); /* Gate RAM 4 */
   }

/* Parse Header for Required Parameters */

    for (i=0; i<6; i++)
      num_gates[i] = (unsigned short)(wvfm -> num_gates[i]);
    for (i=0; i<2; i++)
      {
	  gate_dist1[i] = (unsigned short)(wvfm -> gate_dist1[i]);
	  gate_dist2[i] = (unsigned short)(wvfm -> gate_dist2[i]);
	  gate_dist3[i] = (unsigned short)(wvfm -> gate_dist3[i]);
	  gate_dist4[i] = (unsigned short)(wvfm -> gate_dist4[i]);
	  gate_dist5[i] = (unsigned short)(wvfm -> gate_dist5[i]);
      }

/* Perform Error Detection of Header Values */

    /* check for too many gates in a PRT */

    gate_tm = num_gates[0]*gate_dist1[1]/60000000.0+gate_dist1[0]/60000000.0;
    if(!Silent)
      printf("freq1 gate_tm = %f \n",gate_tm);
    if(gate_tm  > 0.98 * short_prt)
      {
	  if(!Silent)
	    printf("ld_timmod: gates fill more than 98%% of a prt.\n");
	  return;
      }
    gate_tm = num_gates[1]*gate_dist2[1]/60000000.0+gate_dist2[0]/60000000.0;
    printf("freq2 gate_tm = %f \n",gate_tm);
    if(gate_tm  > 0.98 * short_prt)
      {
	  if(!Silent)
	    printf("ld_timmod: gates fill more than 98%% of a prt.\n");
	  return;
      }
    gate_tm = num_gates[2]*gate_dist3[1]/60000000.0+gate_dist3[0]/60000000.0;
    if(!Silent)
      printf("freq3 gate_tm = %f \n",gate_tm);
    if(gate_tm  > 0.98 * short_prt)
      {
	  if(!Silent)
	    printf("ld_timmod: gates fill more than 98%% of a prt.\n");
	  return;
      }
    gate_tm = num_gates[3]*gate_dist4[1]/60000000.0+gate_dist4[0]/60000000.0;
    if(!Silent)
      printf("freq4 gate_tm = %f \n",gate_tm);
    if(gate_tm  > 0.98 * short_prt)
      {
	  if(!Silent)
	    printf("ld_timmod: gates fill more than 98%% of a prt.\n");
	  return;
      }
    gate_tm = num_gates[4]*gate_dist5[1]/60000000.0+gate_dist5[0]/60000000.0;
    if(!Silent)
      printf("freq5 gate_tm = %f \n",gate_tm);
    if(gate_tm  > 0.98 * short_prt)
      {
	  if(!Silent)
	    printf("ld_timmod: gates fill more than 98%% of a prt.\n");
	  return;
      }


    for (i=0; i<5; i++)
      /* check for reasonable number of gates */
      if (num_gates[i] > 1024 || num_gates[i] < 0)
	{
	    if(!Silent)
	      {
		  printf("ld_timmod: Requested number of gates [%d]",num_gates[i]);
		  printf("for frequency %d\n",i);
		  printf("             is not between 0 and 1024.\n");
		  printf("ld_timmod: Aborting without programming timmod...\n\n");
	      }
	    return;
	}
   
    /* check for unreasonable first gates */
    if(num_gates[0])
      {
	  if ((gate_dist1[0] < 8) || (gate_dist1[0] > 2046))
	    {
		if(!Silent)
		  {
		      printf("ld_timmod: first gate must be between 8 counts and 2046 counts.\n");
		      printf("ld_timmod: requested first gate for freq 1 was %d counts.\n",gate_dist1[0]);
		      printf("ld_timmod: aborting without programming timmod...\n\n");
		  }
		return;
	    }
      }
    if (num_gates[1])
      {
	  if ((gate_dist2[0] < 8) || (gate_dist2[0] > 2046))
	    {
		if(!Silent)
		  {
		      printf("ld_timmod: first gate spacing must be between 8 counts and 2046 counts.\n");
		      printf("ld_timmod: requested first gate for freq 2 was %d counts.\n",gate_dist2[0]);
		      printf("ld_timmod: aborting without programming timing module...\n\n");
		  }
		return;
	    }
      }
    if (num_gates[2])
      {
	  if ((gate_dist3[0] < 8) || (gate_dist3[0] > 2046))
	    {
		if(!Silent)
		  {
		      printf("ld_timmod: first gate must be between 8 counts and 2046 counts.\n");
		      printf("ld_timmod: requested first gate for freq 3 was %d counts.\n",gate_dist3[0]);
		      printf("ld_timmod: aborting without programming timing module...\n\n");
		  }
		return;
	    }
      }
    if (num_gates[3])
      {
	  if ((gate_dist4[0] < 8) || (gate_dist4[0] > 2046))
	    {
		if(!Silent)
		  {
		      printf("ld_timmod: first gate must be between 8 counts and 2046 counts.\n");
		      printf("ld_timmod: requested first gate for freq 4 was %d counts.\n",gate_dist4[0]);
		      printf("ld_timmod: aborting without programming timing module...\n\n");
		  }
		return;
	    }
      }
    if (num_gates[4])
      {
	  if ((gate_dist5[0] < 8) || (gate_dist5[0] > 2046))
	    {
		if(!Silent)
		  {
		      printf("ld_timmod: first gate must be between 8 counts and 2046 counts.\n");
		      printf("ld_timmod: requested first gate for freq 5 was %d counts.\n",gate_dist5[0]);
		      printf("ld_timmod: aborting without programming timing module...\n\n");
		  }
		return;
	    }
      }

    /* check for unreasonable gate spacing */
    
    if(num_gates[0])
      {
	  if ((gate_dist1[1] < 8) || (gate_dist1[1] > 800))
	    {
		if(!Silent)
		  {
		      printf("ld_timmod: gate spacing must be between 8 counts and 800 counts.\n");
		      printf("ld_timmod: requested spacing for freq 1 was %d counts.\n",gate_dist1[1]);
		      printf("ld_timmod: aborting without programming timing module...\n\n");
		  }
		return;
	    }
      }
    if (num_gates[1])
      {
	  if ((gate_dist2[1] < 8) || (gate_dist2[1] > 800))
	    {
		if(!Silent)
		  {
		      printf("ld_timmod: gate spacing must be between 8 counts and 800 counts.\n");
		      printf("ld_timmod: requested spacing for freq 2 was %d counts.\n",gate_dist2[1]);
		      printf("ld_timmod: aborting without programming timing module...\n\n");
		  }
		return;
	    }
      }
    if (num_gates[2])
      {
	  if ((gate_dist3[1] < 8) || (gate_dist3[1] > 800))
	    {
		if(!Silent)
		  {
		      printf("ld_timmod: gate spacing must be between 8 counts and 800 counts.\n");
		      printf("ld_timmod: requested spacing for freq 3 was %d counts.\n",gate_dist3[1]);
		      printf("ld_timmod: aborting without programming timing module...\n\n");
		  }
		return;
	    }
      }
    if(num_gates[3])
      {
	  if ((gate_dist4[1] < 8) || (gate_dist4[1] > 800))
	    {
		if(!Silent)
		  {
		      printf("ld_timmod: gate spacing must be between 8 counts and 800 counts.\n");
		      printf("ld_timmod: requested spacing for freq 4 was %d counts.\n",gate_dist4[1]);
		      printf("ld_timmod: aborting without programming timing module...\n\n");
		  }
		return;
	    }
      }
    if (num_gates[4])
      {
	  if ((gate_dist5[1] < 8) || (gate_dist5[1] > 800))
	    {
		if(!Silent)
		  {
		      printf("ld_timmod: gate spacing must be between 8 counts and 800 counts.\n");
		      printf("ld_timmod: requested spacing for freq 5 was %d counts.\n",gate_dist5[1]);
		      printf("ld_timmod: aborting without programming timing module...\n\n");
		  }
		return;
	    }
      }
    
    /* DO GATES */

    /* Load first gate count-up value */
    if(num_gates[0])
      {
	  gatevalue = 0x1003 - gate_dist1[0];
	  *timgate0 = gatevalue;
	  timgate0++;
      }
    if(num_gates[1])
      {
	  gatevalue = 0x1003 - gate_dist2[0];
	  *timgate1 = gatevalue;
	  timgate1++;
      }
    if(num_gates[2])
      {
	  gatevalue = 0x1003 - gate_dist3[0];
	  *timgate2 = gatevalue;
	  timgate2++;
      }
    if(num_gates[3])
      {
	  gatevalue = 0x1003 - gate_dist4[0];
	  *timgate3 = gatevalue;
	  timgate3++;
      }
    if(num_gates[4])
      {
	  gatevalue = 0x1003 - gate_dist5[0];
	  *timgate4 = gatevalue;
	  timgate4++;
      }

    /* Load the rest of the gate count-up values for freq 1*/

    if(num_gates[0])
      {
	  gatevalue = 0x1002 - gate_dist1[1];
	  for (gateindex=1; gateindex<num_gates[0]; gateindex++)
	    {
		*timgate0 = gatevalue;
		timgate0++;
		if(!module) /* module = 1 for new timing module; 0 for old */
		  {
		      if(timgate0 > (unsigned short *) TIMBASE+TIMGATE0+4090)
			{
			    if(!Silent)
			      {
				  printf("ld_timmod: GATE RAM index computation error.\n");
				  printf("ld_timmod: aborting without fully programming timing module.\n\n");
			      }
			    return;
			}
		  }
		else
		  {
		      if(timgate0 > (unsigned short *) TIMBASE-STD_BASE+EXTD_BASE+TIMGATE0+4090)
			{
			    if(!Silent)
			      {
				  printf("ld_timmod: GATE RAM index computation error.\n");
				  printf("ld_timmod: aborting without fully programming timing module.\n\n");
			      }
			    return;
			}
		  }
	    }

	  *timgate0 = 0x0000;    /* Load terminal value */
      }

    /* Load the rest of the gate count-up values for freq 2*/

    if(num_gates[1])
      {
	  gatevalue = 0x1002 - gate_dist2[1];
	  for (gateindex=1; gateindex<num_gates[1]; gateindex++)
	    {
		*timgate1 = gatevalue;
		timgate1++;
		if(!module) /* module = 1 for new timing module; 0 for old */
		  {
		      if(timgate1 > (unsigned short *) TIMBASE+TIMGATE1+4090)
			{
			    if(!Silent)
			      {
				  printf("ld_timmod: GATE RAM index computation error.\n");
				  printf("ld_timmod: aborting without fully programming timing module.\n\n");
			      }
			    return;
			}
		  }
		else
		  {
		      if(timgate1 > (unsigned short *) TIMBASE-STD_BASE+EXTD_BASE+TIMGATE1+4090)
			{
			    if(!Silent)
			      {
				  printf("ld_timmod: GATE RAM index computation error.\n");
				  printf("ld_timmod: aborting without fully programming timing module.\n\n");
			      }
			    return;
			}
		  }		      
	    }
	  *timgate1 = 0x0000;    /* Load terminal value */
      }

    /* Load the rest of the gate count-up values for freq 3*/

    if(num_gates[2])
      {
	  gatevalue = 0x1002 - gate_dist3[1];
	  for (gateindex=1; gateindex<num_gates[2]; gateindex++)
	    {
		*timgate2 = gatevalue;
		timgate2++;
		if(!module) /* 0 for new timing module; 1 for old */
		  {
		      if(timgate2 > (unsigned short *) TIMBASE+TIMGATE2+4090)
			{
			    if(!Silent)
			      {
				  printf("ld_timmod: GATE RAM index computation error.\n");
				  printf("ld_timmod: aborting without fully programming timing module.\n\n");
			      }
			    return;
			}
		  }
		else
		  {
		      if(timgate2 > (unsigned short *) TIMBASE-STD_BASE+EXTD_BASE+TIMGATE2+4090)
			{
			    if(!Silent)
			      {
				  printf("ld_timmod: GATE RAM index computation error.\n");
				  printf("ld_timmod: aborting without fully programming timing module.\n\n");
			      }
			    return;
			}
		  }
	    }
	  *timgate2 = 0x0000;    /* Load terminal value */
      }

    /* Load the rest of the gate count-up values for freq 4*/

    if(num_gates[3])
      {
	  gatevalue = 0x1002 - gate_dist4[1];
	  for (gateindex=1; gateindex<num_gates[3]; gateindex++)
	    {
		*timgate3 = gatevalue;
		timgate3++;
		if(!module) /* 0 for new timing module; 1 for old */
		  {
		      if(timgate3 > (unsigned short *) TIMBASE+TIMGATE3+4090)
			{
			    if(!Silent)
			      {
				  printf("ld_timmod: GATE RAM index computation error.\n");
				  printf("ld_timmod: aborting without fully programming timing module.\n\n");
			      }
			    return;
			}
		  }
		else
		  {
		      if(timgate3 > (unsigned short *) TIMBASE-STD_BASE+EXTD_BASE+TIMGATE3+4090)
			{
			    if(!Silent)
			      {
				  printf("ld_timmod: GATE RAM index computation error.\n");
				  printf("ld_timmod: aborting without fully programming timing module.\n\n");
			      }
			    return;
			}
		  }
	    }
	  *timgate3 = 0x0000;    /* Load terminal value */
      }

    /* Load the rest of the gate count-up values for freq 5*/

    if(num_gates[4])
      {
	  gatevalue = 0x1002 - gate_dist5[1];
	  for (gateindex=1; gateindex<num_gates[4]; gateindex++)
	    {
		*timgate4 = gatevalue;
		timgate4++;
		if(!module)  /* 0 for new timing module; 1 for old */
		  {
		      if(timgate4 > (unsigned short *) TIMBASE+TIMGATE4+4090)
			{
			    if(!Silent)
			      {
				  printf("ld_timmod: GATE RAM index computation error.\n");
				  printf("ld_timmod: aborting without fully programming timing module.\n\n");
			      }
			    return;
			}
		  }
		else
		  {
		      if(timgate4 > (unsigned short *) TIMBASE-STD_BASE+EXTD_BASE+TIMGATE4+4090)
			{
			    if(!Silent)
			      {
				  printf("ld_timmod: GATE RAM index computation error.\n");
				  printf("ld_timmod: aborting without fully programming timing module.\n\n");
			      }
			    return;
			}
		  }
	    }
	  *timgate4 = 0x0000;    /* Load terminal value */
      }
    /* ===========SHOW ACTUAL PROGRAMMED VALUES============== */
    if(!Silent)
      printf("values actually setup by ld_timmod:\n");
    if(num_gates[0])
      if(!Silent)
	printf("  FREQ 1: firstgate = %f[m]  gatespace = %f[m]",gate_dist1[0]*2.5,gate_dist1[1]*2.5);
    if(num_gates[1])
      if(!Silent)
	printf("  FREQ 2: firstgate = %f[m]  gatespace = %f[m]",gate_dist2[0]*2.5,gate_dist2[1]*2.5);
    if(num_gates[2])
      if(!Silent)
	printf("  FREQ 3: firstgate = %f[m]  gatespace = %f[m]",gate_dist3[0]*2.5,gate_dist3[1]*2.5);
    if(num_gates[3])
      if(!Silent)
	printf("  FREQ 4: firstgate = %f[m]  gatespace = %f[m]",gate_dist4[0]*2.5,gate_dist4[1]*2.5);
    if(num_gates[4])
      if(!Silent)
	printf("  FREQ 5: firstgate = %f[m]  gatespace = %f[m]",gate_dist5[0]*2.5,gate_dist5[1]*2.5);
    if(!Silent)
      printf("ld_timmod: finished.\n");
}




