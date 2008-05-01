/******************************  POW_TO_COUNTS.C ****************************/
/*                                                                          */
/*       This subroutine calculated the expected size of the test pulse     */
/*    in peak counts that is incident on the A/D card given a test pulse    */
/*    power and an attenuator setting. (and various other parameters).      */
/*       The following definitions apply:                                   */
/*                                                                          */
/*        BITS             number of converter bits                         */
/*                                                                          */
/*        AD_FULL_SCALLE   voltage required for full scale A/D output       */
/*                         referenced to ground.                            */
/*                                                                          */
/*        COUPLER_ATTEN    difference between receiver power and test pulse */
/*                         power meter reading with atten set at zero dB    */
/*                                                                          */
/*        RECEIVER_GAIN    gain in dB between actuall LNA power and power   */
/*                         right at A/D converter.                          */
/*                                                                          */
/*        tp_power         power of test pulse in dBm                       */
/*                                                                          */
/*        atten            value of attenuator in dB (always positive)      */
/*                                                                          */
/****************************************************************************/

#include  "math.h"
#include  "vxWorks.h"

#define   BITS             12
#define   AD_FULL_SCALE    1.25
#define   COUPLER_ATTEN   41.25
#define   RECEIVER_GAIN   70.0

double  pow_to_counts(tp_power,atten)
double tp_power,atten;
{
double  adpower;        /* power right at A/D inputs (dBm, 50 ohms)  */
double  v_peak;         /* peak voltage at A/D inputs */

adpower = tp_power - atten - COUPLER_ATTEN + RECEIVER_GAIN;
v_peak = sqrt(2.0 * 50 * pow(10.0,adpower/10));
return(pow(2.0,BITS-1.0) * v_peak / AD_FULL_SCALE);
}

