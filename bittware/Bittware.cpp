#include "Bittware.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>		/* for memcpy and string routines */
#include <iostream>

Bittware::Bittware(int devNum) :
    _devNum(devNum), _isok(false) {

    //*********************************************************************************************
    //*    Initialization  
    //*********************************************************************************************

    int error;

    _processor = dsp21k_open(0); //Open Board for communication

    std::cout << "_processor is " << _processor << "\n";
    
    dsp21k_reset_bd(_processor); //Reset the board.
    dsp21k_cfg_proc(_processor); //Configure the board.

    error = dsp21k_get_device_info(_processor, &_cfg);
    if (check_error(error) != DSP21K_SUCCESS)
        return;

    //Setup Physical Memory
    //_physmem.phys_addr = _cfg.pci_badr[2] + AD_OFFSET; //Physical Memory address = BAR2 + offset
    _physmem.phys_addr = _cfg.pci_badr[2] + 0x400000/4; //Physical Memory address = BAR2 + offset
    _physmem.size = BUFFER_SIZE; //Physical Memory size

    //Allocate Physical Memory
    error = dsp21k_alloc_phys_memory(_processor, &_physmem);
    if (check_error(error) != DSP21K_SUCCESS) {
        printf("\nAddress/Data Memory Allocation Error!\n");
        return;
    }

    int temp;
    U32 rd_buffer[BUFFER_SIZE / 4];
    U32 wr_buffer[BUFFER_SIZE / 4];

    for (temp=0; temp<5; temp++)
    {
    	error = dsp21k_rd_phys_memory(&_physmem, temp, _physmem.size / 4, rd_buffer);
        if (check_error(error) != DSP21K_SUCCESS) {
            printf("Memory Read Error!\n");
            return;
        }
        printf("Mem0 %x = %x\n", temp, rd_buffer[0]);
        printf("Mem1 %x = %x\n", temp, rd_buffer[1]);
    }
        
    _isok = true;

    std::cout << "Remora  initialized\n";
}

//////////////////////////////////////////////////////////////////////
Bittware::~Bittware() {

}

//////////////////////////////////////////////////////////////////////
void Bittware::configure(unsigned int gates,
                         unsigned int prf,
                         unsigned int pulsewidth,
                         unsigned int samples,
                         bool dualPrt) {

    _gates = gates;
    _prf = prf;
    _pulsewidth = pulsewidth;
    _samples = _samples;
    _dualPrt = dualPrt;

    // Decimation Setup
    int decimationFactor;
    switch (_pulsewidth) {
    case 250:
        decimationFactor = 2;
        break;
    case 500:
        decimationFactor = 4;
        break;
    case 750:
        decimationFactor = 6;
        break;
    case 1000:
        decimationFactor = 8;
        break;
    case 1250:
        decimationFactor = 10;
        break;
    case 1500:
        decimationFactor = 12;
        break;
    case 1750:
        decimationFactor = 14;
        break;
    case 2000:
        decimationFactor = 16;
        break;
    default:
        std::cout << "WARNING (" <<__FILE__ << ":" << __LINE__
        << "): pulse width must be one of: 250, 500, 750, 1000, 1250, 1500, 1750, 2000\n";
        decimationFactor = 8;
    }

    U32 rd_buffer[BUFFER_SIZE / 4];
    U32 wr_buffer[BUFFER_SIZE / 4];

    U32 *reg_buf;

    // Configure  Timer 1 Control Register
    U32 control = BW_TIMER_ON | BW_TIMER_POS | BW_EXT_CLK | BW_CLK_DIV1;
    wr_buffer[0x0] = BW_TIMER1 | BW_CONTROL_REG; //Address Line
    wr_buffer[0x1] = control; //Data Line
    mem_write(wr_buffer);

    // Read Back Timer 1 Control Register
    wr_buffer[0x0] = BW_TIMER1; //Address Line
    mem_write(wr_buffer);
    reg_buf = mem_read(rd_buffer);

    std::cout << "Read Address Line = " << *reg_buf << "\n";
    std::cout << "Read Data Line = " << *(reg_buf+1) << "\n";

    // Configure Timer 1 Delay Register
    wr_buffer[0x0] = BW_TIMER1 | BW_DELAY_REG; //Address Line
    wr_buffer[0x1] = 0; //Data Line
    mem_write(wr_buffer);

    // Read Back Timer 1 Delay Register
    wr_buffer[0x0] = BW_TIMER1; //Address Line
    mem_write(wr_buffer);
    reg_buf = mem_read(rd_buffer);

    // Configure Timer 1 Width Register
    wr_buffer[0x0] = BW_TIMER1 | BW_WIDTH_REG; //Address Line
    wr_buffer[0x1] = _gates*60*decimationFactor/8; //Data Line
    mem_write(wr_buffer);

    // Read Back Timer 1 Width Register
    wr_buffer[0x0] = BW_TIMER1; //Address Line
    mem_write(wr_buffer);
    reg_buf = mem_read(rd_buffer);

    // Configure Timer 1 Period Register
    double prtClock = (60e6);
    int periodCount = (int) (prtClock/_prf);

    wr_buffer[0x0] = BW_TIMER1 | BW_PERIOD_REG; //Address Line
    wr_buffer[0x1] = periodCount; //Data Line
    mem_write(wr_buffer);

    // Read Back Timer 1 Period Register
    wr_buffer[0x0] = BW_TIMER1; //Address Line
    mem_write(wr_buffer);
    reg_buf = mem_read(rd_buffer);

    // Configure Timer 1 multiple PRT Register
    wr_buffer[0x0] = BW_TIMER1 | BW_PRT_REG; //Address Line
    if (!_dualPrt) {
        wr_buffer[0x1] = 0x0000;
    } else {
        wr_buffer[0x1] = 0x0054;
    }
    mem_write(wr_buffer);

    // Read Back Timer 1 multiple PRT Register
    wr_buffer[0x0] = BW_TIMER1; //Address Line
    mem_write(wr_buffer);
    reg_buf = mem_read(rd_buffer);

    std::cout << "Remora configured\n";
}

/////////////////////////////////////////////////////////////////////////////
void Bittware::start() {

    U32 wr_buffer[BUFFER_SIZE / 4]; //Write Buffer

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
U32 * Bittware::mem_read(U32 rd_buffer[]) {
    int error;
    U32 *read;
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
void Bittware::mem_write(U32 wr_buffer[]) {
    int error;
    error = dsp21k_wr_phys_memory(&_physmem, 0, _physmem.size / 4, wr_buffer);
    if (check_error(error) != DSP21K_SUCCESS) {
        printf("Memory Write Error!\n");
        return;
    }
    return;
}

////////////////////////////////////////////////////////////////////////
void Bittware::copy_buffer(U32 wr_buffer[],
                           U32 rd_buffer[]) {
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
