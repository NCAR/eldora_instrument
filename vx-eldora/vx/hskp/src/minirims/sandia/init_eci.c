/*+**********************************************************************

NAME:		init_eci.c


SYNOPSIS: 	void init_eci();

INPUTS: 	external eci variables

PROCESSING: 	initialize these variables to nominal values


OUTPUTS: 	None.

RETURNS: 	None.

    David M. Kozlowski  1-5-89

**********************************************************************+*/
#include "math.h"
#include "ecinav_v.h"

#define DTOR 0.017453293

double lng_init, lg_init, alt_init;

void init_eci()
{
register struct ecinav_vars *p_eci;
double sLam,cLam,sLg,cLg;

p_eci = &eci;

sLam = sin(lng_init*DTOR);
cLam = cos(lng_init*DTOR);
sLg  = sin(lg_init*DTOR);
cLg  = cos(lg_init*DTOR);

p_eci->tni11 =  -sLg*cLam;
p_eci->tni12 =  -sLam;
p_eci->tni13 =  -cLg*cLam;
p_eci->tni21 =  -sLg*sLam;
p_eci->tni22 =   cLam;
p_eci->tni23 =  -cLg*sLam;
p_eci->tni31 =   cLg;
p_eci->tni32 =   0.0;
p_eci->tni33 =  -sLg;

}
