/*****************************************************************
* rims.h							 *
******************************************************************

<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< N O T E >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

This file is under configuration control.  Any changes to this routine
must be noted in the configuration control log.

<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


Description:	This file includes calibration parameters for mini
	        RIMS and information on the decoding data structures
		needed to access the mini RIMS c data.  This includes
	        definitions of the data structures used to access the
	        bit fields of the RIMS control words.  This file also
	        includes the input and output discrete word decoding.

inputs:		none
outputs:	none

notes:		This file is for use with mini RIMS c only.

		Calibration data is from MINI-RIMS S/N 17 and
                18 - test runs signed off 7/7/92

		Resolver data dated 10/25/88.

		SIZE EFFECT - accelerometer offsets valid only for
			      MINI RIMS 'C' hardware.

called routines: none

*****************************************************************/

#define RIMS17

#ifdef RIMS17		/* PRIMARY ELDORA UNIT */
#define RIMS_ID 17     

/* ----------------- Accelerometer Constants -------------------- */

#define XAK   1.67061E-02     /* Accel Scale Factors */
#define YAK   1.59403E-02     /* (ft/sec/count) */
#define ZAK   1.62578E-02

#define XAB   7.41244E-02     /* Accel Biases (ft/sec/sec) */
#define YAB  -6.65252E-02
#define ZAB   7.82460E-03

#define XAZ   1.111E-03       /* Accel Misalignments (rad) */
#define XAY  -2.329E-03
#define YAZ   2.132E-03
#define YAX   1.191E-03
#define ZAY   1.504E-03
#define ZAX  -1.321E-03

/* ----------------- Gyro Constants ------------------------------ */

#define XGKP  3.3395518E-05     /* Gyro Scale Factors (rad/count) */
#define XRGKP 3.3492093E-05
#define YGKP  3.2938775E-05
#define ZGKP  3.3824578E-05
#define XGKN  3.3391494E-05
#define XRGKN 3.3497280E-05
#define YGKN  3.2942605E-05
#define ZGKN  3.3827875E-05

#define XGB   1.6702801E-04     /* Gyro Biases (rad/sec) */
#define XRGB  1.0894733E-04
#define YGB   1.5548944E-04
#define ZGB   7.0375554E-05

#define XGAX  1.7042255E-06     /* Gyro G Sensitivities */
#define YGAX  1.2933384E-07     /* (rad/sec/ft/sec/sec) */
#define ZGAX  2.5550584E-07     /* XR gyro data used for X terms */
#define XGAY -2.5671035E-07
#define YGAY -1.2103780E-06
#define ZGAY -9.0337953E-10
#define XGAZ -4.5093695E-07
#define YGAZ -1.7962196E-08
#define ZGAZ  1.7085918E-06

#define XGZ      -2.465E-03     /* Gyro Misalignments (rad) */
#define XGY       1.678E-03     /* XR gyro data used for X terms */
#define YGZ      -1.453E-03
#define YGX      -1.319E-03
#define ZGY       1.553E-03
#define ZGX      -1.139E-03


/* -------------------------------------------------------------------	*/
/*		resolver sin and cos error model terms			*/
/* -------------------------------------------------------------------  */

#define R_SIN_NOM	 9.5495
#define R_SIN_OFF	-0.0023
#define R_SIN_BIAS	 0.0225

#define R_COS_NOM	 9.5584
#define R_COS_OFF	-0.0023
#define R_COS_BIAS	 0.0216

#endif

#ifdef RIMS18		/* BACKUP ELDORA UNIT */
#define RIMS_ID 18    

#define XAK   1.66721E-02     /* Accel Scale Factors */
#define YAK   1.63136E-02     /* (ft/sec/count) */
#define ZAK   1.68582E-02

#define XAB   1.29508E-01     /* Accel Biases (ft/sec/sec) */
#define YAB  -4.19244E-02
#define ZAB  -1.26932E-01

#define XAZ   2.052E-03       /* Accel Misalignments (rad) */
#define XAY   2.739E-03
#define YAZ  -4.639E-03
#define YAX   2.952E-03
#define ZAY   1.070E-03
#define ZAX  -2.747E-03

/* ----------------- Gyro Constants ------------------------------ */

#define XGKP  3.4758087E-05     /* Gyro Scale Factors (rad/count) */
#define XRGKP 3.2161279E-05
#define YGKP  3.4242633E-05
#define ZGKP  3.1515362E-05
#define XGKN  3.4753093E-05
#define XRGKN 3.2168842E-05
#define YGKN  3.4245250E-05
#define ZGKN  3.1520937E-05

