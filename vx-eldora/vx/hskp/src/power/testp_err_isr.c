/*
 *	$Id$
 *
 *	Module:	testp_err_isr.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/01  16:48:23  craig
 * Initial revision
 *
 *
 * description: The purpose of this module is to 
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <vxWorks.h>
#include <stdioLib.h>
#include <math.h>
#include <pwrDef.h>
#include <pwrFunc.h>

void testp_err_isr()
{
    testp_isr_done=-1;
    *d1csr=0xff;
    *d1ccr=0x48;
}

