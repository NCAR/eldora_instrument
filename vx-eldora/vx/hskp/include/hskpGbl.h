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
scope RADARDESC *fraddes;
scope RADARDESC *araddes;
scope FIELDRADAR *frad;
scope CELLSPACING *cs;
scope struct VOLUME *vol;
scope struct WAVEFORM *wave;
scope struct RADARDESC *raddes;
scope struct FIELDRADAR *frad;
scope struct CELLSPACING *cs;
scope struct PARAMETER *param;
scope struct NAVDESC *navdes;
scope struct INSITU_PARAMETER *insitdes;
scope struct RAY *rayi;
scope struct PLATFORM *plat;
scope struct FIELDPARAMDATA *fdata;
scope struct INDEP_FREQ *ifreq;
scope struct TIME_SERIES *tseries;
scope struct NAV_INFO *navi;
scope struct INS_DATA *ins;
scope struct MINIRIMS_DATA *minir;
scope struct GPS_DATA *gps;
scope struct INSITU_DATA *insitu;

/* Define a data ray structure that will be used to fill the various
   data areas */
scope long  last_msecs_today;
scope struct ray{
struct RAY this_rayi;
struct PLATFORM this_plat;
struct FIELDPARAMDATA this_fdata;
} 
scope float last_aft_position;
/* Parameters to handle the test pulse */
scope short tp_atten;
scope short tp_atten_start;
scope short tp_dwell_count;
scope short testpulse_max_count;
scope float aft_half_nyquist[5][2];
scope float fore_half_nyquist[5][2];
