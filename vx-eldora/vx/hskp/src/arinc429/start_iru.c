/*
 *	$Id$
 *
 *	Module:	start_iru	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/28  23:03:57  craig
 * Initial revision
 *
 * description: This module set variable concerned with the iru interface to
 *              the valuse they should be at after receiving a start command. 
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";


#define scope extern

#include "hskpAll.h"

void start_iru()
{
long i;

in_iru_isr = 0;
iru_isr_count = 0;
old_iru_interrupts = iru_rpntr->num_interrupts;


/***** Zero Indexes into all of the data arrays */

msec_longitude_indx = 0;
status_indx = 0;
latitude_indx = 0;
longitude_indx = 0;
wind_speed_indx = 0;
wind_direction_indx = 0;
ns_velocity_indx = 0;
ew_velocity_indx = 0;
heading_indx = 0;
drift_indx = 0;
altitude_indx = 0;
inertial_vspeed_indx = 0;
pitch_indx = 0;
roll_indx = 0;
integ_vert_acc_indx = 0;
vert_acc_indx = 0;
track_rate_indx = 0;
pitch_rate_indx = 0;

}


