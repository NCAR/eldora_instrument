/*
 *	$Id$
 */
#ifndef _HSKPSTATUSDEFS_H_
#define _HSKPSTATUSDEFS_H_

#define BAD_RAY_COUNT 0x1
#define RAYS_FULL     0x2
#define ADS_FULL      0x4
#define NAV_FULL      0x8
#define GPS_SYNC      0x10
#define IRU_SYNC      0x20
#define MINI_SYNC     0x40

#define MOTOR_STOPPED 0x1
#define BAD_CORR_VEL  0x2
#define BAD_MOTOR_VEL 0x4

#define POWER_INT_BAD 0x1
#define A_POWER_BAD   0x2
#define B_POWER_BAD   0x4

#define ARINC_INT_BAD 0x1
#define IRU_WORDS_BAD 0x2
#define ARINC_ERRORS  0x4

#define GPS_DATA_LATE    0x1
#define UNKNOWN_GPS_DATA 0x2

#define SLAVE_1_DEAD 0x2
#define SLAVE_2_DEAD 0x4
#define SLAVE_3_DEAD 0x8
#define SLAVE_4_DEAD 0x10
#define SLAVE_5_DEAD 0x20

#define MASTER_OVER_TEMP  0x1
#define SLAVE_1_OVER_TEMP 0x2
#define SLAVE_2_OVER_TEMP 0x4
#define SLAVE_3_OVER_TEMP 0x8
#define SLAVE_4_OVER_TEMP 0x10
#define SLAVE_5_OVER_TEMP 0x20

#define FREQ_1_OOL 0x2
#define FREQ_2_OOL 0x4
#define FREQ_3_OOL 0x8
#define FREQ_4_OOL 0x10
#define FREQ_5_OOL 0x20
#define FREQ_6_OOL 0x40

#define MINIRIMS_WARMING      0x1
#define MINIRIMS_ALIGNING     0x2
#define MINIRIMS_ALIGN_BAD    0x4
#define MINIRIMS_INERTIAL_BAD 0x8
#define GYRO_SPIN_OFF         0x10
#define GYRO_LOOPS_OPEN       0x20
#define GIMBAL_LOOPS_OPEN     0x40
#define MINIRIMS_POWER_LOW    0x80

#define ACCEL_A_F_BAD 0x1
#define X_Y_GYRO_BAD  0x2
#define XR_Y_GYRO_BAD 0x4
#define X_ACCEL_BAD   0x8
#define Y_ACCEL_BAD   0x10
#define Z_ACCEL_BAD   0x20
#define GYRO_A_F_BAD  0x40
#define PLATFORM_BAD  0x80

#define IRIGB_SYNCING     0x1
#define IRIGB_SYNC_FAILED 0x2
#define TIME_SET_READY    0x4

#define NO_ADS_HEADER 0x1
#define NO_ADS_DATA   0x2

#endif /* _HSKPSTATUSDEFS_H_ */
