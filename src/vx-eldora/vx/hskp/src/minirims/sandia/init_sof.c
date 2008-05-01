/*+**************************************************************
* soft_init.c							*
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

#include "navvars.h"
#include "rims.h"
#include "align_v.h"
#include "ecinav_v.h"

#define DTOR 0.017453293

soft_init()
{
register struct nav_vars *n;
register struct ecinav_vars *p_eci;

extern long int countdown,sens_count,start_flag,modetimer,ready;
extern short int reset, euler_flag, bitest, abtest;
extern double as11,as12,as13,as21,as22,as23,as31,as32,as33,
              ae11,ae12,ae13,ae21,ae22,ae23,ae31,ae32,ae33;

n = (struct nav_vars *)NAV_start;
p_eci = &eci;

n->asec = 0;
n->nsec = 0;
n->talo = 0;
n->tasep = 0;
n->intpwr = 0;
n->intpwron = 0;
ready = 0;

n->ixgtot = 0;
n->ixrgtot = 0;
n->iygtot = 0;
n->izgtot = 0;
n->ixatot = 0;
n->iyatot = 0;
n->izatot = 0;

n->rtxak = XAK;
n->rtyak = YAK;
n->rtzak = ZAK;

n->rtxgkp = XGKP;
n->rtxgkn = XGKN;
n->rtxrgkp = XRGKP;
n->rtxrgkn = XRGKN;
n->rtygkp = YGKP;
n->rtygkn = YGKN;
n->rtzgkp = ZGKP;
n->rtzgkn = ZGKN;

n->xgb = n->xgbin;
n->ygb = n->ygbin;
n->zgb = n->zgbin;

n->a11 = n->a11rt;   /* initialize the direction cos matrix */
n->a12 = n->a12rt;
n->a13 = n->a13rt;   /* initialize the direction cos matrix */
n->a21 = n->a21rt;
n->a22 = n->a22rt;
n->a23 = n->a23rt;
n->a31 = n->a31rt;
n->a32 = n->a32rt;
n->a33 = n->a33rt;

n->prtxgkp = 0.0;
n->prtxgkn = 0.0;
n->prtxrgkp = 0.0;
n->prtxrgkn = 0.0;
n->prtygkp = 0.0;
n->prtygkn = 0.0;
n->prtzgkp = 0.0;
n->prtzgkn = 0.0;

n->prtxak = 0.0;
n->prtyak = 0.0;
n->prtzak = 0.0;

countdown = -1;
sens_count = -1;
start_flag = 0;
modetimer = 64*60;

n->adatx = 0;
n->adaty = 0;
n->adatz = 0;

n->alt = n->altin;
n->lg = n->lgin;
n->lng = n->lngin;

n->out1dis = 0;        /* reset output discretes  */
n->out2dis = 0;

n->sepcnt = 0;
n->sepdet = 0;
n->lnchdet = 0;
n->launch = 0;

p_eci->dv_x=0.0;
p_eci->dv_y=0.0;
p_eci->dv_z=0.0;

p_eci->pxeci = 0.0;
p_eci->pyeci = 0.0;
p_eci->pzeci = 0.0;

p_eci->vxeci = 0.0;
p_eci->vyeci = 0.0;
p_eci->vzeci = 0.0;

n->c11 = 0.0;  n->c12 = 0.0;  n->c13 = 0.0;
n->c21 = 0.0;  n->c22 = 0.0;  n->c23 = 0.0;
n->c31 = 0.0;  n->c32 = 0.0;  n->c33 = 0.0;

reset = 0;    /* allows another call to A_Matrix_Reset()  */

as11 = 0.0;  as12 = 0.0;  as13 = 0.0;
as21 = 0.0;  as22 = 0.0;  as23 = 0.0;
as31 = 0.0;  as32 = 0.0;  as33 = 0.0;

ae11 = 0.0;  ae12 = 0.0;  ae13 = 0.0;
ae21 = 0.0;  ae22 = 0.0;  ae23 = 0.0;
ae31 = 0.0;  ae32 = 0.0;  ae33 = 0.0;

if ((bitest == 1) && (abtest == 1))
  euler_flag = 1;
else
  euler_flag = 0;
}
