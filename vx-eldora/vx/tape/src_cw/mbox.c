/*
 *	$ix: mbox.c,v 1.1 1992/08/14 17:50:08 reif Exp reif $
 *
 *	Module:	mail_box
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1994/01/06  21:31:27  craig
 * Initial revision
 *
 * Revision 1.10  1993/09/22  17:12:46  reif
 * *** empty log message ***
 *
 * description: This routine writes all data out to tape.  It begins by
 *              waiting for the mailbox area to be initialized.  Then
 *              it flushes the mailboxes and begins to follow them 
 *              through.  This will happen until the stop command is
 *              received or if drv_sel sences an end-of-tape and turns
 *              off the record flag.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>

/******* START CHECKING MAILBOXES AND WRITING DATA ********/

void mail_box()
{

unsigned int *sync_word;
static unsigned int *num_recs;
static unsigned int *fore_addr;
static unsigned int *aft_addr;
static unsigned short *fore_mb;
static unsigned short *aft_mb;
unsigned short *mb_ptr;
int i;
long mb_count;
RAY *ray;
PLATFORM *pltfrm;
FIELDPARAMDATA *fpd;
INSITU_DATA *ads;
NAV_INFO *nav;
static unsigned int rad_rec_length;
static unsigned int full_rec_length;
unsigned char unschar;
unsigned int nav_rec_length;
unsigned int ads_rec_length;
static int rad_dscr_blk_cnt = 0;
static int nav_dscr_blk_cnt = 0;
static int ads_dscr_blk_cnt = 0;
static unsigned int rad_blk_sz = 0;
static unsigned int nav_blk_sz = 0;
static unsigned int ads_blk_sz = 0;
static int rad_xfer_index = 0;
static int nav_xfer_index = 0;
static int ads_xfer_index = 0;
int done;
static int fore_raycnt=0;
static int fore_raycnt_errcnt=0;
static int aft_raycnt=0;
static int aft_raycnt_errcnt=0;
short type;
int keep;
static long last_mailbox;
unsigned short test_mb;
char stat;

sync_word=(unsigned int *)MAD_BASE; /* Tells us if data path is functional */

if(*sync_word != 0x55555555)
  {
      /* Notify Operator of error and return to flag_check */
      printf("DATA PATH SHOWS BAD SYNC WORD!!!\r");
      return;
  }

/* Sync word now indicates good path status */

*sync_word=0; /* Clear sync word so that next time we will wait on it */
fore_raycnt_errcnt=0;
aft_raycnt_errcnt=0;
rad_dscr_blk_cnt = 0;
nav_dscr_blk_cnt = 0;
ads_dscr_blk_cnt = 0;
rad_blk_sz = 0;
nav_blk_sz = 0;
ads_blk_sz = 0;
rad_xfer_index = 0;
nav_xfer_index = 0;
ads_xfer_index = 0;
fore_mb = (unsigned short *)MAIL_BOX;
mb_ptr = fore_mb;
aft_mb = fore_mb + 1;
fore_addr = (unsigned int *)DATA_RECS;
aft_addr = fore_addr + 1;
num_recs=(unsigned int *)NUM_RECS; /* How many records to scan for */
last_mailbox=MAIL_BOX + *num_recs*2-2;

rad_rec_length=data_ray_length(); /* Calculate size of radar record */
full_rec_length=rad_rec_length; /*save full record size for future use*/

printf("RADAR RECORD LENGTH = %d BYTES\n",rad_rec_length);
printf("HEADER SIZE = %d BYTES\n",hdrsz);
printf(" Will Flush the mailboxes now\n");

/* Flush out all mail boxes with zeros */

for(mb_count=0; mb_count<last_mailbox; mb_count++)
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

/* Look for first occurence of good aft_mb */

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

/*************************************************************/
/****   initialization is complete, begin to record data *****/
/*************************************************************/

do
  {
      test_mb=*fore_mb; /* Check for error status from MCPL */
      if(*fore_mb==0xBFFF)
	{
	    *fore_mb=0x0000; /* Zero mailbox */
	    test_mb=0; /* Clear test */
	    (unsigned int)ray=*fore_addr; /* Make radar data pointer */
	    (unsigned int)nav=*fore_addr; /* Make nav data pointer */
	    (unsigned int)ads=*fore_addr; /* Make ads data pointer */

/************* FORE RADAR DATA HANDLER ********************/

	    /* If first 4 characters are RYIB, then this is a regular
	       fore radar data record */

	    if(!strncmp("RYIB",ray->ray_info,4))
	      {
		  log_ints[0]=ray->hour;
		  log_ints[1]=ray->minute;
		  log_ints[2]=ray->second;

		  (unsigned int)fpd=*fore_addr+sizeof(RAY)+sizeof(PLATFORM);

		  if(fpd->ray_count < 5)  /* first few records have bad data */
		    {
			fore_mb+=2;
			fore_addr+=2;
			if((int)fore_mb > last_mailbox)
			  {
			      fore_mb = (unsigned short *)MAIL_BOX;
			      fore_addr = (unsigned int *)DATA_RECS;
			  }

			continue;
		    }
		  if(fore_raycnt!=fpd->ray_count)
		    {
			fore_raycnt_errcnt++;
			if(fore_raycnt_errcnt >2) /*First few are not errors */
			  {
			      tapeStatus->status[current_unit] |=
				RAY_COUNT_PROBLEMS;
			      printf("FORE IS %d SHOULD BE %d\n",
				     fpd->ray_count,fore_raycnt);
			fore_raycnt=fpd->ray_count;
			/* Log and report bad fore radar ray count */
			  }
		    }
		  type=rad_dscr->data_reduction;

		  keep=reduce_data(type,fore_addr);
		  if(keep==0)
		    {
			fore_mb+=2;
			fore_addr+=2;
			fore_raycnt++;
			if((int)fore_mb > last_mailbox)
			  {
			      fore_mb = (unsigned short *)MAIL_BOX;
			      fore_addr = (unsigned int *)DATA_RECS;
			  }

			continue;
		    }
		  else if((keep != 1) && (keep != 0))
		    {
			if(keep < min_to_record || keep > full_rec_length)
			  {
			      printf("Data reduction error keep = %d\n",keep);
			      keep = full_rec_length;
			  }
			rad_rec_length=keep;
		    }		
		  else
		    {
			rad_rec_length=full_rec_length;
		    }

		  /* If this record takes us over maximum block size,
		     Write tape and start new Scatter Gather */

		  if((rad_blk_sz + rad_rec_length)>=MAX_BLK_SZ)
		    {
			done=1; /* Sets address of last dscr blk to -1 */
			rad_sctr_gthr(rad_dscr_blk_cnt,done); /* Write to SG */
	
			/* Write data out to tape */
			drv_sel((unsigned int *)rad_sg,rad_blk_sz,
				SCATTER_GATHER);
			WRITE_TAPE_STATUS=0;

			/* Clear the temporary radar length and address arrays
			   and the various indexes and counters */

			for(i=0; i<8; i++)    
			  {
			      rad_xfer_len[i]=0;  /* Reinit both arrays */
			      rad_xfer_addr[i]=0; /* So we can use them again*/
			  }
			rad_xfer_index=0; /* Reinit index */
			rad_blk_sz=0; /* Clear data accumulator to zero */
			rad_dscr_blk_cnt=0;/*Clear descriptor block count */
		    }
		  
		  /* Check to see if a descriptor block is full */

		  if(rad_xfer_index >= 7)
		    {

			/* Write present address and length into the
			   last entry of the temporary arrays
			    NOTE: Address Modifier plus length for SG*/
			rad_xfer_len[rad_xfer_index]=(0x0D000000+
						      rad_rec_length);
			rad_xfer_addr[rad_xfer_index]=*fore_addr;

			/* Increment amount of data gathered so far */
			rad_blk_sz+=rad_rec_length;

			/* Copy temporary arrays to the scatter-gather
			   structure NOTE: rad_sctr_gthr will set last
			   address to -1 if this is last descriptor block */
			done=0; /* Data block not done yet */
			rad_sctr_gthr(rad_dscr_blk_cnt,done);

			/* Increment number of descriptor blocks built */
			rad_dscr_blk_cnt++;

			/* Reinitialize index and both arrays */
			
			rad_xfer_index=0;
			for(i=0; i<8; i++)
			  {
			      rad_xfer_len[i]=0;
			      rad_xfer_addr[i]=0;
			  }
			
	/* Have we reached the max number of descriptor blocks? */

			if(rad_dscr_blk_cnt > MAX_RAD_DSCR_BLK) 
			  {
			      /* Write data out to tape */
			      drv_sel((unsigned int *)rad_sg,rad_blk_sz,
				      SCATTER_GATHER);
			      WRITE_TAPE_STATUS=0;

			       /* Clear data accumulator to zero */
			      rad_blk_sz=0;

			      /* Clear count of descriptor blocks */
			      rad_dscr_blk_cnt=0;
			  }
		    } /* if descriptor block is full */

		  else  /* nothing special just add to the temporary arrays */
		    {
			/* fill length array with address modifier plus actual
			   length */
			rad_xfer_len[rad_xfer_index]=(0x0D000000+
						      rad_rec_length);

			/* Fill address array with starting address of data */
			rad_xfer_addr[rad_xfer_index]=*fore_addr;

			/* Keep track of total amount of data in the scatter
			   gather */
			rad_blk_sz+=rad_rec_length;

			/* Increment index into length and address arrays */
			rad_xfer_index++;
		    }

		  fore_raycnt++; /* Increment fore raycount */

	      } /* END if(!strncmp("RYIB",ray->ray_info,4)) */

/************* NAV DATA HANDLER **********************/
/*** if first four characters are NAVD then it is a navigation data record */

	    else if(!strncmp("NAVD",nav->nav_info_id,4))
	      {
		  printf("NAV RECORD FOUND!!\n");
		  nav_rec_length=nav_length(fore_addr);

		  /* Will this record take us over the maximum block size? */

		  if((nav_blk_sz + nav_rec_length)>=MAX_BLK_SZ)
		    {
			/* Copy temporary arrays to the scatter-gather
			   structure */
			done=1; /* Sets address of last dscr blk to -1 */
			nav_sctr_gthr(nav_dscr_blk_cnt,done);

			/* Write entire navigation scatter-gather to tape */
			drv_sel((unsigned int *)nav_sg,nav_blk_sz,
				SCATTER_GATHER);
			WRITE_TAPE_STATUS=0;

			/* Reinitialize nav length and address arrays
			   and their index */

			for(i=0; i<8; i++)    
			  {
			      nav_xfer_len[i]=0;
			      nav_xfer_addr[i]=0;
			  }		
			nav_xfer_index=0;

			nav_blk_sz=0; /* Clear data accumulator to zero */
			nav_dscr_blk_cnt=0; /* Clear descriptor block count */
		    }
		  
		  /* Is this the last record of a descriptor block? */

		  if(nav_xfer_index>=7)
		    {

			/* Write this record's length and address into the
			   temporary arrays, NOTE: always need to add address
			   modifier to length */

			nav_xfer_len[nav_xfer_index]=(0x0D000000+
						      nav_rec_length);
			nav_xfer_addr[nav_xfer_index]=*fore_addr;

			 /* Increment amount of data gathered so far */
			nav_blk_sz+=nav_rec_length;

			/*Copy temporary arrays into scatter-gather structure
			 NOTE: this will also put -1 into the next address if
			 the maximum descriptor block count has been met */

			done=0; /* Data block not done yet */
			nav_sctr_gthr(nav_dscr_blk_cnt,done);

			 /* Increment number of descriptor blocks built */
			nav_dscr_blk_cnt++;

			/* Reinitialize index and the temporary address
			   and length arrays */
			nav_xfer_index=0;
			for(i=0; i<8; i++)
			  {
			      nav_xfer_len[i]=0;
			      nav_xfer_addr[i]=0;
			  }
			
			/* Have we reached the maximum number of navigation
			   data descriptor blocks? */

			if(nav_dscr_blk_cnt>MAX_NAV_DSCR_BLK)
			  {
			      /* Write the data out to tape 
			      drv_sel((unsigned int *)nav_sg,nav_blk_sz,
				      SCATTER_GATHER);
			      WRITE_TAPE_STATUS=0;

			      /* Clear block size accumulation and descriptor
				 block counter */
			      nav_blk_sz=0;
			      nav_dscr_blk_cnt=0;
			  }
		    } /* if last record in a descriptor block */

		  else  /* nothing special just fill temporary length
			   and address array */
		    {
			nav_xfer_len[nav_xfer_index]=(0x0D000000+
						      nav_rec_length);

			nav_xfer_addr[nav_xfer_index]=*fore_addr;

			 /* Keep track of total amount of data */
			nav_blk_sz+=nav_rec_length;

			 /* Increment length and address array index */
			nav_xfer_index++;
		    }

	      } /* END if(!strncmp("NAVD",nav->nav_info_id,4)) */

/*********************** ADS DATA HANDLER *********************************/
/****  if first four characters are ISIT then this is an ADS data record **/

	    else if(!strncmp("ISIT",ads->insitu_data_id,4))
	      {
		  printf("ADS RECORD FOUND!!\n");
		  ads_rec_length=ads_length(fore_addr);

		  /* Will this record put us over maximum block size? */
		  if((ads_blk_sz + ads_rec_length)>=MAX_BLK_SZ)
		    {

			/* Copy temporary arrays into the scatter-gather
			   structure */
			done=1; /* Sets address of last dscr blk to -1 */
			ads_sctr_gthr(ads_dscr_blk_cnt,done);

			/* Write the data out to the tape */
			drv_sel((unsigned int *)ads_sg,ads_blk_sz,
				SCATTER_GATHER);
			WRITE_TAPE_STATUS=0;

			/* Clear out the temporary arrays and their index */
			for(i=0; i<8; i++)    
			  {
			      ads_xfer_len[i]=0;
			      ads_xfer_addr[i]=0;
			  }
			ads_xfer_index=0; /* Reinit index */

			 /* Clear data accumulator to zero */
			ads_blk_sz=0;

			 /* Clear count of descriptor blocks */
			ads_dscr_blk_cnt=0;
		    }

		  /*  Is this the last record in a descriptor block? */
		  
		  if(ads_xfer_index >= 7)
		    {

			/* Fill the last entry in temporary address and length
			   arrays with this record's address and length
			   NOTE: add address modifier to the length */
			ads_xfer_len[ads_xfer_index]=(0x0D000000+
						      ads_rec_length);
			ads_xfer_addr[ads_xfer_index]=*fore_addr;

			 /* Increment amount of data gathered so far */
			ads_blk_sz+=ads_rec_length;

			/* Copy the temporary arrays into the scatter-gather
			   structure. NOTE: if this is last descriptor block,
			   last address will be set to a -1 */
			done=0; /* Data block not done yet */
			ads_sctr_gthr(ads_dscr_blk_cnt,done);

			 /* Increment count of descriptor blocks built */
			ads_dscr_blk_cnt++;

			/* Reinitialize temporary arrays and their index */

			ads_xfer_index=0;			
			for(i=0; i<8; i++)
			  {
			      ads_xfer_len[i]=0;
			      ads_xfer_addr[i]=0;
			  }

			/* Have we reached the maximum number of
			   descriptor blocks */

			if(ads_dscr_blk_cnt > MAX_ADS_DSCR_BLK)
			  {

			      /* Write data out to the tape */
			      drv_sel((unsigned int *)ads_sg,ads_blk_sz,
				      SCATTER_GATHER);
			      WRITE_TAPE_STATUS=0;

			       /* Clear data accumulation counter to zero */
			      ads_blk_sz=0;

			       /* Clear count of descriptor blocks */
			      ads_dscr_blk_cnt=0;
			  }
		    } /* if last record in a descriptor block */

		  else  /* Record is nothing special, just fill temporary
			   arrays with this record's address and length */
		    {
			ads_xfer_len[ads_xfer_index]=(0x0D000000+
						      ads_rec_length);
			ads_xfer_addr[ads_xfer_index]=*fore_addr;

			 /* Keep track of amount of data in Scatter-gather */
			ads_blk_sz+=ads_rec_length;

			/* increment index into temporary arrays */
			ads_xfer_index++;
		    }

	      } /* END if(!strncmp("ISIT",ads->insitu_data_id,4)) */

	    fore_mb+=2; /* Increment data record address */
	    fore_addr+=2; /* Increment mailbox */

	    if((int)fore_mb > last_mailbox) /* Test for last mail box */
	      {
		  /* Set flag to cause drv_sel to update tape status
		     the next time it is called */
		  WRITE_TAPE_STATUS=1;

		  fore_mb = (unsigned short *)MAIL_BOX;
		  fore_addr = (unsigned int *)DATA_RECS;
	      }      

	} /* END if (*fore_mb==0xBFFF) */

      if(test_mb!=0) /* If not 0 or BFFF then its an error */
	{
	    /* Write what test_mb equals to log file */

	    tapeStatus->status[current_unit] |= MCPL_PROBLEMS;
	    printf("FORE MB in error = %X\n",fore_mb); 
	    test_mb=0; /* Clear test_mb */
	    *fore_mb=0; /* Clear fore mailbox */
	    fore_mb+=2;
	    fore_addr+=2;
	    if((int)fore_mb > last_mailbox)
	      {
		  fore_mb = (unsigned short *)MAIL_BOX;
		  fore_addr = (unsigned int *)DATA_RECS;
	      }
	} /* Move on to aft mb */

/******************* AFT RADAR DATA HANDLER ***********************/

      test_mb=*aft_mb;
      if(*aft_mb==0xbfff)
	{
	    *aft_mb=0x0000; /* Zero mailbox */
	    test_mb=0;

	    (unsigned int)ray=*aft_addr; /* Make radar data pointer */
	    log_ints[0]=ray->hour;
	    log_ints[1]=ray->minute;
	    log_ints[2]=ray->second;

	    (unsigned int)fpd=*aft_addr+sizeof(RAY)+sizeof(PLATFORM);

	    if(fpd->ray_count < 5) /* First few rays contain bad data */
	      {
		  aft_mb+=2;
		  aft_addr+=2;
		  if((int)aft_mb > last_mailbox)
		    {
			aft_mb = (unsigned short *)MAIL_BOX + 1;
			aft_addr = (unsigned int *)DATA_RECS +1;
		    }

		  continue;
	      }

	    if(aft_raycnt!=fpd->ray_count)
	      {
		  aft_raycnt_errcnt++;
		  if(aft_raycnt_errcnt > 2) /* First few are not errors */
		    {
			tapeStatus->status[current_unit] |= RAY_COUNT_PROBLEMS;
			printf("AFT IS %d SHOULD BE %d\n",
			       fpd->ray_count,aft_raycnt);
			aft_raycnt=fpd->ray_count;
			/* Log a bad aft ray-count event */
		    }
	      }

	    /* Perform required data reduction algorithm (or non at all) */

	    type=rad_dscr->data_reduction;
	    keep=reduce_data(type,aft_addr);
	    if(keep==0)
	      {
		  aft_mb+=2;
		  aft_addr+=2;
		  aft_raycnt++;
		  if((int)aft_mb > last_mailbox)
		    {
			aft_mb = (unsigned short *)MAIL_BOX + 1;
			aft_addr = (unsigned int *)DATA_RECS +1;
		    }

		  continue;
	      }
	    else if((keep != 1) && (keep != 0))
	      {
		  if(keep < min_to_record || keep > full_rec_length)
		    {
			printf("Data reduction error keep = %d\n",keep);
			keep = full_rec_length;
		    }
		  rad_rec_length=keep;
	      }
	    else
	      {
		  rad_rec_length=full_rec_length;
	      }

	    /* Will this record put us over the maximum block size ? */

		  if((rad_blk_sz + rad_rec_length)>=MAX_BLK_SZ)
		    {

			/* Copy the temporary length and address array over to
			   the scatter gather structure */

			done=1; /* Sets address of last dscr blk to -1 */
			rad_sctr_gthr(rad_dscr_blk_cnt,done);

			/* Write the data out to tape */
			drv_sel((unsigned int *)rad_sg,rad_blk_sz,
				SCATTER_GATHER);
			WRITE_TAPE_STATUS=0;

			/* Clear the temporary length and address arrays and
			   their index */
			for(i=0; i<8; i++)    
			  {
			      rad_xfer_len[i]=0;
			      rad_xfer_addr[i]=0;
			  }
			rad_xfer_index=0;

			 /* Clear data size accumulation to zero */
			rad_blk_sz=0;

			 /* Clear count of descriptor blocks */
			rad_dscr_blk_cnt=0;
		    }

	    /* Is this the last record of a descriptor block? */
	    
		  if(rad_xfer_index>=7)
		    {
			/* Fill the temporary address and length arrays with
			   this record's address and length */
			rad_xfer_len[rad_xfer_index]=(0x0D000000+
						      rad_rec_length);
			rad_xfer_addr[rad_xfer_index] = *aft_addr;

			 /* Increment amount of data gathered so far */
			rad_blk_sz+=rad_rec_length;

			/* Copy the temporary arrays to the scatter-gather
			   structure NOTE: if this is the last descriptor
			   block, the next address will be set to -1 */

			done=0; /* Data block not done yet */
			rad_sctr_gthr(rad_dscr_blk_cnt,done);

			 /* Increment the descriptor block counter */
			rad_dscr_blk_cnt++;

			/* Clear the temporary arrays and their index */

			rad_xfer_index=0;
			for(i=0; i<8; i++)
			  {
			      rad_xfer_len[i]=0;
			      rad_xfer_addr[i]=0;
			  }

			/* have we reached the maximum number of descriptor
			   blocks that can be used? */

			if(rad_dscr_blk_cnt>MAX_RAD_DSCR_BLK)
			  {
			      /* Write all of the data out to tape */
			      drv_sel((unsigned int *)rad_sg,rad_blk_sz,
				      SCATTER_GATHER);
			      WRITE_TAPE_STATUS=0;

			      /* Clear data size accumulation to zero */
			      rad_blk_sz=0;

			       /* Clear count of descriptor blocks */
			      rad_dscr_blk_cnt=0;
			  }
		    } /* if last record of a descriptor block */

		  else  /* record is nothing special, so enter its address
			   and size in the temporary arrays */
		    {	  
			rad_xfer_len[rad_xfer_index]=(0x0D000000+
						      rad_rec_length);
			rad_xfer_addr[rad_xfer_index]=*aft_addr;

			 /* Keep track of amount of data in Scatter-Gather */
			rad_blk_sz+=rad_rec_length;

			 /* Increment index into length and address arrays */
			rad_xfer_index++;
		    }

	    aft_mb+=2; /* Increment data record address */
	    aft_addr+=2; /* Increment mailbox */
	    aft_raycnt++; /* Increment aft raycount */

	    /* Are we at the last mailbox of the MAD area? */

	    if((int)aft_mb > last_mailbox)
	      {
		  /* Set flag to cause drv_sel to set the status word the
		     next time it is called */
		  WRITE_TAPE_STATUS=1;
		  aft_mb = (unsigned short *)MAIL_BOX + 1;
		  aft_addr = (unsigned int *)DATA_RECS + 1;
	      }
	    
	} /* END if (*aft_mb==0xBFFF) */

      if(test_mb!=0)
	{
	    /* Write to log file what test_mb equals */
	    tapeStatus->status[current_unit] |= MCPL_PROBLEMS;
	    printf("AFT MB in error = %X\n",test_mb);
	    test_mb=0; /* Clear test */
	    *aft_mb=0; /*Clear mailbox */
	    aft_mb+=2;
	    aft_addr+=2;

	    if((int)aft_mb > last_mailbox)
	      {
		  aft_mb = (unsigned short *)MAIL_BOX + 1;
		  aft_addr = (unsigned int *)DATA_RECS +1;
	      }
	} /* Move on to next mail box */

}while(RUN_FLAG != 0);

/* Write headers and EOFs to all active drives */

for(i=0; i<number_of_drives; i++)
  {
      unschar = drives_to_use[i];
      stat = write_tape((unsigned int *)tapeHdr,hdrsz,HEADER,unschar);
      if(stat != 0)
	{
	    printf("WRITE ERROR ON DRIVE %d\n",drives_to_use[i]);
	    printf("WRITE STATUS = %X\n",stat);

	    /* Log this error in the log file */
	    log_ints[3] = drives_to_use[i];
	    log_ints[4] = stat;
	    loggerEvent("Tape_Error Time: %2d/%2d/%2d SCSI_ID: %1d Status: %4x Error ending header write\n",log_ints,5);
	      }
      printf("WROTE ENDING HEADER TO %d\n",drives_to_use[i]);
      exb_cmds(WRITE_FILEMARK,WRT_FLMK,unschar);
      printf("WROTE ENDING FILEMARK TO %d\n",drives_to_use[i]);
  }
/* Log end of volume in the log file */
log_ints[3] = vol_num;
log_ints[4] = tape_num;
loggerEvent("Volume_End Time: %2d/%2d/%2d Volume#: %3d Tape#: %2d Operator stopped system, or turned off recording\n",log_ints,5);
new_volume = 1;

return; 
}





