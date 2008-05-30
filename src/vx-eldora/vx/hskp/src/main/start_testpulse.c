
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
 * Revision 1.5  1999/09/27  15:42:20  eric
 * modified to fix slow cal.
 *
 * Revision 1.4  1996/10/29  22:53:33  craig
 * *** empty log message ***
 *
 * Revision 1.3  1996/09/12  17:22:02  craig
 * Completed code
 *
 * Revision 1.2  1994/05/20  20:37:28  craig
 * *** empty log message ***
 *
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
float lambda;
unsigned char B, nr;
unsigned long T;
double frequency, temp;
unsigned char unitnum, ecbaddr,test,attenuation, foraft;
int timeout;

/* First reset all of the global variables that keep track of where we
   are in a given test pulse sequence */

Firstime = 1;
tp_atten_start = 0;
tp_atten = TP_ATTEN_ST;
tp_freq_count = 0;
tp_freq_offset_count = 0;
tp_sum_count = 0;
fore_dbz_sum = 0;
fore_tp_level = 0;
aft_dbz_sum = 0;
aft_tp_level = 0;
fore_vel_sum = 0;
aft_vel_sum = 0;

/* Calculate the radar constant for power at LNA, instead of the antenna */
fore_lna_radar_const = fraddes->radar_const + ffrad->loss_in + 
                       ffrad->loss_rjoint;
aft_lna_radar_const = araddes->radar_const + afrad->loss_in + 
                       afrad->loss_rjoint;
/* Calculate the distance to halfway through the second furthest gate
   and three time the gate width (2.5 meters per 60 MHz clock period) */

/* distance = 2.5 * (wave->gate_dist1[0] +
		  (wave->num_gates[0] - 2) * wave->gate_dist1[1]); 
		  */
distance = 2.5 * (wave->gate_dist1[0] +
		  (wave->num_gates[0] - 4) * wave->gate_dist1[1]); 
/* width = wave->gate_dist1[1] * 2.5 * 3; try larger pulse */
width = wave->gate_dist1[1] * 2.5 * 5;


/* Now round down the distance to an integer number of 15 Mhz periods
   (10 meters in a 15 Mhz interval) */

distance = floor((double)(distance/10)) * 10;

/* Now round up the width to an integer number of 15 Mhz periods
   (10 meters in a 15 Mhz interval) */

width = floor(((double)(width/10)) + 1) * 10;

fore_vmehndshk->tpulse_dist = distance;
fore_vmehndshk->tpulse_width = width;
fore_vmehndshk->tpulse.freq_num[0] = tp_freq_count + 1; 
aft_vmehndshk->tpulse_dist = distance;
aft_vmehndshk->tpulse_width = width;
aft_vmehndshk->tpulse.freq_num[0] = tp_freq_count + 1;

/* Calculate the log value of the range to last gate squared */
/* temp = (width/3 + distance) / 1000.0; */
/* temp = (width/5 + distance) / 1000.0; */
/* Use actual radar processor correction! */
temp = (2.5 * wave->num_gates[0] * wave->gate_dist1[1])/1000.0;
log_testpulse_range2 = 20 * log10(temp);

/* Clear the testpulse handshake flag back to zero */
fore_vmehndshk->tpulse_flg = 0;
aft_vmehndshk->tpulse_flg = 0;

/* Calculate the offset in frequency that will cause 0, 1/4, 1/2, and 3/4
   of a nyquist interval, this will be used as an offset from the transmitted
   frequency */

lambda = 0.2997925/fraddes->freq1;  /* gives wavelength in meters */
fore_freq_offset[0] = 0;
fore_tp_velocity[0] = 0;
fore_freq_offset[1] = 0.25/(fraddes->interpulse_per1 * .002);
fore_tp_velocity[1] = -fore_freq_offset[1] * lambda / 2;
fore_freq_offset[2] = 0.5/(fraddes->interpulse_per1 * .002);
fore_tp_velocity[2] = -fore_freq_offset[2] * lambda / 2;
fore_freq_offset[3] = 0.75/(fraddes->interpulse_per1 * .002);
fore_tp_velocity[3] = -fore_freq_offset[3] * lambda / 2;
fore_freq_offset[4] = -0.25/(fraddes->interpulse_per1 * .002);
fore_tp_velocity[4] = -fore_freq_offset[4] * lambda / 2;
fore_freq_offset[5] = -0.5/(fraddes->interpulse_per1 * .002);
fore_tp_velocity[5] = -fore_freq_offset[5] * lambda / 2;
fore_freq_offset[6] = -0.75/(fraddes->interpulse_per1 * .002);
fore_tp_velocity[6] = -fore_freq_offset[6] * lambda / 2;

