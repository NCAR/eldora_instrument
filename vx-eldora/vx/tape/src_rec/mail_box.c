/*
 *	$ix: mail_box.c,v 1.1 1992/08/14 17:50:08 reif Exp reif $
 *
 *	Module:	mail_box
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
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

#include <recordIncs.h>
#include <tapeDef.h>
#include <tapeGbl.h>

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
int i, full;
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
static int rad_buff_num = 0;
static int nav_buff_num = 0;
static int ads_buff_num = 0;
static int rad_blk_sz = 0;
static int nav_blk_sz = 0;
static int ads_blk_sz = 0;
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

*sync_word = 0; /* Clear sync word so that next time we will wait on it */

fore_raycnt_errcnt=0;
aft_raycnt_errcnt=0;
rad_buff_num = 0;
sg_init(rad_buff_num);
nav_buff_num = 2;
sg_init(nav_buff_num);
ads_buff_num = 4;
sg_init(ads_buff_num);

rad_blk_sz = 0;
nav_blk_sz = 0;
ads_blk_sz = 0;

fore_mb = (unsigned short *)MAIL_BOX;
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

mb_ptr = (unsigned short *)MAIL_BOX;
for(mb_count=0; mb_count<*num_recs; mb_count++)
  {
      *mb_ptr=0; /*Flush all mailboxes */
      mb_ptr++;
      if((int)mb_ptr > last_mailbox)
	    break;
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
      test_mb = *fore_mb; /* Check for error status from MCPL */
      if(*fore_mb == 0xBFFF)
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
		  if(fore_raycnt != fpd->ray_count)
		    {
			fore_raycnt_errcnt++;
			if(fore_raycnt_errcnt >1) /*First one is not an error*/
			  {
			      tapeStatus->status[current_unit] |=
				RAY_COUNT_PROBLEMS;
			      printf("FORE %d :  %d flushing\n",
				     fpd->ray_count,fore_raycnt);

                 /* Flush out all mail boxes with zeros */

                   mb_ptr = (unsigned short *)MAIL_BOX;
                   for(mb_count=0; mb_count<*num_recs; mb_count++)
                           {
			       *mb_ptr=0; /*Flush all mailboxes */
			       mb_ptr++;
			       if((int)mb_ptr > last_mailbox)
				 break;
			   }

			      /* Look for first occurence of good fore_mb */

			      i = 0;
			      while(*fore_mb!=0xBFFF)
				{
				    fore_mb+=2;
				    fore_addr+=2;
				    /* Test for last mail box */
				    if((int)fore_mb > last_mailbox) 
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
				    /* Test for last mail box */
				    if((int)aft_mb > last_mailbox)
				      {
					aft_mb = (unsigned short *)MAIL_BOX+1;
					aft_addr = (unsigned int *)DATA_RECS+1;
					if(i++ > 2000) break;
				      } 
				}

			 /* Log flushed buffer */
			      loggerEvent("Flushed Buffer (fore) %2d:%2d:%2d\n"
					  , log_ints,3);

			      (unsigned int)fpd=*aft_addr+sizeof(RAY)
				+sizeof(PLATFORM);
			      aft_raycnt=fpd->ray_count;
			      (unsigned int)fpd=*fore_addr+sizeof(RAY)
				+sizeof(PLATFORM);
			      *fore_mb = 0;

			  }
			fore_raycnt=fpd->ray_count;
		    }

		  /* We have a good data ray to record, see if we only
		     record part of it */

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

		  if((rad_blk_sz + rad_rec_length) >= max_blk_sz)
		    {

			/* Write data out to tape */
			drv_sel((unsigned int *)sg[rad_buff_num][0],rad_blk_sz,
				SCATTER_GATHER);
			WRITE_TAPE_STATUS=0;

			rad_blk_sz = 0;

			rad_buff_num += 1;
			if(rad_buff_num > 1)
			  rad_buff_num = 0;

			sg_init(rad_buff_num);
		    }

		  /* Stick this ray in the scatter gather buffer */

		  full = fill_sg(fore_addr,rad_rec_length,rad_buff_num);

		  /* Increment amount of data gathered so far */
		  rad_blk_sz+=rad_rec_length;

		  if(full)
		    {
			/* Write data out to tape */
			drv_sel((unsigned int *)sg[rad_buff_num][0],rad_blk_sz,
				SCATTER_GATHER);
			WRITE_TAPE_STATUS=0;

			rad_blk_sz = 0;

			rad_buff_num += 1;
			if(rad_buff_num > 1)
			  rad_buff_num = 0;

			sg_init(rad_buff_num);

		    }

		  fore_raycnt++; /* Increment fore raycount */

	      } /* END if(!strncmp("RYIB",ray->ray_info,4)) */

