#include <stdio.h>
#include <floatingpoint.h>
#include <math.h>

main()
{
double freq, pt, ant_gain, ant_gain_lin, t, pi, C, loss_out, horiz_beam;
double vert_beam, range, range_nmi, lamda, lamda_cm, loss_out_lin; 
double cn, cndb, crh, crhdb, h;
double horiz_rad, vert_rad, k;
double adouble, numerator, denominator, freq_squared;
double logk,twoagain,pwrdb,ccw,ccwdb;

k = 0.93;         /* magnitude of expression containing index of refraction */
freq = 9.5;       /* transmitted frequency in GHz */
pt = 40000.0;     /* transmitted power in watts */
ant_gain = 39.0;  /* antenna gain in db */
t = 0.5;          /* Pulse width in microseconds */
pi = 3.14159;
C = 2.997925e8;   /* speed of light in meters per second */
loss_out = 2;     /* waveguide loss out to antenna in db */
horiz_beam = 1.8;  /* horizontal beamwidth in degrees */
vert_beam = 1.9;   /* vertical beamwidth in degrees */
range = 15;        /* Arbitrary range in kilometers */

lamda = C/(freq*1.e9);   /* wavelength in meters */
lamda_cm = 100*lamda;    /* wavelength in centimeters */
/* printf("lamda = %f m lamda = %f cm\n",lamda,lamda_cm); */

ant_gain_lin = pow((double)10.0,(double)(ant_gain/10.0));
/* linear antenna gain (dimensionless) */
loss_out_lin = pow((double)10.0,(double)(2 * loss_out/10.0)));
/* linear waveguide loss (dimensionless) */
range_nmi = range * .53996;  /* arbitrary range in nautical miles */
/* printf("antenna gain = %f   loss out = %f   range = %f nmi\n",
       ant_gain_lin, loss_out_lin, range_nmi); */

cn = (1.1e-23 * pt * horiz_beam * vert_beam * ant_gain_lin * ant_gain_lin *
       t * loss_out_lin) / (lamda_cm * lamda_cm * .53996 * .53996);

cndb = 10 * log10((double)cn) + 30.0;

printf("Nathanson's cr = %e  in dbm = %f\n",cn,cndb);

h = C * t * 1.0e-6;         /* pulse width in meters */ 
horiz_rad = horiz_beam * pi / 180.;   /* horizontal beam width in radians */
vert_rad = vert_beam * pi / 180.;     /* vertical beam width in radians */

crh = (1.0e-24 * pi * pi * pi * pt * ant_gain_lin * ant_gain_lin * h *
       k * k * horiz_rad * vert_rad * loss_out_lin) /
	 (1024 * log((double)2.0) * lamda * lamda);

crhdb = 10 * log10((double)crh) + 30.;

printf("rinehart's = %e  in dbm = %f\n",crh,crhdb);

freq_squared = freq * freq * 1.0e18;
numerator = pow((double)pi,(double)5.0) * horiz_beam * vert_beam * 
  t * 1.0e-6 *  freq_squared * k * k * 1.0e-24;
denominator = C * 180.0 * 180.0 * 512.0 * 2.0 * log((double)2.0);
adouble = numerator/denominator;

logk = 10 * log10((double)adouble);
twoagain = ant_gain * 2.0;
pwrdb = 10 * log10((double)pt) - 2 * loss_out;
ccwdb = logk + twoagain + pwrdb + 30;
ccw = pow((double)10.0,(double)(ccwdb/10.0));

printf("walther's = %e  in dbm = %f\n",ccw,ccwdb);
/* printf("freq squared = %e num = %e  denom = %e\n",freq_squared,
       numerator,denominator);
printf("adouble = %e   logk = %f   twoagain = %f\n",adouble,logk,twoagain);
printf("pwrdb = %f\n",pwrdb); */

}

