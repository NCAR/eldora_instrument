/*
 *	$Id$
 *
 *	Module:	stop_vmevme	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/25  20:43:01  craig
 * Initial revision
 *
 * description: This module initializes the vme to vme interfaces and their
 *              handshake areas 
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern
#include "hskpAll.h"

void stop_vmevme()
{
/* Clear the radar processors are ready handshake words */

fore_vmehndshk->start_hndshk = 0;
aft_vmehndshk->start_hndshk = 0;

/* Clear the "Hi Radar" in the radar processors to get them  to stop */

fore_vmehndshk->salute[0] = ' ';
fore_vmehndshk->salute[1] = ' ';
fore_vmehndshk->salute[2] = ' ';
fore_vmehndshk->salute[3] = ' ';
fore_vmehndshk->salute[4] = ' ';
fore_vmehndshk->salute[5] = ' ';
fore_vmehndshk->salute[6] = ' ';
fore_vmehndshk->salute[7] = ' ';

aft_vmehndshk->salute[0] = ' ';
aft_vmehndshk->salute[1] = ' ';
aft_vmehndshk->salute[2] = ' ';
aft_vmehndshk->salute[3] = ' ';
aft_vmehndshk->salute[4] = ' ';
aft_vmehndshk->salute[5] = ' ';
aft_vmehndshk->salute[6] = ' ';
aft_vmehndshk->salute[7] = ' ';

}
