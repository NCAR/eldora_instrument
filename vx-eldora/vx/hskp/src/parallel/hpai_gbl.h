/*    Module:     hpai_gbl.h   
      Author:     Craig Walther   
      Copyright:  National Center For Atmospheric Research   
      Date:       October 9, 1991
      Purpose:    This include module contains all of the global  

                  definitions that are used with the high power   
                  amplifier's controller software   
*/   
/* Define some general purpose definitions */   
   
#define  TRUE     1   
#define  FALSE    0
#define  OPEN     0
#define  CLOSED   1
#define  DUMMYLOAD 1
#define  ANTENNA   0   
#define  ERROR    0
#define  NO_ERROR 1
   
#define  MD_REMOTE   1      /* Commands are taken from remote console only */   
#define  MD_LOCAL    0      /* Commands are taken from front panel switches */  

   
#define  INITIAL  0      /* HPA controller is initializing */   
#define  HPA_OFF  1      /* HPA is off, but ready to run */   
#define  STBY_WT  2      /* Mode standby will wait for an operate command */   
#define  STBY_AT  3      /* Mode standby but automatically goes to operate */   
#define  OPERATE  4      /* HPA is operating */   
   
#define  MSEC_PER_TICK  50      /* Milliseconds per timer tick */ 


  
#define MC68681_VECTOR  68  /* Interrupt vector number for the MC68681 */   
#define PRINT_MAX        1  /* Number of Seconds between Prints */
 
/* Masks for relay output card #0 */   
   
#define  MAIN_P_RLY     0x0001     /* Main Power Relay */ 
#define  BLOWER_P_RLY   0x0002     /* TWT Cooling motor power */  
#define  BLOWER_SPD_RLY 0x0004     /* TWT Blower high/low speed */ 
#define  AUX_P_RLY      0x0008     /* Auxilliary Power Supply */ 
#define  COLL_SS_P_RLY  0x0020     /* Collector Step Start Power */ 
#define  COLL_M_P_RLY   0x0040     /* Main Collector High voltage power supply */ 
#define  BODY_SS_P_RLY  0x0080     /* Body Step Start Power supply */ 
#define  BODY_M_P_RLY   0x0100     /* Main Body High voltage Power supply */   
#define  ALL_RELAYS_0   0x01EF     /* All relays on card #0 */   
 
/* Masks for relay output card #1 */   
 
#define  STBY_OPER_RLY  0x0001     /* Standby/Operate Relay */ 
#define  PULSE_EN_RLY   0x0002     /* Input Pulse Enable Relay */ 
#define  RF_PIN_SW_RLY  0x0004     /* RF PIN switch relay */   
#define  FAULT_RESET    0x0008     /* Systems Interface Fault Reset */   
#define  DMY_LOAD_RLY   0x0080     /* Dummy load switch control relay */ 
#define  HOT_RESIST_RLY 0x0100     /* Heated resistor in input air stream */   
#define  ALL_RELAYS_1   0x018F     /* All relays on card #1 */   
   
/* Masks for the lamp drivers */   
   
#define  ANTENNA_LMP    0x80     /* Antenna switch lamp */   
#define  LOCAL_LMP      0x40     /* Local/Remote switch lamp */   
#define  FAULT_CL_LMP   0x20     /* Fault Clear Switch lamp */   
#define  DUMMY_LD_LMP   0x10     /* Dummy load/antenna switch lamp */   
#define  OPERATE_LMP    0x08     /* Operate Switch lamp */   
#define  STANDBY_LMP    0x04     /* Standby switch lamp */   
#define  HPA_OFF_LMP    0x02     /* HPA on/off switch lamp */   
#define  ALL_LAMPS      0xFE     /* All Lamps */   
#define  LAMPS          0x44     /* Port number "D" for the lamp outputs */   

/* Masks for the LED drivers */   

#define  RADIATE_LED    0x80     /* Radiate Indicator LED */   
#define  FAULT_LED      0x40     /* Unlatched Fault Indicator LED */   
#define  BUSY_LED       0x20     /* HPAI Busy Indicator LED */   
#define  ALL_LEDS       0xE0     /* All LED's */   
#define  LEDS           0x45     /* Port number "E" for the LED outputs */   
   
/* Masks for the front panel switch inputs */   
   
