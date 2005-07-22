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
#include <signal.h>

struct BEAM{
    struct ray_i rray;
    struct platform_i rplat;
    struct field_parameter_data rdata;
    short data[200];
};

struct HEAD{
    struct volume_d vol;
    struct waveform_d wave;
    struct radar_d fradar;
    struct field_radar_i ffradar;
    struct cell_spacing_d fcell;
    struct parameter_d fparam[4];
    struct radar_d aradar;
    struct field_radar_i afradar;
    struct cell_spacing_d acell;
    struct parameter_d aparam[4];
};


/* Global parameters used by the print routines */
int hr, min, sec, millsec;
short volume_num, day, month, year;
char flight_num[8], proj_name[20];
float dwelltime, rpm;
char ps_file_name[16];
int cells, gates, ipps, freqs, num_params, eof_detected;

/* Global variables to close file on a control C */
FILE *fp_out, *printer, *popen();
char file_onoff[3], print_onoff[3];
int fp_in;

#define MAX_BUFF 32768

void main(void);
void print_start(FILE *ptr);
void print_end(FILE *ptr);
void print_ray(FILE *ptr, struct BEAM *rptr);
void cleanup(void);

void main()
{

struct BEAM *beam;
struct HEAD *head;

extern char *sys_errlist[];
int temp[MAX_BUFF],ray_count;
int i,j,index,amount,count;
int first, stop, time, raycount;
int old_aft_sweep, old_fore_sweep, input, alert;
char fore_onoff[3], aft_onoff[3], filename[80];
char time_onoff[3];
int old_time, interval;

union
  {
      int ryib_search;
      char ryib_array[4];
  }aryib;

union
  {
      int vol_search;
      char vol_array[4];
  }ahead;

signal(SIGINT, cleanup);

old_aft_sweep = -1;
old_fore_sweep = -1;
raycount = 0;
eof_detected = 0;

aryib.ryib_array[0] = 'R';
aryib.ryib_array[1] = 'Y';
aryib.ryib_array[2] = 'I';
aryib.ryib_array[3] = 'B';

ahead.vol_array[0] = 'V';
ahead.vol_array[1] = 'O';
ahead.vol_array[2] = 'L';
ahead.vol_array[3] = 'D';

first = 1;

file_onoff[0] = 'o';
file_onoff[1] = 'f';
file_onoff[2] = 'f';

print_onoff[0] = 'o';
print_onoff[1] = 'f';
print_onoff[2] = 'f';

fore_onoff[0] = 'o';
fore_onoff[1] = 'f';
fore_onoff[2] = 'f';

aft_onoff[0] = 'o';
aft_onoff[1] = 'f';
aft_onoff[2] = 'f';

time_onoff[0] = 'o';
time_onoff[1] = 'f';
time_onoff[2] = 'f';



/* Query the user for the style of the desired catalog */
do{
    printf("\nEnter the number in front of your choice\n");
    printf("0) Begin catalog of the tape\n");
    printf("1) Record catalog to a data file (%c%c%c)\n",
	   file_onoff[0],file_onoff[1],file_onoff[2]);
    printf("2) Print catalog to the printer (%c%c%c)\n",
	   print_onoff[0],print_onoff[1],print_onoff[2]);
    printf("3) Catalog fore radar sweeps (%c%c%c)\n",
	   fore_onoff[0],fore_onoff[1],fore_onoff[2]);
    printf("4) Catalog aft radar sweeps (%c%c%c)\n",
	   aft_onoff[0],aft_onoff[1],aft_onoff[2]);
    printf("5) Catalog at a fixed time interval (%c%c%c)\n",
	   time_onoff[0],time_onoff[1],time_onoff[2]);

    scanf(" %1d",&input);

    switch(input)
      {
	case 1:
	  printf("Enter the name of the data file to write to: ");
	  getchar();
	  gets(filename);
	  if((fp_out = fopen(filename,"w")) == NULL)
	    printf("Unable to open file: %s",filename);
	  else
	    {
		file_onoff[1] = 'n';
		file_onoff[2] = ' ';
	    }
	  break;
	case 2:
	  if(( printer = popen("lp","w")) == NULL)
	    printf("Unable to access printer\n");
	  else
	    {
		print_onoff[1] = 'n';
		print_onoff[2] = ' ';
	    }
	  break;
	case 3:
	  fore_onoff[1] = 'n';
	  fore_onoff[2] = ' ';
	  break;
	case 4:
	  aft_onoff[1] = 'n';
	  aft_onoff[2] = ' ';
	  break;
	case 5:
	  printf("Enter the interval in seconds: ");
	  scanf(" %d",&interval);
	  if(interval < 1 || interval > 1000) interval = 30;
	  time_onoff[1] = 'n';
	  time_onoff[2] = ' ';
	  break;

	default:
	  break;
      }

}while(input > 0);

alert = 1;
if(fore_onoff[1] == 'n' || aft_onoff[1] == 'n'|| time_onoff[1] == 'n')
  alert = 0;

/* Open The Input Tape Drive */

fp_in = open("/dev/rmt/0bn", O_RDONLY); /* Open tape drive for binary read */
if(fp_in<0)
{
	printf("CANNOT OPEN input tape drive\n");
	printf("REASON: %s\n",sys_errlist[errno]);       
	exit(1); /* If cannot open, exit */
}

/* Get the stop time */

printf("\nEnter the stop time as HH MM SS\n");
printf("Enter 23 59 59 for entire tape: ");
scanf("%2d %2d %2d",&hr,&min,&sec);
stop = sec + 60 * (min + 60 * hr);
time = 0;
hr = 0;
min = 0;
sec = 0;
millsec = 0;

printf("\nWill begin reading tape now. This may take awhile to get started\n");
printf("Depressing ctrl-c at any time will lead to a controlled shutdown\n");

/* Begin Reading Tape */
do{

if(((amount = read(fp_in,temp,MAX_BUFF)) > 0) && time < stop)
{
    index=0;
    eof_detected = 0;

    /* Did we read a volume header? */

    if(temp[index]==ahead.vol_search)
      {
	  if(first == 1)
	    {
		head = (struct HEAD *)&temp[index];

		/* Save description of this header in global variables for
		   use by the printing routines */

		volume_num = head->vol.volume_num;
		day = head->vol.day;
		month = head->vol.month;
		year = head->vol.year;
		for(i=0; i<8; i++)
		  flight_num[i] = head->vol.flight_num[i];
		for(i=0; i<20; i++)
		  proj_name[i] = head->vol.proj_name[i];
		dwelltime = head->wave.repeat_seq_dwel*head->wave.repeat_seq;
		rpm = head->fradar.req_rotat_vel*60.0/360.0;
		for(i=0; i<16; i++)
		  ps_file_name[i] = head->wave.ps_file_name[i];
		cells = 0;
		for(i=0; i<head->fcell.num_segments; i++)
		  cells += head->fcell.num_cells[i];
		gates = head->wave.num_gates[0]; 
		ipps = head->fradar.num_ipps_trans; 
		freqs = head->fradar.num_freq_trans;
		num_params = head->fradar.num_parameter_des;
	    }
	  if(first == 0)
	    {
		print_end(stdout);
		if(file_onoff[1] == 'n')
		  print_end(fp_out);
		if(print_onoff[1] == 'n')
		  print_end(printer);
	    first = 1;
	    }
	  continue;
      }

    while(index < amount/4)
      {
	  /* Did we find the start of a ray info block? */

       	if(temp[index] == aryib.ryib_search)
	  {
	      raycount++;
	      beam = (struct BEAM *)&temp[index];
	      temp[index] = 0;    /* Clear out RYIB for next time */

	      hr = beam->rray.hour;
	      min = beam->rray.minute;
	      sec = beam->rray.second;
	      millsec = beam->rray.millisecond;
	      time = beam->rray.second + 60 * (beam->rray.minute +
					       60 * beam->rray.hour);
	      
	      if(raycount > 2000 && alert)
		{
		    printf("Still Processing At Time: %2d:%2d:%2d:%3d\n",
			   hr,min,sec,millsec);
		    raycount = 0;
		}

	      if(first == 1)
		{
		    old_time = time;
		    print_start(stdout);
		    if(file_onoff[1] == 'n')
		      print_start(fp_out);
		    if(print_onoff[1] == 'n')
		      print_start(printer);
		    first = 0;
		}

	      /* if we are doing time interval, need to check that */
	      if(time_onoff[1] == 'n')
		{
		    if(time - old_time >= interval)
		      {
			  old_time = time;
			  print_ray(stdout,beam);
			  if(file_onoff[1] == 'n')
			    print_ray(fp_out,beam);
			  if(print_onoff[1] == 'n')
			    print_ray(printer,beam);
		      }
		}


	      /* If we are doing sweep numbers then we need to check for
		 a change in the desired sweep number */

	      if(beam->rdata.radar_name[0] == 'F')
		{
		    if(fore_onoff[1] == 'n')
		      {
			  if(beam->rray.sweep_num > old_fore_sweep)
			    {
				old_fore_sweep = beam->rray.sweep_num;
				print_ray(stdout,beam);
				if(file_onoff[1] == 'n')
				  print_ray(fp_out,beam);
				if(print_onoff[1] == 'n')
				  print_ray(printer,beam);
			    }
		      }
		}

	      if(beam->rdata.radar_name[0] == 'A')
		{
		    if(aft_onoff[1] == 'n')
		      {
			  if(beam->rray.sweep_num > old_aft_sweep)
			    {
				old_aft_sweep = beam->rray.sweep_num;
				print_ray(stdout,beam);
				if(file_onoff[1] == 'n')
				  print_ray(fp_out,beam);
				if(print_onoff[1] == 'n')
				  print_ray(printer,beam);
			    }
		      }
		}

	     }
	     index++;
	}/* WHILE(index < amount/4) */

}  /* amount > 0 and time < stop */

/* Read the end-of-file on the start of the tape */ 

if(amount==0)
{
    printf("FOUND END OF FILE MARKER at %2d:%2d:%2d\n",hr,min,sec);
    if(eof_detected)
      {
	  printf("Two EOFs detected in a row, end of tape reached\n");
	  cleanup();
      }
    eof_detected = 1;
}

}while(time < stop);

printf("END TIME REACHED (%2d:%2d:%2d) CLOSING DRIVE\n",hr,min,sec);
cleanup();

}
void print_start(FILE *ptr)
{
fprintf(ptr,"Start of Volume %d at %2d:%2d:%2d:%3d on %2d/%2d/%4d (dd/mm/yr)\n",
	volume_num,hr,min,sec,millsec,day,month,year);
fprintf(ptr," Flight #: %c%c%c%c%c%c%c%c",flight_num[0],flight_num[1],
	flight_num[2], flight_num[3], flight_num[4], flight_num[5], 
	flight_num[6], flight_num[7]);
proj_name[19] = 0;
fprintf(ptr,"  Project: %s\n",proj_name);
fprintf(ptr," RVel: %4.1f rpm  Cells: %4d  Gates: %4d  Num Params: %4d",
	rpm,cells,gates,num_params);
fprintf(ptr," Dwelltime: %4.1f ms\n",dwelltime);
ps_file_name[15] = 0;
fprintf(ptr," Num IPPs: %2d  Num Freqs: %2d  Scheme: %s\n",
	ipps,freqs,ps_file_name);


}
void print_end(FILE *ptr)
{
fprintf(ptr," End of Volume %d at %2d:%2d:%2d:%3d\n",
	volume_num,hr,min,sec,millsec);
}
void print_ray(FILE *ptr, struct BEAM *rptr)
{
fprintf(ptr," Sweep number %5d of %c%c%c%c radar at %2d:%2d:%2d:%3d\n",
	rptr->rray.sweep_num, rptr->rdata.radar_name[0],
	rptr->rdata.radar_name[1], rptr->rdata.radar_name[2],
	rptr->rdata.radar_name[3], hr,min,sec,millsec);
fprintf(ptr," Position: %7.2f %7.2f %7.2f Heading: %7.2f\n",
	rptr->rplat.latitude, rptr->rplat.longitude, rptr->rplat.altitude_msl,
	rptr->rplat.heading);
}
void cleanup()
{
printf("\nPerforming controlled shutdown\n");

close(fp_in);
if(file_onoff[1] == 'n') fclose(fp_out);
if(print_onoff[1] == 'n') pclose(printer);
exit(2);

}
