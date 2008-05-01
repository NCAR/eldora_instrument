/*
 *      $Id$
 *
 *      Module:          eldoraSwap.c
 *      Original Author: Richard Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *      Date:            $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:
 *        Converts from network byte order to little endian, for Intel,
 * etc.
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <netinet/in.h>
#include <string.h>
#include "Volume.h"
#include "RadarDesc.h"
#include "Parameter.h"
#include "Platform.h"
#include "CellSpacing.h"
#include "Waveform.h"
#include "FieldRadar.h"
#include "NavDesc.h"
#include "InSitu.h"

static float ntohf(float);

static float ntohf(float f)
{
  int *i = (int *)&f;

  unsigned long j = ntohl(*i);

  float *p = (float *)&j;

  return *p;
}

swapVolume(VOLUME *ptr)
{
  ptr->volume_des_length = ntohl(ptr->volume_des_length);
  ptr->format_version = ntohs(ptr->format_version);
  ptr->volume_num = ntohs(ptr->volume_num);
  ptr->maximum_bytes = ntohl(ptr->maximum_bytes);
  ptr->year = ntohs(ptr->year);
  ptr->month = ntohs(ptr->month);
  ptr->day = ntohs(ptr->day);
  ptr->data_set_hour = ntohs(ptr->data_set_hour);
  ptr->data_set_minute = ntohs(ptr->data_set_minute);
  ptr->data_set_second = ntohs(ptr->data_set_second);
  ptr->gen_year = ntohs(ptr->gen_year);
  ptr->gen_month = ntohs(ptr->gen_month);
  ptr->gen_day = ntohs(ptr->gen_day);
  ptr->number_sensor_des = ntohs(ptr->number_sensor_des);
}

swapFieldRadar(FIELDRADAR *ptr)
{
  int i;

  ptr->field_radar_info_len = ntohl(ptr->field_radar_info_len);
  ptr->data_sys_id = ntohl(ptr->data_sys_id);
  ptr->loss_out = ntohf(ptr->loss_out);
  ptr->loss_in = ntohf(ptr->loss_in);
  ptr->loss_rjoint = ntohf(ptr->loss_rjoint);
  ptr->ant_v_dim = ntohf(ptr->ant_v_dim);
  ptr->ant_h_dim = ntohf(ptr->ant_h_dim);
  ptr->ant_noise_temp = ntohf(ptr->ant_noise_temp);
  ptr->r_noise_figure = ntohf(ptr->r_noise_figure);
  ptr->x_band_gain = ntohf(ptr->x_band_gain);
  ptr->conversion_gain = ntohf(ptr->conversion_gain);
  ptr->processor_const = ntohf(ptr->processor_const);
  ptr->dly_tube_antenna = ntohl(ptr->dly_tube_antenna);
  ptr->dly_rndtrip_chip_atod = ntohl(ptr->dly_rndtrip_chip_atod);
  ptr->dly_timmod_testpulse = ntohl(ptr->dly_timmod_testpulse);
  ptr->dly_modulator_on = ntohl(ptr->dly_modulator_on);
  ptr->dly_modulator_off = ntohl(ptr->dly_modulator_off);
  ptr->peak_power_offset = ntohf(ptr->peak_power_offset);
  ptr->test_pulse_offset = ntohf(ptr->test_pulse_offset);
  ptr->E_plane_angle = ntohf(ptr->E_plane_angle);
  ptr->H_plane_angle = ntohf(ptr->H_plane_angle);
  ptr->encoder_antenna_up = ntohf(ptr->encoder_antenna_up);
  ptr->pitch_antenna_up = ntohf(ptr->pitch_antenna_up);
  ptr->indepf_times_flg = ntohs(ptr->indepf_times_flg);
  ptr->indep_freq_gate = ntohs(ptr->indep_freq_gate);
  ptr->time_series_gate = ntohs(ptr->time_series_gate);
  ptr->num_base_params = ntohs(ptr->num_base_params);

  for (i = 0; i < 5; i++)
    {
      ptr->scale_factor[i] = ntohf(ptr->scale_factor[i]);
      ptr->xmit_power[i] = ntohf(ptr->xmit_power[i]);
      ptr->receiver_gain[i] = ntohf(ptr->receiver_gain[i]);
      ptr->if_gain[i] = ntohf(ptr->if_gain[i]);  
    }
}
swapRadardesc(RADARDESC *ptr)
{
  ptr->radar_des_length = htonl(ptr->radar_des_length);
  ptr->radar_const = ntohf(ptr->radar_const);
  ptr->peak_power = ntohf(ptr->peak_power);
  ptr->noise_power = ntohf(ptr->noise_power);
  ptr->receiver_gain = ntohf(ptr->receiver_gain);
  ptr->antenna_gain = ntohf(ptr->antenna_gain);
  ptr->system_gain = ntohf(ptr->system_gain);
  ptr->horz_beam_width = ntohf(ptr->horz_beam_width);
  ptr->vert_beam_width = ntohf(ptr->vert_beam_width);
  ptr->radar_type = ntohs(ptr->radar_type);
  ptr->scan_mode = ntohs(ptr->scan_mode);
  ptr->req_rotat_vel = ntohf(ptr->req_rotat_vel);
  ptr->scan_mode_pram0 = ntohf(ptr->scan_mode_pram0);
  ptr->scan_mode_pram1 = ntohf(ptr->scan_mode_pram1);
  ptr->num_parameter_des = ntohs(ptr->num_parameter_des);
  ptr->total_num_des = ntohs(ptr->total_num_des);
  ptr->data_compress = ntohs(ptr->data_compress);
  ptr->data_reduction = ntohs(ptr->data_reduction);
  ptr->data_red_parm0 = ntohf(ptr->data_red_parm0);
  ptr->data_red_parm1 = ntohf(ptr->data_red_parm1);
  ptr->radar_longitude = ntohf(ptr->radar_longitude);
  ptr->radar_latitude = ntohf(ptr->radar_latitude);
  ptr->radar_altitude = ntohf(ptr->radar_altitude);
  ptr->eff_unamb_vel = ntohf(ptr->eff_unamb_vel);
  ptr->eff_unamb_range = ntohf(ptr->eff_unamb_range);
  ptr->num_freq_trans = ntohs(ptr->num_freq_trans);
  ptr->num_ipps_trans = ntohs(ptr->num_ipps_trans);
  ptr->freq1 = ntohf(ptr->freq1);
  ptr->freq2 = ntohf(ptr->freq2);
  ptr->freq3 = ntohf(ptr->freq3);
  ptr->freq4 = ntohf(ptr->freq4);
  ptr->freq5 = ntohf(ptr->freq5);
  ptr->interpulse_per1 = ntohf(ptr->interpulse_per1);
  ptr->interpulse_per2 = ntohf(ptr->interpulse_per2);
  ptr->interpulse_per3 = ntohf(ptr->interpulse_per3);
  ptr->interpulse_per4 = ntohf(ptr->interpulse_per4);
  ptr->interpulse_per5 = ntohf(ptr->interpulse_per5);
}

swapParameter(PARAMETER *ptr)
{
  ptr->parameter_des_length = ntohl(ptr->parameter_des_length);
  ptr->interpulse_time = ntohs(ptr->interpulse_time);
  ptr->xmitted_freq = ntohs(ptr->xmitted_freq);
  ptr->recvr_bandwidth = ntohf(ptr->recvr_bandwidth);
  ptr->pulse_width = ntohs(ptr->pulse_width);
  ptr->polarization = ntohs(ptr->polarization);
  ptr->num_samples = ntohs(ptr->num_samples);
  ptr->binary_format = ntohs(ptr->binary_format);
  ptr->threshold_value = ntohf(ptr->threshold_value);
  ptr->parameter_scale = ntohf(ptr->parameter_scale);
  ptr->parameter_bias = ntohf(ptr->parameter_bias);
  ptr->bad_data = ntohl(ptr->bad_data);
}

swapPlatform(PLATFORM *ptr)
{
  ptr->platform_info_length = ntohl(ptr->platform_info_length);
  ptr->longitude = ntohf(ptr->longitude);
  ptr->latitude = ntohf(ptr->latitude);
  ptr->altitude_msl = ntohf(ptr->altitude_msl);
  ptr->altitude_agl = ntohf(ptr->altitude_agl);
  ptr->ew_velocity = ntohf(ptr->ew_velocity);
  ptr->ns_velocity = ntohf(ptr->ns_velocity);
  ptr->vert_velocity = ntohf(ptr->vert_velocity);
  ptr->heading = ntohf(ptr->heading);
  ptr->roll = ntohf(ptr->roll);
  ptr->pitch = ntohf(ptr->pitch);
  ptr->drift_angle = ntohf(ptr->drift_angle);
  ptr->rotation_angle = ntohf(ptr->rotation_angle);
  ptr->tilt = ntohf(ptr->tilt);
  ptr->ew_horiz_wind = ntohf(ptr->ew_horiz_wind);
  ptr->ns_horiz_wind = ntohf(ptr->ns_horiz_wind);
  ptr->vert_wind = ntohf(ptr->vert_wind);
  ptr->heading_change = ntohf(ptr->heading_change);
  ptr->pitch_change = ntohf(ptr->pitch_change);
}

swapCellspacing(CELLSPACING *ptr)
{
  int i;
  ptr->cell_spacing_des_len = ntohl(ptr->cell_spacing_des_len);

  ptr->num_segments = ntohs(ptr->num_segments);
  ptr->distToFirst = ntohs(ptr->distToFirst);

  for (i = 0; i < 6; i++)
    {
      ptr->spacing[i] = ntohs(ptr->spacing[i]);
      ptr->num_cells[i] = ntohs(ptr->num_cells[i]);
    }
}

swapWaveform(WAVEFORM *ptr)
{
  int i;

  ptr->waveform_des_length = ntohl(ptr->waveform_des_length);
  ptr->repeat_seq = ntohf(ptr->repeat_seq);
  ptr->repeat_seq_dwel = ntohs(ptr->repeat_seq_dwel);
  ptr->total_pcp = ntohs(ptr->total_pcp);
  ptr->ur_pcp = ntohf(ptr->ur_pcp);
  ptr->uv_pcp = ntohf(ptr->uv_pcp);
  ptr->gate_dist1[0] = ntohs(ptr->gate_dist1[0]);
  ptr->gate_dist2[0] = ntohs(ptr->gate_dist2[0]);
  ptr->gate_dist3[0] = ntohs(ptr->gate_dist3[0]);
  ptr->gate_dist4[0] = ntohs(ptr->gate_dist4[0]);
  ptr->gate_dist5[0] = ntohs(ptr->gate_dist5[0]);
  ptr->gate_dist1[1] = ntohs(ptr->gate_dist1[1]);
  ptr->gate_dist2[1] = ntohs(ptr->gate_dist2[1]);
  ptr->gate_dist3[1] = ntohs(ptr->gate_dist3[1]);
  ptr->gate_dist4[1] = ntohs(ptr->gate_dist4[1]);
  ptr->gate_dist5[1] = ntohs(ptr->gate_dist5[1]);

  for (i = 0; i < 6; i++)
    {
      ptr->num_chips[i] = ntohs(ptr->num_chips[i]);
      ptr->chip_offset[i] = ntohs(ptr->chip_offset[i]);
      ptr->chip_width[i] = ntohs(ptr->chip_width[i]);
      ptr->num_gates[i] = ntohs(ptr->num_gates[i]);
    }
}

swapNavdesc(NAVDESC *ptr)
{
  ptr->nav_descript_len = ntohl(ptr->nav_descript_len);
  ptr->ins_flag = ntohs(ptr->ins_flag);
  ptr->gps_flag = ntohs(ptr->gps_flag);
  ptr->minirims_flag = ntohs(ptr->minirims_flag);
  ptr->kalman_flag = ntohs(ptr->kalman_flag);
}

swapInsitudesc(INSITUDESC *ptr)
{
  ptr->insitu_descript_len = ntohl(ptr->insitu_descript_len);
  ptr->number_params = ntohl(ptr->number_params);
}
