#include "navvars.h"
#include "spin.h"
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

extern void ac_fast();

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

  extern void ac_fast();
  struct nav_vars *n;
  register struct out1discrete *po1dis;
  register struct out2discrete *po2dis;
  void check_for_launch(), check_for_sep();

  n= (struct nav_vars *) NAV_start;
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
    case LAUNCH_DETECT:       /* 0 */
      check_for_launch();
      spin_mode = ESTIMATE_ONLY;
      break;

    case GPS_ON:    /* timing for GPS on:  pulse on for 0.5 second */
      if (TALO >= gps_on)    /* 1 */
      {
        po1dis->gpson = 1;
        spin_mode = ESTIMATE_ONLY;
        n->snum = GPS_ON;
      }

    case SEPARATION_DETECT:   /* 2 */
      if (TALO > 185*64)
        check_for_sep();
      spin_mode = ESTIMATE_ONLY;
      break;

    case REL_VALV:            /* 3 */
      if (TASEP >= rel_valv)
      {
        po1dis->pr_vlv = 1;
        n->snum = REL_VALV;
        spin_mode = ESTIMATE_ONLY;
      }
      break;

    case SPIN_UP1:  /* wait for 1 second after separation until spin up */
      if (TASEP >= spin_up1)  /* 4 */
      {
        spin_mode = SPIN;
        n->snum = SPIN_UP1;
      }
      break;

    case EXP_1:      /* timing for experiment #1    flight 3 only */
      if (TASEP >= exp_1)  /* 5 */
      {
        po1dis->esdply = 1;
        spin_mode = SPIN;
        n->snum = EXP_1;
      }
      break;

    case RELAX:    /* relax cold gas */
      if (TASEP >= acs_sleep) /* 6 */
      {
        spin_mode = ACS_SLEEP;
        n->snum = RELAX;
      }
      break;

    case MDI_ON:    /* timing for mdi turn on     */
      if (TASEP >= mdi_on)    /* 7 */
      {
        po2dis->mdion = 1;
        n->snum = MDI_ON;
        spin_mode = ACS_SLEEP;
      }
      break;

    case DE_SPIN:   /* timing for start of despin */
      if (TASEP >= de_spin)   /* 8 */
      {
        spin_mode = DESPIN1;
        n->snum = DE_SPIN;
      }
      break;

    case EXP_3:     /* timing for release of experiment #3 */
      if (TASEP >= exp_3)   /* 9 */
      {
        spin_mode = DESPIN1;
        po1dis->bloon1 = 1;
        po1dis->bloon2 = 1;
        n->snum = EXP_3;
      }
      break;

    case SPIN_UP2:  /* timing for return of spin_up */
      if (TASEP >= spin_up2)   /* 10  */
      {
        spin_mode = SPIN;
        n->snum = SPIN_UP2;
      }
      break;

    case DONE:    /* 11 */
      spin_mode = SPIN;
      break;

  }/* end of flight sequence switch */

  if ((bitest == 1) && (abtest == 1))
      if (TASEP > (abstop*64))
        spin_mode = ESTIMATE_ONLY;

  ac_fast();

  if ((bitest == 1) && (seq_test == 1))
  {
    puls_1 = 0;
    puls_2 = 0;
    puls_3 = 0;
    puls_4 = 0;
    puls_5 = 0;
    puls_6 = 0;
  }

  if(puls_1)
    po1dis->noz_6 = 1;
  else
    po1dis->noz_6 = 0;
  if(puls_2)
    po1dis->noz_5 = 1;
  else
    po1dis->noz_5 = 0;
  if(puls_3)
    po1dis->noz_4 = 1;
  else
    po1dis->noz_4 = 0;
  if(puls_4)
    po1dis->noz_3 = 1;
  else
    po1dis->noz_3 = 0;
  if(puls_5)
    po1dis->noz_2 = 1;
  else
    po1dis->noz_2 = 0;
  if(puls_6)
    po1dis->noz_1 = 1;
  else
    po1dis->noz_1 = 0;

}  /* fltseq */

/***********************************************************************/
void check_for_launch()
{
  register struct nav_vars *n;
  n = (struct nav_vars *)NAV_start;

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
  register struct nav_vars *n;
  struct indiscrete *pindis;
  struct out1discrete *po1dis;

  n = (struct nav_vars *)NAV_start;
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
