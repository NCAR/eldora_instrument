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
 * Revision 1.2  1992/09/01  17:25:12  craig
 * *** empty log message ***
 *
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
#ifndef MV133
#define MIN_BASE 0x20007000
#else
#define MIN_BASE 0xFFFF7000
#endif

#define X_AXIS_ACCEL 0x06 /* XA-OUT */
#define Y_AXIS_ACCEL 0x08 /* YA_OUT */
#define Z_AXIS_ACCEL 0x0A /* ZA-OUT */
#define X_AXIS_VEL 0x00 /* XG-OUT */
#define Y_AXIS_VEL 0x02 /* YG-OUT */
#define Z_AXIS_VEL 0x04 /* ZG-OUT */
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
#define TEMP_SCALE 100.0/65536.0 /* deg C/Hz */ 
#define GIMB_CMND_DSBL 0x000
#define ROT_GIMB_CW 0x6
#define ROT_GIMB_CCW 0x0
#define ROT_GIMB_180 0x8
#define ROT_GIMB_270 0xA
#define ROT_GIMB_90 0xC
#define ROT_GIMB_0 0xE
#define RUN_GYROS 0x80
#define CAGE_INER 0x1
#define STBY 0x0
#define STBY_MASK 0x7F
#define ACCL_BD_TEMP 0x000           /* not valid */
#define X_Y_GYRO_TEMP 0x010
#define XR_Z_GYRO_TEMP 0x020
#define X_ACCL_TEMP 0x030
#define Y_ACCL_TEMP 0x040
#define Z_ACCL_TEMP 0x050
#define LWR_STBL_EL_TEMP 0x060       /* not valid */
#define PLATFORM_TEMP 0x70
#define TEMP_MASK 0x70
#define GIMB_DSBL 0x100

#endif /* INC */





