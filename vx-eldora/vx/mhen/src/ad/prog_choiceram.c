/*
 *      $Id$
 *
 *      Module: PROG_CHOICERAM.c
 *      Original Author: Mitch Randall
 *      Copywrited by the National Center for Atmospheric Research
 *      Date:            $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description: This module programs the Choiceram of the ADC Card given
 *              a threshold value as an argument.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*************************** prog_choiceram.c  *****************************/
/*                                                                         */
/* Program to set the calibration function in the A/D CONVERTER MODULE     */
/* The following definitions apply:                                        */
/*      freq                 frequency number (1,2,or 3)                   */
/*      bd                   board number (always 4)                       */
/*      threshold            level (in counts) above which hi is selected  */
/*                                                                         */
/***************************************************************************/

#include "vxWorks.h"
#include "stdioLib.h"
#include "math.h"
#include "ELDRP7.h"

#define   LOW     0
#define   HIGH    1

/* set the choice  memory based on the threshold */

void   prog_choiceram(unsigned long, unsigned long, long);
void   prog_choiceram(unsigned long freq, unsigned long bd,long threshold)
   {
   unsigned char *atod_c;
   long     qhi,ihi,ltemp,k;

   threshold *= threshold;

   if(bd!=4) printf("not a valid board in function prog_choiceram\n");
   if(freq>5) printf("not a valid freq in function prog_choiceram\n");
   atod_c = (unsigned char *)((freq<<24)+EXTD_BASE+(bd<<20)+ATOD_CH);

   for(k = 1 ; k < 0x20000 ; k = k + 2) /* k is offset to the memory */
     {
     ltemp = k / 2;        
     ltemp ^= 0x8000;           /* invert the m.s. bit to correspond */
                                /* to the VME address decode error */

     /* compute the corresponding value of Q High (upper 8 bits only) */
     qhi = (ltemp & 0xff) <<4;  /* AND off upper eight bits & shift up 4 */
     if (qhi & 0x800)           /* if the sign bit is set, get magnitude */
       qhi = (~qhi & 0xfff) + 1;

     /* compute the corresponding value of I High (upper 8 bits only) */
     ihi = (ltemp & 0xff00) >>4;  /* AND off lower eight bits & shift down 4 */
     if (ihi & 0x800)           /* if the sign bit is set, get magnitude */
       ihi = (~ihi & 0xfff) + 1;

     *(atod_c + k) = (qhi*qhi+ihi*ihi >= threshold) ? HIGH : LOW;
     }
   }




