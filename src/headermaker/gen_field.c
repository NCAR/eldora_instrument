#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#define scope
#include "mkrDef.h"
#include "mkrGbl.h"
#include "mkrFunc.h"

int foreaft();
void initialize_menu();
void edit_menu();
void print_menu_screen();
void print_menu_printer();
void re_cal();
void menu(int*);
void num_params(int radarnum);
 
int
main(int argc,
     char** argv) {

    int choice, size, byteswritten, i, j, flag, num_errors;
    FILE *outfdf = 0;
    char outname[128];

    /* Determine Processor Type */

    processor_type = big_little();

    /* Set all physical parameter constants */
    override = 0;
    set_hpa();
    set_antenna();
    set_pscheme();
    set_rcvr();
    set_attenuator();
    for (;;) {
        menu(&choice);
        switch (choice) {

        /* End the program */

        case 0:
            if (outfdf)
                fclose(outfdf);
            exit(0);
            break;

            /* Get the initialize menu */

        case 1:
            initialize_menu();
            break;

            /* Get the edit menu */

        case 2:
            edit_menu();
            flag = 0;
            verify(flag, &num_errors);
            break;

            /* Write the volume header to a file */

        case 3:
            num_errors = 0;
            flag = 0;
            verify(flag, &num_errors);
            if (num_errors > 0 && !override) {
                printf("Unable to write file due to errors listed above\n\n");
                break;
            }

            /* get the filename and open it */

            printf("OUTPUT FILE NAME: \n");
            fgets(outname, sizeof(outname), stdin);
            /* remove trailing newline */
            outname[strlen(outname)-1] = '\0';
            printf("OPENING OUTPUT FILE: %s\n", outname);
            if ((outfdf = fopen(outname, "wb")) == NULL) {
                printf("UNABLE TO OPEN THE OUTPUT FILE\n");
                exit(-1);
            }
#ifdef NEW
            if(volume.format_version == 1)
                volume.format_version = 2; /* always write new format */
            /* 
             * Copy the tail portion of outname (everything after the last
             * slash, if any) to the file_name of the fore and aft FRIBs
             */
            {
                char fname[80];
                char* lastslash = rindex(outname, '/');
                memset(fname, 0, sizeof(fname)); /* null fill */
                if (lastslash)
                    strncpy(lastslash + 1, fname, sizeof(fname));
                else
                    strncpy(outname, fname, sizeof(fname));
                for(i = 0; i < 80; i++) {
                    memcpy(fradar[FORE].file_name, fname, 80);
                    memcpy(fradar[AFT].file_name, fname, 80);
                }
            }
#endif
            unpack_hd(); /* byte-swap header if on little Endian machine */
            unpk_short(&radar[FORE].num_parameter_des, 1, 0);
            unpk_short(&radar[AFT].num_parameter_des, 1, 0);

            /* write out the entire header */

            size = sizeof(volume);
            cleanup(volume.proj_name, 20);
            cleanup(volume.gen_facility, 8);
            cleanup(volume.flight_num, 8);
            byteswritten = fwrite((char *)&volume, size, 1, outfdf);
            size = sizeof(waveform);
            cleanup(waveform.ps_file_name, 16);
            byteswritten += fwrite((char *)&waveform, size, 1, outfdf);

            /*WRITE OUT SENSOR DESCRIPTORS FOR EACH RADAR*/

            for (i=0; i<actual_num_radars; i++) {
                size = sizeof(struct radar_d);
                cleanup(radar[i].radar_name, 8);
                byteswritten += fwrite((char *)&radar[i], size, 1, outfdf);
#ifdef NEW
                size = sizeof(struct field_radar_i);
#else
                size = sizeof(struct field_radar_i) - 80*sizeof(char);
#endif
                byteswritten += fwrite((char *)&fradar[i], size, 1, outfdf);
                size = sizeof(struct cell_spacing_d);
                byteswritten += fwrite((char *)&cell[i], size, 1, outfdf);
                for (j=0; j<actual_num_params[i]; j++) {
                    size = sizeof(struct parameter_d);
                    cleanup(parameter[j][i].parameter_name, 8);
                    cleanup(parameter[j][i].param_description, 40);
                    cleanup(parameter[j][i].param_units, 8);
                    cleanup(parameter[j][i].threshold_field, 8);
                    byteswritten += fwrite((char *)&parameter[j][i],
                                           size,
                                           1,
                                           outfdf);
                }
            }

            /* Write out the navigation and isitu sensor descriptors */

            size = sizeof(navigation);
            byteswritten += fwrite((char *)&navigation, size, 1, outfdf);
            size = sizeof(insitudata);
            byteswritten += fwrite((char *)&insitudata, size, 1, outfdf);

            fclose(outfdf);
            outfdf = 0;

            unpack_hd(); /* byte-swap header if on little Endian machine */
            unpk_short(&radar[FORE].num_parameter_des, 1, 0);
            unpk_short(&radar[AFT].num_parameter_des, 1, 0);
            break;

            /* Read in an existing header from a file and display it */

        case 4:

            read_header(0);
            unpack_hd(); /* byte swap header -- on little endian machine */
            fprint_volume(&volume, stdout);
            printf("\n");
            /*           sleep(2);
             fprint_waveform(&waveform,stdout);
             printf("\n");
             sleep(2);
             for(i=0; i<actual_num_radars; i++)
             {
             fprint_radar(&radar[i],stdout);
             printf("\n");
             sleep(2);
             fprint_fieldradar(&fradar[i],stdout);
             printf("\n");
             sleep(2);
             fprint_cellspacing(&cell[i],stdout);
             printf("\n");
             sleep(2);
             for(j=0; j<actual_num_params[i]; j++)
             {
             fprint_parameter(&parameter[j][i],stdout);
             printf("\n");
             sleep(2);
             }
             }


             fprint_navigation(&navigation,stdout);
             printf("\n");
             */
            break;

            /* Get the print on screen menu */

        case 5:
            print_menu_screen();
            break;

            /* Get the print on printer menu */

        case 6:
            print_menu_printer();
            break;

            /* Start a new header here */

        case 7:
            make_new();
            flag = 0;
            verify(flag, &num_errors);
            break;

            /* Verify a header here */

        case 8:
            flag = 1;
            verify(flag, &num_errors);
            break;

            /* Re-calibrate an existing header */
        case 9:
            re_cal();
            flag = 0;
            verify(flag, &num_errors);
            break;

        default:
            break;
        }/*switch*/
    }/*for*/

}/*end main*/

