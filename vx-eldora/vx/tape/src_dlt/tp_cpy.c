/*
 *	$Id$
 *
 *	Module:tp_cpy.c		 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:  Copies tape from one to another
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <tapeDef.h>
#include <tapeGbl.h>

void tp_cpy(unsigned int *tape_data,unsigned int max_read)
{
int stat0,stat1;
int max_write;
unsigned int total_data=0;
unsigned int update=0;
unsigned int blk_wrt;
int mode_selected=0;
int DATA, DATA_RECS;

union
  {
      int sense_data;
      char sns_array[4];
  }sns;

while(1)
  {
      stat0=read_tape(tape_data,max_read,DATA,scsi_id[0]);
      if(stat0==1)
	{
	    dlt_cmds(REQUEST_SENSE,scsi_id[0]);
	    if((rqst_sen->sense_key&0x20)==0x20)
	      {
		  sns.sns_array[0]=rqst_sen->info_msb;
		  sns.sns_array[1]=rqst_sen->info_mid_high;
		  sns.sns_array[2]=rqst_sen->info_mid_low;
		  sns.sns_array[3]=rqst_sen->info_lsb;
		  if(sns.sense_data>0)
		    max_write=max_read-sns.sense_data;
		  else
		    max_write=-(sns.sense_data-max_read);
		  max_read=max_write;
		  if(max_read<=1000)
		    {
			if(mode_selected==0)
			  {
			      dlt_cmds(MODE_SELECT,scsi_id[0]);
			      dlt_cmds(MODE_SELECT,scsi_id[1]);
			      mode_selected=1;
			  }
/*			blk_wrt=fill_sg(tape_data,max_read);
*/			if(blk_wrt>0)
			  {
			      tape_data=(unsigned int *)DATA_RECS;
			      total_data=blk_wrt;
			  }
			else
			  {
			      tape_data++;
			  }
		    }
		  else
		    {
			if(mode_selected==0)
			  {
			      dlt_cmds(MODE_SELECT,scsi_id[0]);
			      dlt_cmds(MODE_SELECT,scsi_id[1]);
			      mode_selected=1;
			  }
			stat1=write_tape(tape_data,max_write,DATA,scsi_id[1]);
			if(stat1==1)
			  {
			      dlt_cmds(REQUEST_SENSE,scsi_id[1]);
			      print_rqst_sns();
			      taskDelay(100);
			  }
			/*stat1=write_tape(tape_data,max_write,DATA,DRV2);
			if(stat1==1)
			  {
			      dlt_cmds(REQUEST_SENSE,DRV2);
			      print_rqst_sns();
			      taskDelay(100);
			  }*/
			/*stat1=write_tape(tape_data,max_write,DATA,DRV3);
			if(stat1==1)
			  {
			      dlt_cmds(REQUEST_SENSE,DRV3);
			      print_rqst_sns();
			      taskDelay(100);
			  }*/
			total_data+=max_write;
			update+=max_write;
		    }
	      }
	    if((rqst_sen->sense_key&0x80)==0x80)
	      {
		  printf("WROTE %d BYTES AND FILEMARK TO DRV1\n",total_data);
		  dlt_cmds(WRITE_FILEMARK,scsi_id[1]);
	      }
	    if((rqst_sen->sense_key&0x40)==0x40)
	      {
		  printf("LEOT FOUND\n");
		  taskDelay(5);
		  dlt_cmds(WRITE_FILEMARK,scsi_id[1]);
		  printf("WROTE FILEMARK TO DRV1\n");
		  dlt_cmds(UNLOAD,scsi_id[0]);
		  printf("UNLOADING SCSI Drive %2d\n",scsi_id[0]);
		  dlt_cmds(UNLOAD,scsi_id[1]);
		  printf("UNLOADING DRV1\n");
		  printf("WROTE %d BYTES AND FILEMARK TO DRV1\n",total_data);
		  return;
	      }
	    if((rqst_sen->sense_key&0x08)==0x08)
	      {
		  printf("END OF DATA FOUND\n");
		  taskDelay(5);
		  dlt_cmds(WRITE_FILEMARK,scsi_id[1]);
		  printf("WROTE %d BYTES AND FILEMARK TO DRV1\n",total_data);
		  dlt_cmds(UNLOAD,scsi_id[0]);
		  printf("UNLOADING SCSI Drive %2d\n",scsi_id[0]);
		  dlt_cmds(UNLOAD,scsi_id[1]);
		  printf("UNLOADING DRV1\n");
		  return;
	      }
	    continue;
	}
      if(max_read<=1000)
	{
/*	    blk_wrt=fill_sg(tape_data,max_read);
*/	    if(blk_wrt>0)
	      {
		  tape_data=(unsigned int *)DATA_RECS;
		  total_data=blk_wrt;
	      }
	    else
	      {
		  tape_data++;
	      }
	}
      else
	{
	    stat1=write_tape(tape_data,max_write,DATA,scsi_id[1]);
	    if(stat1==1)
	      {
		  puts("ERROR DURING WRITE DRV1");
		  taskDelay(100);
		  dlt_cmds(REQUEST_SENSE,scsi_id[1]);
		  print_rqst_sns();
		  taskDelay(100);
		  
	      }
	    /*stat1=write_tape(tape_data,max_write,DATA,DRV2);
	    if(stat1==1)
	      {
		  puts("ERROR DURING WRITE DRV2");
		  taskDelay(100);
		  dlt_cmds(REQUEST_SENSE,DRV2);
		  print_rqst_sns();
		  taskDelay(100);
		  
	      }*/
	    /*stat1=write_tape(tape_data,max_write,DATA,DRV3);
	    if(stat1==1)
	      {
		  puts("ERROR DURING WRITE DRV3");
		  taskDelay(100);
		  dlt_cmds(REQUEST_SENSE,DRV3);
		  print_rqst_sns();
		  taskDelay(100);
		  
	      }*/
	    total_data+=max_write;
	    update+=max_write;
	    if(update>=10000000)
	      {
		  printf("A TOTAL OF %d BYTES HAVE BEEN WRITTEN TO DRV1\n",total_data);
		  update=0;
	      }
	}
  }
}




