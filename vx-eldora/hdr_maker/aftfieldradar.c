#include <stdio.h>
#include "field.h"
#include <floatingpoint.h>

/*************************************************************************/
/*              SUBROUTINE INITIALIZE FIELD RADAR INFO                   */
/*************************************************************************/

 initialize_aftfieldradar_info(aftsensordescriptor)
    struct sensorDes  *aftsensordescriptor;
{
   int i,size;

   aftsensordescriptor->fradar.field_radar_info[0] = 'F';
   aftsensordescriptor->fradar.field_radar_info[1] = 'R';
   aftsensordescriptor->fradar.field_radar_info[2] = 'I';
   aftsensordescriptor->fradar.field_radar_info[3] = 'B';
   size = sizeof(aftsensordescriptor->fradar);
   aftsensordescriptor->fradar.field_radar_info_len = size;
   aftsensordescriptor->fradar.data_sys_id = -999;
   aftsensordescriptor->fradar.signal_source = -999;
   aftsensordescriptor->fradar.loss_out = -999.;
   aftsensordescriptor->fradar.loss_in = -999.;
   aftsensordescriptor->fradar.ant_loss = -999.;
   aftsensordescriptor->fradar.sys_loss_0 = -999.;
   aftsensordescriptor->fradar.sys_loss_1 = -999.;
   aftsensordescriptor->fradar.sys_loss_2 =-999.;
   aftsensordescriptor->fradar.sys_loss_3 = -999.;
   aftsensordescriptor->fradar.ant_v_dim = -999.;
   aftsensordescriptor->fradar.ant_h_dim = -999.;
   aftsensordescriptor->fradar.aperture_eff = -999.;
   for(i = 0; i < 5; i++)
      aftsensordescriptor->fradar.filter_num[i] = 2;
   aftsensordescriptor->fradar.bessel_correct = -999.;
   aftsensordescriptor->fradar.ant_noise_temp = -999.;
   aftsensordescriptor->fradar.r_noise_figure = -999.;
   aftsensordescriptor->fradar.dly_tube_antenna = -999;
   aftsensordescriptor->fradar.dly_antenna_dircplr = -999;
   for(i = 0; i < 5; i++)
       aftsensordescriptor->fradar.dly_dircplr_ad[i] = -999;
   aftsensordescriptor->fradar.dly_timmod_testpulse = -999;
   aftsensordescriptor->fradar.dly_modulator_on = -999;
   aftsensordescriptor->fradar.dly_modulator_off = -999;
   aftsensordescriptor->fradar.dly_rf_twt_on = -999;
   aftsensordescriptor->fradar.dly_rf_twt_off = -999;
}

  edit_aftfield_radar_info(aftsensordescriptor)
        struct sensorDes  *aftsensordescriptor;

{
    char input[80];
    int choice, i, filternum;
    int entry;
    float dim,eff,bessel;


