/*+**************************************************************
* hard_init.c							*
*****************************************************************

Description:  generates initial constants and prameters used in
	      the navigation calculations.  All time dependent
	      parameters are calculated here.

inputs:	      raw calibration data stored in this program.
outputs:      time dependent adjusted parameters stored in the nav
	      data structure.

notes:	      This routine must be changed to reflect the calibration
	      data of the mini-rims platform being used.

called routines:  none

***************************************************************+*/

#define NO_MINIR_SCOPE
#include "minirGbl.h"
#include "math.h"
#include "navdata.h"
#include "rims.h"
#include "fltseqda.h"

extern double lg_init,lng_init,alt_init;
extern struct earth
      {
        double req, ome, ome2, flat, eccen2, gm, rj2;
      } erth;

#define DTOR 0.017453293
#define VANDENBERG

hard_init()
{
register NAV_VARS *n;
register struct earth *e;

extern short int soft_reset, euler_flag, timertest;
extern double op_az, ea_phi1;
extern long int sens_count, countdown, seq_test;
extern long int rel_valv, spin_up1, exp_1, acs_sleep, gps_on, de_spin,
                mdi_on, exp_3, spin_up2;
n = nav_ptr;
e = &erth;

n->rtxak = XAK;
n->rtyak = YAK;
n->rtzak = ZAK;

n->load_tm = 0;
n->rtime = 0;
n->irate = INTHRZ;
n->snum = LAUNCH_NOT_DETECTED;
n->onesec = n->irate / 2;
n->foursec = n->irate * 2;      /* delays for iteration calcs */
n->eightsec = n->foursec * 2;
n->dtv = DELTIME;
n->dtv2 = n->dtv * 2.0;		/* del time for routine iterating at half */
				/* the rate of normal iteration of either */
				/* the attitude or position routines.	  */

n->rimsup = 0xff;           /* rimsup status set to start up code */

n->mode = 1;
n->modcnt0 = n->irate;      /* force switch into mode 1 on start */
n->modcnt1 = -1;
n->modcnt2 = n->irate;
n->modcnt3 = n->irate;
n->modcnt4 = n->irate;
n->modcnt5 = n->irate;
n->modcnt6 = n->irate;
n->modcnt8 = n->irate;
n->modcnt9 = n->irate;

n->rxx = RXX * n->onesec;	/* Premult done to yield correct units	*/
n->rxy = RXY * n->onesec;	/* in size effect calculations		*/
n->rxz = RXZ * n->onesec;
n->ryx = RYX * n->onesec;
n->ryy = RYY * n->onesec;
n->ryz = RYZ * n->onesec;
n->rzx = RZX * n->onesec;
n->rzy = RZY * n->onesec;
n->rzz = RZZ * n->onesec;

n->xab = XAB * n->dtv; /* convert bias to ft/sec/sample time */
n->yab = YAB * n->dtv;
n->zab = ZAB * n->dtv;

n->ixan1 = 0;
n->iyan1 = 0;
n->izan1 = 0;

n->xgbin = XRGB * n->dtv; /* convert to radians/DELTIME  */
n->ygbin = YGB * n->dtv;
n->zgbin = ZGB * n->dtv;
/*
n->tdxgb = 0.0;
n->tdygb = 0.0;
n->tdzgb = 0.0;
*/
n->dthn = 0.0;
n->dthe = 0.0;
n->dthd = 0.0;

n->dthx2 = 0.0;
n->dthy2 = 0.0;
n->dthz2 = 0.0;

n->dphxl = 0.0;
n->dphyl = 0.0;
n->dphzl = 0.0;

n->dphx = 0.0;
n->dphy = 0.0;
n->dphz = 0.0;

n->alfx = 0.0;
n->alfy = 0.0;
n->alfz = 0.0;

n->ixan1 = 0;
n->iyan1 = 0;
n->izan1 = 0;

n->adatx = 0;
n->adaty = 0;
n->adatz = 0;

n->a11 = A11IN;   /* initialize the direction cos matrix */
n->a12 = A12IN;
n->a13 = A13IN;   /* initialize the direction cos matrix */
n->a21 = A21IN;
n->a22 = A22IN;
n->a23 = A23IN;
n->a31 = A31IN;
n->a32 = A32IN;
n->a33 = A33IN;

n->a11rt = A11IN;   /* initialize the direction cos matrix */
n->a12rt = A12IN;
n->a13rt = A13IN;
n->a21rt = A21IN;
n->a22rt = A22IN;
n->a23rt = A23IN;
n->a31rt = A31IN;
n->a32rt = A32IN;
n->a33rt = A33IN;

#ifdef ALB
/* Albuquerque	*/
alt_init = 5310.;		/* elevation in feet		*/
lg_init = 35.0509784;	        /* latitude in degrees		*/
lng_init =  -106.5501167;	/* longitude in degrees		*/
#else
/* Vandenburg   */
#ifdef VANDENBERG
alt_init = 307.41;                 /* elevation in feet            */
lg_init = 34.84604194;           /* lattitude in degrees         */
lng_init = -120.5812228;          /* longitude in degrees         */
#endif
#endif

n->altin = alt_init;
n->lgin = lg_init*DTOR;
n->lngin = lng_init*DTOR;

n->alt = n->altin;
n->lg = n->lgin;
n->lng = n->lngin;

n->ixrg_16 = 0;
n->ixg_16 = 0;
n->iyg_16 = 0;
n->izg_16 = 0;
n->ixa_16 = 0;
n->iya_16 = 0;
n->iza_16 = 0;

                       /* earth and gravity variables */
e->req = 20925639.76;
e->ome = 7.292115147e-05;
e->ome2 = e->ome*e->ome;
e->flat = 1.0/298.26;
e->eccen2 = e->flat*(2.0 - e->flat);
e->gm = 0.14076454e+17;
e->rj2 = 0.10826158e-02;

soft_reset = 1;

euler_flag = 0;
op_az = 40.0;
seq_test = 0;
sens_count = -1;
countdown = -1;
timertest = 0;
ea_phi1 = 0;

/* The following time is referenced from TALO */
gps_on = 190*64;

/* The following times are referenced from TASEP */
rel_valv = 32;
spin_up1 = 1*64;
exp_1 = 45*64;
acs_sleep = 130*64;
mdi_on = 1430*64;
de_spin = 1492*64;
exp_3 = 96777;		/* 1512.15 */
spin_up2 = 96906;	/* 1514.15 */
}
