 /*
 *	$id$
 *
 *	Module:		 
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:
 *        
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <Ray.h>
#include <Platform.h>
#include <FieldParam.h>
#include <CellSpacing.h>
#include <FieldRadar.h>
#include <Parameter.h>
#include <RadarDesc.h>
#include <Volume.h>
#include <Waveform.h>
#include <NavDesc.h>
#include <InSitu.h>

#define MAX_BUFF 32768
#define FORE 0
#define AFT 1
#define MAX_PARAM 6


struct BEAM{
    struct ray_i rray;
    struct platform_i rplat;
    struct field_parameter_data rdata;
    short data[200];
};

struct BEAM *beam;
int actual_num_radars, actual_num_params[2];
struct volume_d volume;
struct waveform_d waveform;
struct radar_d radar[2];
struct field_radar_i fradar[2];
struct cell_spacing_d cell[2];
struct parameter_d parameter[10][2];
struct nav_descript navigation;
struct insitu_descript insitu;

void main(void);
int foreaft(void);
int specific_parameter(void);
void print_menu(void);
void fprint_cellspacing(struct cell_spacing_d *,FILE *);
void fprint_fieldradar(struct field_radar_i *,FILE *);
void fprint_insitu(struct insitu_descript *,FILE *);
void fprint_navigation(struct nav_descript *,FILE *);
void fprint_parameter(struct parameter_d *,FILE *);
void fprint_radar(struct radar_d *,FILE *);
void fprint_volume(struct volume_d *,FILE *);
void fprint_waveform(struct waveform_d *,FILE *);
void read_header(void);
void copy_header(char *);
int make_header(char *);
int temp[MAX_BUFF];


void main()
{


int fp_in; /* input file pointer */
extern char *sys_errlist[];
int i, j, amount, raycount, kill;
int hr, min, sec;
char input;

union
  {
      int ray_search;
      char ray_array[4];
  }aray;

union
  {
      int vol_search;
      char vol_array[4];
  }ahead;

hr = 0;
min = 0;
sec = 0;
raycount = 0;
kill = 1;

aray.ray_array[0] = 'R';
aray.ray_array[1] = 'Y';
aray.ray_array[2] = 'I';
aray.ray_array[3] = 'B';

ahead.vol_array[0] = 'V';
ahead.vol_array[1] = 'O';
ahead.vol_array[2] = 'L';
ahead.vol_array[3] = 'D';

/**** Determine if from disk or tape ******/

printf("\n\nWelcome to the ELDORA header dump program");
printf("\n\nDisplay header from tape or disk? (t,d):"); 
input = getchar();
getchar();

if(input == 'd' || input == 'D')
  {
      read_header();
      print_menu();
      exit(1);
  }


/* If user wants header from a tape then Open The Input Tape Drive */

printf("I am going to read the first record of the tape, PLEASE WAIT!\n");

fp_in = open("/dev/rmt/0b", O_RDONLY); /* Open tape drive for binary read */
if(fp_in<0)
{
	printf("CANNOT OPEN input tape drive\n");
	printf("REASON: %s\n",sys_errlist[errno]);       
	exit(2); /* If cannot open, exit */
}

/* Begin Reading Tape */
do{

if((amount = read(fp_in,temp,MAX_BUFF)) > 0)
{
    /* Did we read a volume header? */

    if(temp[0]==ahead.vol_search)
      {
	  printf("Found a volume header size = %d\n",amount);

	  /* Allow user to Write out the volume header */
	  
	  copy_header(( char *)&temp[0]);
	  print_menu();
	  printf("\n\nFind the next header on the tape? (y,n):");
	  input = getchar();
	  getchar();
	  if(input == 'n' || input == 'N') kill = 0;
      }

    /* Did we find the start of a ray info block? */

    if(temp[0]==aray.ray_search)
      {
	  raycount++;
	  beam = (struct BEAM *)&temp[0];
	  temp[0] = 0;      /* clear out to make sure next record is unique */
	  hr = beam->rray.hour;
	  min = beam->rray.minute;
	  sec = beam->rray.second;
	  if(raycount > 100)
	    {
		raycount = 0;
		printf("Processing data record at %2d:%2d:%2d\n",
		       hr,min,sec);
	    }
      }

}  /* amount > 0 */

/* amount == 0 if an end-of-file was encountered */ 

if(amount==0)
{
    printf("FOUND END OF FILE MARKER!\n");
    /* write an end-of-file on output tape */
}

}while(kill != 0);

close(fp_in);

}

/************************************************************************/
/*          Function foreaft gets an F or an A from the user            */
/*                 and returns either FORE or AFT                       */
/************************************************************************/

int foreaft()
{
    char input[80];

    for(;;)
      {

	  printf("Enter F For the fore radar ");
	  printf(" or enter A For the aft radar: ");
	  gets(input);
	  printf("\n");
	  if(input[0] == 'F' || input[0] == 'f')
	    return(FORE);
	  else if(input[0] == 'A' || input[0] == 'a')
	    return(AFT);
	  else
	    printf("invalid entry\n");
      }
}

/************************************************************************/
/*          Function param_num returns the number of the                */
/*            parameter selected by the user                            */
/************************************************************************/

