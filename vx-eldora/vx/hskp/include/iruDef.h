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
 * Revision 1.1  1992/09/04  19:49:08  craig
 * Initial revision
 *
 *
 * description: These are the definitions used to handle the Inertial Reference
 *              Unit (IRU) interface.  This interface was designed at NCAR to
 *              receive data from an ARINC-429 serial bus.  This code is
 *              specific to the Honeywell LaserRef SM IRU.
 */

#ifndef INCiruDefh
#define INCiruDefh

/*****************  SET ADDRESSES IN THE ARINC-429 Dual Port RAM  ************/

#define IRU_BASE       0x800000   /* BASE ADDRESS OF ARINC-429 Dual port Ram */
#define IRU_TXA        0x0000    /* Offset to transmit control structure A */
#define IRU_TXB        0x0322    /* Offset to transmit control structure A */
#define IRU_RXA1       0x0644    /* Offset to receive control structure A1 */
#define IRU_RXA2       0x0FAE    /* Offset to receive control structure A2 */
#define IRU_RXB1       0x12D8    /* Offset to receive control structure B1 */
#define IRU_RXB2       0x1C42    /* Offset to receive control structure B2 */
#define IRU_BIM        0x2000    /* Offset to the ARINC-429 BIM */
#define IRU_HI8282A    0x1F6C    /* Offset to the HI8282 control
				    register port A */

struct iru_rcontrol{
    short max_words;
    short words_buf1;
    short words_buf2;
    unsigned char int_flag;
    unsigned char term_label;
    unsigned char num_interrupts;
    unsigned char spare;
    long buf1[300];
    long buf2[300];
};

/**** define a structure to hold one of each iru variable ****/

struct single_iru{
    short seconds;
    short msec_longitude;
    short status;
    float latitude;
    float longitude;
    float wind_speed;
    float wind_direction;
    float ns_velocity;
    float ew_velocity;
    float heading;
    float drift;
    float altitude;
    float inertial_vspeed;
    float pitch;
    float roll;
    float integ_vert_acc;
    float vert_acc;
    float track_rate;
    float pitch_rate;
};

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
 #define ROLL_LAB           0327   /* 0xD7 */
 #define INTEG_VERT_ACC_LAB  0265   /* 0xB5 */
 #define VERT_ACC_LAB       0364   /* 0xF4 */
 #define TRACK_RATE_LAB     0335   /* 0xDD */
 #define PITCH_RATE_LAB     0336   /* 0xDE */

#endif /* INC */





