
/*
*	COLLATOR & QUAD subroutines from AT&T DSP32C library
*	invf, log10, loge, divf, sqrtq
*	26 Nov 90 f.p.
*/

/*  define HIRESLOG for the full precision log routines */
#define HIRESLOG
#ifdef QUADSUBS
/************************************************************/
/* The routine _invf follows                                */
/************************************************************/
/*
        _invf(A,B)
        float *A,*B;
   scratch register short r1,r2,r3,r4,r5,r14 (DSP32);
   scratch register short r1,r2,r3,r14 (DSP32C);
   scratch register float a0,a1,a2,a3;
*/

_invf:  r1e = *r14++   /* denominator */
        r2e = *r14++   /* point to inverse location */
        a3=*r1        /* load number to be inverted into a3 */
        *r2 = a2 = seed(a3)
        r3e=_invfA
        nop
        a0=*r3++ - a3 * a2       /* a0=1.4074-x*y */
        a1 = *r2 * *r3++         /* a1=.81*x */
        a2=*r3++ - a3 * *r2      /* a2=2.27424-x*y */
        a0=*r3++ + a0 * a0       /* a0=1.71742-2.8148*x*
                                                  y+x^2*y^2 */
        a1=a0 * a1               /* a1=1.14*x-.81*x^2*y */
        a2=a2 * a1               /* a2=1.842*x-.81*x^2*y */
        return (r14)
        *r2 = a0 = a2 + a0 * a1  /* a0=3.8x-5.41x^2* 
                                   y+3.42x^3*y^2-.81x^4*y^3 */
 
_invfA: float 1.4074074347, 0.81, 2.27424702, -.263374728
#endif QUADSUBS

#ifdef COLSUBS
#ifdef HIRESLOG
.global _log10, _log10C
/************************************************************/
/* The routine _log10 follows                               */
/************************************************************/
/*
        _log10(A,B,C)
        float *A, *B, *C;
   scratch register short r5,r6;
   scratch pointer register short r1,r2,r3,r4,r14;
   scratch register float a0,a1,a2,a3;
*/
 
_log10:  r2e=*r14++            /* pointer to a copy of x */
         r1e=*r14++            /* pointer to operand, x */
         r4e=_log10C+36        /* pointer to c8 */
         *r2=a0=*r1           /* load x into copy */
         nop
         r3e=r2 + 4   /* pointer to scratch pad for exponent */
 
      /* extract mantissa, M, and divide by 2 */
         r5=-1 + 128      /* -1 plus bias */
         *r2=r5l   /* replace exponent of operand with "-1" */
         a3=*r2           /* move M/2 into a3 */
 
      /* store exponent, E, as a floating point number */
         r6l=*r1            /* get exponent (byte) */
         r1e=*r14            /* pointer to output */
         r6=r6+1-128        /* remove bias and add 1 */
         *r3=r6             /* store as an integer */
         *r3=a0=float(*r3)  /* convert to float and store */
 
 /* compute log(M/2) in base 2  */
    a0 = a3 * a3                /* (M/2)^2 */
    a1 = *r4-- + a3 * *r4--     /* c7 + c8*M/2 */
    a2 = *r4-- + a3 * *r4--     /* c4 + c5*M/2 */
    a0 = a0 * a3                /* (M/2)^3 */
    a1 = *r4-- + a1 * a3        /* c6+c7*(M/2)+c8*((M/2)^2) */
    a3 = *r4-- + a3 * *r4--     /* c1 + c2*M/2 */
    a2 = *r4-- + a2 * *r2       /* c3+(M/2)*(c4+c5*(M/2)) */
    a1 = a1 * a0           /* c6*((M/2)^3)+...+c8*((M/2)^5) */
    a3 = *r4-- + a3 * *r2   /* c0 + c1*(M/2) + c2*((M/2)^2) */ 
    a0 = *r3 * *r4          /* c9 * (E+1) */
    a2 = a3 + a2 * a0     /* c0+c1*(M/2)+ ... +c5*((M/2)^5) */
    a1 = a2 + a1 * a0     /* c0+c1*(M/2)+ ... +c8*((M/2)^8) */

 /* add c9*(E + 1) and store  */
    *r1 = a0 = a1 + a0
    nop
    goto r14+4
    nop
 
