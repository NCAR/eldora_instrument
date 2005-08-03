#ifdef PERIODOGRAM
.global _fftc

/************************************************************/
/* Routine source file: _fftc.asm                           */
/* DSP32/DSP32C Application Software Library. Version 2.1   */
/************************************************************/

/*
                _fftc (A,B,C,D)
        const   int   A, B;
                int  *C, *D;        
        scratch register short  r1 to r19;
        scratch register float  a0,a1,a2,a3;
*/
/*#include <dspregs.h>*/ /* Translates DSP32C keywords to DSP32 *
                      *   keywords.  See Section 2.3.1      */ 


_fftc:  r10e = *r14++             /* N */
        r12e = *r14++             /* M */
        r2e = *r14++              /* points to _fftR */
        r17e = *r14--             /* points to _fftI */
        r6e = *r14                /* points to _fftR */
        r18e = r10 * 2            /* N * 2 */

/* In place bit reversal ----------------------------- */
        r17e = r17 - r2
        r16e = -r17
        r1e = r2                /* points to _fftR */

#if DSP32C
        r8e = r10 - 3           /* NM3 = N -3 */
        r3 = 0
        r4 = 0

_fcbitA:r3 - r4
        if (ge) pcgoto _fcbitB
        r5e = r1                    
                                     /* Begin complex exchange */
        *r1++r17 = a0 = *r2++r17     /* _fftR(i) = _fftR(j) */
        *r1++r16 = a0 = *r2++r16     /* _fftI(i) = _fftI(j) */
        *r2++r17 = a0 = *r5++r17     /* _fftR(j) = _fftR(i) */
        *r2++r16 = a0 = *r5++r16     /* _fftI(j) = _fftI(i) */

_fcbitB:r1e = r1 + 4
        r3 = r3 + 4
        r4 = r4#r18
        if (r8-- >=0) pcgoto _fcbitA
        r2e = r4 + r6

#else
        r5 = r10 - 3                     /* NM3 = N -3 */

_fftcA: r1 - r2 
        if (ge) goto _fftc5
        r4e = r1                     /* temp. pointer for i */
                                     /* Begin complex exchange */
        *r1++r17 = a0 = *r2++r17     /* _fftR(i) = _fftR(j) */
        *r1++r16 = a0 = *r2++r16     /* _fftI(i) = _fftI(j) */
        *r2++r17 = a0 = *r4++r17     /* _fftR(j) = _fftR(i) */
        *r2++r16 = a0 = *r4++r16     /* _fftI(j) = _fftI(i) */

_fftc5: r3e = r18                         /* N2 = 2 * N */
        r2e = r2-r6 
_fftc6: r3 - r2        
        if (gt) goto _fftc7   
        nop

        r2e = r2 - r3
        goto _fftc6
        r3e = r3/2

_fftc7: r2e = r2 + r6
        r2e = r2 + r3
        if (r5-- >= 0) goto _fftcA
        r1e = r1 + 4
#endif

/* Begin FFT calculation ----------------------------- */

        r18e = r18 * 2                /* 4N */
        r18e = r18 - 4                /* 4N - 4 */

/* Stage 1 and 2 use N/4 radix-4 butterflies */

        r1e = *r14        /* address of _fftR */
        r10 = r10/2       /* N/2 */
        r10 = r10/2       /* N/4 */
        r2e = r1 + 4      /* data item B */
        r3e = r1 + 8      /* data item C */
        r4e = r1 + 12     /* data item D */

        r5e = r1        /* pointer for new A (A') */
        r6e = r2        /* B' */
        r7e = r3        /* C' */
        r8e = r4        /* D' */
        
        r15e = r16 + 16    /* increment for next group of four */
#if DSP32C
        r11 = r10-1        /* loop counter for radix-4 */
        do 15, r11
#else
        r11 = r10-2        /* loop counter for radix-4 */
#endif

