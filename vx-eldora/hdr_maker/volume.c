#include <stdio.h>
#include <time.h>
#include "field.h"




/*********************************************************************
                SUBROUTINE EDIT VOLUME DESCRIPTOR
**********************************************************************/

  edit_volume_descriptor(vheader)
    struct volume_header *vheader;
{

    int size,choice;
    char string[80];
    char input[80];
    short format,volume_number;
    int maxchars;
    int year,month,day;
    int dhour,dmin,dsec;
    int genyear,genmonth,genday;
    int max_bytes;
    int sensors;


    maxchars = 8;
  
     for(;;){  
     printf("VOLUME HEADER MENU\n");
     do{
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
       gets(input);
       choice = atoi(input);
       if((choice < 0) || (choice >10)) printf("INPUT ERROR PLEASE REENTER\n");
   } while((choice <0) || (choice > 10));
       

       switch(choice){
          case 0:
               return;
               break;
	 case 1: 
               printf("ENTER FORMAT VERSION:\n");
               gets(input);
               format = atoi(input);
               vheader->volume.format_version = format;
               break;
         case 2: 
               printf("ENTER VOLUME NUMBER:\n");
               gets(input);
               volume_number = atoi(input);
               vheader->volume.volume_num = volume_number;
               break;
	 case 3:
               printf("ENTER BYTES:\n");
               gets(input);
               max_bytes = atoi(input);
               vheader->volume.maximum_bytes = max_bytes;
               break;
	 case 4:
              printf("ENTER THE PROJECT NAME\n");
              do{
                  gets(string);
                  if(strlen(string) > 20) printf("STRING TOO LONG\n");
              } while(strlen(string) > 20);
              strcpy(vheader->volume.proj_name,string);
              break;
	 case 5:
              printf("YEAR ?\n");
              gets(input);
              year = atoi(input);
              vheader->volume.year = year;
              printf("MONTH ?\n");
              gets(input);
              month = atoi(input);
              vheader->volume.month = month;
              printf("DAY   ?\n");
              gets(input);
              day = atoi(input);
              vheader->volume.day = day;
              break;
	 case 6:
              printf("HOUR ?\n");
              gets(input);
              dhour = atoi(input);
              vheader->volume.data_set_hour = dhour;
              printf("MINUTE ?\n");
              gets(input);
              dmin = atoi(input);
              vheader->volume.data_set_minute = dmin;
              printf("SECOND ?\n");
              gets(input);
              dsec = atoi(input);
              vheader->volume.data_set_second = dsec;
              break;
	 case 7:
              printf("PLEASE ENTER THE FLIGHT NUMBER (8 ASCII)\n");
              do{
                  gets(string);
                  if(strlen(string) > maxchars) printf("STRING TOO LONG\n");
              } while(strlen(string) > maxchars);
              strcpy(vheader->volume.flight_num,string);
              break;
	  case 8:
              do{
                 printf("ENTER THE GENERATION FACILITY (8 ASCII)\n");
                 gets(string);
                 if(strlen(string) > 8) printf("STRING TOO LONG\n");
	      } while(strlen(string) > 8);
              strcpy(vheader->volume.gen_facility,string);
              break;
	  case 9:
              printf("GENERATION YEAR ?\n");
              gets(input);
              genyear = atoi(input);
              vheader->volume.gen_year = genyear;
              printf("GENERATION MONTH ?\n");
              gets(input);
              genmonth = atoi(input);
              vheader->volume.gen_month = genmonth;
              printf("GENERATION DAY   ?\n");
              gets(input);
              genday = atoi(input);
              vheader->volume.gen_day = genday;
              break;
	  case 10:
              printf("ENTER THE NUMBER OF SENSOR DESCRIPTORS\n");
              gets(input);
              sensors = atoi(input);
              vheader->volume.number_sensor_des = sensors;
              break;
         default:
             break;
       }

    }/*for*/
}/*end edit_volume_descriptor*/
  



/***********************************************************************/
/*               SUBROUTINE INITIALIZE VOLUME DESCRIPTOR                    */
/**********************************************************************/
   initialize_volume_descriptor(vheader)
   
    struct volume_header *vheader;

{
    int size,i;
    short  year,month,day,hour,min,sec;
    long   clock;
    struct tm *current_time;
     
    clock = time(NULL);
    current_time = localtime(&clock);    

/*INITIALIZE VOLUME DESCRIPTOR*/  
         vheader->volume.volume_des[0] = 'V';
         vheader->volume.volume_des[1] = 'O';
         vheader->volume.volume_des[2] = 'L';
         vheader->volume.volume_des[3] = 'D';  
         size = sizeof(vheader->volume);
         vheader->volume.volume_des_length = size;
         vheader->volume.format_version  = -999;
         vheader->volume.volume_num  = -999;
         vheader->volume.maximum_bytes   = -999;
         for(i = 0; i < 20; i++){
            vheader->volume.proj_name[i] = ' ';
	 }
         vheader->volume.year  = current_time->tm_year;;
         vheader->volume.month = current_time->tm_mon;
         vheader->volume.day   = current_time->tm_mday;

         vheader->volume.data_set_hour  = current_time->tm_hour;
         vheader->volume.data_set_minute   = current_time->tm_min;
         vheader->volume.data_set_second   = current_time->tm_sec;

