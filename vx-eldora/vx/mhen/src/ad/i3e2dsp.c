/*
 *	$Id$
 *
 *	Module:	I3E2DSP	 
 *	Original Author: Grant Gray
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1991/12/04  16:09:11  eric
 * Initial revision
 *
 *
 * description:
 *
 * Subroutine to convert ieee floating point to DSP32 floating point.
 * Single-precision only.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

unsigned long i3e2dsp(pn)
float pn;
{
   unsigned long  mant = 0, sgn = 0, ex = 0;
   unsigned long ans;
   union 
    	{
	float f;
	unsigned long l;
	}pnp;
 
   pnp.f = pn;	
/* printf("%lx\n",pn); */

   sgn = pnp.l & 0x80000000;
   mant = pnp.l &  0x7FFFFF;
   mant = mant << 8;
   ex = pnp.l &  0x7F800000;
   ex = ex >> 23;
   ex += 1;
   if(ex>0xff)ex = 0xff;
/* printf("s,m,e %lx %lx %lx\n",sgn,mant,ex);*/
   if((pnp.l & 0x7fffffff) == 0)return(0);

   if(ex == 0xFF)
   {
      if(sgn == 0) ans = 0x7FFFFFFF;
      else ans = 0x800000FF;
   }
   else if(ex == 0) ans = 0;
   else
   {
      if(sgn != 0)
      {
	 mant = ((mant ^ 0x7fffff00) + 0x100);
         if((mant & 0x80000000) != 0)ex = ex -1;
         mant = mant & 0x7fffff00;
      }
	
      ans = sgn | mant | ex;
   }
/* printf("ans = %lx\n",ans); */
   return(ans);
}

