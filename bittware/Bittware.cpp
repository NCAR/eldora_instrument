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

    U32 Timer, control;
    double prtClock;
    int periodCount;
    
    // Reset Timers and DCM
    wr_buffer[0x0] = BW_TIMER_RST;
    mem_write(wr_buffer);
    wr_buffer[0x0] = 0;
    mem_write(wr_buffer);
    
    // Check DCM Lock Status
    reg_buf = mem_read(rd_buffer);
    //printf("Status Reg = %08x\n", *reg_buf);
    if (*reg_buf & BW_TIMER_DCMLOCK) printf("Timer DCM Failed to Lock!\n");
    else printf("Timer DCM Failed to Lock!\n");
    
    int stagger = 0;
    
    for (Timer = 0x0<<4; Timer < 0x8<<4; Timer += 0x1<<4)
    {
    
    // Configure  Timer 1 Control Register
    control = BW_TIMER_ON | BW_TIMER_POS | BW_EXT_CLK | BW_CLK_DIV1;
    wr_buffer[0x0] = BW_WRITE | Timer | BW_CONTROL_REG; //Address Line
    wr_buffer[0x1] = control; //Data Line
    mem_write(wr_buffer);
    
    // Read Back Timer 1 Control Register
    wr_buffer[0x0] = BW_READ | Timer | BW_CONTROL_REG; //Address Line
    mem_write(wr_buffer);
    reg_buf = mem_read(rd_buffer);
    
    // Configure Timer 1 Delay Register
    wr_buffer[0x0] = BW_WRITE | Timer | BW_DELAY_REG; //Address Line
    wr_buffer[0x1] = stagger; //Data Line
    mem_write(wr_buffer);
    
    stagger += 10;
    
    // Read Back Timer 1 Delay Register
    wr_buffer[0x0] = BW_READ | Timer | BW_DELAY_REG; //Address Line
    mem_write(wr_buffer);
    reg_buf = mem_read(rd_buffer);
     
    // Configure Timer 1 Width Register
    wr_buffer[0x0] = BW_WRITE | Timer | BW_WIDTH_REG; //Address Line
    //wr_buffer[0x1] = _gates*60*decimationFactor/8; //Data Line
    wr_buffer[0x1] = _gates*10*decimationFactor/8; //Data Line
    printf("Width = %i\n", _gates*10*decimationFactor/8);
    //wr_buffer[0x1] = 100; //Data Line
    mem_write(wr_buffer);

    // Read Back Timer 1 Width Register
    wr_buffer[0x0] = BW_READ | Timer | BW_WIDTH_REG; //Address Line
    mem_write(wr_buffer);
    reg_buf = mem_read(rd_buffer);
    
    // Configure Timer 1 Period Register
    prtClock = (10e6);
    periodCount = (int) (prtClock/_prf);

    wr_buffer[0x0] = BW_WRITE | Timer | BW_PERIOD_REG; //Address Line
    wr_buffer[0x1] = periodCount; //Data Line
    printf("Period = %i\n", periodCount);
    //wr_buffer[0x1] = 200; //Data Line
    mem_write(wr_buffer);
    
    // Read Back Timer 1 Period Register
    wr_buffer[0x0] = BW_READ | Timer | BW_PERIOD_REG; //Address Line
    mem_write(wr_buffer);
    reg_buf = mem_read(rd_buffer);

    // Configure Timer 1 multiple PRT Register
    wr_buffer[0x0] = BW_WRITE | Timer | BW_PRT_REG; //Address Line
    if (!_dualPrt) {
        wr_buffer[0x1] = 0x0000;
    } else {
        wr_buffer[0x1] = 0x0054;
    }
    mem_write(wr_buffer);
    
    // Read Back Timer 1 multiple PRT Register
    wr_buffer[0x0] = BW_READ | Timer | BW_PRT_REG; //Address Line
    mem_write(wr_buffer);
    reg_buf = mem_read(rd_buffer);
    
    }
    
    // Configure the Test Pulse
    Timer = 0x8<<4;
    
    // Configure  Timer 1 Control Register
    	control = BW_TIMER_ON | BW_TIMER_POS | BW_EXT_CLK | BW_CLK_DIV1;
        wr_buffer[0x0] = BW_WRITE | Timer | BW_CONTROL_REG; //Address Line
        wr_buffer[0x1] = control; //Data Line
        mem_write(wr_buffer);
        
        // Read Back Timer 1 Control Register
        wr_buffer[0x0] = BW_READ | Timer | BW_CONTROL_REG; //Address Line
        mem_write(wr_buffer);
        reg_buf = mem_read(rd_buffer);
        
        // Configure Timer 1 Delay Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_DELAY_REG; //Address Line
        wr_buffer[0x1] = 500-30; //Data Line
        printf("Test Pulse = %i gate\n", wr_buffer[0x1]);
        mem_write(wr_buffer);
        
        // Read Back Timer 1 Delay Register
        wr_buffer[0x0] = BW_READ | Timer | BW_DELAY_REG; //Address Line
        mem_write(wr_buffer);
        reg_buf = mem_read(rd_buffer);
         
        // Configure Timer 1 Width Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_WIDTH_REG; //Address Line
        //wr_buffer[0x1] = _gates*60*decimationFactor/8; //Data Line
        wr_buffer[0x1] = 30; //Data Line
        printf("Test Pulse = %i gates long\n", wr_buffer[0x1]);
        mem_write(wr_buffer);

        // Read Back Timer 1 Width Register
        wr_buffer[0x0] = BW_READ | Timer | BW_WIDTH_REG; //Address Line
        mem_write(wr_buffer);
        reg_buf = mem_read(rd_buffer);
        
        // Configure Timer 1 Period Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_PERIOD_REG; //Address Line
        wr_buffer[0x1] = periodCount; //Data Line
        //wr_buffer[0x1] = 200; //Data Line
        mem_write(wr_buffer);
        
        // Read Back Timer 1 Period Register
        wr_buffer[0x0] = BW_READ | Timer | BW_PERIOD_REG; //Address Line
        mem_write(wr_buffer);
        reg_buf = mem_read(rd_buffer);

        // Configure Timer 1 multiple PRT Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_PRT_REG; //Address Line
        if (!_dualPrt) {
            wr_buffer[0x1] = 0x0000;
        } else {
            wr_buffer[0x1] = 0x0054;
        }
        mem_write(wr_buffer);
        
        // Read Back Timer 1 multiple PRT Register
        wr_buffer[0x0] = BW_READ | Timer | BW_PRT_REG; //Address Line
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
    wr_buffer[0x0] |= BW_ADDR_TRIG; //Address Line
    mem_write(wr_buffer);

    std::cout << "Remora started\n";

}

