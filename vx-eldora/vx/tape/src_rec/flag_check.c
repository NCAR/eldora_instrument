/*
 *	$Id$
 *
 *	Module:		 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1996/06/24  22:59:09  craig
 * Initial revision
 *

 * * description:  This routine is the main executive routine of the recording
 *                 processor.  The routine beings by performing all
 *                 necessary initialization, then it enters an infinite run
 *                 time loop.  In this loop it waits for the start command
 *                 and the record command.  If these commands are received,
 *                 the routine writes headers to the tape drive(s) and then
 *                 calls the mail_box routine which actually writes the data.
 *                 The program will not return back from the mail_box routine
 *                 until a stop command is received.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope
#define TAPE_CTRL_SCOPE
#define HEADERRPC_SCOPE

#include <recordIncs.h>
#include <tapeDef.h>
#include <tapeGbl.h>

extern int file_size;

void flag_check(char drv0,char drv1,char drv2,char drv3)
{

int i, j, first_found, second_found, header_written, cells;
int timeout, logger_initialized, cntrl_size;
float dwelltime, rpm, width;
unsigned char sgflg, unschar, drv_stat, stat;
char *cntrl_addr;

union{
    int sqr[2][2];
    int lin[4];
}health;

vol=(VOLUME *)tapeHdr;

/* INITIALIZE GOOD DRIVE ARRAY FROM COMMAND LINE */

health.lin[0]=drv0;
health.lin[1]=drv1;
health.lin[2]=drv2;
health.lin[3]=drv3;

/* INITIALIZE THE PHYSICAL SCSI BUS UNIT NUMBER OF EACH DRIVE */

physical_unit[0][0] = 1;
scsi_id[0] = 1;
physical_unit[0][1] = 2;
scsi_id[0] = 2;
physical_unit[1][0] = 3;
scsi_id[0] = 3;
physical_unit[1][1] = 4;
scsi_id[0] = 4;

/* INITIALIZE THE TAPE CONTROL FLAGS */

logger_initialized = 0;
current_unit=0;
next_unit = 1;
RUN_FLAG=0;
REC_FLAG=0;
REWIND_FLAG=0;
UNLOAD_FLAG=0;
WRITE_TAPE_STATUS=0;
vol_num=-1;
tape_num=1;
header_written = 0;
max_blk_sz = (long)MAX_BLK_SZ;

/*  Set default status words to indicate current conditions */

tapeStatus->eot_warning = 100;
tapeStatus->unit = 0;

/* Setup the array record_sys, based on the healthy drives */

record_sys[0][2] = 0;
record_sys[1][2] = 0;
number_of_drives = 0;

for(i=0; i<2; i++)
  {
      record_sys[0][i] = 0;
      record_sys[1][i] = 0;
      if(health.sqr[0][i])
	{
	    record_sys[0][2]++;
	    number_of_drives++;
	}
      if(health.sqr[1][i])
	{
	    record_sys[1][2]++;
	    number_of_drives++;
	}
  }

switch(number_of_drives)
  {
    case 1:   /* Only one healthy drive available use it always */
      record_sys[0][2] = 1;
      record_sys[1][2] = 0;
      for(i=0; i<4; i++)
	{
	/* can be tricky and use physical_unit[0][i] because rows are laid
	   out in memory completely, then the next complete row etc. */
	    if(health.lin[i])
	      record_sys[0][0] = physical_unit[0][i];
	}
      break;

    case 2:  /* Two healthy drives, no parallel recording, ping pong between
                them */
      record_sys[0][2] = 1;
      record_sys[1][2] = 1;
      first_found = 0;
      for(i=0; i<4; i++)
	{
	/* can be tricky and use physical_unit[0][i] because rows are laid
	   out in memory completely, then the next complete row etc. */
	    if(health.lin[i] && first_found)
	      record_sys[1][0] = physical_unit[0][i];
	    else if(health.lin[i] && !first_found)
	      {
		  record_sys[0][0] = physical_unit[0][i];
		  first_found = 1;
	      }
	}
      break;

    case 3:  /* Three healthy drives, parallel record on first two, ping
		pong over to the second one */
      record_sys[0][2] = 2;
      record_sys[1][2] = 1;
      first_found = 0;
      second_found = 0;
      for(i=0; i<4; i++)
	{
	/* can be tricky and use physical_unit[0][i] because rows are laid
	   out in memory completely, then the next complete row etc. */
	    if(health.lin[i] && second_found)
	      record_sys[1][0] = physical_unit[0][i];
	    else if(health.lin[i] && first_found)
	      {
		  record_sys[0][1] = physical_unit[0][i];
		  second_found = 1;
	      }
	    else if(health.lin[i] && !first_found)
	      {
		  record_sys[0][0] = physical_unit[0][i];
		  first_found = 1;
	      }
	}
      break;

    case 4:  /* Four healthy drives, parallel record on first two, ping
		pong over to the next two (hope this is common) */
      record_sys[0][2] = 2;
      record_sys[1][2] = 2;
      for(i=0; i<2; i++)
	{
	    record_sys[0][i] = physical_unit[0][i];
	    record_sys[1][i] = physical_unit[1][i];
	}
      break;
  }

