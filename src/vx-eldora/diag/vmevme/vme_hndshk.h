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
 * Revision 1.3  1993/09/01  20:07:41  craig
 * *** empty log message ***
 *
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
short start_hndshk;         /* Handshake when radar processors are ready */
short dc_remove_hndshk;     /* handshake to keep dc removal in sync */
short tpulse_flg;           /* Test pulse handshaking flag */
float tpulse_level;         /* Test pulse measured (by power meter)
			       level in dbm */
float tpulse_level_proc;    /* Test pulse level in db (from radar
			       processors) */
long tpulse_dist;           /* Distance to Test pulse in meters */
long tpulse_width;          /* Width of Test pulse in meters */
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