/////////////////////////////////////////////////////////////////////////////
void Bittware::shutdown() {

	U32 wr_buffer[BUFFER_SIZE / 4];
	
    //Reset the Bittware Card to stop pulses.
	wr_buffer[0x0] = BW_TIMER_RST;
	mem_write(wr_buffer);
	wr_buffer[0x0] = 0;
	mem_write(wr_buffer);    	
	    
	//Free allocated physical memory buffer
    dsp21k_free_phys_memory(_processor, &_physmem);
    
    //printf("Bittware Card Shutdown\n");
  
}

////////////////////////////////////////////////////////////////////////
U32 * Bittware::mem_read(U32 rd_buffer[]) {
    //int error;
    U32 *read;
    //error = dsp21k_rd_phys_memory(&_physmem, 0, _physmem.size / 4, rd_buffer);
    //if (check_error(error) != DSP21K_SUCCESS) {
    //    printf("Memory Read Error!\n");
    //    return 0;
    //}
    
    rd_buffer[0] = ((U32*)_physmem.mem_ptr)[0];
    rd_buffer[1] = ((U32*)_physmem.mem_ptr)[1];
    rd_buffer[2] = ((U32*)_physmem.mem_ptr)[2];
    read = rd_buffer;
    return read;
    //return rd_buffer[1];
}

////////////////////////////////////////////////////////////////////////
void Bittware::mem_write(U32 wr_buffer[]) {
    //int error;
    //error = dsp21k_wr_phys_memory(&_physmem, 0, _physmem.size / 4, wr_buffer);
    //if (check_error(error) != DSP21K_SUCCESS) {
    //    printf("Memory Write Error!\n");
    //    return;
    //}
	((U32*)_physmem.mem_ptr)[1] = wr_buffer[0];
	((U32*)_physmem.mem_ptr)[2] = wr_buffer[1];
		
	
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