int specific_param(radarnum)
  int radarnum;
    {
    char input[80];
    int num, i;
    
	for(i=0; i< actual_num_params[radarnum]; i++)
	  printf("%2d) is called: %c%c%c%c%c%c%c%c\n",i,
		 parameter[i][radarnum].parameter_name[0],
		 parameter[i][radarnum].parameter_name[1],
		 parameter[i][radarnum].parameter_name[2],
		 parameter[i][radarnum].parameter_name[3],
		 parameter[i][radarnum].parameter_name[4],
		 parameter[i][radarnum].parameter_name[5],
		 parameter[i][radarnum].parameter_name[6],
		 parameter[i][radarnum].parameter_name[7]
		 );
    do{
	printf("Enter the desired parameter number: ");
	gets(input);
	num = atoi(input);
	if(num >= actual_num_params[radarnum] || num < 0)
	  printf("\nParameter number must be: >=0 and < %d\n",
		 actual_num_params[radarnum]);
    }while(num >= actual_num_params[radarnum] || num < 0);
    return(num);
}


/****************************************************************************/
/*                   SUBROUTINE PRINT MENU PRINTER                          */
/****************************************************************************/
 void print_menu()
{
    int choice, i, j, radarnum, paramnum;
    char input[80], onoff[3], outname[80];
    FILE *printer, *popen(), *fp_out, *outfdf;
    int numbytes, items, byteswritten;
    extern char *sys_errlist[];

    onoff[0] = 'o';
    onoff[1] = 'f';
    onoff[2] = 'f';

    for(;;){ 
      do{
        printf("********PRINT DESCRIPTORS MENU************\n");
        printf("0. EXIT PRINT MENU\n");
	printf("1. TOGGLE ON/OFF OUTPUT TO PRINTER, CURRENTLY: %c%c%c\n",
	       onoff[0], onoff[1], onoff[2]);
        printf("2. PRINT VOLUME DESCRIPTOR\n");
        printf("3. PRINT WAVEFORM DESCRIPTOR\n");
        printf("4. PRINT RADAR DESCRIPTOR\n");
        printf("5. PRINT FIELD RADAR DESCRIPTOR\n");
        printf("6. PRINT CELL SPACING DESCRIPTOR\n");
        printf("7. PRINT PARAMETER DESCRIPTORS\n");
        printf("8. PRINT NAVIGATIONAL DATA DESCRIPTOR\n");
	printf("9. PRINT THE INSITU DATA DESCRIPTOR\n");
        printf("10. PRINT ALL AT ONCE\n");
	printf("11. WRITE THIS HEADER TO DISK;\n");
        printf("12. WRITE THIS HEADER TO TAPE\n");
        printf("ENTER YOUR CHOICE\n");
        gets(input);
        choice = atoi(input);
        if(choice < 0 || choice > 12) printf("INVALID ENTRY\n");
    }while(choice < 0 || choice > 12);

    switch(choice){
	   case 0:
	        if(onoff[1] == 'n') pclose(printer);
	        return;
		break;

	   case 1:
		if(onoff[1] == 'n')
		  {
		      pclose(printer);
		      onoff[1] = 'f';
		      onoff[2] = 'f';
		  }
		else
		  {
		      onoff[1] = 'n';
		      onoff[2] = ' ';
		      if(( printer = popen("lp","w")) == NULL){
			  printf("\nUNABLE TO ACCESS LASER PRINTER\n");
			  return;
		      }
		  }
		break;

           case 2: 
                fprint_volume(&volume,stdout);
		if(onoff[1] == 'n')
		  {
		      fprint_volume(&volume,printer);
		  }
                break;

           case 3:
                fprint_waveform(&waveform,stdout);
		if(onoff[1] == 'n')
		  {
		      fprint_waveform(&waveform,printer);
		  }
                break;

           case 4:
                radarnum = foreaft();
                fprint_radar(&radar[radarnum],stdout);
		if(onoff[1] == 'n')
		  {
		      fprint_radar(&radar[radarnum],printer);
		  }
                break;

           case 5:
                radarnum = foreaft();
                fprint_fieldradar(&fradar[radarnum],stdout);
		if(onoff[1] == 'n')
		  {
		      fprint_fieldradar(&fradar[radarnum],printer);
		  }
                break;

           case 6: 
                radarnum = foreaft();
                fprint_cellspacing(&cell[radarnum],stdout);
		if(onoff[1] == 'n')
		  {
		      fprint_cellspacing(&cell[radarnum],printer);
		  }
                break;

           case 7:
	        radarnum = foreaft();
                paramnum = specific_param(radarnum);
                fprint_parameter(&parameter[paramnum][radarnum],stdout);
		if(onoff[1] == 'n')
		  {
		      fprint_parameter(&parameter[paramnum][radarnum],printer);
		  }
                break;

           case 8: 
                fprint_navigation(&navigation,stdout);
		if(onoff[1] == 'n')
		  {
		      fprint_navigation(&navigation,printer);
		  }
                break;

           case 9: 
                fprint_insitu(&insitu,stdout);
		if(onoff[1] == 'n')
		  {
		      fprint_insitu(&insitu,printer);
		  }
                break;

           default:
                break;

	   case 10:
		fprint_volume(&volume,stdout);

		fprint_waveform(&waveform,stdout);

		for(i=0; i < actual_num_radars; i++)
		  {
		      fprint_radar(&radar[i],stdout);

		      fprint_fieldradar(&fradar[i],stdout);

		      fprint_cellspacing(&cell[i],stdout);

		      for(j=0; j<actual_num_params[i]; j++)
			{
			    fprint_parameter(&parameter[j][i],stdout);
			}
		  }
		fprint_navigation(&navigation,stdout);

		fprint_insitu(&insitu,stdout);


		if(onoff[1] == 'n')
		  {
		      fprintf(printer,"\n\n");
		      fprint_volume(&volume,printer);

		      fprint_waveform(&waveform,printer);

		      for(i=0; i < actual_num_radars; i++)
			{
			    fprint_radar(&radar[i],printer);

			    fprint_fieldradar(&fradar[i],printer);

			    fprint_cellspacing(&cell[i],printer);

			    for(j=0; j<actual_num_params[i]; j++)
			      {
				  fprint_parameter(&parameter[j][i],printer);
			      }
			}
		      fprint_navigation(&navigation,printer);

		      fprint_insitu(&insitu,printer);

		  }
            break;
		
	      case 11:
		/* Write current header to a disk file */
		numbytes = make_header((char *)temp);

		/* get the filename and open it */

		printf("OUTPUT FILE NAME ?\n");
		gets(outname);
		printf("OUTPUT FILE NAME: %s\n",outname);
		if((outfdf = fopen(outname,"wb")) == NULL){
		    printf("UNABLE TO OPEN THE OUTPUT FILE\n");
		    exit();
		}

		/* write out the entire header */

		byteswritten = fwrite((char *)&temp[0],numbytes,1,outfdf);
		close(outfdf);
		break;
		
	      case 12:
		/* Write current header to a tape */
		numbytes = make_header((char *)temp);

		/* get the tape drive name and open it */

		printf("OUTPUT TAPE DRIVE NAME NAME ?\n");
		gets(outname);
		printf("OUTPUT TAPE DRIVE NAME: %s\n",outname);
		fp_out = fopen(outname,"wb");
		/* Open tape drive for writing binary */
		if(fp_out == NULL)
		  {
		      printf("CANNOT OPEN output tape drive\n");
		      printf("REASON: %s\n",sys_errlist[errno]);       
		      exit(1); /* If cannot open, exit */
		  }
		setbuf(fp_out,NULL);

		/* Write out the volume header */
		items = fwrite(temp,numbytes,1,fp_out);
		if(items != 1)
		  {
		      printf("Error on header write items = %d\n",items);
		      exit(1);
		  }
		close(fp_out);

		break;


	}/*switch*/
   }/*for*/
}/*end print_menu_printer*/