_fftcC:
   a0 = *r1++r17 + *r2++r17      /* ReA + ReB */
   a1 = *r3++r17 + *r4++r17      /* ReC + ReD */
   a2 = *r1++r16 + *r2++r16      /* ImA + ImB */
   a3 = *r3++r16 + *r4++r16      /* ImC + ImD */

  *r5++r17 = a0 = a0 + a1        /* ReA' = ReA+ReB+ReC+ReD */
   a0 = *r1++r17 - *r2++r17      /* ReA - ReB */
  *r7++r17 = a1 = -a1 + a0       /* ReC' = -(ReC+ReD)+(ReA+ReB) */
  *r5++r15 = a2 = a2 + a3        /* ImA' = ImA+ImB+ImC+ImD */ 
   a2 = *r3++r17 - *r4++r17      /* ReC - ReD */
  *r7++r15 = a3 = -a3 + a2       /* ImC' = -(ImC+ImD)+(ImA+ImB) */
   a3 = *r1++r15 - *r2++r15      /* ImA - ImB */
   a1 = *r3++r15 - *r4++r15      /* ImC - ImD */
                
   *r6++r17 = a0 = a1 + a0       /* ReB' =  (ImC-ImD)+(ReA-ReB) */
   *r8++r17 = a1 = -a1 + a0      /* ReD' = -(ImC-ImD)+(ReA-ReB) */
   *r6++r15 = a3 = -a2 + a3      /* ImB' = -(ReC-ReD)+(ImA-ImB) */
#if DSP32
    if (r11-- >=0) goto _fftcC
#endif
   *r8++r15 = a2 = a2 + a3       /* ImD' = (ReC-ReD)+(ImA-ImB) */

/* Stage 3 to M-1 --------------------------------- */

        r9 = 8
        r7 = r12 - 5           /* M - 5 */
        r8 = 4
        r4e = _fftcW
        r6e = _fftctwo         /* points to 2.0 */

_fftc30:r10e = r10/2
        r9e = r9*2
        r15e = r9*2
        r15e = r15-r17
#if DSP32C
        r19e = r15 - r18
#else
        r19e = r15
        r19e = r19 - r18
#endif
        r2e = *r14           /* _fftR */
                             /* initialize twiddle factors */
        a2 = *r4++           /* Ur = a2 */
        a3 = *r4--           /* Ui = a3 */
        r13 = r8 - 3         /* loop count */
        r8 = r8*2

        r1e = r2
        r11e = r1

#if DSP32C
        r3e = r1 + r9
        r12e = r3 
        r5e = r10 - 2
        do 3, r5
#else
        r3e = r1
        r3e = r3 + r9
        r12e = r3 
        r5e = r10 - 3
#endif

/* Butterfly # 1 ----------------------------------------- */
_fftc10d:
        *r11++r17=a0=*r1 + *r3++r17         /* RE1 + RE2 */
        *r12++r17=a0=-a0 + *r1++r17 * *r6   /* RE1 - RE2 */
        *r11++r15=a0=*r1 + *r3++r15         /* IM1 + IM2 */
#if DSP32
        if (r5-- >= 0) goto _fftc10d
#endif
        *r12++r15=a0= -a0 + *r1++r15 * *r6  /* IM1 - IM2 */

        *r11++r17=a0=*r1 + *r3++r17         /* RE1 + RE2 */
        *r12++r17=a0=-a0 + *r1++r17 * *r6   /* RE1 - RE2 */
        *r11++r19=a0=*r1 + *r3++r19         /* IM1 + IM2 */
        *r12++r19=a0=-a0 + *r1++r19 * *r6   /* IM1 - IM2 */

/* Butterfly # 2 ----------------------------------------- */

_fftc20: 
#if DSP32C
         do 5, r5
#else
         r5e = r10 - 3
