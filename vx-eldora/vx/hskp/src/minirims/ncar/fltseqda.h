/****************************************************************
* fltseqdat.h							*
*****************************************************************


Description:  This file include paramters which are a function of
	      the mission sequence operations.

inputs:	      none
outputs:      none

notes:	      If this file is changed, all to the routines which
	      include this file must be recomipled.

called routines:  none

*****************************************************************/
#define VTHRESH	 -10.0	        /* vertical velocity threshold 20 ft/sec UP           		*/
#define TALO_INT_1 -2           /* set talo to -2/64 sec for normal launch detect */
#define TALO_INT_2  31          /* set talo to +31/64 sec for backup launch detect */

#define LAUNCH_NOT_DETECTED     -1
#define LAUNCH_DETECT            0
#define GPS_ON                   1
#define SEPARATION_DETECT        2
#define REL_VALV                 3
#define SPIN_UP1                 4
#define EXP_1                    5
#define RELAX                    6
#define MDI_ON                   7
#define DE_SPIN                  8
#define EXP_3                    9
#define SPIN_UP2                10
#define DONE                    11