/****************************************************************************/
/*   SUBROUTINE TO PRINT CELL SPACING DESCRIPTOR                            */
/***************************************************************************/
void fprint_cellspacing(cellspacing_pntr, print_pntr)
      struct cell_spacing_d *cellspacing_pntr;    
      FILE *print_pntr;
  {
    int i;
    fprintf(print_pntr,"\n\n********** CELL SPACING DESCRIPTOR ***************\n");
    fprintf(print_pntr,"IDENTIFIER        : %c%c%c%c\n",
	    cellspacing_pntr->cell_spacing_des[0],
	    cellspacing_pntr->cell_spacing_des[1],
	    cellspacing_pntr->cell_spacing_des[2],
	    cellspacing_pntr->cell_spacing_des[3]);
    fprintf(print_pntr,"CELL SPACING DESCRIPTOR LENGTH : %d\n",
                          cellspacing_pntr->cell_spacing_des_len);
    fprintf(print_pntr,"NUMBER OF SEQMENTS THAT CONTAIN CELLS OF EQUAL WIDTHS : %d\n",
                          cellspacing_pntr->num_segments);
     fprintf(print_pntr,"DISTANCE FOR FIRST GATE(m)    : %d\n",
                          cellspacing_pntr->distToFirst);
     fprintf(print_pntr,"WIDTH OF CELLS IN EACH SEGMENT IN METERS              :\n");
     for(i = 0; i < 6; i++)
        fprintf(print_pntr," %d",cellspacing_pntr->spacing[i]);

     fprintf(print_pntr,"\nNUMBER OF CELL IN EACH SEGMENT\n");
     for(i = 0; i < 6; i++)
        fprintf(print_pntr," %d",cellspacing_pntr->num_cells[i]);
     fprintf(print_pntr,"\n\n");

 }/*end fprint_cellspacing*/


/*****************************************************************************/
/*         SUBROUTINE PRINT FIELD RADAR INFO TO PRINTER                      */
/*****************************************************************************/

void fprint_fieldradar(fradar_pntr,print_pntr)
      struct field_radar_i  *fradar_pntr;
      FILE *print_pntr;

