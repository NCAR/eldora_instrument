/*
 *	$Id$
 *
 *	Module: drv_sel		 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1996/06/24  22:59:57  craig
 * Initial revision
 *
 *
 * description:  Calls the routines that actually write the data
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <recordIncs.h>
#include <tapeDef.h>
#include <tapeGbl.h>

int file_size=0;

void drv_sel(unsigned int *location,unsigned int data_sz,unsigned char sgflg )
{
int i;
unsigned char drive_id;
char stat;
float percent;

union {
    unsigned long along;
    unsigned char achar[4];
}err;

/*******************************************/
/* Log start of new volume in the log file */
/*******************************************/
/* This is done here since the correct time is only known with the first
   ray of data, if we recorded in the log file when we record the
   header, the time would be incorrect in the log file */

if(new_volume)
  {
      log_ints[3] = vol_num;
      log_ints[4] = tape_num;
      loggerEvent("Volume_Start: %2d/%2d/%2d Vol#: %3d Tape#: %2d\n"
		  ,log_ints,5);
      new_volume = 0;
  }


/*********************************************************/
/*           Write out data to all active drives         */
/*********************************************************/

for(i=0; i<number_of_drives; i++)
  {
      drive_id = drives_to_use[i];
      stat = write_tape(location,data_sz,sgflg,drive_id);
      write_attempts++;
      if(stat != 0)
	{
	    tapeStatus->status[current_unit] &= ~RECORDING;
	    printf("WRITE ERROR ON DRIVE %d\n",drives_to_use[i]);
	    printf("WRITE STATUS = %X\n",stat);

	    /* Log this error in the log file */
	    log_ints[3] = drives_to_use[i];
	    log_ints[4] = stat;
	    loggerEvent("Tape_Error: %2d/%2d/%2d SCSI_ID: %1d Stat: %4x Standard data wrt\n",log_ints,5);
	}
  }

/********************************************************************/
/* Is it time to put on an automatic end-of-file and volume header? */
/********************************************************************/

file_size+=data_sz;
if(file_size >= MAX_FILE_SIZE) 
  {
      amount_written[current_unit] += (float)file_size / 1.0e6;
      printf("95\n");
      file_size=0;
      for(i=0; i<number_of_drives; i++) /* write header and eof on all
					   active drives */
	{
	    drive_id = drives_to_use[i];
	    stat = write_tape((unsigned int *)tapeHdr,hdrsz,BLOCKED,drive_id);
	    if(stat != 0)
	      {
		  tapeStatus->status[current_unit] &= ~RECORDING;
		  printf("WRITE ERROR ON DRIVE: %d\n", drives_to_use[i]);
		  printf("WRITE STATUS = %X\n",stat);

		  /* Log this error in the log file */
		  log_ints[3] = drives_to_use[i];
		  log_ints[4] = stat;
		  loggerEvent("Tape_Error: %2d/%2d/%2d SCSI_ID: %1d Stat: %4x Max File End Header Wrt\n",log_ints,5);
	      }

	    dlt_cmds(WRITE_FILEMARK,drive_id);
	}

      /* Log end of volume in the log file */
      log_ints[3] = vol_num;
      log_ints[4] = tape_num;
      loggerEvent("Volume_End: %2d/%2d/%2d Vol#: %3d Tape#: %2d Maximum Size Reached\n",log_ints,5);
      new_volume = 1;

      /* Do not increment volume number write new header on active drives */

      for(i=0; i<number_of_drives; i++)
	{
	    drive_id = drives_to_use[i];
	    stat = write_tape((unsigned int *)tapeHdr,hdrsz,BLOCKED,drive_id);
	    if(stat != 0)
	      {
		  tapeStatus->status[current_unit] &= ~RECORDING;
		  printf("WRITE ERROR ON DRIVE: %d\n", drives_to_use[i]);
		  printf("WRITE STATUS = %X\n",stat);

		  /* Log this error in the log file */
		  log_ints[3] = drives_to_use[i];
		  log_ints[4] = stat;
		  loggerEvent("Tape_Error: %2d/%2d/%2d SCSI_ID: %1d Stat: %4x New Header Wrt\n",log_ints,5);
	      }
	}


      }

/********************************************************************/
/*** Is it time to update status passed to the control processor? ***/
/********************************************************************/

