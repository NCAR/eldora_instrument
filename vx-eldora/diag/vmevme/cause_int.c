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
 *
 * description:  This program causes an interrupt in the Housekeeping
 *               Processor when run in either of the radar processors
 *               This code should be run in the fore radar processor
 *               When answering the middle of beam interrupt
 *
 */

#define OK_RPC
#define scope
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include </vx/h/vxWorks.h>
#include </vx/h/stdioLib.h>
#include </vx/h/math.h>
#include </ELDORA/vx/hskp/include/vmevmeAdr.h>
#include </ELDORA/vx/hskp/include/tp41vAdr.h>

cause_int()
{
char *local_command_reg;

local_command_reg = (char *)(LOCAL_COMMAND_REG + 
                               SHORT_BASE + VMEVME_SHORT_START);
*local_command_reg = 0x40;

}