/**************************************************************************/
/*                            MAIN MENU                                   */
/**************************************************************************/
void menu(int* choice) {
    int ansr;
    char string[80];
    do {
        printf("0. EXIT THIS PROGRAM\n");
        printf("1. INITIALIZE MENU\n");
        printf("2. EDIT MENU\n");
        printf("3. WRITE THE VOLUME HEADER TO A FILE\n");
        printf("4. READ IN A HEADER FROM A FILE\n");
        printf("5. PRINT DESCRIPTORS TO SCREEN MENU\n");
        printf("6. SEND DESCRIPTORS TO LASER PRINTER\n");
        printf("7. CREATE A NEW HEADER FROM SCRATCH\n");
        printf("8. VERIFY CURRENT HEADER\n");
        printf("9. RE_CALIBRATE EXISTING HEADER\n");
        printf("ENTER YOUR CHOICE\n");
        fgets(string, sizeof(string), stdin);
        ansr = atoi(string);
        if (ansr == 77) {
            override = 1;
            printf("Override turned on\n");
        }
        if (ansr < 0 || ansr > 9)
            printf("INVALID ENTRY\n");
    } while (ansr < 0 || ansr > 9);
    *choice = ansr;
}

/*************************************************************************/
/*                     INITIALIZE MENU                                   */
/*************************************************************************/

