/*
 *	$Id$
 *
 *	Module:	update_testpulse	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1996/09/12  17:22:18  craig
 * completed code
 *
 * Revision 1.1  1992/08/25  20:43:04  craig
 * Initial revision
 *
 * description: This module updates the ongoing test pulse calibration.
 *              This routine is called when testpulse_dwell_count is
 *              greater than testpulse_max_count.
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include "hskpAll.h"

void update_testpulse()
{
/* Define some general purpose variables */
unsigned char B, nr;
unsigned long T;
double frequency, temp;
unsigned char unitnum, ecbaddr,test,attenuation, foraft;
int timeout;
float dbz, log_power, received_lna_pwr, injected_lna_pwr, received_vel;
float diff;


/* Zero the testpulse dwell counter */
tp_dwell_count = 0;

/* Check to see if the previous testpulse was within the proper error band */

dbz = ((fore_dbz_sum / tp_sum_count)) - 280.0 / 8.0;
log_power = (dbz - fore_lna_radar_const - log_testpulse_range2) / 10.0;
received_lna_pwr = pow((double)10.0,(double)log_power);
injected_lna_pwr = fore_tp_level_sum / tp_sum_count + 
                   ffrad->test_pulse_offset - fore_vmehndshk->tpulse_atten;
diff = received_lna_pwr - injected_lna_pwr;
if(diff < 0) diff *= -1;
if(diff > MAX_TP_PWR_DIFF) printf("FP "); 
received_vel = ((fore_vel_sum / tp_sum_count) - 8191.0) / f_vscale;
diff = received_vel - fore_tp_velocity[tp_freq_offset_count];
if(diff < 0) diff *= -1;
if(diff > MAX_TP_VEL_DIFF) printf("FV "); 

dbz = ((aft_dbz_sum / tp_sum_count)) - 280.0 / 8.0;
log_power = (dbz - aft_lna_radar_const - log_testpulse_range2) / 10.0;
received_lna_pwr = pow((double)10.0,(double)log_power);
injected_lna_pwr = aft_tp_level_sum / tp_sum_count + 
                   afrad->test_pulse_offset - aft_vmehndshk->tpulse_atten;
diff = received_lna_pwr - injected_lna_pwr;
if(diff < 0) diff *= -1;
if(diff > MAX_TP_PWR_DIFF) printf("AP "); 
received_vel = ((aft_vel_sum / tp_sum_count) - 8191.0) / a_vscale;
diff = received_vel - aft_tp_velocity[tp_freq_offset_count];
if(diff < 0) diff *= -1;
if(diff > MAX_TP_VEL_DIFF) printf("AV "); 

tp_sum_count = 0;
fore_dbz_sum = 0;
fore_tp_level_sum = 0;
aft_dbz_sum = 0;
aft_tp_level_sum = 0;
fore_vel_sum = 0;
aft_vel_sum = 0;

/* Now re-program the fore testpulse with a new value */

fore_vmehndshk->tpulse_freq_num = tp_freq_count + 1;
frequency = fore_freqs[tp_freq_count] + 
(double)fore_freq_offset[tp_freq_offset_count];
printf("Fore frequency: %lf\n",frequency);
attenuation = tp_atten;
fore_vmehndshk->tpulse_freq = frequency;
fore_vmehndshk->tpulse_atten = tp_atten;
fore_vmehndshk->tpulse_flg = 1;

/* Set the fore test pulse frequency */

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

/* Set the fore testpulse attenuation level */

ecbaddr = ECBATTEN;
foraft = 1;
timeout=0;
do
  {
      if(timeout > 0)taskDelay(1);
      timeout++;
  }while((test = ecbSetAtten(ecbaddr, foraft, attenuation) != 0) &&
	  timeout < 30);  



/* Now re-program the fore testpulse with a new value */

aft_vmehndshk->tpulse_freq_num = tp_freq_count + 1;
frequency = aft_freqs[tp_freq_count] +
(double)aft_freq_offset[tp_freq_offset_count];
printf("Aft frequency: %lf\n",frequency);
attenuation = tp_atten;
aft_vmehndshk->tpulse_atten = tp_atten;
aft_vmehndshk->tpulse_freq = frequency;
aft_vmehndshk->tpulse_flg = 1;

/* Update the counters for next time */

tp_freq_count += 1;
if((tp_freq_count + 1) > fraddes->num_freq_trans)
{
  tp_freq_count = 0;
  tp_atten += TP_ATTEN_STEP;
  if(tp_atten > TP_ATTEN_MAX)
  {
    tp_atten_start += 1;
    if(tp_atten_start >= TP_ATTEN_STEP)
    tp_atten_start = 0;
    tp_atten = tp_atten_start;

    tp_freq_offset_count += 1;
    if(tp_freq_offset_count > 6)
    tp_freq_offset_count = 0;

  }
}

/* Set the aft test pulse frequency */

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

/* Set the aft testpulse attenuation level */

ecbaddr = ECBATTEN;
foraft = 0;
timeout = 0;

do
  {
      if(timeout > 0)taskDelay(1);
      timeout++;
  }while((test = ecbSetAtten(ecbaddr, foraft, attenuation) != 0) &&
	 timeout < 30);  

}






