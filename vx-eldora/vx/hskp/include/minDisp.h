/*
 *	$Id$
 *
 *	Module:		 min_Disp.h
 *	Original Author: Eric Loew
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * Initial revision
 *
 *
 *
 * description:
 *
 * Data structures for Minirims display screen.
 */
#ifndef INCmin_Disph
#define INCmin_Disph

#ifdef OK_RPC

#endif /* OK_RPC */

/* Screen Coordinates of Overlay 1 */

#define TITLE_X 25
#define TITLE_Y 0
#define MAIN_HD_X 10
#define MAIN_HD_Y 2
#define ACC_GYR_HD_X 2
#define ACC_GYR_HD_Y 4
#define ACC_GYR_SH1_X 0
#define ACC_GYR_SH1_Y 5
#define ACC_GYR_FH1_X 0
#define ACC_GYR_FH1_Y 7
#define ACC_GYR_FH2_X 0
#define ACC_GYR_FH2_Y 8
#define ACC_GYR_FH3_X 0
#define ACC_GYR_FH3_Y 9
#define ACC_GYR_FH4_X 0
#define ACC_GYR_FH4_Y 11
#define ACC_GYR_FH5_X 0
#define ACC_GYR_FH5_Y 12
#define ACC_GYR_FH6_X 0
#define ACC_GYR_FH6_Y 13
#define RES_X 0
#define RES_Y 15
#define COMM1_X 38
#define COMM1_Y 4
#define COMM2_X 38
#define COMM2_Y 5
#define COMM3_X 38
#define COMM3_Y 6
#define COMM4_X 38
#define COMM4_Y 7
#define COMM5_X 38
#define COMM5_Y 8
#define COMM6_X 38
#define COMM6_Y 9
#define COMM7_X 38
#define COMM7_Y 10
#define COMM8_X 38
#define COMM8_Y 11
#define COMM9_X 38
#define COMM9_Y 12
#define COMM10_X 38
#define COMM10_Y 13
#define COMM11_X 38
#define COMM11_Y 14
#define COMM12_X 38
#define COMM12_Y 15
#define PLTFM_FH1_X 59
#define PLTFM_FH1_Y 4
#define PLTFM_FH2_X 59
#define PLTFM_FH2_Y 5
#define PLTFM_FH3_X 59
#define PLTFM_FH3_Y 6
#define PLTFM_FH4_X 59
#define PLTFM_FH4_Y 7
#define TEMP_HD_X 59
#define TEMP_HD_Y 10
#define TEMP_FH1_X 59
#define TEMP_FH1_Y 12
#define TEMP_FH2_X 59
#define TEMP_FH2_Y 13
#define TEMP_FH3_X 59
#define TEMP_FH3_Y 14
#define TEMP_FH4_X 59
#define TEMP_FH4_Y 15
#define TEMP_FH5_X 59
#define TEMP_FH5_Y 16
#define TEMP_FH6_X 59
#define TEMP_FH6_Y 17
#define TEMP_FH7_X 59
#define TEMP_FH7_Y 18
#define TEMP_FH8_X 59
#define TEMP_FH8_Y 19
#define TIME_FH1_X 0
#define TIME_FH1_Y 16
#define TIME_FH2_X 0
#define TIME_FH2_Y 17
#define TIME_FH3_X 0
#define TIME_FH3_Y 19
#define TIME_FH4_X 0
#define TIME_FH4_Y 20
#define COMM_FH_X 59
#define COMM_FH_Y 22