     for(;;){
     printf("FIELD RADAR INFO MENU\n");
     do{
        printf(" 0.   TO RETURN TO THE MAIN MENU\n");
        printf(" 1.   INPUT DATA SYSTEM IDENTIFICATION\n");
        printf(" 2.   INPUT SIGNAL SOURCE\n");
        printf(" 3.   INPUT ANTENNA VERTICAL DIMENSION\n");
        printf(" 4.   INPUT ANTENNA HORIZONTAL DIMENSION\n");
        printf(" 5.   INPUT APERTURE EFFICIENCY\n");
        printf(" 6.   INPUT IF SIGNAL PROCESSOR FILTER NUMBER\n");
        printf(" 7.   INPUT BESSEL FILTER CORRECTION FACTOR\n");
        printf(" 8.   INPUT THE INDEPF_TIMES_FLAG\n");
        printf(" 9.   INPUT THE INDEP_FREQ_GATE\n");
        printf(" 10.  INPUT THE TIME_SERIES_GATE\n");
        gets(input);
        choice = atoi(input);
        if((choice < 0) || (choice >10)) printf("ERROR PLEASE REENTER\n");
     } while((choice <0) || (choice > 10));
     

       switch(choice){
          case 0:
               return;
               break;
	  case 1:
               printf("ENTER SYSTEM ID\n");
               gets(input);
               entry = atoi(input);
               aftsensordescriptor->fradar.data_sys_id = entry;
               break;
	  case 2:
               printf("ENTER SIGNAL SOURCE\n");
               gets(input);
               entry = atoi(input);
               aftsensordescriptor->fradar.signal_source = entry;    
               break;
	  case 3:
               printf("ENTER DIMENSION\n");
               gets(input);
               dim = atof(input);
               aftsensordescriptor->fradar.ant_v_dim = dim;
               break;
	  case 4:
               printf("ENTER DIMENSION\n");
               gets(input);
               dim = atof(input);
               aftsensordescriptor->fradar.ant_h_dim = dim;
               break;
	  case 5:
               printf("ENTER EFFICIENCY\n");
               gets(input);
               eff = atof(input);
               aftsensordescriptor->fradar.aperture_eff = eff;
               break; 
         case 6:
	       for(i=0; i<5; i++)
		 {
		     do{
			 printf("ENTER FILTER NUMBER FOR FREQUENCY %d\n",i+1);
			 gets(input);
			 filternum = atoi(input);
			 if((filternum < 1) || (filternum >5))
			   printf("ERROR PLEASE REENTER RANGE: 1-5\n");
		     } while((filternum <1) || (filternum > 5));
		     aftsensordescriptor->fradar.filter_num[i] = filternum;
		 }
                 break;
  	 case 7:
               printf("ENTER CORRECTION FACTOR\n");
               gets(input);
               bessel = atof(input);
               aftsensordescriptor->fradar.bessel_correct = bessel;
               break;
         case 8:
               printf("ENTER FLAG:\n");
               gets(input);
               entry = atoi(input);
               aftsensordescriptor->fradar.indepf_times_flg = entry;
               break;
         case 9:
               printf("ENTER FREQUENCY GATE:\n");
               gets(input);
               entry = atoi(input);
               aftsensordescriptor->fradar.indep_freq_gate = entry;
               break;
         case 10:
               printf("ENTER TIME SERIES GATE:\n");
               gets(input);
               entry = atoi(input);
               aftsensordescriptor->fradar.times_series_gate = entry;
               break;
	 default:
              break;
	}/*switch*/
     }/*for*/
 }
   
/*****************************************************************************/
/*         SUBROUTINE PRINT FIELD RADAR INFO                                 */
/*****************************************************************************/

   print_aftfield_radar_descriptor(aftsensordescriptor)
      struct sensorDes  *aftsensordescriptor;

