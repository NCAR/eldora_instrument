/*
 *	$Id$
 *
 *	Module init_vmevme.c 		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	date  $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:  This function initializes the handshake areas with the
 *               radar processors.
 */

#define OK_RPC
#define scope extern
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/* Include needed vx-works .h files */

#include <vxWorks.h>
#include <stdioLib.h>
#include <math.h>
#include <vmevmeAdr.h>
#include <tp41vAdr.h>

void init_vmevme()
{
char *local_command_reg;

local_command_reg = (char *)(LOCAL_COMMAND_REG + 
                               SHORT_BASE + VMEVME_SHORT_START);
*local_command_reg = 0x40;

}