_log10C: float 0.3010299957      /* c9 */
         float -1.332183622      /* c0 */
         float 22.36407471       /* c3 */
         float 4.909769401       /* c1 */
         float -12.05022337      /* c2 */
         float -14.41930397      /* c6 */
         float -28.96563148      /* c4 */
         float 25.42144201       /* c5 */
         float 4.772579384       /* c7 */
         float -0.7005197015     /* c8 */
#else

/* Experimental LOG10 for DSP32C	*/
/* Reduce execution time by cutting precision fp 24 Nov 90 */
/* Compared the original _log10 (presumably good to 5 decimal places */
/* and six significant digits), */
/* to a fast version of log10: average difference was 0.0011 and largest*/
/* difference seen in the test data was 0.0047 */
/* (for the loge versions, largest diff seen was 0.0073) */

	/* series approximation y=lnx from CRC Handbook */
_log10:  r2e=*r14++            /* pointer to scratch */
         r1e=*r14++            /* pointer to input x */
        r4e=_logc0f        /* pointer to constants */
         *r2=a0=*r1           /* load x into scratch */
 
      /* store exponent, E, as a floating point number */
         r6l=*r1            /* get exponent (byte) */
         r1e=*r14            /* pointer to output */

      /* extract mantissa, M, and divide by 2 */
         r5=-1 + 128      /* -1 plus bias */
         *r2=r5l   /* replace exponent of operand with "-1" */
         a3=*r2           /* move M/2 into a3 */
	 a2 = *r4--	/* get c0 into a2, move pointer to c1 */
 
      /* finish exponent, E, as a floating point number */
         r6=r6+1-128        /* remove bias and add 1 */
         *r1=r6             /* store as an integer */
temp10:	a0 = a3 * a3                /* (M/2)^2 */
         *r1=a1=float(*r1)  /* convert E to float and store temporarily in y*/
     /* calculate approxomate y=logx to appropriate base */	
	a2 = a2 + a3 * *r4--			/*  c0 + c1M/2 */
	a3 = a3 * a0		    /* (M/2)^3 */
	a0 = a0 * a0		/* (M/2)^4 */
	a2 = a2 + a0 * *r4--	/* c0 + c1M/2 + c2M^2/2^2*/
	a2 = a2 + a3 * *r4--	/* c0 + c1M/2 + c2M^2/2^2 + c3M^3/2^3 */
	a2 = a2 + a0 * *r4-- /*c0+c1M/2+c2M^2/2^2+c3M^3/2^3+c4M^4/2^4*/
			/* and finally add in   c9(E+1) */ 
	*r1 = a0 = a2 + a1 * *r4 /* we're done */
	nop 	/* nop here to clear pipe into output location */
	/* result also available in a0 */

    goto r14+4
    nop

#define ETO10		0.434294481
_logc9f: 		float 0.3010299957     		 /* c9 */
_logc4f:		float -0.250000000*ETO10	/* c4 */
_logc3f:		float 1.333333333*ETO10		/* c3 */
_logc2f: 	        float -3.00000000*ETO10      /* c2 */
_logc1f:	        float 4.00000000*ETO10       /* c1 */
_logc0f:         	float -2.083333333*ETO10      /* c0 */

#endif HIRESLOG
#endif COLSUBS

#ifdef COLSUBS
#ifdef HIRESLOG
.global _loge, _logeC
/************************************************************/
/* The routine _loge follows                                */
/************************************************************/
/*
        _loge(A,B,C)
        float *A, *B, *C;
   scratch register short r5,r6;
   scratch pointer register short r1,r2,r3,r4,r14;
   scratch register float a0,a1,a2,a3;
*/

_loge:  r2e=*r14++            /* pointer to a copy of x */
        r1e=*r14++            /* pointer to operand, x */
        r4e=_logeC+36      /* pointer to c8 */
        *r2=a0=*r1         /* load x into copy */
        nop
        r3e=r2 + 4    /* pointer to scratch pad for exponent */

     /* extract mantissa, M, and divide by 2 */
        r5=-1 + 128       /* -1 plus bias */
        *r2=r5l    /* replace exponent of operand with "-1" */
        a3=*r2             /* move M/2 into a3 */

     /* store exponent, E, as a floating point number */
        r6l=*r1        /* get exponent (byte) */
        r1e=*r14       /* pointer to output */
        r6=r6+1-128    /* remove bias and add 1 */
        *r3=r6         /* store as an integer, i.e., (E+1) */
        *r3=a0=float(*r3)    /* convert to float and store */

