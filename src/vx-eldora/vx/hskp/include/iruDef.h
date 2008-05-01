/*
 *	$Id$
 *
 *	Module:	iruDef.h	 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.4  1997/08/26  21:25:25  craig
 * *** empty log message ***
 *
 *
 * description: These are the definitions used to handle the Inertial Reference
 *              Unit (IRU) interface.  This interface was designed by VMIC
 *              and is part number VMIVME6005 it receives data from an
 *              ARINC-429 serial bus.  This code is specific to the
 *              Honeywell LaserRef SM IRU.
 */

#ifndef INCiruDefh
#define INCiruDefh

/*****************  SET ADDRESSES IN THE ARINC-429 Dual Port RAM  ************/

#define IRU_BASE       0x700000   /* BASE ADDRESS OF VMIVME6005 board */

struct iru_rcontrol{

  /* General purpose control registers */

  short board_id;          /* Board identification (0x0000) */
  short rev_level;         /* Software revision level (0x0002) */
  short command;           /* Command code (0x0004) */
  short response;          /* Response Code (0x0006) */
  char rxid;               /* Receiver ID register (0x0008) */
  char txid;               /* Transmitter ID register (0x0009) */
  char resp_irq;           /* Slave response interrupt control (0x000A) */
  char resp_vec;           /* Slave Response interrupt vector (0x000B) */
  char rx_mode;            /* Receiver mode Code (0x000C); */
  char tx_bfrst;           /* Transmitter Buffers status (0x000D) */
  short rx_mask;           /* Receiver enable status (0x000E) */

  /* VMEBus interrupt control registers */

  char rxa0_irq;           /* Interrupt control for receiver rxa0 (0x0010) */
  char rxa0_vec;           /* Interrupt vector for receiver rxa0 (0x0011) */
  char rxa1_irq;
  char rxa1_vec;
  char rxa2_irq;
  char rxa2_vec;
  char rxa3_irq;
  char rxa3_vec;
  char rxa4_irq;
  char rxa4_vec;
  char rxa5_irq;
  char rxa5_vec;
  char rxa6_irq;
  char rxa6_vec;
  char rxa7_irq;
  char rxa7_vec;

  char rxb0_irq;          /* Interrupt control for receiver rxb0 (0x0020) */
  char rxb0_vec;          /* Interrupt vector for receiver rxb0 (0x0021) */
  char rxb1_irq;
  char rxb1_vec;
  char rxb2_irq;
  char rxb2_vec;
  char rxb3_irq;
  char rxb3_vec;
  char rxb4_irq;
  char rxb4_vec;
  char rxb5_irq;
  char rxb5_vec;
  char rxb6_irq;
  char rxb6_vec;
  char rxb7_irq;
  char rxb7_vec;

  char tx0_irq;          /* Interrupt control for transmitter tx0 (0x0030) */ 
  char tx0_vec;          /* Interrupt vector for transmitter tx0 (0x0031) */ 
  char tx1_irq;
  char tx1_vec;
  char tx2_irq;
  char tx2_vec;
  char tx3_irq;
  char tx3_vec;
  char tx4_irq;
  char tx4_vec;
  char tx5_irq;
  char tx5_vec;
  char tx6_irq;
  char tx6_vec;
  char tx7_irq;
  char tx7_vec;

  /* Buffer pointers */

  short rxa0_vmeptr;   /* Buffer offset address for receiver rxa0 (0x0040) */
  short rxa1_vmeptr;
  short rxa2_vmeptr;
  short rxa3_vmeptr;
  short rxa4_vmeptr;
  short rxa5_vmeptr;
  short rxa6_vmeptr;
  short rxa7_vmeptr;

  short rxb0_vmeptr;   /* Buffer offset address for receiver rxb0 (0x0050) */
  short rxb1_vmeptr;
  short rxb2_vmeptr;
  short rxb3_vmeptr;
  short rxb4_vmeptr;
  short rxb5_vmeptr;
  short rxb6_vmeptr;
  short rxb7_vmeptr;

  short tx0_vmeptr;   /* Buffer offset address for transmitter tx0 (0x0060)*/
  short tx1_vmeptr;
  short tx2_vmeptr;
  short tx3_vmeptr;
  short tx4_vmeptr;
  short tx5_vmeptr;
  short tx6_vmeptr;
  short tx7_vmeptr;

  /* Receiver word count limit variables */

  short rxa0_limit;  /* Buffer interrupt limit for receiver rxa0 (0x0070) */
  short rxa1_limit;
  short rxa2_limit;
  short rxa3_limit;
  short rxa4_limit;
  short rxa5_limit;
  short rxa6_limit;
  short rxa7_limit;