/*  Set status words to indicate current conditions */

for(i=0; i<2; i++)
  {
      tapeStatus->status[i] = 0;
      tapeStatus->number_of_drives[i] = record_sys[i][2];
      tapeStatus->status[i] |= INITIALIZING;
      tapeStatus->failures[i] = 0;
      tapeStatus->attempts[i] = 0;
  }

/* If opposite system has at least one drive we can ping-pong to it */

if(record_sys[current_unit][2] > 0)
  tapeStatus->status[next_unit] |= PING_PONG_ON;
if(record_sys[next_unit][2] > 0)
  tapeStatus->status[current_unit] |= PING_PONG_ON;

/* if same system has more than one drive we can parallel record */

if(record_sys[current_unit][2] > 1)
  tapeStatus->status[current_unit] |= PARALLEL_ON;
if(record_sys[next_unit][2] > 1)
  tapeStatus->status[next_unit] |= PARALLEL_ON;

tapeStatus->drives0[0] = record_sys[0][0];
tapeStatus->drives0[1] = record_sys[0][1];
tapeStatus->drives1[0] = record_sys[1][0];
tapeStatus->drives1[1] = record_sys[1][1];


/* Print current recording system definitions */
printf(" Recording system    drive 0    drive 1    number of drives\n");
for(i=0; i<2; i++)
printf("      %1d              %1d         %1d          %1d\n",
i,record_sys[i][0],record_sys[i][1],record_sys[i][2]); 


/* Get the size of the area to malloc for the tape control structures,
   malloc the space and initialize all of the control structures */

tape_vme_offset = 0;
cntrl_size = tape_cntrl_len(SCATTER_GATHER);
cntrl_addr = malloc(cntrl_size);
initialize_tape((int)cntrl_addr,SCATTER_GATHER);

/* Initialize all healthy drives then eject all of them */

for(i=0; i<2; i++)
  {
      for(j=0; j<2; j++)
	{
	    if(health.sqr[i][j]) /* IF DRIVE IS HEALTHY, INITIALIZE IT,
				    THEN EJECT THE TAPE */
	      {
		  unschar = physical_unit[i][j];
		  printf("INITIALIZING SCSI DRIVE %2d\n",
			 physical_unit[i][j]);
		  drive_init(unschar);
		  drv_stat=tst_unt_rdy(unschar);
		  while(drv_stat!=0x80 && timeout < 10)
		    {
			drv_stat=tst_unt_rdy(unschar);
			timeout++;
			taskDelay(200);
		    }
		  if(timeout == 10)
			printf("ERROR: DRIVE INITIALIZATION, SCSI DRIVE: %2d, DRIVE STATUS: %X\n",physical_unit[i][j],drv_stat);
		  /* EJECT THE TAPE */
		  printf("Unloading tape drive: %2d\n",physical_unit[i][j]);
		  dlt_cmds(UNLOAD,unschar);
	      }
	}
  }

/* Wait for the LOAD_FLAG to be set then continue */

LOAD_FLAG = 0;
printf("Will wait now for the LOAD_FLAG to be set to 1\n");
while(LOAD_FLAG == 0)
  taskDelay(60);
LOAD_FLAG = 0;


/* Load tape in all heathy drives */

for(i=0; i<2; i++)
  {
      for(j=0; j<2; j++)
	{
	    if(health.sqr[i][j]) /* IF DRIVE IS HEALTHY, load its tape */
	      {
		  unschar = physical_unit[i][j];
		  printf("LOADING TAPE ON SCSI DRIVE: %2d\n",
			       physical_unit[i][j]);
		  dlt_cmds(LOAD,unschar);
	      }
	}
  }

