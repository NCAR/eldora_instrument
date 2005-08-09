/*+**********************************************************************

      NAME:  c_align.c

  SYNOPSIS:  *
 	     *  This function initializes the dcm matrix to a 1st order
	     *  This assumes that fastnav and imucomp have been initialized
	     *  and are running, position data was initialized and T was input
	
    INPUTS: Integration step size, T
	    Down gravity component, gdp, calculated by gravity()
	    Compensated x,y,z velocities from acomp()

PROCESSING:


   OUTPUTS:  The A matrix -- Platform to Geographic transformation
	     The aijrt matrix, A matrix initialization used by initnav()

   RETURNS:  None.

	By:  AL WATTS   July 1987
	     Ported from Otter code to STRYPI nav code Oct 1988 by eec


**********************************************************************+*/

#include "math.h"
#include "navvars.h"

unsigned short int ctime;	/* 64 hz counter,
				   increments each c_align invocation */

void c_align()
{
   void gravity();                     /* Source of Gravity */


   static char  min;                   /* necessary internal variables */
   static double  gT_inv;
   static double  ax,ay,az;
   static double  a,b,c,d, den_x,den_y,den_z;
   static double  aax,aay,aaz;
   extern unsigned short int ctime;

   register struct nav_vars *n;
   n = (struct nav_vars *)NAV_start;

    /* LEVEL PLATFORM ==> c31 c32 c33 */
   if(ctime == 0)
   {
      gravity();
      gT_inv = 1./(n->gdp*50.0);
      ax = 0;
      ay = 0;
      az = 0;
   }

   if((ctime >= 1)&&(ctime <= 50))
   {
      ax += n->dvxi;
      ay += n->dvyi;
      az += n->dvzi;
   }

   if(ctime == 51)
   {
      n->c31 = -gT_inv*ax;
      n->c32 = -gT_inv*ay;
      n->c33 = -gT_inv*az;

    aax = fabs(ax);
    aay = fabs(ay);
    aaz = fabs(az);

    /* HEADING == FORCE SMALLEST COMPONENT IN N-D PLANE */
      if ( (aax < aaz) && (aax < aay) )
         min = 'x';

      if ( (aay < aax) && (aay < aaz) )
         min = 'y';

      if ( (aaz < aax) && (aaz < aay) )
         min = 'z';


      switch(min)
      {
         case 'x':
            den_x = sqrt(n->c32*n->c32 + n->c33*n->c33);
            n->c21 =          0;
            n->c22 =  n->c33/den_x;
            n->c23 = -n->c32/den_x;
	    break;

         case 'y':
            den_y = sqrt(n->c31*n->c31 + n->c33*n->c33);
            n->c21 =  n->c33/den_y;
            n->c22 =          0;
            n->c23 = -n->c31/den_y;
	    break;

         case 'z':
         default:
            den_z = sqrt(n->c31*n->c31 + n->c32*n->c32);
            n->c21 =  n->c32/den_z;
            n->c22 = -n->c31/den_z;
            n->c23 =  0;
	    break;

      }

   /* ROW 1 THRU UNITARY PROPERTIES */
      a = n->c22*n->c33 - n->c23*n->c32;
      b = n->c31*n->c23 - n->c21*n->c33;
      c = n->c21*n->c32 - n->c31*n->c22;

      d = sqrt(a*a + b*b + c*c);

      n->c11 = a/d;
      n->c12 = b/d;
      n->c13 = c/d;

      n->a11 = n->a11rt = n->c11;
      n->a12 = n->a12rt = n->c12;
      n->a13 = n->a13rt = n->c13;
      n->a21 = n->a21rt = n->c21;
      n->a22 = n->a22rt = n->c22;
      n->a23 = n->a23rt = n->c23;
      n->a31 = n->a31rt = n->c31;
      n->a32 = n->a32rt = n->c32;
      n->a33 = n->a33rt = n->c33;

   }

   if(ctime<=51)
     ctime++;
}
