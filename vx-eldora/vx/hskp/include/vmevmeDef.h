/*
 *	$Id$
 *
 *	Module:	vmevmeDef.h	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/19  19:35:36  craig
 * Initial revision
 *
 *        
 */

/* Define the handshake structure for using the Vme to Vme interface with
    the radar processors and the Housekeeping processor */

struct vmevmehndshk {
char salute[8];             /* Will contain the characters "HI RADAR" when
                               the housekeeper has finished initialization */
short tpulse_flg;           /* Test pulse handshaking flag */
float tpulse_level;         /* Test pulse measured level in dbm */
short tpulse_dist;          /* Distance to Test pulse in 15 MHz clock periods*/
short tpulse_width;         /* Width of Test pulse in 15 MHz clock periods */
float tpulse_freq;          /* Test Pulse transmitted frequency in GHz */
short tpulse_atten;         /* Test Pulse attennuator setting in db */
short tpulse_freq_num;      /* Test Pulse frequency Number (1 to 5) */
short nav_length;           /* Length of a navigational data record in Bytes */
short ads_length;           /* Length of a ADS data record in bytes */
char  dummy[2];             /* Dummy area to get other thing to line up */
short radar_hndshk[27];     /* Radar Data handshake areas */
short nav_hndshk[2];        /* navigational data record handshake areas */
short ads_hndshk[2];        /* Airborne Data System (ADS) record
                               handshake areas */
};

/* Definitions to work with the VME to VME interfaces */

#define FIRST_RADAR_OFFSET   0x100
#define RADAR_SIZE_INCR      0x8000
#define NUM_RADAR_HNDSHK     27

/* Stuff to handle recording of nav data */

struct nav_data
{
    struct nav_info s_nav_info;
    struct ins_data s_ins_data;
    struct gps_data s_gps_data;
    struct minirims_data s_minirims_data;
};

#define FIRST_NAV_OFFSET     0xD8100
#define SEC_NAV_OFFSET       0xF0100
#define MAX_NAV_SIZE         0x8000
#define IRU_ON               0x01
#define GPS_ON               0x02
#define MINIRIMS_ON          0x04
#define KALMAN_ON            0x00

/* Define stuff to help with recording of ADS data */

#define FIRST_ADS_OFFSET     0xE8100
#define SEC_ADS_OFFSET       0xE0100
#define MAX_ADS_SIZE         0x8000












