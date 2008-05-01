/*    Module:     hpai_add.h
      Author:     Craig Walther
      Copyright:  National Center For Atmospheric Research
      Date:       August 7, 1991
      Purpose:    This include module contains all of the addresses
                  that are specific to the hardware that is used.  If
                  addresses of boards change, they can be changed
                  here without need to change the source code.
*/

/* Base addresses of all the cards used */

#define  BASE_OPTO_0   0xFF0000       /* Optical Input Card # 1 */
#define  BASE_OPTO_1   0xFF0100       /* Optical Input Card # 2 */
#define  BASE_RELAY_0  0xFF0200       /* Relay Output Card  #1 */
#define  BASE_RELAY_1  0xFF0500       /* Relay Output Card  #2 */
#define  BASE_PARA     0xFF0300       /* Parallel Input/Output Card */
#define  BASE_ATOD     0xFF0400       /* A/D Input Card */
#define UTILBASE       0xFFF000       /* ELDORA UTILITY CARD BASE ADDRESS */

/* Offsets for the Parallel Input/Output Card */

#define  PIO_IN_A     0x1       /* Parallel I/O Input Port A */
#define  PIO_IN_B     0x3       /* Parallel I/O Input Port B */
#define  PIO_IN_C     0x5       /* Parallel I/O Input Port C */
#define  PIO_OUT_D    0x7       /* Parallel I/O Output Port D */
#define  PIO_OUT_E    0x9       /* Parallel I/O Output Port E */
#define  PIO_IN_F     0xB       /* Parallel I/O Input Port F */
#define  PIO_CONTRL   0xD       /* Parallel I/O Control Register */

/* Offsets to the A/D converter registers */

#define  AD_DATA      0x0       /* A/D Converter data register */
#define  AD_STATUS_ID 0x3       /* A/D Converter Status/ID register */
#define  AD_CHAN_SEL  0x1       /* A/D Converter Channel Select register */
#define  AD_GAIN      0x2       /* A/D Converter gain Value register */
#define  AD_INTRPT    0x4       /* A/D Converter Interrupt register */

/* Addresses on the CPU card itself */

#define  MODE_A       0xFE0001   /* Debug Terminal Mode register A */
#define  STATUS_A     0xFE0003   /* Debug Terminal Status register A */
#define  CLOCK_SEL_A  0xFE0003   /* Debug Terminal Clock select register A */
#define  CMND_A       0xFE0005   /* Debug Terminal Command register A */
#define  RECV_A       0xFE0007   /* Debug Terminal Receive Buffer A */
#define  XMIT_A       0xFE0007   /* Debug Terminal transmit Buffer A */

#define  IPCR         0xFE0009   /* Input port change register (read only) */
#define  ACR          0xFE0009   /* Auxilliary Control register (write only) */
#define  ISR          0xFE000B   /* Interrupt status register (read only) */
#define  IMR          0xFE000B   /* Interrupt Mask register (write only) */
#define  CUR          0xFE000D   /* Current MSB of counter (read only) */
#define  CTUR         0xFE000D   /* Counter/timer Upper register (write only) */
#define  CLR          0xFE000F   /* Current LSB of counter (read only) */
#define  CTLR         0xFE000F   /* Counter/timer Lower register (write only) */

#define  MODE_B       0xFE0011   /* Debug Terminal Mode register A */
#define  STATUS_B     0xFE0013   /* Debug Terminal Status register A */
#define  CLOCK_SEL_B  0xFE0013   /* Debug Terminal Clock select register A */
#define  CMND_B       0xFE0015   /* Debug Terminal Command register A */
#define  RECV_B       0xFE0017   /* Debug Terminal Receive Buffer A */
#define  XMIT_B       0xFE0017   /* Debug Terminal transmit Buffer A */

#define  IVR          0xFE0019   /* Interrupt vector register (read/write) */
#define  INPT_PORT    0xFE001B   /* Input Port (read only) */
#define  OPCR         0xFE001B   /* Output port config. register (write only) */
#define  STRT_CNTR    0xFE001D   /* Start Counter Command (read only) */
#define  OPR_D        0xFE001D   /* Bit Set Output port register (write only) */
#define  STOP_CNTR    0xFE001F   /* Stop Counter Command (read only) */
#define  OPR_F        0xFE001F   /* Bit Set Output port register (write only) */

