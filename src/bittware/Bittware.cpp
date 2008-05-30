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
    _samples = samples;
    _dualPrt = dualPrt;

    // Decimation Setup
    int decimationFactor = _pulsewidth/125;
    if (decimationFactor != 2 && decimationFactor != 4 && 
    	decimationFactor != 6 && decimationFactor != 8 && 
    	decimationFactor != 10 && decimationFactor != 12 &&
    	decimationFactor != 14 && decimationFactor != 16)
    {
    	std::cout << "WARNING (" <<__FILE__ << ":" << __LINE__
    	<< "): pulse width must be one of: 250, 500, 750, 1000, 1250, 1500, 1750, 2000\n";
    	decimationFactor = 8;
    }
    
    U32 rd_buffer[BUFFER_SIZE / 4];
    U32 wr_buffer[BUFFER_SIZE / 4];
    U32 *reg_buf;
    
    U32 control = BW_TIMER_ON | BW_TIMER_POS | BW_EXT_CLK | BW_CLK_DIV1;
    U32 Timer;
    
    double prtClock = (60e6);      // Timer Input Clock Freq
    int periodCount;               // Period Count for all Timers
    int PrtScheme;                 // PRT Scheme for all Timers
    
// Timer Variables Parsed from Header
    float IPP1 = 0.5;              // Inter Pulse Period 1
    float IPP2 = 0.625;            // Inter Pulse Period 2
    int Gate_Dist[2];              // TP [Delay, Counts/Gate]
       Gate_Dist[0] = 90;
       Gate_Dist[1] = 60;    
    
    U32 TX_Delay[5];               // TX Pulse Delays
        TX_Delay[0] = 17;
        TX_Delay[1] = 77;
        TX_Delay[2] = 137;
        TX_Delay[3] = 197;
        TX_Delay[4] = 0;
                
        
    U32 TX_Width[5];               // TX Pulse Widths
        TX_Width[0] = 60;
        TX_Width[1] = 60;
        TX_Width[2] = 60;
        TX_Width[3] = 60;
        TX_Width[4] = 257;         
    
// Calculated Timing Variables from Header Variables
    
    // Test Pulse Calculations
    U32 TP_Delay = ((int) ((TX_Delay[0] + Gate_Dist[0] + (_gates - 50) * Gate_Dist[1]) / 10)) * 10;
    //U32 TP_Delay = ((int) (((_gates - 4) * Gate_Dist[1]) / 10)) * 10;
    U32 TP_Width = (int) (Gate_Dist[1] * 50);
    
    // RX Pulse Calculations
    U32 RX_Delay[4];
    RX_Delay[0] = Gate_Dist[1] + TX_Delay[0];
    RX_Delay[1] = Gate_Dist[1] + TX_Delay[1];
    RX_Delay[2] = Gate_Dist[1] + TX_Delay[2];
    RX_Delay[3] = Gate_Dist[1] + TX_Delay[3];        
    
    U32 RX_Width[4];    
    //RX_Width[0] = _gates * Gate_Dist[1]; 
    //RX_Width[1] = _gates * Gate_Dist[1]; 
    //RX_Width[2] = _gates * Gate_Dist[1]; 
    //RX_Width[3] = _gates * Gate_Dist[1];               
    RX_Width[0] = _gates * decimationFactor / 8 * 60; 
    RX_Width[1] = _gates * decimationFactor / 8 * 60;
    RX_Width[2] = _gates * decimationFactor / 8 * 60;
    RX_Width[3] = _gates * decimationFactor / 8 * 60;
               
      
    // Calculate the period and PRT Scheme for dual prt or single prt
    int X, Y;
    if (_dualPrt)  //dual prt
    {
        X = (int) ((int) (IPP2 / IPP1) / ((float) IPP2 / IPP1 - (int) (IPP2 / IPP1)));
        Y = (int) (X * IPP2 / IPP1);
        PrtScheme = (Y<<4) | X;
        periodCount = (int) (IPP1 * ((float) IPP2/IPP1 - (int) (IPP2/IPP1)) / (int) (IPP2/IPP1) * prtClock / 1e3);
       
    }
    else  //single prt
    {
    	//periodCount = (int) (IPP1 * prtClock / 1e3);    
    	periodCount = (int) (prtClock / _prf);  
    	PrtScheme = 0x0000;
    }
    
    // Midbeam Interrupt Calculation
    U32 Midbeam_Width = 60;
    U32 Midbeam_Prt = 0x0;
    U32 Midbeam_Delay;
    U32 Midbeam_periodCount;
    if (_dualPrt)
    {
        Midbeam_Delay = (int) (RX_Delay[0] + (((IPP1+IPP2) * (_samples - 1) + IPP1) * 10e3) + _gates);
    	Midbeam_periodCount = periodCount * (X + Y) * _samples;
    }
    else
    {
    	//Midbeam_Delay = (int) (RX_Delay[0] + ((IPP1 * 10e3) * (_samples - 1)) + _gates);
    	Midbeam_Delay = periodCount * _samples / 2; 
    	Midbeam_periodCount = periodCount * _samples;
    }   
    
