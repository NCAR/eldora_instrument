/***** Sets up the physical parameter constants that are
       used in the header pre-BAMEX Version 1/16/04 ******/

#define scope extern
#include "mkrDef.h"
#include "mkrGbl.h"
#include "mkrFunc.h"

void set_hpa()
{
actual_num_hpas = 2;

/* Applied Systems Engineering Serial Number 1 */

hpa[0].serial_number[0] = 'A';
hpa[0].serial_number[1] = 'S';
hpa[0].serial_number[2] = 'E';
hpa[0].serial_number[3] = '1';
hpa[0].serial_number[4] = ' ';
hpa[0].serial_number[5] = ' ';
hpa[0].serial_number[6] = ' ';
hpa[0].serial_number[7] = ' ';

hpa[0].peak_power[0] = 40.179;    /* nominal peak power in kw 9.3 GHz*/
hpa[0].peak_power[1] = 40.17;    /* nominal peak power in kw 9.4 GHz*/
hpa[0].peak_power[2] = 39.084;    /* nominal peak power in kw 9.5 GHz*/
hpa[0].peak_power[3] = 39.86;    /* nominal peak power in kw 9.6 GHz*/
hpa[0].peak_power[4] = 37.8;     /* nominal peak power in kw 9.7 GHz*/
hpa[0].peak_power[5] = 40.13;    /* nominal peak power in kw 9.8 GHz*/

hpa[0].x_band_gain[0] = 23.0;          /* x-band gain in dB 9.3 GHz */
hpa[0].x_band_gain[1] = 23.0;          /* x-band gain in dB 9.4 GHz */
hpa[0].x_band_gain[2] = 23.0;          /* x-band gain in dB 9.5 GHz */
hpa[0].x_band_gain[3] = 23.0;          /* x-band gain in dB 9.6 GHz */
hpa[0].x_band_gain[4] = 22.1;          /* x-band gain in dB 9.7 GHz */
hpa[0].x_band_gain[5] = 23.0;          /* x-band gain in dB 9.8 GHz */

hpa[0].turn_on_time = 295;     /* in ns */
hpa[0].turn_off_time = 295;    /* in ns */

hpa[0].tr_tube_loss = 0.3;           /* in dB */
hpa[0].peak_power_coupling = 70.35;   /* in dB */

/* hpa[0].test_pulse_coupling = 41.8; */  /* in dB */
hpa[0].test_pulse_coupling = 33.7;  /* in dB */
hpa[0].r_noise_figure = 1.99;        /* in dB */

/* Applied System Engineering Serial Number 2 */

hpa[1].serial_number[0] = 'A';
hpa[1].serial_number[1] = 'S';
hpa[1].serial_number[2] = 'E';
hpa[1].serial_number[3] = '2';
hpa[1].serial_number[4] = ' ';
hpa[1].serial_number[5] = ' ';
hpa[1].serial_number[6] = ' ';
hpa[1].serial_number[7] = ' ';

hpa[1].peak_power[0] = 40.17;    /* nominal peak power in kw 9.3 GHz*/
hpa[1].peak_power[1] = 46.45;    /* nominal peak power in kw 9.4 GHz*/
hpa[1].peak_power[2] = 40.19;    /* nominal peak power in kw 9.5 GHz*/
hpa[1].peak_power[3] = 38.60;    /* nominal peak power in kw 9.6 GHz*/
hpa[1].peak_power[4] = 40.20;    /* nominal peak power in kw 9.7 GHz*/
hpa[1].peak_power[5] = 40.22;    /* nominal peak power in kw 9.8 GHz*/

hpa[1].x_band_gain[0] = 23.0;          /* x-band gain in dB 9.3 GHz */
hpa[1].x_band_gain[1] = 23.0;          /* x-band gain in dB 9.4 GHz */
hpa[1].x_band_gain[2] = 23.0;          /* x-band gain in dB 9.5 GHz */
hpa[1].x_band_gain[3] = 22.4;          /* x-band gain in dB 9.6 GHz */
hpa[1].x_band_gain[4] = 23.0;          /* x-band gain in dB 9.7 GHz */
hpa[1].x_band_gain[5] = 23.0;          /* x-band gain in dB 9.8 GHz */

hpa[1].turn_on_time = 300;     /* in ns */
hpa[1].turn_off_time = 300;    /* in ns */

hpa[1].tr_tube_loss = 0.3;            /* in dB */
hpa[1].peak_power_coupling = 71.70;    /* in dB */
/* hpa[1].test_pulse_coupling = 41.9;  */  /* in dB */
hpa[1].test_pulse_coupling = 32.8 ;   /* in dB */
hpa[1].r_noise_figure = 1.56;         /* in dB */

}
void set_antenna()
{
actual_num_antennas = 2;

antenna[FORE].gain[0] = 38.74;
antenna[FORE].gain[1] = 38.85;
antenna[FORE].gain[2] = 38.96;
antenna[FORE].gain[3] = 39.07;
antenna[FORE].gain[4] = 39.18;
antenna[FORE].gain[5] = 39.3;

antenna[FORE].hbeam_width[0] = 1.85;
antenna[FORE].hbeam_width[1] = 1.83;
antenna[FORE].hbeam_width[2] = 1.81;
antenna[FORE].hbeam_width[3] = 1.79;
antenna[FORE].hbeam_width[4] = 1.77;
antenna[FORE].hbeam_width[5] = 1.75;

antenna[FORE].vbeam_width[0] = 2.1;
antenna[FORE].vbeam_width[1] = 2.07;
antenna[FORE].vbeam_width[2] = 2.04;
antenna[FORE].vbeam_width[3] = 2.01;
antenna[FORE].vbeam_width[4] = 1.98;
antenna[FORE].vbeam_width[5] = 1.95;

antenna[FORE].E_plane_angle[0] = 19.56;
antenna[FORE].E_plane_angle[1] = 18.56;
antenna[FORE].E_plane_angle[2] = 17.57;
antenna[FORE].E_plane_angle[3] = 16.57;
antenna[FORE].E_plane_angle[4] = 15.57;
antenna[FORE].E_plane_angle[5] = 14.57;

antenna[FORE].H_plane_angle[0] = -2.1;
antenna[FORE].H_plane_angle[1] = -1.20;
antenna[FORE].H_plane_angle[2] = -0.30;
antenna[FORE].H_plane_angle[3] = .60;
antenna[FORE].H_plane_angle[4] = 1.50;
antenna[FORE].H_plane_angle[5] = 2.4;

/* For test */
/*
antenna[FORE].H_plane_angle[0] = 0.0;
antenna[FORE].H_plane_angle[1] = 0.0;
antenna[FORE].H_plane_angle[2] = 0.0;
antenna[FORE].H_plane_angle[3] = 0.0;
antenna[FORE].H_plane_angle[4] = 0.0;
antenna[FORE].H_plane_angle[5] = 0.0;
*/
antenna[FORE].tx_waveguide_length = 43.1;  /* in meters  -- this is not actual length but based on wr112 & wr90 & flex losses as if all waveguide is wr112*/ 
antenna[FORE].rx_waveguide_length = 16.5;  /* in meters  -- this is not actual length but based on wr112 & wr90 & flex losses as if all waveguide is wr112*/ 

antenna[FORE].encoder_antenna_up = 96.65354;
/* antenna[FORE].encoder_antenna_up = 0.0;  for test */
antenna[FORE].pitch_antenna_up = 1.5;

antenna[AFT].gain[0] = 38.98;
antenna[AFT].gain[1] = 39.10;
antenna[AFT].gain[2] = 38.23;
antenna[AFT].gain[3] = 39.35;
antenna[AFT].gain[4] = 39.47;
antenna[AFT].gain[5] = 39.59;

antenna[AFT].hbeam_width[0] = 1.85;
antenna[AFT].hbeam_width[1] = 1.83;
antenna[AFT].hbeam_width[2] = 1.81;
antenna[AFT].hbeam_width[3] = 1.79;
antenna[AFT].hbeam_width[4] = 1.77;
antenna[AFT].hbeam_width[5] = 1.74;

antenna[AFT].vbeam_width[0] = 2;
antenna[AFT].vbeam_width[1] = 1.97;
antenna[AFT].vbeam_width[2] = 1.94;
antenna[AFT].vbeam_width[3] = 1.91;
antenna[AFT].vbeam_width[4] = 1.88;
antenna[AFT].vbeam_width[5] = 1.85;

antenna[AFT].E_plane_angle[0] = -19.53;
antenna[AFT].E_plane_angle[1] = -18.53;
antenna[AFT].E_plane_angle[2] = -17.53;
antenna[AFT].E_plane_angle[3] = -16.53;
antenna[AFT].E_plane_angle[4] = -15.53;
antenna[AFT].E_plane_angle[5] = -14.53;

antenna[AFT].H_plane_angle[0] = -2.38;
antenna[AFT].H_plane_angle[1] = -1.5;
antenna[AFT].H_plane_angle[2] = -.62;
antenna[AFT].H_plane_angle[3] = 0.26;
antenna[AFT].H_plane_angle[4] = 1.14;
antenna[AFT].H_plane_angle[5] = 2.02;

antenna[AFT].tx_waveguide_length = 41.9;  /* In meters -- this is not actual length but based on wr112 & wr90 & flex losses as if all waveguide is wr112 */
antenna[AFT].rx_waveguide_length = 15.1;  /* In meters -- this is not actual length but based on wr112 & wr90 & flex losses as if all waveguide is wr112 */

antenna[AFT].encoder_antenna_up = 276.5381;
antenna[AFT].pitch_antenna_up = 1.5;

}

