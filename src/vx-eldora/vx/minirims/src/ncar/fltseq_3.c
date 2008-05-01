#define NO_MINIR_SCOPE
#include "minirGbl.h"
#include "rims.h"
#include "fltseqda.h"
/******************************************************************
function:  fltseq()

Description:  Controls sequence of events during flight

inputs:	 variables in header files

outputs:  output discretes defined in rims.h

notes:

called routines:  check_for_launch, check_for_sep() located at bottom
                  of this file

Last Modified: 1-10-89 DMKOZLO
*******************************************************************/
#define TALO n->talo
#define TASEP n->tasep

#define INTERCEPT    111954   /* 1749.276*64 */
#define SEP_TIME     206.0

extern short abtest, bitest;

long abstop, seq_test;

/*following time is referenced from time after lift off            */

long gps_on    = 190*64;   /* power on GPS                     */

/* following times referenced to time after third stage separation */

long rel_valv  = 32;        /* release pressure valve          */
long spin_up1  = 1*64;      /* spin stabilize                  */
long exp_1     = 45*64;     /* deploy experiment 1             */
long acs_sleep = 130*64;    /* relax acs requirements          */
long mdi_on    = 1430*64;   /* power on MDI                    */
long de_spin   = 1492*64;   /* despin  ??? Can we despin before
                                        experiment 3 deploys   */
long exp_3     = 96777;     /* deploy experiment 3 ~ 1718.15 talo
                                        1512.15 after sep */
long spin_up2  = 96905;     /* spin stabilize 1514.15 after sep */

void
fltseq()
{
  extern long rel_valv;
  extern long spin_up1;
  extern long exp_1;
  extern long acs_sleep;
  extern long gps_on;
  extern long de_spin;
  extern long mdi_on;
  extern long exp_3;
  extern long spin_up2;

  extern long abstop;

  NAV_VARS *n;
  register struct out1discrete *po1dis;
  register struct out2discrete *po2dis;
  void check_for_launch(), check_for_sep();

  n = nav_ptr;
  po1dis = (struct out1discrete *) &n->out1dis;
  po2dis = (struct out2discrete *) &n->out2dis;

/* -----------------Launch & Separation Detect Logic------------------ */

  if (n->snum >= LAUNCH_DETECT)
  {
    TALO++;
    if (n->snum >= SEPARATION_DETECT)
      TASEP++;
  }
       /* go through flight sequence  */

  switch (n->snum+1)       /* initially snum = -1 */
  {
    case GPS_ON:    /* timing for GPS on:  pulse on for 0.5 second */
      if (TALO >= gps_on)    /* 1 */
      {
        po1dis->gpson = 1;
        n->snum = GPS_ON;
      }

  }/* end of flight sequence switch */

}
/***********************************************************************/
void check_for_launch()
{
  register NAV_VARS *n;
  n = nav_ptr;

  TALO = 0;
  if (n->vd < VTHRESH)
  {
    TALO = TALO_INT_2;
    n->snum = LAUNCH_DETECT;
    n->lnchdet = 1;
  }
    /* Anything more to do? */
}
/***********************************************************************/
void check_for_sep()
{
  register NAV_VARS *n;
  struct indiscrete *pindis;
  struct out1discrete *po1dis;

  n = nav_ptr;
  pindis = (struct indiscrete *) &n->indis;
  po1dis = (struct out1discrete *) &n->out1dis;

  TASEP = 0;
  if((pindis->sep1 == 1) || (pindis->sep2 == 1))
  {
    n->sepcnt++;
    if (n->sepcnt >= 32)  /* COUNT 32/64 BEFORE ACKNOWLEDGING SEPARATION */
    {
      TASEP = (long) n->sepcnt;
      n->snum = SEPARATION_DETECT;
      n->sepdet = 1;
      po1dis->gpson = 1;
    }
  }
}
