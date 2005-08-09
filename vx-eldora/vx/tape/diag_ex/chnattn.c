/*
 *	$Id$
 *
 *	Module:		 
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1992/10/25  17:07:55  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/17  21:31:56  reif
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  17:50:02  reif
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>

/******************** CHANNEL ATTENTION **********************/

void chan_attn(int chan)
{
short int *chan_ap;

chan_ap=(short int *)(CIP_BASE+CIP_CHAN_ATTN);
*chan_ap=0;
return;
}