/* Wait for each drive to come ready before proceeding*/

for(i=0; i<2; i++)
  {
      for(j=0; j<2; j++)
	{
	    if(health.sqr[i][j]) /* IF DRIVE IS HEALTHY, wait for it
				    to come ready,THEN WRITE an EOF */
	      {
		  unschar = physical_unit[i][j];
		  drv_stat=tst_unt_rdy(unschar);
		  while(drv_stat!=0x80 && timeout < 2000)
		    {
			printf("Waiting for drive %d to come ready\n",
			       physical_unit[i][j]); 
			drv_stat=tst_unt_rdy(unschar);
			timeout++;
			taskDelay(200);
		    }
		  if(timeout == 2000)
			printf("ERROR: DRIVE NOT READY, SCSI DRIVE: %2d, DRIVE STATUS: %X\n",physical_unit[i][j],drv_stat);
	      }
	}
  }

/* Initialization is done, clear status indication */

tapeStatus->status[0] &= ~INITIALIZING;
tapeStatus->status[1] &= ~INITIALIZING;

/* Set number_of_drives and drives_to_use to unit zero's setup */

number_of_drives = record_sys[0][2];
drives_to_use[0] = record_sys[0][0];
drives_to_use[1] = record_sys[0][1];

/* Set the amount written to tape to zero for each recording system */
amount_written[0] = 0;
amount_written[1] = 0;

/********************************************************************/
/**********   Start of infinite run time loop         ***************/
/********************************************************************/

