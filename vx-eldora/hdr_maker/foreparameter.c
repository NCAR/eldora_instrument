#include <stdio.h>
#include <floatingpoint.h>
#include "field.h"
/****************************************************************************/
/*            SUBROUTINE INITIALIZE PARAMETER DESCRIPTOR                    */
/***************************************************************************/


   initialize_foreparameter_descriptor(foresensordescriptor,num)
       struct sensorDes *foresensordescriptor;
      int *num;
  {
      int size,i;
      int element;
       
       element = *num;
       foresensordescriptor->parameter[element].parameter_des[0] = 'P';
       foresensordescriptor->parameter[element].parameter_des[1] = 'A';
       foresensordescriptor->parameter[element].parameter_des[2] = 'R';
       foresensordescriptor->parameter[element].parameter_des[3] = 'M';    
       size = sizeof(foresensordescriptor->parameter)/MAXPARAMETERS;
       foresensordescriptor->parameter[element].parameter_des_length = size;
       for(i = 0; i < 8; i++)
          foresensordescriptor-> parameter[element].parameter_name[i] = ' ';
        for(i = 0; i < 40; i++)
          foresensordescriptor-> parameter[element].param_description[i] = ' ';
        for(i = 0; i < 8; i++) 
           foresensordescriptor->parameter[element].param_units[i] = ' ';
        foresensordescriptor->parameter[element].interpulse_time = -999;
        foresensordescriptor->parameter[element].xmitted_freq    = -999;
        foresensordescriptor->parameter[element].recvr_bandwidth = -999.0;
        foresensordescriptor->parameter[element].pulse_width     = -999;
        foresensordescriptor->parameter[element].polarization = -999;
        foresensordescriptor->parameter[element].num_samples =  -999;
        foresensordescriptor->parameter[element].binary_format        = 2;
        for(i = 0; i < 8; i++)
           foresensordescriptor->parameter[element].threshold_field[i] = ' ';
        foresensordescriptor->parameter[element].threshold_value = -999.0;
        foresensordescriptor->parameter[element].parameter_scale = -999.0;
        foresensordescriptor->parameter[element].parameter_bias =  -999.0;
        foresensordescriptor->parameter[element].bad_data = -999;
  }/*end initialize parameter*/



