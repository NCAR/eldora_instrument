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
 * Revision 1.6  1999/09/27  15:43:23  eric
 * modified to fix slow cal.
 *
 * Revision 1.5  1997/08/20  17:09:33  eric
 * corrected log power computation.
 * modified testpulse frequency so that only doppler offset
 * is recorded on tape.
 *
 * Revision 1.4  1996/10/29  22:53:43  craig
 * *** empty log message ***
 *
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
float diff, tdiff;


/* Zero the testpulse dwell counter */
tp_dwell_count = 0;

if(!Firstime)
  {
/* Check to see if the previous testpulse was within the proper error band */

dbz = (((fore_dbz_sum / tp_sum_count)) - 280.0) / 8.0;
log_power = dbz + fore_lna_radar_const - log_testpulse_range2;
received_lna_pwr = log_power;
injected_lna_pwr = fore_tp_level - fore_vmehndshk->tpulse_atten + ffrad->test_pulse_offset + 0.3;

tdiff = diff = received_lna_pwr - injected_lna_pwr;
/* printf("TD %2.2f",tdiff); */
if(diff < 0) diff *= -1;
if(diff > MAX_TP_PWR_DIFF) 
  {
    printf("ATF = %d\n",fore_vmehndshk->tpulse_atten);
    printf("FP%d: %2.2f\n",tp_freq_count+1,tdiff);
/*    printf("Dbz = %f; logP = %f\n",dbz,received_lna_pwr); */
/*    printf("Pwr = %f\n",injected_lna_pwr); */
  }
received_vel = ((fore_vel_sum / tp_sum_count) - 8191.0) / f_vscale;
diff = received_vel - fore_tp_velocity[tp_freq_offset_count];
/* printf("Vel = %f\n",received_vel); */
if(diff < 0) diff *= -1;
if(diff > MAX_TP_VEL_DIFF) printf("FV%d %1.2f",tp_freq_count+1,diff); 

dbz = (((aft_dbz_sum / tp_sum_count)) - 280.0) / 8.0;
log_power = dbz + aft_lna_radar_const - log_testpulse_range2;
received_lna_pwr = log_power;
injected_lna_pwr = aft_tp_level - aft_vmehndshk->tpulse_atten + afrad->test_pulse_offset + 0.4;
tdiff = diff = received_lna_pwr - injected_lna_pwr;
if(diff < 0) diff *= -1;
if(diff > MAX_TP_PWR_DIFF) 
   {
     printf("ATA = %d\n",aft_vmehndshk->tpulse_atten);
     printf("AP%d: %2.2f\n",tp_freq_count+1,tdiff); 
/*	printf("Dbz = %f; logP = %f\n",dbz,received_lna_pwr); */
/*	printf("Pwr = %f\n",injected_lna_pwr); */
   }
received_vel = ((aft_vel_sum / tp_sum_count) - 8191.0) / a_vscale;
/* printf("Vel = %f\n",received_vel); */
diff = received_vel - aft_tp_velocity[tp_freq_offset_count];
if(diff < 0) diff *= -1;
if(diff > MAX_TP_VEL_DIFF) printf("AV%d  %1.2f",tp_freq_count+1,diff); 

tp_sum_count = 0;
fore_dbz_sum = 0;
fore_tp_level = 0;
aft_dbz_sum = 0;
aft_tp_level = 0;
fore_vel_sum = 0;
aft_vel_sum = 0;


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

  }
/* Now re-program the fore testpulse with a new value */

fore_vmehndshk->tpulse.freq_num[0] = tp_freq_count + 1;
frequency = fore_freqs[tp_freq_count] +
(double)fore_freq_offset[tp_freq_offset_count];
/* printf("Fore frequency: %lf\n",frequency); */
attenuation = tp_atten;
fore_vmehndshk->tpulse_freq = fore_freq_offset[tp_freq_offset_count]; /* use offset */
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


/* Now re-program the aft testpulse with a new value */

aft_vmehndshk->tpulse.freq_num[0] = tp_freq_count + 1;
frequency = aft_freqs[tp_freq_count] +
(double)aft_freq_offset[tp_freq_offset_count];
/* printf("Aft frequency: %lf\n",frequency); */
attenuation = tp_atten;
aft_vmehndshk->tpulse_atten = tp_atten;
aft_vmehndshk->tpulse_freq = aft_freq_offset[tp_freq_offset_count]; /* use offset */
aft_vmehndshk->tpulse_flg = 1;

/* Counters used to be updated Here !! */

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
if(Firstime)
  Firstime = 0;
New_tp = 1;           /* handshake with Test Pulse Power Meter */
semTake(cal_sync_sem,WAIT_FOREVER);  /* handshake with pgm_rt task */
semGive(xmit_pwr_sem); /* handshake with Transmit Power Meter */

}