/* Define offsets to the non-volitile RAM on utility card */

#define UTILNVRAM   0x000 /* OFFSET TO NVRAM (read-write) */
#define UTILNVXFR   0x200 /* OFFSET TO NVRAM TRANSFER (read-write) */
#define UTILTOD     0x240 /* OFFSET TO TOD CHIP REGISTER BLOCK (read-write) */
#define UTILADCDAC  0x280 /* OFFSET TO ADC/DAC CHIP (read-write) */
#define UTILSTATLED 0x2C0 /* OFFSET TO STATUS REGISTER/LED BANK (read-write) */

/* OFFSETS WITHIN TOD CHIP */

#define TODMAINSTAT 0x00  /* OFFSET TO MAIN STATUS REGISTER */

#define TODREALTIME 0x01  /* OFFSET TO REAL TIME MODE REGISTER */
#define TODOUTPUT   0x02  /* OFFSET TO OUTPUT MODE REGISTER */
#define TODINTRCTL0 0x03  /* OFFSET TO INTERRUPT CONTROL REGISTER 0 */
#define TODINTRCTL1 0x04  /* OFFSET TO INTERRUPT CONTROL REGISTER 1 */

#define TODTIM0CTL  0x01  /* OFFSET TO TIMER 0 CONTROL REGISTER */
#define TODTIM1CTL  0x02  /* OFFSET TO TIMER 1 CONTROL REGISTER */
#define TODPF       0x03  /* OFFSET TO PERIODIC FLAG REGISTER */
#define TODINTROUT  0x04  /* OFFSET TO INTERRUPT ROUTING REGISTER */

#define TOD100S     0x05  /* OFFSET TO 1/100 SECONDS COUNTER */
#define TODSECS     0x06  /* OFFSET TO SECONDS COUNTER */
#define TODMINS     0x07  /* OFFSET TO MINUTES COUNTER */
#define TODHRS      0x08  /* OFFSET TO HOURS COUNTER */
#define TODDOM      0x09  /* OFFSET TO DAY-OF-MONTH COUNTER */
#define TODMON      0x0A  /* OFFSET TO MONTHS COUNTER */
#define TODYRS      0x0B  /* OFFSET TO YEARS COUNTER */
#define TODJUL1     0x0C  /* OFFSET TO JULIAN DAY (UNITS) COUNTER */
#define TODJUL100   0x0D  /* OFFSET TO JULIAN DAY (HUNDREDS) COUNTER */
#define TODDOW      0x0E  /* OFFSET TO DAY-OF-WEEK COUNTER */

#define TODT0LSB    0x0F  /* OFFSET TO TIMER 0 LSB */
#define TODT0MSB    0x10  /* OFFSET TO TIMER 0 MSB */
#define TODT1LSB    0x11  /* OFFSET TO TIMER 1 LSB */
#define TODT1MSB    0x12  /* OFFSET TO TIMER 1 MSB */

#define TODSECSCOMP 0x13  /* OFFSET TO SECONDS COMPARE RAM */
#define TODMINSCOMP 0x14  /* OFFSET TO MINUTES COMPARE RAM */
#define TODHRSCOMP  0x15  /* OFFSET TO HOURS COMPARE RAM */
#define TODDOMCOMP  0x16  /* OFFSET TO DAY-OF-MONTH COMPARE RAM */
#define TODMONCOMP  0x17  /* OFFSET TO MONTHS COMPARE RAM */
#define TODDOWCOMP  0x18  /* OFFSET TO DAY-OF-WEEK COMPARE RAM */

#define TODSECSSAVE 0x19  /* OFFSET TO SECONDS SAVE RAM */
#define TODMINSSAVE 0x1A  /* OFFSET TO MINUTES SAVE RAM */
#define TODHRSSAVE  0x1B  /* OFFSET TO HOURS SAVE RAM */
#define TODDOMSAVE  0x1C  /* OFFSET TO DAY-OF-MONTH SAVE RAM */
#define TODMONSAVE  0x1D  /* OFFSET TO MONTHS SAVE RAM */

#define TODRAM      0x1E  /* OFFSET TO GENERIC RAM LOCATION */
#define TODRAMTEST  0x1F  /* OFFSET TO GENERIC RAM/TEST LOCATION */
