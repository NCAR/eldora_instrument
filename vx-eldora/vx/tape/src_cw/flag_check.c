/*
 *	$Id$
 *
 *	Module:		 
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.9  1993/09/22  16:21:08  reif
 * changing to accept command line arguments and new flag checking
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
#include <cipincl.h>

void flag_check(char drv0,char drv1,char drv2,char drv3)
{

int i, j, first_found, second_found, header_written, current_unit;
unsigned char sgflg, unschar, drv_stat;

union{
    int sqr[2][2];
    int lin[4];
}health;

vol=(VOLUME *)tapeHdr;

/********* INIT RECORDING LOG FILE **********/

loggerInit(RECORD_LOG);

/* INITIALIZE GOOD DRIVE ARRAY FROM COMMAND LINE */

health.lin[0]=drv0;
health.lin[1]=drv1;
health.lin[2]=drv2;
health.lin[3]=drv3;

/* INITIALIZE THE PHYSICAL SCSI BUS UNIT NUMBER OF EACH DRIVE */

physical_unit[0][0] = 0;
physical_unit[0][1] = 1;
physical_unit[1][0] = 2;
physical_unit[1][1] = 3;

/* INITIALIZE THE TAPE CONTROL FLAGS */

UNIT_NUM=0;
PARALLEL_REC=1;
PP_FLAG=1;
RUN_FLAG=0;
REC_FLAG=0;
REWIND_FLAG=0;
UNLOAD_FLAG=0;
WRITE_TAPE_STATUS=0;
vol_num=-1;
tape_num=1;
header_written = 0;

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
      PARALLEL_REC=0;
      PP_FLAG=0;
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
      PARALLEL_REC=0;
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
      tapeStatus->drives_in_use[i][0] = record_sys[i][0];
      tapeStatus->drives_in_use[i][1] = record_sys[i][1];
      tapeStatus->number_of_drives[i] = record_sys[i][2];
      tapeStatus->terr[i].error = 0;
      tapeStatus->serr[i].error = 0;
      tapeStatus->status[i] |= INITIALIZING;
      tapeStatus->status[i] |= PING_PONG_ON;
      if(!PP_FLAG) tapeStatus->status[i] &= ~PING_PONG_ON;
      tapeStatus->status[i] |= PARALLEL_ON;
      if(!PARALLEL_REC) tapeStatus->status[i] &= ~PARALLEL_ON;
  }

/*  Set other status words to indicate current conditions */

tapeStatus->failures = 0;
tapeStatus->attempts = 0;
tapeStatus->eot_warning = 0;
tapeStatus->unit = 0;

/* INITIALIZE STRUCTURES */

parm_blk_init();
md_sel_init();
rqst_sns_init();
sg_init();
md_sns_init();

/* INITIALIZE CIPRICO CARD */

unschar = physical_unit[0][0];
cip_cmds(GEN_OPS,GOPS,unschar);  /* INITIALIZE CIPRICO GENERAL OPTIONS */    
cip_cmds(UNIT_OPS,UOPS,unschar); /* INITIALIZE CIPRICO UNIT OPTIONS */

/* Initialize all healthy drives and write EOF's on all of them */

