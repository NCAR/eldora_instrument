/*
 *	$Id$
 *
 *	Module: chan_attn		 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:  Issues a channel attention to the Ciprico Board.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <tapeDef.h>
#include <tapeGbl.h>

/******************** CHANNEL ATTENTION **********************/

void chan_attn(int chan)
{
short int *chan_ap;

chan_ap=(short int *)(SHORT_BASE + CIP_BASE + CIP_CHAN_ATTN);
*chan_ap=0;

return;
}
