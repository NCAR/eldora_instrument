#ifndef BITTWARE_H_
#define BITTWARE_H_

#include "dsp21k.h"

// Bittware Timer Registers

//Address Register
#define BW_TIMER1    (0x01<<4)
#define BW_TIMER2    (0x02<<4)
#define BW_TIMER3    (0x03<<4)
#define BW_TIMER4    (0x04<<4)
#define BW_TIMER5    (0x05<<4)
#define BW_TIMER6    (0x06<<4)
#define BW_TIMER7    (0x07<<4)
#define BW_TIMER8    (0x08<<4)
#define BW_TIMER9    (0x09<<4)
#define BW_TIMER10   (0x0A<<4)
#define BW_TIMER11   (0xB<<4)
#define BW_TIMER12   (0xC<<4)
#define BW_TIMER13   (0xD<<4)
#define BW_TIMER14   (0xE<<4)
#define BW_TIMER15   (0xF<<4)
#define BW_TIMER16   (0x10<<4)
#define BW_TIMER17   (0x11<<4)
#define BW_TIMER18   (0x12<<4)
#define BW_TIMER19   (0x13<<4)
#define BW_TIMER20   (0x14<<4)
#define BW_TIMER21   (0x15<<4)
#define BW_GLOBAL_EN  (0x1<<12)
#define BW_ADDR_TRIG (0x1<<15)

//MultTimer Sub Registers

#define BW_CONTROL_REG 0x8
#define BW_TIMER_ON  (0x1<<0)
#define BW_TIMER_OFF (0x0<<0)
#define BW_TIMER_NEG (0x1<<1)
#define BW_TIMER_POS (0x0<<1)
#define BW_INT_CLK   (0x1<<2)   
#define BW_EXT_CLK   (0x0<<2)
#define BW_CLK_DIV1  (0x0<<3)
#define BW_CLK_DIV2  (0x1<<3)
#define BW_CLK_DIV4  (0x2<<3)
#define BW_CLK_DIV8  (0x3<<3)
#define BW_DELAY_REG   0x9
#define BW_WIDTH_REG   0xA
#define BW_PERIOD_REG  0xB
#define BW_PRT_REG     0xC
#define BW_PRT1 (0x0<<0)
#define BW_PRT2 (0x0<<4)
#define BW_PRT3 (0x0<<8)
#define BW_PRT4 (0x0<<12)

/// Memory buffer size in bytes
#define BUFFER_SIZE 0x8 

///Address and Data Physical Memory Offset
#define AD_OFFSET 0x00400008

/// Represent the Bittware fpga card, which has been configured 
/// with a bitstream implementing radar timing.
/// The construtor initializes the card. 
class Bittware {
    public:
        /// Constructor
        Bittware();
        /// Destructor
        virtual ~Bittware();
        /// Configure the Bittware timer card
        void configure(unsigned long delay,
                       unsigned long width,
                       unsigned long period,
                       unsigned long prt);
        /// Start the timer
        void start();
        /// @return True if board is operational, false otherwise
        bool isok();
    protected:
        /// Read from Memory store into Buffer 
        ULONG *mem_read(ULONG rd_buffer[]);
        /// Write Buffer to Memory 
        void mem_write(ULONG wr_buffer[]);
        /// Copy Read Buffer to Write Buffer Routine
        void copy_buffer(ULONG wr_buffer[],
                         ULONG rd_buffer[]);
        /// Print Error Message Routine                  
        int check_error(int error);
        /// True if the board is operational, false otherwise.
        bool _isok;
        /// The processor
        PDSP21K _processor;
        /// The configuration
        DSP21K_DEVICE_CFG _cfg;
        /// The physical memory
        DSP21K_PHYS_MEMORY _physmem;

};

#endif /*BITTWARE_H_*/
