#define scope extern
#define RX_MIS 1.8         /* Receiver Mismatch Loss in dB */
#include <stdlib.h>
#include "mkrDef.h"
#include "mkrGbl.h"
#include "mkrFunc.h"

/**************************************************************************/
/*          SUBROUTINE INITIALIZE RADAR DESCRIPTOR                   */
/*************************************************************************/

void initialize_radar(radar_pntr,
                      radarnum)
    struct radar_d *radar_pntr;int radarnum; {
    char input[80];

    radar_pntr->radar_des[0] = 'R';
    radar_pntr->radar_des[1] = 'A';
    radar_pntr->radar_des[2] = 'D';
    radar_pntr->radar_des[3] = 'D';

    radar_pntr->radar_des_length = sizeof(struct radar_d);

    if (radarnum == FORE) {
        radar_pntr->radar_name[0] = 'F';
        radar_pntr->radar_name[1] = 'O';
        radar_pntr->radar_name[2] = 'R';
        radar_pntr->radar_name[3] = 'E';
        radar_pntr->radar_name[4] = ' ';
        radar_pntr->radar_name[5] = ' ';
        radar_pntr->radar_name[6] = ' ';
        radar_pntr->radar_name[7] = ' ';
    } else if (radarnum == AFT) {
        radar_pntr->radar_name[0] = 'A';
        radar_pntr->radar_name[1] = 'F';
        radar_pntr->radar_name[2] = 'T';
        radar_pntr->radar_name[3] = ' ';
        radar_pntr->radar_name[4] = ' ';
        radar_pntr->radar_name[5] = ' ';
        radar_pntr->radar_name[6] = ' ';
        radar_pntr->radar_name[7] = ' ';
    } else {
        radar_pntr->radar_name[0] = ' ';
        radar_pntr->radar_name[1] = ' ';
        radar_pntr->radar_name[2] = ' ';
        radar_pntr->radar_name[3] = ' ';
        radar_pntr->radar_name[4] = ' ';
        radar_pntr->radar_name[5] = ' ';
        radar_pntr->radar_name[6] = ' ';
        radar_pntr->radar_name[7] = ' ';
    }

    radar_pntr->radar_const = -999.;
    radar_pntr->peak_power = -999.;
    radar_pntr->noise_power = -999.;
    radar_pntr->receiver_gain = -999.;
    radar_pntr->antenna_gain = -999.;
    radar_pntr->system_gain = -999.;
    radar_pntr->horz_beam_width = -999.;
    radar_pntr->vert_beam_width = -999.;

    if (radarnum == FORE)
        radar_pntr->radar_type = 1;
    else if (radarnum == AFT)
        radar_pntr->radar_type = 2;
    else
        radar_pntr->radar_type = -999;

    radar_pntr->scan_mode = 8;
    radar_pntr->req_rotat_vel = -999.;
    radar_pntr->scan_mode_pram0 = -999.;
    radar_pntr->scan_mode_pram1 = -999.;
    radar_pntr->num_parameter_des = -999;
    radar_pntr->total_num_des = -999;
    radar_pntr->data_compress = 0;
    radar_pntr->data_reduction = 3;
    radar_pntr->data_red_parm0 = 25;
    radar_pntr->data_red_parm1 = -5;
    printf("\nEnter the longitude of the project airport (degrees +E): ");
    fgets(input, sizeof(input), stdin);
    radar_pntr->radar_longitude = atof(input);
    printf("\nEnter the latitude of the project airport (degrees +N): ");
    fgets(input, sizeof(input), stdin);
    radar_pntr->radar_latitude = atof(input);
    printf("\nEnter the altitude of the project airport (km): ");
    fgets(input, sizeof(input), stdin);
    radar_pntr->radar_altitude = atof(input);

    radar_pntr->eff_unamb_vel = -999.;
    radar_pntr->eff_unamb_range = -999.;
    radar_pntr->num_freq_trans = -999;
    radar_pntr->num_ipps_trans = -999;
    radar_pntr->freq1 = -999.;
    radar_pntr->freq2 = -999.;
    radar_pntr->freq3 = -999.;
    radar_pntr->freq4 = -999.;
    radar_pntr->freq5 = -999.;
    radar_pntr->interpulse_per1 = -999.;
    radar_pntr->interpulse_per2 = -999.;
    radar_pntr->interpulse_per3 = -999.;
    radar_pntr->interpulse_per4 = -999.;
    radar_pntr->interpulse_per5 = -999.;

}/*end initialize_radar_descriptor*/

/************************************************************************/
/*          SUBROUTINE INITIALIZE RADAR STATE                           */
/*************************************************************************/

void initialize_radar_state() {
    radar_st.radar_const = 0.0;
    radar_st.peak_power = 0.0;
    radar_st.noise_power = 0.0;
    radar_st.receiver_gain = 0.0;
    radar_st.antenna_gain = 0.0;
    radar_st.system_gain = 0.0;
    radar_st.horz_beam_width = 0.0;
    radar_st.vert_beam_width = 0.0;
    radar_st.req_rotat_vel = 0.0;
    radar_st.num_parameter_des = 0;
    radar_st.total_num_des = 0;
    radar_st.eff_unamb_vel = 0.0;
    radar_st.eff_unamb_range = 0.0;
    radar_st.num_freq_trans = 0;
    radar_st.num_ipps_trans = 0;
    radar_st.freq1 = 0.0;
    radar_st.interpulse_per1 = 0.0;

}/*end initialize_radar_state*/

