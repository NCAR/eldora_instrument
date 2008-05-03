#define scope extern
#include <stdlib.h>
#include "mkrDef.h"
#include "mkrGbl.h"
#include "mkrFunc.h"

/****************************************************************************/
/*            SUBROUTINE INITIALIZE PARAMETER DESCRIPTOR                    */
/***************************************************************************/

void initialize_parameter(param_pntr)
       struct parameter_d *param_pntr;
  {
      int size,i;
       
       param_pntr->parameter_des[0] = 'P';
       param_pntr->parameter_des[1] = 'A';
       param_pntr->parameter_des[2] = 'R';
       param_pntr->parameter_des[3] = 'M';    
       size = sizeof(struct parameter_d);
       param_pntr->parameter_des_length = size;
       for(i = 0; i < 8; i++)
          param_pntr->parameter_name[i] = ' ';
        for(i = 0; i < 40; i++)
          param_pntr->param_description[i] = ' ';
        for(i = 0; i < 8; i++) 
           param_pntr->param_units[i] = ' ';
        param_pntr->interpulse_time = -999;
        param_pntr->xmitted_freq    = -999;
        param_pntr->recvr_bandwidth = -999.0;
        param_pntr->pulse_width     = -999;
        param_pntr->polarization = -999;
        param_pntr->num_samples =  -999;
        param_pntr->binary_format        = 2;
        for(i = 0; i < 8; i++)
           param_pntr->threshold_field[i] = ' ';
        param_pntr->threshold_value = -999.0;
        param_pntr->parameter_scale = -999.0;
        param_pntr->parameter_bias =  -999.0;
        param_pntr->bad_data = -999;
  }/*end initialize parameter*/

/**************************************************************************/
/*            SUBROUTINE INITIALIZE PARAMETER STATE STRUCTURE             */
/**************************************************************************/

void initialize_parameter_state()
{
    param_st.parameter_name[0] = 0;
    param_st.param_description[0] = 0;
    param_st.param_units[0] = 0;
    param_st.interpulse_time = 0;
    param_st.xmitted_freq    = 0;
    param_st.recvr_bandwidth = 0.0;
    param_st.pulse_width     = 0;
    param_st.polarization = 0;
    param_st.num_samples =  0;
    param_st.threshold_field[0] = 0;
    param_st.threshold_value = 0.0;
    param_st.parameter_scale = 0.0;
    param_st.parameter_bias =  0.0;
    param_st.bad_data = 0;

  } /* end initialize parameter state */

/*****************************************************************************/
/*               SUBROUTINE EDIT PARAMETER DESCRIPTOR                        */
/****************************************************************************/

