/*
 *	$Id$
 *
 *	Module vme_isr.c 		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	date  $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1994/05/12  15:07:50  craig
 * Initial revision
 *
 *
 * description:  This interrrupt service for the vme to vme interfaces
 *               in the Housekeeping Processor these interrupts are caused
 *               by one of the radar processors.
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

void vme_isr()
{

    *fore_remote_command = 0x40;
    *aft_remote_command = 0x40;
    int_count++;

}
