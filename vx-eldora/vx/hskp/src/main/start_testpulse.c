/*
 *	$Id$
 *
 *	Module:	start_testpulse	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/25  20:43:04  craig
 * Initial revision
 *
 * description: This module starts the slow testpulse sequence up at
 *              a known point after receipt of a start command.
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include "hskpAll.h"

void start_testpulse()
{
long distance, width; 

/* First reset all of the global variables that keep track of where we
   are in a given test pulse sequence */

tp_atten_start = 0;
tp_atten = 0;
tp_freq_count = 0;
tp_freq_offset_count = 0;

/* Calculate the distance to halfway through the second furthest gate
   and the twice the gate width (2.5 meters per 60 MHz clock period) */

distance = 2.5 * (wave->gate_dist1[0] +
		  (wave->num_gates[0] - 1.5) * wave->gate_dist1[1]); 
width = wave->gate_dist1[1] * 2.5 * 2;

/* Now round down the distance to an integer number of 15 Mhz periods
   (10 meters in a 15 Mhz interval) */

distance = floor((double)(distance/10)) * 10;

/* Now round up the width to an integer number of 15 Mhz periods
   (10 meters in a 15 Mhz interval) */

width = floor(((double)(width/10)) + 1) * 10;

fore_vmehndshk->tpulse_dist = distance;
fore_vmehndshk->tpulse_width = width;
aft_vmehndshk->tpulse_dist = distance;
aft_vmehndshk->tpulse_width = width;

/* Clear the testpulse handshake flag back to zero */
fore_vmehndshk->tpulse_flg = 0;
aft_vmehndshk->tpulse_flg = 0;

/* For now just set the test pulse level and let it be, we will measure it
   at a later date */

fore_vmehndshk->tpulse_level = 2.0;
aft_vmehndshk->tpulse_level = 2.0;

/* Calculate the offset in frequency that will cause 1/2 of a nyquist
interval, use this as an offset from the transmitted frequency */

fore_half_nyquist[0][0] = fraddes->freq1 * 1.e9
  + 1/(fraddes->interpulse_per1 * .004);
fore_half_nyquist[0][1] = fraddes->freq1 * 1.e9
   - 1/(fraddes->interpulse_per1 * .004);
fore_half_nyquist[1][0] = fraddes->freq2 * 1.e9
   + 1/(fraddes->interpulse_per1 * .004);
fore_half_nyquist[1][1] = fraddes->freq2 * 1.e9
   - 1/(fraddes->interpulse_per1 * .004);
fore_half_nyquist[2][0] = fraddes->freq3 * 1.e9
   + 1/(fraddes->interpulse_per1 * .004);
fore_half_nyquist[2][1] = fraddes->freq3 * 1.e9
   - 1/(fraddes->interpulse_per1 * .004);
fore_half_nyquist[3][0] = fraddes->freq4 * 1.e9
   + 1/(fraddes->interpulse_per1 * .004);
fore_half_nyquist[3][1] = fraddes->freq4 * 1.e9
   - 1/(fraddes->interpulse_per1 * .004);
fore_half_nyquist[4][0] = fraddes->freq5 * 1.e9
   + 1/(fraddes->interpulse_per1 * .004);
fore_half_nyquist[4][1] = fraddes->freq5 * 1.e9
   - 1/(fraddes->interpulse_per1 * .004);

aft_half_nyquist[0][0] = araddes->freq1 * 1.e9
   + 1/(araddes->interpulse_per1 * .004);
aft_half_nyquist[0][1] = araddes->freq1 * 1.e9
   - 1/(araddes->interpulse_per1 * .004);
aft_half_nyquist[1][0] = araddes->freq2 * 1.e9
   + 1/(araddes->interpulse_per1 * .004);
aft_half_nyquist[1][1] = araddes->freq2 * 1.e9
   - 1/(araddes->interpulse_per1 * .004);
aft_half_nyquist[2][0] = araddes->freq3 * 1.e9
   + 1/(araddes->interpulse_per1 * .004);
aft_half_nyquist[2][1] = araddes->freq3 * 1.e9
   - 1/(araddes->interpulse_per1 * .004);
aft_half_nyquist[3][0] = araddes->freq4 * 1.e9
   + 1/(araddes->interpulse_per1 * .004);
aft_half_nyquist[3][1] = araddes->freq4 * 1.e9
   - 1/(araddes->interpulse_per1 * .004);
aft_half_nyquist[4][0] = araddes->freq5 * 1.e9
   + 1/(araddes->interpulse_per1 * .004);
aft_half_nyquist[4][1] = araddes->freq5 * 1.e9
   - 1/(araddes->interpulse_per1 * .004);

/* Set the number of dwell times to wait between handling the testpulse */

testpulse_max_count = 10000.0/dwelltime_msec;
tp_dwell_count = testpulse_max_count;

}
