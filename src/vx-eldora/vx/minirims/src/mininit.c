/*
 *	$Id$
 *
 *	Module: mininit.c		 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1994/05/20  20:37:16  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/19  19:07:18  reif
 * Initial revision
 *
 *
 * description:
 *        
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include "minDef.h"

void mini_init()
{
unsigned short *reset;
unsigned short *ltch_cnt;

reset=(unsigned short *)(MIN_BASE+COUNT_RESET); 
ltch_cnt=(unsigned short *)(MIN_BASE+LATCH_COUNT);
*reset=0x00;    /* Select all counters */
*ltch_cnt=0x00; /* Reset selected counters */
return;
}