// Initialize the Bittware Card and Fill Registers
    
    // Reset Timers and DCM
    wr_buffer[0x0] = BW_TIMER_RST;
    mem_write(wr_buffer);
    wr_buffer[0x0] = 0;
    mem_write(wr_buffer);
    
    // Check DCM Lock Status
    //reg_buf = mem_read(rd_buffer);
    //printf("Status Reg = %08x\n", *reg_buf);
    //if (*reg_buf & BW_TIMER_DCMLOCK) printf("Timer DCM Failed to Lock!\n");
    //else printf("Timer DCM Failed to Lock!\n");
    
    // Example - Read Back Timer 1 Control Register
    //wr_buffer[0x0] = BW_READ | Timer | BW_CONTROL_REG; //Address Line
    //mem_write(wr_buffer);
    //reg_buf = mem_read(rd_buffer);
    //printf("Timer 1 Control = %i\n", *(reg_buf+2));
                
    // Initialize FORE & AFT TX Pulses
    for (int i = 0; i < 5; i++)
    {
    	//FORE
    	Timer = (i<<4) + TX_FORE_CH1 ;
    	
    	// Configure FORE Control Register
    	wr_buffer[0x0] = BW_WRITE | Timer | BW_CONTROL_REG; //Address Line
    	wr_buffer[0x1] = control; //Data Line
    	mem_write(wr_buffer);
    	// Configure FORE Delay Register
    	wr_buffer[0x0] = BW_WRITE | Timer | BW_DELAY_REG; //Address Line
    	wr_buffer[0x1] = TX_Delay[i]; //Data Line
    	mem_write(wr_buffer);
    	// Configure FORE Width Register
    	wr_buffer[0x0] = BW_WRITE | Timer | BW_WIDTH_REG; //Address Line
    	wr_buffer[0x1] = TX_Width[i]; //Data Line
    	mem_write(wr_buffer);
    	// Configure FORE Period Register 
    	wr_buffer[0x0] = BW_WRITE | Timer | BW_PERIOD_REG; //Address Line
    	wr_buffer[0x1] = periodCount; //Data Line
    	mem_write(wr_buffer);
    	// Configure FORE multiple PRT Register
    	wr_buffer[0x0] = BW_WRITE | Timer | BW_PRT_REG; //Address Line
        wr_buffer[0x1] = PrtScheme;
    	mem_write(wr_buffer); 

    	// AFT
    	Timer = (i<<4) + TX_AFT_CH1;
    	
    	// Configure FORE Control Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_CONTROL_REG; //Address Line
        wr_buffer[0x1] = control; //Data Line
        mem_write(wr_buffer);
        // Configure FORE Delay Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_DELAY_REG; //Address Line
        wr_buffer[0x1] = TX_Delay[i]; //Data Line
        mem_write(wr_buffer);
        // Configure FORE Width Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_WIDTH_REG; //Address Line
        wr_buffer[0x1] = TX_Width[i]; //Data Line
        mem_write(wr_buffer);
        // Configure FORE Period Register 
        wr_buffer[0x0] = BW_WRITE | Timer | BW_PERIOD_REG; //Address Line
        wr_buffer[0x1] = periodCount; //Data Line
        mem_write(wr_buffer);
        // Configure FORE multiple PRT Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_PRT_REG; //Address Line
        wr_buffer[0x1] = PrtScheme;
        mem_write(wr_buffer);     	
    }
 
    // Initialize FORE & AFT Test Pulses        
        
        //FORE
       	Timer = TX_FORE_TP;
       	// Configure FORE Control Register
       	wr_buffer[0x0] = BW_WRITE | Timer | BW_CONTROL_REG; //Address Line
       	wr_buffer[0x1] = control; //Data Line
       	mem_write(wr_buffer);
       	// Configure FORE Delay Register
       	wr_buffer[0x0] = BW_WRITE | Timer | BW_DELAY_REG; //Address Line
       	wr_buffer[0x1] = TP_Delay; //Data Line
       	mem_write(wr_buffer);
       	// Configure FORE Width Register
       	wr_buffer[0x0] = BW_WRITE | Timer | BW_WIDTH_REG; //Address Line
       	wr_buffer[0x1] = TP_Width; //Data Line
       	mem_write(wr_buffer);
       	// Configure FORE Period Register 
       	wr_buffer[0x0] = BW_WRITE | Timer | BW_PERIOD_REG; //Address Line
       	wr_buffer[0x1] = periodCount; //Data Line
       	mem_write(wr_buffer);
       	// Configure FORE multiple PRT Register
       	wr_buffer[0x0] = BW_WRITE | Timer | BW_PRT_REG; //Address Line
        wr_buffer[0x1] = PrtScheme;
       	mem_write(wr_buffer);
       	 
       	// AFT
       	Timer = TX_AFT_TP;
       	// Configure FORE Control Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_CONTROL_REG; //Address Line
        wr_buffer[0x1] = control; //Data Line
        mem_write(wr_buffer);
        // Configure FORE Delay Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_DELAY_REG; //Address Line
        wr_buffer[0x1] = TP_Delay; //Data Line
        mem_write(wr_buffer);
        // Configure FORE Width Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_WIDTH_REG; //Address Line
        wr_buffer[0x1] = TP_Width; //Data Line
        mem_write(wr_buffer);
        // Configure FORE Period Register 
        wr_buffer[0x0] = BW_WRITE | Timer | BW_PERIOD_REG; //Address Line
        wr_buffer[0x1] = periodCount; //Data Line
        mem_write(wr_buffer);
        // Configure FORE multiple PRT Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_PRT_REG; //Address Line
        wr_buffer[0x1] = PrtScheme;
        mem_write(wr_buffer);     	   
    
    // Initialize FORE & AFT RX Pulses
    for (int i = 0 ; i < 4 ; i++)
    {
        //FORE
        Timer = (i<<4) + RX_FORE_CH1 ;
        
        // Configure FORE Control Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_CONTROL_REG; //Address Line
        wr_buffer[0x1] = control; //Data Line
        mem_write(wr_buffer);
        // Configure FORE Delay Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_DELAY_REG; //Address Line
        wr_buffer[0x1] = RX_Delay[i]; //Data Line
        mem_write(wr_buffer);
        // Configure FORE Width Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_WIDTH_REG; //Address Line
        wr_buffer[0x1] = RX_Width[i]; //Data Line
        mem_write(wr_buffer);
        // Configure FORE Period Register 
        wr_buffer[0x0] = BW_WRITE | Timer | BW_PERIOD_REG; //Address Line
        wr_buffer[0x1] = periodCount; //Data Line
        mem_write(wr_buffer);
        // Configure FORE multiple PRT Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_PRT_REG; //Address Line
        wr_buffer[0x1] = PrtScheme;
        mem_write(wr_buffer); 
        //printf("Fore CH%i : Delay = %i: Width = %i: Period = %i: PRT = %i\n", Timer>>4, RX_Delay[i], RX_Width[i], periodCount, PrtScheme);   
        
        // AFT
        Timer = (i<<4) + RX_AFT_CH1;   
        
        // Configure FORE Control Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_CONTROL_REG; //Address Line
        wr_buffer[0x1] = control; //Data Line
        mem_write(wr_buffer);
        // Configure FORE Delay Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_DELAY_REG; //Address Line
        wr_buffer[0x1] = RX_Delay[i]; //Data Line
        mem_write(wr_buffer);
        // Configure FORE Width Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_WIDTH_REG; //Address Line
        wr_buffer[0x1] = RX_Width[i]; //Data Line
        mem_write(wr_buffer);
        // Configure FORE Period Register 
        wr_buffer[0x0] = BW_WRITE | Timer | BW_PERIOD_REG; //Address Line
        wr_buffer[0x1] = periodCount; //Data Line
        mem_write(wr_buffer);
        // Configure FORE multiple PRT Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_PRT_REG; //Address Line
        wr_buffer[0x1] = PrtScheme;
        mem_write(wr_buffer);
        //printf("Aft CH%i : Delay = %i: Width = %i: Period = %i: PRT = %i\n", Timer>>4, RX_Delay[i], RX_Width[i], periodCount, PrtScheme);   
                
    }
                
    // Configure the Midbeam Inrt
    	Timer = MIDBEAM_INTR;
    	    	
    	// Configure Control Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_CONTROL_REG; //Address Line
        wr_buffer[0x1] = control; //Data Line
        mem_write(wr_buffer);
        // Configure Delay Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_DELAY_REG; //Address Line
        wr_buffer[0x1] = Midbeam_Delay; //Data Line
        mem_write(wr_buffer);
        // Configure Width Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_WIDTH_REG; //Address Line
        wr_buffer[0x1] = Midbeam_Width; //Data Line
        mem_write(wr_buffer);
        // Configure Period Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_PERIOD_REG; //Address Line
        wr_buffer[0x1] = Midbeam_periodCount; //Data Line
        mem_write(wr_buffer);
        // Configure multiple PRT Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_PRT_REG; //Address Line
        wr_buffer[0x1] = Midbeam_Prt;
        mem_write(wr_buffer);
                
        
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
    //wr_buffer[0x0] |= BW_ADDR_TRIG; //Address Line
    //mem_write(wr_buffer);

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