{
   int i;

   
   printf("FIELD RADAR IDENTIFIER : %s\n",
                         aftsensordescriptor->fradar.field_radar_info);
   printf("FIELD RADAR LENGTH     : %d\n",
                         aftsensordescriptor->fradar.field_radar_info_len);
   printf("DATA SYSTEM ID         :%d\n",
                          aftsensordescriptor->fradar.data_sys_id);
   printf("SIGNAL SOURCE          :%d\n",
                         aftsensordescriptor->fradar.signal_source);
   printf("LOSSES BETWEEN TRANSMITTER AND ANTENNA   :%f\n",
                          aftsensordescriptor->fradar.loss_out);
   printf("LOSSES BETWEEN ANTENNA AND LOW NOISE AMP :%f\n",
                           aftsensordescriptor->fradar.loss_in);
   printf("LOSSES IN ANTENNA                        :%f\n",
                           aftsensordescriptor->fradar.ant_loss);
   printf("SYSTEM LOSS 0                            :%f\n",
                           aftsensordescriptor->fradar.sys_loss_0);
   printf("SYSTEM LOSS 1                            :%f\n",
                           aftsensordescriptor->fradar.sys_loss_1);
   printf("SYSTEM LOSS 2                            :%f\n",
                           aftsensordescriptor->fradar.sys_loss_2);
   printf("SYSTEM LOSS 3                            :%f\n",
                           aftsensordescriptor->fradar.sys_loss_3);
   printf("ANTENNA VERTICAL DIMENSION              :%f\n",
                           aftsensordescriptor->fradar.ant_v_dim);
   printf("ANTENNA HORIZONTAL DIMENSION            :%f\n",
                           aftsensordescriptor->fradar.ant_h_dim);
   printf("APERATURE EFFICIENCY                    :%f\n",
                           aftsensordescriptor->fradar.aperture_eff);
   printf("IF SIGNAL PROCESSOR FILTER NUMBER USED  : ");
   for(i = 0; i < 5; i++)
      printf("%d = %d  ",i+1,aftsensordescriptor->fradar.filter_num[i]);
   printf("\n");
   printf("BESSEL FILTER CORRECTION FACTOR         :%f\n",
                         aftsensordescriptor->fradar.bessel_correct);
   printf("ANTENNA NOISE TEMPERATURE               :%f\n",
                          aftsensordescriptor->fradar.ant_noise_temp);
   printf("RECEIVER NOISE FIGURE                   :%f\n",
                          aftsensordescriptor->fradar.r_noise_figure);
   printf("TIME DELAY TUBE ANTENNA        :%d\n",
                          aftsensordescriptor->fradar.dly_tube_antenna);
   printf("TIME DELAY DIRECTIONAL COUPLER :%d\n",
                          aftsensordescriptor->fradar.dly_antenna_dircplr);
   printf("TIME DELAY A/D CONVERTERS      :");
   for(i = 0; i < 5; i++)
       printf("%d ",aftsensordescriptor->fradar.dly_dircplr_ad[i]);
   printf("\n");
   printf("TIME DELAY TEST PULSE          :%d\n",
                         aftsensordescriptor->fradar.dly_timmod_testpulse);
   printf("MODULATOR RISE TIME            :%d\n",
                         aftsensordescriptor->fradar.dly_modulator_on);
   printf("MODULATOR FALL TIME            :%d\n",
                         aftsensordescriptor->fradar.dly_modulator_off);
   printf("CHIP ON EDGE                   :%d\n",
                         aftsensordescriptor->fradar.dly_rf_twt_on);
   printf("CHIP OFF EDGE                  :%d\n",
                         aftsensordescriptor->fradar.dly_rf_twt_off);
   printf("INDEPDENT FREQUENCY TIMES FLAG :%d\n",
                         aftsensordescriptor->fradar.indepf_times_flg);
   printf("INDEPENDENT FREQUENCY GATE     : %d\n",
                         aftsensordescriptor->fradar.indep_freq_gate);
   printf("TIME SERIES GATE               : %d\n",
                         aftsensordescriptor->fradar.times_series_gate);
}

/*****************************************************************************/
/*         SUBROUTINE PRINT FIELD RADAR INFO TO PRINTER                      */
/*****************************************************************************/

   fprint_aftfield_radar_descriptor(aftsensordescriptor)
      struct sensorDes  *aftsensordescriptor;
{
   int i;
   FILE *printer, *popen();

   if(( printer = popen("lpr -Pnec","w")) == NULL){
         printf("UNABLE TO ACCESS LASER PRINTER\n");
         return;
   }
     