/****************************************************************************/
/*                     EDIT  RADAR DESCRIPTOR                            */
/****************************************************************************/
void edit_radar(radar_pntr)
    struct radar_d *radar_pntr;

{
    int choice;
    float radarconst, peakpower, noisepower, receivergain, antennagain;
    float systemgain, horz, vert, rotatevel, scanpram0, scanpram1;
    short radartype, scanmode, numparamdes, totalnumdes, datareduction;
    short compression;
    float vel, range;
    short freq_trans, ipps_trans;
    float freq1, freq2, freq3, freq4, freq5;
    float per1, per2, per3, per4, per5;
    float parm0, parm1;
    float longit, lat, alt;
    char string[80], input[80];

    for (;;) {
        do {
            printf("********EDIT RADAR DESCRIPTOR MENU***********************\n");
            printf("0.  RETURN TO PREVIOUS MENU\n");
            printf("1.  ENTER RADAR NAME (8 ASCII CHARS\n");
            printf("2.  ENTER RADAR CONSTANT\n");
            printf("3.  ENTER PEAK_POWER\n");
            printf("4.  ENTER NOISE_POWER\n");
            printf("5.  ENTER RECEIVER GAIN\n");
            printf("6.  ENTER ANTENNA_GAIN\n");
            printf("7.  ENTER SYSTEM GAIN\n");
            printf("8.  ENTER HORIZONTAL BEAM WIDTH\n");
            printf("9.  ENTER VERTICAL BEAM WIDTH\n");
            printf("10. ENTER RADAR TYPE\n");
            printf("11. ENTER SCAN MODE\n");
            printf("12. ENTER REQUESTED ROTATIONAL VELOCITY OF THE ANTENNA IN DEG/S\n");
            printf("13. ENTER SCAN MODE SPECIFIC PARAMETER #0\n");
            printf("14. ENTER SCAN MODE SPECIFIC PARAMETER #1\n");
            printf("15. ENTER TOTAL NUMBER OF PARAMETER DESCRIPTOR BLOCKS FOR THIS RADAR\n");
            printf("16. ENTER TOTAL NUMBER OF ADDITIONAL DESCRIPTOR BLOCKS FOR THIS RADAR\n");
            printf("17. ENTER THE DATA COMPRESSION SCHEME: 0 FOR NONE 1 FOR HRD\n");
            printf("18. ENTER THE DATA REDUCTION ALGORITHM IN USE\n");
            printf("19. ENTER THE DATA REDUCTION ALGORITHM SPECIFIC PARAMETER #0\n");
            printf("20. ENTER THE DATA REDUCTION ALGORITHM SPECIFIC PARAMETER #1\n");
            printf("21. ENTER THE RADAR LONGITUDE IN DEGREES\n");
            printf("22. ENTER THE RADAR LATITUDE IN DEGREES\n");
            printf("23. ENTER THE MSL RADAR ALTITUDE IN DEGREES\n");
            printf("24. ENTER THE EFFECTIVE UNAMBIGUOUS VELOCITY IN METERS/SEC\n");
            printf("25. ENTER THE EFFECTIVE UNAMBIGUOUS RANGE IN KM\n");
            printf("26. ENTER THE NUMBER OF FREQUENCIES TRANSMITTED\n");
            printf("27. ENTER THE NUMBER OF DIFFERENT INTER-PULSE PERIODS TRANSMITTED\n");
            printf("28. ENTER FREQUENCY 1\n");
            printf("29. ENTER FREQUENCY 2\n");
            printf("30. ENTER FREQUENCY 3\n");
            printf("31. ENTER FREQUENCY 4\n");
            printf("32. ENTER FREQUENCY 5\n");
            printf("33. ENTER INTERPULSE PERIOD 1\n");
            printf("34. ENTER INTERPULSE PERIOD 2\n");
            printf("35. ENTER INTERPULSE PERIOD 3\n");
            printf("36. ENTER INTERPULSE PERIOD 4\n");
            printf("37. ENTER INTERPULSE PERIOD 5\n");
            printf("ENTER YOUR CHOICE\n");
            fgets(input, sizeof(input), stdin);
            choice = atoi(input);
            if (choice < 0 || choice >37)
                printf("INVALID ENTRY\n");
        } while (choice < 0 || choice >37);

        switch (choice) {
        case 0:
            return;
            break;
        case 1:
            do {
                printf("ENTER NAME\n");
                fgets(string, sizeof(string), stdin);
                if (strlen(string) > 8)
                    printf("NAME TOO LONG\n");
            } while (strlen(string) > 8);
            strcpy(radar_pntr->radar_name, string);
            break;
        case 2:
            printf("ENTER CONSTANT\n");
            fgets(input, sizeof(input), stdin);
            radarconst = atof(input);
            radar_pntr->radar_const = radarconst/1.;
            break;
        case 3:
            printf("ENTER PEAK POWER\n");
            fgets(input, sizeof(input), stdin);
            peakpower = atof(input);
            radar_pntr->peak_power = peakpower;
            break;
        case 4:
            printf("ENTER NOISE POWER\n");
            fgets(input, sizeof(input), stdin);
            noisepower = atof(input);
            radar_pntr->noise_power = noisepower;
            break;
        case 5:
            printf("ENTER RECEIVER GAIN\n");
            fgets(input, sizeof(input), stdin);
            receivergain = atof(input);
            radar_pntr->receiver_gain = receivergain;
            break;
        case 6:
            printf("ENTER ANTENNA GAIN\n");
            fgets(input, sizeof(input), stdin);
            antennagain = atof(input);
            radar_pntr->antenna_gain = antennagain;
            break;
        case 7:
            printf("ENTER SYSTEM GAIN\n");
            fgets(input, sizeof(input), stdin);
            systemgain = atof(input);
            radar_pntr->system_gain = systemgain;
            break;
        case 8:
            printf("ENTER BEAM WIDTH\n");
            fgets(input, sizeof(input), stdin);
            horz = atof(input);
            radar_pntr->horz_beam_width = horz;
            break;
        case 9:
            printf("ENTER BEAM WIDTH\n");
            fgets(input, sizeof(input), stdin);
            vert = atof(input);
            radar_pntr->vert_beam_width = vert;
            break;
        case 10:
            printf("ENTER RADAR TYPE\n");
            fgets(input, sizeof(input), stdin);
            radartype = atoi(input);
            radar_pntr->radar_type = radartype;
            break;
        case 11:
            printf("ENTER SCAN MODE\n");
            fgets(input, sizeof(input), stdin);
            scanmode = atoi(input);
            radar_pntr->scan_mode = scanmode;
            break;
        case 12:
            printf("ENTER VEL\n");
            fgets(input, sizeof(input), stdin);
            rotatevel = atof(input);
            radar_pntr->req_rotat_vel = rotatevel;
            break;
        case 13:
            printf("ENTER PRAM0\n");
            fgets(input, sizeof(input), stdin);
            scanpram0 = atof(input);
            radar_pntr->scan_mode_pram0 = scanpram0;
            break;
        case 14:
            printf("ENTER PRAM1\n");
            fgets(input, sizeof(input), stdin);
            scanpram1 = atof(input);
            radar_pntr->scan_mode_pram1 = scanpram1;
            break;
        case 15:
            printf("ENTER NUMBER\n");
            fgets(input, sizeof(input), stdin);
            numparamdes = atoi(input);
            radar_pntr->num_parameter_des = numparamdes;
            break;
        case 16:
            printf("ENTER NUMBER\n");
            fgets(input, sizeof(input), stdin);
            totalnumdes = atoi(input);
            radar_pntr->total_num_des = totalnumdes;
            break;
        case 17:
            printf("ENTER THE DATA COMPRESSION SCHEME\n");
            fgets(input, sizeof(input), stdin);
            compression = atoi(input);
            radar_pntr->data_compress = compression;
            break;
        case 18:
            printf("ENTER REDUCTION\n");
            fgets(input, sizeof(input), stdin);
            datareduction = atoi(input);
            radar_pntr->data_reduction = datareduction;
            break;
        case 19:
            printf("ENTER PARAMETER\n");
            fgets(input, sizeof(input), stdin);
            parm0 = atof(input);
            radar_pntr->data_red_parm0 = parm0;
            break;
        case 20:
            printf("ENTER PARAMETER\n");
            fgets(input, sizeof(input), stdin);
            parm1 = atof(input);
            radar_pntr->data_red_parm1 = parm1;
            break;
        case 21:
            printf("ENTER LONGITUDE\n");
            fgets(input, sizeof(input), stdin);
            longit = atof(input);
            radar_pntr->radar_longitude = longit;
            break;
        case 22:
            printf("ENTER LATITUDE\n");
            fgets(input, sizeof(input), stdin);
            lat = atof(input);
            radar_pntr->radar_latitude = lat;
            break;
        case 23:
            printf("ENTER ALTITUDE\n");
            fgets(input, sizeof(input), stdin);
            alt = atof(input);
            radar_pntr->radar_altitude = alt;
            break;
        case 24:
            printf("ENTER THE EFFECTIVE UNAMBIGIOUS VELOCITY IN M/SEC\n");
            fgets(input, sizeof(input), stdin);
            vel = atof(input);
            radar_pntr->eff_unamb_vel = vel;
            break;
        case 25:
            printf("ENTER THE EFFECTIVE UNAMBIGIOUS RANGE IN KM \n");
            fgets(input, sizeof(input), stdin);
            range = atof(input);
            radar_pntr->eff_unamb_range = range;
            break;
        case 26:
            printf("ENTER THE NUMBER OF TRANSMITED FREQUENCIES\n");
            fgets(input, sizeof(input), stdin);
            freq_trans = atoi(input);
            radar_pntr->num_freq_trans = freq_trans;
            break;
        case 27:
            printf("ENTER THE NUMBER OF DIFFERENT INTER_PULSE PERIODS TRANSMITTED\n");
            fgets(input, sizeof(input), stdin);
            ipps_trans = atoi(input);
            radar_pntr->num_ipps_trans = ipps_trans;
            break;
        case 28:
            printf("ENTER FREQUENCY 1 \n");
            fgets(input, sizeof(input), stdin);
            freq1 = atof(input);
            radar_pntr->freq1 = freq1;
            break;
        case 29:
            printf("ENTER FREQUENCY 2 \n");
            fgets(input, sizeof(input), stdin);
            freq2 = atof(input);
            radar_pntr->freq2 = freq2;
            break;
        case 30:
            printf("ENTER FREQUENCY 3 \n");
            fgets(input, sizeof(input), stdin);
            freq3 = atof(input);
            radar_pntr->freq3 = freq3;
            break;
        case 31:
            printf("ENTER FREQUENCY 4 \n");
            fgets(input, sizeof(input), stdin);
            freq4 = atof(input);
            radar_pntr->freq4 = freq4;
            break;
        case 32:
            printf("ENTER FREQUENCY 5 \n");
            fgets(input, sizeof(input), stdin);
            freq5 = atof(input);
            radar_pntr->freq5 = freq5;
            break;
        case 33:
            printf("ENTER INTERPULSE PERIOD 1\n");
            fgets(input, sizeof(input), stdin);
            per1 = atof(input);
            radar_pntr->interpulse_per1 = per1;
            break;
        case 34:
            printf("ENTER INTERPULSE PERIOD 2\n");
            fgets(input, sizeof(input), stdin);
            per2 = atof(input);
            radar_pntr->interpulse_per2 = per2;
            break;
        case 35:
            printf("ENTER INTERPULSE PERIOD 3\n");
            fgets(input, sizeof(input), stdin);
            per3 = atof(input);
            radar_pntr->interpulse_per3 = per3;
            break;
        case 36:
            printf("ENTER INTERPULSE PERIOD 4\n");
            fgets(input, sizeof(input), stdin);
            per4 = atof(input);
            radar_pntr->interpulse_per4 = per4;
            break;
        case 37:
            printf("ENTER INTERPULSE PERIOD 5\n");
            fgets(input, sizeof(input), stdin);
            per5 = atof(input);
            radar_pntr->interpulse_per5 = per5;
            break;

        }/*switch*/
    }/*for*/
}/*end edit_radar_descriptor*/

