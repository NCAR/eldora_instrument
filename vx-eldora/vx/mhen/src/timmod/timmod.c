/*
 *	$Id$
 *
 *	Module:	TIMMOD	 
 *	Original Author: Shawn B. Copeland 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:
  Purpose:       Load timing module for ELDORA RP7
  Arguments:     prf        int, prf in Hz.
                 numgates   int, number of gates.
		 gatespace  int, gate spacing in meters.
		 firstgate  int, distance from mid of xmit pulse to first gate.
		 duty       float, duty cycle in percent.
		 tp_pos     int, distance to test pulse in meters.

  Date Written:  12/11/90
  Revised:       12/13/90  by SBC to use load_waveform() and load_gates()
                 12/17/90  by SBC to generate gate array values.
		 02/28/91  by SBC to make duty cycle parametric.
		 03/04/91  by SBC to program Freq. One as test pulse.
		 11/01/91  by SBC to make test pulse distance parametric.
		 11/04/91  by SBC to write gate spacing to collator DPRAM.
		 11/06/91  by SBC: removed part that turned the module "on".
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";



#include "/ELDORA/vx/mhen/include/ELDRP7.h"
#include "/ELDORA/dsp/include/col_dpramdefs.h"

#define PRE_KNOCK_RISE 250.0  /* in nsec    */
#define PRE_KNOCK_FALL 200.0  /* in nsec    */
#define RF_SWITCH_RISE  35.0  /* in nsec    */
#define RF_SWITCH_FALL  35.0  /* in nsec    */

/* #define TP_POS          60.0 */ /* in usec    */

void timmod(prf,numgates,gatespace,firstgate,duty,tp_pos)
     
     int prf,numgates,gatespace,firstgate,tp_pos;
     float duty;
     
{
  short num_chips[6],chip_offset[6],chip_width[6];
  short total_pcp,blank_chip[MAXPCP];
  short freq,gate,chipoff,tpoff,chipcounts,pkcounts;
  unsigned char uniform_gates[MAXRADARS];
  unsigned short cell_total[MAXRADARS],cell_dist[MAXRADARS][MAXGATES];
  float repeat_seq,pulsewidth;
  float floatgate,floatfirst;
  unsigned long *col_first,*col_gate;
  
  unsigned short *timoff;

  col_first = (unsigned long *) COL_RZERO;
  col_gate  = (unsigned long *) COL_GATE_SPACE;
  
  timoff      = (unsigned short *)(TIMBASE + TIMOFF); /* stop module */

  /* do minor error checking */
  if (tp_pos > 9000)
    {
      printf("timmod: test pulse position must be less than 9000 meters.\n");
      printf("timmod: aborting without programming timing module...\n\n");
      return;
    }

  /* print out usage info at Eric's request */
  printf("Hello, this is timmod. ");
  printf("Here are the parameters expected by timmod:\n");
  printf("timmod prf,numgates,gatespace,firstgate,duty,tp_pos\n\n");
  printf("values actually passed to timmod==> %d,%d,%d,%d,%f,%d\n",prf,numgates,gatespace,firstgate,duty,tp_pos);

  /* write gate spacing (in at&t floating point) to collator DPRAM */
  
  floatfirst = (float) firstgate/1000;
  floatgate  = (float) gatespace/1000;
  *col_first = i3e2dsp(floatfirst);
  *col_gate  = i3e2dsp(floatgate);

  *timoff = 0x0000;  /* shut off module */
  
  /* DO CHIPS AND BLANKING */
  chipoff = (PRE_KNOCK_RISE - RF_SWITCH_RISE) / 16.6666666;
  pulsewidth = ( duty * 10000000 / (float) prf);
  chipcounts = (pulsewidth+RF_SWITCH_RISE-RF_SWITCH_FALL) / 16.6666666;
  tpoff = (tp_pos / 2.5);

  /* Frequency zero chip parameters */
  num_chips[0] = 1;
  chip_offset[0] = chipoff;
  chip_width[0] = chipcounts;
  /* Frequency one chip parameters */
  num_chips[1] = 1;
  chip_offset[1] = tpoff;
  chip_width[1] = chipcounts;
  /* Frequency two chip parameters */
  num_chips[2] = 1;
  chip_offset[2] = chipoff;
  chip_width[2] = chipcounts;
  /* Frequency three chip parameters */
  num_chips[3] = 1;
  chip_offset[3] = chipoff;
  chip_width[3] = chipcounts;
  /* Frequency four chip parameters */
  num_chips[4] = 1;
  chip_offset[4] = chipoff;
  chip_width[4] = chipcounts;
  
  /* Pre-knock parameters */
  num_chips[5] = 1;
  chip_offset[5] = 0;
  chip_width[5] = (pulsewidth+PRE_KNOCK_RISE-PRE_KNOCK_FALL) / 16.6666666;
  
  blank_chip[0] = 0x23;
  repeat_seq = 1000.0/(float) prf;
  total_pcp = 1;
  
  load_waveform(num_chips,chip_offset,chip_width,&total_pcp,blank_chip,&repeat_seq);
  
  
  /* DO GATES */
  firstgate = (unsigned short)(0.4 * firstgate); /* convert to counts */
  gatespace = (unsigned short)(0.4 * gatespace); /* convert to counts */

  for (freq=0; freq<MAXRADARS; freq++)
    {
      uniform_gates[freq] = 1;     /* assume uniform gates everywhere */
      cell_total[freq] = numgates;
      cell_dist[freq][0] = firstgate; /* Load first gate */
      cell_dist[freq][1] = gatespace; /* Load the rest of the gates */
    }
  load_gates(uniform_gates,cell_total,cell_dist);

printf("\ntimmod: finished.\n");
  
}

