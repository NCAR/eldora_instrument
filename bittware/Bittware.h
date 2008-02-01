#ifndef BITTWARE_H_
#define BITTWARE_H_

#include "dsp21k.h"

// Bittware Timer Registers

//Address Register
#define TIMER1    (0x01<<4)
#define TIMER2    (0x02<<4)
#define TIMER3    (0x03<<4)
#define TIMER4    (0x04<<4)
#define TIMER5    (0x05<<4)
#define TIMER6    (0x06<<4)
#define TIMER7    (0x07<<4)
#define TIMER8    (0x08<<4)
#define TIMER9    (0x09<<4)
#define TIMER10   (0x0A<<4)
#define TIMER11   (0xB<<4)
#define TIMER12   (0xC<<4)
#define TIMER13   (0xD<<4)
#define TIMER14   (0xE<<4)
#define TIMER15   (0xF<<4)
#define TIMER16   (0x10<<4)
#define TIMER17   (0x11<<4)
#define TIMER18   (0x12<<4)
#define TIMER19   (0x13<<4)
#define TIMER20   (0x14<<4)
#define TIMER21   (0x15<<4)
#define GLOBAL_EN  (0x1<<12)
#define ADDR_TRIG (0x1<<15)

//MultTimer Sub Registers

#define CONTROL_REG 0x8
#define TIMER_ON  (0x1<<0)
#define TIMER_OFF (0x0<<0)
#define TIMER_NEG (0x1<<1)
#define TIMER_POS (0x0<<1)
#define INT_CLK   (0x1<<2)   
#define EXT_CLK   (0x0<<2)
#define CLK_DIV1  (0x0<<3)
#define CLK_DIV2  (0x1<<3)
#define CLK_DIV4  (0x2<<3)
#define CLK_DIV8  (0x3<<3)
#define DELAY_REG   0x9
#define WIDTH_REG   0xA
#define PERIOD_REG  0xB
#define PRT_REG     0xC
#define PRT1 (0x0<<0)
#define PRT2 (0x0<<4)
#define PRT3 (0x0<<8)
#define PRT4 (0x0<<12)

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
