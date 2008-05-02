/*
 * $Id$
 *
 * Module: hskpGbl.h  
 * Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 * Date:   $Date$
 *
 * revision history
 * ----------------
 * $Log: hskpGbl.h,v $
 * Revision 1.16  2005/08/04 22:27:39  granger
 * commit vx/hskp as copied from /net/eldora/eldora, except a few obsolete
 * (afaik) directories were removed, like hskp/src/clock/newclk and
 * hskp/src/arinc_NCAR
 *
 * Revision 1.15  2003/10/01  20:10:25  kapoor
 * *** empty log message ***
 *
 * Revision 1.14  2003/10/01  19:57:40  kapoor
 * *** empty log message ***
 *
 * Revision 1.13  2003/10/01  19:45:41  kapoor
 * *** empty log message ***
 *
 * Revision 1.12  2003/09/24  17:00:18  kapoor
 * Added global bancomm clock card commands
 *
 * Revision 1.11  1999/09/27  15:59:34  eric
 * added global Firstime to keep track of initial time through update
 * testpulse loop.
 *
 * Revision 1.10  1997/08/26  21:25:21  craig
 * *** empty log message ***
 *
 * Revision 1.9  1996/10/11  23:27:29  craig
 * *** empty log message ***
 *
 * Revision 1.8  1996/02/21  17:21:15  craig
 * *** empty log message ***
 *
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

scope int Firstime;                /* Variable to keep track of first call */
                                   /* to update_testpulse */
scope short tp_freq_count;         /* Counter to step through each frequency */
scope short tp_freq_offset_count;  /* Cntr to step thru each freq offset */
scope short tp_atten;              /* The current attenuator setting */
scope short tp_atten_start;        /* The starting value of attenuation */
scope short tp_dwell_count;        /* Dwell counter for changing the testpls */
scope short testpulse_max_count;   /* Maximum number of dwells each testpls */
scope long aft_freq_offset[8];     /* Aft Frequency offsets from transmitted */
scope long fore_freq_offset[8];    /* Fore freq. offsets from transmitted */
scope double fore_freqs[5];        /* the Fore transmitted frequencies */
scope double aft_freqs[5];         /* The aft transmitted frequencies */
scope int autocal;                 /* on/off switch for debugging stuff */
scope float fore_lna_radar_const;  /* radar consant modified for the @ LNA */
scope float aft_lna_radar_const;   /* radar consant modified for the @ LNA */
scope float log_testpulse_range2;  /* lOG10 of testpulse range squared */
scope int tp_sum_start, tp_sum_end; /* Starting and ending dwell
           counts for sum */
scope int tp_sum_count;             /* Summation counter */
scope float fore_dbz_sum, aft_dbz_sum;  /* Sums of the dbz values */
scope float fore_tp_level, aft_tp_level; /* sums of the
          testpulse power */
scope float fore_vel_sum, aft_vel_sum;  /* Sums of the velocities */
scope float f_vscale, a_vscale;       /* fore and aft velocity scale factors */
scope float fore_tp_velocity[8], aft_tp_velocity[8];  /* what the velocities
        should be */


/* Parameters to hold the sine and cosine of the tilt angles */
scope float sin_ftilt;
scope float sin_atilt;
scope float cos_ftilt;
scope float cos_atilt;

/* array to hold mcpl errors discovered in the isr, but need to be reported
   out of it */
scope int mcpl_error[3];
scope short mcpl_xfer_processor; /* Indicates which radar processor is allowed
        to perform mcpl transfers */

/* global parameters to handle the waveguide switch */

scope unsigned char *wg_sw_pntr;
scope unsigned char wg_sw_current_set;
scope float wg_sw_big_angle;
scope float wg_sw_small_angle;
scope int wg_sw_counter;
scope int wg_sw_flag;

/* global command flags for bancomm clock board */

scope int use_IRIGB;        /* True -> use IRIG-B for time source, false ->
      use real time clock as time source */
scope int modulate_IRIGB;   /* True -> use modulated IRIG-B for time source,                                  false -> use DC shift IRIG-B as time source */

scope int clkstart;        /* Tells Bancomm start_clock function not to run
                              designed only for startup.cc, as a safegaurd if
                              TOD board is used again. Will not affect TOD */ 

//Tom 3/20/08
scope int fake_intrs;