/************* NAV DATA HANDLER **********************/
/*** if first four characters are NAVD then it is a navigation data record */

	    else if(!strncmp("NAVD",nav->nav_info_id,4))
	      {
		  printf("NAV RECORD FOUND!!\n");
		  nav_rec_length=nav_length(fore_addr);

		  /* If this record takes us over maximum block size,
		     Write tape and start new Scatter Gather */

		  if((nav_blk_sz + nav_rec_length) >= max_blk_sz)
		    {

			/* Write data out to tape */
			drv_sel((unsigned int *)sg[nav_buff_num][0],nav_blk_sz,
				SCATTER_GATHER);
			WRITE_TAPE_STATUS=0;

			nav_blk_sz = 0;

			nav_buff_num += 1;
			if(nav_buff_num > 3)
			  nav_buff_num = 2;

			sg_init(nav_buff_num);
		    }

		  /* Stick this ray in the scatter gather buffer */

		  full = fill_sg(fore_addr,nav_rec_length,nav_buff_num);

		  /* Increment amount of data gathered so far */
		  nav_blk_sz+=nav_rec_length;

		  if(full)
		    {
			/* Write data out to tape */
			drv_sel((unsigned int *)sg[nav_buff_num][0],nav_blk_sz,
				SCATTER_GATHER);
			WRITE_TAPE_STATUS=0;

			nav_blk_sz = 0;

			nav_buff_num += 1;
			if(nav_buff_num > 3)
			  nav_buff_num = 2;

			sg_init(nav_buff_num);

		    }

	      } /* END if(!strncmp("NAVD",nav->nav_info_id,4)) */

