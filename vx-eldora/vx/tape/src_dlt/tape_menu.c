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
 * Revision 1.1  1996/06/18  16:03:46  craig
 * Initial revision
 *
 *
 * description: Provides a diagnostic menu for the DLT tape drives
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <recordIncs.h>
#include <tapeDef.h>
#include <tapeGbl.h>

#define FREE_MEM_START 0x2000000

/******************* SELECT AN OPERATION ****************/

void tape_menu()
{
volatile unsigned short *status;
int choice, sg_length, sg_buff;
int i,j,k,n,m;
int length, address, bytes_to_print;
int cntrl_size, secs, tot_bytes;
char response, statr, statw;
long number, *long_pntr;
unsigned char drv_stat, read_id, write_id;
char *cntrl_addr;
union{
  int  aint;
  char array[4];
}sch;
float rate;

/* Set the SCSI ID's of each of the drives */

scsi_id[0] = 1;
scsi_id[1] = 2;
scsi_id[2] = 3;
scsi_id[3] = 4;

status=(unsigned short *)(CIP_BASE+SHORT_BASE+CIP_BD_STAT);
tape_vme_offset = 0;

/* Get the size of the area to malloc for the tape control structures,
   malloc the space and initialize all of the control structures */

/* cntrl_size = tape_cntrl_len(SCATTER_GATHER);
cntrl_addr = malloc(cntrl_size);
initialize_tape((int)cntrl_addr,SCATTER_GATHER);
*/

do
  {
      puts("DIGITAL LINEAR TAPE CIPRICO SCSI TAPE MENU");
      puts("ENTER A CHOICE BY NUMBER");
      puts("0) QUIT");
      puts("1) TEST UNIT READY");
      puts("2) REWIND");
      puts("3) UNLOAD");
      puts("4) LOAD");
      puts("5) READ TAPE");
      puts("6) MODE SENSE");
      puts("7) REQUEST SENSE");
      puts("8) WRITE FILEMARK");
      puts("9) TIMED WRITE TEST");
      puts("10) RESET CIPRICO AND TAPE DRIVES");
      puts("11) ERASE TAPE");
      puts("12) COPY TAPE TO TAPE");
      puts("13) INITIALIZE TAPE DRIVES");
      puts("14) SPACE TAPE");
      puts("15) WRITE TAPE BUFFERED");
      puts("16) WRITE TAPE SCATTERED_GATHERED");
      puts("17) LOG SENCE");
      puts("18) LOG PAGES");
      puts("19) LOG SELECT");

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
	    for(i=0; i<4; i++)
	      {
		  printf("LOAD SCSI Drive: %2d?",scsi_id[i]);
		  scanf(" %c",&response);
		  if(response=='y')
		    {
			dlt_cmds(LOAD,scsi_id[i]);
		    }
	      }
	    break;


	  case 5:    /* Read tape */
	    for(i=0; i<4; i++)
	      {
		  printf("\nRead from SCSI Drive %2d? ",scsi_id[i]);
		  scanf(" %c",&response);
		  if(response=='y')
		    {
			printf("\nEnter number of records to read: ");
			scanf(" %d",&number);
			printf("\nEnter length of each record: ");
			scanf(" %d",&length);
			printf("\nEnter the number of bytes to print: ");
			scanf(" %d",&bytes_to_print);
			printf("\nEnter address (in hex) of records");
			printf(" Global memory starts at 0x2000000: ");
			scanf(" %x",&address);
			for(j=0; j<number; j++)
			  {
			      read_tape((unsigned int *)address,
					 (unsigned int)length,
					 (unsigned char)BLOCKED,
					 scsi_id[i]);
			      printf("\n\nReading record record %d",j);

			      for(k=0; k < bytes_to_print/20; k++)
				{
				  long_pntr = (long *)(address + k*20); 
				  printf("\n%5d ",k*20);
				  for(m=0; m<5; m++)
				    printf("%08x ",*(long_pntr + m));
				  for(m=0; m<5; m++)
				    {
				      sch.aint = *(long_pntr + m);
				      if(0x7E >= sch.array[0] &&  
					 sch.array[0] >= 0x20)
					printf("%c",sch.array[0]);
				      else
					printf(".");
				      if(0x7E >= sch.array[1] &&
					 sch.array[1] >= 0x20)
					printf("%c",sch.array[1]);
				      else
					printf(".");
				      if(0x7E >= sch.array[2] &&
					 sch.array[2] >= 0x20)
					printf("%c",sch.array[2]);
				      else
					printf(".");
				      if(0x7E >= sch.array[3] &&
					 sch.array[3] >= 0x20)
					printf("%c",sch.array[3]);
				      else
					printf(".");
				    } /* for m=0 */
				}     /* for k=0 */
			  }           /* for j=0 */
		    }                 /* if(response */
	      }                       /* for i=0 */

	    break;



	  case 6:
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


	  case 7:
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



	  case 8:
	    for(i=0; i<4; i++)
	      {
		  printf("WRITE FILEMARK TO SCSI Drive: %2d?",scsi_id[i]);
		  scanf(" %c",&response);
		  if(response=='y')
		    {
			dlt_cmds(WRITE_FILEMARK,scsi_id[i]);
			printf("WROTE FILEMARK TO SCSI Drive: %2d\n",
			       scsi_id[i]);
		    }
	      }
	    break;



	  case 9: /* Timed write test */ 
	    for(i=0; i<4; i++)
	      {
		  printf("\nTimed Write to SCSI Drive %2d? ",scsi_id[i]);
		  scanf(" %c",&response);
		  if(response=='y')
		    {
			printf("\nEnter number of records to record: ");
			scanf(" %d",&number);
			printf("\nEnter length of each record: ");
			scanf(" %d",&length);
			printf("\nEnter address (in hex) of records");
			printf(" Global memory starts at 0x2000000: ");
			scanf(" %x",&address);
			long_pntr = (long *)address;
			n = tickGet();
			for(j=0; j<number; j++)
			  {
			      *long_pntr = j;
			      write_tape((unsigned int *)address,
					 (unsigned int)length,
					 (unsigned char)BLOCKED,
					 scsi_id[i]);
			  }
			m = tickGet();
			k = sysClkRateGet();
			secs = (m-n)/k;
			tot_bytes = number * length;
			rate = tot_bytes / secs;
			printf("\n%d bytes written in %d seconds rate: %f bytes/sec\n",tot_bytes,secs,rate);
		    }
	      }
	    break;


	  case 10:
	    cip_init();
	    break;


	  case 11:
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



	  case 12:
	    printf("\n Enter the scsi ID to read from: ");
	    scanf(" %d",&read_id);
	    printf("\n Enter the scsi ID to write to: ");
	    scanf(" %d",&write_id);
	    printf("\nEnter address (in hex) where to store records");
	    printf(" Global memory starts at 0x2000000: ");
	    scanf(" %x",&address);
	    length = 32768;
	    number = 0;
	    do{
	      statr = read_tape((unsigned int *)address,
			(unsigned int)length,
			(unsigned char)BLOCKED,
			read_id);
	      statw = write_tape((unsigned int *)address,
			 (unsigned int)length,
			 (unsigned char)BLOCKED,
			 write_id);
	      number += 1;
	    }while(statr == 0 && statw == 0);
	    printf("\n%d records copied from drive %d  to drive %d\n\n",
		   number,read_id,write_id);

	    break;



	  case 13:

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


	  case 14:
	    puts("\n\nSELECT SPACE BY:\n");
	    puts("1) N number of EOFS\n");
	    puts("2) N number of variable length blocks\n");
	    puts("3) EOD\n");
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

		  
	  case 15:
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
			printf(" Global memory starts at 0x2000000: ");
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
		  
	  case 16:
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
			printf(" Global memory starts at 0x2000000: ");
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
			      if(sg_buff >= NUM_SG_STRUCTS) sg_buff = 0;
			      sg_init(sg_buff);
			  }
		    }
	      }
	    break;

	  case 17:
	    for(i=0; i<4; i++)
	      {
		  printf("PRINT LOG SENSE SCSI Drive: %2d?",scsi_id[i]);
		  scanf(" %c",&response);
		  if(response=='y')
		    {
			dlt_cmds(LOG_SENSE,scsi_id[i]);
			print_log_sns();
		    }
	      }
	    break;

	  case 18:
	    for(i=0; i<4; i++)
	      {
		  printf("PRINT LOG SENSE (SPECIAL PAGES) SCSI Drive: %2d?",
			 scsi_id[i]);
		  scanf(" %c",&response);
		  if(response=='y')
		    {
			dlt_cmds(LOG_SENSE_SPGS,scsi_id[i]);
			print_log_pgs();
		    }
	      }
	    break;

	  case 19:
	    for(i=0; i<4; i++)
	      {
		  printf("LOG SELECT SCSI Drive: %2d?",scsi_id[i]);
		  scanf(" %c",&response);
		  if(response=='y')
		    {
			dlt_cmds(LOG_SELECT,scsi_id[i]);
		    }
	      }
	    break;


	  default:
	    puts("NOT A CHOICE. TRY AGAIN");
	    break;
	}
  }while(choice != 0);
}
