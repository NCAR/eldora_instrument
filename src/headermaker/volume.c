#define scope extern
#include <time.h>
#include <stdlib.h>
#include "mkrDef.h"
#include "mkrGbl.h"
#include "mkrFunc.h"

/*********************************************************************
 SUBROUTINE EDIT VOLUME DESCRIPTOR
 **********************************************************************/

void edit_volume(vol_pntr)
    struct volume_d *vol_pntr; {

    int choice;
    char string[80];
    char input[80];
    short format, volume_number;
    int maxchars;
    int year, month, day;
    int dhour, dmin, dsec;
    int genyear, genmonth, genday;
    int max_bytes;
    int sensors;

    maxchars = 8;

    for (;;) {
        printf("VOLUME HEADER MENU\n");
        do {
            printf(" 0.  TO RETURN TO THE MAIN MENU\n");
            printf(" 1.  INPUT THE VERSION NUMBER OF THE FORMAT\n");
            printf(" 2.  INPUT THE VOLUME NUMBER FROM BEGINNING OF THE SCAN\n");
            printf(" 3.  INPUT THE MAXIMUM NUMBER OF BYTES IN A DATA RECORD\n");
            printf(" 4.  INPUT THE PROJECT NAME (20 ASCII CHARACTERS)\n");
            printf(" 5.  INPUT THE DATE\n");
            printf(" 6.  INPUT THE TIME\n");
            printf(" 7.  INPUT THE FLIGHT NUMBER (8 ASCII CHARACTERS)\n");
            printf(" 8.  INPUT THE GENERATION FACILITY\n");
            printf(" 9.  INPUT THE GENERATION DATE OF THE VOLUME\n");
            printf("10.  INPUT THE NUMBER OF SENSOR DESCRIPTORS\n");
            fgets(input, sizeof(input), stdin);
            choice = atoi(input);
            if ((choice < 0) || (choice >10))
                printf("INPUT ERROR PLEASE REENTER\n");
        } while ((choice <0) || (choice > 10));

        switch (choice) {
        case 0:
            return;
            break;
        case 1:
            printf("ENTER FORMAT VERSION:\n");
            fgets(input, sizeof(input), stdin);
            format = atoi(input);
            vol_pntr->format_version = format;
            break;
        case 2:
            printf("ENTER VOLUME NUMBER:\n");
            fgets(input, sizeof(input), stdin);
            volume_number = atoi(input);
            vol_pntr->volume_num = volume_number;
            break;
        case 3:
            printf("ENTER BYTES:\n");
            fgets(input, sizeof(input), stdin);
            max_bytes = atoi(input);
            vol_pntr->maximum_bytes = max_bytes;
            break;
        case 4:
            printf("ENTER THE PROJECT NAME\n");
            do {
                fgets(input, sizeof(string), stdin);
                if (strlen(string) > 20)
                    printf("STRING TOO LONG\n");
            } while (strlen(string) > 20);
            strcpy(vol_pntr->proj_name, string);
            break;
        case 5:
            printf("YEAR ?\n");
            fgets(input, sizeof(input), stdin);
            year = atoi(input);
            vol_pntr->year = year;
            printf("MONTH ?\n");
            fgets(input, sizeof(input), stdin);
            month = atoi(input);
            vol_pntr->month = month;
            printf("DAY   ?\n");
            fgets(input, sizeof(input), stdin);
            day = atoi(input);
            vol_pntr->day = day;
            break;
        case 6:
            printf("HOUR ?\n");
            fgets(input, sizeof(input), stdin);
            dhour = atoi(input);
            vol_pntr->data_set_hour = dhour;
            printf("MINUTE ?\n");
            fgets(input, sizeof(input), stdin);
            dmin = atoi(input);
            vol_pntr->data_set_minute = dmin;
            printf("SECOND ?\n");
            fgets(input, sizeof(input), stdin);
            dsec = atoi(input);
            vol_pntr->data_set_second = dsec;
            break;
        case 7:
            printf("PLEASE ENTER THE FLIGHT NUMBER (8 ASCII)\n");
            do {
                fgets(input, sizeof(string), stdin);
                if (strlen(string) > maxchars)
                    printf("STRING TOO LONG\n");
            } while (strlen(string) > maxchars);
            strcpy(vol_pntr->flight_num, string);
            break;
        case 8:
            do {
                printf("ENTER THE GENERATION FACILITY (8 ASCII)\n");
                fgets(input, sizeof(string), stdin);
                if (strlen(string) > 8)
                    printf("STRING TOO LONG\n");
            } while (strlen(string) > 8);
            strcpy(vol_pntr->gen_facility, string);
            break;
        case 9:
            printf("GENERATION YEAR ?\n");
            fgets(input, sizeof(input), stdin);
            genyear = atoi(input);
            vol_pntr->gen_year = genyear;
            printf("GENERATION MONTH ?\n");
            fgets(input, sizeof(input), stdin);
            genmonth = atoi(input);
            vol_pntr->gen_month = genmonth;
            printf("GENERATION DAY   ?\n");
            fgets(input, sizeof(input), stdin);
            genday = atoi(input);
            vol_pntr->gen_day = genday;
            break;
        case 10:
            printf("ENTER THE NUMBER OF SENSOR DESCRIPTORS\n");
            fgets(input, sizeof(input), stdin);
            sensors = atoi(input);
            vol_pntr->number_sensor_des = sensors;
            break;
        default:
            break;
        }

    }/*for*/
}/*end edit_volume_descriptor*/

/***********************************************************************/
/*               SUBROUTINE INITIALIZE VOLUME DESCRIPTOR                    */
/**********************************************************************/

