/*
 *	$id$
 *
 *	Module: unpack_hd.c		 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description: Routines to unpack ELDORA Header 
 *        
 */

#include <stdio.h>
#define scope extern
#include "mkrDef.h"
#include "mkrGbl.h"
#include "mkrFunc.h"

void unpack_hd() {

    int i;
    int j;

    if (processor_type == 0)
        return;

    /*  Un-pack the Volume descriptor */

    unpk_long(&volume.volume_des_length, 1, 0);
    unpk_short(&volume.format_version, 2, 0);
    unpk_long(&volume.maximum_bytes, 1, 0);
    unpk_short(&volume.year, 6, 0);
    unpk_short(&volume.gen_year, 4, 0);

    /*  Un-pack the Waveform descriptor */

    unpk_long(&waveform.waveform_des_length, 1, 0);
    unpk_short(&waveform.num_chips[0], 6, 0);
    unpk_float(&waveform.repeat_seq, 1, 0);
    unpk_short(&waveform.repeat_seq_dwel, 14, 0);
    unpk_float(&waveform.ur_pcp, 2, 0);
    unpk_short(&waveform.num_gates[0], 16, 0);

    /* Un-pack the Fore and Aft Radar descriptors */

    for (i=0; i<2; i++) {
        unpk_long(&radar[i].radar_des_length, 1, 0);
        unpk_float(&radar[i].radar_const, 8, 0);
        unpk_short(&radar[i].radar_type, 2, 0);
        unpk_float(&radar[i].req_rotat_vel, 3, 0);
        unpk_short(&radar[i].total_num_des, 3, 0); /* note: unpacked num_parameter_des in read_header.c */
        unpk_float(&radar[i].data_red_parm0, 7, 0);
        unpk_short(&radar[i].num_freq_trans, 2, 0);
        unpk_float(&radar[i].freq1, 10, 0);

    }

    /* Un-pack the Fore and Aft Field Radar descriptors */

    for (i=0; i<2; i++) {
        unpk_long(&fradar[i].field_radar_info_len, 2, 0);
        unpk_float(&fradar[i].loss_out, 30, 0);
        unpk_long(&fradar[i].dly_tube_antenna, 5, 0);
        unpk_float(&fradar[i].peak_power_offset, 6, 0);
        unpk_short(&fradar[i].indepf_times_flg, 4, 0);
    }

    /* Un-pack the Fore and Aft Cell Spacing descriptors */

    for (i=0; i<2; i++) {
        unpk_long(&cell[i].cell_spacing_des_len, 1, 0);
        unpk_short(&cell[i].num_segments, 14, 0);
    }
    /* Un-pack the Fore and Aft Parameter descriptors */

    for (i=0; i<2; i++) {
        for (j=0; j<actual_num_params[i]; j++) {
            unpk_long(&parameter[j][i].parameter_des_length, 1, 0);
            unpk_short(&parameter[j][i].interpulse_time, 2, 0);
            unpk_float(&parameter[j][i].recvr_bandwidth, 1, 0);
            unpk_short(&parameter[j][i].pulse_width, 4, 0);
            unpk_float(&parameter[j][i].threshold_value, 3, 0);
            unpk_long(&parameter[j][i].bad_data, 1, 0);
        }
    }
    /* Un-pack the Navigational descriptor */

    unpk_long(&navigation.nav_descript_len, 1, 0);
    unpk_short(&navigation.ins_flag, 4, 0);

    /* Un-pack the In-situ descriptor */

    unpk_long(&insitudata.insitu_descript_len, 2, 0);

}

