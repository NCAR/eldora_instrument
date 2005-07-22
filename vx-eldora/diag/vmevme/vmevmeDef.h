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
 *        
 */


/* Definitions to work with the VME to VME interfaces for diagnostics only!*/

#define FIRST_RADAR_OFFSET   0x100
#define RADAR_SIZE_INCR      0x8000
#define NUM_RADAR_HNDSHK     27

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












