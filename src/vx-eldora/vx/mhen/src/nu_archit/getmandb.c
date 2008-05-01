/******************************** GETMANDB.C ******************************/
/*                                                                        */
/*      These subroutines measure the actual gain and offset of a given   */
/*    A/D channel.  One routine finds just offset given a time series.    */
/*    Another finds only the gain ( always about 1 ) from a time series.  */
/*    A third finds both gain and offset from a single times series,      */
/*    though, at some loss of accuracy for the offset.  It is desirable   */
/*    to compute offset from a time series that does not contain a large  */
/*    sinusoid since A/D nonlinearities and roundoff are then less        */
/*    critical.                                                           */
/*       Once the gain (m) and offset (b) are found, the linearization    */
/*    parameters are then computed by:                                    */
/*                                                                        */
/*                        1                          b                    */
/*                   m'= ---       and     b' =  -  ---                   */
/*                        m                          m                    */
/*                                                                        */
/*    m' and b' are the values that are used to set up the CAL RAM to     */
/*    linearize the A/D response.                                         */
/*                                                                        */
/**************************************************************************/

#include    "math.h"
#include    "vxWorks.h"
#include    "mathVar.h"

typedef  struct {float x,y;}  complex;

#define   mag(a)  sqrt(a.x * a.x + a.y * a.y)

void getm(a,n,counts,freq,mi,mq)     /* compute gain of I and Q channel */
complex       a[];                   /* complex array */
int           n;                     /* number of elements in array */
float         counts;                /* desired amplitude in counts */
float         freq;                  /* normalized frequency */
float         *mi,*mq;               /* gain coefficients */
{
  int        i;                      /* loop index */
  complex    im,qm;                  /* i and q fourier coefficients */
  double     msin,mcos,window,arg;   /* arg, sin, and cos temps, */
                                     /* and raised cos window temp */

  /* Compute only the fourier coefficients needed */
  /* A complete FFT is not necessary */
  im.x = im.y = qm.x = qm.y = 0.0;   /* zero out accumulator coefficients */
  for(i=0; i<n; i++)
    {
    arg = 2.0 * M_PI * (double)i;    /* argument temp */
    window = 1.0 - cos(arg / (double)n);  /* raised cosine window temp */
    msin = sin(arg * freq);          /* sin temp */
    mcos = cos(arg * freq);          /* cos temp */
    im.x += window * a[i].x * mcos;  /* complex fourier coeff of I channel */
    im.y += window * a[i].x * msin;
    qm.x += window * a[i].y * mcos;  /* complex fourier coeff of Q channel */
    qm.y += window * a[i].y * msin;
    }

*mi = mag(im) / (counts * 0.5 * n);  /* divide by number, size, and   */
*mq = mag(qm) / (counts * 0.5 * n);  /* normilization factor. */

#ifdef PHASE
/* Quadrature phase error is an interesting parameter and is readilly */
/* computed from the above numbers.  Therefore, it is computed here.  */
/* A software switch allows this to compile as an engineering tool.   */
printf("quadrature phase error = %f deg\n",90.0+
            180.0 * atan2(im.y * qm.x - im.x * qm.y,
                          im.x * qm.x + im.y * qm.y)/M_PI);
#endif
}

void getb(a,n,bi,bq)
complex       a[];                   /* complex array */
int           n;                     /* number of elements in array */
float         *bi,*bq;               /* linearization coefficients */
{
  int        i;
  complex    b;
  double     window,arg;

  /* Compute only the fourier coefficients needed (zeroth) */
  /* A complete fft is not necessary */
  b.x = b.y = 0.0;
  for(i=0; i<n; i++)
    {
    arg = 2.0 * M_PI * (double)i;
    window = 1.0 - cos(arg / (double)n);  /* window is normalized */
    b.x += window * a[i].x;         /* complex DC offset (of I and Q) */
    b.y += window * a[i].y;
    }

*bi = b.x / n;       /* divide by the number of accumulations (take ave) */
*bq = b.y / n;
}

void getmandb(a,n,counts,freq,mi,bi,mq,bq)
complex       a[];                   /* complex array */
int           n;                     /* number of elements in array */
float         counts;                /* desired amplitude in counts */
float         freq;                  /* normalized frequency */
float         *mi,*bi,*mq,*bq;   /* linearization coefficients */
{
  int        i;
  complex    b;
  complex    im,qm;                  /* i and q fourier coefficients */
  double     msin,mcos,window,arg;

  /* Compute only the two fourier coefficients needed */
  /* A complete fft is not necessary */
  im.x = im.y = qm.x = qm.y = b.x = b.y = 0.0;
  for(i=0; i<n; i++)
    {
    arg = 2.0 * M_PI * (double)i;
    window = 1.0 - cos(arg / (double)n);  /* window is normalized */
    msin = sin(arg * freq);
    mcos = cos(arg * freq);
    b.x  += window * a[i].x;         /* complex DC offset (of I and Q) */
    b.y  += window * a[i].y;
    im.x += window * a[i].x * mcos;  /* complex fourier coeff of I channel */
    im.y += window * a[i].x * msin;
    qm.x += window * a[i].y * mcos;  /* complex fourier coeff of Q channel */
    qm.y += window * a[i].y * msin;
    }

*mi = mag(im) / (0.5 * n * counts);
*bi = b.x / n;
*mq = mag(qm) / (0.5 * n * counts);
*bq = b.y / n;

#ifdef PHASE
/* Quadrature phase error is an interesting parameter and is readilly */
/* computed from the above numbers.  Therefore, it is computed here.  */
/* A software switch allows this to compile as an engineering tool.   */
printf("quadrature phase error = %f deg\n",90.0+
            180.0 * atan2(im.y * qm.x - im.x * qm.y,
                          im.x * qm.x + im.y * qm.y)/M_PI);
#endif
}