#define XGB   6.4625664E-06     /* Gyro Biases (rad/sec) */
#define XRGB  4.5814893E-06
#define YGB  -2.1069518E-04
#define ZGB  -1.3968937E-04

#define XGAX -1.8623169E-06     /* Gyro G Sensitivities */
#define YGAX  3.3771338E-07     /* (rad/sec/ft/sec/sec) */
#define ZGAX -1.3008665E-07     /* XR gyro data used for X terms */
#define XGAY -2.1425151E-07
#define YGAY -2.7221836E-07
#define ZGAY  4.5168976E-10
#define XGAZ -3.5382365E-08
#define YGAZ -7.9798525E-08
#define ZGAZ -1.8642742E-06

#define XGZ   1.779E-03     /* Gyro Misalignments (rad) */
#define XGY   1.753E-03     /* XR gyro data used for X terms */
#define YGZ   2.559E-03
#define YGX   1.284E-03
#define ZGY   2.672E-03
#define ZGX   1.788E-03

/* -------------------------------------------------------------------	*/
/*		resolver sin and cos error model terms			*/
/* -------------------------------------------------------------------  */

#define R_SIN_NOM	 9.1085
#define R_SIN_OFF	 2.4299e-004
#define R_SIN_BIAS	 7.9167e-004

#define R_COS_NOM	 9.0680
#define R_COS_OFF	 3.1831e-004
#define R_COS_BIAS	-0.0088

#endif

#define RXX        0.367167     /* Accel Offsets  (ft) */
#define RXY       -0.064583
#define RXZ        0.066583
#define RYX        0.386667
#define RYY        0.021333
#define RYZ        0.077000
#define RZX        0.227083
#define RZY        0.043000
#define RZZ       -0.042167

/* --------------------------------------------------------------------	*/

/* 	linear prediction coefficients - MINI RIMS C #001		*/

#define SC1  4.07207207	/* xy gyro slope value - positive dir	*/
#define SC2 -8.277593e-5	/* xy intercept value - positive dir	*/
#define SC3  2.56842105	/* xy gyro slope value - negative dir	*/
#define SC4 -4.195793e-5	/* xy intercept value - negative dir	*/
#define SC5 -2.22183099	/* xrz gyro slope value - positive dir	*/
#define SC6  8.754834e-5	/* xrz intercept value - positive dir	*/
#define SC7 -3.28571429	/* xrz gyro slope value - negative dir	*/
#define SC8  1.168080e-4	/* xrz intercept value - negative dir	*/
#define SC9   0.8553054	/* xy accelerometer slope value 	*/
#define SC10  2.152829e-3	/* xy accelerometer intercept value 	*/
#define SC11  0.6881028	/* xz accelerometer slope value 	*/
#define SC12  3.490321e-3	/* xz accelerometer intercept value 	*/

/* ------------------ Temp Modeling Parameters -----------------------	*/

#define T_XGB_NOMP   51.245	/* X gyro nominal Bias 			*/
#define T_XGB_TSLP  -0.16833	/* X gyro bias - Slope Comp - sens temp	*/
#define T_YGB_NOMP   5.798	/* Y gyro nominal Bias 			*/
#define T_YGB_TSLP   0.02032	/* Y gyro bias - Slope Comp - sens temp	*/
#define T_XRGB_NOMP -55.342	/* XR gyro nominal Bias	 		*/
#define T_XRGB_TSLP  0.20522	/* XR gyro bias - Slope Comp - sns temp	*/
#define T_ZGB_NOMP  -21.128	/* Z gyro nominal Bias 			*/
#define T_ZGB_TSLP   0.08693	/* Z gyro bias - Slope Comp - sens temp	*/

#define T_XG_NOMP   5.77229 	/* X gyro nominal Pos Sc factor		*/
#define T_XG_TSLP  -384.1e-6	/* X gyro pos - Slope Comp - sens temp	*/
#define T_XG_AFSLP  3.1e-6	/* X Gyro pos - Slope Comp - a/f temp	*/
#define T_XG_NOMN   5.76978	/* X gyro nominal Neg Sc factor		*/
#define T_XG_TSLN  -382.2e-6	/* X gyro neg - Slope Comp - sens temp	*/
#define T_XG_AFSLN  3.1e-6	/* X Gyro neg - Slope Comp - a/f temp	*/
#define T_YG_NOMP   5.89313	/* Y gyro nominal Pos Sc factor		*/
#define T_YG_TSLP  -386.6e-6	/* Y Gyro pos - Slope Comp - sens temp	*/
#define T_YG_AFSLP  2.9e-6	/* Y Gyro pos - Slope Comp - a/f temp	*/
#define T_YG_NOMN   5.89453	/* Y gyro nominal Neg Sc factor		*/
#define T_YG_TSLN  -386.2e-6	/* Y Gyro neg - Slope Comp - sens temp	*/
#define T_YG_AFSLN  2.9e-6	/* Y Gyro neg - Slope Comp - a/f temp	*/

