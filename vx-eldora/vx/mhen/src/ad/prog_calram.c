/*
 *      $Id$
 *
 *      Module: PROG_CALRAM.c
 *      Original Author: Mitch Randall
 *      Copywrited by the National Center for Atmospheric Research
 *      Date:            $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/11/09  17:00:28  eric
 * Initial revision
 *
 *
 * description: This module programs the Calibration Ram of the ADC
 *              card based on a slope and offset provided as arguments.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*******************************  prog_calram.c  ****************************/
/*                                                                          */
/*        Program to set the cal ram in the A/D CONVERTER MODULE            */
/*     The following definitions apply:                                     */
/*                                                                          */
/*     freq                       frequency number  (1,2,or3)               */
/*     bd                         board number (always 4)                   */
/*     hi_lo                      0 -> high channel,   1 -> low channel     */
/*     q_i                        0 -> Q,  1 -> I,  2 -> Z                  */
/*     m,b                        bipolar slope and offset                  */
/*                                                                          */
/****************************************************************************/

#include "vxWorks.h"
#include "stdioLib.h"
#include "math.h"
#include "ELDRP7.h"

void prog_calram(unsigned long, unsigned long, char, char, float, float);
void prog_calram(unsigned long freq, unsigned long bd, char hi_lo, char q_i,
		 float m, float b)
   {
   unsigned long addr;
   long   x,*p;

   printf("%ld %ld %hd %hd %6.4f %6.4f\n",freq,bd,hi_lo,q_i,m,b);

   if(bd != 4)  printf("not a valid board in call to function prog_calram\n");
   if(freq > 5) printf("not a valid freq in call to function prog_calram\n");
   addr = (freq << 24) + EXTD_BASE + (bd << 20);
   addr += (q_i<2)?((0x8000*q_i)+(0x4000-0x4000*hi_lo)):0x30000;
   p = (long *)addr;

   for(x=-2048; x<2048; x++)
     *(p + (x & 0xfffL)) = i3e2dsp(m * x + b);
   }







