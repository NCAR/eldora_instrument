/*
 *	$Id$
 *
 *	Module:		 RadarGbls.h
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1994/09/06  16:14:10  thor
 * Many changes.
 *
 * Revision 1.2  1993/07/23  15:00:22  eric
 * Added TASS stuff.
 *
 * Revision 1.1  1992/06/25  17:39:03  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCRadarGblsh
#define INCRadarGblsh

#ifndef SCOPE
#define SCOPE extern
#endif /* SCOPE */
#ifndef RP7_SCOPE
#define RP7_SCOPE extern
#endif /* RP7_SCOPE */

#ifdef TASS
#include "TSacq.h"
#endif
#include "Header.h"

struct RadarCommand {
    u_long cmd;
    int dc_points;
#ifdef TASS
    TSACQ tass_info;
#endif
};

struct RadarStatus {
    char rp7;
    char mcpl;
    short status;
    short mailbox;
};

#ifdef OK_RPC
typedef struct RadarCommand RadarCommand;
typedef struct RadarStatus RadarStatus;

#ifndef UNIX
#include "semLib.h"

SCOPE RadarStatus *currStatus;
RP7_SCOPE  volatile int fill_busy, fill_ts_array, pts, ts_flag, ts_samps; /* array fill globals */
RP7_SCOPE  volatile int fft_flag, radar_fore_aft, send_ray[3][2], send_nav, send_ads;
RP7_SCOPE  unsigned long ad_freq;
RP7_SCOPE  volatile long  curr_ray_add[3][2], curr_mailbox_add[3][2], curr_nav_add; 
RP7_SCOPE  volatile long  curr_nav_mailbox_add, curr_ads_add, curr_ads_mailbox_add;
RP7_SCOPE  volatile float *a_pntr;
RP7_SCOPE  volatile int  load, stop, reboot, resync, dc_removal, ts_freq; /* rpc globals */
RP7_SCOPE volatile int Run_flag;
RP7_SCOPE CELLSPACING *cs;
RP7_SCOPE RADARDESC *rdsc;
RP7_SCOPE WAVEFORM *wvfm;
RP7_SCOPE PARAMETER *prm;
RP7_SCOPE FIELDRADAR *fldrdr;

RP7_SCOPE SEM_ID bim_int0_sem;
RP7_SCOPE SEM_ID bim_int1_sem;
RP7_SCOPE SEM_ID bim_int2_sem;
RP7_SCOPE SEM_ID bim_int3_sem;
RP7_SCOPE SEM_ID real_sem;
RP7_SCOPE SEM_ID exec_sem;
RP7_SCOPE SEM_ID fill_array_sem;
RP7_SCOPE SEM_ID array_full_sem;
RP7_SCOPE SEM_ID exec_int0_sem;
RP7_SCOPE  unsigned char  *bim_cr0, *bim_cr1, *bim_cr2, *bim_cr3;
RP7_SCOPE  char ad_chan;
#ifdef TASS
RP7_SCOPE TSACQ *tass_ptr;
RP7_SCOPE int ts_acq_on;
#endif /* TASS */
#endif /* UNIX */
#endif /* OK_RPC */
#endif /* INCRadarGblsh */