/**************************************************************************/
/*               SUBROUTINE PRINT RADAR DESCRIPTOR                        */
/**************************************************************************/

void fprint_radar(radar_pntr,
                  print_pntr)
    struct radar_d *radar_pntr;FILE *print_pntr; {
    int i;

    fprintf(print_pntr, "******************* RADAR DESCRIPTOR ***********\n");
    fprintf(print_pntr, 
            "RADDDES    RADAR DESCRIPTOR                        : ");
    for (i=0; i<4; i++)
        fprintf(print_pntr, "%c", radar_pntr->radar_des[i]);
    fprintf(print_pntr,"\n");
    fprintf(print_pntr, 
            "RADDDESLEN RADAR DESCRIPTOR SIZE                   : %ld\n",
            radar_pntr->radar_des_length);
    fprintf(print_pntr, 
            "RADDNAME   RADAR NAME                              : ");
    for (i = 0; i < 8; i++)
        fprintf(print_pntr, "%c", radar_pntr->radar_name[i]);
    fprintf(print_pntr,"\n");
    fprintf(print_pntr,
            "RADDCONST  RADAR CONSTANT (db)                     : %f\n",
            radar_pntr->radar_const);
    fprintf(print_pntr, 
            "RADDPKPOW  PEAK POWER (kw)                         : %f\n", radar_pntr->peak_power);
    fprintf(print_pntr,
            "RADDNOIPOW NOISE POWER (dBm)                       : %f\n",
            radar_pntr->noise_power);
    fprintf(print_pntr,
            "RADDRXGAIN RECIEVER GAIN (dB)                      : %f\n",
            radar_pntr->receiver_gain);
    fprintf(print_pntr,
            "RADDANTGAI ANTENNA GAIN (dB)                       : %f\n",
            radar_pntr->antenna_gain);
    fprintf(print_pntr,
            "RADDSYSGAI SYSTEM GAIN (dB)                        : %f\n",
            radar_pntr->system_gain);
    fprintf(print_pntr,
            "RADDHORWID HORIZONTAL BEAM WIDTH (deg)             : %f\n",
            radar_pntr->horz_beam_width);
    fprintf(print_pntr,
            "RADDVERWID VERTICAL BEAM WIDTH (deg)               : %f\n",
            radar_pntr->vert_beam_width);
    fprintf(print_pntr, 
            "RADDTYPE   RADAR TYPE                              : %d\n", radar_pntr->radar_type);
    fprintf(print_pntr, 
            "RADDSCAN   SCAN MODE                               : %d\n", radar_pntr->scan_mode);
    fprintf(print_pntr,
            "RADDROTVEL ROTATIONAL VELOCITY (deg/s)             : %f\n",
            radar_pntr->req_rotat_vel);
    fprintf(print_pntr,
            "RADDSCAN1  SCAN MODE PARAMETER #0                  : %f\n",
            radar_pntr->scan_mode_pram0);
    fprintf(print_pntr,
            "RADDSCAN2  SCAN MODE PARAMETER #1                  : %f\n",
            radar_pntr->scan_mode_pram1);
    fprintf(print_pntr,
            "RADDNPARM  NUMBER OF PARAMETER DESCRIPTOR BLOCKS   : %d\n",
            radar_pntr->num_parameter_des);
    fprintf(print_pntr,
            "RADDNDESC  TOTAL NUMBER OF DESCRIPTOR BLOCKS       : %d\n",
            radar_pntr->total_num_des);
    fprintf(print_pntr,
            "RADDCOMP   DATA COMPRESSION SCHEME                 : %d\n",
            radar_pntr->data_compress); 
    fprintf(print_pntr,
            "RADDDATRED DATA REDUCTION ALGORITHM IN USE         : %d\n",
            radar_pntr->data_reduction);
    fprintf(print_pntr,
            "RADDRED0   REDUCTION ALGORITHM SPECIFIC PARAMETER 0: %f\n",
            radar_pntr->data_red_parm0);
    fprintf(print_pntr,
            "RADDRED1   REDUCTION ALGORITHM SPECIFIC PARAMETER 1: %f\n",
            radar_pntr->data_red_parm1);
    fprintf(print_pntr, 
            "RADDLAT    LATITUDE (deg)                          : %f\n", radar_pntr->radar_latitude);
    fprintf(print_pntr, 
            "RADDLON    LONGITUDE (deg)                         : %f\n", radar_pntr->radar_longitude);
    fprintf(print_pntr, 
            "RADDALT    ALTITUDE (m)                            : %f\n", radar_pntr->radar_altitude);
    fprintf(print_pntr,
            "RADDUNVEL  EFFECTIVE UNAMBIGIOUS VELOCITY (m/s)    : %f\n",
            radar_pntr->eff_unamb_vel);
    fprintf(print_pntr,
            "RADDUNRNG  EFFECTIVE UNAMBIGIOUS RANGE (km)        : %f\n",
            radar_pntr->eff_unamb_range);
    fprintf(print_pntr,
            "RADDNFREQ  NUMBER OF TX FREQS                      : %d\n",
            radar_pntr->num_freq_trans);
    fprintf(print_pntr,
            "RADDNIPP   NUM OF DIFF IPPS                        : %d\n",
            radar_pntr->num_ipps_trans);
    fprintf(print_pntr, 
            "RADDFREQ1  FREQUENCY 1 (GHz)                       : %f\n", radar_pntr->freq1);
    fprintf(print_pntr,
            "RADDFREQ2  FREQUENCY 2 (GHz)                       : %f\n", radar_pntr->freq2);
    fprintf(print_pntr, 
            "RADDFREQ3  FREQUENCY 3 (GHz)                       : %f\n", radar_pntr->freq3);
    fprintf(print_pntr, 
            "RADDFREQ4  FREQUENCY 4 (GHz)                       : %f\n", radar_pntr->freq4);
    fprintf(print_pntr, 
            "RADDFREQ5  FREQUENCY 5 (GHz)                       : %f\n", radar_pntr->freq5);
    fprintf(print_pntr,
            "RADDIPP1   INTERPULSE PERIOD 1 (ms)                : %f\n",
            radar_pntr->interpulse_per1);
    fprintf(print_pntr,
            "RADDIPP2   INTERPULSE PERIOD 2 (ms)                : %f\n",
            radar_pntr->interpulse_per2);
    fprintf(print_pntr,
            "RADDIPP3   INTERPULSE PERIOD 3 (ms)                : %f\n",
            radar_pntr->interpulse_per3);
    fprintf(print_pntr,
            "RADDIPP4   INTERPULSE PERIOD 4 (ms)                : %f\n",
            radar_pntr->interpulse_per4);
    fprintf(print_pntr,
            "RADDIPP5   INTERPULSE PERIOD 5 (ms)                : %f\n",
            radar_pntr->interpulse_per5);
}/*end print radar descriptor */

