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


/******************* SELECT AN OPERATION ****************/

void tape_menu()
{
volatile unsigned short *status;
int choice;
int i,j;
int stat;
int recflag;
int original, found;
int hdrsz;
char sgflg,drv_num;
int stat0,stat1;
unsigned int *tape_data;
unsigned int count;
int max_read;
int total_fmks;
char response;
long number;
char test;
unsigned char drv_stat;
short list_length;
char mem_dump;

/* Set the SCSI ID's of each of the drives */

scsi_id[0] = 1;
scsi_id[1] = 2;
scsi_id[2] = 3;
scsi_id[3] = 4;

status=(unsigned short *)(CIP_BASE+CIP_BD_STAT);
total_fmks=0;
max_read=32768;
tape_data=(unsigned int *)DATA_RECS;
parm_blk_init();
md_sel_init();
rqst_sns_init();
md_sns_init();
sg_init();

do
  {
      puts("SCSI DLT TAPE MENU");
      puts("ENTER A CHOICE BY NUMBER");
      puts("0) QUIT");
      puts("1) TEST UNIT READY");
      puts("2) REWIND");
      puts("3) UNLOAD");
      puts("4) SEND DIAGNOSTIC");
      puts("5) MODE SENSE");
      puts("6) REQUEST SENSE");
      puts("7) WRITE FILEMARK");
      puts("8) READ POSITION");
      puts("9) RESET CIPRICO AND DLTs");
      puts("10) ERASE TAPE");
      puts("11) COPY TAPE TO TAPE");
      puts("12) INITIALIZE TAPE DRIVES");
      puts("13) SPACE TAPE");
      puts("14) LOAD TAPE");
      puts("15) LOG SENCE");
      puts("16) PRINT STATUS");

      scanf(" %d",&choice);
      switch(choice)
	{
	  case 0:
	    break;



	  case 1:
	    for(i=0; i<4; i++)
	      {
		  printf("TEST UNIT READY SCSI Drive: %2d?",scsi_id[i]);
		  scanf(" %c",&response);
		  if(response=='y')
		    {
			drv_stat=tst_unt_rdy(scsi_id[i]);
			while(drv_stat!=0x80)
			  {
			      drv_stat=tst_unt_rdy(scsi_id[i]);
			      printf("SCSI STATUS= %X\n",
				     parmblk[TEST_UNIT_READY].scsi_status);
			      printf("SCSI FLAGS= %X\n",drv_stat);
			      printf("CIPRICO ERROR STATUS= %X\n",
				     parmblk[TEST_UNIT_READY].error);
			      printf("CIPRICO BD STATUS= %X\n",*status);
			      taskDelay(100);
			  }
		    }
	      }
	    break;



	  case 2:
	    for(i=0; i<4; i++)
	      {
		  printf("REWIND SCSI Drive: %2d?",scsi_id[i]);
		  scanf(" %c",&response);
		  if(response=='y')
		    {
			exb_cmds(REWND,RWND,scsi_id[i]);
		    }
	      }
	    break;



	  case 3:
	    for(i=0; i<4; i++)
	      {
		  printf("UNLOAD SCSI Drive: %2d?",scsi_id[i]);
		  scanf(" %c",&response);
		  if(response=='y')
		    {
			exb_cmds(UNLOAD,LD_ULD,scsi_id[i]);
		    }
	      }
	    break;


	  case 4:
	    printf("ENTER THE NUMBER OF THE DRIVE TO TEST(0-3)");
	    scanf(" %d",&choice);
	    found = 0;
	    for(i=0; i<4; i++)
	      if(choice == scsi_id[i]) found = 1;
	    if(found != 1) 
	      {
		  printf("DRV%d DOES NOT EXIST ON THIS SYSTEM\n",choice);
		  puts("EXITING");
		  break;
	      }
	    drv_num=choice;
	    puts("SELECT A TEST TO RUN");
	    puts("0) MEMORY DUMP");
	    puts("1) POWER ON TESTS WITHOUT TAPE");
	    puts("2) POWER ON WRT/RD & LOAD TEST WITHOUT TAPE");
	    puts("3) POWER ON TESTS WITH TAPE");
	    puts("4) POWER ON WRT/RD & LOAD TEST WITH TAPE");
	    scanf(" %d",&choice);
	    switch(choice)
	      {
		case 0:
		  puts("NOT IMPLEMENTED YET!!");
		  break;
		case 1:
		  printf("DO NOT INSERT A DATA CARTRIDGE IN DRV%d FOR THIS TEST\n",drv_num);
		  tst_unt_rdy(drv_num);
		  exb_cmds(REQUEST_SENSE,RQST_SNS,drv_num);
		  if((rqst_sen.sense_key&0x02)==0x02)
		    {
			test=0x04;
			list_length=0;
			mem_dump=0x00;
			send_diags(test,list_length,mem_dump,drv_num);
			printf("RCV RESULTS FROM DRV%d?\n",drv_num);
			scanf(" %c",&response);
			if(response=='y')
			  {
			      exb_cmds(RCV_DIAGNOSTICS,RCV_DIAGS,drv_num);
			      print_results();
			      taskDelay(100);
			  }
			break;
		    }
		  printf("DRV%d HAS CARTRIDGE IN IT! REMOVE AND TRY AGAIN\n",drv_num);
		  break;
		case 2:
		  printf("INSERT A DATA CARTRIDGE IN DRV%d AFTER DOOR OPENS\n",drv_num);
		  tst_unt_rdy(drv_num);
		  exb_cmds(REQUEST_SENSE,RQST_SNS,drv_num);
		  if((rqst_sen.sense_key&0x02)==0x02)
		    {
			test=0x05;
			list_length=0;
			mem_dump=0x00;
			send_diags(test,list_length,mem_dump,drv_num);
			printf("RCV RESULTS FROM DRV%d?\n",drv_num);
			scanf(" %c",&response);
			if(response=='y')
			  {
			      exb_cmds(RCV_DIAGNOSTICS,RCV_DIAGS,drv_num);
			      print_results();
			      taskDelay(100);
			  }
			break;
		    }
		  printf("DRV%d HAS A CARTRIDGE IN IT! REMOVE AND TRY AGAIN\n",drv_num);
		  break;
		case 3:
		  printf("INSERT A DATA CARTRIDGE IN DRV%d FOR THIS TEST\n",drv_num);
		  tst_unt_rdy(drv_num);
		  exb_cmds(REQUEST_SENSE,RQST_SNS,drv_num);
		  if((rqst_sen.sense_key&0x02)==0x02)
		    {
			printf("DRV%d NEEDS A CARTRIDGE! INSERT AND TRY AGAIN\n",drv_num);
			break;
		    }
		  test=0x06;
		  list_length=0;
		  mem_dump=0x00;
		  send_diags(test,list_length,mem_dump,drv_num);
		  printf("RCV RESULTS FROM DRV%d?\n",drv_num);
		  scanf(" %c",&response);
		  if(response=='y')
		    {
			exb_cmds(RCV_DIAGNOSTICS,RCV_DIAGS,drv_num);
			print_results();
			taskDelay(100);
		    }
		  break;
		case 4:
		  printf("INSERT A DATA CARTRIDGE IN DRV%d FOR THIS TEST\n",drv_num);
		  tst_unt_rdy(drv_num);
		  exb_cmds(REQUEST_SENSE,RQST_SNS,drv_num);
		  if((rqst_sen.sense_key&0x02)==0x02)
		    {
			printf("DRV%d NEEDS A CARTRIDGE! INSERT AND TRY AGAIN\n",drv_num);
			break;
		    }
		  test=0x07;
		  list_length=0;
		  mem_dump=0x00;
		  send_diags(test,list_length,mem_dump,drv_num);
		  printf("RCV RESULTS FROM DRV%d?\n",drv_num);
		  scanf(" %c",&response);
		  if(response=='y')
		    {
			exb_cmds(RCV_DIAGNOSTICS,RCV_DIAGS,drv_num);
			print_results();
			taskDelay(100);
		    }
		  break;
	      }
	    break;



	  case 5:
	    for(i=0; i<4; i++)
	      {
		  printf("PRINT MODE SENSE SCSI Drive: %2d?",scsi_id[i]);
		  scanf(" %c",&response);
		  if(response=='y')
		    {
			exb_cmds(MODE_SENSE,MD_SNS,scsi_id[i]);
			print_md_sns();
		    }
	      }
	    break;


	  case 6:
	    for(i=0; i<4; i++)
	      {
		  printf("PRINT REQUEST SENSE SCSI Drive: %2d?",scsi_id[i]);
		  scanf(" %c",&response);
		  if(response=='y')
		    {
			exb_cmds(REQUEST_SENSE,RQST_SNS,scsi_id[i]);
			print_rqst_sns();
		    }
	      }
	    break;



	  case 7:
	    for(i=0; i<4; i++)
	      {
		  printf("WRITE FILEMARK TO SCSI Drive: %2d?",scsi_id[i]);
		  scanf(" %c",&response);
		  if(response=='y')
		    {
			timex(exb_cmds,WRITE_FILEMARK,WRT_FLMK,scsi_id[i]);
			total_fmks++;
			printf("WROTE FILEMARK TO SCSI Drive: %2d\n",
			       scsi_id[i]);
			printf("TOTAL FILEMARKS WRITTEN= %d\n",
			       total_fmks);
		    }
	      }
	    break;



	  case 8:
	    break;


	  case 9:
	    cip_init();
	    break;


	  case 10:
	    for(i=0; i<4; i++)
	      {
		  printf("ERASE SCSI Drive: %2d?",scsi_id[i]);
		  scanf(" %c",&response);
		  if(response=='y')
		    {
			exb_cmds(ERASE,ERS,scsi_id[i]);
		    }
	      }
	    break;



	  case 11:
	    tp_cpy(tape_data,max_read);
	    break;



	  case 12:

	    for(i=0; i<4; i++)
	      {
		  printf("INITIALIZE SCSI Drive: %2d?",scsi_id[i]);
		  scanf(" %c",&response);
		  if(response=='y')
		    {
			printf("Calling General Ops SCSI Drive: %2d\n",
			       scsi_id[i]);
			cip_cmds(GEN_OPS,GOPS,scsi_id[i]);
			printf("Calling Unit Ops SCSI Drive: %2d\n",
			       scsi_id[i]);
			cip_cmds(UNIT_OPS,UOPS,scsi_id[i]);
			printf("Testing Unit Ready SCSI Drive: %2d\n",
			       scsi_id[i]);
			tst_unt_rdy(scsi_id[i]);
			printf("Selecting Mode for SCSI Drive: %2d\n",
			       scsi_id[i]);
			exb_cmds(MODE_SELECT,MD_SEL,scsi_id[i]);
		    }
	      }
	    break;


	  case 13:
	    puts("SELECT SPACE BY:");
	    puts("1) N number of EOFS");
	    puts("2) N number of variable length blocks");
	    puts("3) EOD");
	    scanf(" %d",&choice);
	    switch(choice)
	      {
		case 1:
		  puts("ENTER # OF EOFS TO SKIP (+ FORWARD)(- BACK)");
		  scanf(" %d",&number);
		  for(i=0; i<4; i++)
		    {
			printf("SCSI Drive: %2d?",scsi_id[i]);
			scanf(" %c",&response);
			if(response=='y')
			  space_tape(number,EOFS,0x00,scsi_id[i]);
		    }
		  break;

		case 2:
		  puts("ENTER # OF BLOCKS TO SKIP (+ FORWARD)(- BACK)");
		  scanf(" %d",&number);
		  for(i=0; i<4; i++)
		    {
			printf("SCSI Drive: %2d?",scsi_id[i]);
			scanf(" %c",&response);
			if(response=='y')
			  space_tape(number,BLOCKS,0x00,scsi_id[i]);
		    }
		  break;

		case 3:
		  for(i=0; i<4; i++)
		    {
			printf("SCSI Drive: %2d?",scsi_id[i]);
			scanf(" %c",&response);
			if(response=='y')
			  space_tape(0,EOD,0x00,scsi_id[i]);
		    }
		  break;

		default:
		  puts("NOT A CHOICE");
		  break;
	      }
	    break;


	  case 14:
	    for(i=0; i<4; i++)
	      {
		  printf("LOAD SCSI Drive: %2d?",scsi_id[i]);
		  scanf(" %c",&response);
		  if(response=='y')
		    {
			exb_cmds(LOAD,LD_ULD,scsi_id[i]);
		    }
	      }
	    break;

	  case 15:
	    for(i=0; i<4; i++)
	      {
		  printf("PRINT LOG SENSE SCSI Drive: %2d?",scsi_id[i]);
		  scanf(" %c",&response);
		  if(response=='y')
		    {
			exb_cmds(LOG_SENSE,LOG_SEN,scsi_id[i]);
			print_log_sns();
		    }
	      }
	    break;

	  case 16:
	    printf("ENTER COMMAND ID OF STATUS TO DISPLAY: ");
	    scanf(" %d",&i);
	    if(i >= 0 && i <= LAST_CIP_CMD)
	      print_stat(0,i);
	    break;



	  default:
	    puts("NOT A CHOICE. TRY AGAIN");
	    break;
	}
  }while(choice != 0);
}