void set_pscheme()
{
actual_num_pschemes = 13;

/* Setup the description of the first pulsing/sampling scheme */

strcpy(pscheme[0].name,"1Frq_1PRF_4Prm");
cleanup(pscheme[0].name,16);

pscheme[0].num_chips[0] = 1;
pscheme[0].num_chips[1] = 0;
pscheme[0].num_chips[2] = 0;
pscheme[0].num_chips[3] = 0;
pscheme[0].num_chips[4] = 0;
pscheme[0].num_chips[5] = 1;

pscheme[0].chip_percent[0] = 0;
pscheme[0].chip_percent[1] = 0;
pscheme[0].chip_percent[2] = 0;
pscheme[0].chip_percent[3] = 0;
pscheme[0].chip_percent[4] = 0;
pscheme[0].chips_in_pulse = 1;

pscheme[0].samples_in_repeat = 1;
pscheme[0].pcp_sets_prf = 1;
pscheme[0].total_pcp = 1;
pscheme[0].blank_chip[0] = 0x21;

pscheme[0].ur_pcp = 1;
pscheme[0].uv_pcp = 1;

pscheme[0].num_params = 4;

strcpy(pscheme[0].param[0].name,"SW");
cleanup(pscheme[0].param[0].name,8);
strcpy(pscheme[0].param[0].units,"m/s");
cleanup(pscheme[0].param[0].units,8);
strcpy(pscheme[0].param[0].description,"Spectral Width");
cleanup(pscheme[0].param[0].description,40);

strcpy(pscheme[0].param[1].name,"VR");
cleanup(pscheme[0].param[1].name,8);
strcpy(pscheme[0].param[1].units,"m/s");
cleanup(pscheme[0].param[1].units,8);
strcpy(pscheme[0].param[1].description,"Radial Velocity");
cleanup(pscheme[0].param[1].description,40);

strcpy(pscheme[0].param[2].name,"NCP");
cleanup(pscheme[0].param[2].name,8);
strcpy(pscheme[0].param[2].units,"none");
cleanup(pscheme[0].param[2].units,8);
strcpy(pscheme[0].param[2].description,"Normalized Coherent Power");
cleanup(pscheme[0].param[2].description,40);

strcpy(pscheme[0].param[3].name,"DBZ");
cleanup(pscheme[0].param[3].name,8);
strcpy(pscheme[0].param[3].units,"dBZ");
cleanup(pscheme[0].param[3].units,8);
strcpy(pscheme[0].param[3].description,"Radar Reflectivity Factor");
cleanup(pscheme[0].param[3].description,40);

pscheme[0].numipps = 1;

pscheme[0].pcp_per_ipp[0] = 1;
pscheme[0].pcp_per_ipp[1] = -999;
pscheme[0].pcp_per_ipp[2] = -999;
pscheme[0].pcp_per_ipp[3] = -999;
pscheme[0].pcp_per_ipp[4] = -999;

/* Setup the description of the second pulsing/sampling scheme */

strcpy(pscheme[1].name,"2Frq_1PRF_4Prm");
cleanup(pscheme[1].name,16);

pscheme[1].num_chips[0] = 1;
pscheme[1].num_chips[1] = 1;
pscheme[1].num_chips[2] = 0;
pscheme[1].num_chips[3] = 0;
pscheme[1].num_chips[4] = 0;
pscheme[1].num_chips[5] = 1;

pscheme[1].chip_percent[0] = 0;
pscheme[1].chip_percent[1] = 50;
pscheme[1].chip_percent[2] = 0;
pscheme[1].chip_percent[3] = 0;
pscheme[1].chip_percent[4] = 0;
pscheme[1].chips_in_pulse = 2;

pscheme[1].samples_in_repeat = 1;
pscheme[1].pcp_sets_prf = 1;
pscheme[1].total_pcp = 1;
pscheme[1].blank_chip[0] = 0x23;

pscheme[1].ur_pcp = 1;
pscheme[1].uv_pcp = 1;

pscheme[1].num_params = 4;

strcpy(pscheme[1].param[0].name,"SW");
cleanup(pscheme[1].param[0].name,8);
strcpy(pscheme[1].param[0].units,"m/s");
cleanup(pscheme[1].param[0].units,8);
strcpy(pscheme[1].param[0].description,"Spectral Width");
cleanup(pscheme[1].param[0].description,40);

strcpy(pscheme[1].param[1].name,"VR");
cleanup(pscheme[1].param[1].name,8);
strcpy(pscheme[1].param[1].units,"m/s");
cleanup(pscheme[1].param[1].units,8);
strcpy(pscheme[1].param[1].description,"Radial Velocity");
cleanup(pscheme[1].param[1].description,40);

strcpy(pscheme[1].param[2].name,"NCP");
cleanup(pscheme[1].param[2].name,8);
strcpy(pscheme[1].param[2].units,"none");
cleanup(pscheme[1].param[2].units,8);
strcpy(pscheme[1].param[2].description,"Normalized Coherent Power");
cleanup(pscheme[1].param[2].description,40);

strcpy(pscheme[1].param[3].name,"DBZ");
cleanup(pscheme[1].param[3].name,8);
strcpy(pscheme[1].param[3].units,"dBZ");
cleanup(pscheme[1].param[3].units,8);
strcpy(pscheme[1].param[3].description,"Radar Reflectivity Factor");
cleanup(pscheme[1].param[3].description,40);

pscheme[1].numipps = 1;

pscheme[1].pcp_per_ipp[0] = 1;
pscheme[1].pcp_per_ipp[1] = -999;
pscheme[1].pcp_per_ipp[2] = -999;
pscheme[1].pcp_per_ipp[3] = -999;
pscheme[1].pcp_per_ipp[4] = -999;

/* Setup the description of the third pulsing/sampling scheme */

strcpy(pscheme[2].name,"3Frq_1PRF_4Prm");
cleanup(pscheme[2].name,16);

pscheme[2].num_chips[0] = 1;
pscheme[2].num_chips[1] = 1;
pscheme[2].num_chips[2] = 1;
pscheme[2].num_chips[3] = 0;
pscheme[2].num_chips[4] = 0;
pscheme[2].num_chips[5] = 1;

pscheme[2].chip_percent[0] = 0;
pscheme[2].chip_percent[1] = 33.3;
pscheme[2].chip_percent[2] = 66.67;
pscheme[2].chip_percent[3] = 0;
pscheme[2].chip_percent[4] = 0;
pscheme[2].chips_in_pulse = 3;

pscheme[2].samples_in_repeat = 1;
pscheme[2].pcp_sets_prf = 1;
pscheme[2].total_pcp = 1;
pscheme[2].blank_chip[0] = 0x27;

pscheme[2].ur_pcp = 1;
pscheme[2].uv_pcp = 1;

pscheme[2].num_params = 4;

strcpy(pscheme[2].param[0].name,"SW");
cleanup(pscheme[2].param[0].name,8);
strcpy(pscheme[2].param[0].units,"m/s");
cleanup(pscheme[2].param[0].units,8);
strcpy(pscheme[2].param[0].description,"Spectral Width");
cleanup(pscheme[2].param[0].description,40);

strcpy(pscheme[2].param[1].name,"VR");
cleanup(pscheme[2].param[1].name,8);
strcpy(pscheme[2].param[1].units,"m/s");
cleanup(pscheme[2].param[1].units,8);
strcpy(pscheme[2].param[1].description,"Radial Velocity");
cleanup(pscheme[2].param[1].description,40);

strcpy(pscheme[2].param[2].name,"NCP");
cleanup(pscheme[2].param[2].name,8);
strcpy(pscheme[2].param[2].units,"none");
cleanup(pscheme[2].param[2].units,8);
strcpy(pscheme[2].param[2].description,"Normalized Coherent Power");
cleanup(pscheme[2].param[2].description,40);

strcpy(pscheme[2].param[3].name,"DBZ");
cleanup(pscheme[2].param[3].name,8);
strcpy(pscheme[2].param[3].units,"dBZ");
cleanup(pscheme[2].param[3].units,8);
strcpy(pscheme[2].param[3].description,"Radar Reflectivity Factor");
cleanup(pscheme[2].param[3].description,40);

pscheme[2].numipps = 1;

pscheme[2].pcp_per_ipp[0] = 1;
pscheme[2].pcp_per_ipp[1] = -999;
pscheme[2].pcp_per_ipp[2] = -999;
pscheme[2].pcp_per_ipp[3] = -999;
pscheme[2].pcp_per_ipp[4] = -999;

/* Setup the description of the fourth pulsing/sampling scheme */

strcpy(pscheme[3].name,"4Frq_1PRF_4Prm");
cleanup(pscheme[3].name,16);

pscheme[3].num_chips[0] = 1;
pscheme[3].num_chips[1] = 1;
pscheme[3].num_chips[2] = 1;
pscheme[3].num_chips[3] = 1;
pscheme[3].num_chips[4] = 0;
pscheme[3].num_chips[5] = 1;

pscheme[3].chip_percent[0] = 0;
pscheme[3].chip_percent[1] = 25;
pscheme[3].chip_percent[2] = 50;
pscheme[3].chip_percent[3] = 75;
pscheme[3].chip_percent[4] = 0;
pscheme[3].chips_in_pulse = 4;

pscheme[3].samples_in_repeat = 1;
pscheme[3].pcp_sets_prf = 1;
pscheme[3].total_pcp = 1;
pscheme[3].blank_chip[0] = 0x2F;

pscheme[3].ur_pcp = 1;
pscheme[3].uv_pcp = 1;

pscheme[3].num_params = 4;

strcpy(pscheme[3].param[0].name,"SW");
cleanup(pscheme[3].param[0].name,8);
strcpy(pscheme[3].param[0].units,"m/s");
cleanup(pscheme[3].param[0].units,8);
strcpy(pscheme[3].param[0].description,"Spectral Width");
cleanup(pscheme[3].param[0].description,40);

strcpy(pscheme[3].param[1].name,"VR");
cleanup(pscheme[3].param[1].name,8);
strcpy(pscheme[3].param[1].units,"m/s");
cleanup(pscheme[3].param[1].units,8);
strcpy(pscheme[3].param[1].description,"Radial Velocity");
cleanup(pscheme[3].param[1].description,40);

strcpy(pscheme[3].param[2].name,"NCP");
cleanup(pscheme[3].param[2].name,8);
strcpy(pscheme[3].param[2].units,"none");
cleanup(pscheme[3].param[2].units,8);
strcpy(pscheme[3].param[2].description,"Normalized Coherent Power");
cleanup(pscheme[3].param[2].description,40);

strcpy(pscheme[3].param[3].name,"DBZ");
cleanup(pscheme[3].param[3].name,8);
strcpy(pscheme[3].param[3].units,"dBZ");
cleanup(pscheme[3].param[3].units,8);
strcpy(pscheme[3].param[3].description,"Radar Reflectivity Factor");
cleanup(pscheme[3].param[3].description,40);

pscheme[3].numipps = 1;

pscheme[3].pcp_per_ipp[0] = 1;
pscheme[3].pcp_per_ipp[1] = -999;
pscheme[3].pcp_per_ipp[2] = -999;
pscheme[3].pcp_per_ipp[3] = -999;
pscheme[3].pcp_per_ipp[4] = -999;

/* Setup the description of the 5th  pulsing/sampling scheme */

strcpy(pscheme[4].name,"5Frq_1PRF_4Prm");
cleanup(pscheme[4].name,16);

pscheme[4].num_chips[0] = 1;
pscheme[4].num_chips[1] = 1;
pscheme[4].num_chips[2] = 1;
pscheme[4].num_chips[3] = 1;
pscheme[4].num_chips[4] = 1;
pscheme[4].num_chips[5] = 1;

pscheme[4].chip_percent[0] = 0;
pscheme[4].chip_percent[1] = 20;
pscheme[4].chip_percent[2] = 40;
pscheme[4].chip_percent[3] = 60;
pscheme[4].chip_percent[4] = 80;
pscheme[4].chips_in_pulse = 5;

pscheme[4].samples_in_repeat = 1;
pscheme[4].pcp_sets_prf = 1;
pscheme[4].total_pcp = 1;
pscheme[4].blank_chip[0] = 0x3F;

pscheme[4].ur_pcp = 1;
pscheme[4].uv_pcp = 1;

pscheme[4].num_params = 4;

strcpy(pscheme[4].param[0].name,"SW");
cleanup(pscheme[4].param[0].name,8);
strcpy(pscheme[4].param[0].units,"m/s");
cleanup(pscheme[4].param[0].units,8);
strcpy(pscheme[4].param[0].description,"Spectral Width");
cleanup(pscheme[4].param[0].description,40);

strcpy(pscheme[4].param[1].name,"VR");
cleanup(pscheme[4].param[1].name,8);
strcpy(pscheme[4].param[1].units,"m/s");
cleanup(pscheme[4].param[1].units,8);
strcpy(pscheme[4].param[1].description,"Radial Velocity");
cleanup(pscheme[4].param[1].description,40);

strcpy(pscheme[4].param[2].name,"NCP");
cleanup(pscheme[4].param[2].name,8);
strcpy(pscheme[4].param[2].units,"none");
cleanup(pscheme[4].param[2].units,8);
strcpy(pscheme[4].param[2].description,"Normalized Coherent Power");
cleanup(pscheme[4].param[2].description,40);

strcpy(pscheme[4].param[3].name,"DBZ");
cleanup(pscheme[4].param[3].name,8);
strcpy(pscheme[4].param[3].units,"dBZ");
cleanup(pscheme[4].param[3].units,8);
strcpy(pscheme[4].param[3].description,"Radar Reflectivity Factor");
cleanup(pscheme[4].param[3].description,40);

pscheme[4].numipps = 1;

pscheme[4].pcp_per_ipp[0] = 1;
pscheme[4].pcp_per_ipp[1] = -999;
pscheme[4].pcp_per_ipp[2] = -999;
pscheme[4].pcp_per_ipp[3] = -999;
pscheme[4].pcp_per_ipp[4] = -999;

/* Setup the description of the sixth pulsing/sampling scheme */

strcpy(pscheme[5].name,"1F_2PRF_10P_5/4");
cleanup(pscheme[5].name,16);

pscheme[5].num_chips[0] = 2;
pscheme[5].num_chips[1] = 0;
pscheme[5].num_chips[2] = 0;
pscheme[5].num_chips[3] = 0;
pscheme[5].num_chips[4] = 0;
pscheme[5].num_chips[5] = 2;

pscheme[5].chip_percent[0] = 0;
pscheme[5].chip_percent[1] = 0;
pscheme[5].chip_percent[2] = 0;
pscheme[5].chip_percent[3] = 0;
pscheme[5].chip_percent[4] = 0;

pscheme[5].chips_in_pulse = 1;

pscheme[5].samples_in_repeat = 2;
pscheme[5].pcp_sets_prf = 5; /* Requested PRF is the low PRF */
pscheme[5].total_pcp = 9;

/* Note: even though we want the long PRT first and the short PRT second,
   set it up with the short PRT first because the first pulse never comes
   out of the timing generator */

pscheme[5].blank_chip[0] = 0x21;
pscheme[5].blank_chip[1] = 0x00;
pscheme[5].blank_chip[2] = 0x00;
pscheme[5].blank_chip[3] = 0x00;
pscheme[5].blank_chip[4] = 0x21;
pscheme[5].blank_chip[5] = 0x00;
pscheme[5].blank_chip[6] = 0x00;
pscheme[5].blank_chip[7] = 0x00;
pscheme[5].blank_chip[8] = 0x00;

pscheme[5].ur_pcp = 4;
pscheme[5].uv_pcp = 1;

pscheme[5].num_params = 10;

strcpy(pscheme[5].param[0].name,"SWS");
cleanup(pscheme[5].param[0].name,8);
strcpy(pscheme[5].param[0].units,"m/s");
cleanup(pscheme[5].param[0].units,8);
strcpy(pscheme[5].param[0].description,"Spectral Width, Short PRT");
cleanup(pscheme[5].param[0].description,40);

strcpy(pscheme[5].param[1].name,"VS");
cleanup(pscheme[5].param[1].name,8);
strcpy(pscheme[5].param[1].units,"m/s");
cleanup(pscheme[5].param[1].units,8);
strcpy(pscheme[5].param[1].description,"Radial Velocity, Short PRT");
cleanup(pscheme[5].param[1].description,40);

strcpy(pscheme[5].param[2].name,"SWL");
cleanup(pscheme[5].param[2].name,8);
strcpy(pscheme[5].param[2].units,"m/s");
cleanup(pscheme[5].param[2].units,8);
strcpy(pscheme[5].param[2].description,"Spectral Width, Long PRT");
cleanup(pscheme[5].param[2].description,40);

strcpy(pscheme[5].param[3].name,"VL");
cleanup(pscheme[5].param[3].name,8);
strcpy(pscheme[5].param[3].units,"m/s");
cleanup(pscheme[5].param[3].units,8);
strcpy(pscheme[5].param[3].description,"Radial Velocity, Long PRT");
cleanup(pscheme[5].param[3].description,40);

strcpy(pscheme[5].param[4].name,"NCPL");
cleanup(pscheme[5].param[4].name,8);
strcpy(pscheme[5].param[4].units,"none");
cleanup(pscheme[5].param[4].units,8);
strcpy(pscheme[5].param[4].description,"Normalized Coherent Power, Long PRT");
cleanup(pscheme[5].param[4].description,40);

strcpy(pscheme[5].param[5].name,"DBZL");
cleanup(pscheme[5].param[5].name,8);
strcpy(pscheme[5].param[5].units,"dBZ");
cleanup(pscheme[5].param[5].units,8);
strcpy(pscheme[5].param[5].description,"Radar Reflectivity Factor, Long PRT");
cleanup(pscheme[5].param[5].description,40);

strcpy(pscheme[5].param[6].name,"SW");
cleanup(pscheme[5].param[6].name,8);
strcpy(pscheme[5].param[6].units,"m/s");
cleanup(pscheme[5].param[6].units,8);
strcpy(pscheme[5].param[6].description,"Spectral Width, Combined");
cleanup(pscheme[5].param[6].description,40);

strcpy(pscheme[5].param[7].name,"VR");
cleanup(pscheme[5].param[7].name,8);
strcpy(pscheme[5].param[7].units,"m/s");
cleanup(pscheme[5].param[7].units,8);
strcpy(pscheme[5].param[7].description,"Radial Velocity, Combined");
cleanup(pscheme[5].param[7].description,40);

strcpy(pscheme[5].param[8].name,"NCP");
cleanup(pscheme[5].param[8].name,8);
strcpy(pscheme[5].param[8].units,"none");
cleanup(pscheme[5].param[8].units,8);
strcpy(pscheme[5].param[8].description,"Normalized Coherent Power, Combined");
cleanup(pscheme[5].param[8].description,40);

strcpy(pscheme[5].param[9].name,"DBZ");
cleanup(pscheme[5].param[9].name,8);
strcpy(pscheme[5].param[9].units,"dBZ");
cleanup(pscheme[5].param[9].units,8);
strcpy(pscheme[5].param[9].description,"Radar Reflectivity Factor, Combined");
cleanup(pscheme[5].param[9].description,40);

pscheme[5].numipps = 2;

pscheme[5].pcp_per_ipp[0] = 4;
pscheme[5].pcp_per_ipp[1] = 5;
pscheme[5].pcp_per_ipp[2] = -999;
pscheme[5].pcp_per_ipp[3] = -999;
pscheme[5].pcp_per_ipp[4] = -999;

/* Setup the description of the seventh pulsing/sampling scheme */

strcpy(pscheme[6].name,"2F_2PRF_10P_5/4");
cleanup(pscheme[6].name,16);

pscheme[6].num_chips[0] = 2;
pscheme[6].num_chips[1] = 2;
pscheme[6].num_chips[2] = 0;
pscheme[6].num_chips[3] = 0;
pscheme[6].num_chips[4] = 0;
pscheme[6].num_chips[5] = 2;

pscheme[6].chip_percent[0] = 0;
pscheme[6].chip_percent[1] = 50;
pscheme[6].chip_percent[2] = 0;
pscheme[6].chip_percent[3] = 0;
pscheme[6].chip_percent[4] = 0;

pscheme[6].chips_in_pulse = 2;

pscheme[6].samples_in_repeat = 2;
pscheme[6].pcp_sets_prf = 5; /* Requested PRF is the low PRF */
pscheme[6].total_pcp = 9;

/* Note: even though we want the long PRT first and the short PRT second,
   set it up with the short PRT first because the first pulse never comes
   out of the timing generator */

pscheme[6].blank_chip[0] = 0x23;
pscheme[6].blank_chip[1] = 0x00;
pscheme[6].blank_chip[2] = 0x00;
pscheme[6].blank_chip[3] = 0x00;
pscheme[6].blank_chip[4] = 0x23;
pscheme[6].blank_chip[5] = 0x00;
pscheme[6].blank_chip[6] = 0x00;
pscheme[6].blank_chip[7] = 0x00;
pscheme[6].blank_chip[8] = 0x00;

pscheme[6].ur_pcp = 4;
pscheme[6].uv_pcp = 1;

pscheme[6].num_params = 10;

strcpy(pscheme[6].param[0].name,"SWS");
cleanup(pscheme[6].param[0].name,8);
strcpy(pscheme[6].param[0].units,"m/s");
cleanup(pscheme[6].param[0].units,8);
strcpy(pscheme[6].param[0].description,"Spectral Width, Short PRT");
cleanup(pscheme[6].param[0].description,40);

strcpy(pscheme[6].param[1].name,"VS");
cleanup(pscheme[6].param[1].name,8);
strcpy(pscheme[6].param[1].units,"m/s");
cleanup(pscheme[6].param[1].units,8);
strcpy(pscheme[6].param[1].description,"Radial Velocity, Short PRT");
cleanup(pscheme[6].param[1].description,40);

strcpy(pscheme[6].param[2].name,"SWL");
cleanup(pscheme[6].param[2].name,8);
strcpy(pscheme[6].param[2].units,"m/s");
cleanup(pscheme[6].param[2].units,8);
strcpy(pscheme[6].param[2].description,"Spectral Width, Long PRT");
cleanup(pscheme[6].param[2].description,40);

strcpy(pscheme[6].param[3].name,"VL");
cleanup(pscheme[6].param[3].name,8);
strcpy(pscheme[6].param[3].units,"m/s");
cleanup(pscheme[6].param[3].units,8);
strcpy(pscheme[6].param[3].description,"Radial Velocity, Long PRT");
cleanup(pscheme[6].param[3].description,40);

strcpy(pscheme[6].param[4].name,"NCPL");
cleanup(pscheme[6].param[4].name,8);
strcpy(pscheme[6].param[4].units,"none");
cleanup(pscheme[6].param[4].units,8);
strcpy(pscheme[6].param[4].description,"Normalized Coherent Power, Long PRT");
cleanup(pscheme[6].param[4].description,40);

strcpy(pscheme[6].param[5].name,"DBZL");
cleanup(pscheme[6].param[5].name,8);
strcpy(pscheme[6].param[5].units,"dBZ");
cleanup(pscheme[6].param[5].units,8);
strcpy(pscheme[6].param[5].description,"Radar Reflectivity Factor, Long PRT");
cleanup(pscheme[6].param[5].description,40);

strcpy(pscheme[6].param[6].name,"SW");
cleanup(pscheme[6].param[6].name,8);
strcpy(pscheme[6].param[6].units,"m/s");
cleanup(pscheme[6].param[6].units,8);
strcpy(pscheme[6].param[6].description,"Spectral Width, Combined");
cleanup(pscheme[6].param[6].description,40);

strcpy(pscheme[6].param[7].name,"VR");
cleanup(pscheme[6].param[7].name,8);
strcpy(pscheme[6].param[7].units,"m/s");
cleanup(pscheme[6].param[7].units,8);
strcpy(pscheme[6].param[7].description,"Radial Velocity, Combined");
cleanup(pscheme[6].param[7].description,40);

strcpy(pscheme[6].param[8].name,"NCP");
cleanup(pscheme[6].param[8].name,8);
strcpy(pscheme[6].param[8].units,"none");
cleanup(pscheme[6].param[8].units,8);
strcpy(pscheme[6].param[8].description,"Normalized Coherent Power, Combined");
cleanup(pscheme[6].param[8].description,40);

strcpy(pscheme[6].param[9].name,"DBZ");
cleanup(pscheme[6].param[9].name,8);
strcpy(pscheme[6].param[9].units,"dBZ");
cleanup(pscheme[6].param[9].units,8);
strcpy(pscheme[6].param[9].description,"Radar Reflectivity Factor, Combined");
cleanup(pscheme[6].param[9].description,40);

pscheme[6].numipps = 2;

pscheme[6].pcp_per_ipp[0] = 4;
pscheme[6].pcp_per_ipp[1] = 5;
pscheme[6].pcp_per_ipp[2] = -999;
pscheme[6].pcp_per_ipp[3] = -999;
pscheme[6].pcp_per_ipp[4] = -999;

/* Setup the description of the eighth pulsing/sampling scheme */

strcpy(pscheme[7].name,"3F_2PRF_10P_5/4");
cleanup(pscheme[7].name,16);

pscheme[7].num_chips[0] = 2;
pscheme[7].num_chips[1] = 2;
pscheme[7].num_chips[2] = 2;
pscheme[7].num_chips[3] = 0;
pscheme[7].num_chips[4] = 0;
pscheme[7].num_chips[5] = 2;

pscheme[7].chip_percent[0] = 0;
pscheme[7].chip_percent[1] = 33.333;
pscheme[7].chip_percent[2] = 66.667;
pscheme[7].chip_percent[3] = 0;
pscheme[7].chip_percent[4] = 0;

pscheme[7].chips_in_pulse = 3;

pscheme[7].samples_in_repeat = 2;
pscheme[7].pcp_sets_prf = 5; /* Requested PRF is the low PRF */
pscheme[7].total_pcp = 9;

/* Note: even though we want the long PRT first and the short PRT second,
   set it up with the short PRT first because the first pulse never comes
   out of the timing generator */

pscheme[7].blank_chip[0] = 0x27;
pscheme[7].blank_chip[1] = 0x00;
pscheme[7].blank_chip[2] = 0x00;
pscheme[7].blank_chip[3] = 0x00;
pscheme[7].blank_chip[4] = 0x27;
pscheme[7].blank_chip[5] = 0x00;
pscheme[7].blank_chip[6] = 0x00;
pscheme[7].blank_chip[7] = 0x00;
pscheme[7].blank_chip[8] = 0x00;

pscheme[7].ur_pcp = 4;
pscheme[7].uv_pcp = 1;

pscheme[7].num_params = 10;

strcpy(pscheme[7].param[0].name,"SWS");
cleanup(pscheme[7].param[0].name,8);
strcpy(pscheme[7].param[0].units,"m/s");
cleanup(pscheme[7].param[0].units,8);
strcpy(pscheme[7].param[0].description,"Spectral Width, Short PRT");
cleanup(pscheme[7].param[0].description,40);

strcpy(pscheme[7].param[1].name,"VS");
cleanup(pscheme[7].param[1].name,8);
strcpy(pscheme[7].param[1].units,"m/s");
cleanup(pscheme[7].param[1].units,8);
strcpy(pscheme[7].param[1].description,"Radial Velocity, Short PRT");
cleanup(pscheme[7].param[1].description,40);

strcpy(pscheme[7].param[2].name,"SWL");
cleanup(pscheme[7].param[2].name,8);
strcpy(pscheme[7].param[2].units,"m/s");
cleanup(pscheme[7].param[2].units,8);
strcpy(pscheme[7].param[2].description,"Spectral Width, Long PRT");
cleanup(pscheme[7].param[2].description,40);

strcpy(pscheme[7].param[3].name,"VL");
cleanup(pscheme[7].param[3].name,8);
strcpy(pscheme[7].param[3].units,"m/s");
cleanup(pscheme[7].param[3].units,8);
strcpy(pscheme[7].param[3].description,"Radial Velocity, Long PRT");
cleanup(pscheme[7].param[3].description,40);

strcpy(pscheme[7].param[4].name,"NCPL");
cleanup(pscheme[7].param[4].name,8);
strcpy(pscheme[7].param[4].units,"none");
cleanup(pscheme[7].param[4].units,8);
strcpy(pscheme[7].param[4].description,"Normalized Coherent Power, Long PRT");
cleanup(pscheme[7].param[4].description,40);

strcpy(pscheme[7].param[5].name,"DBZL");
cleanup(pscheme[7].param[5].name,8);
strcpy(pscheme[7].param[5].units,"dBZ");
cleanup(pscheme[7].param[5].units,8);
strcpy(pscheme[7].param[5].description,"Radar Reflectivity Factor, Long PRT");
cleanup(pscheme[7].param[5].description,40);

strcpy(pscheme[7].param[6].name,"SW");
cleanup(pscheme[7].param[6].name,8);
strcpy(pscheme[7].param[6].units,"m/s");
cleanup(pscheme[7].param[6].units,8);
strcpy(pscheme[7].param[6].description,"Spectral Width, Combined");
cleanup(pscheme[7].param[6].description,40);

strcpy(pscheme[7].param[7].name,"VR");
cleanup(pscheme[7].param[7].name,8);
strcpy(pscheme[7].param[7].units,"m/s");
cleanup(pscheme[7].param[7].units,8);
strcpy(pscheme[7].param[7].description,"Radial Velocity, Combined");
cleanup(pscheme[7].param[7].description,40);

strcpy(pscheme[7].param[8].name,"NCP");
cleanup(pscheme[7].param[8].name,8);
strcpy(pscheme[7].param[8].units,"none");
cleanup(pscheme[7].param[8].units,8);
strcpy(pscheme[7].param[8].description,"Normalized Coherent Power, Combined");
cleanup(pscheme[7].param[8].description,40);

strcpy(pscheme[7].param[9].name,"DBZ");
cleanup(pscheme[7].param[9].name,8);
strcpy(pscheme[7].param[9].units,"dBZ");
cleanup(pscheme[7].param[9].units,8);
strcpy(pscheme[7].param[9].description,"Radar Reflectivity Factor, Combined");
cleanup(pscheme[7].param[9].description,40);

pscheme[7].numipps = 2;

pscheme[7].pcp_per_ipp[0] = 4;
pscheme[7].pcp_per_ipp[1] = 5;
pscheme[7].pcp_per_ipp[2] = -999;
pscheme[7].pcp_per_ipp[3] = -999;
pscheme[7].pcp_per_ipp[4] = -999;

/* Setup the description of the nineth pulsing/sampling scheme */

strcpy(pscheme[8].name,"4F_2PRF_10P_5/4");
cleanup(pscheme[8].name,16);

pscheme[8].num_chips[0] = 2;
pscheme[8].num_chips[1] = 2;
pscheme[8].num_chips[2] = 2;
pscheme[8].num_chips[3] = 2;
pscheme[8].num_chips[4] = 0;
pscheme[8].num_chips[5] = 2;

pscheme[8].chip_percent[0] = 0;
pscheme[8].chip_percent[1] = 25;
pscheme[8].chip_percent[2] = 50;
pscheme[8].chip_percent[3] = 75;
pscheme[8].chip_percent[4] = 0;

pscheme[8].chips_in_pulse = 4;

pscheme[8].samples_in_repeat = 2;
pscheme[8].pcp_sets_prf = 5; /* Requested PRF is the low PRF */
pscheme[8].total_pcp = 9;

/* Note: even though we want the long PRT first and the short PRT second,
   set it up with the short PRT first because the first pulse never comes
   out of the timing generator */

pscheme[8].blank_chip[0] = 0x2F;
pscheme[8].blank_chip[1] = 0x00;
pscheme[8].blank_chip[2] = 0x00;
pscheme[8].blank_chip[3] = 0x00;
pscheme[8].blank_chip[4] = 0x2F;
pscheme[8].blank_chip[5] = 0x00;
pscheme[8].blank_chip[6] = 0x00;
pscheme[8].blank_chip[7] = 0x00;
pscheme[8].blank_chip[8] = 0x00;

pscheme[8].ur_pcp = 4;
pscheme[8].uv_pcp = 1;

pscheme[8].num_params = 10;

strcpy(pscheme[8].param[0].name,"SWS");
cleanup(pscheme[8].param[0].name,8);
strcpy(pscheme[8].param[0].units,"m/s");
cleanup(pscheme[8].param[0].units,8);
strcpy(pscheme[8].param[0].description,"Spectral Width, Short PRT");
cleanup(pscheme[8].param[0].description,40);

strcpy(pscheme[8].param[1].name,"VS");
cleanup(pscheme[8].param[1].name,8);
strcpy(pscheme[8].param[1].units,"m/s");
cleanup(pscheme[8].param[1].units,8);
strcpy(pscheme[8].param[1].description,"Radial Velocity, Short PRT");
cleanup(pscheme[8].param[1].description,40);

strcpy(pscheme[8].param[2].name,"SWL");
cleanup(pscheme[8].param[2].name,8);
strcpy(pscheme[8].param[2].units,"m/s");
cleanup(pscheme[8].param[2].units,8);
strcpy(pscheme[8].param[2].description,"Spectral Width, Long PRT");
cleanup(pscheme[8].param[2].description,40);

strcpy(pscheme[8].param[3].name,"VL");
cleanup(pscheme[8].param[3].name,8);
strcpy(pscheme[8].param[3].units,"m/s");
cleanup(pscheme[8].param[3].units,8);
strcpy(pscheme[8].param[3].description,"Radial Velocity, Long PRT");
cleanup(pscheme[8].param[3].description,40);

strcpy(pscheme[8].param[4].name,"NCPL");
cleanup(pscheme[8].param[4].name,8);
strcpy(pscheme[8].param[4].units,"none");
cleanup(pscheme[8].param[4].units,8);
strcpy(pscheme[8].param[4].description,"Normalized Coherent Power, Long PRT");
cleanup(pscheme[8].param[4].description,40);

strcpy(pscheme[8].param[5].name,"DBZL");
cleanup(pscheme[8].param[5].name,8);
strcpy(pscheme[8].param[5].units,"dBZ");
cleanup(pscheme[8].param[5].units,8);
strcpy(pscheme[8].param[5].description,"Radar Reflectivity Factor, Long PRT");
cleanup(pscheme[8].param[5].description,40);

strcpy(pscheme[8].param[6].name,"SW");
cleanup(pscheme[8].param[6].name,8);
strcpy(pscheme[8].param[6].units,"m/s");
cleanup(pscheme[8].param[6].units,8);
strcpy(pscheme[8].param[6].description,"Spectral Width, Combined");
cleanup(pscheme[8].param[6].description,40);

strcpy(pscheme[8].param[7].name,"VR");
cleanup(pscheme[8].param[7].name,8);
strcpy(pscheme[8].param[7].units,"m/s");
cleanup(pscheme[8].param[7].units,8);
strcpy(pscheme[8].param[7].description,"Radial Velocity, Combined");
cleanup(pscheme[8].param[7].description,40);

strcpy(pscheme[8].param[8].name,"NCP");
cleanup(pscheme[8].param[8].name,8);
strcpy(pscheme[8].param[8].units,"none");
cleanup(pscheme[8].param[8].units,8);
strcpy(pscheme[8].param[8].description,"Normalized Coherent Power, Combined");
cleanup(pscheme[8].param[8].description,40);

strcpy(pscheme[8].param[9].name,"DBZ");
cleanup(pscheme[8].param[9].name,8);
strcpy(pscheme[8].param[9].units,"dBZ");
cleanup(pscheme[8].param[9].units,8);
strcpy(pscheme[8].param[9].description,"Radar Reflectivity Factor, Combined");
cleanup(pscheme[8].param[9].description,40);

pscheme[8].numipps = 2;

pscheme[8].pcp_per_ipp[0] = 4;
pscheme[8].pcp_per_ipp[1] = 5;
pscheme[8].pcp_per_ipp[2] = -999;
pscheme[8].pcp_per_ipp[3] = -999;
pscheme[8].pcp_per_ipp[4] = -999;


/* Setup the description of the tenth  pulsing/sampling scheme */

strcpy(pscheme[9].name,"1F_2PRF_6P_5/4");
cleanup(pscheme[9].name,16);

pscheme[9].num_chips[0] = 2;
pscheme[9].num_chips[1] = 0;
pscheme[9].num_chips[2] = 0;
pscheme[9].num_chips[3] = 0;
pscheme[9].num_chips[4] = 0;
pscheme[9].num_chips[5] = 2;

pscheme[9].chip_percent[0] = 0;
pscheme[9].chip_percent[1] = 0;
pscheme[9].chip_percent[2] = 0;
pscheme[9].chip_percent[3] = 0;
pscheme[9].chip_percent[4] = 0;

pscheme[9].chips_in_pulse = 1;

pscheme[9].samples_in_repeat = 2;
pscheme[9].pcp_sets_prf = 5;
pscheme[9].total_pcp = 9;


/* Note: even though we want the long PRT first and the short PRT second,
   set it up with the short PRT first because the first pulse never comes
   out of the timing generator */

pscheme[9].blank_chip[0] = 0x21;
pscheme[9].blank_chip[1] = 0x00;
pscheme[9].blank_chip[2] = 0x00;
pscheme[9].blank_chip[3] = 0x00;
pscheme[9].blank_chip[4] = 0x21;
pscheme[9].blank_chip[5] = 0x00;
pscheme[9].blank_chip[6] = 0x00;
pscheme[9].blank_chip[7] = 0x00;
pscheme[9].blank_chip[8] = 0x00;

pscheme[9].ur_pcp = 4;
pscheme[9].uv_pcp = 1;

pscheme[9].num_params = 6;

strcpy(pscheme[9].param[0].name,"VS");
cleanup(pscheme[9].param[0].name,8);
strcpy(pscheme[9].param[0].units,"m/s");
cleanup(pscheme[9].param[0].units,8);
strcpy(pscheme[9].param[0].description,"Radial Velocity, Short PRT");
cleanup(pscheme[9].param[0].description,40);

strcpy(pscheme[9].param[1].name,"VL");
cleanup(pscheme[9].param[1].name,8);
strcpy(pscheme[9].param[1].units,"m/s");
cleanup(pscheme[9].param[1].units,8);
strcpy(pscheme[9].param[1].description,"Radial Velocity, Long PRT");
cleanup(pscheme[9].param[1].description,40);

strcpy(pscheme[9].param[2].name,"SW");
cleanup(pscheme[9].param[2].name,8);
strcpy(pscheme[9].param[2].units,"m/s");
cleanup(pscheme[9].param[2].units,8);
strcpy(pscheme[9].param[2].description,"Spectral Width, Combined");
cleanup(pscheme[9].param[2].description,40);

strcpy(pscheme[9].param[3].name,"VR");
cleanup(pscheme[9].param[3].name,8);
strcpy(pscheme[9].param[3].units,"m/s");
cleanup(pscheme[9].param[3].units,8);
strcpy(pscheme[9].param[3].description,"Radial Velocity, Combined");
cleanup(pscheme[9].param[3].description,40);

strcpy(pscheme[9].param[4].name,"NCP");
cleanup(pscheme[9].param[4].name,8);
strcpy(pscheme[9].param[4].units,"none");
cleanup(pscheme[9].param[4].units,8);
strcpy(pscheme[9].param[4].description,"Normalized Coherent Power, Combined");
cleanup(pscheme[9].param[4].description,40);

strcpy(pscheme[9].param[5].name,"DBZ");
cleanup(pscheme[9].param[5].name,8);
strcpy(pscheme[9].param[5].units,"dBZ");
cleanup(pscheme[9].param[5].units,8);
strcpy(pscheme[9].param[5].description,"Radar Reflectivity Factor, Combined");
cleanup(pscheme[9].param[5].description,40);

pscheme[9].numipps = 2;

pscheme[9].pcp_per_ipp[0] = 4;
pscheme[9].pcp_per_ipp[1] = 5;
pscheme[9].pcp_per_ipp[2] = -999;
pscheme[9].pcp_per_ipp[3] = -999;
pscheme[9].pcp_per_ipp[4] = -999;


/* Setup the description of the eleventh  pulsing/sampling scheme */

strcpy(pscheme[10].name,"2F_2PRF_6P_5/4");
cleanup(pscheme[10].name,16);

pscheme[10].num_chips[0] = 2;
pscheme[10].num_chips[1] = 2;
pscheme[10].num_chips[2] = 0;
pscheme[10].num_chips[3] = 0;
pscheme[10].num_chips[4] = 0;
pscheme[10].num_chips[5] = 2;

pscheme[10].chip_percent[0] = 0;
pscheme[10].chip_percent[1] = 50;
pscheme[10].chip_percent[2] = 0;
pscheme[10].chip_percent[3] = 0;
pscheme[10].chip_percent[4] = 0;

pscheme[10].chips_in_pulse = 2;

pscheme[10].samples_in_repeat = 2;
pscheme[10].pcp_sets_prf = 5;
pscheme[10].total_pcp = 9;


/* Note: even though we want the long PRT first and the short PRT second,
   set it up with the short PRT first because the first pulse never comes
   out of the timing generator */

pscheme[10].blank_chip[0] = 0x23;
pscheme[10].blank_chip[1] = 0x00;
pscheme[10].blank_chip[2] = 0x00;
pscheme[10].blank_chip[3] = 0x00;
pscheme[10].blank_chip[4] = 0x23;
pscheme[10].blank_chip[5] = 0x00;
pscheme[10].blank_chip[6] = 0x00;
pscheme[10].blank_chip[7] = 0x00;
pscheme[10].blank_chip[8] = 0x00;

pscheme[10].ur_pcp = 4;
pscheme[10].uv_pcp = 1;

pscheme[10].num_params = 6;

strcpy(pscheme[10].param[0].name,"VS");
cleanup(pscheme[10].param[0].name,8);
strcpy(pscheme[10].param[0].units,"m/s");
cleanup(pscheme[10].param[0].units,8);
strcpy(pscheme[10].param[0].description,"Radial Velocity, Short PRT");
cleanup(pscheme[10].param[0].description,40);

strcpy(pscheme[10].param[1].name,"VL");
cleanup(pscheme[10].param[1].name,8);
strcpy(pscheme[10].param[1].units,"m/s");
cleanup(pscheme[10].param[1].units,8);
strcpy(pscheme[10].param[1].description,"Radial Velocity, Long PRT");
cleanup(pscheme[10].param[1].description,40);

strcpy(pscheme[10].param[2].name,"SW");
cleanup(pscheme[10].param[2].name,8);
strcpy(pscheme[10].param[2].units,"m/s");
cleanup(pscheme[10].param[2].units,8);
strcpy(pscheme[10].param[2].description,"Spectral Width, Combined");
cleanup(pscheme[10].param[2].description,40);

strcpy(pscheme[10].param[3].name,"VR");
cleanup(pscheme[10].param[3].name,8);
strcpy(pscheme[10].param[3].units,"m/s");
cleanup(pscheme[10].param[3].units,8);
strcpy(pscheme[10].param[3].description,"Radial Velocity, Combined");
cleanup(pscheme[10].param[3].description,40);

strcpy(pscheme[10].param[4].name,"NCP");
cleanup(pscheme[10].param[4].name,8);
strcpy(pscheme[10].param[4].units,"none");
cleanup(pscheme[10].param[4].units,8);
strcpy(pscheme[10].param[4].description,"Normalized Coherent Power, Combined");
cleanup(pscheme[10].param[4].description,40);

strcpy(pscheme[10].param[5].name,"DBZ");
cleanup(pscheme[10].param[5].name,8);
strcpy(pscheme[10].param[5].units,"dBZ");
cleanup(pscheme[10].param[5].units,8);
strcpy(pscheme[10].param[5].description,"Radar Reflectivity Factor, Combined");
cleanup(pscheme[10].param[5].description,40);

pscheme[10].numipps = 2;

pscheme[10].pcp_per_ipp[0] = 4;
pscheme[10].pcp_per_ipp[1] = 5;
pscheme[10].pcp_per_ipp[2] = -999;
pscheme[10].pcp_per_ipp[3] = -999;
pscheme[10].pcp_per_ipp[4] = -999;


/* Setup the description of the twelveth  pulsing/sampling scheme */

strcpy(pscheme[11].name,"3F_2PRF_6P_5/4");
cleanup(pscheme[11].name,16);

pscheme[11].num_chips[0] = 2;
pscheme[11].num_chips[1] = 2;
pscheme[11].num_chips[2] = 2;
pscheme[11].num_chips[3] = 0;
pscheme[11].num_chips[4] = 0;
pscheme[11].num_chips[5] = 2;

pscheme[11].chip_percent[0] = 0;
pscheme[11].chip_percent[1] = 33.33;
pscheme[11].chip_percent[2] = 66,67;
pscheme[11].chip_percent[3] = 0;
pscheme[11].chip_percent[4] = 0;

pscheme[11].chips_in_pulse = 3;

pscheme[11].samples_in_repeat = 2;
pscheme[11].pcp_sets_prf = 5;
pscheme[11].total_pcp = 9;


/* Note: even though we want the long PRT first and the short PRT second,
   set it up with the short PRT first because the first pulse never comes
   out of the timing generator */

pscheme[11].blank_chip[0] = 0x27;
pscheme[11].blank_chip[1] = 0x00;
pscheme[11].blank_chip[2] = 0x00;
pscheme[11].blank_chip[3] = 0x00;
pscheme[11].blank_chip[4] = 0x27;
pscheme[11].blank_chip[5] = 0x00;
pscheme[11].blank_chip[6] = 0x00;
pscheme[11].blank_chip[7] = 0x00;
pscheme[11].blank_chip[8] = 0x00;

pscheme[11].ur_pcp = 4;
pscheme[11].uv_pcp = 1;

pscheme[11].num_params = 6;

strcpy(pscheme[11].param[0].name,"VS");
cleanup(pscheme[11].param[0].name,8);
strcpy(pscheme[11].param[0].units,"m/s");
cleanup(pscheme[11].param[0].units,8);
strcpy(pscheme[11].param[0].description,"Radial Velocity, Short PRT");
cleanup(pscheme[11].param[0].description,40);

strcpy(pscheme[11].param[1].name,"VL");
cleanup(pscheme[11].param[1].name,8);
strcpy(pscheme[11].param[1].units,"m/s");
cleanup(pscheme[11].param[1].units,8);
strcpy(pscheme[11].param[1].description,"Radial Velocity, Long PRT");
cleanup(pscheme[11].param[1].description,40);

strcpy(pscheme[11].param[2].name,"SW");
cleanup(pscheme[11].param[2].name,8);
strcpy(pscheme[11].param[2].units,"m/s");
cleanup(pscheme[11].param[2].units,8);
strcpy(pscheme[11].param[2].description,"Spectral Width, Combined");
cleanup(pscheme[11].param[2].description,40);

strcpy(pscheme[11].param[3].name,"VR");
cleanup(pscheme[11].param[3].name,8);
strcpy(pscheme[11].param[3].units,"m/s");
cleanup(pscheme[11].param[3].units,8);
strcpy(pscheme[11].param[3].description,"Radial Velocity, Combined");
cleanup(pscheme[11].param[3].description,40);

strcpy(pscheme[11].param[4].name,"NCP");
cleanup(pscheme[11].param[4].name,8);
strcpy(pscheme[11].param[4].units,"none");
cleanup(pscheme[11].param[4].units,8);
strcpy(pscheme[11].param[4].description,"Normalized Coherent Power, Combined");
cleanup(pscheme[11].param[4].description,40);

strcpy(pscheme[11].param[5].name,"DBZ");
cleanup(pscheme[11].param[5].name,8);
strcpy(pscheme[11].param[5].units,"dBZ");
cleanup(pscheme[11].param[5].units,8);
strcpy(pscheme[11].param[5].description,"Radar Reflectivity Factor, Combined");
cleanup(pscheme[11].param[5].description,40);

pscheme[11].numipps = 2;

pscheme[11].pcp_per_ipp[0] = 4;
pscheme[11].pcp_per_ipp[1] = 5;
pscheme[11].pcp_per_ipp[2] = -999;
pscheme[11].pcp_per_ipp[3] = -999;
pscheme[11].pcp_per_ipp[4] = -999;


/* Setup the description of the thirteenth  pulsing/sampling scheme */

strcpy(pscheme[12].name,"4F_2PRF_6P_5/4");
cleanup(pscheme[12].name,16);

pscheme[12].num_chips[0] = 2;
pscheme[12].num_chips[1] = 2;
pscheme[12].num_chips[2] = 2;
pscheme[12].num_chips[3] = 2;
pscheme[12].num_chips[4] = 0;
pscheme[12].num_chips[5] = 2;

pscheme[12].chip_percent[0] = 0;
pscheme[12].chip_percent[1] = 25;
pscheme[12].chip_percent[2] = 50;
pscheme[12].chip_percent[3] = 75;
pscheme[12].chip_percent[4] = 0;

pscheme[12].chips_in_pulse = 4;

pscheme[12].samples_in_repeat = 2;
pscheme[12].pcp_sets_prf = 5;
pscheme[12].total_pcp = 9;


/* Note: even though we want the long PRT first and the short PRT second,
   set it up with the short PRT first because the first pulse never comes
   out of the timing generator */

pscheme[12].blank_chip[0] = 0x2F;
pscheme[12].blank_chip[1] = 0x00;
pscheme[12].blank_chip[2] = 0x00;
pscheme[12].blank_chip[3] = 0x00;
pscheme[12].blank_chip[4] = 0x2F;
pscheme[12].blank_chip[5] = 0x00;
pscheme[12].blank_chip[6] = 0x00;
pscheme[12].blank_chip[7] = 0x00;
pscheme[12].blank_chip[8] = 0x00;

pscheme[12].ur_pcp = 4;
pscheme[12].uv_pcp = 1;

pscheme[12].num_params = 6;

strcpy(pscheme[12].param[0].name,"VS");
cleanup(pscheme[12].param[0].name,8);
strcpy(pscheme[12].param[0].units,"m/s");
cleanup(pscheme[12].param[0].units,8);
strcpy(pscheme[12].param[0].description,"Radial Velocity, Short PRT");
cleanup(pscheme[12].param[0].description,40);

strcpy(pscheme[12].param[1].name,"VL");
cleanup(pscheme[12].param[1].name,8);
strcpy(pscheme[12].param[1].units,"m/s");
cleanup(pscheme[12].param[1].units,8);
strcpy(pscheme[12].param[1].description,"Radial Velocity, Long PRT");
cleanup(pscheme[12].param[1].description,40);

strcpy(pscheme[12].param[2].name,"SW");
cleanup(pscheme[12].param[2].name,8);
strcpy(pscheme[12].param[2].units,"m/s");
cleanup(pscheme[12].param[2].units,8);
strcpy(pscheme[12].param[2].description,"Spectral Width, Combined");
cleanup(pscheme[12].param[2].description,40);

strcpy(pscheme[12].param[3].name,"VR");
cleanup(pscheme[12].param[3].name,8);
strcpy(pscheme[12].param[3].units,"m/s");
cleanup(pscheme[12].param[3].units,8);
strcpy(pscheme[12].param[3].description,"Radial Velocity, Combined");
cleanup(pscheme[12].param[3].description,40);

strcpy(pscheme[12].param[4].name,"NCP");
cleanup(pscheme[12].param[4].name,8);
strcpy(pscheme[12].param[4].units,"none");
cleanup(pscheme[12].param[4].units,8);
strcpy(pscheme[12].param[4].description,"Normalized Coherent Power, Combined");
cleanup(pscheme[12].param[4].description,40);

strcpy(pscheme[12].param[5].name,"DBZ");
cleanup(pscheme[12].param[5].name,8);
strcpy(pscheme[12].param[5].units,"dBZ");
cleanup(pscheme[12].param[5].units,8);
strcpy(pscheme[12].param[5].description,"Radar Reflectivity Factor, Combined");
cleanup(pscheme[12].param[5].description,40);

pscheme[12].numipps = 2;

pscheme[12].pcp_per_ipp[0] = 4;
pscheme[12].pcp_per_ipp[1] = 5;
pscheme[12].pcp_per_ipp[2] = -999;
pscheme[12].pcp_per_ipp[3] = -999;
pscheme[12].pcp_per_ipp[4] = -999;


}