/* compute log(M/2) in base 2  */
   a0 = a3 * a3                 /* (M/2)^2 */
   a1 = *r4-- + a3 * *r4--      /* c7 + c8*M/2 */
   a2 = *r4-- + a3 * *r4--      /* c4 + c5*M/2 */
   a0 = a0 * a3                 /* (M/2)^3 */
   a1 = *r4-- + a1 * a3         /* c6+c7*(M/2)+c8*((M/2)^2) */
   a3 = *r4-- + a3 * *r4--      /* c1 + c2*M/2 */
   a2 = *r4-- + a2 * *r2   /* c3+(M/2)*(c4+c5*(M/2)) */
   a1 = a1 * a0            /* c6*((M/2)^3)+...+c8*((M/2)^5) */
   a3 = *r4-- + a3 * *r2   /* c0 + c1*(M/2) + c2*((M/2)^2) */ 
   a0 = *r3 * *r4          /* c9 * (E+1) */
   a2 = a3 + a2 * a0       /* c0+c1*(M/2)+...+c5*((M/2)^5) */
   a1 = a2 + a1 * a0       /* c0+c1*(M/2)+...+c8*((M/2)^8) */

     /* add c9*(E +1) and store  */
        *r1 = a0 = a1 + a0
        nop
        goto r14+4
        nop

_logeC: float 0.6931471806       /* c9 */
        float -3.067466148       /* c0 */
        float 51.49518505        /* c3 */
        float 11.30516183        /* c1 */
        float -27.7466647        /* c2 */
        float -33.20167437       /* c6 */
        float -66.69583126       /* c4 */
        float 58.53503341        /* c5 */
        float 10.98927014        /* c7 */
        float -1.613006222       /* c8 */

#else

/* Experimental LOGE for DSP32C	*/
/* Reduce execution time by cutting precision fp 24 Nov 90 */
/* Compared the original _log10 (presumably good to 5 decimal places */
/* and six significant digits), */
/* to a fast version of log10: average difference was 0.0011 and largest*/
/* difference seen in the test data was 0.0047 */
/* (for the loge versions, largest diff seen was 0.0073) */
	/* series approximation y=lnx from CRC Handbook */
.global _loge, tempef
_loge:  r2e=*r14++            /* pointer to scratch */
         r1e=*r14++            /* pointer to input x */
        r4e=_logc0e        /* pointer to constants */
         *r2=a0=*r1           /* load x into scratch */
 
      /* store exponent, E, as a floating point number */
         r6l=*r1            /* get exponent (byte) */
         r1e=*r14            /* pointer to output */

      /* extract mantissa, M, and divide by 2 */
         r5=-1 + 128      /* -1 plus bias */
         *r2=r5l   /* replace exponent of operand with "-1" */
         a3=*r2           /* move M/2 into a3 */
	 a2 = *r4--	/* get c0 into a2, move pointer to c1 */
 
      /* finish exponent, E, as a floating point number */
         r6=r6+1-128        /* remove bias and add 1 */
         *r1=r6             /* store as an integer */
tempef:	a0 = a3 * a3                /* (M/2)^2 */
         *r1=a1=float(*r1)  /* convert E to float and store temporarily in y*/
     /* calculate approxomate y=logx to appropriate base */	
	a2 = a2 + a3 * *r4--			/*  c0 + c1M/2 */
	a3 = a3 * a0		    /* (M/2)^3 */
	a0 = a0 * a0		/* (M/2)^4 */
	a2 = a2 + a0 * *r4--	/* c0 + c1M/2 + c2M^2/2^2*/
	a2 = a2 + a3 * *r4--	/* c0 + c1M/2 + c2M^2/2^2 + c3M^3/2^3 */
	a2 = a2 + a0 * *r4-- /*c0+c1M/2+c2M^2/2^2+c3M^3/2^3+c4M^4/2^4*/
			/* and finally add in   c9(E+1) */ 
	*r1 = a0 = a2 + a1 * *r4 /* we're done */
	nop 	/* nop here to clear pipe into output location */
	/* result also available in a0 */

    goto r14+4
    nop

