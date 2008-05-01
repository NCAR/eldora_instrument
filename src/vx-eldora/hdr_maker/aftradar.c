#include <stdio.h>
#include <floatingpoint.h>
#include "field.h"

/**************************************************************************/
/*          SUBROUTINE INITIALIZE AFT RADAR DESCRIPTOR                    */
/*************************************************************************/

initialize_aftradar_descriptor(aftsensordescriptor)
    struct sensorDes *aftsensordescriptor;
{

  int i;

     aftsensordescriptor->radar.radar_des[0] = 'R';
     aftsensordescriptor->radar.radar_des[1] = 'A';
     aftsensordescriptor->radar.radar_des[2] = 'D';
     aftsensordescriptor->radar.radar_des[3] = 'D';
     aftsensordescriptor->radar.radar_des_length = 
                            sizeof(aftsensordescriptor->radar);
     aftsensordescriptor->radar.radar_name[0] = 'A';
     aftsensordescriptor->radar.radar_name[1] = 'F';
     aftsensordescriptor->radar.radar_name[2] = 'T';
     aftsensordescriptor->radar.radar_name[3] = ' ';
     aftsensordescriptor->radar.radar_name[4] = ' ';
     aftsensordescriptor->radar.radar_name[5] = ' ';
     aftsensordescriptor->radar.radar_name[6] = ' ';
     aftsensordescriptor->radar.radar_name[7] = ' ';
     aftsensordescriptor->radar.radar_const = -999.;
     aftsensordescriptor->radar.peak_power = -999.;
     aftsensordescriptor->radar.noise_power = -999.;
     aftsensordescriptor->radar.receiver_gain = -999.;
     aftsensordescriptor->radar.antenna_gain = -999.;
     aftsensordescriptor->radar.system_gain = -999.;
     aftsensordescriptor->radar.horz_beam_width = -999.;
     aftsensordescriptor->radar.vert_beam_width = -999.;
     aftsensordescriptor->radar.radar_type = -999.;
     aftsensordescriptor->radar.scan_mode = -999;
     aftsensordescriptor->radar.req_rotat_vel = -999.;
     aftsensordescriptor->radar.scan_mode_pram0 = -999.;
     aftsensordescriptor->radar.scan_mode_pram1 = -999.;
     aftsensordescriptor->radar.num_parameter_des = -999;
     aftsensordescriptor->radar.total_num_des = -999;
     aftsensordescriptor->radar.data_compress = -999;
     aftsensordescriptor->radar.data_reduction = -999;
     aftsensordescriptor->radar.data_red_parm0 = -999.;
     aftsensordescriptor->radar.data_red_parm1 = -999.;
     aftsensordescriptor->radar.radar_longitude = -999.;
     aftsensordescriptor->radar.radar_latitude = -999.;
     aftsensordescriptor->radar.radar_altitude = -999.;
     aftsensordescriptor->radar.eff_unamb_vel  = -999.;
     aftsensordescriptor->radar.eff_unamb_range = -999.;
     aftsensordescriptor->radar.num_freq_trans  = -999;
     aftsensordescriptor->radar.num_ipps_trans  = -999;
     aftsensordescriptor->radar.freq1  = -999.;
     aftsensordescriptor->radar.freq2  = -999.;
     aftsensordescriptor->radar.freq3  = -999.;
     aftsensordescriptor->radar.freq4  = -999.;
     aftsensordescriptor->radar.freq5  = -999.;
     aftsensordescriptor->radar.interpulse_per1 = -999.;
     aftsensordescriptor->radar.interpulse_per2 = -999.;
     aftsensordescriptor->radar.interpulse_per3 = -999.;
     aftsensordescriptor->radar.interpulse_per4 = -999.;
     aftsensordescriptor->radar.interpulse_per5 = -999.;
     
}/*end initialize_radar_descriptor*/




