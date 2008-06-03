#ifndef BITTWARE_H_
#define BITTWARE_H_

#include "EldoraRadarParams.h"
#include "dsp21k.h"

#include <math.h>

// Bittware Timer Registers

//Status Register
#define BW_TIMER_DCMLOCK (0x01<<10)

//Address Register
#define BW_WRITE     (0x01<<3)
#define BW_READ      (0x00<<3)
#define TX_FORE_CH1  (0x00<<4)
#define TX_FORE_CH2  (0x01<<4)
#define TX_FORE_CH3  (0x02<<4)
#define TX_FORE_CH4  (0x03<<4)
#define TX_FORE_PK   (0x04<<4)
#define TX_FORE_TP   (0x05<<4)
#define TX_AFT_CH1   (0x06<<4)
#define TX_AFT_CH2   (0x07<<4)
#define TX_AFT_CH3   (0x08<<4)
#define TX_AFT_CH4   (0x09<<4)
#define TX_AFT_PK    (0x0A<<4)
#define TX_AFT_TP    (0x0B<<4)
#define RX_FORE_CH1  (0x0C<<4)
#define RX_FORE_CH2  (0x0D<<4)
#define RX_FORE_CH3  (0x0E<<4)
#define RX_FORE_CH4  (0x0F<<4)
#define RX_AFT_CH1   (0x10<<4)
#define RX_AFT_CH2   (0x11<<4)
#define RX_AFT_CH3   (0x12<<4)
#define RX_AFT_CH4   (0x13<<4)
#define MIDBEAM_INTR (0x14<<4)

#define BW_GLOBAL_EN (0x1<<12)
#define BW_TIMER_RST (0x01<<13)
#define BW_ADDR_TRIG (0x1<<15)

//MultTimer Sub Registers

#define BW_CONTROL_REG 0x0
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
#define BW_DELAY_REG   0x1
#define BW_WIDTH_REG   0x2
#define BW_PERIOD_REG  0x3
#define BW_PRT_REG     0x4
#define BW_PRT1 (0x0<<0)
#define BW_PRT2 (0x0<<4)
#define BW_PRT3 (0x0<<8)
#define BW_PRT4 (0x0<<12)

/// Memory buffer size in bytes
#define BUFFER_SIZE 0xC 

///Address and Data Physical Memory Offset
#define AD_OFFSET 0x00400004

/// Represent the Bittware fpga card, which has been configured 
/// with a bitstream implementing radar timing.
/// The construtor initializes the card. 
class Bittware {
    public:
        /// Constructor
        /// @param devNum The card number
        Bittware(int devNum);
        /// Destructor
        virtual ~Bittware();
        /// Configure the Bittware timer card
        /// @param The radar operating parameters.
       void configure(EldoraRadarParams radarParams);
        /// Start the timer
        void start();
        /// Stop the timer
        void shutdown();        
        /// @return True if board is operational, false otherwise
        bool isok();
    protected:
        /// Read from Memory store into Buffer 
        U32 *mem_read(U32 rd_buffer[]);
        /// Write Buffer to Memory 
        void mem_write(U32 wr_buffer[]);
        /// Copy Read Buffer to Write Buffer Routine
        void copy_buffer(U32 wr_buffer[],
                         U32 rd_buffer[]);
        /// Print Error Message Routine                  
        int check_error(int error);
        /// The card number
        int _devNum;
        /// True if the board is operational, false otherwise.
        bool _isok;
        /// The number of gates.
        unsigned int _gates;       
        /// The prf in hz
        int _prf;        
        /// The pulsewidth in ns.
        int _pulsewidth;
        /// The number of samples.
        unsigned int _samples;
        /// Dual prt true or false.
        bool _dualPrt;
        /// The processor
        PDSP21K _processor;
        /// The configuration
        DSP21K_DEVICE_CFG _cfg;
        /// The physical memory
        DSP21K_PHYS_MEMORY _physmem;

};

#endif /*BITTWARE_H_*/
