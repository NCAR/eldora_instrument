/**********************************************************************************************

 Desc: ChannelAdapter DDC Registers

**********************************************************************************************/

//PulseWidths for Decimation
  #define _0_25us 2
  #define _0_50us 4
  #define _0_75us 6
  #define _1_00us 8
  #define _1_25us 10
  #define _1_50us 12
  #define _1_75us 14
  #define _2_00us 16


//Kaiser Filter Registers
  #define KAISER_ADDR 0xA00
  #define KAISER_DATA 0xA04
  #define KAISER_READ 0xA08
  #define KAISER_WR   0xA0C
//Values
  #define DDC_START 0x0


//Gaussian Filter Registers
  #define GAUSSIAN_ADDR 0xA10
  #define GAUSSIAN_DATA 0xA14
  #define GAUSSIAN_READ 0xA18
  #define GAUSSIAN_WR   0xA1C

//Clock Decimation Registers
  #define DEC_REG     0xA20
  #define DEC_RST_REG 0xA24
  #define RST_ACT     0x1
  #define RST_CLR     0x0

//Pulse Pair Processor Registers
  #define M_REG        0xA40
  #define N_REG        0xA44
  #define IQ_START_IDX 0xA48
  #define IQ_GATE_LEN  0xA4C
  #define PP_RST       0xA54
//Values
  #define PP_RST_ACT   0x1
  #define PP_RST_CLR   0x0

//Gate Splitter Register
  #define DPRT_REG 0xA50
//Values
  #define DPRT_ON  0x1
  #define DPRT_OFF 0x0

//Timing Select Register
  #define TIMING_SEL 0xA58
//Values
  #define INTERNAL_TIMING 0x1
  #define EXTERNAL_TIMING 0x0

//MultiTimer Registers
  #define MT_ADDR 0xA30
  #define MT_DATA 0xA34
  #define MT_WR   0xA38

//MultTimer Sub Registers
  #define WRITE_ON  0x3
  #define WRITE_OFF 0x0

  #define CONTROL_REG 0x8
  #define DELAY_REG   0x9
  #define WIDTH_REG   0xA
  #define PERIOD_REG  0xB
  #define PRT_REG     0xC

  #define TIMER0 (0x01<<4)
  #define TIMER1 (0x02<<4)
  #define TIMER2 (0x04<<4)
  #define TIMER3 (0x08<<4)

  #define F_Ch0 (0x01<<4)
  #define F_Ch1 (0x02<<4)
  #define F_Ch2 (0x04<<4)
  #define F_Ch3 (0x08<<4)
  #define A_Ch0 (0x10<<4)
  #define A_Ch1 (0x20<<4)
  #define A_Ch2 (0x40<<4)
  #define A_Ch3 (0x80<<4)

  #define TIMER_EN  (0x1<<12)
  #define ADDR_TRIG (0x1<<15)

  #define TIMER_ON  (0x1<<0)
  #define TIMER_OFF (0x0<<0)
  #define TIMER_NEG (0x1<<1)
  #define TIMER_POS (0x0<<1)
  #define CLK_DIV1  (0x0<<2)
  #define CLK_DIV2  (0x1<<2)
  #define CLK_DIV4  (0x2<<2)
  #define CLK_DIV8  (0x3<<2)

  #define PRT1 (0x0<<0)
  #define PRT2 (0x0<<4)
  #define PRT3 (0x0<<8)
  #define PRT4 (0x0<<12)



//-------------------------------
//Added by Tom 1-3-08 for timing error detection 
//Bits 9-6 of status register corresponde to sync error for channel A-D
#define A_SYNC_ERROR (0x1<<9)
#define B_SYNC_ERROR (0x1<<8)
#define C_SYNC_ERROR (0x1<<7)
#define D_SYNC_ERROR (0x1<<6)
