/*
 *	$Id$
 *
 *	Module:	cntrlDef.h	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1993/09/01  20:07:10  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  21:36:36  reif
 * Initial revision
 *
 *
 * description:  Definitions that go with the motion control interface
 *             
 *             
 */

#ifndef INCcntrlDefh
#define INCcntrlDefh

/*************** REGISTER DEFINITIONS ***************/

#define MOTOR_BASE              (0x5000 + SHORT_BASE)
#define FLAG_REGISTER           (0x00 + MOTOR_BASE)
#define PROGRAM_COUNTER         (0x05 + MOTOR_BASE)
#define STATUS_REGISTER         (0x07 + MOTOR_BASE)
#define EIGHT_BIT_MOTOR_COMMAND (0x08 + MOTOR_BASE)
#define PWM_MOTOR_COMMAND       (0x09 + MOTOR_BASE)
#define COMMAND_POSITION_MSB    (0x0C + MOTOR_BASE)
#define COMMAND_POSITION_MID    (0x0D + MOTOR_BASE)
#define COMMAND_POSITION_LSB    (0x0E + MOTOR_BASE)
#define SAMPLE_TIMER            (0x0F + MOTOR_BASE)
#define READ_POSITION_MSB       (0x12 + MOTOR_BASE)
#define READ_POSITION_MID       (0x13 + MOTOR_BASE)
#define READ_POSITION_LSB       (0x14 + MOTOR_BASE)
#define PRESET_POSITION_MSB     (0x15 + MOTOR_BASE)
#define PRESET_POSITION_MID     (0x16 + MOTOR_BASE)
#define PRESET_POSITION_LSB     (0x17 + MOTOR_BASE)
#define COMMUTATOR_RING         (0X18 + MOTOR_BASE)
#define COMMUTATOR_VEL_TIMER    (0X19 + MOTOR_BASE)
#define X                       (0X1A + MOTOR_BASE)
#define Y_PHASE_OVERLAP         (0X1B + MOTOR_BASE)
#define COMMUTATOR_OFFSET       (0X1C + MOTOR_BASE)
#define MAX_PHASE_ADVANCE       (0X1F + MOTOR_BASE)
#define FILTER_ZERO_A           (0X20 + MOTOR_BASE)
#define FILTER_POLE_B           (0X21 + MOTOR_BASE)
#define GAIN_K                  (0X22 + MOTOR_BASE)
#define CMND_VEL_PROP_LSB       (0X23 + MOTOR_BASE)
#define CMND_VEL_PROP_MSB       (0X24 + MOTOR_BASE)
#define ACCELERATION_LSB        (0X26 + MOTOR_BASE)
#define ACCELERATION_MSB        (0X27 + MOTOR_BASE)
#define MAXIMUM_VELOCITY        (0X28 + MOTOR_BASE)
#define FINAL_POSITION_LSB      (0X29 + MOTOR_BASE)
#define FINAL_POSITION_MID      (0X2A + MOTOR_BASE)
#define FINAL_POSITION_MSB      (0X2B + MOTOR_BASE)
#define ACTUAL_VELOCITY_LSB     (0X34 + MOTOR_BASE)
#define ACTUAL_VELOCITY_MSB     (0X35 + MOTOR_BASE)
#define COMMAND_VELOCITY        (0X3C + MOTOR_BASE)
#define STATUS_1100             (0x40 + MOTOR_BASE)
#define RESET_1100              (0X80 + MOTOR_BASE)
#define CONTROL_LATCH           (0XC0 + MOTOR_BASE)
#define READ_2016               (0X100 + MOTOR_BASE)
#define SELECT_ABSOLUTE_UPPER   (0X140 + MOTOR_BASE)
#define SELECT_ABSOLUTE_LOWER   (0X180 + MOTOR_BASE)
#define COUNTS                  65536.0
#define MAX_SAMPLE              0xCB
#define PERIOD                  0.001000 /* In seconds */
#define GAIN                    10.0 /* In v/rpm */

#define INTEGRATOR_GAIN         0.01   /* Gain of motion control integrator */
#define DELTA_DEGPERSEC        1.0   /* Integrator allowable error */
#endif /* INC */