#endif
_fftc10:
    a0 = *r1++r17 + a2 * *r3++r17     /* a0=Re(i)+Ur*Re(k) */
   *r11++r17=a0=a0 - *r3++r16 * a3    /* Re(i)=a0=a0-Ui*Im(k) */
    a1 = *r1++r16 + a3 * *r3++r17     /* a1=Im(i)+Ui*Re(k) */
   *r12++r17=a0=-a0+ *r1++r17 * *r6   /* Re(k)=-a0+2*Re(i) */
   *r11++r15=a1=a1+ *r3++r15 * a2     /* Im(i)=a1=a1+Ur*Im(k) */
#if DSP32
    if (r5-- >= 0) goto _fftc10
#endif
   *r12++r15=a1=-a1+ *r1++r15 * *r6   /* Im(k)=-a1+2*Im(i) */

    a0 = *r1++r17 + a2 * *r3++r17     /* a0=Re(i)+Ur*Re(k) */
   *r11++r17=a0=a0 - *r3++r16 * a3    /* Re(i)=a0=a0-Ui*Im(k) */
    a1 = *r1++r16 + a3 * *r3++r17     /* a1=Im(i)+Ui*Re(k) */
   *r12++r17=a0=-a0+ *r1++r17 * *r6   /* Re(k)=-a0+2*Re(i) */
   *r11++r19=a1=a1+ *r3++r19 * a2     /* Im(i)=a1=a1+Ur*Im(k) */
   *r12++r19=a1=-a1+ *r1++r19 * *r6   /* Im(k)=-a1+2*Im(i) */

        a0 = a2 * *r4++          /* Compute new twiddle */
        a2 = a0 - a3 * *r4       /* U = U * _fftcW */
        a0 = a2 * *r4--
        a3 = a0 + a3 * *r4
        if (r13-- >= 0) pcgoto _fftc20
        r2e = r2 + 4

        if (r7-- >= 0) pcgoto _fftc30
        r4e = r4 + 8

/* Stage M -------------------------------------------- */
        r9 = r9*2
        r1e = *r14             /* _fftR */
        r13e = _fftctwo + 4    /* points to 1.0 */
        a2 = *r13++            /* Ur = a2 = 1.0 */
        a3 = *r13--            /* Ui = a3 = 0.0 */
        r11e = r1
        r15e = r16 + 4

#if DSP32C
        r3e = r1 + r9
        r12e = r3
        r13 = r8 - 1
        do 10, r13
#else
        r3e = r1
        r3e = r3 + r9
        r12e = r3
        r13 = r8 - 2
#endif

_fftc20A:
    a0 = *r1++r17 + a2 * *r3++r17     /* a0=Re(i)+Ur*Re(k) */
    nop
   *r11++r17=a0=a0 - *r3++r16 * a3    /* Re(i)=a0=a0-Ui*Im(k) */
    a1 = *r1++r16 + a3 * *r3++r17     /* a1=Im(i)+Ui*Re(k) */
   *r12++r17=a0=-a0+ *r1++r17 * *r6   /* Re(k)=-a0+2*Re(i) */
   *r11++r15=a1=a1+ *r3++r15 * a2     /* Im(i)=a1=a1+Ur*Im(k) */
   *r12++r15=a1=-a1+ *r1++r15 * *r6   /* Im(k)=-a1+2*Im(i) */
    a0 = a2 * *r4++                   /* Compute new twiddle */
    a2 = a0 - a3 * *r4                /*   U = U * _fftcW    */
    a0 = a2 * *r4--
#if DSP32
    if (r13 -- >= 0) goto _fftc20A
#endif
    a3 = a0 + a3 * *r4

/* --------------------------------------------------- */

#if DSP32C
        goto r14+8
#else
        goto r14+4
#endif
        nop

/* Constant table ------------------------------------ */

_fftctwo: float 2.0, 1.0, 0.0

/* Twiddle table ------------------------------------- */