{
   int i;
   
   fprintf(print_pntr,"\n\n************** FIELD RADAR INFO BLOCK *********\n");
   fprintf(print_pntr,"FIELD RADAR IDENTIFIER : ");
   for(i=0; i<4; i++)
     fprintf(print_pntr,"%c",fradar_pntr->field_radar_info[i]);
   fprintf(print_pntr,"\nFIELD RADAR LENGTH     : %d\n",
                         fradar_pntr->field_radar_info_len);
   fprintf(print_pntr,"DATA SYSTEM ID         : %d\n",
                          fradar_pntr->data_sys_id);
   fprintf(print_pntr,"LOSSES BETWEEN TRANSMITTER AND ANTENNA (dB):%f\n",
                          fradar_pntr->loss_out);
   fprintf(print_pntr,"LOSSES BETWEEN ANTENNA AND LOW NOISE AMP (dB):%f\n",
                           fradar_pntr->loss_in);
   fprintf(print_pntr,"LOSSES IN ROTARY JOINT (dB)             :%f\n",
                           fradar_pntr->loss_rjoint);
   fprintf(print_pntr,"ANTENNA VERTICAL DIMENSION (m)          :%f\n",
                           fradar_pntr->ant_v_dim);
   fprintf(print_pntr,"ANTENNA HORIZONTAL DIMENSION (m)        :%f\n",
                           fradar_pntr->ant_h_dim);
   fprintf(print_pntr,"ANTENNA NOISE TEMPERATURE (K)           :%f\n",
                          fradar_pntr->ant_noise_temp);
   fprintf(print_pntr,"RECEIVER NOISE FIGURE (dB)              :%f\n",
                          fradar_pntr->r_noise_figure);
   fprintf(print_pntr,"PEAK TRANSMIT POWER BY CHANNEL (dBm) \n");
   for(i = 0; i < 5; i++)
      fprintf(print_pntr,"%d = %f  ",i+1,fradar_pntr->xmit_power[i]);
   fprintf(print_pntr,"\n");
   fprintf(print_pntr,"X BAND GAIN (dB)                         :%f\n",
                          fradar_pntr->x_band_gain);
   fprintf(print_pntr,"RECEIVER GAIN BY CHANNEL (dB)\n");
   for(i = 0; i < 5; i++)
      fprintf(print_pntr,"%d = %f  ",i+1,fradar_pntr->receiver_gain[i]);
   fprintf(print_pntr,"\n");
   fprintf(print_pntr,"IF GAIN BY CHANNEL (dB)\n");
   for(i = 0; i < 5; i++)
      fprintf(print_pntr,"%d = %f  ",i+1,fradar_pntr->if_gain[i]);
   fprintf(print_pntr,"\n");
   fprintf(print_pntr,"A to D CONVERSION GAIN (dB)               :%f\n",
                          fradar_pntr->conversion_gain);
   fprintf(print_pntr,"SCALE FACTOR BY CHANNEL \n");
   for(i = 0; i < 5; i++)
      fprintf(print_pntr,"%d = %f  ",i+1,fradar_pntr->scale_factor[i]);
   fprintf(print_pntr,"\n");
   fprintf(print_pntr,"PROCESSOR CONSTANT (scaled dBZ)           :%f\n",
                          fradar_pntr->processor_const);
   fprintf(print_pntr,"TIME DELAY TWT TUBE TO ANTENNA (ns)       :%d\n",
                          fradar_pntr->dly_tube_antenna);
fprintf(print_pntr,"TIME DELAY (round trip) CHIP TO A TO D CONVERTER (ns):%d\n"
	,fradar_pntr->dly_rndtrip_chip_atod);
   fprintf(print_pntr,"TIME DELAY TIMING MOD TO TESTPULSE (ns)   :%d\n",
                          fradar_pntr->dly_timmod_testpulse);
   fprintf(print_pntr,"HPA MODULATOR RISE TIME (ns)            :%d\n",
                         fradar_pntr->dly_modulator_on);
   fprintf(print_pntr,"HPA MODULATOR FALL TIME (ns)           :%d\n",
                         fradar_pntr->dly_modulator_off);
   fprintf(print_pntr,"PEAK POWER METER OFFSET (dB)             :%f\n",
                         fradar_pntr->peak_power_offset);
   fprintf(print_pntr,"TEST PULSE POWER METER OFFSET (dB)       :%f\n",
                         fradar_pntr->test_pulse_offset);
   fprintf(print_pntr,"ANTENNA E PLANE ANGLE (deg) (AKA TILT)   :%f\n",
                         fradar_pntr->E_plane_angle);
   fprintf(print_pntr,"ANTENNA H PLANE ANGLE (deg)              :%f\n",
                         fradar_pntr->H_plane_angle);
   fprintf(print_pntr,"ENCODER ANGLE ANTENNA FACING UP (deg)    :%f\n",
                         fradar_pntr->encoder_antenna_up);
   fprintf(print_pntr,"ANTENNA PITCH ANGLE ANTENNA FACING UP (deg):%f\n",
                         fradar_pntr->pitch_antenna_up);
   fprintf(print_pntr,"INDEPENDENT FREQUENCY FLAG :%d\n",
                         fradar_pntr->indepf_times_flg);
   fprintf(print_pntr,"INDEPENDENT FREQUENCY GATE     : %d\n",
                         fradar_pntr->indep_freq_gate);
   fprintf(print_pntr,"TIME SERIES GATE               : %d\n",
                         fradar_pntr->time_series_gate);
   fprintf(print_pntr,"\n\n");

}

/*****************************************************************************/
/*              SUBROUTINE PRINT  INSITU DATA DESCRIPTOR                     */
/*****************************************************************************/


void fprint_insitu(insitu_pntr, print_pntr)
    struct  insitu_descript  *insitu_pntr;
    FILE *print_pntr;
  {
 
     int i;

     fprintf(print_pntr,"\n\nINSITU DATA DESCRIPTOR IDENTIFIER  :");
     for(i = 0; i < 4; i++){
         fprintf(print_pntr,"%c", insitu_pntr->insitu_descript_id[i]);
     }
     fprintf(print_pntr,"\n");
     fprintf(print_pntr,"INSITU DATA LENGTH  %d\n", 
                           insitu_pntr->insitu_descript_len);
     fprintf(print_pntr,"\n\n");

  }

