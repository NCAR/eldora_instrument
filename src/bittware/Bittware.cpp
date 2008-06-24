#include "Bittware.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>		/* for memcpy and string routines */
#include <iostream>

// Greatest common divisor
static inline int GCD(int a, int b) {
    while (1) {
        a %= b;
        if (a == 0)
            return b;
        b %= a;
        if (b == 0)
            return a;
    }
}


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
    shutdown();
}

//////////////////////////////////////////////////////////////////////
void Bittware::configure(EldoraRadarParams radarParams) {

    U32 wr_buffer[BUFFER_SIZE / 4];
    U32 control = BW_TIMER_ON | BW_TIMER_POS | BW_EXT_CLK | BW_CLK_DIV1;
    U32 Timer;
    
    double clockFreq = (60e6);  // Timer Input Clock Freq
    int baseCount;              // base period in clock counts for all timers
    int prtScheme;              // encoded multipliers for timers
    int prtSchemeCount;         // period in clock counts between scheme repeats
    
// Calculated Timing Variables from Header Variables
    
    // Test Pulse Calculations
    U32 TP_Delay = ((int) ((radarParams.wave_chpoff[0] + radarParams.wave_gate1[0] + 
                           (radarParams.wave_ngates[0] - 5) * radarParams.wave_gate1[1]) / 10)) * 10;
    U32 TP_Width = (int) (radarParams.wave_gate1[1] * 5);
    
    // Calculate the period and PRT Scheme for dual prt or single prt
    if (radarParams.radd_nipp == 2)  //dual prt
    {
        // Convert inter-pulse periods to clock counts.  The ipp values are 
        // in milliseconds.
        int ipp1Count = (int)(radarParams.radd_ipp1 * 0.001 * clockFreq + 0.5);
        int ipp2Count = (int)(radarParams.radd_ipp2 * 0.001 * clockFreq + 0.5);
        
        // Find the GCD (in counts) of the two periods.  All timers will be
        // an integer multiple of this period.
        baseCount = GCD(ipp1Count, ipp2Count);
        
        // Find the multipliers to turn the base period into IPP1 and IPP2
        int mult1 = ipp1Count / baseCount;
        int mult2 = ipp2Count / baseCount;

        // encode the multipliers into the PRT scheme
        prtScheme = (mult2 << 4) + mult1;
        
        // Period between PRT scheme repeats
        prtSchemeCount = (mult1 + mult2) * baseCount;  
    }
    else  //single prt
    {
        // Convert inter-pulse period to clock counts.  The ipp is 
        // in milliseconds.
        baseCount = (int)(radarParams.radd_ipp1 * 0.001 * clockFreq + 0.5);
        prtScheme = 0x0000;
        
        // Period between PRT scheme repeats
        prtSchemeCount = baseCount;    
    }
    
    // Midbeam Interrupt Calculation
    U32 Midbeam_Width = 60;
    U32 Midbeam_Prt = 0x0;
    U32 Midbeam_Delay;
    U32 Midbeam_periodCount;
    Midbeam_periodCount = prtSchemeCount * radarParams.wave_seqrep;  
    Midbeam_Delay = Midbeam_periodCount / 2 +
        (int)(radarParams.wave_chpoff[0] + radarParams.wave_gate1[0] + 0.5);
    
// Initialize the Bittware Card and Fill Registers
    
    // Reset Timers and DCM
    wr_buffer[0x0] = BW_TIMER_RST;
    mem_write(wr_buffer);
    wr_buffer[0x0] = 0;
    mem_write(wr_buffer);
    
    // Check DCM Lock Status
    //U32 rd_buffer[BUFFER_SIZE / 4];
    //U32 *reg_buf;
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
    printf("FORE & AFT TX Pulses\n");        
    for (int i = 0; i < 4; i++)
    {
    	//FORE
    	Timer = (i<<4) + TX_FORE_CH1 ;
    	
    	// Configure FORE Control Register
    	wr_buffer[0x0] = BW_WRITE | Timer | BW_CONTROL_REG; //Address Line
    	wr_buffer[0x1] = control; //Data Line
    	mem_write(wr_buffer);
    	// Configure FORE Delay Register
    	wr_buffer[0x0] = BW_WRITE | Timer | BW_DELAY_REG; //Address Line
    	wr_buffer[0x1] = radarParams.wave_chpoff[i]; //Data Line
    	mem_write(wr_buffer);
    	// Configure FORE Width Register
    	wr_buffer[0x0] = BW_WRITE | Timer | BW_WIDTH_REG; //Address Line
    	wr_buffer[0x1] = radarParams.wave_chpwid[i]; //Data Line
    	mem_write(wr_buffer);
    	// Configure FORE Period Register 
    	wr_buffer[0x0] = BW_WRITE | Timer | BW_PERIOD_REG; //Address Line
    	wr_buffer[0x1] = baseCount; //Data Line
    	mem_write(wr_buffer);
    	// Configure FORE multiple PRT Register
    	wr_buffer[0x0] = BW_WRITE | Timer | BW_PRT_REG; //Address Line
        wr_buffer[0x1] = prtScheme;
    	mem_write(wr_buffer); 
    	
    	printf("Fore CH%i : Delay = %i: Width = %i: Period = %i: PRT = %x\n", 
    	        Timer>>4, 
    	        radarParams.wave_chpoff[i], 
    	        radarParams.wave_chpwid[i],
    	        baseCount, 
    	        prtScheme);   
    	        
    	// AFT
    	Timer = (i<<4) + TX_AFT_CH1;
    	
    	// Configure FORE Control Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_CONTROL_REG; //Address Line
        wr_buffer[0x1] = control; //Data Line
        mem_write(wr_buffer);
        // Configure FORE Delay Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_DELAY_REG; //Address Line
        wr_buffer[0x1] = radarParams.wave_chpoff[i]; //Data Line
        mem_write(wr_buffer);
        // Configure FORE Width Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_WIDTH_REG; //Address Line
        wr_buffer[0x1] = radarParams.wave_chpwid[i]; //Data Line
        mem_write(wr_buffer);
        // Configure FORE Period Register 
        wr_buffer[0x0] = BW_WRITE | Timer | BW_PERIOD_REG; //Address Line
        wr_buffer[0x1] = baseCount; //Data Line
        mem_write(wr_buffer);
        // Configure FORE multiple PRT Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_PRT_REG; //Address Line
        wr_buffer[0x1] = prtScheme;
        mem_write(wr_buffer);     	
        
        printf("Aft CH%i : Delay = %i: Width = %i: Period = %i: PRT = %x\n", 
                        Timer>>4, 
                        radarParams.wave_chpoff[i], 
                        radarParams.wave_chpwid[i],
                        baseCount, 
                        prtScheme);   
                
    }
 
    // Initialize FORE & AFT Pre-Knock Pulses        
       printf("Pre-Knocks\n");
       //FORE
       Timer = TX_FORE_PK;
       // Configure FORE Control Register
       wr_buffer[0x0] = BW_WRITE | Timer | BW_CONTROL_REG; //Address Line
       wr_buffer[0x1] = control; //Data Line
       mem_write(wr_buffer);
       // Configure FORE Delay Register
       wr_buffer[0x0] = BW_WRITE | Timer | BW_DELAY_REG; //Address Line
       wr_buffer[0x1] = radarParams.wave_chpoff[5]; //Data Line
       mem_write(wr_buffer);
       // Configure FORE Width Register
       wr_buffer[0x0] = BW_WRITE | Timer | BW_WIDTH_REG; //Address Line
       wr_buffer[0x1] = radarParams.wave_chpwid[5]; //Data Line
       mem_write(wr_buffer);
       // Configure FORE Period Register 
       wr_buffer[0x0] = BW_WRITE | Timer | BW_PERIOD_REG; //Address Line
       wr_buffer[0x1] = baseCount; //Data Line
       mem_write(wr_buffer);
       // Configure FORE multiple PRT Register
       wr_buffer[0x0] = BW_WRITE | Timer | BW_PRT_REG; //Address Line
       wr_buffer[0x1] = prtScheme;
       mem_write(wr_buffer);
       
       printf("Fore PK : Delay = %i: Width = %i: Period = %i: PRT = %x\n", 
                       radarParams.wave_chpoff[5], 
                       radarParams.wave_chpwid[5],
                       baseCount, 
                       prtScheme);   
       
       
       // AFT
       Timer = TX_AFT_PK;
       // Configure FORE Control Register
       wr_buffer[0x0] = BW_WRITE | Timer | BW_CONTROL_REG; //Address Line
       wr_buffer[0x1] = control; //Data Line
       mem_write(wr_buffer);
       // Configure FORE Delay Register
       wr_buffer[0x0] = BW_WRITE | Timer | BW_DELAY_REG; //Address Line
       wr_buffer[0x1] = radarParams.wave_chpoff[5]; //Data Line
       mem_write(wr_buffer);
       // Configure FORE Width Register
       wr_buffer[0x0] = BW_WRITE | Timer | BW_WIDTH_REG; //Address Line
       wr_buffer[0x1] = radarParams.wave_chpwid[5]; //Data Line
       mem_write(wr_buffer);
       // Configure FORE Period Register 
       wr_buffer[0x0] = BW_WRITE | Timer | BW_PERIOD_REG; //Address Line
       wr_buffer[0x1] = baseCount; //Data Line
       mem_write(wr_buffer);
       // Configure FORE multiple PRT Register
       wr_buffer[0x0] = BW_WRITE | Timer | BW_PRT_REG; //Address Line
       wr_buffer[0x1] = prtScheme;
       mem_write(wr_buffer);          
       
       printf("Aft PK : Delay = %i: Width = %i: Period = %i: PRT = %x\n", 
                              radarParams.wave_chpoff[5], 
                              radarParams.wave_chpwid[5],
                              baseCount, 
                              prtScheme);   
       
              
    // Initialize FORE & AFT Test Pulses        
        printf("Test Pulses\n");
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
       	wr_buffer[0x1] = baseCount; //Data Line
       	mem_write(wr_buffer);
       	// Configure FORE multiple PRT Register
       	wr_buffer[0x0] = BW_WRITE | Timer | BW_PRT_REG; //Address Line
        wr_buffer[0x1] = prtScheme;
       	mem_write(wr_buffer);
       	
       	printf("Fore TP : Delay = %i: Width = %i: Period = %i: PRT = %x\n", 
       	                       TP_Delay, 
       	                       TP_Width,
       	                       baseCount, 
       	                       prtScheme);   
       	       
       	 
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
        wr_buffer[0x1] = baseCount; //Data Line
        mem_write(wr_buffer);
        // Configure FORE multiple PRT Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_PRT_REG; //Address Line
        wr_buffer[0x1] = prtScheme;
        mem_write(wr_buffer); 
        
        printf("Aft TP : Delay = %i: Width = %i: Period = %i: PRT = %x\n", 
                               TP_Delay, 
                               TP_Width,
                               baseCount, 
                               prtScheme);   
               
    
    // Initialize FORE & AFT RX Pulses
    printf("RX Pulses\n");
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
        wr_buffer[0x1] = radarParams.wave_gate1[0] + radarParams.wave_chpoff[i]; //Data Line
        mem_write(wr_buffer);
        // Configure FORE Width Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_WIDTH_REG; //Address Line
        wr_buffer[0x1] = radarParams.wave_chpwid[i] * radarParams.wave_ngates[i]; //Data Line
        mem_write(wr_buffer);
        // Configure FORE Period Register 
        wr_buffer[0x0] = BW_WRITE | Timer | BW_PERIOD_REG; //Address Line
        wr_buffer[0x1] = baseCount; //Data Line
        mem_write(wr_buffer);
        // Configure FORE multiple PRT Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_PRT_REG; //Address Line
        wr_buffer[0x1] = prtScheme;
        mem_write(wr_buffer); 
        
        printf("Fore CH%i : Delay = %i: Width = %i: Period = %i: PRT = %x\n", 
                        Timer>>4, 
                        radarParams.wave_gate1[0] + radarParams.wave_chpoff[i], 
                        radarParams.wave_chpwid[i] * radarParams.wave_ngates[i], 
                        baseCount, 
                        prtScheme);   
        
        
        // AFT
        Timer = (i<<4) + RX_AFT_CH1;   
        
        // Configure FORE Control Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_CONTROL_REG; //Address Line
        wr_buffer[0x1] = control; //Data Line
        mem_write(wr_buffer);
        // Configure FORE Delay Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_DELAY_REG; //Address Line
        wr_buffer[0x1] = radarParams.wave_gate1[0] + radarParams.wave_chpoff[i]; //Data Line
        mem_write(wr_buffer);
        // Configure FORE Width Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_WIDTH_REG; //Address Line
        wr_buffer[0x1] = radarParams.wave_chpwid[i] * radarParams.wave_ngates[i]; //Data Line
        mem_write(wr_buffer);
        // Configure FORE Period Register 
        wr_buffer[0x0] = BW_WRITE | Timer | BW_PERIOD_REG; //Address Line
        wr_buffer[0x1] = baseCount; //Data Line
        mem_write(wr_buffer);
        // Configure FORE multiple PRT Register
        wr_buffer[0x0] = BW_WRITE | Timer | BW_PRT_REG; //Address Line
        wr_buffer[0x1] = prtScheme;
        mem_write(wr_buffer);
        
        printf("Aft CH%i : Delay = %i: Width = %i: Period = %i: PRT = %x\n", 
                                Timer>>4, 
                                radarParams.wave_gate1[0] + radarParams.wave_chpoff[i], 
                                radarParams.wave_chpwid[i] * radarParams.wave_ngates[i], 
                                baseCount, 
                                prtScheme);   
        
                        
    }
                
    // Configure the Midbeam Intr
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
        
        printf("Midbeam Interrupt\n");
        printf("Midbeam: Delay = %i: Width = %i: Period = %i: PRT = %x\n",          
                                Midbeam_Delay, 
                                Midbeam_Width, 
                                Midbeam_periodCount, 
                                Midbeam_Prt);   
                       
        
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
