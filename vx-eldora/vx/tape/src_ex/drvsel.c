/*
 *	$Id$
 *
 *	Module: drv_sel		 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1995/01/26  16:19:20  craig
 * Fixed problem with volume numbers counting incorrectly
 *
 * Revision 1.2  1994/11/14  17:50:23  craig
 * Modified prints to the log file
 *
 * Revision 1.1  1994/01/06  21:31:47  craig
 * Initial revision
 *
 * Revision 1.7  1993/09/21  20:46:16  reif
 * *** empty log message ***
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>
int file_size=0;

void drv_sel(unsigned int *location,unsigned int data_sz,unsigned char sgflg )
{
int i;
unsigned char drive_id;
char stat;
float percent;

union
  {
      unsigned long sense_data;
      unsigned char tape[4];
  }tp;

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
      loggerEvent("Volume_Start Time: %2d/%2d/%2d Volume#: %3d Tape#: %2d\n"
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
	    loggerEvent("Tape_Error Time: %2d/%2d/%2d SCSI_ID: %1d Status: %4x Error on standard data write\n",log_ints,5);
	}
  }

/********************************************************************/
/* Is it time to put on an automatic end-of-file and volume header? */
/********************************************************************/

file_size+=data_sz;
if(file_size >= MAX_FILE_SIZE) 
  {
      printf("95\n");
      file_size=0;
      for(i=0; i<number_of_drives; i++) /* write header and eof on all
					   active drives */
	{
	    drive_id = drives_to_use[i];
	    stat = write_tape((unsigned int *)tapeHdr,hdrsz,HEADER,drive_id);
	    if(stat != 0)
	      {
		  tapeStatus->status[current_unit] &= ~RECORDING;
		  printf("WRITE ERROR ON DRIVE: %d\n", drives_to_use[i]);
		  printf("WRITE STATUS = %X\n",stat);

		  /* Log this error in the log file */
		  log_ints[3] = drives_to_use[i];
		  log_ints[4] = stat;
		  loggerEvent("Tape_Error Time: %2d/%2d/%2d SCSI_ID: %1d Status: %4x Error on Maximum File Ending Header Write\n",log_ints,5);
	      }

	    exb_cmds(WRITE_FILEMARK,WRT_FLMK,drive_id);
	}

      /* Log end of volume in the log file */
      log_ints[3] = vol_num;
      log_ints[4] = tape_num;
      loggerEvent("Volume_End Time: %2d/%2d/%2d Volume#: %3d Tape#: %2d Maximum Volume Size Reached\n",log_ints,5);
      new_volume = 1;

      /* Do not increment volume number write new header on active drives */

      for(i=0; i<number_of_drives; i++)
	{
	    drive_id = drives_to_use[i];
	    stat = write_tape((unsigned int *)tapeHdr,hdrsz,HEADER,drive_id);
	    if(stat != 0)
	      {
		  tapeStatus->status[current_unit] &= ~RECORDING;
		  printf("WRITE ERROR ON DRIVE: %d\n", drives_to_use[i]);
		  printf("WRITE STATUS = %X\n",stat);

		  /* Log this error in the log file */
		  log_ints[3] = drives_to_use[i];
		  log_ints[4] = stat;
		  loggerEvent("Tape_Error Time: %2d/%2d/%2d SCSI_ID: %1d Status: %4x Error on New Header Write\n",log_ints,5);
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
      exb_cmds(REQUEST_SENSE,RQST_SNS,drive_id);
      tp.tape[1] = rqst_sen.rem_tape_msb;
      tp.tape[2] = rqst_sen.rem_tape_mid;
      tp.tape[3] = rqst_sen.rem_tape_lsb;
      percent = (((float)tp.sense_data/
		  (float)starting_amnt[current_unit])*100);
      tapeStatus->eot_warning = (int)percent;
      tp.tape[1] = rqst_sen.err_ctr_msb;
      tp.tape[2] = rqst_sen.err_ctr_mid;
      tp.tape[3] = rqst_sen.err_ctr_lsb;
      tapeStatus->failures[current_unit] = tp.sense_data;

      /* Has the logical end of tape been encountered, or is there less than
         ENDING_PERCENT of the tape remaining, if so end this tape */

      if((rqst_sen.sense_key & LEOT) || percent < ENDING_PERCENT) 
	{
	    file_size = 0;
	    for(i=0; i<number_of_drives; i++) /* Write header, eof and unload
						 all active drives */
	      {

		  /* Write header to the drive */

		  drive_id = drives_to_use[i];
		  stat = write_tape((unsigned int *)tapeHdr,hdrsz,
				    HEADER,drive_id);
		  if(stat != 0)
		    {
			tapeStatus->status[current_unit] &= ~RECORDING;
			printf("WRITE ERROR ON DRIVE: %d\n", drives_to_use[i]);
			printf("WRITE STATUS = %X\n",stat);

			/* Log this error in the log file */
			log_ints[3] = drives_to_use[i];
			log_ints[4] = stat;
			loggerEvent("Tape_Error Time: %2d/%2d/%2d SCSI_ID: %1d Status: %4x Error on Ending Header Write\n",log_ints,5);
		    }

		  /* Log end of volume in the log file */
		  log_ints[3] = vol_num;
		  log_ints[4] = tape_num;
		  loggerEvent("Volume_End Time: %2d/%2d/%2d Volume#: %3d Tape#: %2d End of Tape Reached\n",log_ints,5);
		  new_volume = 1;

		  /* Write a filemark and then unload each active drive */

		  exb_cmds(WRITE_FILEMARK,WRT_FLMK,drive_id);
		  exb_cmds(UNLOAD,ULD,drive_id);
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
			      sgflg=HEADER;
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
				    loggerEvent("Tape_Error Time: %2d/%2d/%2d SCSI_ID: %1d Status: %4x Error on New Header Write\n",log_ints,5);
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