/* Calculate the scale factor that the velocities will be scaled by the radar
   processors with - remember we will always get VS if there is dual PRT, 
   interpulse_per1 is guarenteed to be the one for VS */

f_vscale = 32764.0 / (lambda * 1000. / fraddes->interpulse_per1);
 

lambda = 0.2997925/araddes->freq1;  /* gives wavelength in meters */
aft_freq_offset[0] = 0;
aft_tp_velocity[0] = 0;
aft_freq_offset[1] = 0.25/(araddes->interpulse_per1 * .002);
aft_tp_velocity[1] = -aft_freq_offset[1] * lambda / 2;
aft_freq_offset[2] = 0.5/(araddes->interpulse_per1 * .002);
aft_tp_velocity[2] = -aft_freq_offset[2] * lambda / 2;
aft_freq_offset[3] = 0.75/(araddes->interpulse_per1 * .002);
aft_tp_velocity[3] = -aft_freq_offset[3] * lambda / 2;
aft_freq_offset[4] = -0.25/(araddes->interpulse_per1 * .002);
aft_tp_velocity[4] = -aft_freq_offset[4] * lambda / 2;
aft_freq_offset[5] = -0.5/(araddes->interpulse_per1 * .002);
aft_tp_velocity[5] = -aft_freq_offset[5] * lambda / 2;
aft_freq_offset[6] = -0.75/(araddes->interpulse_per1 * .002);
aft_tp_velocity[6] = -aft_freq_offset[6] * lambda / 2;

/* Calculate the scale factor that the velocities will be scaled by the radar
   processors with - remember we will always get VS if there is dual PRT, 
   interpulse_per1 is guarenteed to be the one for VS */

a_vscale = 32764.0 / (lambda * 1000. / araddes->interpulse_per1);
 

/* Set the number of dwell times to wait between handling the testpulse */

testpulse_max_count = 10000.0/dwelltime_msec; /*New Calibration every 10 sec */
tp_dwell_count = testpulse_max_count;

/* Set the starting and ending dwell count to sum over */
/* tp_sum_start = 2000.0/dwelltime_msec; *//* start 2 seconds into calibration */
/* tp_sum_end = 9000.0/dwelltime_msec; *//* end 9 seconds into calibration */

/* See if delaying testpulse sum makes forward tp more stable */
tp_sum_start = 3000.0/dwelltime_msec; /* start 3 seconds into calibration */
tp_sum_end = 9000.0/dwelltime_msec; /* end 8 seconds into calibration */

//********************************************************************************\
// Tom 5/30/08 DDS and Attenuators now programmed in DRX
/*

// Program Testpulse DDS's to be way out of band for dc removal to work correctly 

frequency = 9310000000;
attenuation = 110;

// Set the fore test pulse frequency 

ecbaddr = ECBRFFOR;
B = (unsigned char)(361 - (int)(frequency / 30.0e6));
temp = 15445.3333333333333 - (frequency / 703125.0);
temp = (16777216.0 / (B+1)) * temp;
T = (unsigned long)(temp + 0.5);
unitnum = 6;
nr = 1;
timeout = 0;
do
  {
      if(timeout > 0)taskDelay(1);
      timeout++;
  }while((test = ecbSetDDS(ecbaddr,unitnum,B,T,nr) != 0) &&
	 timeout < 30);

// Set the fore testpulse attenuation level 

ecbaddr = ECBATTEN;
foraft = 1;
timeout=0;
do
  {
      if(timeout > 0)taskDelay(1);
      timeout++;
  }while((test = ecbSetAtten(ecbaddr, foraft, attenuation) != 0) &&
	  timeout < 30);  

// Set the aft test pulse frequency 

ecbaddr = ECBRFAFT;
B = (unsigned char)(361 - (int)(frequency / 30.0e6));
temp = 15445.3333333333333 - (frequency / 703125.0);
temp = (16777216.0 / (B+1)) * temp;
T = (unsigned long)(temp + 0.5);
unitnum = 6;
nr = 1;
timeout = 0;
do
  {
      if(timeout > 0)taskDelay(1);
      timeout++;
  }while((test = ecbSetDDS(ecbaddr,unitnum,B,T,nr) != 0) &&
	 timeout < 30);

// Set the aft testpulse attenuation level 

ecbaddr = ECBATTEN;
foraft = 0;
timeout = 0;

do
  {
      if(timeout > 0)taskDelay(1);
      timeout++;
  }while((test = ecbSetAtten(ecbaddr, foraft, attenuation) != 0) &&
	 timeout < 30);  
*/
//********************************************************************

}
