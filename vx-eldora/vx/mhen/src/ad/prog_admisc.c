/*
 *      $Id$
 *
 *      Module: PROG_ADMISC.c
 *      Original Author: Mitch Randall
 *      Copywrited by the National Center for Atmospheric Research
 *      Date:            $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description: This module programs severam miscellaneous ADC Carc
 *              functions. These include daisy chain, first hit or
 *              every hit, and produce I,Q or I,Q,Z.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/******************************  prog_admisc.c  ***************************/
/*   the following definitions apply:                                     */
/*      daisy           number of gates before passing daisy chain        */
/*      every_hit       0 -> each hit    1 -> use first choice thru dwell */
/*      iqz             0 -> use IQZ     1 -> use I and Q only            */
/**************************************************************************/

#include "vxWorks.h"
#include "stdioLib.h"
#include "ELDRP7.h"

void prog_admisc(unsigned long, unsigned long, short, char, char);
void prog_admisc(unsigned long freq, unsigned long bd, short daisy,
                 char every_hit, char iqz)
   {
   short *daisychain,*modesel;

   if(bd!=4) printf("not a valid board in function prog_admisc\n");
   if(freq>5) printf("not a valid freq in function prog_admisc\n");
   daisychain = (short *)((freq<<24)+EXTD_BASE+(bd<<20)+ATOD_DC);   
   modesel = (short *)((freq<<24)+EXTD_BASE+(bd<<20)+ATOD_M);

   *daisychain = daisy;
   *modesel = iqz + (every_hit * 2);
   }