#define ETOE		1.0000
_logc9e: 		float 0.69314718     		 /* c9 */
_logc4e:		float -0.250000000*ETOE		/* c4 */
_logc3e:		float 1.333333333*ETOE		/* c3 */
_logc2e: 	        float -3.00000000*ETOE      /* c2 */
_logc1e:	        float 4.00000000*ETOE       /* c1 */
_logc0e:         	float -2.083333333*ETOE      /* c0 */
#endif HIRESLOG
#endif COLSUBS

#ifdef COLSUBS
.global _divf, _sqrtf
/************************************************************/
/* The routine _divf follows                                */
/************************************************************/
/*
        _divf(A,B,C)
        float *A,*B,*C;
   scratch register short r5,r6 (DSP32 only);
   scratch pointer register short r1,r2,r3,r4,r14;
   scratch register float a0,a1,a2,a3;
*/

_divf:  r4e = *r14++          /* numerator pointer */
        r1e = *r14++          /* denominator pointer */
        r2e = *r14            /* point to quotient location */

        a2 = seed(*r1)
        a3=*r1           /* load denominator (y) into a3 */
        r3e=_divfA
        a1 = a2 * *r3++           /* a1=.81*x */
        a0 = *r3++ - a3 * a2      /* a0=1.4074-x*y */
        a2 = *r3++ - a3 * a2      /* a2=2.27424-x*y */
        a1 = a1 * *r4              /* scale by numerator */
        nop

        a0 = *r3++ + a0 * a0        /* a0=1.71742-2.8148*x* 
                                                   y+x^2*y^2 */
        a1 = a0 * a1                /* a1=1.14*x-.81*x^2*y */
        a2 = a2 * a1                /* a2=1.842*x-.81*x^2*y */
        goto r14+4
        *r2 =  a0 = a2 + a0 * a1     /* a0=3.8x-5.41x^2*
                                   y+3.42x^3*y^2-.81x^4*y^3 */

_divfA: float 0.81, 1.4074074347, 2.27424702, -.263374728

#endif COLSUBS

#if defined(COLSUBS) || defined(QUADSUBS)
/************************************************************/
/* The routine _sqrtq follows                               */
/************************************************************/
/*
        _sqrtq(A,B,C)
        float *A, *B, *C;
   scratch register short r5, r6, r7;
   scratch pointer register short r1,r2,r3,r4,r14;
   scratch register float a0,a1;
*/

_sqrtq: r2e = *r14++       /* pointer to local variable */
        r1e = *r14++       /* pointer to operand, x */
        r3e = *r14         /* pointer to output */
        *r2 = a0 = *r1     /* load x into *r2 */
        r4e = _sqrtqC      /* pointer to coefficients */
 
        nop
        r5 = 128           /*  bias */
        *r2 = r5l          /* replace exponent of x with "0" */
        a1 = *r2 - *r4++   /* M-1.47 */
 
/* compute square root of mantissa */
   a0 = *r2++ * *r4++    /* 0.4237288136*M */
   a0 = a0 + *r4++       /* 0.59 + 0.4237288136*M */
   a1 = a1 * a1          /* (M-1.47)**2 */
   r7l = *r1             /* exponent of x, i.e., E */
   r4e = _sqrtqD
   a1 = a1 * a1          /* (M-1.47)**4 */
   r6 = r7/2             /* int(E/2) */
 
/* Adjust for exponent, E */
   r6 = r6 + 64                 /* exponent of square root */
   *r3 = a0 = a0 - a1 * *r4++   /* estimate of M**0.5 */
   nop
   r7 = r7/2                 /* int(E/2) */
   if (cc) goto _sqrtqA
   *r3 = r6l                 /* write exponent to output */
   *r3 = a0 = *r3 * *r4      /* modify square root by 2^0.5 */

_sqrtqA: nop
         goto r14+4
         nop
 
_sqrtqC: float 1.47, 0.4237288136, 0.59
_sqrtqD: float 0.28, 1.414213562

/* -------------------------------------------------------- */


