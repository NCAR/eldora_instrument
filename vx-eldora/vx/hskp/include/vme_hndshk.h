/*
 *	$Id$
 *
 *	Module:	vme_hndshk.h	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/10  19:35:36  craig
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
short polled;               /* Polled interrupt location set to 1 in
                                Mid-Beam interrupt by radar processor */
short radar_hndshk[27];     /* Radar Data handshake areas */
short nav_hndshk[2];        /* navigational data record handshake areas */
short ads_hndshk[2];        /* Airborne Data System (ADS) record
                               handshake areas */
};