/*****************************************************************************/
/*               SUBROUTINE EDIT PARAMETER DESCRIPTOR                        */
/****************************************************************************/

    edit_foreparameter_descriptor(foresensordescriptor,num)
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
      for(;;){
        do{
          printf("****************EDIT PARAMETER DESCRIPTOR MENU**********\n");
          printf("0.  RETURN TO MAIN MENU\n");
          printf("1.  ENTER PARAMETER NAME\n");
          printf("2.  ENTER PARAMETER DESCRIPTION\n");
          printf("3.  ENTER PARAMETER UNITS\n");
          printf("4.  ENTER THE EFFECTIVE INTER-PULSE PERION IN us\n");
          printf("5.  ENTER THE AVERAGE TRANSMITTED FREQUENCY IN GHz\n");
          printf("6.  ENTER THE RECEIVER BANDWIDTH FOR PARAMETER IN MHz\n");
          printf("7.  ENTER THE PULSE WIDTH IN M\n");
          printf("8. ENTER THE POLARIZATION OF THE RADAR BEAM\n");
          printf("9. ENTER THE NUMBER OF SAMPLES USED IN ESTIMATE\n");
          printf("10. ENTER THE BINARY FORMAT OF RADAR DATA\n");
          printf("11. ENTER THE NAME OF PARAMETER UPON WHICH THIS PARAMETER IS THERHOLDED (8 ASCII CHARS)\n");
          printf("12. ENTER THE VALUE OF THE THRESHOLD\n");
          printf("13. ENTER THE SCALE FACTOR FOR THIS PARAMETER\n");
          printf("14. ENTER THE BIAS FACTOR FOR THIS PARAMETER\n");
          printf("15. ENTER THE BAD DATA FLAG\n");
          printf("ENTER YOUR CHOICE\n");
          gets(string);
          choice = atoi(string);
          if(choice <0 || choice > 15) printf("INVALID ENTRY\n");
          } while(choice <0 || choice > 15);

 
          switch(choice){
             case 0:
                    return;
                    break;
             case 1:
                    do{
                       printf("ENTER NAME\n");
                       gets(string);
                       if(strlen(string) > 8) printf("NAME IS TOO LONG\n");
                    } while(strlen(string) > 8);
                      strcpy(foresensordescriptor->parameter[element].parameter_name,string);
                    break;
             case 2:
                    do{
                       printf("ENTER DESCRIPTION\n");
                       gets(string);
                       if(strlen(string) > 40) printf("NAME IS TOO LONG\n");
                    } while(strlen(string) > 40);
                      strcpy(foresensordescriptor->parameter[element].param_description,string);
                    break;
             case 3:
                    do{
                       printf("ENTER UNITS\n");
                       gets(string);
                       if(strlen(string) > 8) printf("UNITS ARE TOO LONG\n");
                    } while(strlen(string) > 8);
                      strcpy(foresensordescriptor->parameter[element].param_units,string);
                    break;
             case 4:
                    printf("ENTER INTERPULSE TIME\n");
                    gets(string);
                    time  = atoi(string);
                    foresensordescriptor->parameter[element].interpulse_time = time;
                    break;
             case 5:
                    printf("ENTER FREQUENCY\n");
                    gets(string);
                    freq = atoi(string);
                    foresensordescriptor->parameter[element].xmitted_freq = freq;
                    break;
             case 6:
                    printf("ENTER BANDWIDTH\n");
                    gets(string);
                    bandwidth = atof(string);
                    foresensordescriptor->parameter[element].recvr_bandwidth = bandwidth;
                    break;
             case 7:
                    printf("ENTER WIDTH\n");
                    gets(string);
                    width = atoi(string);
                    foresensordescriptor->parameter[element].pulse_width = width;
                     break;
             case 8:
                    printf("ENTER POLARIZATION\n");
                    gets(string);
                    polarization = atoi(string);
                    foresensordescriptor->parameter[element].polarization = polarization;
                    break;
             case 9:
                    printf("ENTER NUMBER OF SAMPLES\n");
                    gets(string);
                    samples = atoi(string);
                    foresensordescriptor->parameter[element].num_samples = samples;
                    break;
             case 10:
                    printf("ENTER FORMAT\n");
                    gets(string);
                    format = atoi(string);
                    foresensordescriptor->parameter[element].binary_format = format;  
                    break;
             case 11:
                    do{
                       printf("ENTER NAME\n");
                       gets(string);
                       if(strlen(string) > 8) printf("NAME IS TOO LONG\n");
                    } while(strlen(string) > 8);
                      strcpy(foresensordescriptor->parameter[element].threshold_field,string); 
                      break;
             case 12:
                    printf("ENTER VALUE\n");
                    gets(string);
                    value = atof(string);
                    foresensordescriptor->parameter[element].threshold_value = value;  
                    break;
             case 13:
                    printf("ENTER SCALE\n");
                    gets(string);
                    scale = atof(string);
                    foresensordescriptor->parameter[element].parameter_scale= scale;  
                    break;
             case 14:
                    printf("ENTER BIAS\n");
                    gets(string);
                    bias = atof(string);
                    foresensordescriptor->parameter[element].parameter_bias = bias;
                    break;
             case 15:
                    printf("ENTER DATA DATA FLAG\n");
                    gets(string);
                    bad = atof(string);
                    foresensordescriptor->parameter[element].bad_data = bad;
                    break;
             default:
                    break;
         }/*switch*/
      }/*for*/
}/*end initialize parameter descriptor*/