if(WRITE_TAPE_STATUS==1)
  {
      drive_id = drives_to_use[0];
      tapeStatus->attempts[current_unit] = write_attempts;

      dlt_cmds(LOG_SENSE,drive_id);
      err.achar[0] = log_page->total_errors_cor.pvalue_msb & 0x7F;
      err.achar[1] = log_page->total_errors_cor.pvalue_midh;
      err.achar[2] = log_page->total_errors_cor.pvalue_midl;
      err.achar[3] = log_page->total_errors_cor.pvalue_lsb;


      percent = (TOTAL_TAPE_SIZE - amount_written[current_unit]) /
	TOTAL_TAPE_SIZE * 100.0;

      tapeStatus->eot_warning = (int)percent;
      tapeStatus->failures[current_unit] = err.along;

      /* Has the logical end of tape been encountered, or is there less than
         ENDING_PERCENT of the tape remaining, if so end this tape */

      if(((rqst_sen->sense_key & LEOT) == LEOT) || 
	 (int)percent < ENDING_PERCENT) 
	{
	    file_size = 0;
	    for(i=0; i<number_of_drives; i++) /* Write header, eof and unload
						 all active drives */
	      {

		  /* Write header to the drive */

		  drive_id = drives_to_use[i];
		  stat = write_tape((unsigned int *)tapeHdr,hdrsz,
				    BLOCKED,drive_id);
		  if(stat != 0)
		    {
			tapeStatus->status[current_unit] &= ~RECORDING;
			printf("WRITE ERROR ON DRIVE: %d\n", drives_to_use[i]);
			printf("WRITE STATUS = %X\n",stat);

			/* Log this error in the log file */
			log_ints[3] = drives_to_use[i];
			log_ints[4] = stat;
			loggerEvent("Tape_Error: %2d/%2d/%2d SCSI_ID: %1d Stat: %4x Ending Header Wrt\n",log_ints,5);
		    }

		  /* Log end of volume in the log file */
		  log_ints[3] = vol_num;
		  log_ints[4] = tape_num;
		  loggerEvent("Volume_End: %2d/%2d/%2d Vol#: %3d Tape#: %2d End of Tape\n",log_ints,5);
		  new_volume = 1;

		  /* Write a filemark and then unload each active drive */

		  dlt_cmds(WRITE_FILEMARK,drive_id);
		  dlt_cmds(UNLOAD,drive_id);
	      }

	    tapeStatus->status[current_unit] |= EJECTING;
	    tapeStatus->status[current_unit] &= ~RECORDING;
	    write_attempts = 0;

	    /* If the next unit has active drives we should ping-pong,
	       also we need to setup new  number_of_drives and
	       drives_to_use parameters, else we should end recording */

		  current_unit++;
		  if(current_unit > 1) current_unit = 0;
		  next_unit = 0;
		  if(current_unit == 0) next_unit = 1;
		  
		  number_of_drives = record_sys[current_unit][2];

		  tapeStatus->failures[current_unit] = 0;
		  tapeStatus->attempts[current_unit] = 0;
		  tapeStatus->eot_warning = 100;
		  tapeStatus->unit = current_unit;
	          amount_written[current_unit] = 0;

	    /* If we have drive(s) to record on great
	       else flag error and quit */

		  tapeStatus->status[current_unit] = 0;
		  if(number_of_drives > 0)
		    {
			if(number_of_drives == 2)
			  tapeStatus->status[current_unit] |= PARALLEL_ON;

			drives_to_use[0] = record_sys[current_unit][0];
			drives_to_use[1] = record_sys[current_unit][1];

			tapeStatus->status[current_unit] |= RECORDING;
			if(record_sys[next_unit][2] > 0)
			  tapeStatus->status[current_unit] |= PING_PONG_ON;

			vol_num++;

			for(i=0; i<number_of_drives; i++)
			  {
			      drive_id = drives_to_use[i];
			      sgflg=BLOCKED;
			      vol->volume_num = vol_num;
			      stat = write_tape((unsigned int *)tapeHdr,hdrsz,
					 sgflg,drive_id);
			      if(stat != 0)
				{
				    tapeStatus->status[current_unit] &=
				      ~RECORDING;
				    printf("WRITE ERROR ON DRIVE: %d\n",
					   drives_to_use[i]);
				    printf("WRITE STATUS = %X\n",stat);

				    /* Log this error in the log file */
				    log_ints[3] = drives_to_use[i];
				    log_ints[4] = stat;
				    loggerEvent("Tape_Error: %2d/%2d/%2d SCSI_ID: %1d Stat: %4x New Header Wrt\n",log_ints,5);
				}
			  }
		    } /* if(number_of_drives > 0) */
		  else
		    {
			REC_FLAG = 0;
			tapeStatus->status[current_unit] &= ~RECORDING;
		    }

	} /* if logical eot has been reached,
	     or less than ENDING_PERCENT remains */

  } /* if(WRITE_TAPE_STATUS == 1) */

return;
}