void initialize_volume(vol_pntr)

    struct volume_d *vol_pntr;

{
    int size, i;
    char input[80];

    /*INITIALIZE VOLUME DESCRIPTOR*/

    vol_pntr->volume_des[0] = 'V';
    vol_pntr->volume_des[1] = 'O';
    vol_pntr->volume_des[2] = 'L';
    vol_pntr->volume_des[3] = 'D';

    size = sizeof(struct volume_d);
    vol_pntr->volume_des_length = size;
#ifdef NEW
    vol_pntr->format_version = 2;
#endif
#ifndef NEW
    vol_pntr->format_version = 1;
#endif
    vol_pntr->volume_num = -999;
    vol_pntr->maximum_bytes = 32768;

    printf("Enter the project name: ");
    fgets(input, sizeof(input), stdin);
    for (i = 0; i < 20; i++)
        vol_pntr->proj_name[i] = input[i];
    cleanup(vol_pntr->proj_name, 20);

    vol_pntr->flight_num[0] = ' ';
    vol_pntr->flight_num[1] = ' ';
    vol_pntr->flight_num[2] = ' ';
    vol_pntr->flight_num[3] = ' ';
    vol_pntr->flight_num[4] = ' ';
    vol_pntr->flight_num[5] = ' ';
    vol_pntr->flight_num[6] = ' ';
    vol_pntr->flight_num[7] = ' ';

    vol_pntr->gen_facility[0] = 'E';
    vol_pntr->gen_facility[1] = 'L';
    vol_pntr->gen_facility[2] = 'D';
    vol_pntr->gen_facility[3] = 'O';
    vol_pntr->gen_facility[4] = 'R';
    vol_pntr->gen_facility[5] = 'A';
    vol_pntr->gen_facility[6] = ' ';
    vol_pntr->gen_facility[7] = ' ';

    vol_pntr->number_sensor_des = 4;

}/*end initialize_volume */

/**************************************************************************/
/*             SUBROUTINE fPRINT VOLUME DESCRIPTOR                        */
/**************************************************************************/
void fprint_volume(vol_pntr,
                   print_pntr)
    struct volume_d *vol_pntr;FILE *print_pntr; {
    int i;

    fprintf(print_pntr,
            "************VOLUME DESCRIPTOR************************\n");
    fprintf(print_pntr, 
            "VOLDID     VOLUME IDENTIFIER                : ");
    for (i=0; i<4; i++)
        fprintf(print_pntr, "%c", vol_pntr->volume_des[i]);
    fprintf(print_pntr, "\n");
    fprintf(print_pntr, 
            "VOLDDESLEN VOLUME DESCRIPTOR LENGTH         : %ld\n",
            vol_pntr->volume_des_length);
    fprintf(print_pntr,
            "VOLDFORVER FORMAT VERSION NUMBER IS         : %d\n",
            vol_pntr->format_version);
    fprintf(print_pntr, 
            "VOLDVOLNUM VOLUME NUMBER IS                 : %d\n", vol_pntr->volume_num);
    fprintf(print_pntr,
            "VOLDMAXBYT MAXIMUM NUM BYTES IN DATA RECORD : %ld\n",
            vol_pntr->
            maximum_bytes);
    fprintf(print_pntr, 
            "VOLDPROJ   PROJECT NAME                     : ");
    for (i = 0; i < 20; i++)
        fprintf(print_pntr, "%c", vol_pntr->proj_name[i]);
    fprintf(print_pntr,"\n");
    fprintf(print_pntr,
            "VOLDDATE   DATE                             : %2d/%2d/%4d (dy/mn/yr)\n",
            vol_pntr->day,
            vol_pntr->month,
            vol_pntr->year);
    fprintf(print_pntr,
            "VOLDTIME   TIME                             : %d  : %d : %d\n",
            vol_pntr->data_set_hour,
            vol_pntr->data_set_minute,
            vol_pntr->data_set_second);
    fprintf(print_pntr, 
            "VOLDFLIGHT FLIGHT NUMBER                    : ");
    for (i = 0; i < 8; i++)
        fprintf(print_pntr, "%c", vol_pntr->flight_num[i]);
    fprintf(print_pntr, "\n");
    fprintf(print_pntr, 
            "VOLDFCILTY GENERATION FACILITY NAME         : ");
    for (i = 0; i < 8; i++)
        fprintf(print_pntr, "%c", vol_pntr->gen_facility[i]);
    fprintf(print_pntr,"\n");
    fprintf(print_pntr,
            "VOLDGENDAT VOLUME GENERATION DATE           : %2d/%2d/%4d (dy/mn/yr)\n",
            vol_pntr->gen_day,
            vol_pntr->gen_month,
            vol_pntr->gen_year);
    fprintf(print_pntr,
            "VOLDNUMSEN NUMBER OF SENSOR DESCRIPTORS     : %d\n",
            vol_pntr->number_sensor_des);
}/*end print_volume_descriptor*/

/***********************************************************************/
/*               SUBROUTINE CALCULATE VOLUME DESCRIPTOR               */
/**********************************************************************/

int calculate_volume() {
    struct tm *current_time;
    long clock;

    clock = time(NULL);
    current_time = localtime(&clock);

    volume.year = current_time->tm_year;
    volume.month = current_time->tm_mon + 1;
    volume.day = current_time->tm_mday;

    volume.data_set_hour = current_time->tm_hour;
    volume.data_set_minute = current_time->tm_min;
    volume.data_set_second = current_time->tm_sec;

    volume.gen_year = current_time->tm_year;
    volume.gen_month = current_time->tm_mon + 1;
    volume.gen_day = current_time->tm_mday;

    return (0);
}/*end calculate_volume */

