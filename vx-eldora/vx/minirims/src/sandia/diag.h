/*+ DEC/CMS REPLACEMENT HISTORY, Element DIAG.H+*/
/*+ *4    28-JUL-1988 09:39:20 JRFELLE ""+*/
/*+ *3    23-MAY-1988 12:18:40 ECREEL "rwed prot set by eec"+*/
/*+ *2    20-MAY-1988 15:14:41 ECREEL "fltseqdat.h"+*/
/*+ *1     7-MAR-1988 15:44:53 JRFELLE ""+*/
/*+ DEC/CMS REPLACEMENT HISTORY, Element DIAG.H+*/
/****************************************************************
* diag.h							*
*****************************************************************

<<<<<<<<<<<<<<<<<<<<<<<<<<<< N O T E >>>>>>>>>>>>>>>>>>>>>>>>>>>>

This file is under configuration control.  Any changes to this routine
must be noted in the configuration control log.

<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


Description:  This file defines the data structure which contains
	      diagnostic variable to be used in software testing.
	      This file will not be used in production code.

inputs:	      none
outputs:      none

notes:	      If this file is changed, all to the routines which
	      include this file must be recomipled.

called routines:  none

*****************************************************************/

/* Last Modified:  11/10/87    BY:  E.J. Nava, 9131		*/
/* Changed addresses from proc #2 to proc #f			*/
/* Added max value variables for monitoring			*/
/* Start addressed changed to avoid overlap conflict		*/

#define DIAG_start 0x0f01c180L

struct diag_vars
{
long toddst,toddend;
long tevst,tevend,dtime[128];

short int index;
short int maxeven,maxodd;

};
