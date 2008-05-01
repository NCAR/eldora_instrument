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
 *
 * description: This module set variable concerned with the iru interface to
 *              the values they should be at after receiving a start command. 
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";


#define scope extern

#include "hskpAll.h"

void start_iru()
{
int i;

/* calculate the pointers to the iru data */

calc_iru_pointers();

/* Clear out the new data test array */

for(i=0; i<16; i++)
    iru_new_data_test[i] = 0;

/* Fill the starting to parameter with the proper values */

last_iru_data.ins_data_id[0] = 'I';
last_iru_data.ins_data_id[1] = 'N';
last_iru_data.ins_data_id[2] = 'S';
last_iru_data.ins_data_id[3] = 'D';

last_iru_data.ins_data_len = sizeof(struct ins_data);


}

void calc_iru_pointers(void)
{
int i;

rxa_buffer_offset = iru_pntr->rxa0_vmeptr;

/***** Calculate the pointers to the data */

i = IRU_BASE + STANDARD_BASE + rxa_buffer_offset + LATITUDE_LAB*4;
latitude_pntr = (long *)i;
i = IRU_BASE + STANDARD_BASE + rxa_buffer_offset + LONGITUDE_LAB*4;
longitude_pntr = (long *)i;
i = IRU_BASE + STANDARD_BASE + rxa_buffer_offset + WIND_SPEED_LAB*4;
wind_speed_pntr = (long *)i;
i = IRU_BASE + STANDARD_BASE + rxa_buffer_offset + WIND_DIRECTION_LAB*4;
wind_direction_pntr = (long *)i;
i = IRU_BASE + STANDARD_BASE + rxa_buffer_offset + NS_VELOCITY_LAB*4;
ns_velocity_pntr = (long *)i;
i = IRU_BASE + STANDARD_BASE + rxa_buffer_offset + EW_VELOCITY_LAB*4;
ew_velocity_pntr = (long *)i;
i = IRU_BASE + STANDARD_BASE + rxa_buffer_offset + HEADING_LAB*4;
heading_pntr = (long *)i;
i = IRU_BASE + STANDARD_BASE + rxa_buffer_offset + DRIFT_LAB*4;
drift_pntr = (long *)i;
i = IRU_BASE + STANDARD_BASE + rxa_buffer_offset + ALTITUDE_LAB*4;
altitude_pntr = (long *)i;
i = IRU_BASE + STANDARD_BASE + rxa_buffer_offset + INERTIAL_VSPEED_LAB*4;
inertial_vspeed_pntr = (long *)i;
i = IRU_BASE + STANDARD_BASE + rxa_buffer_offset + PITCH_LAB*4;
pitch_pntr = (long *)i;
i = IRU_BASE + STANDARD_BASE + rxa_buffer_offset + ROLL_LAB*4;
roll_pntr = (long *)i;
i = IRU_BASE + STANDARD_BASE + rxa_buffer_offset + INTEG_VERT_ACC_LAB*4;
integ_vert_acc_pntr = (long *)i;
i = IRU_BASE + STANDARD_BASE + rxa_buffer_offset + VERT_ACC_LAB*4;
vert_acc_pntr = (long *)i;
i = IRU_BASE + STANDARD_BASE + rxa_buffer_offset + TRACK_RATE_LAB*4;
track_rate_pntr = (long *)i;
i = IRU_BASE + STANDARD_BASE + rxa_buffer_offset + PITCH_RATE_LAB*4;
pitch_rate_pntr = (long *)i;

}

