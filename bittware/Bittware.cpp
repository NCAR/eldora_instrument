#include "Bittware.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>		/* for memcpy and string routines */
#include <timer_registers.h>

Bittware::Bittware()
{

   //*********************************************************************************************
   //*    Initialization  
   //*********************************************************************************************

   PDSP21K processor;
   DSP21K_DEVICE_CFG cfg;
   DSP21K_PHYS_MEMORY AddrData;

   int error;
   int i;

   processor = dsp21k_open(0);						//Open Board for communication

   dsp21k_reset_bd(processor);						//Reset the board.
   dsp21k_cfg_proc(processor);						//Configure the board.

   error = dsp21k_get_device_info(processor, &cfg);
   if(check_error(processor, error) != DSP21K_SUCCESS) return 1; 

   //Variable Declaration
   int buffer_size = 0x8;						      //Memory buffer size in bytes
   ULONG ad_offset = 0x00400008;					      //Address and Data Physical Memory Offset
   ULONG rd_buffer[buffer_size / 4];					//Read Buffer
   ULONG wr_buffer[buffer_size / 4];					//Write Buffer

   //Setup Physical Memory
   AddrData.phys_addr = cfg.pci_badr[2] + ad_offset;			//Physical Memory address = BAR2 + offset
   AddrData.size = buffer_size;					      //Physical Memory size

   //Allocate Physical Memory
   error = dsp21k_alloc_phys_memory(processor, &AddrData);
   if(check_error(processor, error) != DSP21K_SUCCESS) printf("\nAddress/Data Memory Allocation Error!\n");

   //*********************************************************************************************
   //* Set Timer 1
   //*********************************************************************************************

   ULONG *reg_buf;
   ULONG CONTROL = TIMER_ON | TIMER_POS | EXT_CLK | CLK_DIV1;     
   ULONG DELAY = 15;
   ULONG WIDTH = 4000;
   ULONG PERIOD = 5000;
   ULONG PRT = 0x0000;

   //***************************************************************************

	  //Write to Timer 1 Control Register
   	  wr_buffer[0x0] = TIMER1 | CONTROL_REG;					//Address Line
       wr_buffer[0x1] = CONTROL;							//Data Line
       mem_write(processor, AddrData, wr_buffer);

       //Read Back Timer 1 Control Register
	  wr_buffer[0x0] = TIMER1;							//Address Line
	  mem_write(processor, AddrData, wr_buffer);
	  reg_buf = mem_read(processor, AddrData, rd_buffer)

       printf("Read Address Line = %x\n", *reg_buf);
       printf("Read Data Line = %x\n", *(reg_buf+1);


   //***************************************************************************

       //Write to Timer 1 Delay Register
   	  wr_buffer[0x0] = TIMER1 | DELAY_REG;					//Address Line
       wr_buffer[0x1] = DELAY;							//Data Line
       mem_write(processor, AddrData, wr_buffer);

       //Read Back Timer 1 Delay Register
	  wr_buffer[0x0] = TIMER1;							//Address Line
	  mem_write(processor, AddrData, wr_buffer);
	  reg_buf = mem_read(processor, AddrData, rd_buffer)

   //***************************************************************************

       //Write to Timer 1 Width Register
   	  wr_buffer[0x0] = TIMER1 | WIDTH_REG;					//Address Line
       wr_buffer[0x1] = WIDTH;							//Data Line
       mem_write(processor, AddrData, wr_buffer);

       //Read Back Timer 1 Width Register
	  wr_buffer[0x0] = TIMER1;							//Address Line
	  mem_write(processor, AddrData, wr_buffer);
	  reg_buf = mem_read(processor, AddrData, rd_buffer)

   //***************************************************************************

	  //Write to Timer 1 Period Register
   	  wr_buffer[0x0] = TIMER1 | PERIOD_REG;					//Address Line
       wr_buffer[0x1] = PERIOD;							//Data Line
       mem_write(processor, AddrData, wr_buffer);

       //Read Back Timer 1 Period Register
	  wr_buffer[0x0] = TIMER1;							//Address Line
	  mem_write(processor, AddrData, wr_buffer);
	  reg_buf = mem_read(processor, AddrData, rd_buffer)

   //***************************************************************************

	  //Write to Timer 1 PRT Register
   	  wr_buffer[0x0] = TIMER1 | PRT_REG;					//Address Line
       wr_buffer[0x1] = PRT;								//Data Line
       mem_write(processor, AddrData, wr_buffer);

       //Read Back Timer 1 PRT Register
	  wr_buffer[0x0] = TIMER1;							//Address Line
	  mem_write(processor, AddrData, wr_buffer);
	  reg_buf = mem_read(processor, AddrData, rd_buffer)

   //***************************************************************************
   //* Enable Timers
   //*********************************************************************************************

	  //Set Global Enable bit on Address Line
   	  wr_buffer[0x0] = GLOBAL_ENABLE;						//Address Line
       mem_write(processor, AddrData, wr_buffer);

   //***************************************************************************
   //* Trigger Timers
   //*********************************************************************************************

	  //Set Address Trigger bit on Address Line
   	  wr_buffer[0x0] = ADDR_TRIG;							//Address Line
       mem_write(processor, AddrData, wr_buffer);

return 0; 
}

//********************************************************************
//* Read from Memory store into Buffer				     *
//********************************************************************
ULONG *mem_read(PDSP21K processor, DSP21K_PHYS_MEMORY phys_mem, ULONG rd_buffer[])
{
int error;
ULONG *read;
   error = dsp21k_rd_phys_memory(&phys_mem, 0, phys_mem.size / 4, rd_buffer);
   if(check_error(processor, error) != DSP21K_SUCCESS)
   {
    	printf("Memory Read Error!\n");
   	return 0; 
   }
   read = rd_buffer;
  //printf("1   %08x     %08x\n",*read, rd_buffer[0]);
  //printf("2   %08x     %08x\n",*(read+1), rd_buffer[1]);
return read;
//return rd_buffer[1];
}
//********************************************************************
//* Write Buffer to Memory 					     *
//********************************************************************
void mem_write(PDSP21K processor, DSP21K_PHYS_MEMORY phys_mem, ULONG wr_buffer[])
{
int error;
   error = dsp21k_wr_phys_memory(&phys_mem, 0, phys_mem.size / 4, wr_buffer);
   if(check_error(processor, error) != DSP21K_SUCCESS)
   {
    	printf("Memory Write Error!\n");
   	return; 
   }
return;
}
//********************************************************************
//* Copy Read Buffer to Write Buffer Routine			     *
//********************************************************************
void copy_buffer(DSP21K_PHYS_MEMORY phys_mem, ULONG wr_buffer[], ULONG rd_buffer[])
{
int i;

	for (i = 0; i < phys_mem.size / 4; i++) wr_buffer[i] = rd_buffer[i];
	
return;
}
//********************************************************************
//* Print Error Message Routine						     *
//********************************************************************
int check_error(PDSP21K processor, int error)
{
//check error
if(error < 0)
{
	// Display last error, always close processor to
	// free memory before exiting program
	printf("\nERROR : %s\n", dsp21k_err_msgs(error));
	// Close the processor and exit
	dsp21k_close(processor);
	return 1;
}
return 0;
}
