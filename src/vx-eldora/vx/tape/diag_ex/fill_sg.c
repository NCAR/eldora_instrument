/*
 *	$ix: mbox.c,v 1.1 1992/08/14 17:50:08 reif Exp reif $
 *
 *	Module:fill_sg.c		 
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.8  1992/11/04  04:52:54  reif
 * *** empty log message ***
 *
 * Revision 1.7  1992/10/30  23:54:45  reif
 * changed to flush mailboxes and ignore zero raycounts
 *
 * Revision 1.6  1992/10/25  17:08:00  reif
 * *** empty log message ***
 *
 * Revision 1.5  1992/10/17  21:32:08  reif
 * changed to global transfer arrays
 *
 * Revision 1.4  1992/10/15  17:52:36  reif
 * *** empty log message ***
 *
 * Revision 1.3  1992/08/24  20:39:16  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/08/19  21:14:00  reif
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  17:50:08  reif
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <cipincl.h>

int fill_sg(unsigned int *tape_data,unsigned int max_read)
{
static unsigned int sg_dscr_blk_cnt = 0;
static unsigned int sg_blk_sz = 0;
static unsigned int sg_xfer_index = 0;
int done,i;
int stat1;
unsigned int blk_wrt;
static unsigned int total=0;
static unsigned int update=0;

if((sg_blk_sz+max_read)>=MAX_BLK_SZ)
  {           /* If over max block size, Write tape and start new SG. */
      done=1; /* Sets address of last dscr blk to -1 */
      sctr_gthr(sg_dscr_blk_cnt,done); /* Write to SG */
      stat1=write_tape((unsigned int *)sg,sg_blk_sz,SCATTER_GATHER,scsi_id[1]);
      total+=sg_blk_sz;
      update+=sg_blk_sz;
      if(update>=10000000)
	{
	    printf("%d TOTAL BYTES WRITTEN\n",total);
	    update=0;
	}
      for(i=0; i<8; i++)    
	{
	    sg_xfer_len[i]=0;  /* Reinit both arrays */
	    sg_xfer_addr[i]=0; /* So we can use them again */
	}
      
      sg_xfer_index=0; /* Reinit index */
      sg_blk_sz=0; /* Clear data accumulator to zero */
      sg_dscr_blk_cnt=0; /* Clear count of descriptor blocks */	
      if(stat1!=0)
	{
	    printf("ERROR WRITING TO SCSI Drive %2d\n",scsi_id[1]);
	    return(blk_wrt=0);
	}
      return(blk_wrt=total);
  }

if(sg_xfer_index>=7) /* If one descriptor block gets built (8 transfers) */
  {
      done=0; /* Data block not done yet */
      /* Send out the 8th record to fill up the block */
      sg_xfer_len[sg_xfer_index]=(0x0D000000+max_read); /* AM plus length for SG */
      sg_xfer_addr[sg_xfer_index]=(int)tape_data; /* Load address of record */
      sg_blk_sz+=max_read; /* Increment amount of data gathered so far */
      sctr_gthr(sg_dscr_blk_cnt,done); /* Write to SG */
      sg_dscr_blk_cnt++; /* Incr. number of descriptor blocks built (5 max) */
      sg_xfer_index=0; /* Reinit index */      
      for(i=0; i<8; i++)
	{
	    sg_xfer_len[i]=0;  /* Reinit both arrays */
	    sg_xfer_addr[i]=0; /* So we can use them again */
	}
      if(sg_dscr_blk_cnt>9) /* If we reach max number of descriptor blocks (10) */ 
	{
	    done=1; /* Means we have one data block */			      
	    stat1=write_tape((unsigned int *)sg,sg_blk_sz,SCATTER_GATHER,
			     scsi_id[1]);
	    total+=sg_blk_sz;
	    sg_blk_sz=0; /* Clear data accumulator to zero */
	    sg_dscr_blk_cnt=0; /* Clear count of descriptor blocks */
	    if(stat1!=0)
	      {
		  printf("ERROR WRITING SCSI Drive: %2d\n",scsi_id[1]);
		  return(blk_wrt=0);
	      }
	    else
	      {
		  printf("WROTE %d BYTES TO SCSI Drive: %2d\n",sg_blk_sz,
			 scsi_id[1]);
		  return(blk_wrt=total);
	      }
	}
      return(blk_wrt=0);
  }
else
  {
      sg_xfer_len[sg_xfer_index]=(0x0D000000+max_read); /* AM plus length for SG */
      sg_xfer_addr[sg_xfer_index]=(int)tape_data; /* Load address of record */
      sg_blk_sz+=max_read; /* Keep track of amount of data in SG */
      sg_xfer_index++; /* Increment length and addr arrays */
      return(blk_wrt=0);
  }

} 









