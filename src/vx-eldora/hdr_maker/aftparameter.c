#include <stdio.h>
#include <floatingpoint.h>
#include "field.h"
/****************************************************************************/
/*            SUBROUTINE INITIALIZE PARAMETER DESCRIPTOR                    */
/***************************************************************************/


   initialize_aftparameter_descriptor(aftsensordescriptor,num)
       struct sensorDes *aftsensordescriptor;
      int *num;
  {
      int size,i;
      int element;
       
       element = *num;
       aftsensordescriptor->parameter[element].parameter_des[0] = 'P';
       aftsensordescriptor->parameter[element].parameter_des[1] = 'A';
       aftsensordescriptor->parameter[element].parameter_des[2] = 'R';
       aftsensordescriptor->parameter[element].parameter_des[3] = 'M';    
       size = sizeof(aftsensordescriptor->parameter)/MAXPARAMETERS;
       aftsensordescriptor->parameter[element].parameter_des_length = size;
       for(i = 0; i < 8; i++)
          aftsensordescriptor-> parameter[element].parameter_name[i] = ' ';
        for(i = 0; i < 40; i++)
          aftsensordescriptor-> parameter[element].param_description[i] = ' ';
        for(i = 0; i < 8; i++) 
           aftsensordescriptor->parameter[element].param_units[i] = ' ';
        aftsensordescriptor->parameter[element].interpulse_time = -999;
        aftsensordescriptor->parameter[element].xmitted_freq    = -999;
        aftsensordescriptor->parameter[element].recvr_bandwidth = -999.0;
        aftsensordescriptor->parameter[element].pulse_width     = -999;
        aftsensordescriptor->parameter[element].polarization = -999;
        aftsensordescriptor->parameter[element].num_samples =  -999;
        aftsensordescriptor->parameter[element].binary_format        = 2;
        for(i = 0; i < 8; i++)
           aftsensordescriptor->parameter[element].threshold_field[i] = ' ';
        aftsensordescriptor->parameter[element].threshold_value = -999.0;
        aftsensordescriptor->parameter[element].parameter_scale = -999.0;
        aftsensordescriptor->parameter[element].parameter_bias =  -999.0;
        aftsensordescriptor->parameter[element].bad_data = -999;
  }/*end initialize parameter*/



/*****************************************************************************/
/*               SUBROUTINE EDIT PARAMETER DESCRIPTOR                        */
/****************************************************************************/

    edit_aftparameter_descriptor(aftsensordescriptor,
                                 foresensordescriptor,num)
      struct sensorDes *aftsensordescriptor;
      struct sensorDes *foresensordescriptor;
      int *num;

   {
      int i,choice;
      char string[80];
      float bandwidth,velocity,rng,value,scale,bias;
      short width,polarization,samples,format,bad;
      short time,freq;
      long  fflag;
      int element;
    

      element = *num;
      
      for(i = 0; i < 8; i++){
           aftsensordescriptor->parameter[element].parameter_name[i] =
                 foresensordescriptor->parameter[element].parameter_name[i];
      }

      for(i = 0; i < 40; i++){
            aftsensordescriptor->parameter[element].param_description[i]  =
                foresensordescriptor->parameter[element].param_description[i];
      }

      for(i = 0; i < 8; i++){
           aftsensordescriptor->parameter[element].param_units[i] =
                foresensordescriptor->parameter[element].param_units[i];
      }

      aftsensordescriptor->parameter[element].interpulse_time = 
                foresensordescriptor->parameter[element].interpulse_time;


      aftsensordescriptor->parameter[element].xmitted_freq  =
                foresensordescriptor->parameter[element].xmitted_freq;


      aftsensordescriptor->parameter[element].recvr_bandwidth = 
                foresensordescriptor->parameter[element].recvr_bandwidth;

      aftsensordescriptor->parameter[element].pulse_width = 
                foresensordescriptor->parameter[element].pulse_width;

      aftsensordescriptor->parameter[element].polarization = 
                foresensordescriptor->parameter[element].polarization;

      aftsensordescriptor->parameter[element].num_samples = 
                foresensordescriptor->parameter[element].num_samples;

      aftsensordescriptor->parameter[element].binary_format = 
                foresensordescriptor->parameter[element].binary_format;  

      for(i = 0; i < 8; i++){
             aftsensordescriptor->parameter[element].threshold_field[i] =
                 foresensordescriptor->parameter[element].threshold_field[i];
      }

      aftsensordescriptor->parameter[element].threshold_value =
                foresensordescriptor->parameter[element].threshold_value ;

      aftsensordescriptor->parameter[element].parameter_scale = 
                foresensordescriptor->parameter[element].parameter_scale;


      aftsensordescriptor->parameter[element].parameter_bias = 
                foresensordescriptor->parameter[element].parameter_bias;


      aftsensordescriptor->parameter[element].bad_data =
                foresensordescriptor->parameter[element].bad_data;
     
}/*end initialize parameter descriptor*/