/****************************************************************************/
/*   SUBROUTINE TO PRINT NAVIGATIONAL DESCRIPTOR                            */
/***************************************************************************/
void fprint_navigation(nav_pntr,print_pntr)
      struct nav_descript *nav_pntr;
      FILE *print_pntr;
  {
    int i;
     fprintf(print_pntr,"\n\n********* NAVIGATIONAL DATA DESCRIPTOR **********\n");
     fprintf(print_pntr,"Indentifier:     ");
     for(i = 0; i < 4; i++)
         fprintf(print_pntr,"%c",nav_pntr->nav_descript_id[i]);
     fprintf(print_pntr,"\nNAVIGATIONAL DATA DESCRIPTOR LENGTH : %d\n",
                          nav_pntr->nav_descript_len);
     fprintf(print_pntr,"INS FLAG     : %d\n",nav_pntr->ins_flag);
     fprintf(print_pntr,"GPS FLAG     : %d\n",nav_pntr->gps_flag);
     fprintf(print_pntr,"MINIRIMS FLAG: %d\n",
                                      nav_pntr->minirims_flag);
     fprintf(print_pntr,"KALMAN FLAG  : %d\n",
                                      nav_pntr->kalman_flag);
    fprintf(print_pntr,"\n\n");

 }/*end print_navigational_descriptor*/                          


/***************************************************************************/
/*   SUBROUTINE TO PRINT PARAMETER DESCRIPTOR                             */
/***************************************************************************/

void fprint_parameter(param_pntr,print_pntr)
    struct parameter_d  *param_pntr;
    FILE *print_pntr;

  {

    int i;

      fprintf(print_pntr,"\n\n**********PARAMETER DESCRIPTOR**********\n");
      fprintf(print_pntr,"PARAMETER DESCRIPTOR        : ");
      for(i=0; i<4; i++)
	fprintf(print_pntr,"%c",param_pntr->parameter_des[i]);
      fprintf(print_pntr,"\nPARAMETER DESCRIPTOR LENGTH : %d\n",
                  param_pntr->parameter_des_length);
      fprintf(print_pntr,"PARAMETER NAME              : ");
       for(i = 0; i < 8; i++)
           fprintf(print_pntr,"%c",
                  param_pntr->parameter_name[i]);
      fprintf(print_pntr,"\nPARAMETER DESCRIPTION       :\n");
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
       fprintf(print_pntr,"BAD DATA FLAG               : %d\n",
                       param_pntr->bad_data);
    fprintf(print_pntr,"\n\n");

}
                 
/**************************************************************************/
/*               SUBROUTINE PRINT RADAR DESCRIPTOR                        */
/**************************************************************************/

void fprint_radar(radar_pntr, print_pntr)
    struct radar_d *radar_pntr;
    FILE *print_pntr;
{
    int i;
    int element;
    fprintf(print_pntr,"\n\n******************* RADAR DESCRIPTOR ***********\n");
    fprintf(print_pntr,"RADAR DESCRIPTOR     :");
    for(i=0; i<4; i++)
      fprintf(print_pntr,"%c",radar_pntr->radar_des[i]);
    fprintf(print_pntr,"\nRADAR DESCRIPTOR SIZE: %d\n",
                    radar_pntr->radar_des_length);
    fprintf(print_pntr,"RADAR NAME           : ");
    for(i = 0; i < 8; i++)
        fprintf(print_pntr,"%c",radar_pntr->radar_name[i]);
    fprintf(print_pntr,"\nRADAR CONSTANT (db)    : %f\n",
                     radar_pntr->radar_const);
    fprintf(print_pntr,"PEAK POWER (kw)        : %f\n",
                     radar_pntr->peak_power);
    fprintf(print_pntr,"NOISE POWER (dBm)      : %f\n",
                     radar_pntr->noise_power);
    fprintf(print_pntr,"RECIEVER GAIN (dB)     : %f\n",
                     radar_pntr->receiver_gain);
    fprintf(print_pntr,"ANTENNA GAIN (dB)      : %f\n",
                     radar_pntr->antenna_gain);
    fprintf(print_pntr,"SYSTEM GAIN (dB)       : %f\n",
                     radar_pntr->system_gain);
    fprintf(print_pntr,"HORIZONTAL BEAM WIDTH (deg): %f\n",
                     radar_pntr->horz_beam_width);
    fprintf(print_pntr,"VERTICAL BEAM WIDTH (deg)  : %f\n",
                     radar_pntr->vert_beam_width);
    fprintf(print_pntr,"RADAR TYPE           : %d\n",
                     radar_pntr->radar_type);
    fprintf(print_pntr,"SCAN MODE            : %d\n",
                     radar_pntr->scan_mode);
    fprintf(print_pntr,"ROTATIONAL VELOCITY (deg/s): %f\n",
                     radar_pntr->req_rotat_vel);
    fprintf(print_pntr,"SCAN MODE PARAMETER #0: %f\n",
                      radar_pntr->scan_mode_pram0);
    fprintf(print_pntr,"SCAN MODE PARAMETER #1: %f\n",
                      radar_pntr->scan_mode_pram1);
    fprintf(print_pntr,"NUMBER OF PARAMETER DESCRIPTOR BLOCKS :%d\n",
                                 radar_pntr->num_parameter_des);
    fprintf(print_pntr,"TOTAL NUMBER OF DESCRIPTOR BLOCKS :%d\n",
                                 radar_pntr->total_num_des);
    fprintf(print_pntr,"DATA COMPRESSION SCHEME               : %d\n",
                                 radar_pntr->data_compress);
    fprintf(print_pntr,"DATA REDUCTION ALGORITHM IN USE        : %d\n",
                                 radar_pntr->data_reduction);
    fprintf(print_pntr,"REDUCTION ALGORITHM SPECIFIC PARAMETER 0: %f\n",
                                 radar_pntr->data_red_parm0);
    fprintf(print_pntr,"REDUCTION ALGORITHM SPECIFIC PARAMETER 1: %f\n",
                                 radar_pntr->data_red_parm1);
    fprintf(print_pntr,"LATITUDE (deg) :%f\n",radar_pntr->radar_latitude);
    fprintf(print_pntr,"LONGITUDE (deg):%f\n",radar_pntr->radar_longitude);
    fprintf(print_pntr,"ALTITUDE (m)   :%f\n",radar_pntr->radar_altitude);
    fprintf(print_pntr,"EFFECTIVE UNAMBIGIOUS VELOCITY (m/s)    : %f\n",
                                 radar_pntr->eff_unamb_vel);
    fprintf(print_pntr,"EFFECTIVE UNAMBIGIOUS RANGE (km)        : %f\n",
                                 radar_pntr->eff_unamb_range);
    fprintf(print_pntr,"NUMBER OF TRANSMITTED FREQUENCIES       : %d\n",
                                 radar_pntr->num_freq_trans);
    fprintf(print_pntr,"NUMBER OF DIFFERENT INTER-PULSE PERIODS : %d\n",
                                 radar_pntr->num_ipps_trans);
    fprintf(print_pntr,"FREQUENCY 1 (GHz)    : %f\n",radar_pntr->freq1);
    fprintf(print_pntr,"FREQUENCY 2 (GHz)    : %f\n",radar_pntr->freq2);
    fprintf(print_pntr,"FREQUENCY 3 (GHz)    : %f\n",radar_pntr->freq3);
    fprintf(print_pntr,"FREQUENCY 4 (GHz)    : %f\n",radar_pntr->freq4);
    fprintf(print_pntr,"FREQUENCY 5 (GHz)    : %f\n",radar_pntr->freq5);
 fprintf(print_pntr,"INTERPULSE PERIOD 1 (ms): %f\n",
	 radar_pntr->interpulse_per1);
 fprintf(print_pntr,"INTERPULSE PERIOD 2 (ms): %f\n",
	 radar_pntr->interpulse_per2);
 fprintf(print_pntr,"INTERPULSE PERIOD 3 (ms): %f\n",
	 radar_pntr->interpulse_per3);
 fprintf(print_pntr,"INTERPULSE PERIOD 4 (ms): %f\n",
	 radar_pntr->interpulse_per4);
 fprintf(print_pntr,"INTERPULSE PERIOD 5 (ms): %f\n",
	 radar_pntr->interpulse_per5);
    fprintf(print_pntr,"\n\n");

}/*end print radar descriptor */