/**************************************************************************/
/*          SUBROUTINE CALCULATE RADAR DESCRIPTOR                         */
/*************************************************************************/

int calculate_radar() {
#define C  2.997925e8           /* Speed of light in m/s */
#define PI 3.14159
#define BOLTZMAN        1.380658e-23   /* Boltzman's constant */

    float receiver_noise_temp, waveguide_loss, bandwidth, system_noise_temp;
    float afloat, numerator, denominator, freq_squared;
    int i, number_to_calculate, freq_mult[5];
    float logk, twoagain, pwrdbm, freqs[2][5], rx_mismatch;
    double delta_freq, tmp_dbl;

    number_to_calculate = 15;
    rx_mismatch = RX_MIS; /* 1.8 dB for matched filter */

    /* This is the world famous radar constant (in dB) no one really knows what
     this should be, but calculate it here, with respect to the HPA
     take into account the losses in the waveguide and rotary joint between
     the transmitter and the antenna -- Receiver mismatch added and |Kw|^2 
     correction made 7/11/05, E.L.
     */

    if (radar_st.radar_const != 0)
        number_to_calculate--;
    else if (radar_st.horz_beam_width > 0 && radar_st.vert_beam_width > 0
            && engin_st.chipdur > 0 && radar_st.antenna_gain > 0
            && radar_st.peak_power > 0 && fradar_st.loss_out > 0
            && fradar_st.loss_rjoint > 0 && engin_st.avg_freq[0] > 0
            && fradar_st.loss_in > 0) {
        freq_squared = engin_win.avg_freq[FORE] * engin_win.avg_freq[FORE]
                * 1.0e18;
        numerator = pow((double)PI, (double)5.0) * radar[FORE].horz_beam_width
                * radar[FORE].vert_beam_width * engin_win.chipdur * 1.0e-6
                * freq_squared * 0.93 * 1.0e-24;
        denominator = C * 180.0 * 180.0 * 512.0 * 2.0 * log((double)2.0) * pow((double)10.0,(double)(rx_mismatch/10.0));
        afloat = numerator/denominator;

        logk = 10 * log10((double)afloat);
        twoagain = radar[FORE].antenna_gain * 2.0;
        pwrdbm = 30 + 10 * log10((double)(radar[FORE].peak_power * 1000.0))
                - fradar[FORE].loss_out;
        radar[FORE].radar_const = logk + twoagain + pwrdbm;

        freq_squared = engin_win.avg_freq[AFT] * engin_win.avg_freq[AFT]
                * 1.0e18;
        numerator = pow((double)PI, (double)5.0) * radar[AFT].horz_beam_width
                * radar[AFT].vert_beam_width * engin_win.chipdur * 1.0e-6
                * freq_squared * 0.93 * 1.0e-24;
        denominator = C * 180.0 * 180.0 * 512.0 * 2.0 * log((double)2.0)* pow((double)10.0,(double)(rx_mismatch/10.0));
        afloat = numerator/denominator;

        logk = 10 * log10((double)afloat);
        twoagain = radar[AFT].antenna_gain * 2.0;
        pwrdbm = 30 + 10 * log10((double)(radar[AFT].peak_power * 1000.0))
                - fradar[AFT].loss_out;
        radar[AFT].radar_const = logk + twoagain + pwrdbm;

        radar_st.radar_const = 3;
        number_to_calculate--;
    }

    /* The nominal peak power comes from a measurement made on each individual
     HPA.  This power is transmitted frequency dependent, but simply
     place the value for the average transmitted frequency here  (in kw) */

    if (radar_st.peak_power != 0)
        number_to_calculate--;
    else if (engin_st.forehpa_snum > 0 && engin_st.afthpa_snum > 0
            && engin_st.avg_freq_index[0] > 0) {
        radar[FORE].peak_power
                = hpa[engin_win.forehpa_snum].peak_power[engin_win.avg_freq_index[FORE]];
        radar[AFT].peak_power
                = hpa[engin_win.afthpa_snum].peak_power[engin_win.avg_freq_index[AFT]];
        radar_st.peak_power = 4;
        number_to_calculate--;
    }

    /* Currently the ELDORA assumes a noise power of zero, this is a very hard
     parameter to measure since it is dependent on the background the radar
     is looking in to (in dBm) The receiver noise floor (dBm) can be calculated
     (and used for sensitivity estimates only) from the antenna noise
     temperature and the receiver noise figure*/

    if (radar_st.noise_power != 0)
        number_to_calculate--;
    else if (fradar_st.r_noise_figure > 0 && fradar_st.loss_in > 0
            && fradar_st.ant_noise_temp > 0 && engin_st.chipdur > 0) {
        receiver_noise_temp = 290.0 * (pow(10.0,
                                           (double)(fradar[FORE].r_noise_figure
                                                   /10.0)) - 1.0);
        waveguide_loss = pow(10.0, (double)(fradar[FORE].loss_in/10.0));
        system_noise_temp = fradar[FORE].ant_noise_temp + 290.0
                * (waveguide_loss - 1) + waveguide_loss * receiver_noise_temp;
        bandwidth = 1.0e6/engin_win.chipdur;
        radar[FORE].noise_power = 30 + 10
                * log10((double)(BOLTZMAN * system_noise_temp * bandwidth));

        receiver_noise_temp = 290.0 * (pow(10.0,
                                           (double)(fradar[AFT].r_noise_figure
                                                   /10.0)) - 1.0);
        waveguide_loss = pow(10.0, (double)(fradar[AFT].loss_in/10.0));
        system_noise_temp = fradar[AFT].ant_noise_temp + 290.0
                * (waveguide_loss - 1) + waveguide_loss * receiver_noise_temp;
        bandwidth = 1.0e6/engin_win.chipdur;
        radar[AFT].noise_power = 30 + 10
                * log10((double)(BOLTZMAN * system_noise_temp * bandwidth));

        radar_st.noise_power = 3;
        number_to_calculate--;
    }

    /* The receiver gain is frequency dependent but just place the average here
     (in dB) */

    if (radar_st.receiver_gain != 0)
        number_to_calculate--;
    else if (engin_st.avg_freq_index[0] > 0) {
        radar[FORE].receiver_gain
                = rcvr[FORE].receiver_gain[engin_win.avg_freq_index[FORE]];
        radar[AFT].receiver_gain
                = rcvr[AFT].receiver_gain[engin_win.avg_freq_index[AFT]];
        radar_st.receiver_gain = 1;
        number_to_calculate--;
    }

    /* The antenna gain is dependent on which antenna is in use.  LCTAR has given
     us gains to use, but these are dependent on transmitted frequency (in dB) */

    if (radar_st.antenna_gain != 0)
        number_to_calculate--;
    else if (engin_st.avg_freq_index[0] > 0) {
        radar[FORE].antenna_gain
                = antenna[FORE].gain[engin_win.avg_freq_index[FORE]];
        radar[AFT].antenna_gain
                = antenna[AFT].gain[engin_win.avg_freq_index[AFT]];
        radar_st.antenna_gain = 1;
        number_to_calculate--;
    }

    /* System gain (in dB) is defined to be the sum of all gains from the LNA
     to the digitizers */

    if (radar_st.system_gain != 0)
        number_to_calculate--;
    else if (fradar_st.x_band_gain > 0 && radar_st.receiver_gain > 0
            && engin_st.avg_freq_index[0] > 0) {

        radar[FORE].system_gain = fradar[FORE].x_band_gain
                + radar[FORE].receiver_gain
                + rcvr[FORE].if_gain[engin_win.avg_freq_index[FORE]];

        radar[AFT].system_gain = fradar[AFT].x_band_gain
                + radar[AFT].receiver_gain
                + rcvr[AFT].if_gain[engin_win.avg_freq_index[AFT]];

        radar_st.system_gain = 3;
        number_to_calculate--;
    }

    /* Calculate the horizontal beamwidth of the fore and aft antennas.  This
     depends on transmitted frequency and was measured by LCTAR.  */

    if (radar_st.horz_beam_width != 0)
        number_to_calculate--;
    else if (engin_st.avg_freq_index[0] > 0) {
        radar[FORE].horz_beam_width
                = antenna[FORE].hbeam_width[engin_win.avg_freq_index[FORE]];
        radar[AFT].horz_beam_width
                = antenna[AFT].hbeam_width[engin_win.avg_freq_index[AFT]];
        radar_st.horz_beam_width = 3;
        number_to_calculate--;
    }

    /* Calculate the vertical beamwidth of the fore and aft antennas.  This
     depends on transmitted frequency and was measured by LCTAR.  */

    if (radar_st.vert_beam_width != 0)
        number_to_calculate--;
    else if (engin_st.avg_freq_index[0] > 0) {
        radar[FORE].vert_beam_width
                = antenna[FORE].vbeam_width[engin_win.avg_freq_index[FORE]];
        radar[AFT].vert_beam_width
                = antenna[AFT].vbeam_width[engin_win.avg_freq_index[AFT]];
        radar_st.vert_beam_width = 3;
        number_to_calculate--;
    }

    /* The required rotational velocity depends on the rotational sample spacing
     desired and on the dwell time (in deg/sec) */

    if (radar_st.req_rotat_vel != 0)
        number_to_calculate--;
    else if (engin_st.rot_spacing > 0 && waveform_st.repeat_seq > 0
            && waveform_st.repeat_seq_dwel > 0) {
        /* First calculate the dwell time in seconds */
        afloat = waveform.repeat_seq * waveform.repeat_seq_dwel * 0.001;

        radar[FORE].req_rotat_vel = engin_win.rot_spacing / afloat;
        radar[AFT].req_rotat_vel = engin_win.rot_spacing / afloat;

        radar_st.req_rotat_vel = 3;
        number_to_calculate--;
    }

    /* The number of parameters in use is dependent on the pulsing/sampling
     scheme number */

    if (radar_st.num_parameter_des != 0)
        number_to_calculate--;
    else if (engin_st.schemenum > 0) {
        radar[FORE].num_parameter_des = pscheme[engin_win.schemenum].num_params;
        radar[AFT].num_parameter_des = pscheme[engin_win.schemenum].num_params;

        actual_num_params[FORE] = pscheme[engin_win.schemenum].num_params;
        actual_num_params[AFT] = pscheme[engin_win.schemenum].num_params;

        radar_st.num_parameter_des = 1;
        number_to_calculate--;
    }

    /* The total number of descriptors is two (2) plus the number of parameters
     the two extra are the field radar info and the cell spacing descriptor */

    if (radar_st.total_num_des != 0)
        number_to_calculate--;
    else if (radar_st.num_parameter_des > 0) {
        radar[FORE].total_num_des = radar[FORE].num_parameter_des + 2;
        radar[AFT].total_num_des = radar[AFT].num_parameter_des + 2;

        radar_st.total_num_des = 1;
        number_to_calculate--;
    }

    /* The effective unambiguous velocity (m/s) depends on the number of PCP that
     set the unambigous velocity and the time of a PCP */

    if (radar_st.eff_unamb_vel != 0)
        number_to_calculate--;
    else if (waveform_st.repeat_seq > 0 && waveform_st.total_pcp > 0
            && waveform_st.uv_pcp > 0 && engin_st.avg_freq[0] > 0) {
        /* first obtain the PCP (pulse clock period) in seconds */
        afloat = (waveform.repeat_seq * 0.001)/waveform.total_pcp;

        radar[FORE].eff_unamb_vel
                = C /
                (afloat * waveform.uv_pcp * 4 * engin_win.avg_freq[FORE] * 1.0e9);
        radar[AFT].eff_unamb_vel = C /
        (afloat * waveform.uv_pcp * 4 * engin_win.avg_freq[AFT] * 1.0e9);

        radar_st.eff_unamb_vel = 3;
        number_to_calculate--;
    }
    /* The effective unambiguous range (km) depends on the number of PCP that
     set the unambigous range and the time of a PCP */

    if (radar_st.eff_unamb_range != 0)
        number_to_calculate--;
    else if (waveform_st.repeat_seq > 0 && waveform_st.total_pcp > 0
            && waveform_st.ur_pcp > 0) {
        /* first obtain the PCP (pulse clock period) in micro - seconds */
        afloat = (waveform.repeat_seq * 1000.0)/waveform.total_pcp;

        radar[FORE].eff_unamb_range = .15 * afloat * waveform.ur_pcp;
        radar[AFT].eff_unamb_range = .15 * afloat * waveform.ur_pcp;

        radar_st.eff_unamb_range = 3;
        number_to_calculate--;
    }

    /* The number of frequencies transmitted is pulse/sampling scheme dependent */

    if (radar_st.num_freq_trans != 0)
        number_to_calculate--;
    else if (waveform_st.num_chips[0] > 0 && engin_st.chipdur > 0) {
        if (engin_win.chipdur == 0.25)
            delta_freq = 0.012;
        if (engin_win.chipdur == 0.5)
            delta_freq = 0.010;
        if (engin_win.chipdur == 0.75)
            delta_freq = 0.009333;
        if (engin_win.chipdur == 1.0)
            delta_freq = 0.010;
        if (engin_win.chipdur == 1.25)
            delta_freq = 0.0096;
        if (engin_win.chipdur == 1.5)
            delta_freq = 0.008666;
        if (engin_win.chipdur == 1.75)
            delta_freq = 0.009143;
        if (engin_win.chipdur == 2.0)
            delta_freq = 0.010;
        if (engin_win.chipdur > 2.0)
            delta_freq = 0.010;

#ifdef LO_RT_SDLB
        freq_mult[0] = 2;
        freq_mult[1] = 0;
        freq_mult[2] = -2;
        freq_mult[3] = 4;
        freq_mult[4] = -4;
#endif
        /* NOTE: Changed order of freq to fix fore/aft leakage problem in f2! */
#ifndef LO_RT_SDLB
        freq_mult[0] = 1;
        freq_mult[1] = 0;
        freq_mult[2] = -1;
        freq_mult[3] = 2;
        freq_mult[4] = -2;
#endif  
        radar[FORE].num_freq_trans = 0;
        radar[AFT].num_freq_trans = 0;
        for (i=0; i<5; i++) {
            tmp_dbl = (double)rcvr[FORE].cntr_freq + (double)(freq_mult[i]
                    * delta_freq);
            freqs[FORE][i] = tmp_dbl;
            tmp_dbl = (double)rcvr[AFT].cntr_freq + (double)(freq_mult[i]
                    * delta_freq);
            freqs[AFT][i] = tmp_dbl;

            if (waveform.num_chips[i] > 0) {
                radar[FORE].num_freq_trans++;
                radar[AFT].num_freq_trans++;
            }
        }

        if (waveform.num_chips[0] > 0) {
            radar[FORE].freq1 = freqs[FORE][0];
            radar[AFT].freq1 = freqs[AFT][0];
        } else {
            radar[FORE].freq1 = -999.;
            radar[AFT].freq1 = -999.;
        }

        if (waveform.num_chips[1] > 0) {
            radar[FORE].freq2 = freqs[FORE][1];
            radar[AFT].freq2 = freqs[AFT][1];
        } else {
            radar[FORE].freq2 = -999.;
            radar[AFT].freq2 = -999.;
        }

        if (waveform.num_chips[2] > 0) {
            radar[FORE].freq3 = freqs[FORE][2];
            radar[AFT].freq3 = freqs[AFT][2];
        } else {
            radar[FORE].freq3 = -999.;
            radar[AFT].freq3 = -999.;
        }

        if (waveform.num_chips[3] > 0) {
            radar[FORE].freq4 = freqs[FORE][3];
            radar[AFT].freq4 = freqs[AFT][3];
        } else {
            radar[FORE].freq4 = -999.;
            radar[AFT].freq4 = -999.;
        }
        if (waveform.num_chips[4] > 0) {
            radar[FORE].freq5 = freqs[FORE][4];
            radar[AFT].freq5 = freqs[AFT][4];
        } else {
            radar[FORE].freq5 = -999.;
            radar[AFT].freq5 = -999.;
        }

        radar_st.num_freq_trans = 1;
        radar_st.freq1 = 3;
        number_to_calculate--;
    }

    /* The number of inter-pulse-periods (ipps) used is pulsing/sampling
     scheme dependent.  The length of the interpulse periods depends
     also on the length of a PCP */

    if (radar_st.num_ipps_trans != 0)
        number_to_calculate--;
    else if (engin_st.schemenum > 0 && waveform_st.repeat_seq > 0
            && waveform_st.total_pcp > 0) {
        radar[FORE].num_ipps_trans = pscheme[engin_win.schemenum].numipps;
        radar[AFT].num_ipps_trans = pscheme[engin_win.schemenum].numipps;

        /* Now calculate the PCP in milli-seconds */
        afloat = waveform.repeat_seq / waveform.total_pcp;

        /* there is always at least one ipp */
        radar[FORE].interpulse_per1
                = pscheme[engin_win.schemenum].pcp_per_ipp[0] * afloat;
        radar[AFT].interpulse_per1
                = pscheme[engin_win.schemenum].pcp_per_ipp[0] * afloat;

        if (radar[FORE].num_ipps_trans > 1) {
            radar[FORE].interpulse_per2
                    = pscheme[engin_win.schemenum].pcp_per_ipp[1] * afloat;
            radar[AFT].interpulse_per2
                    = pscheme[engin_win.schemenum].pcp_per_ipp[1] * afloat;
        } else {
            radar[FORE].interpulse_per2 = -999.;
            radar[AFT].interpulse_per2 = -999.;
        }
        if (radar[FORE].num_ipps_trans > 2) {
            radar[FORE].interpulse_per3
                    = pscheme[engin_win.schemenum].pcp_per_ipp[2] * afloat;
            radar[AFT].interpulse_per3
                    = pscheme[engin_win.schemenum].pcp_per_ipp[2] * afloat;
        } else {
            radar[FORE].interpulse_per3 = -999.;
            radar[AFT].interpulse_per3 = -999.;
        }
        if (radar[FORE].num_ipps_trans > 3) {
            radar[FORE].interpulse_per4
                    = pscheme[engin_win.schemenum].pcp_per_ipp[3] * afloat;
            radar[AFT].interpulse_per4
                    = pscheme[engin_win.schemenum].pcp_per_ipp[3] * afloat;
        } else {
            radar[FORE].interpulse_per4 = -999.;
            radar[AFT].interpulse_per4 = -999.;
        }
        if (radar[FORE].num_ipps_trans > 4) {
            radar[FORE].interpulse_per5
                    = pscheme[engin_win.schemenum].pcp_per_ipp[4] * afloat;
            radar[AFT].interpulse_per5
                    = pscheme[engin_win.schemenum].pcp_per_ipp[4] * afloat;
        } else {
            radar[FORE].interpulse_per5 = -999.;
            radar[AFT].interpulse_per5 = -999.;
        }

        radar_st.num_ipps_trans = 3;
        number_to_calculate--;
    }

    return (number_to_calculate);

}/*end calculate_radar */