for(i=0; i<2; i++)
  {
      for(j=0; j<2; j++)
	{
	    if(health.sqr[i][j]) /* IF DRIVE IS HEALTHY, INITIALIZE IT,
				    THEN WRITE EOF */
	      {
		  unschar = physical_unit[i][j];
		  starting_amnt[i]=drive_init(unschar);
		  printf("PHYSICAL DRIVE %d HAS BEEN INITIALIZED\n",
			 physical_unit[i][j]);
		  drv_stat=tst_unt_rdy(unschar);
		  while(drv_stat!=0x80)
		    {
			drv_stat=tst_unt_rdy(unschar);
			loggerEvent("%d ERROR= %X\n",physical_unit[i][j],
				    &drv_stat,2);
			taskDelay(200);
		    }
		  exb_cmds(WRITE_FILEMARK,WRT_FLMK,unschar);
		  printf("WROTE BEGINNING FILEMARK TO %d\n",
			 physical_unit[i][j]);
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

/********************************************************************/
/**********   Start of infinite run time loop         ***************/
/********************************************************************/

for(;;)
  {
      /* While we are stopped we should check rewind and unload flags */

      current_unit = UNIT_NUM;
      while(RUN_FLAG == 0)
	{
	    printf("WAITING FOR SYSTEM TO START\r");
	    header_written = 0;

	    if(UNLOAD_FLAG==1)
	      {
		UNLOAD_FLAG = 0;
		tapeStatus->status[current_unit] |= EJECTING;
		for(i=0; i<number_of_drives; i++)
		  {
		      unschar = drives_to_use[i];
		      exb_cmds(UNLOAD,ULD,unschar);
		  }
	      }

	    if(REWIND_FLAG==1)
	      {
		REWIND_FLAG = 0;
		tapeStatus->status[current_unit] |= REWINDING;
		for(i=0; i<number_of_drives; i++)
		  {
		      unschar = drives_to_use[i];
		      exb_cmds(REWND,RWND,unschar);
		  }
	      }
	    taskDelay(60);
	}

/*  We have been told to go, but we must first check to see if the record
    flag is set, if not this is an error */

      while(REC_FLAG == 0 && RUN_FLAG != 0)
	{
	    printf("ERROR: RECORD FLAG NOT ON\r");
	    tapeStatus->status[0] &= ~RECORDING;
	    tapeStatus->status[1] &= ~RECORDING;
	    taskDelay(60);
	}

/* REC_FLAG is now on, or we have been told to stop.  If we are going
   to record, we need to setup the array drives_to_use and the variable
   number_of_drives, based on all the commands.  If we have been told
   to stop, just go back up and wait for the run flag again  */

      if(REC_FLAG != 0 && !header_written)
	{
	    number_of_drives = record_sys[UNIT_NUM][2];
	    tapeStatus->status[UNIT_NUM] = 0;

	    /* If we have drive(s) to record on great
	       else flag error and quit */

	    if(number_of_drives > 0)
	      {
		  if(PARALLEL_REC != 0 && number_of_drives == 2)
		    tapeStatus->status[UNIT_NUM] |= PARALLEL_ON;
		  else if(PARALLEL_REC == 0 || number_of_drives == 1)
		    {
			number_of_drives == 1;
			tapeStatus->status[UNIT_NUM] &= ~PARALLEL_ON;
		    }

		  drives_to_use[0] = record_sys[UNIT_NUM][0];
		  drives_to_use[1] = record_sys[UNIT_NUM][1];

		  tapeStatus->status[UNIT_NUM] |= RECORDING;

		  if(PP_FLAG != 0)
		    tapeStatus->status[UNIT_NUM] |= PING_PONG_ON;
		  else if(PP_FLAG == 0)
		    tapeStatus->status[UNIT_NUM] &= ~PING_PONG_ON;

		  tapeStatus->failures = 0;
		  tapeStatus->attempts = 0;
		  tapeStatus->eot_warning = 0;
		  tapeStatus->unit = UNIT_NUM;
		  tapeStatus->terr[UNIT_NUM].error = 0;
		  tapeStatus->serr[UNIT_NUM].error = 0;

		  vol_num++;
		  tape_header();
		  rad_dscr = GetRadar(inHeader,1);
		  cs = GetCellSpacing(inHeader);
		  header_written = 1;

		  for(i=0; i<number_of_drives; i++)
		    {
			unschar = drives_to_use[i];
			sgflg=HEADER;
			vol->volume_num=vol_num;
			write_tape((unsigned int *)tapeHdr,hdrsz,
				   sgflg,unschar);
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
	      REC_FLAG = 0;
	} /* if(REC_FLAG != 0 && !header_written) */

      if(REC_FLAG != 0 && header_written)
	mail_box();

  }/* Infinite for loop for(;;) */ 
}/* End of flag_check */
