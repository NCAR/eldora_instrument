/*
 *	$Id$
 *
 *	Module:	hskpGbl.h	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.7  1993/09/01  20:07:25  craig
 * *** empty log message ***
 *
 * Revision 1.6  1992/11/03  22:53:48  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/19  19:33:48  craig
 * Initial revision
 *
 *        
 */

/* Define global pointers to header structures so everyone can benefit */
scope VOLUME *vol;
scope WAVEFORM *wave;
scope RADARDESC *fraddes;
scope RADARDESC *araddes;
scope FIELDRADAR *ffrad;
scope FIELDRADAR *afrad;
scope CELLSPACING *cs;
scope PARAMETER *param;
scope NAVDESC *navdes;
scope INSITUDESC *insitdes;
scope RAY *rayi;
scope PLATFORM *plat;
scope FIELDPARAMDATA *fdata;
scope INDEP_FREQ *ifreq;
scope TIME_SERIES *tseries;
scope NAV_INFO *navi;
scope INS_DATA *ins;
scope MINIRIMS_DATA *minir;
scope GPS_DATA *gps;
scope INSITU_DATA *insitdat;

/* Pointers to the current fore and aft radar data rays */

scope struct DATARAY *fore_ray_pntr;
scope struct DATARAY *aft_ray_pntr;

/* Variables to hold old values of various parameters */
scope long  last_msecs_today;
scope char  last_day;
scope float last_fore_position;
scope float last_aft_position;
scope float last_position;
scope float last_fore_angle;
scope float last_aft_angle;
scope long fore_sweep_num;
scope long aft_sweep_num;

/* Milliseconds since midnight of all rays in the handshake areas */
scope long msecs_ray[27];
scope long dwelltime_msec;
scope float half_dwelltime_msec;

/* Global Control flags */
scope short stop_flag;
scope short reload_flag;

/* Parameters to handle the test pulse */

scope short tp_freq_count;
scope short tp_freq_offset_count;
scope short tp_atten;
scope short tp_atten_start;
scope short tp_dwell_count;
scope short testpulse_max_count;
scope float aft_half_nyquist[5][2];
scope float fore_half_nyquist[5][2];

/* Parameters to hold the sine and cosine of the tilt angles */
scope float sin_ftilt;
scope float sin_atilt;
scope float cos_ftilt;
scope float cos_atilt;

/* array to hold mcpl errors discovered in the isr, but need to be reported
   out of it */
scope int mcpl_error[3];

/* global parameters to handle the waveguide switch */

scope unsigned short *wg_sw_pntr;
scope unsigned short wg_sw_current_set;
scope float wg_sw_big_angle;
scope float wg_sw_small_angle;
scope int wg_sw_counter;
scope int wg_sw_flag;
