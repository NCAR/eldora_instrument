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
 * Revision 1.1  1992/09/04  19:49:12  craig
 * Initial revision
 *
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

scope struct iru_rcontrol *iru_rpntr;  /* Pointer to the iru receive
                                            control structure */
scope short *iru_hi8282a; /* Pointer to the HI8282 control register port a */

/***** data structure that contains the lastest and greatest IRU data ****/

scope struct single_iru last_iru_data;

/***** Global IRU flag saying the ISR has been entered *****/

scope short in_iru_isr;
scope long  iru_isr_count;   /* Counter to make sure enough iru interrupts are
                                being handled */
scope char old_iru_interrupts; /* Used to be able to poll for iru data */

/***** Indexes into all of the data arrays */

scope short msec_longitude_indx;
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
scope short integ_vert_acc_indx;
scope short vert_acc_indx;
scope short track_rate_indx;
scope short pitch_rate_indx;

#endif /* INC */