#define  FAULT_CL_SW   0x80     /* Latched Fault Clear Switch */  
#define  DUMMY_LD_SW   0x40     /* Dummy load switch */   
#define  HPA_OFF_SW    0x20     /* HPA on/off switch lamp */   
#define  LOCAL_SW      0x10     /* Local/Remote switch */   
#define  STANDBY_SW    0x08     /* Standby Switch */   
#define  ANTENNA_SW    0x04     /* Antenna switch */   
#define  OPERATE_SW    0x02     /* Operate Switch */   
#define  SWITCHES      0x46     /* Port number "F" for the switch inputs */   
   
/* Masks for the yes/no inputs */   
 
/* Port NO_YES_0 => "A" */ 
   
#define  WVGD_ARC       0x01     /* Waveguide arc */   
#define  TWT_OVER_TMP   0x02     /* TWT over temperature */   
#define  BODY_OVER_I    0x08     /* Body over current */   
#define  PRF_RCVD       0x10     /* PRF Received */   
#define  INTERLOCK      0x20     /* Interlock open */   
#define  COLLECT_P      0x40     /* Collector Power */ 
#define  BODY_P         0x80     /* Body Power */ 
#define  NO_YES_0       0x41     /* First No/Yes input port number "A" */   
 
/* Port NO_YES_1 => "B" */ 
 
#define  AUX_P          0x80     /* Auxillary power relay open/close */ 
#define  BLOWER_P       0x40     /* Cooling blower power relay open/close */ 
#define  MAIN_P         0x20     /* Main Power relay open/close */ 
#define  SYS_INTER      0x10     /* Systems Interface is intervening */ 
#define  PRF_LIMIT      0x08     /* HPA is PRF Limiting */ 
#define  SAFETY_CONN    0x04     /* Safety Connector open/close */
#define  PW_LIMIT       0x02     /* HPA is pulse width limiting */
#define  REFLECT_P      0x01     /* High reflected power has been detected */ 
#define  NO_YES_1       0x42     /* Second No/Yes input port number "B" */   
 
/* Port NO_YES_2 => "C" */ 
 
#define  ION_PMP_NY     0x80     /* Ion Pump over current detected */ 
#define  CATHODE_I      0x40     /* Cathode average current over maximum */ 
#define  WG_STATE       0x20     /* State of the waveguide switch */ 
#define  WG_TRANS       0x10     /* Waveguide switch is in transition */ 
#define  THREE_PHS      0x08     /* Three Phase input power monitor fault */
#define  CROWBAR        0x04     /* A crowbar has occured fault */
#define  LOW_WVGD_PRESS 0x02     /* Low wave guide pressure */   
#define  NO_YES_2       0x43     /* Third No/Yes input port number "C" */   
   
/* Definitions for the optical input cards */   
   
#define  NUM_OI         6       /* Total Number of optical Input channels */ 
#define  TWT_HEAT_V   0x0       /* T.W.T. Heater Voltage */   
#define  TWT_HEAT_I   0x1       /* T.W.T. Heater Current */   
#define  TWT_GRID_V   0x2       /* T.W.T. Grid Voltage */   
#define  TWT_PULSEUP  0x3       /* T.W.T. Pulse-up Voltage */   
#define  TWT_CATH_I   0x4       /* T.W.T. Cathode Current */   
#define  TWT_COLL_V   0x5       /* T.W.T. Collector Voltage */   
  
/* Definitions for the A/D channels */  
 
#define  NUM_AD      16     /* Number of A/D Input Channels */  
#define  BODY_V       0     /* TWT Body Voltage */  
#define  PT_SCRN_V    1     /* Pass Tube screen voltage */  
#define  BODY_I       2     /* Body Current */  
#define  V_28         3     /* +28 volt supply */  
#define  V_15_1       4     /* +15 volt supply  # 1 */  
#define  V_5          5     /* +5 volt supply */  
#define  V_15_2       6     /* +15 volt supply  # 2 */  
#define  PT_GRID_V    9     /* Pass Tube Grid Voltage */ 
#define  ION_PMP_I   10     /* Ion Pump Current */ 
#define  OUTFLOW_T   11     /* TWT outflow temperature */  
#define  HEAT_R_T    12     /* Heated resistor temperature */  
#define  DMY_LOAD_T1 13     /* Dummy load temperature #1 */  
#define  DMY_LOAD_T2 14     /* Dummy load temperature #2 */
#define  PASS_TUBE_T 15     /* Pass Tube cavity Temperature */  
#define  GAIN_AD_1    0     /* Pattern For a Gain Of 1 */  
#define  GAIN_AD_10   1     /* Pattern For a Gain Of 10 */  
#define  GAIN_AD_100  3     /* Pattern For a Gain Of 100 */  
#define  VOLTS        0.0048828  /* Volts for 1 bit of the A/D converter */