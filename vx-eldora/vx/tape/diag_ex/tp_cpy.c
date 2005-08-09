/*
 *	$Id$
 *
 *	Module:tp_cpy.c		 
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.4  1992/10/25  17:08:13  reif
 * *** empty log message ***
 *
 * Revision 1.3  1992/10/17  21:32:59  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/15  17:52:42  reif
 * *** empty log message ***
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <cipincl.h>

void tp_cpy(unsigned int *tape_data,unsigned int max_read)
{
int stat0,stat1;
int max_write;
unsigned int total_data=0;
unsigned int update=0;
unsigned int blk_wrt;
int mode_selected=0;

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
	    exb_cmds(REQUEST_SENSE,RQST_SNS,scsi_id[0]);
	    if((rqst_sen.sense_key&0x20)==0x20)
	      {
		  sns.sns_array[0]=rqst_sen.info_msb;
		  sns.sns_array[1]=rqst_sen.info_mid_high;
		  sns.sns_array[2]=rqst_sen.info_mid_low;
		  sns.sns_array[3]=rqst_sen.info_lsb;
		  if(sns.sense_data>0)
		    max_write=max_read-sns.sense_data;
		  else
		    max_write=-(sns.sense_data-max_read);
		  max_read=max_write;
		  if(max_read<=1000)
		    {
			if(mode_selected==0)
			  {
			      mod_sel.motion_thr=0x30;
			      mod_sel.recon_thr=0xD0;
			      exb_cmds(MODE_SELECT,MD_SEL,scsi_id[0]);
			      mod_sel.motion_thr=0xD0;
			      mod_sel.recon_thr=0x30;
			      exb_cmds(MODE_SELECT,MD_SEL,scsi_id[1]);
			      mode_selected=1;
			  }
			blk_wrt=fill_sg(tape_data,max_read);
			if(blk_wrt>0)
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
			      mod_sel.motion_thr=0x80;
			      mod_sel.recon_thr=0x80;
			      exb_cmds(MODE_SELECT,MD_SEL,scsi_id[0]);
			      mod_sel.motion_thr=0x80;
			      mod_sel.recon_thr=0x80;
			      exb_cmds(MODE_SELECT,MD_SEL,scsi_id[1]);
			      mode_selected=1;
			  }
			stat1=write_tape(tape_data,max_write,DATA,scsi_id[1]);
			if(stat1==1)
			  {
			      exb_cmds(REQUEST_SENSE,RQST_SNS,scsi_id[1]);
			      print_rqst_sns();
			      taskDelay(100);
			  }
			/*stat1=write_tape(tape_data,max_write,DATA,DRV2);
			if(stat1==1)
			  {
			      exb_cmds(REQUEST_SENSE,RQST_SNS,DRV2);
			      print_rqst_sns();
			      taskDelay(100);
			  }*/
			/*stat1=write_tape(tape_data,max_write,DATA,DRV3);
			if(stat1==1)
			  {
			      exb_cmds(REQUEST_SENSE,RQST_SNS,DRV3);
			      print_rqst_sns();
			      taskDelay(100);
			  }*/
			total_data+=max_write;
			update+=max_write;
		    }
	      }
	    if((rqst_sen.sense_key&0x80)==0x80)
	      {
		  printf("WROTE %d BYTES AND FILEMARK TO DRV1\n",total_data);
		  exb_cmds(WRITE_FILEMARK,WRT_FLMK,scsi_id[1]);
	      }
	    if((rqst_sen.sense_key&0x40)==0x40)
	      {
		  printf("LEOT FOUND\n");
		  taskDelay(5);
		  exb_cmds(WRITE_FILEMARK,WRT_FLMK,scsi_id[1]);
		  printf("WROTE FILEMARK TO DRV1\n");
		  exb_cmds(UNLOAD,LD_ULD,scsi_id[0]);
		  printf("UNLOADING SCSI Drive %2d\n",scsi_id[0]);
		  exb_cmds(UNLOAD,LD_ULD,scsi_id[1]);
		  printf("UNLOADING DRV1\n");
		  printf("WROTE %d BYTES AND FILEMARK TO DRV1\n",total_data);
		  return;
	      }
	    if((rqst_sen.sense_key&0x08)==0x08)
	      {
		  printf("END OF DATA FOUND\n");
		  taskDelay(5);
		  exb_cmds(WRITE_FILEMARK,WRT_FLMK,scsi_id[1]);
		  printf("WROTE %d BYTES AND FILEMARK TO DRV1\n",total_data);
		  exb_cmds(UNLOAD,LD_ULD,scsi_id[0]);
		  printf("UNLOADING SCSI Drive %2d\n",scsi_id[0]);
		  exb_cmds(UNLOAD,LD_ULD,scsi_id[1]);
		  printf("UNLOADING DRV1\n");
		  return;
	      }
	    continue;
	}
      if(max_read<=1000)
	{
	    blk_wrt=fill_sg(tape_data,max_read);
	    if(blk_wrt>0)
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
		  exb_cmds(REQUEST_SENSE,RQST_SNS,scsi_id[1]);
		  print_rqst_sns();
		  taskDelay(100);
		  
	      }
	    /*stat1=write_tape(tape_data,max_write,DATA,DRV2);
	    if(stat1==1)
	      {
		  puts("ERROR DURING WRITE DRV2");
		  taskDelay(100);
		  exb_cmds(REQUEST_SENSE,RQST_SNS,DRV2);
		  print_rqst_sns();
		  taskDelay(100);
		  
	      }*/
	    /*stat1=write_tape(tape_data,max_write,DATA,DRV3);
	    if(stat1==1)
	      {
		  puts("ERROR DURING WRITE DRV3");
		  taskDelay(100);
		  exb_cmds(REQUEST_SENSE,RQST_SNS,DRV3);
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