_fftcW:
  float  0.7071068, -0.7071068    /* cos(pi/4),-sin(pi/4)  */
  float  0.9238795, -0.3826834    /* cos(pi/8),-sin(pi/8)  */
  float  0.9807853, -0.1950903    /* cos(pi/16),-sin(pi/16)*/
  float  0.9951847, -0.0980171    /* cos(pi/32),-sin(pi/32)*/
  float  0.9987955, -4.9067674e-2 /* cos and -sin(pi/64)   */
  float  0.9996988, -2.4541228e-2 /* cos and -sin(pi/128)  */
  float  0.9999247, -1.2271538e-2 /* cos and -sin(pi/256)  */
  float  0.9999812, -6.1358846e-3 /* cos and -sin(pi/512)  */

  float 0.9999952938, -3.067956691e-3 /* cos and -sin(pi/1024) */
  float 0.9999988234, -1.533980094e-3 /* cos and -sin(pi/2048) */

.global _hann0
/************************************************************/
/* Routine source file: _hann0.asm                          */
/* DSP32/DSP32C Application Software Library. Version 2.1   */
/************************************************************/

/*
        _hann0(A,B,C,D)
        int A, B;
        float *C;
   scratch register short r1,r6;
   scratch pointer register short r2,r3,r4,r5,r14;
   scratch register float a0,a1,a2,a3;
*/
/*#include <dspregs.h>*/ /* Translates DSP32C keywords to DSP32 *
                      *   keywords.  See Section 2.3.1      */ 

_hann0: r1e = *r14++       /* dimension of array, N  */
        r6e = *r14++       /* M  where 2^M = N */
        r4e = *r14         /* points to top of array data */

        r5 = r1 * 2
        r5 = r5 * 2        /* 4*N */
        r5 = r5 - 4        /* 4*(N-1) */
        r5e = r5 + r4      /* pointer to end of array */
        r1 = r1/2
        r1 = r1 - 2        /* used as loop counter */

        r3e = _hann0C      /* pointer to constant 0.5 */
                      /* initialization  of a2 and a3 for n=0 */
        a2 = -*r3         /* a2 = -0.5*cos[2*pi*n/(N-1)]=-0.5 */
        a3 = a2 + *r3     /* a3 = -0.5*sin[2*pi*n/(N-1)]= 0.0 */
        a1 = a3           /* first window value = 0.0 */

        r6 = r6 * 2
        r6 = r6 * 2
        r6 = r6 * 2       /* r6 = 8*M */
                               /* pointer to cos[2*pi/(N-1)] and 
                                  sin[2*pi/(N-1)] table */
#if DSP32C
        r2e = r3 + r6
#else
        r2e = r6 + _hann0C
#endif

_hann0A: *r4++ = a0 = a1 * *r4 /* store (data * window) value */
         *r5-- = a0 = a1 * *r5 /* store (data * window) value */

        a0 = a2 * *r2++        /* a2 * cos[2*pi/(N-1)] */
        a2 = a0 - a3 * *r2     /* updated a2 for (n+1) */
        a0 = a2 * *r2--        /* a2 * sin[2*pi/(N-1)] */
        a3 = a0 + a3 * *r2     /* updated a3 for (n+1) */
        a1 = a2 + *r3          /* updated window value */

        if(r1-- >= 0) pcgoto _hann0A
        nop

        goto r14+4
        nop

_hann0C: float 0.5, 0.0
   /*  Table values for cos[2*pi/(N-1)], sin[2*pi/(N-1)] */
        float 1.0, 0.                         /* N = 2 */
        float -0.5, 0.8660254042              /* N = 4 */
        float 0.6234898019, 0.7818314824      /* N = 8 */
        float 0.9135454575, 0.4067366433      /* N = 16 */
        float 0.9795299412, 0.20129852        /* N = 32 */
        float 0.9950307753, 0.0995678465      /* N = 64 */
        float 0.9987764161, 0.0494537198      /* N = 128 */
        float 0.9996964521, 0.0246374491      /* N = 256 */
        float 0.9999244068, 0.0122955517      /* N = 512 */
        float 0.9999811383, 0.006141882437    /* N = 1024 */
        float 0.999995289,  0.003069455488    /* N = 2048 */
        float 0.999998823,  0.001534354693    /* N = 4096 */

#endif PERIODOGRAM


