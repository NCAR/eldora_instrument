/******************************************************************
function:  eris_tm()

Description:  Outputs telemetry format so proc e can send out message

inputs:  none

outputs:  480 byte telemetry buffer

notes:

called routines:  eris_subcom(), located at bottom of this file

Last Modified: 3-22-89
*******************************************************************/

#include "navvars.h"
#include "spin.h"
#include "align_v.h"
#include "ecinav_v.h"
#include "eris_tm_.h"

extern char fr1ptr;
extern double ea_psi, ea_theta, ea_phi;

static char      sf0[16];
static short int sf1[16], sf2[16], sf3[16];
static long int  sf4[16];
static float     sf5[16], sf6[16], sf7[16], sf8[16];
static float     sf9[16], sf10[16], sf11[16];

extern struct ERIS_TM_BUF eris_tm_buf;

void eris_tm()
{
  void eris_subcom();
  register struct ERIS_TM_BUF *etm;
  register struct nav_vars *n;
  register char ptr;
  register struct ecinav_vars *e;

  etm = &eris_tm_buf;
  n = (struct nav_vars *)NAV_start;
  e = &eci;

  ptr = fr1ptr;

  if (ptr == 0)
    eris_subcom();

/* format the 16 Hz buffer */

  etm->sync = 0xa0ad5799;           /* 0-3 */
  etm->rtime = n->rtime;            /* 4-7 */
  etm->talo = n->talo;              /* 8-11 */
  etm->tasep = n->tasep;            /* 12-15 */
  etm->frmptr = ptr;                /* 16 */

/* 1 Hz subframe allocations */

  etm->sf0_c1   = sf0[ptr];         /* 17 */
  etm->sf1_si1  = sf1[ptr];         /* 18-19 */
  etm->sf2_si2  = sf2[ptr];         /* 20-21 */
  etm->sf3_si3  = sf3[ptr];         /* 22-23 */
  etm->sf4_li1  = sf4[ptr];         /* 24-27 */
  etm->sf5_f1   = sf5[ptr];         /* 28-31 */
  etm->sf6_f2   = sf6[ptr];         /* 32-35 */
  etm->sf7_f3   = sf7[ptr];         /* 36-39 */
  etm->sf8_f4   = sf8[ptr];         /* 40-43 */
  etm->sf9_f5   = sf9[ptr];         /* 44-47 */
  etm->sf10_f6  = sf10[ptr];        /* 48-51 */
  etm->sf11_f7  = sf11[ptr];        /* 52-55 */

/* end of subframe allocations */

/* Continuation of 16 Hz Buffer */

  etm->lg = n->lg;                  /* 56-63 */
  etm->lng = n->lng;                /* 64-71 */
  etm->alt = (float) n->alt;        /* 72-75 */
  etm->vn = (float) n->vn;          /* 76-79 */
  etm->ve = (float) n->ve;          /* 80-83 */
  etm->vd = (float) n->vd;          /* 84-87 */
  etm->pxeci = (float) e->pxeci;    /* 88-91 */
  etm->pyeci = (float) e->pyeci;    /* 92-95 */
  etm->pzeci = (float) e->pzeci;    /* 96-99 */
  etm->vxeci = (float) e->vxeci;    /* 100-103 */
  etm->vyeci = (float) e->vyeci;    /* 104-107 */
  etm->vzeci = (float) e->vzeci;    /* 108-111 */
  etm->a11 = (float) n->a11;        /* 112-115 */
  etm->a12 = (float) n->a12;        /* 116-119 */
  etm->a13 = (float) n->a13;        /* 120-123 */
  etm->a21 = (float) n->a21;        /* 124-127 */
  etm->a22 = (float) n->a22;        /* 128-131 */
  etm->a23 = (float) n->a23;        /* 132-135 */
  etm->t11 = (float) n->t11;        /* 136-139 */
  etm->t12 = (float) n->t12;        /* 140-143 */
  etm->t13 = (float) n->t13;        /* 144-147 */
  etm->t21 = (float) n->t21;        /* 148-151 */
  etm->t22 = (float) n->t22;        /* 152-155 */
  etm->t23 = (float) n->t23;        /* 156-159 */
  etm->psi = (float) ea_psi;        /* 160-163 */
  etm->theta = (float) ea_theta;    /* 164-167 */
  etm->phi = (float) ea_phi;        /* 168-171 */
  etm->rmrd = n->rmrd;              /* 172-175 */
  etm->rmad0 = n->rmad[0];          /* 176-177 */
  etm->rmad1 = n->rmad[1];          /* 178-179 */
  etm->rmad2 = n->rmad[2];          /* 180-181 */
  etm->rmad9 = n->rmad[9];          /* 182-183 */
  etm->rmad10 = n->rmad[10];        /* 184-185 */
  etm->rmad11 = n->rmad[11];        /* 186-187 */
  etm->out1dis = n->out1dis;        /* 188-189 */
  etm->out2dis = (char) n->out2dis; /* 190-190 */
  etm->mode = (char) n->mode;       /* 191-191 */

/* Cold Gas ACS Variables */
  etm->spin_mode = (char) spin_mode;/* 192-192 */
  etm->tnum = (char) tnum;          /* 193-193 */
  etm->snum = n->snum;              /* 194-195 */
  etm->thn = (float) thn;           /* 196-199 */
  etm->psn = (float) psn;           /* 200-203 */
  etm->thh = (float) thh;           /* 204-207 */
  etm->thdh = (float) thdh;         /* 208-211 */
  etm->psh = (float) psh;           /* 212-215 */
  etm->psdh = (float) psdh;         /* 216-219 */
  etm->phh = (float) phh;           /* 220-223 */
  etm->w0 = (float) w0;             /* 224-227 */
  etm->ths = (float) ths;           /* 228-231*/
  etm->pss = (float) pss;           /* 232-235 */
  etm->phs = (float) phs;           /* 236-239 */
  etm->therrl_r = (float) therrl_r; /* 240-243 */
  etm->pserrl_r = (float) pserrl_r; /* 244-247 */
  etm->thd_r = (float) thd_r;       /* 248-251 */
  etm->psd_r = (float) psd_r;       /* 252-255 */
  etm->thdel_r = (float) thdel_r;   /* 256-259 */
  etm->psdel_r = (float) psdel_r;   /* 260-263 */
  etm->pherrl = (float) pherrl;     /* 264-267 */
  etm->phdel = (float) phdel;       /* 268-271 */
  etm->ps_err_pz = (float) ps_err_pz; /* 272-275 */
  etm->th_err_pz = (float) th_err_pz; /* 276-279 */
  etm->despin_seq = despin_seq;     /* 280-281 */
  etm->enabl_th = enabl_th;         /* 282-283 */
  etm->enabl_ps = enabl_ps;         /* 284-285 */
  etm->nppos_x = nppos_x;           /* 286-287 */
  etm->nppos_y = nppos_y;           /* 288-289 */
  etm->nppos_z = nppos_z;           /* 290-291 */
  etm->npneg_x = npneg_x;           /* 292-293 */
  etm->npneg_y = npneg_y;           /* 294-295 */
  etm->npneg_z = npneg_z;           /* 296-297 */
  etm->ixrg_16 = n->ixrg_16;        /* 298-299 */
  etm->ixg_16 = n->ixg_16;          /* 300-301 */
  etm->iyg_16 = n->iyg_16;          /* 302-303 */
  etm->izg_16 = n->izg_16;          /* 304-305 */
  etm->ixa_16 = n->ixa_16;          /* 306-307 */
  etm->iya_16 = n->iya_16;          /* 308-309 */
  etm->iza_16 = n->iza_16;          /* 310-311 */
  etm->ton_x_eq = (float) ton_x_eq; /* 312-315 */
  etm->dthcen = (float) dthcen;     /* 316-319 */
  etm->dpscen = (float) dpscen;     /* 320-323 */
/* End of Cold Gas ACS Variables */

/* Alignment Mode Variables */
  etm->dthn = (float) n->dthn;      /* 324-327 */
  etm->dthe = (float) n->dthe;      /* 328-331 */
  etm->dthd = (float) n->dthd;      /* 332-335 */
  etm->tdxgb = n->tdxgb;            /* 336-339 */
  etm->tdygb = n->tdygb;            /* 340-343 */
  etm->tdzgb = n->tdzgb;            /* 344-347 */
  etm->p11 = (float) p[1][1];       /* 348-351 */
  etm->p22 = (float) p[2][2];       /* 352-355 */
  etm->p33 = (float) p[3][3];       /* 356-359 */
  etm->p44 = (float) p[4][4];       /* 360-363 */
  etm->p55 = (float) p[5][5];       /* 364-367 */
  etm->p66 = (float) p[6][6];       /* 368-371 */
  etm->p77 = (float) p[7][7];       /* 372-375 */
  etm->p88 = (float) p[8][8];       /* 376-379 */
  etm->p99 = (float) p[9][9];       /* 380-383 */
  etm->q11 = (float) q[1][1];       /* 384-387 */
  etm->q22 = (float) q[2][2];       /* 388-391 */
  etm->q33 = (float) q[3][3];       /* 392-395 */
  etm->q44 = (float) q[4][4];       /* 396-399 */
  etm->q55 = (float) q[5][5];       /* 400-403 */
  etm->q66 = (float) q[6][6];       /* 404-407 */
  etm->q77 = (float) q[7][7];       /* 408-411 */
  etm->q88 = (float) q[8][8];       /* 412-415 */
  etm->q99 = (float) q[9][9];       /* 416-419 */
/*   End of Alignment Mode Variables */

/*   Fixed Point Variables */
  etm->w0_int = (short int) (w0 * FACT_1);      /* 420-421 */
  etm->thdh_int = (short int) (thdh * FACT_2);  /* 422-423 */
  etm->psdh_int = (short int) (psdh * FACT_2);  /* 424-425 */
  etm->ths_int = (short int) (ths * FACT_1);    /* 426-427 */
  etm->pss_int = (short int) (pss * FACT_1);    /* 428-429 */
  etm->ton_int = (short int) (ton_x_eq * FACT_3); /* 430-431 */
  etm->rmrd_int = (short int) (n->rmrd * FACT_4); /* 432-433 */
  etm->fill = (short int) NA;			/* 434-435 */

/*   Body to eci transformation matrix */
  etm->eb11 = (float) e->eb11;			/* 436-439 */
  etm->eb12 = (float) e->eb12;			/* 440-443 */
  etm->eb13 = (float) e->eb13;			/* 444-447 */
  etm->eb21 = (float) e->eb21;			/* 448-451 */
  etm->eb22 = (float) e->eb22;			/* 452-455 */
  etm->eb23 = (float) e->eb23;			/* 456-459 */
  etm->eb31 = (float) e->eb31;			/* 460-463 */
  etm->eb32 = (float) e->eb32;			/* 464-467 */
  etm->eb33 = (float) e->eb33;			/* 468-471 */
  etm->w0h =  (float) w0h;			/* 472-475 */
  fr1ptr++;
  fr1ptr %= 16;
} /* End of eris_tm() */