void initialize_menu() {
    int choice;
    int i;
    int radarnum;
    char input[80];

    for (;;) {
        do {
            printf("\n***************INITIALIZE MENU*******************\n");
            printf("0. RETURN TO MAIN MENU\n");
            printf("1. INITIALIZE THE VOLUME DESCRIPTOR\n");
            printf("2. INITIALIZE THE WAVEFORM DESCRIPTOR\n");
            printf("3. INITIALIZE THE FORE AND AFT RADAR DESCRIPTORS\n");
            printf("4. INITIALIZE THE CELL SPACING DESCRIPTORS\n");
            printf("5. INITIALIZE THE FIELD RADAR DESCRIPTORS\n");
            printf("6. INITIALIZE THE PARAMETER DESCRIPTORSS\n");
            printf("7. INITIALIZE THE NAVIGATIONAL DATA DESCRIPTOR\n");
            printf("8. INITIALIZE THE INSITU DATA DESCRIPTOR\n");
            printf("9. INITIALIZE ALL OF THE ABOVE AT ONCE\n");
            printf("ENTER YOUR CHOICE\n");
            fgets(input, sizeof(input), stdin);
            choice = atoi(input);
            if (choice < 0 || choice > 9)
                printf("INVALID ENTRY\n");
        } while (choice < 0 || choice > 9);
        switch (choice) {

        case 0:
            return;
            break;
        case 1:
            initialize_volume(&volume);
            fprint_volume(&volume, stdout);
            break;
        case 2:
            initialize_waveform(&waveform);
            fprint_waveform(&waveform, stdout);
            break;
        case 3:
            radarnum = foreaft();
            initialize_radar(&radar[radarnum], radarnum);
            fprint_radar(&radar[radarnum], stdout);
            break;
        case 4:
            radarnum = foreaft();
            initialize_cellspacing(&cell[radarnum]);
            fprint_cellspacing(&cell[radarnum], stdout);
            break;
        case 5:
            radarnum = foreaft();
            initialize_fieldradar(&fradar[radarnum]);
            fprint_fieldradar(&fradar[radarnum], stdout);
            break;
        case 6:
            radarnum = foreaft();
            num_params(radarnum);
            for (i = 0; i < actual_num_params[radarnum]; i++) {
                initialize_parameter(&parameter[i][radarnum]);
                fprint_parameter(&parameter[i][radarnum], stdout);
            }
            break;
        case 7:
            initialize_navigation(&navigation);
            fprint_navigation(&navigation, stdout);
            break;
        case 8:
            initialize_insitudata(&insitudata);
            fprint_insitudata(&insitudata, stdout);
            break;
        case 9:
            initialize_volume(&volume);
            initialize_waveform(&waveform);
            actual_num_radars = 2;
            initialize_radar(&radar[FORE], FORE);
            initialize_radar(&radar[AFT], AFT);
            initialize_fieldradar(&fradar[FORE]);
            initialize_fieldradar(&fradar[AFT]);
            initialize_cellspacing(&cell[FORE]);
            initialize_cellspacing(&cell[AFT]);
            num_params(FORE);
            num_params(AFT);
            for (i = 0; i < actual_num_params[FORE]; i++)
                initialize_parameter(&parameter[i][FORE]);
            for (i = 0; i < actual_num_params[AFT]; i++)
                initialize_parameter(&parameter[i][AFT]);
            initialize_navigation(&navigation);
            initialize_insitudata(&insitudata);
            break;
        default:
            break;
        }/*switch*/

    }/*for*/

}/*end initialize menu*/
/************************************************************************/
/*          Function foreaft gets an F or an A from the user            */
/*                 and returns either FORE or AFT                       */
/************************************************************************/
int foreaft() {
    char input[80];

    for (;;) {

        printf("Enter F For the fore radar ");
        printf(" or enter A For the aft radar: ");
        fgets(input, sizeof(input), stdin);
        printf("\n");
        if (input[0] == 'F' || input[0] == 'f')
            return (FORE);
        else if (input[0] == 'A' || input[0] == 'a')
            return (AFT);
        else
            printf("invalid entry\n");
    }
}
/************************************************************************/
/*          Function num_params gets the number of parameters           */
/*                 they want to define for a radar                      */
/************************************************************************/
void num_params(int radarnum) {
    char input[80];
    int num;

    do {
        if (radarnum == FORE)
            printf("Enter the number of parameters for the FORE radar: ");
        else if (radarnum == AFT)
            printf("Enter the number of parameters for the AFT radar: ");
        else
            printf("Enter the number of parameters for an unknown radar: ");
        fgets(input, sizeof(input), stdin);
        num = atoi(input);
        if (num > MAX_PARAM || num < 1)
            printf("\nNumber of parameters must be: >1 and < %d", MAX_PARAM);
    } while (num > MAX_PARAM || num < 1);
    actual_num_params[radarnum] = num;
}

