/*
 *	$ix: cal_sum.c,v 1.1 1992/08/14 17:50:08 reif Exp reif $
 *
 *	Module:	cal_sum
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1995/01/26  16:20:25  craig
 * Initial revision
 *
 *
 * description: This routine calculates the RMS value of the time
 *              series data for use when calibrating the system.
 *              This routine will be call whenever the variable
 *              "calibrate" is true i.e. set calibrate=1 at the Vxworks
 *              prompt.  No tape recording will occur while this routine
 *              is running.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <cipincl.h>


void cal_sum()
{

static unsigned int *fore_addr, *num_recs;
static unsigned int *aft_addr;
static unsigned short *fore_mb;
static unsigned short *aft_mb;
unsigned short *mb_ptr;
int i, kill;

FIELDRADAR *fldrdr;
RAY *ray;

static unsigned int rad_rec_length;
static long last_mailbox;
int num_averaged, num_printed;
int time_series_size, number_per_dwell;
int offset_to_time_series, fore_aft, num_to_average, num_to_print;
int input, fnum, mb_count; 
double big;
float bigdb, *flt_pntr, bigft;
float scale_factor, if_gain;

fore_mb = (unsigned short *)MAIL_BOX;
mb_ptr = fore_mb;
aft_mb = fore_mb + 1;
fore_addr = (unsigned int *)DATA_RECS;
aft_addr = fore_addr + 1;
num_recs=(unsigned int *)NUM_RECS; /* How many records to scan for */
last_mailbox=MAIL_BOX + *num_recs*2-2;

fldrdr = GetFieldRadar(Hdr,1);
rad_dscr = GetRadar(Hdr,1);
wave = GetWaveform(Hdr);
kill = 0;

rad_rec_length=data_ray_length(); /* Calculate size of radar record */


if(fldrdr->indepf_times_flg != 3)
  {
      printf("No time series in this header - Returning to main program\n");
      return;
  }

/* Calculate the time series size minus 8 for the TIME characters and length */

time_series_size = rad_dscr->num_freq_trans * 2 * 4 * 
  wave->repeat_seq_dwel * wave->num_chips[0];

/*Calculate the number of samples per dwell (there is an I and Q per sample) */

number_per_dwell = wave->repeat_seq_dwel * wave->num_chips[0];

/* Calculate the offset to the start of the time series data itself */

offset_to_time_series = rad_rec_length - time_series_size;


fore_aft = 0;
num_to_average = 1000;
num_to_print = 100;
fnum = 1;
if_gain = 0.0;

