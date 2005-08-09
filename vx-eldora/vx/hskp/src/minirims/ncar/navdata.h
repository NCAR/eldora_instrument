/*+**************************************************************
* navdata.h							*
*****************************************************************

Description:  This file include navigation related paramters which
	      are a function of either the mission trajectory or
	      hardware specific parameters.

inputs:	      none
outputs:      none

notes:	      If this file is changed, all to the routines which
	      include this file must be recompiled.

called routines:  none

***************************************************************+*/

/* Last Modified:  5/19/88    BY:  E. E. Creel, 9131		*/

/* 5/19/88 Defined constants for launcher locations removed --  */
/*  initialization now to occur using S5 to set ACS global variables */
/* Modified to set DCM for -90 initial orientation (pointing N)	*/
/* Modified to remove ACS related parameters (now in acs.h)	*/
/* Modified Kauai initial elevation to agree with sim data	*/


#define  A11IGC		 0.0	/* initial DCM values used in modechng	*/
#define  A12IGC		 1.0	/* for alignment when caged at -90 deg	*/
#define  A13IGC		 0.0
#define  A21IGC		 0.0
#define  A22IGC		 0.0
#define  A23IGC		-1.0
#define  A31IGC		-1.0
#define  A32IGC		 0.0
#define  A33IGC		 0.0

#define  A11IN		 0.0
#define  A12IN		-0.642
#define  A13IN		 0.766
#define  A21IN		 0.0
#define  A22IN		 0.766
#define  A23IN		 0.642
#define  A31IN		-1.0
#define  A32IN		 0.0
#define  A33IN		 0.0

