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
 *
 * description: These are the global variables used to handle the
 *              Inertial Reference Unit (IRU) interface.
 *              This interface consists of an ARINC-429 serial bus
 *              interface constructed by VMIC and is part number 
 *              VMIVME-6005.  This code is specific
 *              to the Honeywell LaserRef SM IRU
 */

#ifndef INCiruGblh
#define INCiruGblh

/**** Declare global variables associated with the ARINC-429 board  ****/

scope struct iru_rcontrol *iru_pntr;  /* Pointer to the iru
                                            control structure */

scope short rxa_buffer_offset;

/***** data structure that contains the lastest and greatest IRU data ****/

scope struct ins_data last_iru_data;

/***** Array used to test the data to see if it has changed ****/

scope long iru_new_data_test[16];

/* Pointers to each parameter on the VME bus */

scope long *latitude_pntr;
scope long *longitude_pntr;
scope long *wind_speed_pntr;
scope long *wind_direction_pntr;
scope long *ns_velocity_pntr;
scope long *ew_velocity_pntr;
scope long *heading_pntr;
scope long *drift_pntr;
scope long *altitude_pntr;
scope long *inertial_vspeed_pntr;
scope long *pitch_pntr;
scope long *roll_pntr;
scope long *integ_vert_acc_pntr;
scope long *vert_acc_pntr;
scope long *track_rate_pntr;
scope long *pitch_rate_pntr;

#endif /* INC */



