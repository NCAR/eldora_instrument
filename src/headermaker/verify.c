/***** Checks to make sure that a header is possible for the hardware ******/

#define scope extern
#include "mkrDef.h"
#include "mkrGbl.h"
#include "Ray.h"
#include "Platform.h"
#include "FieldParam.h"

void verify(int flag,
            int *num_errors)
{

    int num_beams, i, cells_top, cells_bot, bytes_per_cell, total_cells;
    int max_possible_cells, total_recorded_cells, overhead, recorded_cells;
    int num_samples;
    float len_opp_top, len_opp_bot, hypt_top, hypt_bot, duty_cycle, temp;
    float beam_spacing, current_dist, diff, sweep_time, max_possible_rate;
    float recording_rate, max_range_gates, max_range_pcp, unambig_velocity;
    double angle_top, angle_bot;
    float dwelltime, percent_of_prt, min_dwelltime, along_tk_res;
    float prf1, prf2, chip_width, num_indep_samples, pulse_width;

    /* Calculate the pulse repetition frequency from the IPPs */
    prf1 = 1000.0 / radar[0].interpulse_per1;
    if (radar[0].num_ipps_trans > 1)
        prf2 = 1000.0 / radar[0].interpulse_per2;
    else
        prf2 = 0;

    /* Calculate the chip width in micro seconds */
    chip_width = 0.01666666667 * waveform.chip_width[0];

    /* Caluculate the along track resolution in meters */
    along_tk_res = 120.0 * (360.0 / radar[0].req_rotat_vel);

    /* Calculate the total number of cells in a beam */
    total_cells = 0;
    for (i=0; i < cell[0].num_segments; i++)
        total_cells += cell[0].num_cells[i];

    /* Calculate the number of samples in a dwelltime */
    num_samples = 0;
    for (i=0; i<actual_num_params[0]; i++) {
        if (parameter[i][0].num_samples > num_samples)
            num_samples = parameter[i][0].num_samples;
    }

    /* Calculate the beam (around the circle) spacing in degrees */
    beam_spacing = waveform.repeat_seq_dwel * waveform.repeat_seq / 1000.
            * radar[0].req_rotat_vel;

    /* Calculate the number of bytes in each data cell */
    bytes_per_cell = 0;
    for (i=0; i<radar[0].num_parameter_des; i++)
        bytes_per_cell += parameter[i][0].binary_format;

    /* Is "between two altitudes" data reduction turned on? */

    if (radar[0].data_reduction == 3) {

        /* assume aircraft is flying at approximately 10000 feet (3 km) */

        len_opp_top = (radar[0].data_red_parm0 - 3.0) * 1000;
        len_opp_bot = (3.0 - radar[0].data_red_parm1) * 1000;

        /* First, go through 90 degrees of motion calculating reduction of data */

        num_beams = 0;
        angle_bot = 0;
        cells_top = 0;
        cells_bot = 0;

        do {

            num_beams += 1;

            /* calculate number of cells out to the top restriction */

            angle_top = 90.0 - angle_bot;
            if (angle_top > 0)
                hypt_top = len_opp_top / sin((double)(angle_top/57.295));
            else
                hypt_top = 1000000.;

            current_dist = cell[0].distToFirst;
            recorded_cells = 0;

            for (i=0; i<cell[0].num_segments; i++) {
                current_dist += cell[0].num_cells[i]*cell[0].spacing[i];
                if (current_dist >= hypt_top) {
                    current_dist -= cell[0].num_cells[i]*cell[0].spacing[i];
                    diff = hypt_top - current_dist;
                    recorded_cells += 1 + (diff / cell[0].spacing[i]);

                    if (recorded_cells > total_cells)
                        recorded_cells = total_cells;
                    break;
                }
                recorded_cells += cell[0].num_cells[i];
            }
            cells_top += recorded_cells;

            /* Calculate the number of cells down to the bottom restriction */

            if (angle_bot > 0)
                hypt_bot = len_opp_bot / sin((double)(angle_bot/57.295));
            else
                hypt_bot = 1000000.;

            current_dist = cell[0].distToFirst;
            recorded_cells = 0;

            for (i=0; i<cell[0].num_segments; i++) {
                current_dist += cell[0].num_cells[i]*cell[0].spacing[i];
                if (current_dist >= hypt_bot) {
                    current_dist -= cell[0].num_cells[i]*cell[0].spacing[i];
                    diff = hypt_bot - current_dist;
                    recorded_cells += 1 + (diff / cell[0].spacing[i]);

                    if (recorded_cells > total_cells)
                        recorded_cells = total_cells;
                    break;
                }
                recorded_cells += cell[0].num_cells[i];
            }
            cells_bot += recorded_cells;

            /*increment angle one ray and stop if > 90 degrees */
            angle_bot += beam_spacing;

        } while (angle_bot <= 90.0);

        total_recorded_cells = 2 * (cells_bot + cells_top);
        overhead = sizeof(struct ray_i) + sizeof(struct platform_i)
                + sizeof(struct field_parameter_data);
        if (fradar[0].indepf_times_flg > 0)
            overhead = overhead + 8 + radar[0].num_parameter_des
                    * radar[0].num_freq_trans * 2;
        if (fradar[0].indepf_times_flg > 1)
            overhead = overhead + 8 + radar[0].num_freq_trans * 4 * 2
                    * parameter[0][0].num_samples;

        sweep_time = num_beams * 4 * waveform.repeat_seq_dwel
                * waveform.repeat_seq / 1000.;

        max_possible_cells = num_beams * 4 * total_cells;

        /* calculate data rates: 2 is for fore and aft radars, 4 is due to
         num_beams being the number of beams in 90 degrees and we want 360 deg*/

        max_possible_rate = 2 * (num_beams * 4 * overhead + max_possible_cells
                * bytes_per_cell) / sweep_time;
        recording_rate = 2 * (num_beams * 4 * overhead + total_recorded_cells
                * bytes_per_cell) / sweep_time;
    }

    /* Is no data reduction turned on? */

    else if (radar[0].data_reduction == 0) {
        num_beams = 0;
        angle_bot = 0;

        do {
            num_beams =+ 1;
            angle_bot += 1;
        } while (angle_bot <= 90);

        overhead = sizeof(struct ray_i) + sizeof(struct platform_i)
                + sizeof(struct field_parameter_data);
        if (fradar[0].indepf_times_flg > 0)
            overhead = overhead + 8 + radar[0].num_parameter_des
                    * radar[0].num_freq_trans * 2;
        if (fradar[0].indepf_times_flg > 1)
            overhead = overhead + 8 + radar[0].num_freq_trans * 4 * 2
                    * parameter[0][0].num_samples;

        sweep_time = num_beams * 4 * waveform.repeat_seq_dwel
                * waveform.repeat_seq / 1000.;

        max_possible_cells = num_beams * 4 * total_cells;

        /* calculate data rates (first 2 is for fore and aft radars) */

        max_possible_rate = 2 * (num_beams * 4 * overhead + max_possible_cells
                * bytes_per_cell) / sweep_time;

        recording_rate = max_possible_rate;
    }

    /* Other types of data reduction are not supported */

    else {
        recording_rate = -999;
        max_possible_rate = -999;
    }

    /* Calculate the dwell time in milliseconds */

    dwelltime = waveform.repeat_seq_dwel * waveform.repeat_seq;

    /* Calculate the duty cycle (this is true duty cycle, not percent) */

    duty_cycle = (float)(1.6667e-3 * waveform.num_chips[5]
            * waveform.chip_width[5]) / waveform.repeat_seq;

    /* Calculate the number of independent samples in a dwelltime
     (assume 5 ms as the time to independence) */
    num_indep_samples = (1.0 + dwelltime / 5.0)
            * (float)radar[0].num_freq_trans;

    /* Calculate the maximum range, using gates and PCP (in meters) */

    max_range_gates = 0;
    if (waveform.num_chips[0] > 0) {
        temp = waveform.gate_dist1[0]*2.5 + waveform.gate_dist1[1]* 2.5
                *waveform.num_gates[0];
        if (temp > max_range_gates)
            max_range_gates = temp;
    }

    if (waveform.num_chips[1] > 0) {
        temp = waveform.gate_dist2[0]*2.5 + waveform.gate_dist2[1]* 2.5
                *waveform.num_gates[1];
        if (temp > max_range_gates)
            max_range_gates = temp;
    }

    if (waveform.num_chips[2] > 0) {
        temp = waveform.gate_dist3[0]*2.5 + waveform.gate_dist3[1]* 2.5
                *waveform.num_gates[2];
        if (temp > max_range_gates)
            max_range_gates = temp;
    }

    if (waveform.num_chips[3] > 0) {
        temp = waveform.gate_dist4[0]*2.5 + waveform.gate_dist4[1]* 2.5
                *waveform.num_gates[3];
        if (temp > max_range_gates)
            max_range_gates = temp;
    }

    if (waveform.num_chips[4] > 0) {
        temp = waveform.gate_dist5[0]*2.5 + waveform.gate_dist5[1]* 2.5
                *waveform.num_gates[4];
        if (temp > max_range_gates)
            max_range_gates = temp;
    }

    /* Calculate the maximum possible range based on the PRF (in meters) */

    max_range_pcp
            = (waveform.ur_pcp * waveform.repeat_seq / waveform.total_pcp)
                    * 150000;

    /* Calculate the unambiguous velocity */
    unambig_velocity = 3.0e8 / (waveform.uv_pcp * waveform.repeat_seq
            / waveform.total_pcp * 1.0e-3 * 4 * radar[0].freq1 * 1.0e9);

    /* Calculate the minimum dwell time the radar processors can process */
    min_dwelltime = waveform.num_gates[0] * 0.0012 * radar[0].num_freq_trans;

    /* If flag is true, give a summary of all checks we have made, otherwise
     only warn of potential hazards */

    max_possible_rate /= 1000.;
    recording_rate /= 1000.;
    max_range_gates /= 1000.;
    max_range_pcp /= 1000.;

    if (flag) {
        printf("************** GENERAL PARAMETERS **************\n");
        printf("VGENFILNAM  File Name: %s\n", inname);
        volume.proj_name[19] = 0;
        printf("VGENPROJ    Project Name: %s\n", volume.proj_name);

        printf("************** ENGINEERING PARAMETERS **************\n");
        waveform.ps_file_name[15] = 0;
        printf("VENGPULSE   Pulsing Scheme Name: %s\n", waveform.ps_file_name);
        printf("VENGPRF1    Pulse Repetition Frequency #1: %5.0f Hertz\n", prf1);
        printf("VENGPRF2    Pulse Repetition Frequency #2: %5.0f Hertz\n", prf2);
        printf("VENGGATES   Number Of Gates Sampled: %5d \n", waveform.num_gates[0]);
        printf("VENGCHIP    Chip Width: %7.3f us\n", chip_width);
        printf("VENGREDTOP  Data Reduction Top Altitude: %5.1f km\n",
               radar[0].data_red_parm0);
        printf("VENGREDBOT  Data Reduction Bottom Altitude: %5.1f km\n",
               radar[0].data_red_parm1);
        if (fradar[0].indepf_times_flg >= 1)
            printf("VENG        Independent Frequency Data Being Recorded At Gate: %4d\n",
                   fradar[0].indep_freq_gate);
        else
            printf("VENG        Independent Frequency Data Not Being Recorded\n");
        if (fradar[0].indepf_times_flg == 2)
            printf("VENG        Time Series Data Being Recorded At Gate: %4d\n",
                   fradar[0].time_series_gate);
        else
            printf("VENG        Time Series Data Not Being Recorded\n");
        printf("VENG        Data Rate Out Of The Radar Processors: %7.2f Kbytes/sec\n",
               max_possible_rate);
        printf("VENG        Data Rate Recorded To Tape: %7.2f Kbytes/sec\n",
               recording_rate);
        printf("VENG        The Duty Cycle Is: %5.2f percent\n", duty_cycle);
        printf("VENG        Rotation Rate:  %7.2f deg/sec\n", radar[0].req_rotat_vel);

        printf("************** SCIENTIFIC PARAMETERS **************\n");
        printf("VSCI        Total Number Of Data Cells: %4d\n", total_cells);
        printf("VSCI        Cell Spacing: %3d m\n", cell[0].spacing[0]);
        printf("VSCI        Distance To The First Cell: %3d m\n", cell[0].distToFirst);
        printf("VSCI        Rotational Spacing Of Data Beams:  %5.2f deg\n", beam_spacing);
        printf("VSCI        Along Track Spacing Of Data Beams:  %7.2f m\n", along_tk_res);
        printf("VSCI        Dwell Time For A Data Beam Is: %5.1f ms\n", dwelltime);
        printf("VSCI        The Number Of Samples In A Dwell Period: %4d\n", num_samples);
        printf("VSCI        The Number Of Independent Samples In A Dwell Period: %6.2f\n",
               num_indep_samples);
        printf("VSCI        The Maximum Sampled Range Is: %7.2f km\n", max_range_gates);
        printf("VSCI        The Maximum Possible Range Is: %7.2f km\n", max_range_pcp);
        printf("VSCI        The Unambiguous Velocity Range Is: +/-%7.2f m/s\n",
               unambig_velocity);

    }

    if (unambig_velocity < 12.0)
        printf("!!! WARNING low unambiguous velocity range could lead to problems\n");

    if (recording_rate > 1200.) {
        printf("!!! ERROR record data rate exceeds what is possible\n");
        *num_errors += 1;
    }

    if (dwelltime < 10.) {
        printf("!!! ERROR dwell time is less than possible\n");
        *num_errors += 1;
    }

    if (duty_cycle > 1.0) {
        printf("!!! ERROR duty cycle exceeds what is possible\n");
        *num_errors += 1;
    }

    percent_of_prt = max_range_gates / max_range_pcp;
    if (percent_of_prt > 0.98) {
        printf("!!! ERROR Number of gates exceeds what is possible,");
        printf(" or gate spacing is too large\n");
        *num_errors += 1;
    }

    pulse_width = chip_width * (float)radar[0].num_freq_trans;
    if (pulse_width > 10) {
        printf("!!! Error pulse length exceeds what is possible (max: 10 us)\n");
        *num_errors += 1;
    }

}