   fprintf("FIELD RADAR IDENTIFIER : %s\n",
                         aftsensordescriptor->fradar.field_radar_info);
   fprintf("FIELD RADAR LENGTH     : %d\n",
                         aftsensordescriptor->fradar.field_radar_info_len);
   fprintf("DATA SYSTEM ID         : %d\n",
                          aftsensordescriptor->fradar.data_sys_id);
   fprintf("SIGNAL SOURCE          : %d\n",
                         aftsensordescriptor->fradar.signal_source);
   fprintf("LOSSES BETWEEN TRANSMITTER AND ANTENNA   :%f\n",
                          aftsensordescriptor->fradar.loss_out);
   fprintf("LOSSES BETWEEN ANTENNA AND LOW NOISE AMP :%f\n",
                           aftsensordescriptor->fradar.loss_in);
   fprintf("LOSSES IN ANTENNA                        :%f\n",
                           aftsensordescriptor->fradar.ant_loss);
   fprintf("SYSTEM LOSS 0                            :%f\n",
                           aftsensordescriptor->fradar.sys_loss_0);
   fprintf("SYSTEM LOSS 1                            :%f\n",
                           aftsensordescriptor->fradar.sys_loss_1);
   fprintf("SYSTEM LOSS 2                            :%f\n",
                           aftsensordescriptor->fradar.sys_loss_2);
   fprintf("SYSTEM LOSS 3                            :%f\n",
                           aftsensordescriptor->fradar.sys_loss_3);
   fprintf("ANTENNA VERTICAL DIMENSION              :%f\n",
                           aftsensordescriptor->fradar.ant_v_dim);
   fprintf("ANTENNA HORIZONTAL DIMENSION            :%f\n",
                           aftsensordescriptor->fradar.ant_h_dim);
   fprintf("APERATURE EFFICIENCY                    :%f\n",
                           aftsensordescriptor->fradar.aperture_eff);
   fprintf("IF SIGNAL PROCESSOR FILTER NUMBER USED  : ");
   for(i = 0; i < 5; i++)
      fprintf("%d = %d  ",i+1,aftsensordescriptor->fradar.filter_num[i]);
   fprintf("\n");
   fprintf("BESSEL FILTER CORRECTION FACTOR         :%f\n",
                         aftsensordescriptor->fradar.bessel_correct);
   fprintf("ANTENNA NOISE TEMPERATURE               :%f\n",
                          aftsensordescriptor->fradar.ant_noise_temp);
   fprintf("RECEIVER NOISE FIGURE                   :%f\n",
                          aftsensordescriptor->fradar.r_noise_figure);
   fprintf("TIME DELAY TUBE ANTENNA        :%d\n",
                          aftsensordescriptor->fradar.dly_tube_antenna);
   fprintf("TIME DELAY DIRECTIONAL COUPLER :%d\n",
                          aftsensordescriptor->fradar.dly_antenna_dircplr);
   fprintf("TIME DELAY A/D CONVERTERS      :");
   for(i = 0; i < 5; i++)
       fprintf(" %d",aftsensordescriptor->fradar.dly_dircplr_ad[i]);
   fprintf("\n");
   fprintf("TIME DELAY TEST PULSE          :d\n",
                         aftsensordescriptor->fradar.dly_timmod_testpulse);
   fprintf("MODULATOR RISE TIME            :%d\n",
                         aftsensordescriptor->fradar.dly_modulator_on);
   fprintf("MODULATOR FALL TIME            :%d\n",
                         aftsensordescriptor->fradar.dly_modulator_off);
   fprintf("CHIP ON EDGE                   :%d\n",
                         aftsensordescriptor->fradar.dly_rf_twt_on);
   fprintf("CHIP OFF EDGE                  :%d\n",
                         aftsensordescriptor->fradar.dly_rf_twt_off);
   fprintf("INDEPDENT FREQUENCY TIMES FLAG :%d\n",
                         aftsensordescriptor->fradar.indepf_times_flg);
   fprintf("INDEPENDENT FREQUENCY GATE     : %d\n",
                         aftsensordescriptor->fradar.indep_freq_gate);
   fprintf("TIME SERIES GATE               : %d\n",
                         aftsensordescriptor->fradar.times_series_gate);
    close(printer);
}





















