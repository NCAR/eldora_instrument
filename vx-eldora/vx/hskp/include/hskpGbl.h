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
 *        
 */

/* Define global pointers to header structures so everyone can benefit */
scope VOLUME *vol;
scope WAVEFORM *wave;
scope RADARDESC *fraddes;
scope RADARDESC *araddes;
scope FIELDRADAR *frad;
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
scope long msecs_ray[27];
scope long dwelltime_msec;

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
