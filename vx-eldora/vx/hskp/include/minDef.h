/*
 *	$Id$
 *
 *	Module: minDef.h		 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/14  21:38:15  reif
 * Initial revision
 *
 *
 * description:
 *        
 */

#ifndef INCminDefh
#define INCminDefh

/***************** MINIRIMS DEFINES ************************/

#define MIN_BASE 0x20007000
#define X_AXIS_ACCEL 0x00 /* XA-OUT */
#define Y_AXIS_ACCEL 0x02 /* YA_OUT */
#define Z_AXIS_ACCEL 0x04 /* ZA-OUT */
#define X_AXIS_VEL 0x06 /* XG-OUT */
#define Y_AXIS_VEL 0x08 /* YG-OUT */
#define Z_AXIS_VEL 0x0A /* ZG-OUT */
#define XR_AXIS_VEL 0x0C /* XRG-OUT */
#define READ_TEMP 0x0E /* TEMP OUT */
#define LATCH_COUNT 0x10 /* LATCHCNT */
#define MIN_STATUS 0x12
#define MIN_COMMAND 0x14
#define RESOLVER 0x16 
#define COUNT_RESET 0x18
#define RESOLVER_STAT 0x1A
#define X_ACCEL_SCALE 0.0213660
#define Y_ACCEL_SCALE 0.0215166
#define Z_ACCEL_SCALE 0.0212280
#define X_GYRO_SCALE 7.95390
#define Y_GYRO_SCALE 7.83644
#define Z_GYRO_SCALE 7.27752
#define XR_GYRO_SCALE 7.21600
#define TEMP_SCALE 0.002
#define GIMB_CMND_DSBL 0x000
#define ROT_GIMB_CW 0x001
#define ROT_GIMB_CCW 0x007
#define ROT_GIMB_180 0x009
#define ROT_GIMB_270 0x00B
#define ROT_GIMB_90 0x00D
#define ROT_GIMB_0 0x00F
#define RUN_GYROS 0x080
#define ACCL_BD_TEMP 0x000
#define X_Y_GYRO_TEMP 0x010
#define XR_Z_GYRO_TEMP 0x020
#define X_ACCL_TEMP 0x030
#define Y_ACCL_TEMP 0x040
#define Z_ACCL_TEMP 0x050
#define LWR_STBL_EL_TEMP 0x060
#define GIMB_DSBL 0x100

#endif /* INC */





