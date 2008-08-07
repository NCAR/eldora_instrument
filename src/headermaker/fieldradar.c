#define scope extern
#include <stdlib.h>
#include "mkrDef.h"
#include "mkrGbl.h"
#include "mkrFunc.h"

/*************************************************************************/
/*              SUBROUTINE INITIALIZE FIELD RADAR INFO                   */
/*************************************************************************/

void initialize_fieldradar(fradar_pntr)
    struct field_radar_i *fradar_pntr; {
    int i, size;

    fradar_pntr->field_radar_info[0] = 'F';
    fradar_pntr->field_radar_info[1] = 'R';
    fradar_pntr->field_radar_info[2] = 'I';
    fradar_pntr->field_radar_info[3] = 'B';
    size = sizeof(struct field_radar_i);
    fradar_pntr->field_radar_info_len = size;
    fradar_pntr->data_sys_id = 1;
    fradar_pntr->loss_out = -999.;
    fradar_pntr->loss_in = -999.;
    fradar_pntr->loss_rjoint = -999.;
    fradar_pntr->ant_v_dim = -999.;
    fradar_pntr->ant_h_dim = -999.;
    fradar_pntr->ant_noise_temp = -999.;
    fradar_pntr->r_noise_figure = -999.;

    fradar_pntr->xmit_power[0] = -999.;
    fradar_pntr->xmit_power[1] = -999.;
    fradar_pntr->xmit_power[2] = -999.;
    fradar_pntr->xmit_power[3] = -999.;
    fradar_pntr->xmit_power[4] = -999.;

    fradar_pntr->x_band_gain = -999.;

    fradar_pntr->receiver_gain[0] = -999.;
    fradar_pntr->receiver_gain[1] = -999.;
    fradar_pntr->receiver_gain[2] = -999.;
    fradar_pntr->receiver_gain[3] = -999.;
    fradar_pntr->receiver_gain[4] = -999.;

    fradar_pntr->if_gain[0] = -999.;
    fradar_pntr->if_gain[1] = -999.;
    fradar_pntr->if_gain[2] = -999.;
    fradar_pntr->if_gain[3] = -999.;
    fradar_pntr->if_gain[4] = -999.;

    fradar_pntr->conversion_gain = -999.;

    fradar_pntr->scale_factor[0] = -999.;
    fradar_pntr->scale_factor[1] = -999.;
    fradar_pntr->scale_factor[2] = -999.;
    fradar_pntr->scale_factor[3] = -999.;
    fradar_pntr->scale_factor[4] = -999.;

    fradar_pntr->processor_const = -999;

    fradar_pntr->dly_tube_antenna = -999;
    fradar_pntr->dly_rndtrip_chip_atod = -999;
    fradar_pntr->dly_timmod_testpulse = -999;
    fradar_pntr->dly_modulator_on = -999;
    fradar_pntr->dly_modulator_off = -999;

    fradar_pntr->peak_power_offset = -999;
    fradar_pntr->test_pulse_offset = -999;
    fradar_pntr->E_plane_angle = -999.;
    fradar_pntr->H_plane_angle = -999.;

    fradar_pntr->encoder_antenna_up = -999.;
    fradar_pntr->pitch_antenna_up = -999.;

    fradar_pntr->indepf_times_flg = -999;
    fradar_pntr->indep_freq_gate = -999;
    fradar_pntr->time_series_gate = -999;
    for (i=0; i<80; i++)
        fradar_pntr->file_name[i] = ' ';
}

/*************************************************************************/
/*              SUBROUTINE INITIALIZE FIELD RADAR STATE                  */
/*************************************************************************/

void initialize_fieldradar_state() {

    fradar_st.loss_out = 0.0;
    fradar_st.loss_in = 0.0;
    fradar_st.loss_rjoint = 0.0;
    fradar_st.ant_v_dim = 0.0;
    fradar_st.ant_h_dim = 0.0;
    fradar_st.ant_noise_temp = 0.0;
    fradar_st.r_noise_figure = 0.0;
    fradar_st.xmit_power[0] = 0.0;
    fradar_st.x_band_gain = 0.0;
    fradar_st.receiver_gain[0] = 0.0;
    fradar_st.if_gain[0] = 0.0;
    fradar_st.conversion_gain = 0.0;
    fradar_st.scale_factor[0] = 0.0;
    fradar_st.processor_const = 0;
    fradar_st.dly_tube_antenna = 0;
    fradar_st.dly_rndtrip_chip_atod = 0;
    fradar_st.dly_timmod_testpulse = 0;
    fradar_st.dly_modulator_on = 0;
    fradar_st.dly_modulator_off = 0;
    fradar_st.peak_power_offset = 0;
    fradar_st.test_pulse_offset = 0;
    fradar_st.E_plane_angle = 0;
    fradar_st.H_plane_angle = 0;
    fradar_st.encoder_antenna_up = 0;
    fradar_st.pitch_antenna_up = 0;
    fradar_st.indepf_times_flg = 0;
    fradar_st.indep_freq_gate = 0;
    fradar_st.time_series_gate = 0;

}

