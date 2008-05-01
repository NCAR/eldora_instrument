/*
  Module Name:   baudin
  Purpose:       Set up timing module for Baudin waveform
  Arguments:     prf        int, prf in Hz.
                 pcp1       int, PCP's between first and second Preknock
                 pcp2       int, PCP's after second Preknock.
  Author:        Shawn B. Copeland
  Date Written:  05/06/91  by SBC
  Revised:       
*/

#include "/ELDORA/vx/mhen/include/ELDRP7.h"

#define PRE_KNOCK_RISE 250.0  /* in nsec    */
#define PRE_KNOCK_FALL 200.0  /* in nsec    */
#define RF_SWITCH_RISE  35.0  /* in nsec    */
#define RF_SWITCH_FALL  35.0  /* in nsec    */

#define DUTY             10.0  /* in percent */

void baudin(prf,pcp1,pcp2)
     
     int prf,pcp1,pcp2;
     
{
  short num_chips[6],chip_offset[6],chip_width[6];
  short total_pcp;
  unsigned short blank_chip[MAXPCP];
  short freq,chipoff,chipcounts,pkcounts;
  unsigned char uniform_gates[MAXRADARS];
  unsigned short cell_total[MAXRADARS],cell_dist[MAXRADARS][MAXGATES];
  float repeat_seq,pulsewidth;
  short i,pcpcount;

  unsigned short *timon,*timoff;
  
  timon       = (unsigned short *)(TIMBASE + TIMON);
  timoff      = (unsigned short *)(TIMBASE + TIMOFF);
  
  *timoff = 0x0000;  /* shut off module */

  /* DO CHIPS AND BLANKING */
  chipoff = (PRE_KNOCK_RISE - RF_SWITCH_RISE) / 16.6666666;
  pulsewidth = ( DUTY * 10000000 / (float) prf);
  chipcounts = (pulsewidth+RF_SWITCH_RISE-RF_SWITCH_FALL) / 50.00000;

printf("chipoff,pulsewidth,chipcounts==> %d,%f,%d\n\n",chipoff,pulsewidth,chipcounts);

  /* Frequency zero chip parameters */
  num_chips[0] = 1;
  chip_offset[0] = chipoff;
  chip_width[0] = chipcounts;
  /* Frequency one chip parameters */
  num_chips[1] = 1;
  chip_offset[1] = chipoff+chipcounts;
  chip_width[1] = chipcounts;
  /* Frequency two chip parameters */
  num_chips[2] = 1;
  chip_offset[2] = chipoff + 2*chipcounts;
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

  repeat_seq = 1000.0/(float) prf;
  total_pcp = pcp1 + pcp2;

  for (i=0; i<total_pcp; i++)
    blank_chip[i] = 0x0000;

  blank_chip[0] = 0xFFFF;
  blank_chip[pcp1] = 0xFFFF;

for (i=0; i<total_pcp; i++)
printf("i,blank_chip[i] ==> %d, %x\n",i,blank_chip[i]);
  
  load_waveform(num_chips,chip_offset,chip_width,&total_pcp,blank_chip,&repeat_seq);
  
  *timon = (unsigned short) 0x0000;
}







