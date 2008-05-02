 
#include <stdio.h>
#ifndef linux
#include <floatingpoint.h>
#endif
#include <math.h>

/* Declare the main header structures that will
   contain the actual parameters */

scope struct volume_d volume;
scope struct waveform_d waveform;
scope struct radar_d  radar[MAX_RADAR];
scope struct cell_spacing_d cell[MAX_RADAR];
scope struct field_radar_i  fradar[MAX_RADAR];
scope struct parameter_d parameter[MAX_PARAM][MAX_RADAR];
scope struct nav_descript navigation;
scope struct insitu_descript insitudata;
scope struct engineer engin_win;

/* Declare the state structures that will hold the state of each parameter */ 

scope struct waveform_d waveform_st;
scope struct radar_d  radar_st;
scope struct cell_spacing_d cell_st;
scope struct field_radar_i  fradar_st;
scope struct parameter_d param_st;
scope struct engineer engin_st;

/* Declare some general purpose global variables */

scope int actual_num_radars;
scope int actual_num_params[MAX_RADAR];
scope int override;
scope char inname[80];

/* Declare structures/variables that hold the physical constants */

scope int actual_num_hpas;
scope struct hpa_s  hpa[MAX_HPA];

scope int actual_num_rcvrs;
scope struct rcvr_s  rcvr[MAX_RCVR];
scope struct attenuator atten[MAX_RCVR];

scope int actual_num_antennas;
scope struct antenna_s  antenna[MAX_ANTENNA];

scope int actual_num_pschemes;
scope struct pscheme_s  pscheme[MAX_PSCHEME];

scope int processor_type;
