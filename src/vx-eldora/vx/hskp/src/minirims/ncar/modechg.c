/****************************************************************
* modechg.c							*
*****************************************************************

<<<<<<<<<<<<<<<<<<<<<<<<<< N O T E >>>>>>>>>>>>>>>>>>>>>>>>>>>>

This file is under configuration control.  Any changes to this routine
must be noted in the configuration control log.

<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


Description:  This routine is used to switch the operational mode
	      of the navigator software based on the state of the
	      lower four input discrete switches.

inputs:	      input discrete switches.
outputs:      mode indication variable in the nav data structure.

notes:	      This routine must be changed to reflect the calibration
	      data of the mini-rims platform being used.
	      ALTMODE is a substitution for a block of repetitive
	      code contained in the include file altmode.h.

called routines: A_Matrix_Reset()
                 init_nav()
		 updcon()
		 mode1()	- at bottom of this file -

*****************************************************************/

/* Last Modified:  8/04/88 E. E. Creel, 9131		*/

/* Added logic to turn on external counter on out1dis bit zero              */
/* Added declaration for sens_cnt and logic to set and reset sensread cntrs */
/* Added logic to log alignment kf gains to utility module ram */
/* Added logic to accumulate gyro and accelerometer counts during gyrocompass */
/* Modified to cage to 270 in mode 8 and 0 deg in mode 9	*/
/* Modified to reset gyro bias' when modein = 7 in mode1()	*/
/* JANUARY : made new alignment changes				*/
/* 1/7/88 fixed mode 9 - nav bug introduced by new alignment mods.
   this required removing reference to n->lastpass, which is not
   used anywhere else, best as I can tell..   J.R.F		*/

#define NO_MINIR_SCOPE
#include "minirGbl.h"
#include "navdata.h"
#include "rims.h"
#include "align_v.h"
#include "vrtxvisi.h"
#include "diag.h"

extern unsigned short int vrtx_errf;
extern int align_mail_box;
extern short int cage_control;	/* set default align mode to 0,270 */
extern unsigned short int ctime;

extern short int modein,reset,soft_reset,bitest;

extern long int countdown, sens_count;
extern long int xrg_cnts, yg_cnts, zg_cnts;
extern long int xa_cnts, ya_cnts, za_cnts;

extern long int xg1, yg1, zg1, xa1, ya1, za1;
extern long int xg2, yg2, zg2, xa2, ya2, za2;

extern double w0_spin;

short int modein,reset;

/* Accumulated instrument counts during 1st & 2nd position gyrocompass */
long int xg1, yg1, zg1, xa1, ya1, za1;
long int xg2, yg2, zg2, xa2, ya2, za2;

#define BEG_UTMOD_RAM 0x90000

#define   P270_0	0
#define   P90_180	1
short int cage_control = P270_0;

