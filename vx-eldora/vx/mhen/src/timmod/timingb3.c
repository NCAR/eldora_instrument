/*
 *	$Id$
 *
 *	Module:	TIMINGB3	 
 *	Original Author: Shawn B. Copeland 
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * Revision 1.1  1991/12/04  19:01:58  eric
 * Initial revision
 *
 *
 * description:
 *
  Purpose:       Program ELDORA timing module rev b to generate a single prf
		 on F0,F1 and F2, and a pre-delayed preknock.
		 Also programs N number of uniform-spaced gates on GATE(0),
		 GATE(1) and GATE(2).
  Arguments:     prf        int, prf in Hz.
                 numgates   int, number of gates.
		 gatespace  int, gate spacing in meters.
		 duty       float, duty cycle in percent.
  Date Written:  09/11/92  (copied timing3.c)
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";


#include "ELDRP7.h"
#include "coll_dpramdefs.h"

#define PK_TO_F0_RISE       22  /* in counts  */
#define PK_TO_F0_FALL       19  /* in counts  */

void timingb3(prf,numgates,gatespace,duty)
     
     int prf,numgates,gatespace;
     float duty;
     
{
  float floatgate,gatetime,prt,pulsewidth,realduty;
  unsigned short *timgate0,*timgate1,*timgate2;
  unsigned short *timpcpn,*timblank,*timchip,*timoff;
  unsigned short pcpn,chipcounts,gatecounts;
  unsigned short f0start,f0end,f1start,f1end,f2start,f2end;
  unsigned short pkstart,pkend;
  unsigned short maxindex,chipindex,chipramvalue;
  unsigned short gateindex,gatevalue;

/* Pointers to Collator */
  unsigned long  *col_first,*col_gate;
  col_first  = (unsigned long *) (COL0BASE + COL_RZERO);           /* Col. first-gate   */
  col_gate   = (unsigned long *) (COL0BASE + COL_GATE_SPACE);      /* Col. gate-spacing */

  timgate0   = (unsigned short *)(TIMBASE + TIMGATE0); /* Gate RAM 0 */
  timgate1   = (unsigned short *)(TIMBASE + TIMGATE1); /* Gate RAM 1 */
  timgate2   = (unsigned short *)(TIMBASE + TIMGATE2); /* Gate RAM 2 */
  timpcpn    = (unsigned short *)(TIMBASE + TIMPCPN);  /* N for PCP Counter */
  timblank   = (unsigned short *)(TIMBASE + TIMBLANK); /* Blanking RAM */
  timchip    = (unsigned short *)(TIMBASE + TIMCHIP);  /* Chip RAM */
  timoff     = (unsigned short *)(TIMBASE + TIMOFF);   /* Stop address */

  /* compute derived parameters */
  prt      = (1.0 / prf);
  realduty = duty / 100.0;
  pulsewidth = realduty * prt;
  gatecounts = (float) gatespace / 2.5;

  /* print out usage info at Eric's request */
  printf("Hello, this is timingb3.  Usage:\n");
  printf("timingb3 prf[Hz],numgates,gatespace[m],duty[%%],\n\n");
  printf("values actually passed to timingb3==> %d,%d,%d,%f\n",prf,numgates,gatespace,duty);
  printf("computed values: prt = %f, realduty = %f, pulsewidth = %f\n\n",prt,realduty,pulsewidth);


  /* do minor error checking */
  if (prf < 250)
    {
      printf("timingb3: prf must be greater than 250 Hz.\n");
      printf("timingb3: aborting without programming timing module...\n\n");
      return;
    }
  if (prf > 20000)
    {
      printf("timingb3: requested prf is greater than 20000.\n");
      printf("timingb3: aborting without programming timing module...\n\n");
      return;
    }
  /* check for reasonable number of gates */
  if (numgates > 1024)
    {
      printf("timingb3: requested number of gates is greater than 1024.\n");
      printf("timingb3: aborting without programming timing module...\n\n");
      return;
    }
  /* check for unreasonable gate spacing */
  if ((gatespace < 30) | (gatespace > 1200))
    {
      printf("timingb3: gate spacing must be between 30 and 1200 meters.\n");
      printf("timingb3: requested spacing was %d meters.\n",gatespace);
      printf("timingb3: aborting without programming timing module...\n\n");
      return;
    }
  /* check for overflow of gates for requested prf */
  gatetime = (numgates * gatespace) / 150000000.0;  /* in seconds */
  if (gatetime > 0.98*prt)
    {
      printf("timingb3: gates fill more than 98%% of a prt.\n");
      printf("timingb3: time for all gates = %f usec\n",gatetime*1000000);
      printf("timingb3: time in one prf    = %f usec\n",prt*1000000);
      printf("timingb3: aborting without programming timing module...\n\n");
      return;
    }
  /* check for duty cycle > 1% */
  if (duty > 1.0)
    {
      printf("timingb3: duty cycle must not be more than 1%%.\n");
      printf("timingb3: requested duty cycle = %f%%.\n",duty);
      printf("timingb3: aborting without programming timing module...\n\n");
      return;
    } 
  /* check for > 10 usec pulse */
  if (pulsewidth > 0.000010)
    {
      printf("timingb3: requested pulsewidth is more than 10 usec.\n");
      printf("timingb3: pulsewidth = %f usec\n",pulsewidth*1000000);
      printf("timingb3: aborting without programming timing module...\n\n");
      return;
    }

  *timoff = 0x0000;  /* shut off module */

  /* write gate spacing in km (in at&t floating point) to collator DPRAM */
  floatgate  = (float) gatespace/1000;
  *col_first = i3e2dsp(floatgate);
  *col_gate  = i3e2dsp(floatgate);

  /* Compute N for needed PCP, write it to timing module */
  pcpn = 15000000.0 / prf;
  pcpn = pcpn - 1;
  *timpcpn = pcpn;

  /* Write Blanking RAM sequence */
  *timblank = (unsigned short) 0x0407;

  /* ======================DO CHIPS======================= */
  /* quantize chipwidth (for one of the three chips) */
  chipcounts = realduty * 20000000.0 / prf;
  /* compute start and end of pre-knock, f0, f1, and f2 */
  pkstart = 1;
  pkend   = pkstart + chipcounts*3 + PK_TO_F0_RISE - PK_TO_F0_FALL;
  f0start = pkstart + PK_TO_F0_RISE;
  f0end   = f0start + chipcounts;
  f1start = f0end;
  f1end   = f1start + chipcounts;
  f2start = f1end;
  f2end   = f2start + chipcounts;

  /* find end of sequence */
  maxindex = pkend;
  if (f0end > maxindex) maxindex = f0end;
  if (f1end > maxindex) maxindex = f1end;
  if (f2end > maxindex) maxindex = f2end;

  /* error checking */
  if (maxindex > 4090)
    {
      printf("timingb3: index computation error.  CHIP RAM bounds exceeded.\n");
      printf("timingb3: aborting without fully programming timing module.\n\n");
      return;
    }

  /* Load chip RAM */
  *timchip = 0x8000;  /* Load initial, special value */
  timchip++;
  for (chipindex = 1; chipindex<maxindex; chipindex++)
    {
      chipramvalue = 0x8000;
      if ( (chipindex >= pkstart) & (chipindex < pkend) )
	chipramvalue = chipramvalue | 0x0400;
      if ( (chipindex >= f0start) & (chipindex < f0end) )
	chipramvalue = chipramvalue | 0x0001;
      if ( (chipindex >= f1start) & (chipindex < f1end) )
	chipramvalue = chipramvalue | 0x0002;
      if ( (chipindex >= f2start) & (chipindex < f2end) )
	chipramvalue = chipramvalue | 0x0004;
      *timchip = chipramvalue;
      timchip++;
      if (timchip > (unsigned short *) TIMBASE+TIMCHIP+4090)
	{
	  printf("timingb3: CHIP RAM index computation error.\n");
	  printf("timingb3: aborting without fully programming timing module.\n\n");
	  return;
	}
    }
  /* Write terminal value (twice, as required by timing module) */
  *timchip = 0x0000;
  timchip++;
  *timchip = 0x0000;

  /* ======================DO GATES======================= */
  /* Load first gate count-up value */
  gatevalue = 0x1003 - (2*gatecounts);
  *timgate0 = gatevalue;
  *timgate1 = gatevalue;
  *timgate2 = gatevalue;
  timgate0++;
  timgate1++;
  timgate2++;
  /* Load the rest of the gate count-up values */
  gatevalue = 0x1002 - gatecounts;
  for (gateindex=1; gateindex<numgates; gateindex++)
    {
	*timgate0 = gatevalue;
	*timgate1 = gatevalue;
	*timgate2 = gatevalue;
	timgate0++;
	if(timgate0 > (unsigned short *) TIMBASE+TIMGATE0+4090)
	  {
	      printf("timingb3: GATE RAM index computation error.\n");
	      printf("timingb3: aborting without fully programming timing module.\n\n");
	      return;
	  }
	timgate1++;
	if(timgate1 > (unsigned short *) TIMBASE+TIMGATE1+4090)
	  {
	      printf("timingb3: GATE RAM index computation error.\n");
	      printf("timingb3: aborting without fully programming timing module.\n\n");
	      return;
	  }
	timgate2++;
	if(timgate2 > (unsigned short *) TIMBASE+TIMGATE2+4090)
	  {
	      printf("timingb3: GATE RAM index computation error.\n");
	      printf("timingb3: aborting without fully programming timing module.\n\n");
	      return;
	  }
    }

  /* Load terminal value */
  *timgate0 = 0x0000;
  *timgate1 = 0x0000;
  *timgate2 = 0x0000;

  /* ===========SHOW ACTUAL PROGRAMMED VALUES============== */
  printf("values actually setup by timingb3:\n");
  printf("  prf       = %f\n  numgates  = %d\n",15000000.0/(pcpn+1),numgates);
  printf("  gatespace = %f\n  duty      = %f%%\n",gatecounts*2.5,duty);
	    
printf("timingb3: finished.\n");
  
}





