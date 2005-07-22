/*
 *	$Id$
 *
 *	Module cause_int.c 		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	date  $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1994/05/12  15:07:55  craig
 * Initial revision
 *
 *
 * description:  This program causes an interrupt in the Housekeeping
 *               Processor when run in either of the radar processors
 *               This code should be run in the fore radar processor
 *               When answering the middle of beam interrupt
 *
 */


static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/* Include needed vx-works .h files */

#define scope extern
#include <vxWorks.h>
#include <stdioLib.h>
#include <math.h>
#include <vmevmeDiag.h>
#include <vmevmeAdr.h>
#include <vme_hndshk.h>
#include <tp41vAdr.h>
#include <vmevmeDef.h>
#include <vmevmeGbl.h>
#include <hskpInt.h>

cause_int()
{

*local_command_reg = 0x20;

}
