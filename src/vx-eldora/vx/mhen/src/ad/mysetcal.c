/*
 *      $Id$
 *
 *      Module: MYSETCAL.c
 *      Original Author: Mitch Randall
 *      Copywrited by the National Center for Atmospheric Research
 *      Date:            $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/11/09  17:00:32  eric
 * Initial revision
 *
 *
 * description: This module programs the ADC Cards with initial conditions.
 *              
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "vxWorks.h"
#include "stdioLib.h"
#include "ELDRP7.h"
#include "math.h"

#define    B             0.0
#define    DSY           1
#define    THRESHOLD     648L

void   mysetcal(num_freq)
int  num_freq;
   {
   unsigned long   freq,bd;
   char   hi_lo,q_i;
   void prog_calram(unsigned long,unsigned long,char,char,float,float);
   void prog_choiceram(unsigned long,unsigned long,long);
   void prog_admisc(unsigned long,unsigned long,short,char,char);

   bd = 4;
   for(freq=1; freq<=num_freq; freq++)
     {
     for(q_i=0; q_i<2; q_i++)
        for(hi_lo=0; hi_lo<2; hi_lo++)
           prog_calram(freq,bd,hi_lo,q_i,hi_lo?LOWm:HIGHm,B);
     prog_choiceram(freq,bd,THRESHOLD);
     prog_admisc(freq,bd,DSY,FIRSTOFDWELL,IANDQONLY);
     }
   }



