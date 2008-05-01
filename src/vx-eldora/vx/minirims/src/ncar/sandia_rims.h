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

		Calibration data is from MINI-RIMS-C #001 - test runs
		dated 10/19/88

		Resolver data dated 10/25/88.

		SIZE EFFECT - accelerometer offsets valid only for
			      MINI RIMS 'C' hardware.

called routines: none

*****************************************************************/

#define RIMS8

#ifdef RIMS3		/* PRIMARY UNIT FOR FLIGHT 2 */
#define RIMS_ID 3	/* 8/6/90 */

/* ----------------- Accelerometer Constants -------------------- */

#define XAK   1.5872808E-02     /* Accel Scale Factors */
#define YAK   1.5761033E-02     /* (ft/sec/count) */
#define ZAK   1.4833078E-02

#define XAB  -1.3168145E-01     /* Accel Biases (ft/sec/sec) */
#define YAB  -7.1956196E-02
#define ZAB   1.0566454E-01

#define XAZ       7.246E-04     /* Accel Misalignments (rad) */
#define XAY      -7.369E-04
#define YAZ       8.578E-04
#define YAX       8.010E-04
#define ZAY      -8.111E-04
#define ZAX       1.020E-03

/* ----------------- Gyro Constants ------------------------------ */

#define XGKP  3.2664141E-05     /* Gyro Scale Factors (rad/count) */
#define XRGKP 3.4539133E-05
#define YGKP  3.4690842E-05
#define ZGKP  3.4504520E-05
#define XGKN  3.2665651E-05
#define XRGKN 3.4537855E-05
#define YGKN  3.4690526E-05
#define ZGKN  3.4514454E-05

#define XGB   2.5110042E-04     /* Gyro Biases (rad/sec) */
#define XRGB -3.0309365E-04
#define YGB  -1.3515107E-04
#define ZGB  -1.8568251E-04

#define XGAX -8.5108723E-07     /* Gyro G Sensitivities */
#define YGAX  2.0164704E-07     /* (rad/sec/ft/sec/sec) */
#define ZGAX  3.6617681E-07     /* XR gyro data used for X terms */
#define XGAY -2.8635903E-07
#define YGAY -1.1413259E-06
#define ZGAY -4.3614754E-08
#define XGAZ -3.5191496E-07
#define YGAZ  3.2613442E-07
#define ZGAZ -8.2821761E-07

#define XGZ       1.306E-04     /* Gyro Misalignments (rad) */
#define XGY      -1.332E-03     /* XR gyro data used for X terms */
#define YGZ       8.931E-04
#define YGX       2.531E-03
#define ZGY      -9.466E-04
#define ZGX      -6.935E-04


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

#ifdef RIMS6		/* BACKUP UNIT FOR FLIGHT 3 */
#define RIMS_ID 6	/* 8/1/91 */

/* ----------------- Accelerometer Constants -------------------- */

#define XAK   1.4502490E-02     /* Accel Scale Factors */
#define YAK   1.4669333E-02     /* (ft/sec/count) */
#define ZAK   1.4267524E-02

#define XAB  -2.5643131E-02     /* Accel Biases (ft/sec/sec) */
#define YAB  -2.1588737E-01
#define ZAB  -2.2830474E-01

#define XAZ      -1.156E-04     /* Accel Misalignments (rad) */
#define XAY       5.785E-04
#define YAZ       2.618E-05
#define YAX      -1.049E-03
#define ZAY       4.779E-05
#define ZAX       5.135E-04

/* ----------------- Gyro Constants ------------------------------ */

#define XGKP  3.5871012E-05     /* Gyro Scale Factors (rad/count) */
#define XRGKP 3.1387721E-05
#define YGKP  3.5604508E-05
#define ZGKP  3.1681781E-05
#define XGKN  3.5863993E-05
#define XRGKN 3.1398745E-05
#define YGKN  3.5614446E-05
#define ZGKN  3.1693624E-05

#define XGB   5.0772481E-05     /* Gyro Biases (rad/sec) */
#define XRGB -4.4394944E-04
#define YGB  -2.5850776E-04
#define ZGB   5.8855974E-05

#define XGAX -9.5579987E-07     /* Gyro G Sensitivities */
#define YGAX  1.4173673E-06     /* (rad/sec/ft/sec/sec) */
#define ZGAX  1.1559102E-06     /* XR gyro data used for X terms */
#define XGAY -1.3616887E-06
#define YGAY -1.4532878E-06
#define ZGAY  6.7421612E-08
#define XGAZ -1.1302319E-06
#define YGAZ -3.4560991E-08
#define ZGAZ -9.8510434E-07

#define XGZ       7.333E-04     /* Gyro Misalignments (rad) */
#define XGY      -2.699E-04     /* XR gyro data used for X terms */
#define YGZ      -4.610E-04
#define YGX       9.872E-05
#define ZGY      -2.190E-04
#define ZGX      -1.864E-03

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

#ifdef RIMS8		/* PRIMARY UNIT FOR FLIGHT 3 */
#define RIMS_ID 8	/* 11/15/91 */

/* ----------------- Accelerometer Constants -------------------- */

#define XAK   1.3631382E-02     /* Accel Scale Factors */
#define YAK   1.4244335E-02     /* (ft/sec/count) */
#define ZAK   1.4506386E-02

#define XAB  -8.3847192E-02     /* Accel Biases (ft/sec/sec) */
#define YAB   2.5266073E-02
#define ZAB  -5.4233685E-03

#define XAZ       1.624E-04     /* Accel Misalignments (rad) */
#define XAY       5.588E-04
#define YAZ      -2.634E-04
#define YAX      -7.502E-04
#define ZAY      -3.855E-04
#define ZAX      -1.817E-03

/* ----------------- Gyro Constants ------------------------------ */

#define XGKP  2.8437994E-05     /* Gyro Scale Factors (rad/count) */
#define XRGKP 3.4870578E-05
#define YGKP  2.8715589E-05
#define ZGKP  3.2922207E-05
#define XGKN  2.8428516E-05
#define XRGKN 3.4871751E-05
#define YGKN  2.8720393E-05
#define ZGKN  3.2934271E-05

#define XGB   4.5911323E-06     /* Gyro Biases (rad/sec) */
#define XRGB  1.3541687E-04
#define YGB  -3.8518740E-04
#define ZGB  -1.9295700E-04

#define XGAX -1.2529536E-06     /* Gyro G Sensitivities */
#define YGAX  5.9066955E-07     /* (rad/sec/ft/sec/sec) */
#define ZGAX  2.1105292E-07     /* XR gyro data used for X terms */
#define XGAY -7.2840539E-08
#define YGAY -6.3818718E-07
#define ZGAY -7.4148599E-08
#define XGAZ -2.0556066E-07
#define YGAZ -9.2517975E-08
#define ZGAZ -1.2648951E-06

#define XGZ      -4.814E-04     /* Gyro Misalignments (rad) */
#define XGY      -3.903E-04     /* XR gyro data used for X terms */
#define YGZ       4.011E-04
#define YGX      -6.925E-04
#define ZGY      -9.610E-06
#define ZGX      -1.249E-03

/* -------------------------------------------------------------------	*/
/*		resolver sin and cos error model terms			*/
/* -------------------------------------------------------------------  */

#define R_SIN_NOM	 9.3909
#define R_SIN_OFF	 0.0025
#define R_SIN_BIAS	-0.0290

#define R_COS_NOM	 9.5030
#define R_COS_OFF	 0.0018
#define R_COS_BIAS	-0.0337

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