/*********************** ADS DATA HANDLER *********************************/
/****  if first four characters are ISIT then this is an ADS data record **/

	    else if(!strncmp("ISIT",ads->insitu_data_id,4))
	      {
		  printf("ADS RECORD FOUND!!\n");
		  ads_rec_length=ads_length(fore_addr);


		  /* If this record takes us over maximum block size,
		     Write tape and start new Scatter Gather */

		  if((ads_blk_sz + ads_rec_length) >= max_blk_sz)
		    {

			/* Write data out to tape */
			drv_sel((unsigned int *)sg[ads_buff_num][0],ads_blk_sz,
				SCATTER_GATHER);
			WRITE_TAPE_STATUS=0;

			ads_blk_sz = 0;

			ads_buff_num += 1;
			if(ads_buff_num > 5)
			  ads_buff_num = 4;

			sg_init(ads_buff_num);
		    }

		  /* Stick this ray in the scatter gather buffer */

		  full = fill_sg(fore_addr,ads_rec_length,ads_buff_num);

		  /* Increment amount of data gathered so far */
		  ads_blk_sz+=ads_rec_length;

		  if(full)
		    {
			/* Write data out to tape */
			drv_sel((unsigned int *)sg[ads_buff_num][0],ads_blk_sz,
				SCATTER_GATHER);
			WRITE_TAPE_STATUS=0;

			ads_blk_sz = 0;

			ads_buff_num += 1;
			if(ads_buff_num > 5)
			  ads_buff_num = 4;

			sg_init(ads_buff_num);

		    }

	      } /* END if(!strncmp("ISIT",ads->insitu_data_id,4)) */

	    /****  Move the fore mailbox pointer forward */
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
		  if(aft_raycnt_errcnt > 1) /* First few are not errors */
		    {
			tapeStatus->status[current_unit] |= RAY_COUNT_PROBLEMS;
			printf("AFT: %d   %d flushing\n",
			       fpd->ray_count,aft_raycnt);

                 /* Flush out all mail boxes with zeros */

                   mb_ptr = (unsigned short *)MAIL_BOX;
                   for(mb_count=0; mb_count<*num_recs; mb_count++)
                           {
			       *mb_ptr=0; /*Flush all mailboxes */
			       mb_ptr++;
			       if((int)mb_ptr > last_mailbox)
				 break;
			   }

			      /* Look for first occurence of good fore_mb */

			      i = 0;
			      while(*fore_mb!=0xBFFF)
				{
				    fore_mb+=2;
				    fore_addr+=2;
				    /* Test for last mail box */
				    if((int)fore_mb > last_mailbox) 
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
				    /* Test for last mail box */
				    if((int)aft_mb > last_mailbox)
				      {
					aft_mb = (unsigned short *)MAIL_BOX+1;
					aft_addr = (unsigned int *)DATA_RECS+1;
					if(i++ > 2000) break;
				      } 
				}

			 /* Log flushed buffer */
			      loggerEvent("Flushed Buffer (aft) %2d:%2d:%2d\n",
					  log_ints,3);
			(unsigned int)fpd=*fore_addr+sizeof(RAY)
			  +sizeof(PLATFORM);
			fore_raycnt = fpd->ray_count;
			(unsigned int)fpd=*aft_addr+sizeof(RAY)
			  +sizeof(PLATFORM);
			*aft_mb = 0;
		    }

		  aft_raycnt=fpd->ray_count;
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

		  /* If this record takes us over maximum block size,
		     Write tape and start new Scatter Gather */

		  if((rad_blk_sz + rad_rec_length) >= max_blk_sz)
		    {

			/* Write data out to tape */
			drv_sel((unsigned int *)sg[rad_buff_num][0],rad_blk_sz,
				SCATTER_GATHER);
			WRITE_TAPE_STATUS=0;

			rad_blk_sz = 0;

			rad_buff_num += 1;
			if(rad_buff_num > 1)
			  rad_buff_num = 0;

			sg_init(rad_buff_num);
		    }

		  /* Stick this ray in the scatter gather buffer */

		  full = fill_sg(aft_addr,rad_rec_length,rad_buff_num);

		  /* Increment amount of data gathered so far */
		  rad_blk_sz+=rad_rec_length;

		  if(full)
		    {
			/* Write data out to tape */
			drv_sel((unsigned int *)sg[rad_buff_num][0],rad_blk_sz,
				SCATTER_GATHER);
			WRITE_TAPE_STATUS=0;

			rad_blk_sz = 0;

			rad_buff_num += 1;
			if(rad_buff_num > 1)
			  rad_buff_num = 0;

			sg_init(rad_buff_num);

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
	    loggerEvent("Tape_Error: %2d/%2d/%2d SCSI_ID: %1d Stat: %4x Ending header wrt\n",log_ints,5);
	      }
      printf("WROTE ENDING HEADER TO %d\n",drives_to_use[i]);
      exb_cmds(WRITE_FILEMARK,WRT_FLMK,unschar);
      printf("WROTE ENDING FILEMARK TO %d\n",drives_to_use[i]);
  }
/* Log end of volume in the log file */
log_ints[3] = vol_num;
log_ints[4] = tape_num;
loggerEvent("Volume_End: %2d/%2d/%2d Vol#: %3d Tape#: %2d Operator intervention\n",log_ints,5);
new_volume = 1;

return; 
}