/****************************************************************************/
/*                     EDIT AFT RADAR DESCRIPTOR                            */
/****************************************************************************/
edit_aftradar_descriptor(aftsensordescriptor)
    struct sensorDes *aftsensordescriptor;

{ 
    int element;
    int i,choice;
    float radarconst,peakpower,noisepower,receivergain,antennagain;
    float systemgain,horz,vert,rotatevel,scanpram0,scanpram1;
    short radartype,scanmode,numparamdes,totalnumdes,datareduction;
    short compression;
    float vel,range;
    short freq_trans,ipps_trans;
    float freq1,freq2,freq3,freq4,freq5;
    float per1,per2,per3,per4,per5;
    short algor;
    float parm0,parm1;
    float longit,lat,alt;
    char string[80],input[80];


    for(;;){
       do{
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
         gets(input);
         choice = atoi(input);
         if(choice < 0 || choice >37) printf("INVALID ENTRY\n");
     }while(choice < 0 || choice >37);
     
     switch(choice){
          case 0: 
                return;
                break;
          case 1:
                do{
                   printf("ENTER NAME\n");
                   gets(string);
                   if(strlen(string) > 8) printf("NAME TOO LONG\n");
	         } while(strlen(string) > 8);
                 strcpy(aftsensordescriptor->radar.radar_name,string);
                 break;
           case 2:
                 printf("ENTER CONSTANT\n");
                 gets(input);
                 radarconst = atof(input);
                 aftsensordescriptor->radar.radar_const = radarconst/1.;
                 break;
           case 3:
                 printf("ENTER PEAK POWER\n");
                 gets(input);
                 peakpower = atof(input);
                 aftsensordescriptor->radar.peak_power = peakpower;
                 break;               
           case 4:
                 printf("ENTER NOISE POWER\n");
                 gets(input);
                 noisepower = atof(input);
                 aftsensordescriptor->radar.noise_power = noisepower;
                 break;
           case 5:
                 printf("ENTER RECEIVER GAIN\n");
                 gets(input);
                 receivergain = atof(input);
                 aftsensordescriptor->radar.receiver_gain = receivergain;
                 break;               
           case 6:
                 printf("ENTER ANTENNA GAIN\n");
                 gets(input);
                 antennagain = atof(input);
                 aftsensordescriptor->radar.antenna_gain = antennagain;
                 break;
           case 7:
                 printf("ENTER SYSTEM GAIN\n");
                 gets(input);
                 systemgain = atof(input);
                 aftsensordescriptor->radar.system_gain = systemgain;
                 break;
           case 8:
                printf("ENTER BEAM WIDTH\n");
                gets(input);
                horz = atof(input);
                aftsensordescriptor->radar.horz_beam_width = horz;
                break;
           case 9:
                printf("ENTER BEAM WIDTH\n");
                gets(input);
                vert = atof(input);
                aftsensordescriptor->radar.vert_beam_width = vert;
                break;
           case 10:
                printf("ENTER RADAR TYPE\n");
                gets(input);
                radartype = atoi(input);
                aftsensordescriptor->radar.radar_type = radartype;
                break;
	   case 11:
                printf("ENTER SCAN MODE\n");
                gets(input);
                scanmode = atoi(input);
                aftsensordescriptor->radar.scan_mode = scanmode;
                break;
	   case 12:
                printf("ENTER VEL\n");
                gets(input);
                rotatevel = atof(input);
                aftsensordescriptor->radar.req_rotat_vel = rotatevel;
                break;
          case 13:
                printf("ENTER PRAM0\n");
                gets(input);
                scanpram0 = atof(input);
                aftsensordescriptor->radar.scan_mode_pram0 = scanpram0;
                break;
          case 14:
                printf("ENTER PRAM1\n");
                gets(input);
                scanpram1 = atof(input);
                aftsensordescriptor->radar.scan_mode_pram1 = scanpram0;
                break;
          case 15:
                printf("ENTER NUMBER\n");
                gets(input);
                numparamdes = atoi(input);
                aftsensordescriptor->radar.num_parameter_des = numparamdes;
                break;
          case 16:
                printf("ENTER NUMBER\n");
                gets(input);
                totalnumdes = atoi(input);
                aftsensordescriptor->radar.total_num_des = totalnumdes;
                break;
          case 17: 
                printf("ENTER THE DATA COMPRESSION SCHEME\n");
                gets(input);
                compression = atoi(input);
                aftsensordescriptor->radar.data_compress = compression;
                break;
          case 18: 
                printf("ENTER REDUCTION\n");
                gets(input);
                datareduction = atoi(input);
                aftsensordescriptor->radar.data_reduction = datareduction;
                break;
          case 19:
                printf("ENTER PARAMETER\n");
                gets(input);
                parm0 = atof(input);                
                aftsensordescriptor->radar.data_red_parm0 = parm0;
                break;
          case 20:
                printf("ENTER PARAMETER\n");
                gets(input);
                parm1 = atof(input);
                aftsensordescriptor->radar.data_red_parm1 = parm1;
                break;
          case 21:
                printf("ENTER LONGITUDE\n");
                gets(input);
                longit = atof(input);
                aftsensordescriptor->radar.radar_longitude = longit;
                break;
          case 22:
                printf("ENTER LATITUDE\n");
                gets(input);
                lat = atof(input);
                aftsensordescriptor->radar.radar_latitude  = lat;
                break;
          case 23:
                printf("ENTER ALTITUDE\n");
                gets(input);
                alt = atof(input);
                aftsensordescriptor->radar.radar_altitude = alt;
                break;
	  case 24:
                printf("ENTER THE EFFECTIVE UNAMBIGIOUS VELOCITY IN M/SEC\n");
                gets(input);
                vel = atof(input);
                aftsensordescriptor->radar.eff_unamb_vel = vel;
                break;
	  case 25:
                printf("ENTER THE EFFECTIVE UNAMBIGIOUS RANGE IN KM \n");
                gets(input);
                range = atof(input);
                aftsensordescriptor->radar.eff_unamb_range = range;
                break;
	 case 26:
                printf("ENTER THE NUMBER OF TRANSMITEED FREQUENCIES\n");
                gets(input);
                freq_trans  = atoi(input);
                aftsensordescriptor->radar.num_freq_trans = freq_trans;
                break;
	case 27:
                printf("ENTER THE NUMBER OF DIFFERENT INTER_PULSE PERIODS TRANSMITTED\n");
                gets(input);
                ipps_trans = atoi(input);
                aftsensordescriptor->radar.num_ipps_trans = ipps_trans;
                break;
        case 28:
                printf("ENTER FREQUENCY 1 \n");
                gets(input);
                freq1 = atof(input);
                aftsensordescriptor->radar.freq1 = freq1;
                break;
        case 29:
                printf("ENTER FREQUENCY 2 \n");
                gets(input);
                freq2 = atof(input);
                aftsensordescriptor->radar.freq2 = freq2;
                break;
        case 30:
                printf("ENTER FREQUENCY 3 \n");
                gets(input);
                freq3 = atof(input);
                aftsensordescriptor->radar.freq3 = freq3;
                break;
        case 31:
                printf("ENTER FREQUENCY 4 \n");
                gets(input);
                freq4 = atof(input);
                aftsensordescriptor->radar.freq4 = freq4;
                break;
        case 32:
                printf("ENTER FREQUENCY 5 \n");
                gets(input);
                freq5 = atof(input);
                aftsensordescriptor->radar.freq5 = freq5;
                break;
	case 33:
                printf("ENTER INTERPULSE PERIOD 1\n");
                gets(input);
                per1 = atof(input);
                aftsensordescriptor->radar.interpulse_per1 = per1;
                break;
	case 34:
                printf("ENTER INTERPULSE PERIOD 2\n");
                gets(input);
                per2 = atof(input);
                aftsensordescriptor->radar.interpulse_per2 = per2;
                break;
	case 35:
                printf("ENTER INTERPULSE PERIOD 3\n");
                gets(input);
                per3 = atof(input);
                aftsensordescriptor->radar.interpulse_per3 = per3;
                break;
	case 36:
                printf("ENTER INTERPULSE PERIOD 4\n");
                gets(input);
                per4 = atof(input);
                aftsensordescriptor->radar.interpulse_per4 = per4;
                break;
	case 37:
                printf("ENTER INTERPULSE PERIOD 5\n");
                gets(input);
                per5 = atof(input);
                aftsensordescriptor->radar.interpulse_per5 = per5;
                break;

          }/*switch*/
        }/*for*/
}/*end edit_radar_descriptor*/


                 
/**************************************************************************/
/*               SUBROUTINE PRINT RADAR DESCRIPTOR                        */
/**************************************************************************/

  print_aftradar_descriptor(aftsensordescriptor)
    struct sensorDes *aftsensordescriptor;
{