/**************************************************************************/
/*             SUBROUTINE fPRINT VOLUME DESCRIPTOR                        */
/**************************************************************************/
void fprint_volume(vol_pntr, print_pntr)
      struct volume_d *vol_pntr;
      FILE *print_pntr;
    {
     int i;

 fprintf(print_pntr,"\n\n************VOLUME DESCRIPTOR************************\n");
    fprintf(print_pntr,"VOLUME IDENTIFIER          :");
    for(i=0; i<4; i++)
      fprintf(print_pntr,"%c",vol_pntr->volume_des[i]);
    fprintf(print_pntr,"\nVOLUME DESCRIPTOR LENGTH : %d\n",
                    vol_pntr->volume_des_length);
    fprintf(print_pntr,"FORMAT VERSION NUMBER IS :%d\n",
                    vol_pntr->format_version);
    fprintf(print_pntr,"VOLUME NUMBER IS         :%d\n",
                    vol_pntr->volume_num);
    fprintf(print_pntr,"MAXIMUM NUMBER OF BYTES IN DATA RECORD IS  : %d\n",
                    vol_pntr->
           maximum_bytes);
    fprintf(print_pntr,"PROJECT NAME             : ");
    for(i = 0; i < 20; i++) fprintf(print_pntr,"%c",vol_pntr->proj_name[i]);
    fprintf(print_pntr,"\nDATE  %2d/%2d/%4d (dy/mn/yr)\n",
               vol_pntr->day,vol_pntr->month,vol_pntr->year);
    fprintf(print_pntr,"TIME  %d  : %d : %d\n",vol_pntr->data_set_hour,
                        vol_pntr->data_set_minute,
                        vol_pntr->data_set_second);
    fprintf(print_pntr,"FLIGHT NUMBER            : ");
    for(i = 0; i < 8; i++) fprintf(print_pntr,"%c",vol_pntr->flight_num[i]);
    fprintf(print_pntr,"\nGENERATION FACILITY NAME : ");
    for(i = 0; i < 8; i++) 
          fprintf(print_pntr,"%c",vol_pntr->gen_facility[i]);
    fprintf(print_pntr,"\nVOLUME GENERATION DATE   : %2d/%2d/%4d (dy/mn/yr)\n",
           vol_pntr->gen_day,vol_pntr->gen_month,vol_pntr->gen_year);
    fprintf(print_pntr,"NUMBER OF SENSOR DESCRIPTORS: %d\n",
            vol_pntr->number_sensor_des);
     fprintf(print_pntr,"\n\n");

    }/*end print_volume_descriptor*/


/**************************************************************************/
/*               SUBROUTINE FPRINT WAVEFORM DESCRIPTOR                     */
/**************************************************************************/