#define T_XRG_NOMP   5.79706	/* XR gyro nominal Pos Sc factor	*/
#define T_XRG_TSLP  -273.5e-6	/* XR gyro pos - Slope Comp - sens temp	*/
#define T_XRG_AFSLP  7.9e-6	/* XR Gyro pos - Slope Comp - a/f temp	*/
#define T_XRG_NOMN   5.80437	/* XR gyro nominal Neg Sc factor	*/
#define T_XRG_TSLN  -286.1e-6	/* XR gyro neg - Slope Comp - sens temp	*/
#define T_XRG_AFSLN  7.9e-6	/* XR Gyro neg - Slope Comp - a/f temp	*/
#define T_ZG_NOMP    5.59325	/* Z gyro nominal Pos Sc factor		*/
#define T_ZG_TSLP   -285.5e-6	/* Z Gyro pos - Slope Comp - sens temp	*/
#define T_ZG_AFSLP   0.9e-6	/* Z Gyro pos - Slope Comp - a/f temp	*/
#define T_ZG_NOMN    5.59370	/* Z gyro nominal Neg Sc factor		*/
#define T_ZG_TSLN   -284.3e-6	/* Z Gyro neg - Slope Comp - sens temp	*/
#define T_ZG_AFSLN   0.9e-6	/* Z Gyro neg - Slope Comp - a/f temp	*/

#define T_XAB_NOMP  -0.266218	/* X accel nominal Bias 		*/
#define T_XAB_TSLP  -1.1164e-3	/* X acc bias - Slope Comp - sens temp	*/
#define T_XA_CAL_TMP	69.54	/* X acc cal temperature - deg c	*/
#define T_YAB_NOMP   0.000032	/* Y accel nominal Bias 		*/
#define T_YAB_TSLP   0.7000e-3	/* Y acc bias - Slope Comp - sens temp	*/
#define T_YA_CAL_TMP	67.86	/* Y acc cal temperature - deg c	*/
#define T_ZAB_NOMP   0.058063	/* Z accel nominal Bias 		*/
#define T_ZAB_TSLP  -0.0453e-3	/* Z acc bias - Slope Comp - sens temp	*/
#define T_ZA_CAL_TMP	70.85	/* Z acc cal temperature - deg c	*/

#define T_XAK_NOMP   0.0165881	/* X accel nominal Sc factor		*/
#define T_XAK_TSLP   136.0e-6	/* X accel sf - Slope Comp - sens temp	*/
#define T_XAK_AFSLP -2.0e-6	/* X accel sf - Slope Comp - a/f temp	*/
#define T_YAK_NOMP   0.0149042	/* Y accel nominal Sc factor		*/
#define T_YAK_TSLP   143.0e-6	/* Y accel sf - Slope Comp - sens temp	*/
#define T_YAK_AFSLP  2.0e-6	/* Y accel sf - Slope Comp - a/f temp	*/
#define T_ZAK_NOMP   0.0163395	/* Z accel nominal Sc factor		*/
#define T_ZAK_TSLP   144.0e-6	/* Z accel sf - Slope Comp - sens temp	*/
#define T_ZAK_AFSLP  1.0e-6	/* Z accel sf - Slope Comp - a/f temp	*/

#define TEMP_AAF  37.0		/* Nominal A/F used in temp modeling	*/
#define TEMP_GAF  43.0		/* Nominal A/F used in temp modeling	*/
#define TEMP_SNS 75.0		/* Nominal Sensor Temp used in model	*/

/* ---------------- Interrupt Rate Related Constants ------------------	*/

#define DELTIME 0.015625	/* 1/64 sec - time between updates	*/
#define ONESEC	64
#define INTHRZ	128		/* number of interrupts per sec		*/

/* --------------------------------------------------------------------	*/

/* RIMS Interface and Control related parameters and structures		*/