/************************************************************/
/* The routine _sqrtf follows                               */
/************************************************************/
/*
        _sqrtf(A,B,C)
        float *A, *B, *C;
   scratch register short r4, r5, r7, r8;
   scratch pointer register short r1,r2,r3,r6,r14;
   scratch register float a0,a1,a2,a3;
*/

_sqrtf: r2e = *r14++       /* pointer to  mantissa of x */
        r1e = *r14++       /* pointer to operand, x */
        r3e = *r14         /* pointer to output */
        *r2 = a0 = *r1     /* load x into *r2 */
        r6e = _sqrtfC      /* pointer to coefficients */
        nop
        r5 = 128          /*  bias */
        *r2 = r5l         /* replace exponent of x with "0" */
 
/* compute square root of mantissa */
   a2 = *r2++ * *r6++        /* 0.4237288136*M */
   *r3 = a3 = a2 + *r6++     /* 0.59 + 0.4237288136*M = 
                                    1st (M**0.5) iteration  */
 
/* compute inverse of *r3 */
   *r2 = a1 = seed(a3)
   r7l = *r1                 /* exponent of x, i.e., E */
   r6e = _sqrtfD
   r8 = r7/2                 /* int(E/2) */
   a0 = *r6++ - a3 * *r2   /* a0=1.4074-x*y */
   a1 = *r2 * *r6++        /* a1=.81*x */
   a2 = *r6++ - a3 * *r2-- /* a2=2.27424-x*y */
 
   a0 = *r6++ + a0 * a0    /* a0=1.71742-2.8148*x*y+x^2*y^2 */
   a1 = a0 * a1            /* a1=1.14*x-.81*x^2*y */
   a2 = a2 * a1            /* a2=1.842*x-.81*x^2*y */
   a3 = *r2 * *r6          /*  a3 = 0.5 * M */
   a0 = a2 + a0 * a1       /* a0=3.8x-5.41x^2*y+3.42x^3*y^2
                                      -.81x^4*y^3 = inverse */
   a1 = *r3 * *r6++        /* 0.5 * 1st root iteration */
   nop
   *r3 = a1 = a1 + a3 * a0     /* 2nd square root iteration */
 
/* Adjust for exponent, E */
   r8 = r8 + 64              /* exponent of square root */
   r7 = r7/2                 /* int(E/2) */
   if (cc) goto _sqrtfA
   *r3 = r8l                 /* write exponent to output */
   *r3 = a0 = *r3 * *r6      /* modify square root by 2^0.5 */

_sqrtfA: nop
         goto r14+4
         nop
 
_sqrtfC: float 0.4237288136, 0.59
_sqrtfD: float 1.4074074347, 0.81, 2.27424702, -.263374728
         float 0.5, 1.414213562

/* -------------------------------------------------------- */


#endif

#ifdef COLSUBS
#ifdef PERIODOGRAM
/************************************************************/
/* The routine _fftc follows                                */
/************************************************************/
/*
                _fftc (A,B,C,D)
        const   int   A, B;
                int  *C, *D;        
        scratch register short  r1 to r19;
        scratch register float  a0,a1,a2,a3;
*/

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
        r11 = r10-1        /* loop counter for radix-4 */
        do 15, r11

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
        r19e = r15 - r18
        r2e = *r14           /* _fftR */
                             /* initialize twiddle factors */
        a2 = *r4++           /* Ur = a2 */
        a3 = *r4--           /* Ui = a3 */
        r13 = r8 - 3         /* loop count */
        r8 = r8*2

        r1e = r2
        r11e = r1

        r3e = r1 + r9
        r12e = r3 
        r5e = r10 - 2
        do 3, r5

/* Butterfly # 1 ----------------------------------------- */
_fftc10d:
        *r11++r17=a0=*r1 + *r3++r17         /* RE1 + RE2 */
        *r12++r17=a0=-a0 + *r1++r17 * *r6   /* RE1 - RE2 */
        *r11++r15=a0=*r1 + *r3++r15         /* IM1 + IM2 */
        *r12++r15=a0= -a0 + *r1++r15 * *r6  /* IM1 - IM2 */

        *r11++r17=a0=*r1 + *r3++r17         /* RE1 + RE2 */
        *r12++r17=a0=-a0 + *r1++r17 * *r6   /* RE1 - RE2 */
        *r11++r19=a0=*r1 + *r3++r19         /* IM1 + IM2 */
        *r12++r19=a0=-a0 + *r1++r19 * *r6   /* IM1 - IM2 */