void edit_parameter(param_pntr)
      struct parameter_d *param_pntr;

   {
      int choice;
      char string[80];
      float bandwidth;
      float value;
      float scale;
      float bias;
      short width;
      short polarization;
      short samples;
      short format;
      short bad;
      short time;
      short freq;


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
                      strcpy(param_pntr->parameter_name,string);
                    break;
             case 2:
                    do{
                       printf("ENTER DESCRIPTION\n");
                       gets(string);
                       if(strlen(string) > 40) printf("NAME IS TOO LONG\n");
                    } while(strlen(string) > 40);
                      strcpy(param_pntr->param_description,string);
                    break;
             case 3:
                    do{
                       printf("ENTER UNITS\n");
                       gets(string);
                       if(strlen(string) > 8) printf("UNITS ARE TOO LONG\n");
                    } while(strlen(string) > 8);
                      strcpy(param_pntr->param_units,string);
                    break;
             case 4:
                    printf("ENTER INTERPULSE TIME\n");
                    gets(string);
                    time  = atoi(string);
                    param_pntr->interpulse_time = time;
                    break;
             case 5:
                    printf("ENTER FREQUENCY\n");
                    gets(string);
                    freq = atoi(string);
                    param_pntr->xmitted_freq = freq;
                    break;
             case 6:
                    printf("ENTER BANDWIDTH\n");
                    gets(string);
                    bandwidth = atof(string);
                    param_pntr->recvr_bandwidth = bandwidth;
                    break;
             case 7:
                    printf("ENTER WIDTH\n");
                    gets(string);
                    width = atoi(string);
                    param_pntr->pulse_width = width;
                     break;
             case 8:
                    printf("ENTER POLARIZATION\n");
                    gets(string);
                    polarization = atoi(string);
                    param_pntr->polarization = polarization;
                    break;
             case 9:
                    printf("ENTER NUMBER OF SAMPLES\n");
                    gets(string);
                    samples = atoi(string);
                    param_pntr->num_samples = samples;
                    break;
             case 10:
                    printf("ENTER FORMAT\n");
                    gets(string);
                    format = atoi(string);
                    param_pntr->binary_format = format;  
                    break;
             case 11:
                    do{
                       printf("ENTER NAME\n");
                       gets(string);
                       if(strlen(string) > 8) printf("NAME IS TOO LONG\n");
                    } while(strlen(string) > 8);
                      strcpy(param_pntr->threshold_field,string); 
                      break;
             case 12:
                    printf("ENTER VALUE\n");
                    gets(string);
                    value = atof(string);
                    param_pntr->threshold_value = value;  
                    break;
             case 13:
                    printf("ENTER SCALE\n");
                    gets(string);
                    scale = atof(string);
                    param_pntr->parameter_scale= scale;  
                    break;
             case 14:
                    printf("ENTER BIAS\n");
                    gets(string);
                    bias = atof(string);
                    param_pntr->parameter_bias = bias;
                    break;
             case 15:
                    printf("ENTER DATA DATA FLAG\n");
                    gets(string);
                    bad = atof(string);
                    param_pntr->bad_data = bad;
                    break;
             default:
                    break;
         }/*switch*/
      }/*for*/
}/*end initialize parameter descriptor*/


/***************************************************************************/
/*   SUBROUTINE TO PRINT PARAMETER DESCRIPTOR                             */
/***************************************************************************/

void fprint_parameter(param_pntr,print_pntr)
    struct parameter_d  *param_pntr;
    FILE *print_pntr;

  {

    int i;

      fprintf(print_pntr,"**********PARAMETER DESCRIPTOR**********\n");
      fprintf(print_pntr,"PARAMETER DESCRIPTOR        : ");
      for(i=0; i<4; i++)
	fprintf(print_pntr,"%c",param_pntr->parameter_des[i]);
      fprintf(print_pntr,"\nPARAMETER DESCRIPTOR LENGTH : %ld\n",
                  param_pntr->parameter_des_length);
      fprintf(print_pntr,"PARAMETER NAME              : ");
       for(i = 0; i < 8; i++)
           fprintf(print_pntr,"%c",
                  param_pntr->parameter_name[i]);
      fprintf(print_pntr,"\nPARAMETER DESCRIPTION       : ");
       for(i = 0; i < 40; i++)
      fprintf(print_pntr,"%c",
                   param_pntr->param_description[i]);
       fprintf(print_pntr,"\nPARAMETER UNITS             : ");
       for(i = 0; i < 8; i++)
             fprintf(print_pntr,"%c",
                    param_pntr->param_units[i]);
       fprintf(print_pntr,"\nINTERPULSE PERIODS USED       : 0x%02x\n",
                    param_pntr->interpulse_time);
       fprintf(print_pntr,"TRANS. FREQUENCIES USED       : 0x%02x\n",
                     param_pntr->xmitted_freq);
       fprintf(print_pntr,"EFFECTIVE RECEIVER BANDWIDTH (MHz) : %f\n",
                     param_pntr->recvr_bandwidth);
       fprintf(print_pntr,"PULSE WIDTH (m)             : %d\n",
                     param_pntr->pulse_width);
       fprintf(print_pntr,"POLARIZATION                : %d\n",
                     param_pntr->polarization);
    fprintf(print_pntr,"NUMBER OF SAMPLES USED IN ESTIMATE OF PARAMETER: %d\n",
                     param_pntr->num_samples);
       fprintf(print_pntr,"BINARY FORMAT OF RADAR DATA : %d\n",
                      param_pntr->binary_format);
      fprintf(print_pntr,"THRESHOLD FIELD NAME        : ");
       for(i = 0; i < 8; i++)
           fprintf(print_pntr,"%c",
                  param_pntr->threshold_field[i]);
       fprintf(print_pntr,"\nTHRESHOLD VALUE             : %f\n",
                       param_pntr->threshold_value);
       fprintf(print_pntr,"PARAMETER SCALE             : %f\n",
                       param_pntr->parameter_scale);
       fprintf(print_pntr,"PARAMETER BIAS              : %f\n",
                       param_pntr->parameter_bias);
       fprintf(print_pntr,"BAD DATA FLAG               : %ld\n",
                       param_pntr->bad_data);
}