    printf("RADAR DESCRIPTOR     : %s \n",aftsensordescriptor->radar.radar_des);
    printf("RADAR DESCRIPTOR SIZE: %d\n",
                    aftsensordescriptor->radar.radar_des_length);
    printf("RADAR NAME           : %s\n",aftsensordescriptor->radar.radar_name);
    printf("RADAR CONSTANT       : %f\n",aftsensordescriptor->radar.radar_const);
    printf("PEAK POWER           : %f\n",aftsensordescriptor->radar.peak_power);
    printf("NOISE POWER          : %f\n",aftsensordescriptor->radar.noise_power);
    printf("RECIEVER GAIN        : %f\n",aftsensordescriptor->radar.receiver_gain);
    printf("ANTENNA GAIN         : %f\n",aftsensordescriptor->radar.antenna_gain);
    printf("SYSTEM GAIN          : %f\n",aftsensordescriptor->radar.system_gain);
    printf("HORIZONTAL BEAM WIDTH: %f\n",
                     aftsensordescriptor->radar.horz_beam_width);
    printf("VERTICAL BEAM WIDTH  : %f\n",
                     aftsensordescriptor->radar.vert_beam_width);
    printf("RADAR TYPE           : %d\n",aftsensordescriptor->radar.radar_type);
    printf("SCAN MODE            : %d\n",aftsensordescriptor->radar.scan_mode);
    printf("ROTATIONAL VELOCITY  : %f\n",aftsensordescriptor->radar.req_rotat_vel);
    printf("SCAN MODE PRAMATER #0: %f\n",
                      aftsensordescriptor->radar.scan_mode_pram0);
    printf("SCAN MODE PRAMATER #1: %f\n",
                      aftsensordescriptor->radar.scan_mode_pram1);
    printf("NUMBER OF PARRAMETER DESCRIPTOR BLOCKS :%d\n",
                                 aftsensordescriptor->radar.num_parameter_des);
    printf("TOTAL NUMER OF ADDITIONAL  DESCRIPTOR BLOCKS :%d\n",
                                 aftsensordescriptor->radar.total_num_des);
    printf("DATA COMPRESSION SCHEME               : %d\n",
                                 aftsensordescriptor->radar.data_compress);
    printf("DATA REDUCTION ALGORITHM IN USE        : %d\n",
                                 aftsensordescriptor->radar.data_reduction);
    printf("REDUCTION ALGORITHM SPECIFIC PARAMETER 0: %f\n",
                                 aftsensordescriptor->radar.data_red_parm0);
    printf("REDUCTION ALGORITHM SPECIFIC PARAMETER 1: %f\n",
                                 aftsensordescriptor->radar.data_red_parm1);
    printf("LATITUDE :%f\n",aftsensordescriptor->radar.radar_latitude);
    printf("LONGITUDE:%f\n",aftsensordescriptor->radar.radar_longitude);
    printf("ALTITUDE :%f\n",aftsensordescriptor->radar.radar_altitude);
    printf("EFFECTIVE UNAMBIGIOUS VELOCITY          : %f\n",
                                 aftsensordescriptor->radar.eff_unamb_vel);
    printf("EFFECTIVE UNAMBIGIOUS RANGE             : %f\n",
                                 aftsensordescriptor->radar.eff_unamb_range);
    printf("NUMBER OF TRANSMITEED FREQUENCIES       : %d\n",
                                 aftsensordescriptor->radar.num_freq_trans);
    printf("NUMBER OF DIFFERENT INTER-PULSE PERIODS : %d\n",
                                 aftsensordescriptor->radar.num_ipps_trans);
    printf("FREQUENCY 1          : %f\n",aftsensordescriptor->radar.freq1);
    printf("FREQUENCY 2          : %f\n",aftsensordescriptor->radar.freq2);
    printf("FREQUENCY 3          : %f\n",aftsensordescriptor->radar.freq3);
    printf("FREQUENCY 4          : %f\n",aftsensordescriptor->radar.freq4);
    printf("FREQUENCY 5          : %f\n",aftsensordescriptor->radar.freq5);
    printf("INTERPULSE PERIOD 1  : %f\n",aftsensordescriptor->radar.interpulse_per1);
    printf("INTERPULSE PERIOD 2  : %f\n",aftsensordescriptor->radar.interpulse_per2);
    printf("INTERPULSE PERIOD 3  : %f\n",aftsensordescriptor->radar.interpulse_per3);
    printf("INTERPULSE PERIOD 4  : %f\n",aftsensordescriptor->radar.interpulse_per4);
    printf("INTERPULSE PERIOD 5  : %f\n",aftsensordescriptor->radar.interpulse_per5);
}/*end print waveform descriptor*/

                 
/**************************************************************************/
/*               SUBROUTINE PRINT RADAR DESCRIPTOR TO PERINTER            */
/**************************************************************************/

  fprint_aftradar_descriptor(aftsensordescriptor)
    struct sensorDes *aftsensordescriptor;
{

