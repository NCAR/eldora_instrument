/*    Module:     hpai_var.h 
      Author:     Craig Walther 
      Copyright:  National Center For Atmospheric Research 
      Date:       October 31, 1991
      Purpose:    This include module contains all of the global 
                  variables that are used with the high power 
                  amplifier's controller software 
*/ 
 
/* Define some general purpose parameters */ 
 
scope short   mode,rlmode;       /* Current mode of the HPAI, Remote or local */ 
scope long    ticks;             /* Number of timer ticks since turn on */ 
scope short   error,terror;       /* Master error flag, temporary error flag */ 
scope short   numchar;            /* Number of characters received from port B */ 
scope short   timer_int;          /* Timer interrupt has occured flag */ 
scope char    port_b_buf[90];     /* Storage for commands from port B */ 
scope long    ticks_at_operate;   /* Tick counter when ok to operate */ 
scope long    ticks_when_off;     /* Tick counter at operate to standby trans */ 
scope long    ticks_check_heat;  /* Tick counter when ok to check heater I&V */ 
scope long    ticks_check_screen; /* Tick counter when ok to check pass
                                     tube screen */ 
scope long    ticks_check_supplies; /* Tick counter when ok to check +5V and
                                       both +15V power supplies */
scope long    ticks_check_body;    /* tick counter when body supply should be
                                      within tight limits */
scope short   video_on_flag;       /* Flag to turn video on after body supply is
                                      full up. */
scope short   system_inter_pulsed_flag;   /* flag to pulse systems interface
                                     after the twt heater is up in standby
                                     this clears indicated errors that are
                                     no longer happening */  
scope long    local_sw_debounce;  /* local switch debounce counter */
scope short   skip_flg;           /* Skip No/Yes inputs when scrolling display */
scope short   point_at_dummyload; /* Flag to point waveguide switch
                                      at dummyload */ 
scope short   dmy_load_state;     /* Dummy Load state => True means waveguide
                                     pointing to dummy load */  
scope long    ticks_to_warm;      /* number of ticks to sit in standby state */ 
scope char    port_command;       /* Same format as "switches" 
                                              (using port commands) */ 
scope char    commands;           /* Combination of "switches" and "port_command" 
                                        based on rlmode */
scope short   fprint_cnt;         /* Fault print counter */
scope short   warmup_time;        /* Time to wait for TWT to warm up (seconds) */
scope short   twt_over_temp_flag; /* Flag to hold HPA in off until twt over
                                     temperature fault is acknowledged */
scope short   print_cnt;          /* Output print counter */
scope short   main_p_rly;         /* Flag true if relay closed false otherwise */ 

scope short   body_p_rly;         /* Flag true if relay closed false otherwise */ 

scope short   collector_p_rly;    /* Flag true if relay closed false otherwise */ 

scope short   aux_p_rly;          /* Flag true if relay closed false otherwise */ 

scope short   blower_p_rly;       /* Flag true if relay closed false otherwise */
scope short   hot_resist_rly;     /* Flag true if relay closed false otherwise */
 
/* Parameters used to write correctly on the Day-Star Nova Display */ 
 
scope short   status_print;      /* Flag is TRUE if printing status */ 
scope char    status_buf[22];    /* Contains current status info */ 
scope short   ready_flag;        /* Flag is FALSE before TWT heated up */ 

/* Areas to save current setups of each multi-control word */ 
 
scope short curr_relay_0;    /* Current setup of relay card 0 */ 
scope short curr_relay_1;    /* Current setup of relay card 1 */ 
scope char curr_pio_d;       /* Current setup of PIO port d */ 
scope char curr_pio_e;       /* Current setup of PIO port e */ 
 
/* Analog input channel information */ 
 
scope short  ad_samples[16];  /* Actual A/D card outputs */ 
scope char   ad_chan[16];     /* A/D channel numbers */ 
scope char   ad_gain[16];     /* A/D card gains */ 
scope short  offset_cal;      /* Amplifier offset to remove from A/D samples */ 
scope float  ad_mult[16];     /* A/D channel multiplier (for calibrations) */ 
scope float  ad_offset[16];   /* A/D channel offset (for calibrations) */ 
scope float  ad_min[16];      /* Minimum good A/D channel value */ 
scope float  ad_max[16];      /* Maximum good A/D channel value */ 
 
/* Optical input channel information */ 
 
scope long   oi_old[12];      /* Last value of the each optical input */ 
scope float  oi_samples[12];  /* Actual optical input card readings */ 
scope long   pmsecs;          /* Present number of milliseconds since power on */ 
scope long   lmsecs;          /* Number of milliseconds at last optical sample */ 
scope float  oi_mult[12];     /* Optical Input multiplier (for calibrations) */ 
scope float  oi_min[12];      /* Minimum good optical input values */ 
scope float  oi_max[12];      /* Maximum good optical input values */ 
 
/* Parallel Input card information */ 
 
scope unsigned char no_yes_0,no_yes_1;  /* Input byte from porta A and B */
scope unsigned char no_yes_2,switches;  /* Input byte from ports C and F */