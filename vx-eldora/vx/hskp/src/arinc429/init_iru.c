/*
 *	$Id$
 *
 *	Module:	init_iru	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/19  15:40:59  craig
 * Initial revision
 *
 * description: This module initializes the Inertial Reference Unit (IRU)
 *              interface and it pointers etc.  This interface is a ARINC429
 *              serial bus interface designed at NCAR.
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern

#include "hskpAll.h"

void init_iru(short interrupt)
{
char *char_pntr;

iru_rpntr = (struct iru_rcontrol *)(IRU_BASE + STANDARD_BASE + IRU_RXA1); 
iru_hi8282a = (short *)(IRU_BASE + STANDARD_BASE + IRU_HI8282A);

/* Set up interface to interrupt on either 300 words or the longitude label */

iru_rpntr->max_words = 300;
iru_rpntr->term_label = LONGITUDE_LAB;
*iru_hi8282a = 0x20;

/*  Setup for interrupts, (if requested) */

iru_rpntr->int_flag = 0;
if(interrupt)
  {
      iru_rpntr->int_flag = 1;
      char_pntr = (unsigned char *)(IRU_BASE + STANDARD_BASE + IRU_BIM
                   + BIMCR0);
      *char_pntr = (char)(ARINC_IRQ + 0x10);
      char_pntr = (unsigned char *)(IRU_BASE + STANDARD_BASE + IRU_BIM
                   + BIMVR0);
      *char_pntr = ARINC_VEC;

      /* Will have to attached iru_isr to the interrupt here before
         actually using the interrupt feature */
  }

}