    int i;
    int element;
    FILE *printer, *popen();

    if(( printer = popen("lpr -Pnec","w")) == NULL){
         printf("UNABLE TO ACCESS LASER PRINTER\n");
         return;
     }

    fprintf(printer,"RADAR DESCRIPTOR     : %s \n",
                   aftsensordescriptor->radar.radar_des);
    fprintf(printer,"RADAR DESCRIPTOR SIZE: %d\n",
                    aftsensordescriptor->radar.radar_des_length);
    fprintf("RADAR NAME           : ");
    for(i = 0; i < 8; i++)
        fprintf(printer,"%c",aftsensordescriptor->radar.radar_name[i]);
    fprintf(printer,"\nRADAR CONSTANT       : %f\n",
                     aftsensordescriptor->radar.radar_const);
    fprintf(printer,"\nPEAK POWER           : %f\n",
                     aftsensordescriptor->radar.peak_power);
    fprintf(printer,"NOISE POWER          : %f\n",
                     aftsensordescriptor->radar.noise_power);
    fprintf(printer,"RECIEVER GAIN        : %f\n",
                     aftsensordescriptor->radar.receiver_gain);
    fprintf(printer,"ANTENNA GAIN         : %f\n",
                     aftsensordescriptor->radar.antenna_gain);
    fprintf(printer,"SYSTEM GAIN          : %f\n",
                     aftsensordescriptor->radar.system_gain);
    fprintf(printer,"HORIZONTAL BEAM WIDTH: %f\n",
                     aftsensordescriptor->radar.horz_beam_width);
    fprintf(printer,"VERTICAL BEAM WIDTH  : %f\n",
                     aftsensordescriptor->radar.vert_beam_width);
    fprintf(printer,"RADAR TYPE           : %d\n",
                     aftsensordescriptor->radar.radar_type);
    fprintf(printer,"SCAN MODE            : %d\n",
                     aftsensordescriptor->radar.scan_mode);
    fprintf(printer,"ROTATIONAL VELOCITY  : %f\n",
                     aftsensordescriptor->radar.req_rotat_vel);
    fprintf(printer,"SCAN MODE PRAMATER #0: %f\n",
                      aftsensordescriptor->radar.scan_mode_pram0);
    fprintf(printer,"SCAN MODE PRAMATER #1: %f\n",
                      aftsensordescriptor->radar.scan_mode_pram1);
    fprintf(printer,"NUMBER OF PARRAMETER DESCRIPTOR BLOCKS :%d\n",
                                 aftsensordescriptor->radar.num_parameter_des);
    fprintf(printer,"TOTAL NUMER OF ADDITIONAL  DESCRIPTOR BLOCKS :%d\n",
                                 aftsensordescriptor->radar.total_num_des);
    fprintf(printer,"DATA COMPRESSION SCHEME               : %d\n",
                                 aftsensordescriptor->radar.data_compress);
    fprintf(printer,"DATA REDUCTION ALGORITHM IN USE        : %d\n",
                                 aftsensordescriptor->radar.data_reduction);
    fprintf(printer,"REDUCTION ALGORITHM SPECIFIC PARAMETER 0: %f\n",
                                 aftsensordescriptor->radar.data_red_parm0);
    fprintf(printer,"REDUCTION ALGORITHM SPECIFIC PARAMETER 1: %f\n",
                                 aftsensordescriptor->radar.data_red_parm1);
    fprintf(printer,"LATITUDE :%f\n",aftsensordescriptor->radar.radar_latitude);
    fprintf(printer,"LONGITUDE:%f\n",aftsensordescriptor->radar.radar_longitude);
    fprintf(printer,"ALTITUDE :%f\n",aftsensordescriptor->radar.radar_altitude);
    fprintf(printer,"EFFECTIVE UNAMBIGIOUS VELOCITY          : %f\n",
                                 aftsensordescriptor->radar.eff_unamb_vel);
    fprintf(printer,"EFFECTIVE UNAMBIGIOUS RANGE             : %f\n",
                                 aftsensordescriptor->radar.eff_unamb_range);
    fprintf(printer,"NUMBER OF TRANSMITEED FREQUENCIES       : %f\n",
                                 aftsensordescriptor->radar.num_freq_trans);
    fprintf(printer,"NUMBER OF DIFFERENT INTER-PULSE PERIODS : %f\n",
                                 aftsensordescriptor->radar.num_ipps_trans);
    fprintf(printer,"FREQUENCY 1          : %f\n",aftsensordescriptor->radar.freq1);
    fprintf(printer,"FREQUENCY 2          : %f\n",aftsensordescriptor->radar.freq2);
    fprintf(printer,"FREQUENCY 3          : %f\n",aftsensordescriptor->radar.freq3);
    fprintf(printer,"FREQUENCY 4          : %f\n",aftsensordescriptor->radar.freq4);
    fprintf(printer,"FREQUENCY 5          : %f\n",aftsensordescriptor->radar.freq5);
    fprintf(printer,"INTERPULSE PERIOD 1  : %f\n",aftsensordescriptor->radar.interpulse_per1);
    fprintf(printer,"INTERPULSE PERIOD 2  : %f\n",aftsensordescriptor->radar.interpulse_per2);
    fprintf(printer,"INTERPULSE PERIOD 3   : %f\n",aftsensordescriptor->radar.interpulse_per3);
    fprintf(printer,"INTERPULSE PERIOD 4  : %f\n",aftsensordescriptor->radar.interpulse_per4);
    fprintf(printer,"INTERPULSE PERIOD 5  : %f\n",aftsensordescriptor->radar.interpulse_per5);
    close(printer);
}/*end print waveform descriptor to printer*/

