/*
 *	$Id$
 *
 *	Module:		 MiniRIMS.h
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1996/10/07  14:30:29  thor
 * *** empty log message ***
 *
 * Revision 1.1  1992/07/28  17:23:46  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCMiniRIMSh
#define INCMiniRIMSh

struct minirims_data {
    char  minirims_data_id[4];	/* Identifier = MINI. */
    long  minirims_data_len;	/* Block size in bytes. */
    short second;
    short millisecond;
    short command;		/* Current command latch setting. */
    short status;		/* Current status. */
    float temperature;		/* Degrees C. */
    short x_axis_gyro_up[128];	/* Roll axis gyro up counts. */
    short x_axis_gyro_dn[128];	/* Roll axis gyro down counts. */
    short y_axis_gyro_up[128];	/* Pitch axis gyro up counts. */
    short y_axis_gyro_dn[128];	/* Pitch axis gyro down counts. */
    short z_axis_gyro_up[128];	/* Yaw axis gyro up counts. */
    short z_axis_gyro_dn[128];	/* Yaw axis gyro down counts. */
    short xr_axis_gyro_up[128];	/* Roll axis redundate gyro up counts. */
    short xr_axis_gyro_dn[128];	/* Roll axis redundate gyro down counts. */
    short x_axis_acc[128];	/* Longitudinal axis accelerometer counts. */
    short y_axis_acc[128];	/* Lateral axis accelerometer counts. */
    short z_axis_acc[128];	/* Vertical axis accelerometer counts. */
    float x_axis_pos[128];	/* Roll axis gimbal angle. */
};

typedef struct minirims_data minirims_data;
typedef struct minirims_data MINIRIMS_DATA;

#endif /* INCMiniRIMSh */