do{ /* Keeps you comming back to the menu after calculating RMS power */
do{ /* Keeps you comming back to the menu while setting up */
    
    printf("\nCalibration RMS of the time series menu, enter your choice\n\n");
    printf("0) Exit back to the data recording code\n");
    printf("1) Change the radar to operate on (0 = fore, 1=aft) %1d\n",
	   fore_aft);
    printf("2) Change the number of pulses to average over:  %d\n",
	   num_to_average); 
    printf("3) Change the number of averages to make before returning to menu: %d\n",num_to_print);
    printf("4) Change the frequency being calibrated: %2d\n",fnum);
    printf("5) Change the IF gain used  %7.4f\n",if_gain);
    printf("6) Begin calculating the RMS values of the time series\n");
    scanf(" %1d",&input);

    switch(input)
      {
	case 0:
	  return;
	  break;

	case 1:
	  if(fore_aft == 0) fore_aft = 1;
	  else if(fore_aft == 1) fore_aft = 0;
	  break;

	case 2:
	  printf("Enter the number of pulses to average over: ");
	  scanf(" %d",&num_to_average);
	  break;

	case 3:
	  printf("Enter the number of averages to print before returning to menu: ");
	  scanf(" %d",&num_to_print);
	  break;

	case 4:
	  printf("Enter the frequency number to calibrate: ");
	  scanf(" %d",&fnum);
	  break;


	case 5:
	  printf("Enter the IF gain to use: ");
	  scanf(" %f",&if_gain);
	  break;

	default:
	  break;
      }

}while(input != 6);

/* Begin by Flushing out all mail boxes with zeros */

mb_ptr = (unsigned short *)MAIL_BOX;
for(mb_count=0; mb_count<*num_recs; mb_count++)
  {
      *mb_ptr=0; /*Flush all mailboxes */
      mb_ptr++;
      if((int)mb_ptr > last_mailbox)
	{
	    mb_ptr = fore_mb;
	    break;
	}
  }

/* Look for first occurence of good fore_mb */
if(!fore_aft)
  {
      i = 0;
      while(*fore_mb!=0xBFFF)
	{
	    fore_mb+=2;
	    fore_addr+=2;
	    if((int)fore_mb > last_mailbox) /* Test for last mail box */
	      {
		  fore_mb = (unsigned short *)MAIL_BOX;
		  fore_addr = (unsigned int *)DATA_RECS;
		  if(i++ > 2000) break;
	      } 
	}
  }

/* Look for first occurence of good aft_mb */

if(fore_aft)
  {
      i = 0;
      while(*aft_mb!=0xBFFF)
	{
	    aft_mb+=2;
	    aft_addr+=2;
	    if((int)aft_mb > last_mailbox) /* Test for last mail box */
	      {
		  aft_mb = (unsigned short *)MAIL_BOX+1;
		  aft_addr = (unsigned int *)DATA_RECS+1;
		  if(i++ > 2000) break;
	      } 
	}
  }


num_printed = 0;
num_averaged = 0;
big = 0.0;
scale_factor = fldrdr->conversion_gain + if_gain;

/**************************************************************************/
/****   initialization is complete, begin averaging and printing data *****/
/**************************************************************************/

if( !fore_aft)
  {
      do
	{

	    if(*fore_mb==0xBFFF)
	      {
		  *fore_mb=0x0000; /* Zero mailbox */

		  (unsigned int)ray=*fore_addr; /* Make radar data pointer */

		  /************* FORE RADAR DATA HANDLER ********************/

		  /* If first 4 characters are RYIB, then this is a regular
		     fore radar data record */

		  if(!strncmp("RYIB",ray->ray_info,4))
		    {
			flt_pntr = (float *)(*fore_addr + 
					     offset_to_time_series + 
					     4 * 2 * wave->repeat_seq_dwel * 
					     (fnum-1));
			for(i=0; i<number_per_dwell; i++)
			  {
			      big += *flt_pntr * *flt_pntr;
			      flt_pntr++;
			      big += *flt_pntr * *flt_pntr;
			      flt_pntr++;
			      num_averaged++;

			      if(num_averaged >= num_to_average)
				{
				    num_printed++;
				    big = big/(float)num_averaged;
				    bigdb = 10.0 * log10(big) - scale_factor;
				    bigft = big;
				    printf("%d  dB = %f\n",
					   num_printed, bigdb);
				    num_averaged = 0;
				    big = 0.0;
				}
			  }

		    } /* END if(!strncmp("RYIB",ray->ray_info,4)) */


		  fore_mb+=2; /* Increment data record address */
		  fore_addr+=2; /* Increment mailbox */

		  if((int)fore_mb > last_mailbox) /* Test for last mail box */
		    {
			fore_mb = (unsigned short *)MAIL_BOX;
			fore_addr = (unsigned int *)DATA_RECS;
		    }      

	      } /* END if (*fore_mb==0xBFFF) */

	}while(num_printed < num_to_print);
  }

if( fore_aft )
  {
      do
	{

	    if(*aft_mb==0xBFFF)
	      {
		  *aft_mb=0x0000; /* Zero mailbox */

		  (unsigned int)ray=*aft_addr; /* Make radar data pointer */

		  /************* AFT RADAR DATA HANDLER ********************/

		  /* If first 4 characters are RYIB, then this is a regular
		     aft radar data record */

		  if(!strncmp("RYIB",ray->ray_info,4))
		    {
			flt_pntr = (float *)(*aft_addr + 
					     offset_to_time_series +  4 * 2 *
					     wave->repeat_seq_dwel * (fnum-1));

			for(i=0; i<number_per_dwell; i++)
			  {
			      big += *flt_pntr * *flt_pntr;
			      flt_pntr++;
			      big += *flt_pntr * *flt_pntr;
			      flt_pntr++;
			      num_averaged++;

			      if(num_averaged >= num_to_average)
				{
				    num_printed++;
				    big = big/(float)num_averaged;
				    bigdb = 10.0 * log10(big) - scale_factor;
				    bigft = big;
				    printf("%d   dB = %f\n",
					   num_printed, bigdb);
				    big = 0.0;
				    num_averaged = 0;
				}
			  }

		    } /* END if(!strncmp("RYIB",ray->ray_info,4)) */


		  aft_mb+=2; /* Increment data record address */
		  aft_addr+=2; /* Increment mailbox */

		  if((int)aft_mb > last_mailbox) /* Test for last mail box */
		    {
			aft_mb = (unsigned short *)MAIL_BOX;
			aft_addr = (unsigned int *)DATA_RECS;
		    }      

	      } /* END if (*aft_mb==0xBFFF) */

	}while(num_printed < num_to_print);
  }

}while(kill == 0);

}





