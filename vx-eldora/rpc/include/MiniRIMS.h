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
 *
 *
 * description:
 *        
 */
#ifndef INCMiniRIMSh
#define INCMiniRIMSh

#ifdef OK_RPC

#ifdef UNIX
#include <rpc/rpc.h>
#else
#include "rpc/rpc.h"
#endif /* UNIX */

#endif /* OK_RPC */

struct minirims_data {
    char  minirims_data_id[4];	/* Identifier = MINI. */
    long  minirims_data_len;	/* Block size in bytes. */
    short command;		/* Current command latch setting. */
    short status;		/* Current status. */
    float temperature;		/* Degrees C. */
    float x_axis_gyro[128];	/* Roll axis gyro position. */
    float y_axis_gyro[128];	/* Pitch axis gyro position. */
    float z_axis_gyro[128];	/* Yaw axis gyro position. */
    float xr_axis_gyro[128];	/* Roll axis redundate gyro position. */
    float x_axis_vel[128];	/* Longitudinal axis velocity. */
    float y_axis_vel[128];	/* Lateral axis velocity. */
    float z_axis_vel[128];	/* Vertical axis velocity. */
    float x_axis_pos[128];	/* Roll axis gimbal. */
};

#ifdef OK_RPC

typedef struct minirims_data minirims_data;
typedef struct minirims_data MINIRIMS_DATA;

extern bool_t xdr_minirims_data(XDR *, minirims_data *);

#endif /* OK_RPC */
#endif /* INCMiniRIMSh */