/************************************************************************/
/*          Function param_num returns the number of the                */
/*            parameter selected by the user                            */
/************************************************************************/

int specific_param(int radarnum) {
    char input[80];
    int num, i;

    for (i=0; i< actual_num_params[radarnum]; i++)
        printf("%2d) is called: %c%c%c%c%c%c%c%c\n",
               i,
               parameter[i][radarnum].parameter_name[0],
               parameter[i][radarnum].parameter_name[1],
               parameter[i][radarnum].parameter_name[2],
               parameter[i][radarnum].parameter_name[3],
               parameter[i][radarnum].parameter_name[4],
               parameter[i][radarnum].parameter_name[5],
               parameter[i][radarnum].parameter_name[6],
               parameter[i][radarnum].parameter_name[7]);
    do {
        printf("Maximum number of paramaters is %d\n", MAX_PARAM);
        printf("Enter the desired parameter number or %d to add a parameter",
               actual_num_params[radarnum]);
        fgets(input, sizeof(input), stdin);
        num = atoi(input);
        if (num > actual_num_params[radarnum] || num < 0)
            printf("\nParameter number must be: >=0 and <= %d",
                   actual_num_params[radarnum]);
    } while (num > actual_num_params[radarnum] || num < 0);
    if (num == actual_num_params[radarnum] && num < MAX_PARAM) {
        actual_num_params[radarnum]++;
        initialize_parameter(&parameter[num][radarnum]);
    }
    return (num);
}

/************************************************************************/
/*              SUBROUTINE EDIT MENU                                   */
/***********************************************************************/
void edit_menu() {
    int choice;
    int paramnum;
    int radarnum;
    char input[80];

    for (;;) {
        do {
            printf("\n***************EDIT MENU*******************\n");
            printf("0. RETURN TO MAIN MENU\n");
            printf("1. EDIT THE VOLUME DESCRIPTOR\n");
            printf("2. EDIT THE WAVEFORM DESCRIPTOR\n");
            printf("3. EDIT THE FORE OR AFT RADAR DESCRIPTORS\n");
            printf("4. EDIT THE FORE OR AFT FIELD RADAR INFORMATION DESC\n");
            printf("5. EDIT THE FORE OR AFT CELL SPACING DESCRIPTORS\n");
            printf("6. EDIT THE PARAMETER DESCRIPTORS\n");
            printf("7. SET THE NAVIGATIONAL DATA DESCRIPTOR FLAGS\n");
            printf("ENTER YOUR CHOICE\n");
            fgets(input, sizeof(input), stdin);
            choice = atoi(input);
            if (choice < 0 || choice > 7)
                printf("INVALID ENTRY\n");
        } while (choice < 0 || choice > 7);
        switch (choice) {
        case 0:
            return;
        case 1:
            edit_volume(&volume);
            fprint_volume(&volume, stdout);
            break;
        case 2:
            edit_waveform(&waveform);
            fprint_waveform(&waveform, stdout);
            break;
        case 3:
            radarnum = foreaft();
            edit_radar(&radar[radarnum]);
            fprint_radar(&radar[radarnum], stdout);
            break;
        case 4:
            radarnum = foreaft();
            edit_fieldradar(&fradar[radarnum]);
            fprint_fieldradar(&fradar[radarnum], stdout);
            break;
        case 5:
            radarnum = foreaft();
            edit_cellspacing(&cell[radarnum]);
            fprint_cellspacing(&cell[radarnum], stdout);
            break;
        case 6:
            radarnum = foreaft();
            paramnum = specific_param(radarnum);
            edit_parameter(&parameter[paramnum][radarnum]);
            fprint_parameter(&parameter[paramnum][radarnum], stdout);
            break;
        case 7:
            edit_navigation(&navigation);
            fprint_navigation(&navigation, stdout);
            break;
        default:
            break;

        }/*switch*/

    }/*for*/

}/*end edit_menu*/

