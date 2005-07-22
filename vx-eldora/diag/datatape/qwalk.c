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
short volume_num;

/* Global variables to close file on a control C */
FILE *fp_out, *printer, *popen();
char file_onoff[3], print_onoff[3];
int fp_in;

#define MAX_BUFF 32768

void main(void);
void print_start(FILE *);
void print_end(FILE *);
void print_bad_start(FILE *, char *);
void print_bad_id(FILE *, char *, char *);
void cleanup(void);
int temp[MAX_BUFF], index;

void main()
{

struct BEAM *beam;
struct HEAD *head;

extern char *sys_errlist[];
int i,j,amount;
int first, stop, present_time, raycount;
int input, indepf_times_flg, eof_detected;
char fore_onoff[3], aft_onoff[3], filename[80];
char pause_onoff[3], yesno;

union
  {
      int search;
      char array[4];
  }ryib;

union
  {
      int search;
      char array[4];
  }tempu;

union
  {
      int search;
      char array[4];
  }vol;

union
  {
      int search;
      char array[4];
  }asib;

union
  {
      int search;
      char array[4];
  }frad;

union
  {
      int search;
      char array[4];
  }indf;

union
  {
      int search;
      char array[4];
  }timeu;

signal(SIGINT, cleanup);

raycount = 0;

timeu.array[0] = 'T';
timeu.array[1] = 'I';
timeu.array[2] = 'M';
timeu.array[3] = 'E';

indf.array[0] = 'I';
indf.array[1] = 'N';
indf.array[2] = 'D';
indf.array[3] = 'F';

frad.array[0] = 'F';
frad.array[1] = 'R';
frad.array[2] = 'A';
frad.array[3] = 'D';

asib.array[0] = 'A';
asib.array[1] = 'S';
asib.array[2] = 'I';
asib.array[3] = 'B';

ryib.array[0] = 'R';
ryib.array[1] = 'Y';
ryib.array[2] = 'I';
ryib.array[3] = 'B';

vol.array[0] = 'V';
vol.array[1] = 'O';
vol.array[2] = 'L';
vol.array[3] = 'D';

first = 1;
eof_detected = 0;

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

pause_onoff[0] = 'o';
pause_onoff[1] = 'f';
pause_onoff[2] = 'f';



/* Query the user for the style of the desired catalog */
do{
    printf("\nEnter the number in front of your choice\n");
    printf("0) Begin quick walk of the tape\n");
    printf("1) Record quick walk to a data file (%c%c%c)\n",
	   file_onoff[0],file_onoff[1],file_onoff[2]);
    printf("2) Print quick walk to the printer (%c%c%c)\n",
	   print_onoff[0],print_onoff[1],print_onoff[2]);
    printf("3) Pause at every error (%c%c%c)\n",
	   pause_onoff[0],pause_onoff[1],pause_onoff[2]);

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
	  pause_onoff[1] = 'n';
	  pause_onoff[2] = ' ';
	  break;

	default:
	  break;
      }

}while(input > 0);

/* Open The Input Tape Drive */

fp_in = open("/dev/rmt/0b", O_RDONLY); /* Open tape drive for binary read */
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
present_time = 0;
hr = 0;
min = 0;
sec = 0;
millsec = 0;

printf("\nWill begin reading tape now. This may take awhile to get started\n");
printf("Depressing ctrl-c at any time will lead to a controlled shutdown\n");