/* Butterfly # 2 ----------------------------------------- */

_fftc20: 
         do 5, r5
_fftc10:
    a0 = *r1++r17 + a2 * *r3++r17     /* a0=Re(i)+Ur*Re(k) */
   *r11++r17=a0=a0 - *r3++r16 * a3    /* Re(i)=a0=a0-Ui*Im(k) */
    a1 = *r1++r16 + a3 * *r3++r17     /* a1=Im(i)+Ui*Re(k) */
   *r12++r17=a0=-a0+ *r1++r17 * *r6   /* Re(k)=-a0+2*Re(i) */
   *r11++r15=a1=a1+ *r3++r15 * a2     /* Im(i)=a1=a1+Ur*Im(k) */
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

        r3e = r1 + r9
        r12e = r3
        r13 = r8 - 1
        do 9, r13

_fftc20A:
    a0 = *r1++r17 + a2 * *r3++r17     /* a0=Re(i)+Ur*Re(k) */
   *r11++r17=a0=a0 - *r3++r16 * a3    /* Re(i)=a0=a0-Ui*Im(k) */
    a1 = *r1++r16 + a3 * *r3++r17     /* a1=Im(i)+Ui*Re(k) */
   *r12++r17=a0=-a0+ *r1++r17 * *r6   /* Re(k)=-a0+2*Re(i) */
   *r11++r15=a1=a1+ *r3++r15 * a2     /* Im(i)=a1=a1+Ur*Im(k) */
   *r12++r15=a1=-a1+ *r1++r15 * *r6   /* Im(k)=-a1+2*Im(i) */
    a0 = a2 * *r4++                   /* Compute new twiddle */
    a2 = a0 - a3 * *r4                /*   U = U * _fftcW    */
    a0 = a2 * *r4--
    a3 = a0 + a3 * *r4

/* --------------------------------------------------- */
fret:        goto r14+8
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

/* -------------------------------------------------------- */
#endif PERIODOGRAM

.align 4
endcode:	int24	0x5555
#endif COLSUBS

#ifdef COLSUBS
#ifdef OLD_ATAN2
/* atan is not in checksum because of local storage */
/* in the atan function */

 /*
        _atan2(B,A,theta)
        float *A,*B,theta;
    {
    scratch register short r4,r5;
    scratch pointer register short r1,r2,r3,r14;
    scratch register float a0,a1,a2,a3;
 #asm
 */
 _atan2:r2=*r14++     /* pointer to B */
        r3=*r14++   
        r1=_atanA     /* r1 points to in-line local variable */
        *r1++=a3=*r2 + *r3       /* B+A */
        *r1--=a0=*r2 - *r3       /* B-A */
        r3 = _atanB 

        r2 = *r14++   /* pointer to out */
        r14 = r14 + 2    /* Bump r14 to align return */

        r4=*r1++           /* load x+1=y into cau */
        r5=*r1++
        r4=r4^0xffff       /* invert all bits except sign bit */
        r5=r5^0x7fff
        *r2++=r4           /* 1st estimate of 1/y,
                              or x, written to memory */
        *r2--=r5

        a0=*r3++ -a3 * *r2  /* a0=1.4074-x*y */
        a1=*r3++ * *r2      /* a1=.81*x */
        a2=*r3++ -a3 * *r2  /* a2=2.27424-x*y */
        a0=*r3++ +a0 * a0   /* a0=1.71742-2.8148*x*y+x^2*y^2 */
        a1=a0*a1             /* a1=1.14*x-.81*x^2*y */
        a2=a2*a1             /* a2=1.842*x-.81*x^2*y */
        nop
        a0=a2+a0*a1          /* a0=3.8x-5.41x^2*y+3.42x^3* ...
                                 y^2-.81x^4*y^3             */
        nop
        nop
        a1=*r3 + a3*a0        /* a1=-1 + y* x(new) */
        a0 = a0 * *r1         /* scale by "(x-1)" */
        nop
        nop
        *r2 = a2 = a0-a1*a0   /* third iteration, ...  */
        nop                   /*    a2=(x-1)/(x+1)     */
        nop
        a0=a2*a2              /* y**2 */
        r1=_atanC
        nop
        a0=a0**r2             /* y**3 */

        a1 = *r1++ + a0 * *r1++   /* c7 + c9*(y^2) */
        a2 = *r1++ + a0 * *r1++   /* c3 + c5*(y^2) */
        a0 = a0 * *r2          /* y^4 */
        a1 = a1 * a0           /* c7*(y^3) + c9*(y^5) */
        a2 = *r1++ + a2 * a0   /* pi/4 + c3*(y^3) + c5*(y^5) */
 
        a2 = a2 + *r1++ * *r2    /* pi/4+c1*y+ ...
                                    c3*(y^3)+c5*(y^5) */
        *r2 = a1 = a2 + a1 * a0  /* pi/4+c1*y+c3*(y^3)+c5*(y^5)
                                     ... +c7*(y^7)+c9*(y^9)  */  
 
        nop
        return (r14)
        nop

 _atanA: 2*float 0.0           /* two in-line local variables */
 _atanB: float 1.4074074347, 0.81, 2.27424702, -.263374728, -1.0
 _atanC: float .0208351, -.085133      /* c9, c7 */
         float .180141, -.3302995      /* c5, c3 */ 
         float .785398163, .999866     /* pi/4, c1 */


