/*
 *	$Id$
 *
 *	Module: tape_menu		 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description: Provides a diagnostic menu for the DLT tape drives
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <recordIncs.h>
#include <tapeDef.h>
#include <tapeGbl.h>

#define FREE_MEM_START 0x20000000

/******************* SELECT AN OPERATION ****************/

void tape_menu()
{
volatile unsigned short *status;
int choice, sg_length, sg_buff;
int i,j,k,n;
int stat;
int recflag;
int original, found;
int hdrsz;
char sgflg,drv_num;
int stat0,stat1;
unsigned int *tape_data;
unsigned int count;
int max_read, length, address;
int total_fmks;
char response;
long number, *long_pntr;
char test;
unsigned char drv_stat;
short list_length;
char mem_dump;

/* Set the SCSI ID's of each of the drives */

scsi_id[0] = 1;
scsi_id[1] = 2;
scsi_id[2] = 3;
scsi_id[3] = 4;

status=(unsigned short *)(CIP_BASE+SHORT_BASE+CIP_BD_STAT);
total_fmks=0;
max_read=32768;
tape_vme_offset = 0;
/* tape_data= (unsigned int *)0x2001000; */

initialize_tape(FREE_MEM_START,SCATTER_GATHER);

do
  {
      puts("DIGITAL LINEAR TAPE CIPRICO SCSI TAPE MENU");
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
      puts("9) RESET CIPRICO AND TAPE DRIVES");
      puts("10) ERASE TAPE");
      puts("11) COPY TAPE TO TAPE");
      puts("12) INITIALIZE TAPE DRIVES");
      puts("13) SPACE TAPE");
      puts("14) WRITE TAPE BUFFERED");
      puts("15) WRITE TAPE SCATTERED_GATHERED");

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
				     parmblk[TEST_UNIT_READY]->scsi_status);
			      printf("SCSI FLAGS= %X\n",drv_stat);
			      printf("CIPRICO ERROR STATUS= %X\n",
				     parmblk[TEST_UNIT_READY]->error);
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
			dlt_cmds(REWND,scsi_id[i]);
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
			dlt_cmds(UNLOAD,scsi_id[i]);
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
		  dlt_cmds(REQUEST_SENSE,drv_num);
		  if((rqst_sen->sense_key&0x02)==0x02)
		    {
			test=0x04;
			list_length=0;
			mem_dump=0x00;
			send_diags(test,list_length,mem_dump,drv_num);
			printf("RCV RESULTS FROM DRV%d?\n",drv_num);
			scanf(" %c",&response);
			if(response=='y')
			  {
			      dlt_cmds(RCV_DIAGNOSTICS,drv_num);
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
		  dlt_cmds(REQUEST_SENSE,drv_num);
		  if((rqst_sen->sense_key&0x02)==0x02)
		    {
			test=0x05;
			list_length=0;
			mem_dump=0x00;
			send_diags(test,list_length,mem_dump,drv_num);
			printf("RCV RESULTS FROM DRV%d?\n",drv_num);
			scanf(" %c",&response);
			if(response=='y')
			  {
			      dlt_cmds(RCV_DIAGNOSTICS,drv_num);
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
		  dlt_cmds(REQUEST_SENSE,drv_num);
		  if((rqst_sen->sense_key&0x02)==0x02)
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
			dlt_cmds(RCV_DIAGNOSTICS,drv_num);
			print_results();
			taskDelay(100);
		    }
		  break;
		case 4:
		  printf("INSERT A DATA CARTRIDGE IN DRV%d FOR THIS TEST\n",drv_num);
		  tst_unt_rdy(drv_num);
		  dlt_cmds(REQUEST_SENSE,drv_num);
		  if((rqst_sen->sense_key&0x02)==0x02)
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
			dlt_cmds(RCV_DIAGNOSTICS,drv_num);
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
			dlt_cmds(MODE_SENSE,scsi_id[i]);
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
			dlt_cmds(REQUEST_SENSE,scsi_id[i]);
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
			timex(dlt_cmds,WRITE_FILEMARK,scsi_id[i]);
			total_fmks++;
			printf("WROTE FILEMARK TO SCSI Drive: %2d\n",
			       scsi_id[i]);
			printf("TOTAL FILEMARKS WRITTEN= %d\n",
			       total_fmks);
		    }
	      }
	    break;



	  case 8:
	    printf("Sorry!! not implemented\n\n\n");
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
			dlt_cmds(ERASE,scsi_id[i]);
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
			drive_init(scsi_id[i]);
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
		  printf("\nWrite blocked to SCSI Drive %2d? ",scsi_id[i]);
		  scanf(" %c",&response);
		  if(response=='y')
		    {
			printf("\nEnter number of records to record: ");
			scanf(" %d",&number);
			printf("\nEnter length of each record: ");
			scanf(" %d",&length);
			printf("\nEnter address (in hex) of records: ");
			scanf(" %x",&address);
			for(j=0; j<number; j++)
			  {
			      long_pntr = (long *)address;
			      printf("Writing record %d at %x\n",j,long_pntr);
			      for(k=0; k<length/4; k++)
				*long_pntr++ = j;
			      write_tape((unsigned int *)address,
					 (unsigned int)length,
					 (unsigned char)BLOCKED,
					 scsi_id[i]);
			  }
		    }
	      }
	    break;
		  
	  case 15:
	    for(i=0; i<4; i++)
	      {
		  printf("\nWrite scatter gather to SCSI Drive %2d? ",
			 scsi_id[i]);
		  scanf(" %c",&response);
		  if(response=='y')
		    {
			printf("\nEnter number of records to record: ");
			scanf(" %d",&number);
			printf("\nEnter length of each record: ");
			scanf(" %d",&length);
			printf("\nEnter address (in hex) of records: ");
			scanf(" %x",&address);
			sg_length = length / 19;
			sg_length = (int)(sg_length / 4) * 4;
			length = sg_length * 19;
			sg_buff = 0;
			sg_init(sg_buff);

			for(j=0; j<number; j++)
			  {
			      printf("Writing %d byte record #%d from: %x\n",
				     length,j,address);
			      
			      for(n = 0; n < 19; n++)
				{
				    long_pntr = (long *)((int)address + 
						     n * sg_length);
				    fill_sg((unsigned int *)long_pntr,
					    sg_length,sg_buff);
				    for(k=0; k<sg_length/4 + 1; k++)
				      *long_pntr++ = j * n;
				}


			    write_tape((unsigned int *)
				       &sg[sg_buff][0]->next_sg_blk_add,
					 (unsigned int)length,
					 (unsigned char)SCATTER_GATHER,
					 scsi_id[i]);
			      sg_buff += 1;
			      if(sg_buff > 1) sg_buff = 0;
			      sg_init(sg_buff);
			  }
		    }
	      }
	    break;

	  default:
	    puts("NOT A CHOICE. TRY AGAIN");
	    break;
	}
  }while(choice != 0);
}
