/*
 *	$Id$
 *
 *	Module:	init_vmevme	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * Revision 1.1  1992/08/14  20:44:35  craig
 * Initial revision
 *
 *
 * description: This module initializes the vme to vme interface vectors
 *              for use in the radar processors for use with the 
 *              vmevme diagnostics
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern

/* Include needed vx-works .h files */

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

void init_vmevme()
{


/* Setup all of the pointers to the VME to VME interface areas */

local_status_reg = (char *)(LOCAL_STATUS_REG + VMEVME_SHORT_START +
			    SHORT_BASE);
local_command_reg = (char *)(LOCAL_COMMAND_REG +
			      VMEVME_SHORT_START + SHORT_BASE);
remote_status_reg = (char *)(REMOTE_STATUS_REG +
			      VMEVME_SHORT_START + SHORT_BASE);
remote_command_reg = (char *)(REMOTE_COMMAND_REG +
			       VMEVME_SHORT_START + SHORT_BASE);
}
