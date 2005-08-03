/*
 *	$Id$
 *
 *	Module:		 RadarRpcUtil.c
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "RadarGbls.h"

void SetMcplStatus(FAST int offender, FAST u_short status)
{
    currStatus->mcpl = 0;

    currStatus->mcpl |= (u_long)status & 0xffff;

    currStatus->mcpl |= offender << 16;;
}

void 
    