/****************************************************************************/
/*                 SUBROUTINE PRINT TO SCREEN MENU                           */
/****************************************************************************/

void print_menu_screen() {
    int choice;
    int i;
    int j;
    int radarnum;
    int paramnum;
    char input[80];

    for (;;) {
        do {
            printf("********PRINT DESCRIPTORS MENU************\n");
            printf("0. EXIT PRINT MENU\n");
            printf("1. PRINT VOLUME DESCRIPTOR\n");
            printf("2. PRINT WAVEFORM DESCRIPTOR\n");
            printf("3. PRINT RADAR DESCRIPTOR\n");
            printf("4. PRINT FIELD RADAR DESCRIPTOR\n");
            printf("5. PRINT CELL SPACING DESCRIPTOR\n");
            printf("6. PRINT PARAMETER DESCRIPTORS\n");
            printf("7. PRINT NAVIGATIONAL DATA DESCRIPTOR\n");
            printf("8. PRINT ALL AT ONCE\n");
            printf("ENTER YOUR CHOICE\n");
            fgets(input, sizeof(input), stdin);
            choice = atoi(input);
            if (choice < 0 || choice > 8)
                printf("INVALID ENTRY\n");
        } while (choice < 0 || choice > 8);

        switch (choice) {
        case 0:
            return;
        case 1:
            fprint_volume(&volume, stdout);
            break;
        case 2:
            fprint_waveform(&waveform, stdout);
            break;
        case 3:
            radarnum = foreaft();
            fprint_radar(&radar[radarnum], stdout);
            break;
        case 4:
            radarnum = foreaft();
            fprint_fieldradar(&fradar[radarnum], stdout);
            break;
        case 5:
            radarnum = foreaft();
            fprint_cellspacing(&cell[radarnum], stdout);
            break;
        case 6:
            radarnum = foreaft();
            paramnum = specific_param(radarnum);
            fprint_parameter(&parameter[paramnum][radarnum], stdout);
            break;
        case 7:
            fprint_navigation(&navigation, stdout);
            break;
        default:
            break;
        case 8:
            fprint_volume(&volume, stdout);
            sleep(2);
            fprint_waveform(&waveform, stdout);
            sleep(2);
            for (i=0; i < actual_num_radars; i++) {
                fprint_radar(&radar[i], stdout);
                sleep(2);
                fprint_fieldradar(&fradar[i], stdout);
                sleep(2);
                fprint_cellspacing(&cell[i], stdout);
                sleep(2);
                for (j=0; j<actual_num_params[i]; j++) {
                    fprint_parameter(&parameter[j][i], stdout);
                    sleep(2);
                }
            }
            fprint_navigation(&navigation, stdout);
            sleep(2);
            break;

        }/*switch*/
    }/*for*/
}/*end print_menu*/