modechg()
{
extern void A_Matrix_Reset(), init_nav(), ac_slow(), ac_fast()
           ,teci_init(), ecinav_att(), kill_gatar(), revive_gatar();

register NAV_VARS *n;
register struct rims *r;

register int times;
long int msg,err;


struct plat_control *rc;	/* pointers to rims bit field data structures */
struct indiscrete *pindis;
struct diag_vars *d;

void mode1();

/* initialize data structure pointers	*/
n = nav_ptr;
d = (struct diag_vars *)DIAG_start; 	/* diagnostic data structure address */

rc = (struct plat_control *)&n->rimscmd;
pindis = (struct indiscrete *)&n->indis;

modein = pindis->ndis;		/* Look as nav discrete bits only */
times = n->irate;                   /* redefine for speed as reg var */

switch (modein)
{
case 0:
case 7:
    n->modcnt1 = times;
    n->modcnt2 = times;
    n->modcnt3 = times;
    n->modcnt4 = times;
    n->modcnt5 = times;
    n->modcnt6 = times;
    n->modcnt8 = times;
    n->modcnt9 = times;
    if (n->modcnt0 > 0)
    {
      n->modcnt0--;
      if (n->modcnt0==0)
      {
        init_nav();
        teci_init();  /* initialize body to eci before initializing control */
        ecinav_att();
        ac_slow();
        ac_fast();
        d->maxeven=0;
        d->maxodd=0;
        mode1();
      }
    }
    break;
case 1:
    n->modcnt0 = times;
    n->modcnt2 = times;
    n->modcnt3 = times;
    n->modcnt4 = times;
    n->modcnt5 = times;
    n->modcnt6 = times;
    n->modcnt8 = times;
    n->modcnt9 = times;
    if (n->modcnt1 >= 0)
        {
        n->modcnt1--;
        if (n->modcnt1 == 0)
            {
	    rc->rims_cont = CAGE_0;		/* cage rims to 0 */
	    rc->rims_oper = RUN_CAGE_SLEW;	/* set to cage mode */
            n->nsec=0;
            n->asec=0;
            n->mode=1;
            soft_reset=1;
            }
        }
    break;
case 2:
case 14:
case 15:
    n->modcnt0 = times;
    n->modcnt1 = times;
    n->modcnt3 = times;
    n->modcnt4 = times;
    n->modcnt5 = times;
    n->modcnt6 = times;
    n->modcnt8 = times;
    n->modcnt9 = times;

    if(n->modcnt2 >= 0)
      {
      n->modcnt2--;
      if (n->modcnt2 == 0)   /* 2s switch debounce complete */
          {
	  /* Cage platform to Zero unless transitioning
 	     from mode 5 */
	  if(n->mode != 5)
	    {
	    rc->rims_cont = CAGE_0;		/* cage rims to 0 */
	    rc->rims_oper = RUN_CAGE_SLEW;	/* set to cage mode */
	    }

	  if(modein==15)	/* reset DCM and scale factors		*/
	    {
	    n->a11rt = A11IN;  n->a12rt = A12IN;  n->a13rt = A13IN;
	    n->a21rt = A21IN;  n->a22rt = A22IN;  n->a23rt = A23IN;
	    n->a31rt = A31IN;  n->a32rt = A32IN;  n->a33rt = A33IN;
	    n->rtxgkp =  XGKP;
	    n->rtxrgkp = XRGKP;
	    n->rtygkp =  YGKP;
	    n->rtzgkp =  ZGKP;
	    n->rtxgkn =  XGKN;
	    n->rtxrgkn = XRGKN;
	    n->rtygkn =  YGKN;
	    n->rtzgkn =  ZGKN;
	    n->rtxak =   XAK;
	    n->rtyak =   YAK;
	    n->rtzak =   ZAK;
	    }
	  if(modein == 14)	/* reset error count words	*/
	    {
	    }

          if(n->mode == 3)   /* use new scale factors for nav */
            updcon();

	  n->mode = 2;
	  n->nsec = 0;
	  n->asec = 0;

          }  /* end of == 0 loop */
      }      /* end of >= 0 loop */
    break;
case 3:
    n->modcnt0 = times;
    n->modcnt1 = times;
    n->modcnt2 = times;
    n->modcnt4 = times;
    n->modcnt5 = times;
    n->modcnt6 = times;
    n->modcnt8 = times;
    n->modcnt9 = times;
    if (n->modcnt3 >= 0)
        {
        n->modcnt3--;
        if (n->modcnt3 == 0)
            {
	    rc->rims_cont = CAGE_0;		/* cage rims to 0 */
	    rc->rims_oper = RUN_CAGE_SLEW;	/* set to cage mode */
            n->nsec=0;
            n->asec=0;
            n->mode=3;
            }
        }
    break;
case 4:
   n->modcnt0 = times;
   n->modcnt1 = times;
   n->modcnt2 = times;
   n->modcnt3 = times;
   n->modcnt6 = times;
   n->modcnt5 = times;
   n->modcnt8 = times;
   n->modcnt9 = times;
   if(n->modcnt4 >= 0)
    {
      n->modcnt4--;
      if(n->modcnt4 == 0)
      {
        if(reset==0)
        {
          A_Matrix_Reset();

          n->a11rt=n->a11;  n->a12rt=n->a12;  n->a13rt=n->a13;
          n->a21rt=n->a21;  n->a22rt=n->a22;  n->a23rt=n->a23;
          n->a31rt=n->a31;  n->a32rt=n->a32;  n->a33rt=n->a33;

          reset = 1;
        }
        i_align();      /* sets a_mode to FINE for Quick Align */
        update=0;
        propagate=1;
        n->nsec = 0;
        n->asec = 0;
	n->mode = 4;
	init_nav();
        sc_post(&align_mail_box,100L,&err);
        if(err != RET_OK)
          vrtx_errf++;
      }
    }
  break;

case 5:
    n->modcnt0 = times;
    n->modcnt1 = times;
    n->modcnt2 = times;
    n->modcnt3 = times;
    n->modcnt4 = times;
    n->modcnt8 = times;
    n->modcnt9 = times;
    if(n->modcnt5 >= 0)
      {
      n->modcnt5--;
      if(n->modcnt5 == 0)
        {
	n->mode = 5;
	n->nsec = 0;
	n->asec = 0;
	ctime = 0;	/* reset c_align counter */
	}
      }
    break;

case 6:
    n->modcnt0 = times;
    n->modcnt1 = times;
    n->modcnt2 = times;
    n->modcnt3 = times;
    n->modcnt4 = times;
    n->modcnt5 = times;
    n->modcnt8 = times;
    n->modcnt9 = times;
    if (n->modcnt6 >= 0)
        {
        n->modcnt6--;
        if (n->modcnt6 == 0)
            {
            i_align();	/* sets a_mode to FINE for Quick Align */
	    update=0;
	    propagate=1;
	    n->mode = 6;
	    n->nsec = 0;
	    n->asec = 0;
	    init_nav();
            sc_post(&align_mail_box,100L,&err);
            if(err != RET_OK)
              vrtx_errf++;
	    }
	}
    break;

case 8:
    n->modcnt0 = times;
    n->modcnt1 = times;
    n->modcnt2 = times;
    n->modcnt3 = times;
    n->modcnt4 = times;
    n->modcnt5 = times;
    n->modcnt6 = times;
    n->modcnt9 = times;
    if(n->modcnt8 >= 0)
      {
      n->modcnt8--;
      if(n->modcnt8 == 0)	/* resolver starts to cage 2s after switch */
        {		
	countdown = sens_count = -2L;

	if(cage_control == P270_0)
	  rc->rims_cont = CAGE_90;
	else
	  rc->rims_cont = CAGE_270;
	 rc->rims_oper = RUN_CAGE_SLEW;	/* set to cage mode */

        if(n->mode != 9)
          {
          i_align();
	  update=0;
	  propagate=1;
          sc_post(&align_mail_box,100L,&err);
          if(err != RET_OK)
            vrtx_errf++;
          }
        }  /* end of ==0 loop */
      }    /* end of >= loop */

    else	/* logic for minus values of modcnt8 -- after debounce */
      {
      if(n->modcnt8 > (-2*(n->eightsec)) )  /* 16 sec delay before switch */
        {
        n->modcnt8--;
        n->nsec = 0;
        n->asec = 0;
        }
	/* counters will reset 4s after resolver starts to cage */
        if(n->modcnt8 == -(n->foursec))
          {
	  if(n->mode == 0x2  || n->mode == 0x1)
	    {
	    /* zero all counters */
	    countdown = -1L;
	    sens_count = -1L;
	    xg1 = yg1 = zg1 = xa1 = ya1 = za1 = 0L;
	    xg2 = yg2 = zg2 = xa2 = ya2 = za2 = 0L;
	    }
	  if(n->mode == 0x9)
	    {
	    /* copy counters in gcomp() to preserve */
	    xg1 = xrg_cnts;    yg1 = yg_cnts;    zg1 = zg_cnts;
	    xa1 = xa_cnts;     ya1 = ya_cnts;    za1 = za_cnts;
	    /* zero counters in gcomp() for next position gyrocompass */
	    countdown = -1L;
            sens_count = -1L;
	    }
        }  /* end of logic for modcnt8 equal to -4s */

	/* skip coarse align mode if transitioning from mode 9 */
	if( (n->mode==9) && (n->modcnt8 == -2*n->eightsec) )
	  n->modcnt8 = -2*n->eightsec-n->onesec;

	/* switch to coarse align mode for 56 seconds */
	if( (n->modcnt8 <= -2*n->eightsec) &&
            (n->modcnt8 > (-2*n->eightsec-n->onesec) ) )
	  {
	  if(n->modcnt8 == -2*n->eightsec)
	    ctime = 0;
	  n->mode = 5;
	  n->modcnt8--;
	  }

        if(n->modcnt8 == (-2*n->eightsec-n->onesec) )
          {
	  n->modcnt8--;
          n->mode = 8;
	  init_nav();
	  /* set countdown for 300s of count accumulations */
	  countdown = 64L * 300L;
          sens_count = 2L * 64L * 300L;
	  }
       }    /* end of logic for modcnt8 minus times */
    break;

case 9:
    n->modcnt0 = times;
    n->modcnt1 = times;
    n->modcnt2 = times;
    n->modcnt3 = times;
    n->modcnt4 = times;
    n->modcnt5 = times;
    n->modcnt6 = times;
    n->modcnt8 = times;
    if(n->modcnt9 >= 0)
      {
      n->modcnt9--;
      if(n->modcnt9 == 0)
        {
	countdown = -2L;	sens_count = -2L;

	if(cage_control == P270_0)
	  rc->rims_cont = CAGE_0;		
    	else
	  rc->rims_cont = CAGE_180;	
	rc->rims_oper = RUN_CAGE_SLEW;	/* set to cage mode */

        if(n->mode != 8)
          {
          i_align();
          update=0;
          propagate=1;
          sc_post(&align_mail_box,100L,&err);
          if(err != RET_OK)
            vrtx_errf++;
          }
        }  /* end of modcnt9 == 0 logic */
      }    /* end of modcnt9 positive time logic -- switch debounce */
    else
      {
      if(n->modcnt9 > (-2*(n->eightsec)) )  /* 16 sec delay before switch */
        {
        n->modcnt9--;
        n->nsec = 0;
        n->asec = 0;
        }
	/* counters will reset 4s after resolver starts to cage */
      if(n->modcnt9 == -(n->foursec))
        {
	if(n->mode == 0x2 || n->mode == 0x1)
	  {

	  /* zero all counters */
	  countdown = sens_count = -1L;

	  xg1 = yg1 = zg1 = xa1 = ya1 = za1 = 0L;
	  xg2 = yg2 = zg2 = xa2 = ya2 = za2 = 0L;
          }
	if(n->mode == 0x8)
	  {
	  /* copy counters in gcomp() to preserve */
	  xg1 = xrg_cnts;    yg1 = yg_cnts;    zg1 = zg_cnts;
	  xa1 = xa_cnts;     ya1 = ya_cnts;    za1 = za_cnts;
	  /* zero counters in gcomp() for next position gyrocompass */
	  countdown = -1L;
	  sens_count = -1L;
	  }
        }  /* end of logic for modcnt9 == to -4s */

	/* skip coarse align mode if transitioning from mode 8 */
	if( (n->mode==8) && (n->modcnt9 == -2*n->eightsec) )
	  n->modcnt9 = -2*n->eightsec-n->onesec;

	/* switch to coarse align mode for 56 seconds */
	if( (n->modcnt9 <= -2*n->eightsec) &&
            (n->modcnt9 > (-2*n->eightsec-n->onesec) ) )
	  {
	  if(n->modcnt9 == -2*n->eightsec)
	    ctime = 0;
	  n->mode = 5;
	  n->modcnt9--;
	  }

        if(n->modcnt9 == (-2*n->eightsec-n->onesec) )
          {
	  n->modcnt9--;
          n->mode = 9;
	  init_nav();
	  /* set countdown for 300s of count accumulations */
	  countdown = 64L * 300L;
	  sens_count = 2L * 64L * 300L;
	  }
      }
    break;
default:
    n->modcnt0 = times;
    n->modcnt1 = times;
    n->modcnt2 = times;
    n->modcnt3 = times;
    n->modcnt4 = times;
    n->modcnt5 = times;
    n->modcnt6 = times;
    n->modcnt8 = times;
    n->modcnt9 = times;
    break;
    }	/* end of the switch */
} /* end of modechg() */


