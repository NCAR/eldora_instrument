/*+ DEC/CMS REPLACEMENT HISTORY, Element UPDCON.C+*/
/*+ *1     7-MAR-1988 14:36:40 JRFELLE "update constants routine"+*/
/*+ DEC/CMS REPLACEMENT HISTORY, Element UPDCON.C+*/
/****************************************************************
* updcon.c							*
*****************************************************************

<<<<<<<<<<<<<<<<<<<<<<<<<< N O T E >>>>>>>>>>>>>>>>>>>>>>>>>>>>

This file is under configuration control.  Any changes to this routine
must be noted in the configuration control log.

<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


Description:  this routine updates the gyro and accelerometer scale
	      factors with the values measured and estimated in the
	      test mode.

inputs:	      predicted scale factors stored in the nav data structure.
outputs:      updated real-time scale factors.

notes:	      none

called routines:  none

*****************************************************************/

/* Last Modified:  10/16/87    BY:  E.J. Nava, 9131		*/
/* Modified to remove accelerometer bias value transfer		*/

#define NO_MINIR_SCOPE
#include "minirGbl.h"

updcon()
{
register NAV_VARS *n;

n = nav_ptr;

n->rtxgkp = n->prtxgkp;
n->rtxgkn = n->prtxgkn;
n->rtxrgkp = n->prtxrgkp;
n->rtxrgkn = n->prtxrgkn;
n->rtygkp = n->prtygkp;
n->rtygkn = n->prtygkn;
n->rtzgkp = n->prtzgkp;
n->rtzgkn = n->prtzgkn;

/* do not copy accelerometer scale factors unless x axis exactly vertical */
/*
n->rtxak = n->prtxak;
n->rtyak = n->prtyak;
n->rtzak = n->prtzak;
*/
}
