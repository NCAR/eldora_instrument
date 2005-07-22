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

struct BEAM{
    struct ray_i rray;
    struct platform_i rplat;
    struct field_parameter_data rdata;
    short data[200];
};


#define MAX_BUFF 32768

void main(void);


void main()
{

int fp; /* File pointer */
extern char *sys_errlist[];
int temp[MAX_BUFF];
int i,j,index,amount,k;
int hr, min, sec, start, time;
int old_index, location, diff, input, skip;
char yesno;
struct BEAM *ray_pntr;

union
  {
      int aint;
      char array[4];
  }sch;

union
  {
      int head_int;
      char head_array[4];
  }head_sch;

union
  {
      int ray_int;
      char ray_array[4];
  }ray_sch;

head_sch.head_array[0] = 'V';
head_sch.head_array[1] = 'O';
head_sch.head_array[2] = 'L';
head_sch.head_array[3] = 'D';

ray_sch.ray_array[0] = 'R';
ray_sch.ray_array[1] = 'Y';
ray_sch.ray_array[2] = 'I';
ray_sch.ray_array[3] = 'B';

/* Greet the user */
printf("\n\nWelcome to the DORADE tape walking routine\n");

/* Get the start time */

printf("\nEnter the start time as HH MM SS\n");
printf("(Enter 00 00 00 to walk first record on tape) ");
scanf("%2d %2d %2d",&hr,&min,&sec);
start = sec + 60 * (min + 60 * hr);
time = 0;

/* Open The Tape Drive */

printf("I am going to read the first record on the tape, please wait\n");
fp = open("/dev/rmt/1bn",O_RDONLY); /* Open tape drive for binary read */
if(fp<0)
{
	printf("CANNOT OPEN TAPE DRIVE\n");
	printf("REASON: %s\n",sys_errlist[errno]);       
	exit(1); /* If cannot open, exit */
}


/* Go forward down the tape to the start time */

if(start > 1.0)
{
    k = 0;
    do{
	amount = read(fp,temp,MAX_BUFF);
	if(temp[0] == ray_sch.ray_int)
	  {
	  ray_pntr = (struct BEAM *)&temp[0];
	  time = ray_pntr->rray.second + 60 * (ray_pntr->rray.minute +
					       60 * ray_pntr->rray.hour);
	  k += 1;
	  if(k > 200)
	    {
		printf("Still looking for start time at: %2d:%2d:%2d\n",
		       ray_pntr->rray.hour, ray_pntr->rray.minute,
		       ray_pntr->rray.second);
		k = 0;
	    }
          }
    }while(time < start);
}



/* Begin Reading Tape */
input = 1;
do {
   while((amount = read(fp,temp,MAX_BUFF))>0)
      {
	  old_index = 0;
	  index = 0;
	  printf("\n******************************************************\n");
	  if(input == 1)
	    printf("Number of bytes read from new data record: %6d\n",amount);
	  else if(input == 4)
	    printf("CHARACTERS NOT FOUND!! new record of %6d bytes read\n",
		   amount);
	  else if(input !=4 && input != 1)
	    printf("END OF CURRENT RECORD REACHED!! new record of %6d bytes read\n",amount);
	  printf("******************************************************\n\n");
 

	  do{
	      location = index * 4;
	      diff = (index-old_index)*4;
	      old_index = index;
	      printf("*******************************************\n\n");
	      printf("Current location in record is: %d\n",location);
	      printf("Bytes since last data block: %d\n",diff);
	      sch.aint = temp[index];
	      printf("First four characters of current data block are: %c%c%c%c \n",
		     sch.array[0],sch.array[1],sch.array[2],sch.array[3]);
	      printf("Length of current data block is: %d",temp[index+1]);
	      if(temp[index] == ray_sch.ray_int)
		{
		    ray_pntr = (struct BEAM *)&temp[index];
		    printf("  Time:  %2d:%2d:%2d\n",
			   ray_pntr->rray.hour, ray_pntr->rray.minute,
			   ray_pntr->rray.second);
		}
	      else
		printf("\n");


	      printf("\nYou have many choices:\n");
	      printf("0) End this program\n");
	      printf("1) Read the next data record\n");
	      printf("2) Skip forward %d bytes to see next data block",temp[index+1]);
	      printf("(Standard Method)\n");
	      printf("3) Skip forward a number of bytes you choose to find the next block\n");
	      printf("4) Find first four characters of a new data block\n");
	      printf("5) Describe a typical ELDORA Volume Header\n");
	      printf("6) Describe a typical ELDORA data record\n");
	      printf("7) Do a Hexidecimal dump of this data record\n");
	      printf("8) Walk the remainder of this volume\n");

	      scanf(" %d",&input);

	      switch(input)
		{
		  case 0:
		    exit(1);
		    break;
		  case 1:
		    index = index + amount;
		    break;
		  case 2:
		    index = index + temp[index+1]/4;
		    break;
		  case 3:
		    printf("Enter number of bytes to skip (evenly / 4): ");
		    scanf(" %d",&skip);
		    index = index + skip/4;
		    break;
		  case 4:
		    printf("Enter the four ASCII characters to look for: ");
		    scanf(" %c%c%c%c",&sch.array[0],&sch.array[1],
			  &sch.array[2],&sch.array[3]);
		    do{
			index += 1;
		    }while((temp[index] != sch.aint) && index < amount/4);
		    break;
		  case 5:
		    printf("A typical ELDORA Volume Header: \n");
		    printf("Characters  location  length \n");
		    printf("VOLD            0        72 \n");
		    printf("WAVE           72       364 \n");
		    printf("RADD          436       144 \n");
		    printf("FRIB          580       184 \n");
		    printf("CSPD          764        36 \n");
		    printf("PARM          800       104 \n");
		    printf("PARM          904       104 \n");
		    printf("PARM         1008       104 \n");
		    printf("PARM         1112       104 \n");
		    printf("RADD         1216       144 \n");
		    printf("FRIB         1360       184 \n");
		    printf("CSPD         1544        36 \n");
		    printf("PARM         1580       104 \n");
		    printf("PARM         1684       104 \n");
		    printf("PARM         1788       104 \n");
		    printf("PARM         1892       104 \n");
		    printf("NDDS         1996        16 \n");
		    printf("SITU         2012      4108 \n");
	         printf("The number of PARM blocks may change for dual PRT\n");
		    break;
		  case 6:
		    printf("A typical data record: \n");
		    printf("Characters  length \n");
		    printf("RYIB           44\n");
		    printf("ASIB           80\n");
		    printf("FRAD        Totally variable\n");
	    printf("INDF        Totally variable (may be nonexistant\n");
	    printf("TIME        Totally variable (may be nonexistant\n");
		    printf("The same order and number of blocks should be\n");
		    printf("repeated throughout the record and volume\n");
		    break;
		  case 7:
		    k = index;
		    do{
		    for(i=0; i<20; i++)
		      {
			  printf("\n%5d %4d ",k*4+i*20,i*20);
			  for(j=0; j<5; j++)
			    printf("%08x ",temp[j+k+i*5]);
			  for(j=0; j<5; j++)
			    {
				sch.aint = temp[j+k+i*5];
			      if(0x7E >= sch.array[0] &&  sch.array[0] >= 0x20)
				printf("%c",sch.array[0]);
			      else
				printf(".");
			      if(0x7E >= sch.array[1] &&  sch.array[1] >= 0x20)
				printf("%c",sch.array[1]);
			      else
				printf(".");
			      if(0x7E >= sch.array[2] &&  sch.array[2] >= 0x20)
				printf("%c",sch.array[2]);
			      else
				printf(".");
			      if(0x7E >= sch.array[3] &&  sch.array[3] >= 0x20)
				printf("%c",sch.array[3]);
			      else
				printf(".");
			    }
		      }
		    k = k+100;
		    printf("\nContinue? (y/n)\n");
		    scanf(" %c",&yesno);
		    }while(yesno == 'y' && k < amount/4);
		    break;

		}
	  }while(index < amount/4);
      }


if(amount<0)
{
    printf("READ ERROR DUE TO:\n");
    printf("%s\n",sys_errlist[errno]);
    if(close(fp)<0)
      {
	  printf("ERROR! CANNOT CLOSE DRIVE DUE TO:");
	  printf("%s\n",sys_errlist[errno]);
      }
     exit(1);
}

if(amount==0)
{
    printf("REACHED END OF FILE!");
}

if(close(fp)<0)
{
    printf("ERROR! CANNOT CLOSE DRIVE DUE TO:");
    printf("%s\n",sys_errlist[errno]);
    exit(1);
}

}while(input !=0);

printf("CLOSING DRIVE\n");
}