for(;;)
  {
      /* While we are stopped we should check rewind and unload flags */

      if(RUN_FLAG == 0)
	printf("Will wait here for the Control Processor to start me\n");

      while(RUN_FLAG == 0)
	{
	    header_written = 0;

	    if(UNLOAD_FLAG==1)
	      {
		UNLOAD_FLAG = 0;
		tapeStatus->status[current_unit] |= EJECTING;
		for(i=0; i<number_of_drives; i++)
		  {
		      unschar = drives_to_use[i];
		      dlt_cmds(UNLOAD,unschar);
		  }
	      }

	    if(REWIND_FLAG==1)
	      {
		REWIND_FLAG = 0;
		tapeStatus->status[current_unit] |= REWINDING;
		for(i=0; i<number_of_drives; i++)
		  {
		      unschar = drives_to_use[i];
		      dlt_cmds(REWND,unschar);
		  }
	      }

	    if(REC_FLAG != 0)
		  tapeStatus->status[current_unit] != RECORDING;
	    else
		  tapeStatus->status[current_unit] &= ~RECORDING;


	    taskDelay(60);
	}

/*  We have been told to go, but we must first check to see if the record
    flag is set, if not this is an error */

      if(REC_FLAG == 0) printf("ERROR: RECORD FLAG NOT ON\n");

      while(REC_FLAG == 0 && RUN_FLAG != 0)
	{
	    tapeStatus->status[current_unit] &= ~RECORDING;
	    taskDelay(60);
	}

/* REC_FLAG is now on, or we have been told to stop.  If we are going
   to record, we need to setup the array drives_to_use and the variable
   number_of_drives, based on all the commands.  If we have been told
   to stop, just go back up and wait for the run flag again  */

      if(REC_FLAG != 0 && !header_written)
	{

	    /********* INIT RECORDING LOG FILE **********/
	    if(logger_initialized == 0)
	      {
		/*	  loggerInit(RECORD_LOG);  */
		  logger_initialized = 1;
	      }

	    file_size = 0;
	    number_of_drives = record_sys[current_unit][2];
	    tapeStatus->status[current_unit] = 0;

	    /* If we have drive(s) to record on great
	       else flag error and quit */

	    if(number_of_drives > 0)
	      {
		  if(number_of_drives == 2)
		    tapeStatus->status[current_unit] |= PARALLEL_ON;

		  drives_to_use[0] = record_sys[current_unit][0];
		  drives_to_use[1] = record_sys[current_unit][1];

		  tapeStatus->status[current_unit] |= RECORDING;

		  if(record_sys[next_unit] > 0)
		    tapeStatus->status[current_unit] |= PING_PONG_ON;

		  tapeStatus->unit = current_unit;

		  vol_num++;

		  /* Get pointers to various data structures in the header
		     then use these pointers to write the current setup
		     records in the logfile */

		  tape_header();

		  head = GetVolume(Hdr);
		  wave = GetWaveform(Hdr);
		  rad_dscr = GetRadar(Hdr,1);
		  cs = GetCellSpacing(Hdr,1);

		  /* if needed initialize the data reduction arrays */
		  if(rad_dscr->data_reduction == 3) reduce_init();

		  /* First line */
		  sprintf(log_chars,"Project_name:                     Date: %2d/%2d/%2d Time: %2d:%2d:%2d Flight:        \n",head->month,head->day,
head->year,head->data_set_hour,head->data_set_minute,head->data_set_second);
		  for(i=0; i<19; i++)
		    log_chars[i+14] = head->proj_name[i];
		  for(i=0; i<7; i++)
		    log_chars[i+72] = head->flight_num[i];
		  /*	  loggerEvent(log_chars,log_ints,0); */
		  printf("%s",log_chars);

		  /* Second line */
		  dwelltime = wave->repeat_seq_dwel * wave->repeat_seq;
		  rpm = rad_dscr->req_rotat_vel*60.0/360.0;
		  width = wave->chip_width[0] * 16.6666667*1.e-3;
		  sprintf(log_chars,"Pulsing_scheme:                  Dwell: %4.1fms RSpeed %4.1fRPM Chipwidth %3.1fus\n",dwelltime,rpm,width);
		  for(i=0; i<16; i++)
		    log_chars[i+16] = wave->ps_file_name[i];
		  /*		  loggerEvent(log_chars,log_ints,0); */
		  printf("%s",log_chars);

		  /* Third Line */
		  cells = 0;
		  for(i=0; i<cs->num_segments; i++)
		    cells += cs->num_cells[i];
		  sprintf(log_chars,"#IPPs: %2d #Gates: %4d #Parameters: %2d #cells: %4d #Frequencies: %2d\n",rad_dscr->num_ipps_trans, wave->num_gates[0],
rad_dscr->num_parameter_des, cells, rad_dscr->num_freq_trans);
		  /* loggerEvent(log_chars,log_ints,0); */
		  printf("%s",log_chars);

		  /* Write out the needed headers */
		  header_written = 1;
		  new_volume = 1;
		  for(i=0; i<number_of_drives; i++)
		    {
			unschar = drives_to_use[i];

			/* test to make sure drive is ready 
			   (user could have stuck in a new tape */

			drv_stat=tst_unt_rdy(unschar);
			while(drv_stat!=0x80 && timeout < 2000)
			  {
			      printf("Waiting for drive %d to come ready\n",
				     physical_unit[i][j]); 
			      drv_stat=tst_unt_rdy(unschar);
			      timeout++;
			      taskDelay(200);
			  }
			if(timeout == 2000)
			  {
			      printf("ERROR: DRIVE NOT READY, SCSI DRIVE: %2d",
				     drives_to_use[i]);
			      printf(" DRIVE STATUS: %X\n",drv_stat);
			  }

			/* Reset all of the error counters in the tape drive
			   to zero */

			dlt_cmds(LOG_SELECT,unschar);


			vol->volume_num=vol_num;
			printf("WRITING HEADER TO SCSI DRIVE%2d\n",
			       drives_to_use[i]);

			stat = write_tape((unsigned int *)tapeHdr,hdrsz,
				   BLOCKED,unschar);
			if(stat != 0)
			  {
			      tapeStatus->status[current_unit] &= ~RECORDING;
			      printf("WRITE ERROR ON DRIVE %d\n",
				     drives_to_use[i]);
			      printf("WRITE STATUS = %X\n",stat);

			      /* Log this error in the log file */
			      log_ints[3] = drives_to_use[i];
			      log_ints[4] = stat;
			      /* loggerEvent("Tape_Error: %2d/%2d/%2d SCSI_ID: %1d Stat: %4x Standard data wrt\n",log_ints,5); */
			  }
		    }
	      } /* if(number_of_drives > 0) */
	    else
	      {
		  REC_FLAG = 0;
		  tapeStatus->status[current_unit] &= ~RECORDING;
	      }
	} /* if(REC_FLAG != 0 && !header_written) */

      if(REC_FLAG != 0 && header_written)
	{
	    mail_box();
	}
  }/* Infinite for loop for(;;) */ 
}/* End of flag_check */

