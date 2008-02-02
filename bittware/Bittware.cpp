#include "Bittware.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>		/* for memcpy and string routines */
#include <iostream>

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
    if (check_error(error) != DSP21K_SUCCESS)
        return;

    //Setup Physical Memory
    _physmem.phys_addr = _cfg.pci_badr[2] + AD_OFFSET; //Physical Memory address = BAR2 + offset
    _physmem.size = BUFFER_SIZE; //Physical Memory size

    //Allocate Physical Memory
    error = dsp21k_alloc_phys_memory(_processor, &_physmem);
    if (check_error(error) != DSP21K_SUCCESS) {
        printf("\nAddress/Data Memory Allocation Error!\n");
        return;
    }

    _isok = true;
    
    std::cout << "Remora  initialized\n";
}

//////////////////////////////////////////////////////////////////////
Bittware::~Bittware() {
    
}

//////////////////////////////////////////////////////////////////////
void Bittware::configure(unsigned long delay,
                         unsigned long width,
                         unsigned long period,
                         unsigned long prt) {

    ULONG rd_buffer[BUFFER_SIZE / 4];
    ULONG wr_buffer[BUFFER_SIZE / 4];

    // Set Timer 1

    ULONG *reg_buf;
    ULONG control = BW_TIMER_ON | BW_TIMER_POS | BW_EXT_CLK | BW_CLK_DIV1;

    // Write to Timer 1 Control Register
    wr_buffer[0x0] = BW_TIMER1 | BW_CONTROL_REG; //Address Line
    wr_buffer[0x1] = control; //Data Line
    mem_write(wr_buffer);

    // Read Back Timer 1 Control Register
    wr_buffer[0x0] = BW_TIMER1; //Address Line
    mem_write(wr_buffer);
    reg_buf = mem_read(rd_buffer);

    std::cout << "Read Address Line = " << *reg_buf << "\n";
    std::cout << "Read Data Line = " << *(reg_buf+1) << "\n";

    // Write to Timer 1 Delay Register
    wr_buffer[0x0] = BW_TIMER1 | BW_DELAY_REG; //Address Line
    wr_buffer[0x1] = delay; //Data Line
    mem_write(wr_buffer);

    // Read Back Timer 1 Delay Register
    wr_buffer[0x0] = BW_TIMER1; //Address Line
    mem_write(wr_buffer);
    reg_buf = mem_read(rd_buffer);

    // Write to Timer 1 Width Register
    wr_buffer[0x0] = BW_TIMER1 | BW_WIDTH_REG; //Address Line
    wr_buffer[0x1] = width; //Data Line
    mem_write(wr_buffer);

    // Read Back Timer 1 Width Register
    wr_buffer[0x0] = BW_TIMER1; //Address Line
    mem_write(wr_buffer);
    reg_buf = mem_read(rd_buffer);

    // Write to Timer 1 Period Register
    wr_buffer[0x0] = BW_TIMER1 | BW_PERIOD_REG; //Address Line
    wr_buffer[0x1] = period; //Data Line
    mem_write(wr_buffer);

    // Read Back Timer 1 Period Register
    wr_buffer[0x0] = BW_TIMER1; //Address Line
    mem_write(wr_buffer);
    reg_buf = mem_read(rd_buffer);

    // Write to Timer 1 PRT Register
    wr_buffer[0x0] = BW_TIMER1 | BW_PRT_REG; //Address Line
    wr_buffer[0x1] = prt; //Data Line
    mem_write(wr_buffer);

    // Read Back Timer 1 PRT Register
    wr_buffer[0x0] = BW_TIMER1; //Address Line
    mem_write(wr_buffer);
    reg_buf = mem_read(rd_buffer);
    
    std::cout << "Remora configured\n";
}

/////////////////////////////////////////////////////////////////////////////
void Bittware::start() {

    ULONG wr_buffer[BUFFER_SIZE / 4]; //Write Buffer

    // Enable Timers

    //Set Global Enable bit on Address Line
    wr_buffer[0x0] = BW_GLOBAL_EN; //Address Line
    mem_write(wr_buffer);

    // Trigger Timers

    //Set Address Trigger bit on Address Line
    wr_buffer[0x0] = BW_ADDR_TRIG; //Address Line
    mem_write(wr_buffer);
    
    std::cout << "Remora started\n";

}

////////////////////////////////////////////////////////////////////////
ULONG * Bittware::mem_read(ULONG rd_buffer[]) {
    int error;
    ULONG *read;
    error = dsp21k_rd_phys_memory(&_physmem, 0, _physmem.size / 4, rd_buffer);
    if (check_error(error) != DSP21K_SUCCESS) {
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
void Bittware::mem_write(ULONG wr_buffer[]) {
    int error;
    error = dsp21k_wr_phys_memory(&_physmem, 0, _physmem.size / 4, wr_buffer);
    if (check_error(error) != DSP21K_SUCCESS) {
        printf("Memory Write Error!\n");
        return;
    }
    return;
}

////////////////////////////////////////////////////////////////////////
void Bittware::copy_buffer(ULONG wr_buffer[],
                           ULONG rd_buffer[]) {
    unsigned int i;

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
bool Bittware::isok() {
    return _isok;
}