void fprint_waveform(waveform_pntr, print_pntr)
    struct waveform_d *waveform_pntr;
    FILE *print_pntr;
  {
    int i,k;

   fprintf(print_pntr,"\n\n***************WAVEFORM DESCRIPTOR*****************\n");
     fprintf(print_pntr,"WAVEFORM IDENTIFIER        :");
     for(i=0; i<4; i++)
       fprintf(print_pntr,"%c",waveform_pntr->waveform_des[i]);
     fprintf(print_pntr,"\nWAVEFORM DESCRIPTOR LENGTH : %d\n",
             waveform_pntr->waveform_des_length);
     fprintf(print_pntr,"PULSING SCHEME FILE NAME   : ");
     for(i = 0; i < 16; i++)
        fprintf(print_pntr,"%c",waveform_pntr->ps_file_name[i]);
     fprintf(print_pntr,"\nNUMBER OF CHIPS IN A REPEAT:\n");
     for(i = 0; i< 6; i++) 
        fprintf(print_pntr," %d",waveform_pntr->num_chips[i]);
      fprintf(print_pntr,"\nBLANKING RAM SEQUENCE      :\n");
         k = 0;
          do{
             fprintf(print_pntr,"%3d: ",k);      
             for(i=0; i<10; i++)
	       {
		   fprintf(print_pntr,"%2x ",waveform_pntr->blank_chip[k]);
		   k++;
	       }
	     fprintf(print_pntr,"\n");
	 }while(k < waveform_pntr->total_pcp);

      fprintf(print_pntr,"\nNUMBER OF MILLISECONDS PER REPEAT : %f\n",
              waveform_pntr->repeat_seq);
      fprintf(print_pntr,"NUMBER OF REPEAT SEQUENCES IN A DWELL TIME: %d\n",
		 waveform_pntr->repeat_seq_dwel);
      fprintf(print_pntr,"TOTAL NUMBER OF PCP IN A REPEAT SEQUENCE  : %d\n",
                 waveform_pntr->total_pcp);
      fprintf(print_pntr,"CHIP OFFSET (60 MHz counts)       :\n");
       for(i = 0; i < 6; i++)  
         fprintf(print_pntr," %d",waveform_pntr->chip_offset[i]);
      fprintf(print_pntr,"\nCHIP WIDTH (60 MHz counts)\n");
      for(i = 0; i < 6; i++)
         fprintf(print_pntr," %d",waveform_pntr->chip_width[i]);
      fprintf(print_pntr,"\nPCP THAT SET THE UNAMBIGIOUS RANGE : %f\n",
                                     waveform_pntr->ur_pcp);
      fprintf(print_pntr,"PCP THAT SET THE UNAMBIGIOUS VELOCITY : %f\n",
                                     waveform_pntr->uv_pcp);
      fprintf(print_pntr,"TOTAL NUMBER OF SAMPLED GATES:\n");
      for(i = 0; i < 5; i++) 
          fprintf(print_pntr," %d",waveform_pntr->num_gates[i]);
   fprintf(print_pntr,"\n");
   fprintf(print_pntr,"GATE DISTANCE 1 (60 MHz Counts): %d %d\n",
	   waveform_pntr->gate_dist1[0],waveform_pntr->gate_dist1[1]);
   fprintf(print_pntr,"GATE DISTANCE 2 (60 MHz Counts): %d %d\n",
	   waveform_pntr->gate_dist2[0],waveform_pntr->gate_dist2[1]);
   fprintf(print_pntr,"GATE DISTANCE 3 (60 MHz Counts): %d %d\n",
	   waveform_pntr->gate_dist3[0],waveform_pntr->gate_dist3[1]);
   fprintf(print_pntr,"GATE DISTANCE 4 (60 MHz Counts): %d %d\n",
	   waveform_pntr->gate_dist4[0],waveform_pntr->gate_dist4[1]);
   fprintf(print_pntr,"GATE DISTANCE 5 (60 MHz Counts): %d %d\n",
	   waveform_pntr->gate_dist5[0],waveform_pntr->gate_dist5[1]);
   fprintf(print_pntr,"\n\n");

  }/*end fprint_waveform */

/*********************************************************************/
/******   Subroutine to read a header from a disk file ***************/
/*********************************************************************/ 

void read_header()
 {
  
     int bytesread, bytestoread, i;
     char inname[80];
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
     bytestoread = sizeof(struct field_radar_i);
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
     bytestoread = sizeof(struct field_radar_i);
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
     bytesread = fread(&insitu,bytestoread,1,filepntr);
     if(bytesread <= 0){
        printf("EOF or bad read on insitu data descriptor\n");
       return;
     }

     close(filepntr);
 }


/*********************************************************************/
/**   Subroutine to copy a header from an array to various stuctures */
/*********************************************************************/ 