         vheader->volume.flight_num[0] = ' ';
         vheader->volume.flight_num[1] = ' ';
         vheader->volume.flight_num[2] = ' ';
         vheader->volume.flight_num[3] = ' ';
         vheader->volume.flight_num[4] = ' ';
         vheader->volume.flight_num[5] = ' ';
         vheader->volume.flight_num[6] = ' ';
         vheader->volume.flight_num[7] = ' ';

         vheader->volume.gen_facility[0] = ' ';
         vheader->volume.gen_facility[1] = ' ';
         vheader->volume.gen_facility[2] = ' ';
         vheader->volume.gen_facility[3] = ' ';
         vheader->volume.gen_facility[4] = ' ';
         vheader->volume.gen_facility[5] = ' ';
         vheader->volume.gen_facility[6] = ' ';
         vheader->volume.gen_facility[7] = ' ';

         vheader->volume.gen_year = current_time->tm_year;
         vheader->volume.gen_month = current_time->tm_mon;
         vheader->volume.gen_day   = current_time->tm_mday;

         vheader->volume.number_sensor_des = 0;
}/*end initialize_volume_descriptor*/


/**************************************************************************/
/*             SUBROUTINE PRINT VOLUME DESCRIPTOR                            */
/**************************************************************************/
   print_volume_descriptor(vheader)
      struct volume_header *vheader;

    {
     int i;
    
    printf("***************VOLUME DESCRIPTOR***************************\n");
    printf("CODE IDENTIFIER          : %s\n",vheader->volume.volume_des);
    printf("VOLUME DESCRIPTOR LENGTH : %d\n",vheader->volume.volume_des_length);
    printf("FORMAT VERSION NUMBER IS :%d\n",vheader->volume.format_version);
    printf("VOLUME NUMBER IS         :%d\n",vheader->volume.volume_num);
    printf("MAXIMUM NUMBER OF BYTES IN DATA RECORD IS  : %d\n",vheader->volume.
           maximum_bytes);
    printf("PROJECT NAME             : ");
    for(i = 0; i < 20; i++) printf("%c",vheader->volume.proj_name[i]);
    printf("\nDATE  %d / %d / %d\n",vheader->volume.year,vheader->volume.month,
                        vheader->volume.day);
    printf("TIME  %d  : %d : %d\n",vheader->volume.data_set_hour,
                        vheader->volume.data_set_minute,
                        vheader->volume.data_set_second);
    printf("FLIGHT NUMBER            : ");
    for(i = 0; i < 8; i++) printf("%c",vheader->volume.flight_num[i]);
    printf("\nGENERATION FACILITY NAME : ");
    for(i = 0; i < 8; i++) printf("%c",vheader->volume.gen_facility[i]);
    printf("\nVOLUME GENERATION DATE   : %d / %d / %d\n",
           vheader->volume.gen_year,vheader->volume.gen_month,
           vheader->volume.gen_day);
    printf("NUMBER OF SENSOR DESCRIPTORS                : %d\n",
            vheader->volume.number_sensor_des);
    }/*end print_volume_descriptor*/



/**************************************************************************/
/*             SUBROUTINE PRINT VOLUME DESCRIPTOR TO PRINTER              */
/**************************************************************************/
   fprint_volume_descriptor(vheader)
      struct volume_header *vheader;

    {
     int i;
     FILE *printer, *popen();
 
    
    if((printer = popen("lpr -Pnec","w")) == NULL){
       printf("UNABLE TO ACCESS LASER PRINTER\n");
       return;
    }     					       
    fprintf(printer,"************VOLUME DESCRIPTOR************************\n");
    fprintf(printer,"CODE IDENTIFIER          : %s\n",
                    vheader->volume.volume_des);
    fprintf(printer,"VOLUME DESCRIPTOR LENGTH : %d\n",
                    vheader->volume.volume_des_length);
    fprintf(printer,"FORMAT VERSION NUMBER IS :%d\n",
                    vheader->volume.format_version);
    fprintf(printer,"VOLUME NUMBER IS         :%d\n",
                    vheader->volume.volume_num);
    fprintf(printer,"MAXIMUM NUMBER OF BYTES IN DATA RECORD IS  : %d\n",
                    vheader->volume.
           maximum_bytes);
    fprintf(printer,"PROJECT NAME             : ");
    for(i = 0; i < 20; i++) fprintf(printer,"%c",vheader->volume.proj_name[i]);
    fprintf(printer,"\nDATE  %d / %d / %d\n",
               vheader->volume.year,vheader->volume.month,vheader->volume.day);
    fprintf(printer,"TIME  %d  : %d : %d\n",vheader->volume.data_set_hour,
                        vheader->volume.data_set_minute,
                        vheader->volume.data_set_second);
    fprintf(printer,"FLIGHT NUMBER            : ");
    for(i = 0; i < 8; i++) fprintf(printer,"%c",vheader->volume.flight_num[i]);
    fprintf(printer,"\nGENERATION FACILITY NAME : ");
    for(i = 0; i < 8; i++) 
          fprintf(printer,"%c",vheader->volume.gen_facility[i]);
    fprintf(printer,"\nVOLUME GENERATION DATE   : %d / %d / %d\n",
           vheader->volume.gen_year,vheader->volume.gen_month,
           vheader->volume.gen_day);
    fprintf(printer,"NUMBER OF SENSOR DESCRIPTORS                : %d\n",
            vheader->volume.number_sensor_des);
    close(printer);
    }/*end print_volume_descriptor*/








