/*
 *	$Id$
 *
 *	Module:	iruGbl.h	 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/03  17:30:06  craig
 * Initial revision
 *
 * description: These are the global variables used to handle the
 *              Inertial Reference Unit (IRU) interface.
 *              This interface consists of an ARINC-429 serial bus
 *              interface constructed at NCAR.  This code is specific
 *              to the Honeywell LaserRef SM IRU
 */

#ifndef INCiruGblh
#define INCiruGblh

/**** Declare global variables associated with the ARINC-429 board  ****/

scope struct iru_control *iru_pntr;  /* Pointer to the iru control structure */

/***** data structure that contains the lastest and greatest IRU data ****/

scope struct single_iru last_iru_data;

/***** Global IRU flag saying the ISR has been entered *****/

scope short in_iru_isr;

/***** Indexes into all of the data arrays */

scope short msec_latitude_indx;
scope short status_indx;
scope short latitude_indx;
scope short longitude_indx;
scope short wind_speed_indx;
scope short wind_direction_indx;
scope short ns_velocity_indx;
scope short ew_velocity_indx;
scope short heading_indx;
scope short drift_indx;
scope short altitude_indx;
scope short inertial_vspeed_indx;
scope short pitch_indx;
scope short roll_indx;
scope short long_accel_indx;
scope short lateral_accel_indx;
scope short normal_accel_indx;
scope short track_rate_indx;
scope short pitch_rate_indx;

#endif /* INC */