void copy_header(char *array)
 {
  
     int bytestocopy, i, j;


     bytestocopy = sizeof(volume);
     for(i=0; i < bytestocopy; i++)
       {
	   volume.volume_des[i] = *array;
	   array++;
       }

     bytestocopy = sizeof(waveform);
     for(i=0; i < bytestocopy; i++)
       {
	   waveform.waveform_des[i] = *array;
	   array++;
       }


/*** copy the fore radar information ***/

     bytestocopy = sizeof(struct radar_d);
     for(i=0; i < bytestocopy; i++)
       {
	   radar[FORE].radar_des[i] = *array;
	   array++;
       }

     bytestocopy = sizeof(struct field_radar_i);
     for(i=0; i < bytestocopy; i++)
       {
	   fradar[FORE].field_radar_info[i] = *array;
	   array++;
       }

     bytestocopy = sizeof(struct cell_spacing_d);
     for(i=0; i < bytestocopy; i++)
       {
	   cell[FORE].cell_spacing_des[i] = *array;
	   array++;
       }

     actual_num_radars = 2;
     actual_num_params[FORE] = radar[FORE].num_parameter_des;
     printf("Fore radar contains %d parameters\n",actual_num_params[FORE]);

     bytestocopy = sizeof(struct parameter_d);

     for(j=0; j<actual_num_params[FORE]; j++)
       {
     for(i=0; i < bytestocopy; i++)
       {
	   parameter[j][FORE].parameter_des[i] = *array;
	   array++;
       }
       }

/*** copy the aft radar information ***/

     bytestocopy = sizeof(struct radar_d);
     for(i=0; i < bytestocopy; i++)
       {
	   radar[AFT].radar_des[i] = *array;
	   array++;
       }

     bytestocopy = sizeof(struct field_radar_i);
     for(i=0; i < bytestocopy; i++)
       {
	   fradar[AFT].field_radar_info[i] = *array;
	   array++;
       }

     bytestocopy = sizeof(struct cell_spacing_d);
     for(i=0; i < bytestocopy; i++)
       {
	   cell[AFT].cell_spacing_des[i] = *array;
	   array++;
       }

     actual_num_radars = 2;
     actual_num_params[AFT] = radar[AFT].num_parameter_des;
     printf("Aft radar contains %d parameters\n",actual_num_params[AFT]);

     bytestocopy = sizeof(struct parameter_d);

     for(j=0; j<actual_num_params[AFT]; j++)
       {
     for(i=0; i < bytestocopy; i++)
       {
	   parameter[j][AFT].parameter_des[i] = *array;
	   array++;
       }
       }

/*** copy the navigational and insitu data descriptors ***/

     bytestocopy = sizeof(struct nav_descript);
     for(i=0; i < bytestocopy; i++)
       {
	   navigation.nav_descript_id[i] = *array;
	   array++;
       }

     bytestocopy = sizeof(struct insitu_descript);
     for(i=0; i < bytestocopy; i++)
       {
	   insitu.insitu_descript_id[i] = *array;
	   array++;
       }

 }



/****************************************************************************/
/**   Subroutine to copy the various header stuctures to a continuous array */
/****************************************************************************/ 

int make_header(char *array)
 {
  
     int bytestocopy, i, j, totalbytes;

     totalbytes = 0;

     bytestocopy = sizeof(volume);
     for(i=0; i < bytestocopy; i++)
       {
	   *array = volume.volume_des[i];
	   totalbytes +=1;
	   array++;
       }

     bytestocopy = sizeof(waveform);
     for(i=0; i < bytestocopy; i++)
       {
	   *array = waveform.waveform_des[i];
	   totalbytes +=1;
	   array++;
       }


/*** copy the fore radar information ***/

     bytestocopy = sizeof(struct radar_d);
     for(i=0; i < bytestocopy; i++)
       {
	   *array = radar[FORE].radar_des[i];
	   totalbytes +=1;
	   array++;
       }

     bytestocopy = sizeof(struct field_radar_i);
     for(i=0; i < bytestocopy; i++)
       {
	   *array = fradar[FORE].field_radar_info[i];
	   totalbytes +=1;
	   array++;
       }

     bytestocopy = sizeof(struct cell_spacing_d);
     for(i=0; i < bytestocopy; i++)
       {
	   *array = cell[FORE].cell_spacing_des[i];
	   totalbytes +=1;
	   array++;
       }

     actual_num_radars = 2;
     actual_num_params[FORE] = radar[FORE].num_parameter_des;
     printf("Fore radar contains %d parameters\n",actual_num_params[FORE]);

     bytestocopy = sizeof(struct parameter_d);

     for(j=0; j<actual_num_params[FORE]; j++)
       {
     for(i=0; i < bytestocopy; i++)
       {
	   *array = parameter[j][FORE].parameter_des[i];
	   totalbytes +=1;
	   array++;
       }
       }

/*** copy the aft radar information ***/

     bytestocopy = sizeof(struct radar_d);
     for(i=0; i < bytestocopy; i++)
       {
	   *array = radar[AFT].radar_des[i];
	   totalbytes +=1;
	   array++;
       }

     bytestocopy = sizeof(struct field_radar_i);
     for(i=0; i < bytestocopy; i++)
       {
	   *array = fradar[AFT].field_radar_info[i];
	   totalbytes +=1;
	   array++;
       }

     bytestocopy = sizeof(struct cell_spacing_d);
     for(i=0; i < bytestocopy; i++)
       {
	   *array = cell[AFT].cell_spacing_des[i];
	   totalbytes +=1;
	   array++;
       }

     actual_num_radars = 2;
     actual_num_params[AFT] = radar[AFT].num_parameter_des;
     printf("Aft radar contains %d parameters\n",actual_num_params[AFT]);

     bytestocopy = sizeof(struct parameter_d);

     for(j=0; j<actual_num_params[AFT]; j++)
       {
     for(i=0; i < bytestocopy; i++)
       {
	   *array = parameter[j][AFT].parameter_des[i];
	   totalbytes +=1;
	   array++;
       }
       }

/*** copy the navigational and insitu data descriptors ***/

     bytestocopy = sizeof(struct nav_descript);
     for(i=0; i < bytestocopy; i++)
       {
	   *array = navigation.nav_descript_id[i];
	   totalbytes +=1;
	   array++;
       }

     bytestocopy = sizeof(struct insitu_descript);
     for(i=0; i < bytestocopy; i++)
       {
	   *array = insitu.insitu_descript_id[i];
	   totalbytes +=1;
	   array++;
       }

     return(totalbytes);

 }