/***************************************************************************/
/*           SUBROUTINE PRINT PARAMETER DESCRIPTOR                         */
/***************************************************************************/

  print_foreparameter_descriptor(foresensordescriptor,index)
    struct sensorDes  *foresensordescriptor;
    int *index;

  {

    int i;
    int element,number;
   
       element = *index;
       number = element + 1;
      printf("**********PARAMETER NUMBER %d DESCRIPTOR************\n",number);
       printf("PARAMETER DESCRIPTOR        :%s\n",
                  foresensordescriptor->parameter[element].parameter_des);
       printf("PARAMETER DESCRITPOR LENGTH : %d\n",
                  foresensordescriptor->parameter[element].parameter_des_length);
       printf("PARAMETER NAME              : ");
       for(i = 0; i < 8; i++)
           printf("%c",foresensordescriptor->parameter[element].parameter_name[i]);
       printf("\nPARAMETER DESCRIPTION       : ");
       for(i = 0; i < 40; i++)
       printf("%c",foresensordescriptor->parameter[element].param_description[i]);
       printf("\nPARAMETER UNITS             : ");
       for(i = 0; i < 8; i++)
              printf("%c",foresensordescriptor->parameter[element].param_units[i]);
       printf("\nINTERPULSE TIME             : %d\n",
                    foresensordescriptor->parameter[element].interpulse_time);
       printf("TRANSMITTED FREQUENCY       : %d\n",
                     foresensordescriptor->parameter[element].xmitted_freq);
       printf("EFFECTIVE RECEIVER BANDWIDTH IN MHz : %f\n",
                     foresensordescriptor->parameter[element].recvr_bandwidth);
       printf("PULSE WIDTH                 ; %d\n",
                     foresensordescriptor->parameter[element].pulse_width);
       printf("POLARIZATION                : %d\n",
                     foresensordescriptor->parameter[element].polarization);
       printf("NUMBER OF SAMPLES USED IN ESTIMATE OF PARAMETER: %d\n",
                     foresensordescriptor->parameter[element].num_samples);
       printf("BINARY FORMAT OF RADAR DATA : %d\n",
                      foresensordescriptor->parameter[element].binary_format);
       printf("THRESHOLD FIELD NAME        : %s\n",
                      foresensordescriptor->parameter[element].threshold_field);
       printf("THRESHOLD VALUE             : %f\n",
                       foresensordescriptor->parameter[element].threshold_value);
       printf("PARAMETER SCALE             : %.8f\n",
                       foresensordescriptor->parameter[element].parameter_scale);
       printf("PARAMETER BIAS              : %.8f\n",
                       foresensordescriptor->parameter[element].parameter_bias);
       printf("BAD DATA FLAG               : %d\n",
                       foresensordescriptor->parameter[element].bad_data);
}


/***************************************************************************/
/*   SUBROUTINE TO  PRINT PARAMETER DESCRIPTOR LASER PRINTER               */
/***************************************************************************/

  fprint_foreparameter_descriptor(foresensordescriptor,index)
    struct sensorDes  *foresensordescriptor;
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
                  foresensordescriptor->parameter[element].parameter_des);
      fprintf(printer,"PARAMETER DESCRITPOR LENGTH : %d\n",
                  foresensordescriptor->parameter[element].parameter_des_length);
      fprintf(printer,"PARAMETER NAME              : ");
       for(i = 0; i < 8; i++)
           fprintf(printer,"%c",
                  foresensordescriptor->parameter[element].parameter_name[i]);
      fprintf(printer,"\nPARAMETER DESCRIPTION       :\n");
       for(i = 0; i < 40; i++)
      fprintf(printer,"%c",
                   foresensordescriptor->parameter[element].param_description[i]);
       fprintf(printer,"\nPARAMETER UNITS             :\n");
       for(i = 0; i < 8; i++)
             fprintf(printer,"%c",
                    foresensordescriptor->parameter[element].param_units[i]);
       fprintf(printer,"\nINTERPULSE TIME             : %d\n",
                    foresensordescriptor->parameter[element].interpulse_time);
       fprintf(printer,"TRANSMITTED FREQUENCY       : %d\n",
                     foresensordescriptor->parameter[element].xmitted_freq);
       fprintf(printer,"EFFECTIVE RECEIVER BANDWIDTH IN MHz : %f\n",
                     foresensordescriptor->parameter[element].recvr_bandwidth);
       fprintf(printer,"PULSE WIDTH                 ; %d\n",
                     foresensordescriptor->parameter[element].pulse_width);
       fprintf(printer,"POLARIZATION                : %d\n",
                     foresensordescriptor->parameter[element].polarization);
       fprintf(printer,"NUMBER OF SAMPLES USED IN ESTIMATE OF PARAMETER: %d\n",
                     foresensordescriptor->parameter[element].num_samples);
       fprintf(printer,"BINARY FORMAT OF RADAR DATA : %d\n",
                      foresensordescriptor->parameter[element].binary_format);
       fprintf(printer,"THRESHOLD FIELD NAME        : %s\n",
                      foresensordescriptor->parameter[element].threshold_field);
       fprintf(printer,"THRESHOLD VALUE             : %f\n",
                       foresensordescriptor->parameter[element].threshold_value);
       fprintf(printer,"PARAMETER SCALE             : %f\n",
                       foresensordescriptor->parameter[element].parameter_scale);
       fprintf(printer,"PARAMETER BIAS              : %f\n",
                       foresensordescriptor->parameter[element].parameter_bias);
       fprintf(printer,"BAD DATA FLAG               : %d\n",
                       foresensordescriptor->parameter[element].bad_data);
       close(printer);
}



