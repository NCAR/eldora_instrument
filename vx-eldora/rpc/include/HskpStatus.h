/*
 *	$Id$
 *
 *	Module:		 HskpStatus.h
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1992/07/27  14:45:29  thor
 * Added extern for init routine.
 *
 * Revision 1.2  1992/07/14  14:10:13  thor
 * Changed ALIVE to DEAD.
 *
 * Revision 1.1  1992/07/13  20:19:02  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCHskpStatush
#define INCHskpStatush

struct HskpStatus {
    unsigned long count;
    unsigned char fore;
    unsigned char aft;
    unsigned char motorCtrl;
    unsigned char transPower;
    unsigned char testPower;
    unsigned char iru;
    unsigned char gps;
    unsigned char ecb;
    unsigned char slave;
    unsigned char onTemp;
    unsigned char offTemp;
    unsigned char foreDDS;
    unsigned char aftDDS;
    unsigned char minirims;
    unsigned char minirimsTemp;
    unsigned char clock;
    unsigned char insitu;
};

#ifdef OK_RPC
typedef struct HskpStatus HskpStatus;

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

#define GPS_DATA_LATE    0x1
#define UNKNOWN_GPS_DATA 0x2

#define ECB_MASTER_BAD 0x1

#define SLAVE_1_DEAD 0x2
#define SLAVE_2_DEAD 0x4
#define SLAVE_3_DEAD 0x8
#define SLAVE_4_DEAD 0x10
#define SLAVE_5_DEAD 0x20

#define MASTER_TEMP_BAD  0x1
#define SLAVE_1_TEMP_BAD 0x2
#define SLAVE_2_TEMP_BAD 0x4
#define SLAVE_3_TEMP_BAD 0x8
#define SLAVE_4_TEMP_BAD 0x10
#define SLAVE_5_TEMP_BAD 0x20

#define SLAVE_1_LOCK_BAD 0x2
#define SLAVE_2_LOCK_BAD 0x4
#define SLAVE_3_LOCK_BAD 0x8
#define SLAVE_4_LOCK_BAD 0x10
#define SLAVE_5_LOCK_BAD 0x20

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

#ifndef HSKP_RPC_SCOPE
#define HSKP_RPC_SCOPE extern
#endif

HSKP_RPC_SCOPE HskpStatus *currStatus; /* Our current status. */

#ifdef __cplusplus
extern "C" {
#endif
extern int HskpRpcInit(void);	/* Initialize RPC. */
#ifdef __cplusplus
};
#endif

#endif /* OK_RPC */

#endif /* INCHskpStatush */

