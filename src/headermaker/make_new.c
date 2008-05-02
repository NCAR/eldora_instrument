/***** Makes a new header up from scratch ******/

#define scope extern
#include <stdlib.h>
#include "mkrDef.h"
#include "mkrGbl.h"
#include "mkrFunc.h"

float get_input();

void make_new()
{

 char input[80];
 int i, j, num_calculate_left, old_num, good;

 initialize_volume(&volume);
 initialize_waveform(&waveform);
 actual_num_radars = 2;
 for(i=0; i<actual_num_radars; i++)
   {
       initialize_radar(&radar[i],i);
       initialize_cellspacing(&cell[i]);
       initialize_fieldradar(&fradar[i]);
       for(j=0; j<MAX_PARAM; j++)
	 initialize_parameter(&parameter[j][i]);
   }
initialize_navigation(&navigation);
initialize_insitudata(&insitudata);
initialize_engin();
initialize_waveform_state();
initialize_fieldradar_state();
initialize_radar_state();
initialize_cellspacing_state();
initialize_parameter_state();
initialize_engin_state();
 printf("\n");
 printf("Enter the Header file name: ");
 gets(input);
 for(i=0; i<80; i++)
   {
     fradar[FORE].file_name[i] = input[i];
     fradar[AFT].file_name[i] = input[i];
   }
 /*
 cleanup(&fradar[FORE].file_name,80);
 cleanup(&fradar[AFT].file_name,80);
 */
 printf("\n");

 printf("Select a pulsing scheme name: ");
 for(i=0; i<actual_num_pschemes; i++)
   {
       printf("\n%2d) ",i+1);
       for (j=0; j<16; j++)
	 printf("%c",pscheme[i].name[j]);
   }

 engin_win.schemenum = get_input(0.,(float)(actual_num_pschemes+1))-1;
 engin_st.schemenum = 2;

 printf("ENTER NUMBER OF SAMPLES (PER FREQUENCY) IN A DWELL TIME\n");
 printf("IN DUAL PRT MODE THIS IS THE TOTAL NUMBER FOR COMBINED PARAMETERS\n");
 printf("\n");
 engin_win.numsamples = get_input(2.,600.);
 engin_st.numsamples = 2;

 printf("ENTER PULSE REPETITION FREQUENCY (in Hertz)\n");
 printf("IN DUAL PRT MODE THIS IS THE LOWEST FREQUENCY\n");
 engin_win.prf = get_input(500.,5000.);
 engin_st.prf = 2;

 do{
     printf("ENTER NUMBER OF GATES\n");
     engin_win.numgates = get_input(16.,1024.);
     if((engin_win.numgates % 8) != 0) printf("Must be divisible by 8 \n");
 }while((engin_win.numgates % 8) != 0);
 engin_st.numgates = 2;

 good = 0;
 do{
     printf("ENTER THE CHIP WIDTH (in us)\n");
     engin_win.chipdur = get_input(0.24,4.1);
     if(engin_win.chipdur == 0.25) good = 1;
     if(engin_win.chipdur == 0.5) good = 1;
     if(engin_win.chipdur == 0.75) good = 1;
     if(engin_win.chipdur == 1.0) good = 1;
     if(engin_win.chipdur == 1.25) good = 1;
     if(engin_win.chipdur == 1.5) good = 1;
     if(engin_win.chipdur == 1.75) good = 1;
     if(engin_win.chipdur == 2.00) good = 1;
     if(engin_win.chipdur == 2.25) good = 1;
     if(engin_win.chipdur == 2.5) good = 1;
     if(engin_win.chipdur == 2.75) good = 1;
     if(engin_win.chipdur == 3.00) good = 1;
     if(engin_win.chipdur == 3.25) good = 1;
     if(engin_win.chipdur == 3.5) good = 1;
     if(engin_win.chipdur == 3.75) good = 1;
     if(engin_win.chipdur == 4.00) good = 1;
     if(!good) printf("Must be multiple of 0.25 us\n");
 }while(!good);
     engin_st.chipdur = 2;

 printf("ENTER THE DISTANCE TO THE FIRST GATE (in meters)\n");
 engin_win.first_gate_dis = get_input(-1.0,1001.0);
 engin_st.first_gate_dis = 2;

printf("ENTER THE ROTATIONAL SPACING OF THE DATA BEAMS (in degrees)\n");
 engin_win.rot_spacing = get_input(0.5,5.0);
 engin_st.rot_spacing = 2;

 printf("Select the fore HPA serial number: ");
 for(i=0; i<actual_num_hpas; i++)
   {
       printf("\n%2d) ",i+1);
       for (j=0; j<8; j++)
	 printf("%c",hpa[i].serial_number[j]);
   }
 engin_win.forehpa_snum = get_input(0.,(float)(actual_num_hpas+1))-1;
 engin_st.forehpa_snum = 2;

 printf("Select the aft HPA serial number: ");
 for(i=0; i<actual_num_hpas; i++)
   {
       printf("\n%2d) ",i+1);
       for (j=0; j<8; j++)
	 printf("%c",hpa[i].serial_number[j]);
   }
 engin_win.afthpa_snum = get_input(0.,(float)(actual_num_hpas+1))-1; 
 engin_st.afthpa_snum = 2;

/* Now calculate the rest of the header parameters */

do{

    old_num = num_calculate_left;
    num_calculate_left = 0;
    num_calculate_left += calculate_volume();
    num_calculate_left += calculate_waveform();
printf("Waveform Number to calculate left = %d\n",num_calculate_left);
    num_calculate_left += calculate_radar();
printf("Radar Number to calculate left = %d\n",num_calculate_left);
    num_calculate_left += calculate_engin();
printf("Engin Number to calculate left = %d\n",num_calculate_left);
    num_calculate_left += calculate_parameter();
printf("Parameter Number to calculate left = %d\n",num_calculate_left);
    num_calculate_left += calculate_fieldradar();
printf("FieldRadar Number to calculate left = %d\n",num_calculate_left);
    num_calculate_left += calculate_cellspacing();
printf("Cellspacing Number to calculate left = %d\n",num_calculate_left);

}while(num_calculate_left != old_num);

} /* End of the make new function */

/* Function to get input and check within bounds */

float get_input(min,max)
  float min, max;
{
char input[80];
float afloat;
for(;;)
  {
      gets(input);
      afloat = atof(input);
      printf("%7.2f\n",afloat);
      if( afloat > min && afloat < max ) return(afloat);
      printf("Value must be > %7.2f and < %7.2f Please reenter\n",min,max);
  }
}
