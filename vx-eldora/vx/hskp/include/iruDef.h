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
 * Revision 1.1  1992/09/03  19:33:44  craig
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

struct iru_control{
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
    short msec_latitude;
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
    float long_accel;
    float lateral_accel;
    float normal_accel;
    float track_rate;
    float pitch_rate;
};



#endif /* INC */