/***************************************************************************/
/*            SUBROUTINE CALCULATE PARAMETER DESCRIPTORS                   */
/***************************************************************************/

int calculate_parameter()
{
    #define C 2.997925e8            /* Speed of light in Meters per second */
    #define PI 3.141593
    int i, j, number_to_calculate;
    float lamda, prfs, prfl;

    number_to_calculate =9;
    
    /* The parameter names, units and descriptions are fully defined if
       the pulsing scheme has been selected */
    
    if(param_st.parameter_name[0] != 0)
      number_to_calculate--;
    else if(engin_st.schemenum > 0 && radar_st.num_parameter_des > 0)
      {
	  for(i = 0; i < radar[FORE].num_parameter_des; i++)
	    {
		for(j=0; j<8; j++)
		  parameter[i][FORE].parameter_name[j] = 
		    pscheme[engin_win.schemenum].param[i].name[j];

		for(j=0; j<40; j++)
		  parameter[i][FORE].param_description[j] = 
		    pscheme[engin_win.schemenum].param[i].description[j];

		for(j=0; j<8; j++)
		  parameter[i][FORE].param_units[j] = 
		    pscheme[engin_win.schemenum].param[i].units[j];
	    }

	  for(i = 0; i < radar[AFT].num_parameter_des; i++)
	    {
		for(j=0; j<8; j++)
		  parameter[i][AFT].parameter_name[j] = 
		    pscheme[engin_win.schemenum].param[i].name[j];

		for(j=0; j<40; j++)
		  parameter[i][AFT].param_description[j] = 
		    pscheme[engin_win.schemenum].param[i].description[j];

		for(j=0; j<8; j++)
		  parameter[i][AFT].param_units[j] = 
		    pscheme[engin_win.schemenum].param[i].units[j];
	    }

	  param_st.parameter_name[0] = 1;
	  param_st.param_description[0] = 1;
	  param_st.param_units[0] = 1;
	  number_to_calculate--;
      }

/* The interpulse period (IPP) used to calculate this parameter, is based on
   the type of parameter, All of the standard parameters use all of the IPPS,
   this is true for the simple pulsing scheme, or for dual prt.  The "short"
   parameters (i.e. VS, SWS NCPS etc.) in the dual PRT use PRT #1 and the
   "long" parameters (VL SWL etc.) use PRT #2 */

    if(param_st.interpulse_time != 0)
      number_to_calculate--;
    else if(radar_st.num_parameter_des > 0 && radar_st.num_ipps_trans > 0)
      {
	  for(i = 0; i < radar[FORE].num_parameter_des; i++)
	    {
		parameter[i][FORE].interpulse_time = 0;
		for(j=0; j<radar[FORE].num_ipps_trans; j++)
		  parameter[i][FORE].interpulse_time |= 
		    (short)pow(2.0,(double)j);

		if(!strncmp(parameter[i][FORE].parameter_name,"VS",2))
		  parameter[i][FORE].interpulse_time = 0x1; 
		else if(!strncmp(parameter[i][FORE].parameter_name,"SWS",3))
		  parameter[i][FORE].interpulse_time = 0x1; 
		else if(!strncmp(parameter[i][FORE].parameter_name,"NCPS",4))
		  parameter[i][FORE].interpulse_time = 0x1; 
		else if(!strncmp(parameter[i][FORE].parameter_name,"DBZS",2))
		  parameter[i][FORE].interpulse_time = 0x1; 
		else if(!strncmp(parameter[i][FORE].parameter_name,"VL",2))
		  parameter[i][FORE].interpulse_time = 0x2; 
		else if(!strncmp(parameter[i][FORE].parameter_name,"SWL",3))
		  parameter[i][FORE].interpulse_time = 0x2; 
		else if(!strncmp(parameter[i][FORE].parameter_name,"NCPL",4))
		  parameter[i][FORE].interpulse_time = 0x2; 
		else if(!strncmp(parameter[i][FORE].parameter_name,"DBZL",2))
		  parameter[i][FORE].interpulse_time = 0x2; 

	    }

	  for(i = 0; i < radar[AFT].num_parameter_des; i++)
	    {
		parameter[i][AFT].interpulse_time = 0;
		for(j=0; j<radar[AFT].num_ipps_trans; j++)
		  parameter[i][AFT].interpulse_time |= 
		    (short)pow(2.0,(double)j);

		if(!strncmp(parameter[i][AFT].parameter_name,"VS",2))
		  parameter[i][AFT].interpulse_time = 0x1; 
		else if(!strncmp(parameter[i][AFT].parameter_name,"SWS",3))
		  parameter[i][AFT].interpulse_time = 0x1; 
		else if(!strncmp(parameter[i][AFT].parameter_name,"NCPS",4))
		  parameter[i][AFT].interpulse_time = 0x1; 
		else if(!strncmp(parameter[i][AFT].parameter_name,"DBZS",2))
		  parameter[i][AFT].interpulse_time = 0x1; 
		else if(!strncmp(parameter[i][AFT].parameter_name,"VL",2))
		  parameter[i][AFT].interpulse_time = 0x2; 
		else if(!strncmp(parameter[i][AFT].parameter_name,"SWL",3))
		  parameter[i][AFT].interpulse_time = 0x2; 
		else if(!strncmp(parameter[i][AFT].parameter_name,"NCPL",4))
		  parameter[i][AFT].interpulse_time = 0x2; 
		else if(!strncmp(parameter[i][AFT].parameter_name,"DBZL",2))
		  parameter[i][AFT].interpulse_time = 0x2; 

	    }
	  param_st.interpulse_time = 3;
	  number_to_calculate--;
      }

/* The frequencies used to calculate this parameter, is assumed to be
   all of the frequencies in use.  If it is ever required that a
   parameter is recorded for an individual frequency, the code below must 
   change. */

    if(param_st.xmitted_freq != 0)
      number_to_calculate--;
    else if(radar_st.num_parameter_des > 0 && waveform_st.num_chips[0] > 0)
      {
	  for(i = 0; i < radar[FORE].num_parameter_des; i++)
	    {
		parameter[i][FORE].xmitted_freq = 0;
		for(j=0; j<5; j++)
		  if(waveform.num_chips[j] > 0)
		    parameter[i][FORE].xmitted_freq |= 
		      (short)pow(2.0,(double)j);
	    }

	  for(i = 0; i < radar[AFT].num_parameter_des; i++)
	    {
		parameter[i][AFT].xmitted_freq = 0;
		for(j=0; j<5; j++)
		  if(waveform.num_chips[j] > 0)
		    parameter[i][AFT].xmitted_freq |= 
		      (short)pow(2.0,(double)j);
	    }

	  param_st.xmitted_freq = 3;
	  number_to_calculate--;
      }

/* The receiver bandwidth of the Digital IF is based solely on the pulse
   width.  For now say it is 1/T where T is the pulse width.  The actual
   noise bandwidth of a sinx/x function is approximately 0.78/T. */

    if(param_st.recvr_bandwidth != 0)
      number_to_calculate--;
    else if(radar_st.num_parameter_des > 0 && engin_st.chipdur > 0)
      {
	  for(i = 0; i < radar[FORE].num_parameter_des; i++)
		parameter[i][FORE].recvr_bandwidth = 1.0 / engin_win.chipdur;
	  for(i = 0; i < radar[AFT].num_parameter_des; i++)
		parameter[i][AFT].recvr_bandwidth = 1.0 / engin_win.chipdur;

	  param_st.recvr_bandwidth = 3;
	  number_to_calculate--;
      };

/* The pulse width in meters comes directly from the engineering window */

    if(param_st.pulse_width != 0)
      number_to_calculate--;
    else if(radar_st.num_parameter_des > 0 && engin_st.chipdur > 0)
      {
	  for(i = 0; i < radar[FORE].num_parameter_des; i++)
	    parameter[i][FORE].pulse_width = 150. * engin_win.chipdur;
	  for(i = 0; i < radar[AFT].num_parameter_des; i++)
	    parameter[i][AFT].pulse_width = 150. * engin_win.chipdur;

	  param_st.pulse_width = 3;
	  number_to_calculate--;
      };

/* The ELDORA antennas are horizontally polarized - this is fixed and
   cannot change */

    if(param_st.polarization != 0)
      number_to_calculate--;
    else if(radar_st.num_parameter_des > 0)
      {

	  for(i = 0; i < radar[FORE].num_parameter_des; i++)
	    parameter[i][FORE].polarization = 0;
	  for(i = 0; i < radar[AFT].num_parameter_des; i++)
	    parameter[i][AFT].polarization = 0;

	  param_st.polarization = 3;
	  number_to_calculate--;
      }

/* The number of samples comes directly from the engineering window */

    if(param_st.num_samples != 0)
      number_to_calculate--;
    else if(radar_st.num_parameter_des > 0 && engin_st.chipdur > 0)
      {

	  for(i = 0; i < radar[FORE].num_parameter_des; i++)
	    {
		parameter[i][FORE].num_samples = engin_win.numsamples;
		if(!strncmp(parameter[i][FORE].parameter_name,"VS",2))
		  parameter[i][FORE].num_samples = engin_win.numsamples/2;
		if(!strncmp(parameter[i][FORE].parameter_name,"VL",2))
		  parameter[i][FORE].num_samples = engin_win.numsamples/2;
		if(!strncmp(parameter[i][FORE].parameter_name,"SWL",3))
		  parameter[i][FORE].num_samples = engin_win.numsamples/2;
		if(!strncmp(parameter[i][FORE].parameter_name,"NCPL",4))
		  parameter[i][FORE].num_samples = engin_win.numsamples/2;
		if(!strncmp(parameter[i][FORE].parameter_name,"DBZL",4))
		  parameter[i][FORE].num_samples = engin_win.numsamples/2;
	    }

	  for(i = 0; i < radar[AFT].num_parameter_des; i++)
	    {
		parameter[i][AFT].num_samples = engin_win.numsamples;
		if(!strncmp(parameter[i][AFT].parameter_name,"VS",2))
		  parameter[i][AFT].num_samples = engin_win.numsamples/2;
		if(!strncmp(parameter[i][AFT].parameter_name,"VL",2))
		  parameter[i][AFT].num_samples = engin_win.numsamples/2;
		if(!strncmp(parameter[i][AFT].parameter_name,"SWL",3))
		  parameter[i][AFT].num_samples = engin_win.numsamples/2;
		if(!strncmp(parameter[i][AFT].parameter_name,"NCPL",4))
		  parameter[i][AFT].num_samples = engin_win.numsamples/2;
		if(!strncmp(parameter[i][AFT].parameter_name,"DBZL",4))
		  parameter[i][AFT].num_samples = engin_win.numsamples/2;
	    }


	  param_st.num_samples = 1;
	  number_to_calculate--;
      }

/* Currently no realtime thresholding is planned for the ELDORA so
   the threshold field value can be set to 'NONE' */

    if(param_st.threshold_field[0] != 0)
      number_to_calculate--;
    else if(radar_st.num_parameter_des > 0)
      {

	  for(i = 0; i < radar[FORE].num_parameter_des; i++)
	    {
		parameter[i][FORE].threshold_field[0] = 'N';
		parameter[i][FORE].threshold_field[1] = 'O';
		parameter[i][FORE].threshold_field[2] = 'N';
		parameter[i][FORE].threshold_field[3] = 'E';
		for(j=4; j<8; j++)
		  parameter[i][FORE].threshold_field[j] = ' ';
	    }
	  for(i = 0; i < radar[AFT].num_parameter_des; i++)
	    {
		parameter[i][AFT].threshold_field[0] = 'N';
		parameter[i][AFT].threshold_field[1] = 'O';
		parameter[i][AFT].threshold_field[2] = 'N';
		parameter[i][AFT].threshold_field[3] = 'E';
		for(j=4; j<8; j++)
		  parameter[i][AFT].threshold_field[j] = ' ';
	    }

	  param_st.threshold_field[0] = 1;
	  number_to_calculate--;
      }

/* The parameter scale and offset varies based on the parameter also
   the velocity and the spectral width depend on the transmitted
   frequency and the PRT */

    if(param_st.parameter_scale != 0)
      {
	number_to_calculate--;
	/*	return (number_to_calculate); */
      }
    else if(radar_st.num_parameter_des > 0 && engin_st.prf > 0 &&
	    param_st.parameter_name[0] > 0 && radar_st.num_ipps_trans > 0 &&
	    engin_st.avg_freq[0] > 0)
      {
	  lamda = C / (1.0e9 * engin_win.avg_freq[FORE]);
	  /* period #1 is always shortest */
	  prfs = 1000.0 / radar[FORE].interpulse_per1;
	  /* Period #2 will be longer */
	  prfl = 1000.0 / radar[FORE].interpulse_per2;

	  for(i = 0; i < radar[FORE].num_parameter_des; i++)
	    {
		/* Spectral width short PRT ? */
		if(!strncmp(parameter[i][FORE].parameter_name,"SWS",3))
		  {
		      parameter[i][FORE].parameter_scale = 
			4042.0363 / (lamda * prfs);
		      parameter[i][FORE].parameter_bias =  0.0;
		  }

		/* Spectral width long PRT ? */
		else if(!strncmp(parameter[i][FORE].parameter_name,"SWL",3))
		  {
		      parameter[i][FORE].parameter_scale = 
			4042.0363 / (lamda * prfl);
		      parameter[i][FORE].parameter_bias =  0.0;
		  }

		/* Spectral width combined, or single PRT ? */
		else if(!strncmp(parameter[i][FORE].parameter_name,"SW ",3))
		  {
		      if(radar[FORE].num_ipps_trans == 1)
			{
			 parameter[i][FORE].parameter_scale = 
			     4042.0363 / (lamda * prfs);
			 parameter[i][FORE].parameter_bias =  0.0;
		        }
		      else if(radar[FORE].num_ipps_trans == 2)
			{
			 parameter[i][FORE].parameter_scale = 
			     4042.0363 / (lamda * prfl);
			 parameter[i][FORE].parameter_bias =  0.0;
		        }
		  }

		/* Velocity combined or single PRT? VR */
		else if(!strncmp(parameter[i][FORE].parameter_name,"VR",2))
		  {
		      if(radar[FORE].num_ipps_trans == 1)
			{
			    parameter[i][FORE].parameter_scale = 
			      32764.0 / (lamda * prfs);
			    parameter[i][FORE].parameter_bias =  8191.0;
		        }
		      if(radar[FORE].num_ipps_trans == 2)
			{
			    parameter[i][FORE].parameter_scale = 
			      26213.6 / (lamda * prfl);
			    parameter[i][FORE].parameter_bias =  32767.0;
		        }
		  }

		/* Velocity? VS */
		else if(!strncmp(parameter[i][FORE].parameter_name,"VS",2))
		  {
		      parameter[i][FORE].parameter_scale = 
			32764.0 / (lamda * prfs);
		      parameter[i][FORE].parameter_bias =  8191.0;
		  }

		/* Velocity? VL */
		else if(!strncmp(parameter[i][FORE].parameter_name,"VL",2))
		  {
		      parameter[i][FORE].parameter_scale = 
			32764.0 / (lamda * prfl);
		      parameter[i][FORE].parameter_bias =  8191.0;
		  }

		/* Normalized Coherent Power ? */
		else if(!strncmp(parameter[i][FORE].parameter_name,"NCP",3))
		  {
		      parameter[i][FORE].parameter_scale = 1024.0;
		      parameter[i][FORE].parameter_bias =  0.0;
		  }
		/* Reflectivity ? */
		else if(!strncmp(parameter[i][FORE].parameter_name,"DBZ",3))
		  {
		      parameter[i][FORE].parameter_scale = 8.0;
		      parameter[i][FORE].parameter_bias =  280.0;
		  }
		else
		  {
		      parameter[i][FORE].parameter_scale = -999.0;
		      parameter[i][FORE].parameter_bias =  -999.0;
		  }
	    }

	  /* Do the same thing for the aft radar */

	  lamda = C / (1.0e9 * engin_win.avg_freq[AFT]);
	  /* period #1 is always shortest */
	  prfs = 1000.0 / radar[AFT].interpulse_per1;
	  /* Period #2 will be longer */
	  prfl = 1000.0 / radar[AFT].interpulse_per2;

	  for(i = 0; i < radar[AFT].num_parameter_des; i++)
	    {
		/* Spectral width short PRT ? */
		if(!strncmp(parameter[i][AFT].parameter_name,"SWS",3))
		  {
		      parameter[i][AFT].parameter_scale = 
			4042.0363 / (lamda * prfs);
		      parameter[i][AFT].parameter_bias =  0.0;
		  }

		/* Spectral width long PRT ? */
		else if(!strncmp(parameter[i][AFT].parameter_name,"SWL",3))
		  {
		      parameter[i][AFT].parameter_scale = 
			4042.0363 / (lamda * prfl);
		      parameter[i][AFT].parameter_bias =  0.0;
		  }

		/* Spectral width combined, or single PRT ? */
		else if(!strncmp(parameter[i][AFT].parameter_name,"SW ",3))
		  {
		      if(radar[AFT].num_ipps_trans == 1)
			{
			 parameter[i][AFT].parameter_scale = 
			     4042.0363 / (lamda * prfs);
			 parameter[i][AFT].parameter_bias =  0.0;
		        }
		      else if(radar[AFT].num_ipps_trans == 2)
			{
			 parameter[i][AFT].parameter_scale = 
			     4042.0363 / (lamda * prfl);
			 parameter[i][AFT].parameter_bias =  0.0;
		        }
		  }

		/* Velocity combined or single PRT? VR */
		else if(!strncmp(parameter[i][AFT].parameter_name,"VR",2))
		  {
		      if(radar[AFT].num_ipps_trans == 1)
			{
			    parameter[i][AFT].parameter_scale = 
			      32764.0 / (lamda * prfs);
			    parameter[i][AFT].parameter_bias =  8191.0;
		        }
		      if(radar[AFT].num_ipps_trans == 2)
			{
			    parameter[i][AFT].parameter_scale = 
			      26213.6 / (lamda * prfl);
			    parameter[i][AFT].parameter_bias =  32767.0;
		        }
		  }

		/* Velocity? VS */
		else if(!strncmp(parameter[i][AFT].parameter_name,"VS",2))
		  {
		      parameter[i][AFT].parameter_scale = 
			32764.0 / (lamda * prfs);
		      parameter[i][AFT].parameter_bias =  8191.0;
		  }

		/* Velocity? VL */
		else if(!strncmp(parameter[i][AFT].parameter_name,"VL",2))
		  {
		      parameter[i][AFT].parameter_scale = 
			32764.0 / (lamda * prfl);
		      parameter[i][AFT].parameter_bias =  8191.0;
		  }

		/* Normalized Coherent Power ? */
		else if(!strncmp(parameter[i][AFT].parameter_name,"NCP",3))
		  {
		      parameter[i][AFT].parameter_scale = 1024.0;
		      parameter[i][AFT].parameter_bias =  0.0;
		  }
		/* Reflectivity ? */
		else if(!strncmp(parameter[i][AFT].parameter_name,"DBZ",3))
		  {
		      parameter[i][AFT].parameter_scale = 8.0;
		      parameter[i][AFT].parameter_bias =  280.0;
		  }
		else
		  {
		      parameter[i][AFT].parameter_scale = -999.0;
		      parameter[i][AFT].parameter_bias =  -999.0;
		  }
	    }

	  param_st.parameter_scale = 3;
	  param_st.parameter_bias = 3;
	  number_to_calculate--;
      }

/* There is no way for the real time system to flag bad data at the
   present time, therefore, just set to -999 */

    for(i = 0; i < radar[FORE].num_parameter_des; i++)
      parameter[i][FORE].bad_data = -999;
    for(i = 0; i < radar[AFT].num_parameter_des; i++)
      parameter[i][AFT].bad_data = -999;

  return(number_to_calculate);

  }/*end calculate parameter*/
