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
 * Revision 1.7  1993/09/21  20:46:16  reif
 * *** empty log message ***
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>

void drv_sel(unsigned int *location,unsigned int data_sz,unsigned char sgflg )
{
int i;
unsigned char drive_id;
char stat;
float percent;
static unsigned int file_size=0;

union
  {
      unsigned long sense_data;
      unsigned char tape[4];
  }tp;


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
	    printf("WRITE ERROR ON DRIVE %d\n",drives_to_use[i]);
	    printf("WRITE STATUS = %X\n",stat);
	    /* Log write error and set in status block */
	}
  }

/********************************************************************/
/* Is it time to put on an automatic end-of-file and volume header? */
/********************************************************************/

file_size+=data_sz;
if(file_size >= MAX_FILE_SIZE) 
  {
      for(i=0; i<number_of_drives; i++) /* write header and eof on all
					   active drives */
	{
	    drive_id = drives_to_use[i];
	    write_tape((unsigned int *)tapeHdr,hdrsz,HEADER,drive_id);
	    exb_cmds(WRITE_FILEMARK,WRT_FLMK,drive_id);
	}

      /* Increment volume number write new header on active drives */

      vol->volume_num = vol_num++;
      for(i=0; i<number_of_drives; i++)
	{
	    drive_id = drives_to_use[i];
	    write_tape((unsigned int *)tapeHdr,hdrsz,HEADER,drive_id);
	}

      /* Log the start of an automatic file */

      printf("sv#%d_tp#%d_%d:%d:%d\n",vol_num,tape_num,hr,min,sec);
      loggerEvent("SV_%d/",&hr,1);
      loggerEvent("%d/",&min,1);
      loggerEvent("%d_",&sec,1);
      loggerEvent("%d_",&vol_num,1);
      loggerEvent("%d\n",&tape_num,1);
      file_size=0;
      }

/********************************************************************/
/*** Is it time to update status passed to the control processor? ***/
/********************************************************************/

if(WRITE_TAPE_STATUS==1)
  {
      drive_id = drives_to_use[0];
      tapeStatus->attempts = write_attempts;
      exb_cmds(REQUEST_SENSE,RQST_SNS,drive_id);
      tp.tape[1] = rqst_sen.rem_tape_msb;
      tp.tape[2] = rqst_sen.rem_tape_mid;
      tp.tape[3] = rqst_sen.rem_tape_lsb;
      percent = (((float)tp.sense_data/(float)starting_amnt[UNIT_NUM])*100);
      tapeStatus->eot_warning = (int)percent;
      tp.tape[1] = rqst_sen.err_ctr_msb;
      tp.tape[2] = rqst_sen.err_ctr_mid;
      tp.tape[3] = rqst_sen.err_ctr_lsb;
      tapeStatus->failures = tp.sense_data;

      /* Has the logical end of tape been encountered, or is there less than
         ENDING_PERCENT of the tape remaining, if so end this tape */

      if((rqst_sen.sense_key & LEOT) || percent < ENDING_PERCENT) 
	{
	    for(i=0; i<number_of_drives; i++) /* Write header, eof and unload
						 all active drives */
	      {
		  drive_id = drives_to_use[i];
		  stat = write_tape((unsigned int *)tapeHdr,hdrsz,
				    HEADER,drive_id);
		  printf("WROTE ENDING HEADER DRIVE: %d\n", drives_to_use[i]);
		  if(stat != 0)
		    {
			printf("WRITE ERROR ON DRIVE: %d\n", drives_to_use[i]);
			printf("WRITE STATUS = %X\n",stat);

			/*Log error and set tape status */
		    }
		  exb_cmds(WRITE_FILEMARK,WRT_FLMK,drive_id);
		  printf("WROTE ENDING FILEMARK DRIVE: %d\n",drives_to_use[i]);

		  exb_cmds(UNLOAD,ULD,drive_id);
	      }
	    tapeStatus->status[UNIT_NUM] |= EJECTING;
	    tapeStatus->status[UNIT_NUM] &= ~RECORDING;
	    write_attempts = 0;

	    /* If ping-ponging is enabled, we need to setup new 
	       number_of_drives and drives_to_use parameters, else
	       we should end recording */

	    if(PP_FLAG != 0)  /* if ping ponging is enabled */
	      {
		  UNIT_NUM++;
		  if(UNIT_NUM > 1) UNIT_NUM = 0;
		  
		  number_of_drives = record_sys[UNIT_NUM][2];

		  tapeStatus->failures = 0;
		  tapeStatus->attempts = 0;
		  tapeStatus->eot_warning = 0;
		  tapeStatus->unit = UNIT_NUM;
		  tapeStatus->terr[UNIT_NUM].error = 0;
		  tapeStatus->serr[UNIT_NUM].error = 0;

	    /* If we have drive(s) to record on great
	       else flag error and quit */

		  if(number_of_drives > 0)
		    {
			if(PARALLEL_REC != 0 && number_of_drives == 2)
			  tapeStatus->status[UNIT_NUM] |= PARALLEL_ON;
			else if(PARALLEL_REC == 0 || number_of_drives == 1)
			  {
			      number_of_drives = 1;
			      tapeStatus->status[UNIT_NUM] &= ~PARALLEL_ON;
			  }

			drives_to_use[0] = record_sys[UNIT_NUM][0];
			drives_to_use[1] = record_sys[UNIT_NUM][1];

			tapeStatus->status[UNIT_NUM] |= RECORDING;
			tapeStatus->status[UNIT_NUM] |= PING_PONG_ON;

			vol_num++;

			for(i=0; i<number_of_drives; i++)
			  {
			      drive_id = drives_to_use[i];
			      sgflg=HEADER;
			      vol->volume_num = vol_num;
			      write_tape((unsigned int *)tapeHdr,hdrsz,
					 sgflg,drive_id);
			      printf("WROTE BEGINNING HEADER TO %d\n",
				     drives_to_use[i]);
			      printf("sv#%d_tp#%d_%d:%d:%d\n",vol_num,
				     tape_num,hr,min,sec);
			      loggerEvent("SV_%d/",&hr,1);
			      loggerEvent("%d/",&min,1);
			      loggerEvent("%d_",&sec,1);
			      loggerEvent("%d_",&vol_num,1);
			      loggerEvent("%d\n",&tape_num,1);
			  }
		    } /* if(number_of_drives > 0) */
		  else
		    {
			REC_FLAG = 0;
			tapeStatus->status[UNIT_NUM] &= ~RECORDING;
		    }
	      }
	    else /* Ping-Ponging is not enabled */
	      {
		  REC_FLAG=0;
	      }

	} /* if logical eot has been reached,
	     or less than ENDING_PERCENT remains */

  } /* if(WRITE_TAPE_STATUS == 1 */
return;
}