  short rxb0_limit;  /* Buffer interrupt limit for receiver rxb0 (0x0080) */
  short rxb1_limit;
  short rxb2_limit;
  short rxb3_limit;
  short rxb4_limit;
  short rxb5_limit;
  short rxb6_limit;
  short rxb7_limit;

  /* Receiver Buffer activity flags */

  char rxa0_ready;  /* Buffer ready flag for receiver rxa0 (0x0090) */
  char rxa1_ready;
  char rxa2_ready;
  char rxa3_ready;
  char rxa4_ready;
  char rxa5_ready;
  char rxa6_ready;
  char rxa7_ready;

  char rxb0_ready;    /* Buffer ready flag for receiver rxb0 (0x0098) */
  char rxb1_ready;
  char rxb2_ready;
  char rxb3_ready;
  char rxb4_ready;
  char rxb5_ready;
  char rxb6_ready;
  char rxb7_ready;

  /* Transmitter buffer activity flags */

  char tx0_active;  /* Transmitter tx0 active flag (0x00A0) */
  char tx1_active;
  char tx2_active;
  char tx3_active;
  char tx4_active;
  char tx5_active;
  char tx6_active;
  char tx7_active;

  /* Sorting mode type variable */
  char sort_type;   /* sorting mode type-SDI and label or just label (0x00A8)*/
  char unused[7];

  /* Receiver comparison codes */
  short rxa0_compare; /* Comparison code for receiver rxa0 (0x00B0) */
  short rxa1_compare; 
  short rxa2_compare; 
  short rxa3_compare; 
  short rxa4_compare; 
  short rxa5_compare; 
  short rxa6_compare; 
  short rxa7_compare;
  
};

/* User interface commands */

#define NOOP         0x0000
#define REQ_TX_BFR   0x0001
#define XMIT_DATA    0x0002
#define PIPELINE     0x0003
#define SET_RX_MASK  0x0004
#define SET_RX_LIMIT 0x0005
#define LOOPBACK     0x0006
#define NORMALIO     0x0007
#define SELFTEST     0x0008
#define SET_RX_MODE  0x0009
#define INITIALIZE   0x000A


/* VMIVME6005 Response */

#define COMMAND_ACK  0x0001
#define REQ_DENIED   0x0002
#define TX_BFR_RDY   0x0003
#define TX_ENABLED   0x0004
#define PIPE_ACK     0x0005

#define SPURIOUS        24
#define UNINITIALIZED   15

/* Define the ARINC 429 labels for each of the variables to be recorded */
 #define LATITUDE_LAB       0310   /* 0xC8 */
 #define LONGITUDE_LAB      0311   /* 0xC9 */
 #define WIND_SPEED_LAB     0315   /* 0xCD */
 #define WIND_DIRECTION_LAB 0316   /* 0xCE */
 #define NS_VELOCITY_LAB    0366   /* 0xF6 */
 #define EW_VELOCITY_LAB    0367   /* 0xF7 */
 #define HEADING_LAB        0314   /* 0xCC */
 #define DRIFT_LAB          0321   /* 0xD1 */
 #define ALTITUDE_LAB       0361   /* 0xF1 */
 #define INERTIAL_VSPEED_LAB  0365   /* 0xF5 */
 #define PITCH_LAB          0324   /* 0xD4 */
 #define ROLL_LAB           0325   /* 0xD5 */
 #define INTEG_VERT_ACC_LAB  0265   /* 0xB5 */
 #define VERT_ACC_LAB       0364   /* 0xF4 */
 #define TRACK_RATE_LAB     0335   /* 0xDD */
 #define PITCH_RATE_LAB     0336   /* 0xDE */

/* Define offsets into the new IRU data test arrays */

 #define IRU_LATITUDE           0
 #define IRU_LONGITUDE          1
 #define IRU_WIND_SPEED         2
 #define IRU_WIND_DIRECTION     3
 #define IRU_NS_VELOCITY        4
 #define IRU_EW_VELOCITY        5
 #define IRU_HEADING            6
 #define IRU_DRIFT              7
 #define IRU_ALTITUDE           8
 #define IRU_INERTIAL_VSPEED    9
 #define IRU_PITCH              10
 #define IRU_ROLL               11
 #define IRU_INTEG_VERT_ACC     12
 #define IRU_VERT_ACC           13
 #define IRU_TRACK_RATE         14
 #define IRU_PITCH_RATE         15

#endif /* INC */