void
eris_subcom()
{
  register struct nav_vars *n;
  register char *csfp;        /* character subframe pointer */
  register short *ssfp;       /* short integer subframe pointer */
  register long *lsfp;        /* long integer subframe pointer */
  register float *fsfp;       /* float subframe pointer */
  register double *p_dbl;

  static long int lastxrg,lastxg,lastyg,lastzg,lastxa,lastya,lastza;
  long int        currxrg,currxg,curryg,currzg,currxa,currya,currza;

  extern char _evne, _evnf, _tece, _tecf;
  extern short vrtx_erre, vrtx_errf, res_counte, res_countf;
  extern short crcev, crcea, crcfv, crcfa;
  extern long int _epce, _epcf, _evtae, _evtaf, seq_test, ready;

  n=(struct nav_vars *) NAV_start;

/* subframe 0, character data, processor flags and status */
  csfp = sf0;
  csfp[0] = (char)_evne;
  csfp[1] = (char)_tece;
  csfp[2] = (char)vrtx_erre;
  csfp[3] = (char)res_counte;
  csfp[4] = (char)_evnf;
  csfp[5] = (char)_tecf;
  csfp[6] = (char)vrtx_errf;
  csfp[7] = (char)res_countf;
  csfp[8] = (char)n->lnchdet;
  csfp[9] = (char)n->sepdet;
  csfp[10] = (char)irot;
  csfp[11] = (char)inotch;
  csfp[12] = (char)NA;
  csfp[13] = (char)NA;
  csfp[14] = (char)NA;
  csfp[15] = (char)NA;

/* subframe 1, CRC values & RIMs Status */
  ssfp = sf1;
  ssfp[0] = crcev;
  ssfp[1] = crcea;
  ssfp[2] = crcfv;
  ssfp[3] = crcfa;
  ssfp[4] = (short int)NA;
  ssfp[5] = (short int)NA;
  ssfp[6] = n->rmstat;
  ssfp[7] = n->rimscmd;
  ssfp[8] = n->indis;
  ssfp[9] = n->rimsup;
  ssfp[10] = n->rimsstr;
  ssfp[11] = (short int)NA;
  ssfp[12] = (short int)NA;
  ssfp[13] = (short int)NA;
  ssfp[14] = (short int)NA;
  ssfp[15] = (short int)NA;

/* subframe 2, Alignment Parameters, & Cold Gas Parameters */
  ssfp = sf2;
  ssfp[0] = (short int) t_align;
  ssfp[1] = (short int) t_up;
  ssfp[2] = (short int) t_up_1;
  ssfp[3] = (short int) a_mode;
  ssfp[4] = (short int) propagate;
  ssfp[5] = (short int) update;
  ssfp[6] = (short int) NA;
  ssfp[7] = (short int) NA;
  ssfp[8] = adjust_delay;
  ssfp[9] = m2count_max;
  ssfp[10] = m2quiet_max;
  ssfp[11] = m4count_max;
  ssfp[12] = m4quiet_max;
  ssfp[13] = abtest;
  ssfp[14] = bitest;
  ssfp[15] = seq_test;

/* subframe 3, One second accumulations of RIMs Gyro & Accel Counts
               & euler_seq */
  ssfp = sf3;

  currxrg = n->ixrgtot;
  currxg  = n->ixgtot;
  curryg  = n->iygtot;
  currzg  = n->izgtot;
  currxa  = n->ixatot;
  currya  = n->iyatot;
  currza  = n->izatot;

  ssfp[0] = (short int) (currxrg - lastxrg);
  ssfp[1] = (short int) (currxg - lastxg);
  ssfp[2] = (short int) (curryg - lastyg);
  ssfp[3] = (short int) (currzg - lastzg);
  ssfp[4] = (short int) (currxa - lastxa);
  ssfp[5] = (short int) (currya - lastya);
  ssfp[6] = (short int) (currza - lastza);
  ssfp[7] = (short int) NA;
  ssfp[8] = (short int) NA;
  ssfp[9] = (short int) NA;
  ssfp[10] = (short int) NA;
  ssfp[11] = (short int) NA;
  ssfp[12] = (short int) NA;
  ssfp[13] = (short int) NA;
  ssfp[14] = (short int) NA;
  ssfp[15] = (short int) euler_seq;

  lastxrg = currxrg;
  lastxg = currxg;
  lastyg = curryg;
  lastzg = currzg;
  lastxa = currxa;
  lastya = currya;
  lastza = currza;

/* subframe 4, Exception PC, VBR addresses & CG ACS Times */
  lsfp = sf4;
  lsfp[0] = _epce;
  lsfp[1] = _epcf;
  lsfp[2] = _evtae;
  lsfp[3] = _evtaf;
  lsfp[4] = n->nsec;
  lsfp[5] = ready;
  lsfp[6] = rate_check;
  lsfp[7] = (long int) NA;
  lsfp[8] = (long int) NA;
  lsfp[9] = (long int) NA;
  lsfp[10] = (long int) NA;
  lsfp[11] = (long int) NA;
  lsfp[12] = (long int) NA;
  lsfp[13] = (long int) NA;
  lsfp[14] = (long int) NA;
  lsfp[15] = (long int) NA;

/* subframe 5, RIMs Platform Temperatures & Gyro Scale Factors */
  fsfp = sf5;
  fsfp[0] = n->tmpstab;
  fsfp[1] = n->tmpxy;
  fsfp[2] = n->tmpxrz;
  fsfp[3] = n->tmpxa;
  fsfp[4] = n->tmpya;
  fsfp[5] = n->tmpza;
  fsfp[6] = n->tmpgaf;
  fsfp[7] = n->tmpaaf;
  fsfp[8] = n->rtxgkp;
  fsfp[9] = n->rtxrgkp;
  fsfp[10] = n->rtygkp;
  fsfp[11] = n->rtzgkp;
  fsfp[12] = n->rtxgkn;
  fsfp[13] = n->rtxrgkn;
  fsfp[14] = n->rtygkn;
  fsfp[15] = n->rtzgkn;

/* subframe 6, Accelerometer factors, Gravity Terms, & Cold Gas ACS */
  fsfp = sf6;
  fsfp[0] = n->xab;
  fsfp[1] = n->yab;
  fsfp[2] = n->zab;
  fsfp[3] = n->rtxak;
  fsfp[4] = n->rtyak;
  fsfp[5] = n->rtzak;
  fsfp[6] = (float) n->gdp;
  fsfp[7] = (float) n->gnp;
  fsfp[8] = (float) b1;
  fsfp[9] = (float) b2;
  fsfp[10] = (float) sa0;
  fsfp[11] = (float) sa1;
  fsfp[12] = (float) sa2;
  fsfp[13] = (float) rdb_x;
  fsfp[14] = (float) pdb_x;
  fsfp[15] = (float) rdb_yz;

/* subframe 7, Cold Gas ACS Parameters */
  p_dbl = &te11;
  fsfp = sf7;
  fsfp[0] = (float) pdb_yz;
  fsfp[1] = (float) rlim_x;
  fsfp[2] = (float) plim_x;
  fsfp[3] = (float) rlim_yz;
  fsfp[4] = (float) plim_yz;
  fsfp[5] = (float) rlim_yz0;
  fsfp[6] = (float) inn;
  fsfp[7] = (float) ixx;
  fsfp[8] = (float) arm_x;
  fsfp[9] = (float) arm_y;
  fsfp[10] = (float) arm_z;
  fsfp[11] = (float) thr_lo;
  fsfp[12] = (float) thr_hi;
  fsfp[13] = (float) w0_spin;
  fsfp[14] = (float) ph_deploy;
  fsfp[15] = (float) p_dbl[0];    /* te11 */

/* subframe 8, Cold Gas ACS Parameters */
  fsfp = sf8;
  fsfp[0] = (float) p_dbl[1];    /* te12 */
  fsfp[1] = (float) p_dbl[2];    /* te13 */
  fsfp[2] = (float) p_dbl[3];    /* te21 */
  fsfp[3] = (float) p_dbl[4];    /* te22 */
  fsfp[4] = (float) p_dbl[5];    /* te23 */
  fsfp[5] = (float) p_dbl[6];    /* te31 */
  fsfp[6] = (float) p_dbl[7];    /* te32 */
  fsfp[7] = (float) p_dbl[8];    /* te33 */
  fsfp[8] = (float) tqfac;
  fsfp[9] = (float) tdelay;
  fsfp[10] = (float) rfac_x;
  fsfp[11] = (float) rdbm_x;
  fsfp[12] = (float) pfac_x;
  fsfp[13] = (float) pdb_x0;
  fsfp[14] = (float) rfac_y;
  fsfp[15] = (float) rdbm_y;

/* subframe 9, Cold Gas ACS Parameters */
  fsfp = sf9;
  fsfp[0] = (float) pfac_y;
  fsfp[1] = (float) pfac_y0;
  fsfp[2] = (float) pdb_y0;
  fsfp[3] = (float) rlfac_x;
  fsfp[4] = (float) rlfac_y;
  fsfp[5] = (float) rlfac_y0;
  fsfp[6] = (float) m2fac;
  fsfp[7] = (float) m4fac;
  fsfp[8] = (float) m4tol_x;
  fsfp[9] = (float) m4tol_y;
  fsfp[10] = (float) s_alf;
  fsfp[11] = (float) s_wn;
  fsfp[12] = (float) krp_x;
  fsfp[13] = (float) krp_yz;
  fsfp[14] = (float) mfac_x;
  fsfp[15] = (float) mfac_yz;

/* subframe 10, Alignment Parameters & gyro biases */
  fsfp = sf10;
  fsfp[0] = n->xgb;
  fsfp[1] = n->ygb;
  fsfp[2] = n->zgb;
  fsfp[3] = (float) NA;
  fsfp[4] = (float) spin_time;
  fsfp[5] = (float) NA;
  fsfp[6] = (float) dvn;
  fsfp[7] = (float) dve;
  fsfp[8] = n->salf;
  fsfp[9] = n->calf;
  fsfp[10] = (float) r[1][1];
  fsfp[11] = (float) r[1][2];
  fsfp[12] = (float) r[2][1];
  fsfp[13] = (float) r[2][2];
  fsfp[14] = (float) kal_gn[1][1];
  fsfp[15] = (float) kal_gn[1][2];

/* subframe 11, Alignment Parameters */
  fsfp = sf11;
  fsfp[0] = (float) kal_gn[2][1];
  fsfp[1] = (float) kal_gn[2][2];
  fsfp[2] = (float) kal_gn[3][1];
  fsfp[3] = (float) kal_gn[3][2];
  fsfp[4] = (float) kal_gn[4][1];
  fsfp[5] = (float) kal_gn[4][2];
  fsfp[6] = (float) kal_gn[5][1];
  fsfp[7] = (float) kal_gn[5][2];
  fsfp[8] = (float) kal_gn[6][1];
  fsfp[9] = (float) kal_gn[6][2];
  fsfp[10] = (float) kal_gn[7][1];
  fsfp[11] = (float) kal_gn[7][2];
  fsfp[12] = (float) kal_gn[8][1];
  fsfp[13] = (float) kal_gn[8][2];
  fsfp[14] = (float) kal_gn[9][1];
  fsfp[15] = (float) kal_gn[9][2];
}
