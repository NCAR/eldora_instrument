/*
 *	$Id$
 *
 *	Module:	TIMON	 
 *	Original Author: Shawn B. Copeland
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1992/09/10  20:54:32  shawn
 * Changed value written to 0x01, from 0x00, to accomodate timmod rev. b
 * too.  Rev. A had two addresses, an on and an off, Rev. B has one, and
 * you write a 1 to turn it on and a 0 to turn it off.  Thus, timon will
 * now work with both cards.  timoff will be left alone, since for rev.
 * A there will be a seperate address to write to, and for rev. B it will
 * be the same one as to turn it on.  There will also be created two new
 * modules, timonb and timoffb, to be used exclusively with the rev. B
 * board.  This change accompanies a change to ELDRP7.h in which a line
 * was added to define the offset to the rev. B ON/OFF address, namely,
 * a #define TIMONOFFB.
 *
 * The weather was quite nice while I was making these changes, unlike
 * the other staff member who had to be threatened with root access just
 * to do a simple thing like check in a piece of my code.
 *
 * Revision 1.2  1992/09/10  20:40:43  eric
 * *** empty log message ***
 *
 * Revision 1.1  1991/12/04  19:06:43  eric
 * Initial revision
 *
 *
 * description:
 *        
 *  Starts the Timing Module
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

  
#include "ELDRP7.h"

timon()
{
   unsigned short *timon;
 /* map Old Timing Module in VME Extended Space */ 
   timon = (unsigned short *)(TIMBASE - STD_BASE + EXTD_BASE + TIMON);
   *timon = 0x0001;  /* changed value to 0x01 to accomodate rev. B board */
}
