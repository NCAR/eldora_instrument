#include "Bittware.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>		/* for memcpy and string routines */

Bittware::Bittware() :
    _isok(false) {

    //*********************************************************************************************
    //*    Initialization  
    //*********************************************************************************************

    int error;

    _processor = dsp21k_open(0); //Open Board for communication

    dsp21k_reset_bd(_processor); //Reset the board.
    dsp21k_cfg_proc(_processor); //Configure the board.

    error = dsp21k_get_device_info(_processor, &_cfg);
    if (check_error(_processor, error) != DSP21K_SUCCESS)
        return;

    //Setup Physical Memory
    _physmem.phys_addr = _cfg.pci_badr[2] + AD_OFFSET; //Physical Memory address = BAR2 + offset
    _physmem.size = BUFFER_SIZE; //Physical Memory size

    //Allocate Physical Memory
    error = dsp21k_alloc_phys_memory(_processor, &_physmem);
    if (check_error(_processor, error) != DSP21K_SUCCESS) {
        printf("\nAddress/Data Memory Allocation Error!\n");
        return;
    }

    _isok = true;
}

//////////////////////////////////////////////////////////////////////
void Bittware::configure() {

    unsigned int rd_buffer[BUFFER_SIZE / 4];
    unsigned int wr_buffer[BUFFER_SIZE / 4];

    // Set Timer 1

    unsigned int *reg_buf;
    unsigned int CONTROL= TIMER_ON | TIMER_POS | EXT_CLK | CLK_DIV1;
    unsigned int DELAY = 15;
    unsigned int WIDTH = 4000;
    unsigned int PERIOD = 5000;
    unsigned int PRT = 0x0000;

    // Write to Timer 1 Control Register
    wr_buffer[0x0] = TIMER1 | CONTROL_REG; //Address Line
    wr_buffer[0x1] = CONTROL; //Data Line
    mem_write(wr_buffer);

    // Read Back Timer 1 Control Register
    wr_buffer[0x0] = TIMER1; //Address Line
    mem_write(wr_buffer);
    reg_buf = mem_read(rd_buffer);

    printf("Read Address Line = %x\n", *reg_buf);
    printf("Read Data Line = %x\n", *(reg_buf+1));

    // Write to Timer 1 Delay Register
    wr_buffer[0x0] = TIMER1 | DELAY_REG; //Address Line
    wr_buffer[0x1] = DELAY; //Data Line
    mem_write(wr_buffer);

    // Read Back Timer 1 Delay Register
    wr_buffer[0x0] = TIMER1; //Address Line
    mem_write(wr_buffer);
    reg_buf = mem_read(rd_buffer);

    // Write to Timer 1 Width Register
    wr_buffer[0x0] = TIMER1 | WIDTH_REG; //Address Line
    wr_buffer[0x1] = WIDTH; //Data Line
    mem_write(wr_buffer);

    // Read Back Timer 1 Width Register
    wr_buffer[0x0] = TIMER1; //Address Line
    mem_write(wr_buffer);
    reg_buf = mem_read(rd_buffer);

    // Write to Timer 1 Period Register
    wr_buffer[0x0] = TIMER1 | PERIOD_REG; //Address Line
    wr_buffer[0x1] = PERIOD; //Data Line
    mem_write(wr_buffer);

    // Read Back Timer 1 Period Register
    wr_buffer[0x0] = TIMER1; //Address Line
    mem_write(wr_buffer);
    reg_buf = mem_read(rd_buffer);

    // Write to Timer 1 PRT Register
    wr_buffer[0x0] = TIMER1 | PRT_REG; //Address Line
    wr_buffer[0x1] = PRT; //Data Line
    mem_write(wr_buffer);

    // Read Back Timer 1 PRT Register
    wr_buffer[0x0] = TIMER1; //Address Line
    mem_write(wr_buffer);
    reg_buf = mem_read(rd_buffer);
}

/////////////////////////////////////////////////////////////////////////////
void Bittware::start() {

    unsigned int wr_buffer[BUFFER_SIZE / 4]; //Write Buffer

    // Enable Timers

    //Set Global Enable bit on Address Line
    wr_buffer[0x0] = GLOBAL_ENABLE; //Address Line
    mem_write(wr_buffer);

    // Trigger Timers

    //Set Address Trigger bit on Address Line
    wr_buffer[0x0] = ADDR_TRIG; //Address Line
    mem_write(wr_buffer);

}

////////////////////////////////////////////////////////////////////////
unsigned int * Bittware::mem_read(unsigned int rd_buffer[]) {
    int error;
    unsigned int *read;
    error = dsp21k_rd_phys_memory(&_physmem, 0, _physmem.size / 4, rd_buffer);
    if (check_error(_processor, error) != DSP21K_SUCCESS) {
        printf("Memory Read Error!\n");
        return 0;
    }
    read = rd_buffer;
    //printf("1   %08x     %08x\n",*read, rd_buffer[0]);
    //printf("2   %08x     %08x\n",*(read+1), rd_buffer[1]);
    return read;
    //return rd_buffer[1];
}

////////////////////////////////////////////////////////////////////////
void Bittware::mem_write(unsigned int wr_buffer[]) {
    int error;
    error = dsp21k_wr_phys_memory(&_physmem, 0, _physmem.size / 4, wr_buffer);
    if (check_error(_processor, error) != DSP21K_SUCCESS) {
        printf("Memory Write Error!\n");
        return;
    }
    return;
}

////////////////////////////////////////////////////////////////////////
void Bittware::copy_buffer(unsigned int wr_buffer[],
                           unsigned int rd_buffer[]) {
    int i;

    for (i = 0; i < _physmem.size / 4; i++)
        wr_buffer[i] = rd_buffer[i];

    return;
}

////////////////////////////////////////////////////////////////////////
int Bittware::check_error(int error) {
    //check error
    if (error < 0) {
        // Display last error, always close processor to
        // free memory before exiting program
        printf("\nERROR : %s\n", dsp21k_err_msgs(error));
        // Close the processor and exit
        dsp21k_close(_processor);
        return 1;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////
int Bittware::status() {
    return _isok;
}