struct mini_Ov1 {
    char  title[35];            /* screen title */
    char  main_head[60];	/* main screen heading */
    char  acc_gyr_head[40];     /* accel. and gyro field heading */
    char  temp_head[20];        /* temperature status heading */
    char  comm1[15];            /* command number 1 */
    char  comm2[15];            /* command number 2 */
    char  comm3[15];            /* command number 3 */
    char  comm4[15];            /* command number 4 */
    char  comm5[15];            /* command number 5 */
    char  comm6[15];            /* command number 6 */
    char  comm7[15];            /* command number 7 */
    char  comm8[15];            /* command number 8 */
    char  comm9[15];            /* command number 9 */
    char  comm10[15];           /* command number 10 */
    char  comm11[15];           /* command number 11 */
    char  comm12[15];           /* command number 12 */
    char  temp_fh1[10];         /* temp. field heading 1 */
    char  temp_fh2[10];         /* temp. field heading 2 */
    char  temp_fh3[10];         /* temp. field heading 3 */
    char  temp_fh4[10];         /* temp. field heading 4 */
    char  temp_fh5[10];         /* temp. field heading 5 */
    char  temp_fh6[10];         /* temp. field heading 6 */
    char  temp_fh7[10];         /* temp. field heading 7 */
    char  temp_fh8[10];         /* temp. field heading 8 */
    char  pltfm_fh1[15];        /* platform field heading 1 */
    char  pltfm_fh2[15];        /* platform field heading 2 */
    char  pltfm_fh3[15];        /* platform field heading 3 */
    char  pltfm_fh4[15];        /* platform field heading 4 */
    char  acc_gyr_fh1[10];      /* accel. and gyro field heading 1 */
    char  acc_gyr_fh2[10];      /* accel. and gyro field heading 2 */
    char  acc_gyr_fh3[10];      /* accel. and gyro field heading 3 */
    char  acc_gyr_fh4[10];      /* accel. and gyro field heading 4 */
    char  acc_gyr_fh5[10];      /* accel. and gyro field heading 5 */
    char  acc_gyr_fh6[10];      /* accel. and gyro field heading 6 */
    char  resolver_fh[10];      /* resolver field heading */
    char  time_fh1[12];         /* time field heading 1 */
    char  time_fh2[12];         /* time field heading 2 */
    char  time_fh3[12];         /* time field heading 3 */
    char  time_fh4[12];         /* time field heading 4 */
    char  acc_gyr_sh1[40];      /* accel. and gyro sub heading 1 */
    char comm_fh[10];          /* command field heading */
    
}; /* End of Structure */

/* typedef struct mini_Ov1 MINI_OV1; */

struct mini_data {
    char  temp_df1[10];         /* temp. field heading 1 */
    char  temp_df2[10];         /* temp. field heading 2 */
    char  temp_df3[10];         /* temp. field heading 3 */
    char  temp_df4[10];         /* temp. field heading 4 */
    char  temp_df5[10];         /* temp. field heading 5 */
    char  temp_df6[10];         /* temp. field heading 6 */
    char  temp_df7[10];         /* temp. field heading 7 */
    char  temp_df8[10];         /* temp. field heading 8 */
    char  pltfm_df1[10];        /* platform field heading 1 */
    char  pltfm_df2[10];        /* platform field heading 2 */
    char  pltfm_df3[10];        /* platform field heading 3 */
    char  pltfm_df4[10];        /* platform field heading 4 */
    char  acc_gyr_df1a[10];      /* accel. and gyro field heading 1a */
    char  acc_gyr_df1b[10];      /* accel. and gyro field heading 1b */
    char  acc_gyr_df1c[10];      /* accel. and gyro field heading 1c */
    char  acc_gyr_df2a[10];      /* accel. and gyro field heading 2a */
    char  acc_gyr_df2b[10];      /* accel. and gyro field heading 2b */
    char  acc_gyr_df2c[10];      /* accel. and gyro field heading 2c */
    char  acc_gyr_df3a[10];      /* accel. and gyro field heading 3a */
    char  acc_gyr_df3b[10];      /* accel. and gyro field heading 3b */
    char  acc_gyr_df3c[10];      /* accel. and gyro field heading 3c */
    char  acc_gyr_df4a[10];      /* accel. and gyro field heading 4a */
    char  acc_gyr_df4b[10];      /* accel. and gyro field heading 4b */
    char  acc_gyr_df4c[10];      /* accel. and gyro field heading 4c */
    char  acc_gyr_df5a[10];      /* accel. and gyro field heading 5a */
    char  acc_gyr_df5b[10];      /* accel. and gyro field heading 5b */
    char  acc_gyr_df5c[10];      /* accel. and gyro field heading 5c */
    char  acc_gyr_df6a[10];      /* accel. and gyro field heading 6a */
    char  acc_gyr_df6b[10];      /* accel. and gyro field heading 6b */
    char  acc_gyr_df6c[10];      /* accel. and gyro field heading 6c */
    char  resolver_df[10];       /* resolver data field heading */
    char  time_df1[10];         /* time field heading 1 */
    char  time_df2[10];         /* time field heading 2 */
    char  time_df3[10];         /* time field heading 3 */
    char  time_df4[10];        /* time field heading 4 */
    char  comm_df[15];          /* command data field */
    
}; /* End of Structure */

/* typedef struct mini_data MINI_D; */

#ifdef OK_RPC


#endif /* OK_RPC */

#endif /* INCRayh */

