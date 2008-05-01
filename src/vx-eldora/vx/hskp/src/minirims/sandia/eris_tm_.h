/******************************************************************
function:  eris_tmbuf.h

Description:  Header file describing the telemetry output buffer

inputs:	

outputs:

notes:

called routines:

Last Modified:
*******************************************************************/
#define  NA	       0x4e
#define  TBD	       0x0
#define  FACT_1        9387.054          /* 32767/(200*pi/180) */
#define  FACT_2        93870.54          /* 32767/(20*pi/180) */
#define  FACT_3        65.534            /* 32767/500 */
#define  FACT_4        5215.03           /* 32767/(360*pi/180) */
struct ERIS_TM_BUF {
      long int sync;    /* sync word */            /* 0-3 */
      long int rtime;   /* rims time */            /* 4-7 */
      long int talo;    /* time after lift off */  /* 8-11 */
      long int tasep;   /* time after seperation */ /* 12-15 */
      char frmptr;      /* subframe frame pointer */ /* 16-16 */

      /* subframe allocations */
      char sf0_c1;                      /* 17-17 */
      short int sf1_si1;                /* 18-19 */
      short int sf2_si2;                /* 20-21 */
      short int sf3_si3;                /* 22-23 */
      long int sf4_li1;                 /* 24-27 */
      float sf5_f1;                     /* 28-31 */
      float sf6_f2;                     /* 32-35 */
      float sf7_f3;                     /* 36-39 */
      float sf8_f4;                     /* 40-43 */
      float sf9_f5;                     /* 44-47 */
      float sf10_f6;                    /* 48-51 */
      float sf11_f7;                    /* 52-55 */
/* End of subframe allocations */

/* Navigation Variables */
      double lg;                       /* 56-63 */
      double lng;                      /* 64-71 */
      float alt;                       /* 72-75 */
      float vn;                        /* 76-79 */
      float ve;
      float vd;
      float pxeci;
      float pyeci;
      float pzeci;
      float vxeci;
      float vyeci;
      float vzeci;
      float a11;
      float a12;
      float a13;
      float a21;
      float a22;
      float a23;
      float t11;              /* 136-139 */
      float t12;
      float t13;
      float t21;
      float t22;
      float t23;
      float psi;
      float theta;
      float phi;
      float rmrd;
      short int rmad0;
      short int rmad1;
      short int rmad2;
      short int rmad9;
      short int rmad10;
      short int rmad11;
      short int out1dis;
      char out2dis;
      char mode;
/* End of Navigation variables */

/* Cold Gas ACS variables */
      char spin_mode;
      char tnum;
      short int snum;
      float thn;
      float psn;
      float thh;
      float thdh;
      float psh;
      float psdh;
      float phh;
      float w0;
      float ths;
      float pss;
      float phs;
      float therrl_r;
      float pserrl_r;
      float thd_r;
      float psd_r;
      float thdel_r;
      float psdel_r;
      float pherrl;
      float phdel;               /* 268-271 */
      float ps_err_pz;           /* 272-275 */
      float th_err_pz;           /* 276-279 */
      short int despin_seq;      /* 280-281 */
      short int enabl_th;        /* 282-283 */
      short int enabl_ps;        /* 284-285 */
      short int nppos_x;         /* 286-287 */
      short int nppos_y;         /* 288-289 */
      short int nppos_z;         /* 290-291 */
      short int npneg_x;         /* 292-293 */
      short int npneg_y;         /* 294-295 */
      short int npneg_z;         /* 296-297 */
      short int ixrg_16;         /* 298-299 */
      short int ixg_16;          /* 300-301 */
      short int iyg_16;          /* 302-303 */
      short int izg_16;          /* 304-305 */
      short int ixa_16;          /* 306-307 */
      short int iya_16;          /* 308-309 */
      short int iza_16;          /* 310-311 */
      float ton_x_eq;            /* 312-315 */
      float dthcen;              /* 316-319 */
      float dpscen;              /* 320-323 */
/* End of navigation variables */

/* Alignment mode variables    */
      float dthn;                /* 324-327 */
      float dthe;                /* 328-331 */
      float dthd;                /* 332-335 */
      float tdxgb;               /* 336-339 */
      float tdygb;               /* 340-343 */
      float tdzgb;               /* 344-347 */
/* Error Covariance Diagonal    */
      float p11;                 /* 348-351 */
      float p22;                 /* 352-355 */
      float p33;                 /* 356-359 */
      float p44;                 /* 360-363 */
      float p55;                 /* 364-367 */
      float p66;                 /* 368-371 */
      float p77;                 /* 372-375 */
      float p88;                 /* 376-379 */
      float p99;                 /* 380-383 */
/* State Process noise Covariance Diagonal */
      float q11;                 /* 384-387 */
      float q22;                 /* 388-391 */
      float q33;                 /* 392-395 */
      float q44;                 /* 396-399 */
      float q55;                 /* 400-403 */
      float q66;                 /* 404-407 */
      float q77;                 /* 408-411 */
      float q88;                 /* 412-415 */
      float q99;                 /* 416-419 */
/* End of Alignment Mode Variables */

/* Fixed Point Variables */
      short int w0_int;          /* 420-421 */
      short int thdh_int;        /* 422-423 */
      short int psdh_int;        /* 424-425 */
      short int ths_int;         /* 426-427 */
      short int pss_int;         /* 428-429 */
      short int ton_int;         /* 430-431 */
      short int rmrd_int;        /* 432-433 */
      short int fill;		 /* 434-435 */
/* End of Fixed Point Variables */

/* body to eci transformation matrix */
      float eb11;		/* 436-439 */
      float eb12;		/* 440-443 */
      float eb13;		/* 444-447 */
      float eb21;		/* 448-451 */
      float eb22;		/* 452-455 */
      float eb23;		/* 456-459 */
      float eb31;		/* 460-463 */
      float eb32;               /* 464-467 */
      float eb33;		/* 468-471 */

      float w0h;		/* 472-475 */
      char spare[4];  /* 4 bytes of spare space, 476-479 */

      };
/* End of Telemetry Message */