void edit_fieldradar(fradar_pntr)
    struct field_radar_i *fradar_pntr;

{
    char input[80];
    int choice, i;
    int entry;
    float dim;

    for (;;) {
        printf("FIELD RADAR INFO MENU\n");
        do {
            printf(" 0.   TO RETURN TO THE MAIN MENU\n");
            printf(" 1.   INPUT DATA SYSTEM IDENTIFICATION\n");
            printf(" 3.   INPUT ANTENNA VERTICAL DIMENSION\n");
            printf(" 4.   INPUT ANTENNA HORIZONTAL DIMENSION\n");
            printf(" 5.   INPUT THE SCALE FACTORS\n");
            printf(" 6.   INPUT THE H PLANE ANGLE\n");
            printf(" 8.   INPUT THE INDEPF_TIMES_FLAG\n");
            printf(" 9.   INPUT THE INDEP_FREQ_GATE\n");
            printf(" 10.  INPUT THE TIME_SERIES_GATE\n");
            printf(" 11.  INPUT THE HEADER NAME\n");
            fgets(input, sizeof(input), stdin);
            choice = atoi(input);
            if ((choice < 0) || (choice >11))
                printf("ERROR PLEASE REENTER\n");
        } while ((choice <0) || (choice > 11));

        switch (choice) {
        case 0:
            return;
            break;
        case 1:
            printf("ENTER SYSTEM ID\n");
            fgets(input, sizeof(input), stdin);
            entry = atoi(input);
            fradar_pntr->data_sys_id = entry;
            break;
        case 3:
            printf("ENTER DIMENSION\n");
            fgets(input, sizeof(input), stdin);
            dim = atof(input);
            fradar_pntr->ant_v_dim = dim;
            break;
        case 4:
            printf("ENTER DIMENSION\n");
            fgets(input, sizeof(input), stdin);
            dim = atof(input);
            fradar_pntr->ant_h_dim = dim;
            break;
        case 5:
            for (i=0; i<5; i++) {
                printf("\nENTER SCALE FACTOR FREQ #%1d   ", i+1);
                fgets(input, sizeof(input), stdin);
                fradar_pntr->scale_factor[i] = atof(input);
            }
            break;
        case 6:
            printf("ENTER ANGLE\n");
            fgets(input, sizeof(input), stdin);
            dim = atof(input);
            fradar_pntr->H_plane_angle = dim;
            break;
        case 8:
            printf("ENTER FLAG:\n");
            fgets(input, sizeof(input), stdin);
            entry = atoi(input);
            fradar_pntr->indepf_times_flg = entry;
            break;
        case 9:
            printf("ENTER FREQUENCY GATE:\n");
            fgets(input, sizeof(input), stdin);
            entry = atoi(input);
            fradar_pntr->indep_freq_gate = entry;
            break;
        case 10:
            printf("ENTER TIME SERIES GATE:\n");
            fgets(input, sizeof(input), stdin);
            entry = atoi(input);
            fradar_pntr->time_series_gate = entry;
            break;
        case 11:
            printf("ENTER HEADER FILE NAME:\n");
            fgets(fradar_pntr->file_name, sizeof(fradar_pntr->file_name), stdin);
            break;
        default:
            break;
        }/*switch*/
    }/*for*/
}

/*****************************************************************************/
/*         SUBROUTINE PRINT FIELD RADAR INFO TO PRINTER                      */
/*****************************************************************************/

void fprint_fieldradar(fradar_pntr,
                       print_pntr)
    struct field_radar_i *fradar_pntr;FILE *print_pntr;

{
    int i;