/***** This routine sets some basic physical constants of the receivers ****/

void set_rcvr()
{
int i;

actual_num_rcvrs = 2;

rcvr[FORE].cntr_freq = 9.7;

rcvr[FORE].receiver_gain[0] = 30.30;
rcvr[FORE].receiver_gain[1] = 30.40;
rcvr[FORE].receiver_gain[2] = 30.40;
rcvr[FORE].receiver_gain[3] = 30.80;
rcvr[FORE].receiver_gain[4] = 30.00;

rcvr[FORE].if_gain[0] = 0.0;
rcvr[FORE].if_gain[1] = 0.0;
rcvr[FORE].if_gain[2] = 0.0;
rcvr[FORE].if_gain[3] = 0.0;
rcvr[FORE].if_gain[4] = 0.0;

rcvr[FORE].delay_chip_rcvrs = 450;    /* 8 MHz anti-aliasing filter */

for(i=0; i<5; i++)
rcvr[FORE].freq_indexs[i] = (rcvr[FORE].cntr_freq - 9.25) * 10.0;

rcvr[AFT].cntr_freq = 9.6;

rcvr[AFT].receiver_gain[0] = 30.30;
rcvr[AFT].receiver_gain[1] = 31.50;
rcvr[AFT].receiver_gain[2] = 31.30;
rcvr[AFT].receiver_gain[3] = 31.80;
rcvr[AFT].receiver_gain[4] = 30.00;

rcvr[AFT].if_gain[0] = 0.0;
rcvr[AFT].if_gain[1] = 0.0;
rcvr[AFT].if_gain[2] = 0.0;
rcvr[AFT].if_gain[3] = 0.0;
rcvr[AFT].if_gain[4] = 0.0;

rcvr[AFT].delay_chip_rcvrs = 450;    /* 8 MHz anti-aliasing filter */

for(i=0; i<5; i++)
  rcvr[AFT].freq_indexs[i] = (rcvr[AFT].cntr_freq - 9.25) * 10.0;

}

void set_attenuator()
{
    atten[FORE].loss_rawtp_hpa = 0.0;  /* need to measure */
    atten[FORE].loss_rawtp_powermeter = 0.0;

    atten[AFT].loss_rawtp_hpa = 0.0; /* need to measure */
    atten[AFT].loss_rawtp_powermeter = 0.0;
}