void
mode1()		/* routine to implement redundant nav sw code */
{
register NAV_VARS *n;

extern short int modein;

struct plat_control *rc;

n = nav_ptr;

rc = (struct plat_control *)&n->rimscmd;

if(modein == 7)
    {
    n->xgb = n->xgbin;
    n->ygb = n->ygbin;
    n->zgb = n->zgbin;
    }

if(n->mode==8 || n->mode==9 || n->mode == 4 || n->mode == 6)
    {
    if(modein!=7)
	{
	n->a11rt = n->a11;   n->a12rt = n->a12;   n->a13rt = n->a13;
	n->a21rt = n->a21;   n->a22rt = n->a22;   n->a23rt = n->a23;
	n->a31rt = n->a31;   n->a32rt = n->a32;   n->a33rt = n->a33;
	}
    if(n->mode==0x8 || n->mode==0x9)
      {
      /* save counters from gcomp() in 2nd memory copy */
      xg2 = xrg_cnts;    yg2 = yg_cnts;    zg2 = zg_cnts;
      xa2 = xa_cnts;     ya2 = ya_cnts;    za2 = za_cnts;
      /* set countdown to idle state */
      countdown = -2L;
      sens_count = -2L;
      }

	/* tinit() used to be called here */
    }

n->mode = 0;		/* mode switching action common to all switches	*/
rc->rims_oper = RUN_INERT;	/* run in inertial mode		*/
n->nsec = 0;
n->asec = 0;
}