/****************************************************************************/
/*                   SUBROUTINE PRINT MENU PRINTER                          */
/****************************************************************************/
void print_menu_printer() {
    int choice;
    int i;
    int j;
    int radarnum;
    int paramnum;
    char input[80];
    FILE *printer;
//    , *popen();
    char *debug_file = getenv("Debug_Output");
    if (debug_file) {
        if ((printer = fopen(debug_file, "w")) == NULL) {
            printf("UNABLE TO OPEN FILE\n");
            return;
        }
        printf("DESCRIPTORS WILL BE PRINTED TO FILE\n");
    } else {
        if ((printer = popen("lp", "w")) == NULL) {
            printf("UNABLE TO ACCESS LASER PRINTER\n");
            return;
        }
        printf("DESCRIPTORS WILL BE PRINTED ON THOR'S LASER PRINTER\n");
    }

    for (;;) {
        do {
            printf("********PRINT DESCRIPTORS MENU************\n");
            printf("0. EXIT PRINT MENU\n");
            printf("1. PRINT VOLUME DESCRIPTOR\n");
            printf("2. PRINT WAVEFORM DESCRIPTOR\n");
            printf("3. PRINT RADAR DESCRIPTOR\n");
            printf("4. PRINT FIELD RADAR DESCRIPTOR\n");
            printf("5. PRINT CELL SPACING DESCRIPTOR\n");
            printf("6. PRINT PARAMETER DESCRIPTORS\n");
            printf("7. PRINT NAVIGATIONAL DATA DESCRIPTOR\n");
            printf("8. PRINT ALL AT ONCE\n");
            printf("ENTER YOUR CHOICE\n");
            fgets(input, sizeof(input), stdin);
            choice = atoi(input);
            if (choice < 0 || choice > 8)
                printf("INVALID ENTRY\n");
        } while (choice < 0 || choice > 8);

        switch (choice) {
        case 0:
            if (debug_file)
                fclose(printer);
            else
                pclose(printer);
            return;
            break;
        case 1:
            fprint_volume(&volume, printer);
            fprintf(printer, "\n\n");
            break;
        case 2:
            fprint_waveform(&waveform, printer);
            fprintf(printer, "\n\n");
            break;
        case 3:
            radarnum = foreaft();
            fprint_radar(&radar[radarnum], printer);
            fprintf(printer, "\n\n");
            break;
        case 4:
            radarnum = foreaft();
            fprint_fieldradar(&fradar[radarnum], printer);
            fprintf(printer, "\n\n");
            break;
        case 5:
            radarnum = foreaft();
            fprint_cellspacing(&cell[radarnum], printer);
            fprintf(printer, "\n\n");
            break;
        case 6:
            radarnum = foreaft();
            paramnum = specific_param(radarnum);
            fprint_parameter(&parameter[paramnum][radarnum], printer);
            fprintf(printer, "\n\n");
            break;
        case 7:
            fprint_navigation(&navigation, printer);
            fprintf(printer, "\n\n");
            break;
        default:
            break;
        case 8:
            fprint_volume(&volume, printer);
            fprintf(printer, "\n\n");
            fprint_waveform(&waveform, printer);
            fprintf(printer, "\n\n");
            for (i=0; i < actual_num_radars; i++) {
                fprint_radar(&radar[i], printer);
                fprintf(printer, "\n\n");
                fprint_fieldradar(&fradar[i], printer);
                fprintf(printer, "\n\n");
                fprint_cellspacing(&cell[i], printer);
                fprintf(printer, "\n\n");
                for (j=0; j<actual_num_params[i]; j++) {
                    fprint_parameter(&parameter[j][i], printer);
                    fprintf(printer, "\n\n");
                }
            }
            fprint_navigation(&navigation, printer);
            fprintf(printer, "\n\n");
            break;

        }/*switch*/
    }/*for*/
}/*end print_menu_printer*/

/*******************************************************************/
/****    Function to cleanup ascii arrays so they look better   ****/
/*******************************************************************/
void cleanup(char* array, int size)
{
    int i, endfound;

    endfound = 0;
    for (i=0; i<size; i++)
        if (endfound)
            *array++ = 0x20; /* fill end with space characters */
        else if ((*array > 0x1F) && (*array < 0x7F))
            array++;
        else {
            *array++ = 0x20;
            endfound = 1;
        }
}