#else

.global _atan2, _atanA, _atanB, _atanC

 /*
        _atan2(B,A,theta)
        float *A,*B,theta;
    scratch register short r4,r5;
    scratch pointer register short r1,r2,r3,r14;
    scratch register float a0,a1,a2,a3;
 */
 _atan2:r2e=*r14++     /* pointer to B */
        r3e=*r14++   
        r1e=_atanA     /* r1 points to in-line local variable */
        *r1++=a3=*r2 + *r3       /* B+A */

       a2 = seed(a3)
        *r1=a0=*r2 - *r3       /* B-A */
       r3e=_atanB
       r2e = *r14         /* pointer to out */
       a0=*r3++ -a3 * a2   /* a0=1.4074-x*y */
       a1=*r3++ * a2       /* a1=.81*x */
       a2=*r3++ -a3 * a2   /* a2=2.27424-x*y */

       a0=*r3++ +a0 * a0   /* a0=1.71742-2.8148*x*y+x^2*y^2 */
       a1=a0*a1             /* a1=1.14*x-.81*x^2*y */
       a2=a2*a1             /* a2=1.842*x-.81*x^2*y */
       nop
       a0=a2+a0*a1          /* a0=3.8x-5.41x^2*y+3.42x^3* ...
                                y^2-.81x^4*y^3             */
       nop
       nop
       a1=*r3 + a3*a0        /* a1=-1 + y* x(new) */
       a0 = a0 * *r1         /* scale by "(x-1)" */
       nop
       nop
       *r2 = a2 = a0-a1*a0   /* third iteration, ...  */
       nop                   /*    a2=(x-1)/(x+1)     */
       nop
       a0=a2*a2              /* y**2 */
       r1e=_atanC
       nop
       a0=a0**r2             /* y**3 */

       a1 = *r1++ + a0 * *r1++   /* c7 + c9*(y^2) */
       a2 = *r1++ + a0 * *r1++   /* c3 + c5*(y^2) */
       a0 = a0 * *r2          /* y^4 */
       a1 = a1 * a0           /* c7*(y^3) + c9*(y^5) */
       a2 = *r1++ + a2 * a0   /* pi/4 + c3*(y^3) + c5*(y^5) */

       a2 = a2 + *r1++ * *r2    /* pi/4+c1*y+ ...
                                   c3*(y^3)+c5*(y^5) */
       *r2 = a0 = a2 + a1 * a0  /* pi/4+c1*y+c3*(y^3)+c5*(y^5)
                                    ... +c7*(y^7)+c9*(y^9)  */    
       nop
       goto r14+4
       nop

 _atanA: 2*float 0.0           /* two in-line local variables */
 _atanB: float 1.4074074347, 0.81, 2.27424702, -.263374728, -1.0
 _atanC: float .0208351, -.085133      /* c9, c7 */
         float .180141, -.3302995      /* c5, c3 */ 
         float .785398163, .999866     /* pi/4, c1 */

#endif OLD_ATAN2
#endif COLSUBS

/* end of COLLATOR subroutines */