    fprintf(print_pntr, "************** FIELD RADAR INFO BLOCK *********\n");
    fprintf(print_pntr, 
            "FRIBID     FIELD RADAR IDENTIFIER                   : ");
    for (i=0; i<4; i++)
        fprintf(print_pntr, "%c", fradar_pntr->field_radar_info[i]);
    fprintf(print_pntr,"\n");
    fprintf(print_pntr,
            "FRIBDESLEN FIELD RADAR LENGTH                       : %ld\n",
            fradar_pntr->field_radar_info_len);
    fprintf(print_pntr,
            "FRIBSYSID  DATA SYSTEM ID                           : %ld\n",
            fradar_pntr->data_sys_id);
    fprintf(print_pntr,
            "FRIBANTLOS LOSSES BTWN TX AND ANTENNA (dB)          : %f\n",
            fradar_pntr->loss_out);
    fprintf(print_pntr,
            "FRIBLNALOS LOSSES BTWN ANTENNA AND LNA (dB)         : %f\n",
            fradar_pntr->loss_in);
    fprintf(print_pntr,
            "FRIBRJLOS  LOSSES IN ROTARY JOINT (dB)              : %f\n",
            fradar_pntr->loss_rjoint);
    fprintf(print_pntr,
            "FRIBANTVER ANTENNA VERTICAL DIMENSION (m)           : %f\n",
            fradar_pntr->ant_v_dim);
    fprintf(print_pntr,
            "FRIBANTHOR ANTENNA HORIZONTAL DIMENSION (m)         : %f\n",
            fradar_pntr->ant_h_dim);
    fprintf(print_pntr,
            "FRIBANTNOI ANTENNA NOISE TEMPERATURE (K)            : %f\n",
            fradar_pntr->ant_noise_temp);
    fprintf(print_pntr,
            "FRIBRXNOI  RECEIVER NOISE FIGURE (dB)               : %f\n",
            fradar_pntr->r_noise_figure);
    fprintf(print_pntr, 
            "FRIBPEAKTX PEAK TRANSMIT POWER BY CHANNEL (dBm)     : ");
    for (i = 0; i < 5; i++)
        fprintf(print_pntr, "%f ", fradar_pntr->xmit_power[i]);
    fprintf(print_pntr, "\n");
    fprintf(print_pntr,
            "FRIBXGAIN  X BAND GAIN (dB)                         : %f\n",
            fradar_pntr->x_band_gain);
    fprintf(print_pntr, 
            "FRIBRXGAIN RECEIVER GAIN BY CHANNEL (dB)            : ");
    for (i = 0; i < 5; i++)
        fprintf(print_pntr, "%f ", fradar_pntr->receiver_gain[i]);
    fprintf(print_pntr, "\n");
    fprintf(print_pntr, 
            "FRIBIFGAIN IF GAIN BY CHANNEL (dB)                  : ");
    for (i = 0; i < 5; i++)
        fprintf(print_pntr, "%f ", fradar_pntr->if_gain[i]);
    fprintf(print_pntr, "\n");
    fprintf(print_pntr,
            "FRIBADGAIN A to D CONVERSION GAIN (dB)              : %f\n",
            fradar_pntr->conversion_gain);
    fprintf(print_pntr, 
            "FRIBSCALE  SCALE FACTOR BY CHAN (volt counts)       : ");
    for (i = 0; i < 5; i++)
        fprintf(print_pntr, "%g  ", fradar_pntr->scale_factor[i]);
    fprintf(print_pntr, "\n");
    fprintf(print_pntr,
            "FRIBPROCON PROCESSOR CONSTANT (scaled dBZ)          : %f\n",
            fradar_pntr->processor_const);
    fprintf(print_pntr,
            "FRIBTIME   DELAY TWT TUBE TO ANTENNA (ns)           : %ld\n",
            fradar_pntr->dly_tube_antenna);
    fprintf(print_pntr,
            "FRIBDLYAD  TIME DELAY(round trip) CHIP TO A TO D(ns): %ld\n",
            fradar_pntr->dly_rndtrip_chip_atod);
    fprintf(print_pntr,
            "FRIBDLYTP  TIME DELAY TIMING MOD TO TESTPULSE (ns)  : %ld\n",
            fradar_pntr->dly_timmod_testpulse);
    fprintf(print_pntr,
            "FRIBHPARIS HPA MODULATOR RISE TIME (ns)             : %ld\n",
            fradar_pntr->dly_modulator_on);
    fprintf(print_pntr,
            "FRIBHPAFAL HPA MODULATOR FALL TIME (ns)             : %ld\n",
            fradar_pntr->dly_modulator_off);
    fprintf(print_pntr,
            "FRIBPKOFF  PEAK POWER METER OFFSET (dB)             : %f\n",
            fradar_pntr->peak_power_offset);
    fprintf(print_pntr,
            "FRIBTSTOFF TEST PULSE POWER METER OFFSET (dB)       : %f\n",
            fradar_pntr->test_pulse_offset);
    fprintf(print_pntr,
            "FRIBANTEAS ANTENNA E PLANE ANGLE (deg) (AKA TILT)   : %f\n",
            fradar_pntr->E_plane_angle);
    fprintf(print_pntr,
            "FRIBHORANG ANTENNA H PLANE ANGLE (deg)              : %f\n",
            fradar_pntr->H_plane_angle);
    fprintf(print_pntr,
            "FRIBENCANG ENCODER ANGLE ANTENNA FACING UP (deg)    : %f\n",
            fradar_pntr->encoder_antenna_up);
    fprintf(print_pntr,
            "FRIBPITANG ANTENNA PITCH ANGLE FACING UP (deg)      : %f\n",
            fradar_pntr->pitch_antenna_up);
    fprintf(print_pntr,
            "FRIBFRQFLG INDEPENDENT FREQUENCY FLAG               : %d\n",
            fradar_pntr->indepf_times_flg);
    fprintf(print_pntr,
            "FRIBFRQGAT INDEPENDENT FREQUENCY GATE               : %d\n",
            fradar_pntr->indep_freq_gate);
    fprintf(print_pntr,
            "FRIBTSGATE TIME SERIES GATE                         : %d\n",
            fradar_pntr->time_series_gate);
    fprintf(print_pntr,
            "FRIBHDRNAM HEADER FILE NAME                         : %s\n",
            fradar_pntr->file_name);
}

