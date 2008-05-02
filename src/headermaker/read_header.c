#include <stdio.h>
#define scope extern
#include "mkrDef.h"
#include "mkrGbl.h"
#include "mkrFunc.h"

void read_header()
 {
  
     int bytesread, bytestoread, i, version;
     FILE *filepntr;

     /*open inupt file*/

     printf("NAME OF INPUT FILE?\n");
     gets(inname);
     if((filepntr = fopen(inname,"rb")) == NULL){
        printf("UNABLE TO OPEN INPUT FILE: %s\n",inname);
        return;
     }
     printf("INPUT FILE %s IS OPEN\n",inname);

     /*START READING IN THE DATA FROM THE FILE*/
     
     bytestoread = 10; /* read in first 10 bytes of Volume */
     bytesread = fread(&volume,bytestoread,1,filepntr);
     unpk_short(&volume.format_version,2,0);
     version = volume.format_version;
     rewind(filepntr);  /* go back to beginning of file */
     bytestoread = sizeof(volume);
     bytesread = fread(&volume,bytestoread,1,filepntr);
     if(bytesread <= 0){
        printf("EOF or bad read on volume descriptor\n");
       return;
     }
     bytestoread = sizeof(waveform);
     bytesread = fread(&waveform,bytestoread,1,filepntr);
     if(bytesread <= 0){
        printf("EOF or bad read on Waveform descriptor\n");
       return;
     }

/*** read the fore radar information ***/

     bytestoread = sizeof(struct radar_d);
     bytesread = fread(&radar[FORE],bytestoread,1,filepntr);
     if(bytesread <= 0){
        printf("EOF or bad read on fore radar descriptor\n");
       return;
     }
     if(version == 2)      /* New Format with header filename */
       bytestoread = sizeof(struct field_radar_i);
     else if(version == 1) /* Previous Format */
       bytestoread = sizeof(struct field_radar_i) - 80*sizeof(char);
     else
       printf("Unknown Format Version!!\n");
     bytesread = fread(&fradar[FORE],bytestoread,1,filepntr);
     if(bytesread <= 0){
        printf("EOF or bad read on fore field radar info\n");
       return;
     }
     bytestoread = sizeof(struct cell_spacing_d);
     bytesread = fread(&cell[FORE],bytestoread,1,filepntr);
     if(bytesread <= 0){
        printf("EOF or bad read on fore cell spacing descriptor\n");
       return;
     }

     actual_num_radars = 2;
     unpk_short(&radar[FORE].num_parameter_des,1,0);
     actual_num_params[FORE] = radar[FORE].num_parameter_des;
     printf("Fore radar contains %d parameters\n",actual_num_params[FORE]);
     bytestoread = sizeof(struct parameter_d);

     for(i=0; i<actual_num_params[FORE]; i++)
       {
	   bytesread = fread(&parameter[i][FORE],bytestoread,1,filepntr);
	   if(bytesread <= 0){
	       printf("EOF or bad read on fore parameter number: %d\n",i);
	       return;
	   }
       }

/*** read the aft radar information ***/

     bytestoread = sizeof(struct radar_d);
     bytesread = fread(&radar[AFT],bytestoread,1,filepntr);
     if(bytesread <= 0){
        printf("EOF or bad read on AFT radar descriptor\n");
       return;
     }
     if(version == 2)      /* New Format with header filename */
       bytestoread = sizeof(struct field_radar_i);
     else if(version == 1) /* Previous Format */
       bytestoread = sizeof(struct field_radar_i) - 80*sizeof(char);
     else
       printf("Unknown Format Version!!\n");
     bytesread = fread(&fradar[AFT],bytestoread,1,filepntr);
     if(bytesread <= 0){
        printf("EOF or bad read on AFT field radar info\n");
       return;
     }
     bytestoread = sizeof(struct cell_spacing_d);
     bytesread = fread(&cell[AFT],bytestoread,1,filepntr);
     if(bytesread <= 0){
        printf("EOF or bad read on AFT cell spacing descriptor\n");
       return;
     }

     unpk_short(&radar[AFT].num_parameter_des,1,0);
     actual_num_params[AFT] = radar[AFT].num_parameter_des;
     printf("AFT radar contains %d parameters\n",actual_num_params[AFT]);
     bytestoread = sizeof(struct parameter_d);

     for(i=0; i<actual_num_params[AFT]; i++)
       {
	   bytesread = fread(&parameter[i][AFT],bytestoread,1,filepntr);
	   if(bytesread <= 0){
	       printf("EOF or bad read on AFT parameter number: %d\n",i);
	       return;
	   }
       }

/*** read the navigational and insitu data descriptors ***/

     bytestoread = sizeof(struct nav_descript);
     bytesread = fread(&navigation,bytestoread,1,filepntr);
     if(bytesread <= 0){
        printf("EOF or bad read on navigational data descriptor\n");
       return;
     }
     bytestoread = sizeof(struct insitu_descript);
     bytesread = fread(&insitudata,bytestoread,1,filepntr);
     if(bytesread <= 0){
        printf("EOF or bad read on insitu data descriptor\n");
       return;
     }

     fclose(filepntr);
 }




