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

scope int actual_num_radars;
scope int actual_num_params[MAX_RADAR];

scope char inname[80];