/***************************************************************************/
/*           SUBROUTINE PRINT PARAMETER DESCRIPTOR                         */
/***************************************************************************/

  print_aftparameter_descriptor(aftsensordescriptor,index)
    struct sensorDes  *aftsensordescriptor;
    int *index;

  {

    int i;
    int element,number;
   
       element = *index;
       number = element + 1;
      printf("**********PARAMETER NUMBER %d DESCRIPTOR************\n",number);
       printf("PARAMETER DESCRIPTOR        :%s\n",
                  aftsensordescriptor->parameter[element].parameter_des);
       printf("PARAMETER DESCRITPOR LENGTH : %d\n",
                  aftsensordescriptor->parameter[element].parameter_des_length);
       printf("PARAMETER NAME              : ");
       for(i = 0; i < 8; i++)
           printf("%c",aftsensordescriptor->parameter[element].parameter_name[i]);
       printf("\nPARAMETER DESCRIPTION       : ");
       for(i = 0; i < 40; i++)
       printf("%c",aftsensordescriptor->parameter[element].param_description[i]);
       printf("\nPARAMETER UNITS             : ");
       for(i = 0; i < 8; i++)
              printf("%c",aftsensordescriptor->parameter[element].param_units[i]);
       printf("\nINTERPULSE TIME             : %d\n",
                    aftsensordescriptor->parameter[element].interpulse_time);
       printf("TRANSMITTED FREQUENCY       : %d\n",
                     aftsensordescriptor->parameter[element].xmitted_freq);
       printf("EFFECTIVE RECEIVER BANDWIDTH IN MHz : %f\n",
                     aftsensordescriptor->parameter[element].recvr_bandwidth);
       printf("PULSE WIDTH                 ; %d\n",
                     aftsensordescriptor->parameter[element].pulse_width);
       printf("POLARIZATION                : %d\n",
                     aftsensordescriptor->parameter[element].polarization);
       printf("NUMBER OF SAMPLES USED IN ESTIMATE OF PARAMETER: %d\n",
                     aftsensordescriptor->parameter[element].num_samples);
       printf("BINARY FORMAT OF RADAR DATA : %d\n",
                      aftsensordescriptor->parameter[element].binary_format);
       printf("THRESHOLD FIELD NAME        : %s\n",
                      aftsensordescriptor->parameter[element].threshold_field);
       printf("THRESHOLD VALUE             : %f\n",
                       aftsensordescriptor->parameter[element].threshold_value);
       printf("PARAMETER SCALE             : %.8f\n",
                       aftsensordescriptor->parameter[element].parameter_scale);
       printf("PARAMETER BIAS              : %.8f\n",
                       aftsensordescriptor->parameter[element].parameter_bias);
       printf("BAD DATA FLAG               : %d\n",
                       aftsensordescriptor->parameter[element].bad_data);
}


/***************************************************************************/
/*   SUBROUTINE TO  PRINT PARAMETER DESCRIPTOR LASER PRINTER               */
/***************************************************************************/

  fprint_aftparameter_descriptor(aftsensordescriptor,index)
    struct sensorDes  *aftsensordescriptor;
    int *index;

  {

    int i;
    int element,number;
    FILE *printer, *popen();
   
       element = *index;
       number = element + 1;

      if((printer = popen("lpr -Pnec","w")) == NULL){
          printf("UNABLE TO ACCESS LASER PRINTER\n");
          return;
      }
      fprintf(printer,"**********PARAMETER NUMBER %d DESCRIPTOR************\n",
                   number);
      fprintf(printer,"PARAMETER DESCRIPTOR        :%s\n",
                  aftsensordescriptor->parameter[element].parameter_des);
      fprintf(printer,"PARAMETER DESCRITPOR LENGTH : %d\n",
                  aftsensordescriptor->parameter[element].parameter_des_length);
      fprintf(printer,"PARAMETER NAME              : ");
       for(i = 0; i < 8; i++)
           fprintf(printer,"%c",
                  aftsensordescriptor->parameter[element].parameter_name[i]);
      fprintf(printer,"\nPARAMETER DESCRIPTION       :\n");
       for(i = 0; i < 40; i++)
      fprintf(printer,"%c",
                   aftsensordescriptor->parameter[element].param_description[i]);
       fprintf(printer,"\nPARAMETER UNITS             :\n");
       for(i = 0; i < 8; i++)
             fprintf(printer,"%c",
                    aftsensordescriptor->parameter[element].param_units[i]);
       fprintf(printer,"\nINTERPULSE TIME             : %d\n",
                    aftsensordescriptor->parameter[element].interpulse_time);
       fprintf(printer,"TRANSMITTED FREQUENCY       : %d\n",
                     aftsensordescriptor->parameter[element].xmitted_freq);
       fprintf(printer,"EFFECTIVE RECEIVER BANDWIDTH IN MHz : %f\n",
                     aftsensordescriptor->parameter[element].recvr_bandwidth);
       fprintf(printer,"PULSE WIDTH                 ; %d\n",
                     aftsensordescriptor->parameter[element].pulse_width);
       fprintf(printer,"POLARIZATION                : %d\n",
                     aftsensordescriptor->parameter[element].polarization);
       fprintf(printer,"NUMBER OF SAMPLES USED IN ESTIMATE OF PARAMETER: %d\n",
                     aftsensordescriptor->parameter[element].num_samples);
       fprintf(printer,"BINARY FORMAT OF RADAR DATA : %d\n",
                      aftsensordescriptor->parameter[element].binary_format);
       fprintf(printer,"THRESHOLD FIELD NAME        : %s\n",
                      aftsensordescriptor->parameter[element].threshold_field);
       fprintf(printer,"THRESHOLD VALUE             : %f\n",
                       aftsensordescriptor->parameter[element].threshold_value);
       fprintf(printer,"PARAMETER SCALE             : %f\n",
                       aftsensordescriptor->parameter[element].parameter_scale);
       fprintf(printer,"PARAMETER BIAS              : %f\n",
                       aftsensordescriptor->parameter[element].parameter_bias);
       fprintf(printer,"BAD DATA FLAG               : %d\n",
                       aftsensordescriptor->parameter[element].bad_data);
       close(printer);
}