/*************************************************************************/
/*              SUBROUTINE CALCULATE FIELD RADAR INFO                   */
/*************************************************************************/

int calculate_fieldradar() {
    int i, number_to_calculate, midgate, integer;
    float afloat;
    float afloat1;
    float numsamples;
    float max_input_power;
    float max_power_counts;
    float db;
    float proc_gain;

#define WAVEGUIDE_LARGE_DIM  0.028448  /* in meters */
#define C  2.997925e8                  /* in meters/second */
#define WAVEGUIDE_LOSS  0.1109         /* in db/meter */

    number_to_calculate = 16;

    /* Calculate the waveguide loss parameters (db)  Waveguide loss in dB per
     meter comes from the Continental Electronics Catalog, waveguide length
     was estimated from FS numbers but should eventually be measured */

    fradar[FORE].loss_out = WAVEGUIDE_LOSS * antenna[FORE].tx_waveguide_length;
    fradar[FORE].loss_in = WAVEGUIDE_LOSS * antenna[FORE].rx_waveguide_length;
    fradar[AFT].loss_out = WAVEGUIDE_LOSS * antenna[AFT].tx_waveguide_length;
    fradar[AFT].loss_in = WAVEGUIDE_LOSS * antenna[AFT].rx_waveguide_length;
    fradar_st.loss_out = 3; /* Calculated */
    fradar_st.loss_in = 3; /* Calculated */

    /* The loss in the rotary joint comes directly from the Kevlin spec sheet
     for the joint (in dB) */

    fradar[FORE].loss_rjoint = 0.2;
    fradar[AFT].loss_rjoint = 0.3;
    fradar_st.loss_rjoint = 1;

    /* Antenna horizontal and vertical dimensions (meters) taken from drawings
     supplied by LCTAR */

    fradar[FORE].ant_v_dim = 1.321; /* In meters */
    fradar[AFT].ant_v_dim = 1.321; /* In meters */
    fradar_st.ant_v_dim = 1; /* Measured constant */

    fradar[FORE].ant_h_dim = 1.483; /* in meters */
    fradar[AFT].ant_h_dim = 1.483; /* in meters */
    fradar_st.ant_h_dim = 1; /* Measured constant */

    /* The antenna noise temperature (K) depends on what portion of the sky
     is being observed by the radar and whether any sidelobes are in
     contact with the ground etc.  Since this is really only used with
     with the scientific window parameters to estimate radar sensitivity,
     for now set it to 40 the middle between really bad and really good.
     Doviak and Zrnic pg 54 to 57 discuss this in detail */

    fradar[FORE].ant_noise_temp = 40;
    fradar[AFT].ant_noise_temp = 40;
    fradar_st.ant_noise_temp = 1;

    /* The receiver noise figure (dB) is basically the noise figure of the LNA.
     The Miteq spec sheet has noise figure listed by received frequency
     and is HPA serial number specific (since the LNA is physically mounted
     in the HPA rack) */

    if (fradar_st.r_noise_figure != 0)
        number_to_calculate--;
    else if (engin_st.forehpa_snum > 0 && engin_st.afthpa_snum > 0) {
        fradar[FORE].r_noise_figure
                = hpa[engin_win.forehpa_snum].r_noise_figure;
        fradar[AFT].r_noise_figure = hpa[engin_win.afthpa_snum].r_noise_figure;
        number_to_calculate--;
        fradar_st.r_noise_figure = 4;
    }

    /* The peak transmitted power (in dBm) depends on the HPA serial number
     and on the frequencies that are being transmitted, this is stored with
     the HPA information */

    if (fradar_st.xmit_power[0] != 0)
        number_to_calculate--;
    else if (engin_st.forehpa_snum > 0 && engin_st.afthpa_snum > 0) {
        for (i=0; i<5; i++) {
            fradar[FORE].xmit_power[i] = 30.0 + 10.0
                    * log10((double)(hpa[engin_win.forehpa_snum].peak_power[
                    rcvr[FORE].freq_indexs[i]] * 1000.));
            fradar[AFT].xmit_power[i] = 30.0 + 10.0
                    * log10((double)(hpa[engin_win.afthpa_snum].peak_power[
                    rcvr[AFT].freq_indexs[i]] * 1000.));
        }
        fradar_st.xmit_power[0] = 3;
        number_to_calculate--;
    }

#ifndef NU_RX
    /* The X-band gain is given in dB it is defined to be: the gain from
     the waveguide through the TR switch and LNA and out to the precision
     N connector on the HPA connector panel.  This gain is measured by 
     injecting a signal into the waveguide and measuring the output of the
     HPA with a spectrum analyser and will be different based on the HPA
     serial number Note: this gain should not have the coupling from the
     input to the waveguide included in it */

    if (fradar_st.x_band_gain != 0)
        number_to_calculate--;
    else if (engin_st.forehpa_snum > 0 && engin_st.afthpa_snum > 0
            && engin_st.avg_freq_index[0] > 0) {
        fradar[FORE].x_band_gain
                = hpa[engin_win.forehpa_snum].x_band_gain[engin_win.avg_freq_index[FORE]];
        fradar[AFT].x_band_gain
                = hpa[engin_win.afthpa_snum].x_band_gain[engin_win.avg_freq_index[AFT]];

        fradar_st.x_band_gain = 1;
        number_to_calculate--;
    }

    /* The receiver gain in dB is a measured constant that is receiver specific and
     transmitted frequency dependent.  This is the gain from the output port
     of the HPA to the output port of the receiver exciter chassis.  Note that
     this gain includes the loss in the cable that runs from the HPA to the
     receiver exciter chassis */

    for (i=0; i<5; i++) {
        fradar[FORE].receiver_gain[i] = rcvr[FORE].receiver_gain[i];
        fradar[AFT].receiver_gain[i] = rcvr[AFT].receiver_gain[i];
    }
    fradar_st.receiver_gain[0] = 4;

    /* The IF gain in dB is a measured constant that is IF specific and
     transmitted frequency dependent.  This is the gain from the output port
     of the receiver exciter chassis to the 4 MHz signal that is digitized by
     the digital IF (or either the I or Q output of the IF signal processors on
     the current system).  Note that this gain includes the loss in the cable
     that runs from the receiver exciter chassis to the IF signal processor and
     will need to be measured on the aircraft to be accurate */

    for (i=0; i<5; i++) {
        fradar[FORE].if_gain[i] = rcvr[FORE].if_gain[i];
        fradar[AFT].if_gain[i] = rcvr[AFT].if_gain[i];
    }
    fradar_st.if_gain[0] = 4;
#endif

#ifdef NU_RX
    /* The X-band gain is given in dB it is defined to be: the gain from
     the waveguide through the TR switch and LNA, through the down and
     upconverters, into the rx/exciter at the output of the testpulse coupler.
     This gain is measured by injecting a signal into the waveguide and 
     measuring the output of the coupler with a spectrum analyser or power meter
     and will be different for FORE and AFT. 
     */

    if(fradar_st.x_band_gain != 0)
    number_to_calculate--;
    else if(engin_st.forehpa_snum > 0 && engin_st.afthpa_snum > 0
            && engin_st.avg_freq_index[0] > 0)
    {
        fradar[FORE].x_band_gain =
        hpa[engin_win.forehpa_snum].x_band_gain[engin_win.avg_freq_index[FORE]];
        fradar[AFT].x_band_gain =
        hpa[engin_win.afthpa_snum].x_band_gain[engin_win.avg_freq_index[AFT]];

        fradar_st.x_band_gain = 1;
        number_to_calculate--;
    }

    /* The receiver gain in dB is a measured constant that is receiver specific and
     transmitted frequency dependent.  This is the gain from the output of the
     of the coupler in the rx/exciter chassis to the input of the DIGIF card. In
     practice it is the difference between the gain of the entire receiver chain
     and the measured X-band gain.
     */

    for(i=0; i<5; i++)
    {
        fradar[FORE].receiver_gain[i] = rcvr[FORE].receiver_gain[i];
        fradar[AFT].receiver_gain[i] = rcvr[AFT].receiver_gain[i];
    }
    fradar_st.receiver_gain[0] = 4;

    /* The IF gain is no longer used and is defined to be 0.0 */

    for(i=0; i<5; i++)
    {
        fradar[FORE].if_gain[i] = rcvr[FORE].if_gain[i];
        fradar[AFT].if_gain[i] = rcvr[AFT].if_gain[i];
    }
    fradar_st.if_gain[0] = 4;
#endif
    /* The conversion gain in dB is defined to be the gain caused by the 
     analog to digital conversion process (i.e. it is the gain from
     dBm to processor counts) This is calculated by determining the maximum
     input power to the A/D converter in dbm and the maximum count out of
     the A/D converter-matched filter combination.  Remember there is summing
     that goes on in the digital filter on the digital IF board.
     The gain due to this summing depends on the chip width, since the matched
     filter receives data at a constant 8 MHz over the chosen chip width. */

    if (fradar_st.conversion_gain != 0)
        number_to_calculate--;
    else if (engin_st.chipdur > 0) {
#ifdef PRE_BAMEX  /* conversion gain was incorrectly computed this way 
                     prior to the BAMEX project -- it didn't effect
		     radar calibration because it was offset by receiver
		     gain (E.L. 1/16/03) */
        max_input_power = 30.0 +
        10 * log10((double)((1.024 * 1.024)/(2.0 * 50.0)));
        /* 30 => gets you to dBm, 1.024^2 => V peak squared, 2 =>
         Vpeak^2 to Vrms^2, 50 => load resistance */

        max_power_counts = 10 *
        log10(pow((double)(2048.0 * 8.0 * engin_win.chipdur),2.0));
        /* 2048 => maximum count out of 12 bit A/D converter, 8.0 *
         chip duration => number of values that were summed in the
         matched filter, squaring it gets you to power counts */
#else
        max_input_power = 30.0 + 10 * log10((double)((1.024 * 1.024)/(50.0)));
        /* 30 => gets you to dBm, 1.024^2 => V peak squared,
         50 => load resistance */

        max_power_counts = 10 * log10(pow((double)(2048.0 * 32.0 * 8.0
                * engin_win.chipdur), 2.0));
        /* 2048 => maximum count out of 12 bit A/D converter, 
         32.0 => offset in counts of high channel, 8.0 *
         chip duration => number of values that were summed in the
         matched filter, squaring it gets you to power counts */
#endif
        fradar[FORE].conversion_gain = max_power_counts - max_input_power;
        fradar[AFT].conversion_gain = max_power_counts - max_input_power;

        fradar_st.conversion_gain = 3;
        number_to_calculate--;
    }

    /* The scale factor is the number that each I and each Q is multiplied by
     before they are used in the pulse pair processor.  This factor accounts
     for the fact that the pulse pair processor never divides its sum by the
     number of samples in the dwell time and the Doppler processor never divides
     its sum by the number of frequencies in use.  It also take into account the
     radar equation and  Also this scale factor
     is used to account for any differences between the channels themselves */

    if (fradar_st.scale_factor[0] != 0)
        number_to_calculate--;
    else if (radar_st.radar_const > 0 && fradar_st.loss_in > 0
            && radar_st.num_ipps_trans > 0 && fradar_st.x_band_gain > 0
            && fradar_st.receiver_gain[0] > 0 && fradar_st.if_gain[0] > 0
            && fradar_st.conversion_gain > 0 && engin_st.numsamples > 0
            && radar_st.num_freq_trans) {
        numsamples = engin_win.numsamples/radar[FORE].num_ipps_trans;
        for (i=0; i<5; i++) {
            /* first calculate the dBs that will take us to the pulse pair
             processor (remember the radar constant was figured at the
             HPA using the actual transmitted power) */

            db = radar[FORE].radar_const - fradar[FORE].loss_in
                    + fradar[FORE].receiver_gain[i] + fradar[FORE].if_gain[i]
                    + fradar[FORE].conversion_gain + fradar[FORE].x_band_gain;

            /* Now figure the two processing gains due to summing */
            proc_gain = 10.0 * log10((double)radar[FORE].num_freq_trans) + 10.0
                    * log10((double)numsamples);

            /* now get the linear term for this whole mess (divide by 20
             since the incomming Is and Qs will be multiplied by this
             scale factor and power is I^2 + Q^2 */

            fradar[FORE].scale_factor[i] = pow((double)10.0, (double)((db
                    + proc_gain)/-20.0));

            /*	printf("dbs = %f  proc_gain = %f  scale_factor = %f\n",
             db,proc_gain,fradar[FORE].scale_factor[i]); */
            /* first calculate the dBs that will take us to the pulse pair
             processor (remember the radar constant was figured at the
             antenna using the actual transmitted power) */

            numsamples = engin_win.numsamples/radar[AFT].num_ipps_trans;

            db = radar[AFT].radar_const - fradar[AFT].loss_in
                    + fradar[AFT].receiver_gain[i] + fradar[AFT].if_gain[i]
                    + fradar[AFT].conversion_gain + fradar[AFT].x_band_gain;

            /* Now figure the two processing gains due to summing */
            proc_gain = 10.0 * log10((double)radar[AFT].num_freq_trans) + 10.0
                    * log10((double)numsamples);

            /* now get the linear term for this whole mess (divide by 20
             since the incomming Is and Qs will be multiplied by this
             scale factor and power is I^2 + Q^2 */

            fradar[AFT].scale_factor[i] = pow((double)10.0, (double)((db
                    + proc_gain)/-20.0));

        }

        fradar_st.scale_factor[0] = 3;
        number_to_calculate--;
    }

    /* The processor constant (in dBz) is the constant that the
     doppler processors use to scale the radar reflectivity factor
     so that it is never negative and spans the range of 0 to 1023.
     The actual equation the processors use is: Z(0.125dB) = 40logR + 20logP + K
     where R is the range to the gate and K is the constant calculated
     below and P is the average of all the R(0)'s from each channel.
     Currently the scale is 8 and the offset is 280 */

    fradar[FORE].processor_const = 280;
    fradar[AFT].processor_const = 280;
    fradar_st.processor_const = 1;

    /* Calculate the delay from the tube to the antennna (in the waveguide)
     this is the single trip delay (need to use twice it to set first
     gate spacing number) (in nanoseconds) */

    if (fradar_st.dly_tube_antenna != 0)
        number_to_calculate--;
    else if (engin_st.avg_freq[0] > 0) {
        afloat = engin_win.avg_freq[FORE] * 1.0e9; /* Convert to hertz */
        afloat1 = C / (2.0 * afloat * WAVEGUIDE_LARGE_DIM);
        afloat = antenna[FORE].tx_waveguide_length / ( C * sqrt((double)(1.0 - afloat1 * afloat1)));
        fradar[FORE].dly_tube_antenna = afloat * 1.e9; /* Convert to ns */

        afloat = engin_win.avg_freq[AFT] * 1.0e9; /* Convert to hertz */
        afloat1 = C / (2.0 * afloat * WAVEGUIDE_LARGE_DIM);
        afloat = antenna[AFT].tx_waveguide_length / ( C * sqrt((double)(1.0 - afloat1 * afloat1)));
        fradar[AFT].dly_tube_antenna = afloat * 1.e9; /* Convert to ns */
        fradar_st.dly_tube_antenna = 3;
        number_to_calculate--;
    }

    /* Now set the roundtrip delay from a chip in the timing module to the 
     the chip being at the A/D converter in nanoseconds */

    if (fradar_st.dly_rndtrip_chip_atod != 0)
        number_to_calculate--;
    else {
        fradar[FORE].dly_rndtrip_chip_atod = rcvr[FORE].delay_chip_rcvrs;
        fradar[AFT].dly_rndtrip_chip_atod = rcvr[AFT].delay_chip_rcvrs;
        fradar_st.dly_rndtrip_chip_atod = 3;
        number_to_calculate--;
    }

    /* Set the delay from the testpulse on in the timing module to the
     testpulse actually being seen at the  a to d converters (this 
     delay does not include any desired range delay) (in nanoseconds) */

    fradar[FORE].dly_timmod_testpulse = rcvr[FORE].testpulse_delay;
    fradar[AFT].dly_timmod_testpulse = rcvr[AFT].testpulse_delay;
    fradar_st.dly_timmod_testpulse = 1;

    /* Set the delay between preknock entering the HPA and the
     HPA being on and ready to transmit (in nanoseconds) this is dependent
     on the HPA serial number */

    if (fradar_st.dly_modulator_on != 0)
        number_to_calculate--;
    else if (engin_st.forehpa_snum > 0) {
        fradar[FORE].dly_modulator_on
                = hpa[engin_win.forehpa_snum].turn_on_time;
        fradar[FORE].dly_modulator_off
                =hpa[engin_win.forehpa_snum].turn_off_time;
        fradar[AFT].dly_modulator_on = hpa[engin_win.afthpa_snum].turn_on_time;
        fradar[AFT].dly_modulator_off
                = hpa[engin_win.afthpa_snum].turn_off_time;
        fradar_st.dly_modulator_on = 4;
        fradar_st.dly_modulator_off = 4;
        number_to_calculate--;
    }

    /* The peak power offset (dB) is the value that, when added to the peak power
     meter reading yields the actual peak transmitted power, it is likely that
     this number will be programmed into the peak power meter so that its front
     panel display reads out in reconizable units */

    if (fradar_st.peak_power_offset != 0)
        number_to_calculate--;
    else if (engin_st.schemenum > 0) {
        fradar[FORE].peak_power_offset
                = hpa[engin_win.forehpa_snum].peak_power_coupling;

        fradar[AFT].peak_power_offset
                = hpa[engin_win.afthpa_snum].peak_power_coupling;

        fradar_st.peak_power_offset = 1;
        number_to_calculate--;
    }

    /* The test pulse offset (dB) is the value that, when added to the test pulse
     power meter reading yields the actual value of the injected testpulse, if
     the attenuator setting were 0. */

    if (fradar_st.test_pulse_offset != 0)
        number_to_calculate--;
    else if (engin_st.schemenum > 0) {
#ifndef NU_RX
        fradar[FORE].test_pulse_offset = atten[FORE].loss_rawtp_powermeter
                - atten[FORE].loss_rawtp_hpa
                - hpa[engin_win.forehpa_snum].test_pulse_coupling
                - hpa[engin_win.forehpa_snum].tr_tube_loss;

        fradar[AFT].test_pulse_offset = atten[AFT].loss_rawtp_powermeter
                - atten[AFT].loss_rawtp_hpa
                - hpa[engin_win.afthpa_snum].test_pulse_coupling
                - hpa[engin_win.afthpa_snum].tr_tube_loss;
#endif
#ifdef NU_RX
        fradar[FORE].test_pulse_offset =
        -hpa[engin_win.forehpa_snum].test_pulse_coupling;

        fradar[AFT].test_pulse_offset =
        -hpa[engin_win.afthpa_snum].test_pulse_coupling;
#endif

        fradar_st.test_pulse_offset = 3;
        number_to_calculate--;
    }

    /* The E_plane angles (TILT angles) are antenna dependent, and are frequency 
     dependent (in degrees) */

    if (fradar_st.E_plane_angle != 0)
        number_to_calculate--;
    else if (engin_st.avg_freq_index[0] > 0) {
        fradar[FORE].E_plane_angle
                = antenna[FORE].E_plane_angle[engin_win.avg_freq_index[FORE]];

        fradar[AFT].E_plane_angle
                = antenna[AFT].E_plane_angle[engin_win.avg_freq_index[AFT]];

        fradar_st.E_plane_angle = 3;
        number_to_calculate--;
    }

    /* The H_plane angles are antenna dependent, and are frequency 
     dependent (in degrees) */

    if (fradar_st.H_plane_angle != 0)
        number_to_calculate--;
    else if (engin_st.avg_freq_index[0] > 0) {
        fradar[FORE].H_plane_angle
                = antenna[FORE].H_plane_angle[engin_win.avg_freq_index[FORE]];

        fradar[AFT].H_plane_angle
                = antenna[AFT].H_plane_angle[engin_win.avg_freq_index[AFT]];

        fradar_st.H_plane_angle = 3;
        number_to_calculate--;
    }

    /* The encoder reading when the antenna is pointed up is antenna
     dependent (in degrees) */

    if (fradar_st.encoder_antenna_up != 0)
        number_to_calculate--;
    else {
        fradar[FORE].encoder_antenna_up = antenna[FORE].encoder_antenna_up;
        fradar[AFT].encoder_antenna_up = antenna[AFT].encoder_antenna_up;

        fradar_st.encoder_antenna_up = 3;
        number_to_calculate--;
    }
    /* The measured pitch angle when the antenna is pointed up is antenna
     dependent (in degrees) */

    if (fradar_st.pitch_antenna_up != 0)
        number_to_calculate--;
    else {
        fradar[FORE].pitch_antenna_up = antenna[FORE].pitch_antenna_up;
        fradar[AFT].pitch_antenna_up = antenna[AFT].pitch_antenna_up;

        fradar_st.pitch_antenna_up = 3;
        number_to_calculate--;
    }

    /* The independent frequency gate number should be a gate that impacts the
     earth each revolution, since the aircraft's maximum altitude is about
     26000 feet (8000 meters) it would be best to select a gate that is at
     a range of approximately 8 km. Since the first Doppler processor will
     put these data out, this number must be divisible by 4 (i.e. 0,4,8,12...)
     but not past the middle gate of the beam! */

    if (fradar_st.indep_freq_gate != 0)
        number_to_calculate--;
    else if (engin_st.numgates > 0 && engin_st.chipdur > 0) {
        midgate = engin_win.numgates/2 - 4; /* Last gate first Doppler
         processor will see */
        /* Calculate gate we would like to use, but make sure it appears
         in the first Doppler processor */

        integer = 8000.0 / ((float)engin_win.chipdur * 150.0);
        for (i=0; i<4; i++)
            if ((integer+i) % 4 == 0)
                break;
        integer += i;

        /* now make sure it is not too far out in range */
        if (integer > midgate)
            integer = midgate;

        fradar[FORE].indep_freq_gate = integer;
        fradar[AFT].indep_freq_gate = integer;

        fradar_st.indep_freq_gate = 3;
        number_to_calculate--;

        /* Turn on independent frequency data and time series data */
        fradar[FORE].indepf_times_flg = 3;
        fradar[AFT].indepf_times_flg = 3;
    }

    /* The time series gate number should be any gate that is processed by the
     first pulse pair processor therefore, this number must be divisible by
     4 (i.e. 0,4,8,12...) but not past total gates - 4 ! put it 10 km from
     the radar */

    if (fradar_st.time_series_gate != 0)
        number_to_calculate--;
    else if (engin_st.numgates > 0 && engin_st.chipdur > 0) {
        /* Calculate gate we would like to use, but make sure it appears
         in the first Doppler processor ( gate should be at 10 km) */

        integer = 10000.0 / ((float)engin_win.chipdur * 150.0);
        for (i=0; i<4; i++)
            if ((integer+i) % 4 == 0)
                break;
        integer += i;

        /* now make sure it is not too far out in range */
        if (integer > engin_win.numgates)
            integer = engin_win.numgates - 4;

        fradar[FORE].time_series_gate = integer;
        fradar[AFT].time_series_gate = integer;

        fradar_st.time_series_gate = 3;
        number_to_calculate--;
    }

    return (number_to_calculate);
}