/* Begin Reading Tape */
do{

if(((amount = read(fp_in,temp,MAX_BUFF)) > 0) && present_time < stop)
{
    index=0;
    eof_detected = 0;

    /* Did we read a volume header? */

    if(temp[index]==vol.search)
      {
	  if(first == 1)
	    {
		head = (struct HEAD *)&temp[index];

		/* Save description of this header in global variables for
		   use by the printing routines */

		volume_num = head->vol.volume_num;
		indepf_times_flg = head->ffradar.indepf_times_flg;
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
	  index = amount;
	  continue;
      }

/** All records should start with a RYIB or a VOLD ***/

    if(temp[0] != ryib.search && temp[0] != vol.search)
      {
	  tempu.search = temp[0];
	  print_bad_start(stdout,tempu.array);
	  if(file_onoff[1] == 'n')
	    print_bad_start(fp_out,tempu.array);
	  if(print_onoff[1] == 'n')
	    print_bad_start(printer,tempu.array);
	  if(pause_onoff[1] == 'n')
	    {
		printf("Pausing after detected error, continue? (y,n) ");
		scanf(" %c",&yesno);
		if(yesno != 'y') exit(1);
	    }
	  index = amount;
	  continue;
      }
  

    while(index < amount/4)
      {
	  /* Index should be pointing to a ray info block */

       	if(temp[index] != ryib.search)
	  {
	      tempu.search = temp[index];
	      print_bad_id(stdout,ryib.array,tempu.array);
	      if(file_onoff[1] == 'n')
		print_bad_id(fp_out,ryib.array,tempu.array);
	      if(print_onoff[1] == 'n')
		print_bad_id(printer,ryib.array,tempu.array);
	      if(pause_onoff[1] == 'n')
		{
		    printf("Pausing after detected error, continue? (y,n) ");
		    scanf(" %c",&yesno);
		    if(yesno != 'y') exit(1);
		}
	      index = amount;
	      continue;
	  }
	/* Have we found the start of a good data ray */
	else if(temp[index] == ryib.search)
	  {
	      raycount++;
	      beam = (struct BEAM *)&temp[index];
	      hr = beam->rray.hour;
	      min = beam->rray.minute;
	      sec = beam->rray.second;
	      millsec = beam->rray.millisecond;
	      present_time = beam->rray.second + 60 * (beam->rray.minute +
					       60 * beam->rray.hour);
	      
	      if(raycount > 2000)
		{
		    printf("Still Processing At Time: %2d:%2d:%2d:%3d\n",
			   hr,min,sec,millsec);
		    raycount = 0;
		}

	      temp[index] = 0;    /* Clear out RYIB for next time */
	      index = index + temp[index+1]/4;

	      if(first == 1)
		{
		    print_start(stdout);
		    if(file_onoff[1] == 'n')
		      print_start(fp_out);
		    if(print_onoff[1] == 'n')
		      print_start(printer);
		    first = 0;
		}

	  } /* else if temp[index] == ryib.search */

	/* Should be pointing at a platform info block */

	if(temp[index] != asib.search)
	  {
	      tempu.search = temp[index];
	      print_bad_id(stdout,asib.array,tempu.array);
	      if(file_onoff[1] == 'n')
		print_bad_id(fp_out,asib.array,tempu.array);
	      if(print_onoff[1] == 'n')
		print_bad_id(printer,asib.array,tempu.array);
	      if(pause_onoff[1] == 'n')
		{
		    printf("Pausing after detected error, continue? (y,n) ");
		    scanf(" %c",&yesno);
		    if(yesno != 'y') exit(1);
		}
	      index = amount;
	      continue;
	  }
	/* Have we found the start of a good platform block */
	else if(temp[index] == asib.search)
	  {
	      temp[index] = 0;    /* Clear out ASIB for next time */
	      index = index + temp[index+1]/4;
	  }

	/* Should be pointing at a field data record */

	if(temp[index] != frad.search)
	  {
	      tempu.search = temp[index];
	      print_bad_id(stdout,frad.array,tempu.array);
	      if(file_onoff[1] == 'n')
		print_bad_id(fp_out,frad.array,tempu.array);
	      if(print_onoff[1] == 'n')
		print_bad_id(printer,frad.array,tempu.array);
	      if(pause_onoff[1] == 'n')
		{
		    printf("Pausing after detected error, continue? (y,n) ");
		    scanf(" %c",&yesno);
		    if(yesno != 'y') exit(1);
		}
	      index = amount;
	      continue;
	  }
	/* Have we found the start of a good field data record */
	else if(temp[index] == frad.search)
	  {
	      temp[index] = 0;    /* Clear out FRAD for next time */
	      index = index + temp[index+1]/4;
	  }

	/* Should be pointing at an independent frequency data block */

	if(indepf_times_flg > 0)
	  {
	      if(temp[index] != indf.search)
		{
		    tempu.search = temp[index];
		    print_bad_id(stdout,indf.array,tempu.array);
		    if(file_onoff[1] == 'n')
		      print_bad_id(fp_out,indf.array,tempu.array);
		    if(print_onoff[1] == 'n')
		      print_bad_id(printer,indf.array,tempu.array);
		    if(pause_onoff[1] == 'n')
		      {
		      printf("Pausing after detected error, continue? (y,n) ");
			  scanf(" %c",&yesno);
			  if(yesno != 'y') exit(1);
		      }
		    index = amount;
		    continue;
		}
	      /* Have we found the start of a good independent
		 frequency block */
	      else if(temp[index] == indf.search)
		{
		    temp[index] = 0;    /* Clear out INDF for next time */
		    index = index + temp[index+1]/4;
		}
	  }


	/* Should be pointing at a time series data block */

	if(indepf_times_flg > 1)
	  {
	      if(temp[index] != timeu.search)
		{
		    tempu.search = temp[index];
		    print_bad_id(stdout,timeu.array,tempu.array);
		    if(file_onoff[1] == 'n')
		      print_bad_id(fp_out,timeu.array,tempu.array);
		    if(print_onoff[1] == 'n')
		      print_bad_id(printer,timeu.array,tempu.array);
		    if(pause_onoff[1] == 'n')
		      {
		      printf("Pausing after detected error, continue? (y,n) ");
			  scanf(" %c",&yesno);
			  if(yesno != 'y') exit(1);
		      }
		    index = amount;
		    continue;
		}
	      /* Have we found the start of a good time series block */
	      else if(temp[index] == timeu.search)
		{
		    temp[index] = 0;    /* Clear out TIME for next time */
		    index = index + temp[index+1]/4;
		}
	  }



	}/* WHILE(index < amount/4) */

}  /* amount > 0 and present_time < stop */

/* Read the end-of-file on the start of the tape */ 

if(amount==0)
{
    if(eof_detected)
      {
	  printf("Two EOFs in a row, ending process at: %2d:%2d:%2d\n",
		 hr,min,sec);
	  cleanup();
	  exit(1);
      }
    printf("FOUND END OF FILE MARKER at %2d:%2d:%2d\n",hr,min,sec);
    eof_detected = 1;
}

}while(present_time < stop);

printf("END TIME REACHED (%2d:%2d:%2d) CLOSING DRIVE\n",hr,min,sec);
cleanup();

}
void print_start(FILE *ptr)
{
fprintf(ptr," Start of Volume %d at %2d:%2d:%2d:%3d\n",
	volume_num,hr,min,sec,millsec);
}

void print_end(FILE *ptr)
{
fprintf(ptr," End of Volume %d at %2d:%2d:%2d:%3d\n",
	volume_num,hr,min,sec,millsec);
}

void print_bad_start(FILE *ptr, char *bad)
{

fprintf(ptr,"Record started with bad identifier: %c%c%c%c\n",
	*bad,*(bad + 1),*(bad + 2),*(bad + 3));
}

void print_bad_id(FILE *ptr, char *good, char *bad)
{

fprintf(ptr,"Bad identifier discovered, should be: %c%c%c%c",
	*good,*(good + 1),*(good + 2),*(good + 3));
fprintf(ptr,"  really is: %c%c%c%c  Time: %2d:%2d:%2d\n",
	*bad,*(bad + 1),*(bad + 2),*(bad + 3),hr,min,sec);
}
void cleanup()
{
printf("\nPerforming controlled shutdown\n");

close(fp_in);
if(file_onoff[1] == 'n') fclose(fp_out);
if(print_onoff[1] == 'n') pclose(printer);
exit(2);

}