#define RIMS_START 0xeffc000L;		/* interface addresses	*/

#define  ACC_AF_TMP	0	/* data_sel decoding table	*/
#define  XY_GYRO_TMP	1
#define  XRZ_GYRO_TMP	2
#define  X_ACC_TMP	3
#define  Y_ACC_TMP	4
#define  Z_ACC_TMP	5
#define  GYRO_AF_TMP	6
#define  STAB_EL_TMP	7

#define  SLEW_CCW	0	/* rims_control decoding table	*/
#define  UNDEF1		1
#define  UNDEF2		2
#define  SLEW_CW	3
#define  CAGE_180	4
#define  CAGE_270	5
#define  CAGE_90	6
#define  CAGE_0		7

#define  STD_BY		0	/* rims_oper decoding table	*/
#define  RUN_CAGE_SLEW	1
#define  RUN_INERT	3
#define  UNDEF3		2

#define  INTER_ENABLE	0xFF	/* interrupt enable control word	*/
#define  INTER_DISABLE	0

struct plat_control				/* RIMS platform control word bit fields */
	{
	unsigned unas1  : 8;		/* bits 8-15 of interface word	*/
	unsigned rims_oper : 2;		/* bits 6-7 rims operation fld	*/
	unsigned rims_cont : 3;		/* bits 3-5 rims control field	*/
	unsigned data_sel : 3;		/* bits 0-2 data select field	*/
	};


struct indiscrete
	{
	unsigned unas2  : 8;	/* bits 8-15 of discrete word		*/
	unsigned idis7  : 1;	/* bit 7 input discrete			*/
	unsigned idis6  : 1;	/* bit 6 acs disable discrete		*/
	unsigned sep2   : 1;	/* bit 5 separation discrete signal 2	*/
	unsigned sep1   : 1;	/* bit 4 separation discrete signal 1   */
	unsigned ndis   : 4;	/* bits 0-3 RIMS mode discrete		*/
	};
struct out1discrete		/* 1st output discrete word		*/
	{
	unsigned odis16 : 1;	/* bit 15 output discrete		*/
	unsigned gpson  : 1;	/* bit 14 GPS on			*/
	unsigned odis14 : 1;	/* bit 13 output discrete		*/
	unsigned odis13 : 1;	/* bit 12 output discrete               */
	unsigned esdply : 1;	/* bit 11 ES deploy                     */
	unsigned bloon2 : 1;	/* bit 10 Balloon #2 deploy             */
	unsigned bloon1 : 1;	/* bit 9 Balloon #1 deploy              */
	unsigned pr_vlv : 1;	/* bit 8 Pressure Valve                 */
	unsigned odis8  : 1;	/* bit 7 output discrete                */
	unsigned noz_6  : 1;	/* bit 6 CG ACS nozzle number 6 	*/
	unsigned noz_5  : 1;	/* bit 5 CG ACS nozzle number 5 	*/
	unsigned noz_4  : 1;	/* bit 4 CG ACS nozzle number 4		*/
	unsigned odis4  : 1;	/* bit 3 output discrete                */
	unsigned noz_3  : 1;	/* bit 2 CG ACS nozzle number 3		*/
	unsigned noz_2  : 1;	/* bit 1 CG ACS nozzle number 2		*/
	unsigned noz_1  : 1;	/* bit 0 CG ACS nozzle number 1         */
	};

struct out2discrete		/* 2nd output discrete word		*/
	{
	unsigned unas3  : 8;	/* bits 8-15 of output discrete word	*/
	unsigned odis24 : 1;	/* bit 7 output discrete		*/
	unsigned odis23 : 1;	/* bit 6 output discrete		*/
	unsigned odis22 : 1;	/* bit 5 output discrete		*/
	unsigned odis21 : 1;	/* bit 4 output discrete		*/
	unsigned odis20 : 1;	/* bit 3 output discrete		*/
	unsigned odis19 : 1;	/* bit 2 output discrete		*/
	unsigned odis18 : 1;	/* bit 1 output discrete                */
	unsigned mdion  : 1;	/* bit 0 MDI on                         */
	};

struct rims
	{
	short int ad,dac1,dac2,dac3,dac4;
	short int control;
	short int int_control;
	short int cs1c,cs2c,cs3c;
	short int cs1d,cs2d,cs3d;
	short int csd;
	short int out1dis;
	short int out2dis;
	short int indis;
	short int ad2,ad1,ad0;
	short int rtcinhib;
	};
