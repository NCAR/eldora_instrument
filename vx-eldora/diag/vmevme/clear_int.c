/*
 *	$Id$
 *
 *	Module clear_int.c 		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	date  $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:  This program clears an interrupt in the Housekeeping
 *               Processor caused by one of the radar processors.
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

clear_int()
{
char *local_status_reg, *remote_command_reg, *remote_status_reg;

local_status_reg = (char *)(LOCAL_STATUS_REG + SHORT_BASE + FORE_SHORT_START);
remote_command_reg = (char *)(REMOTE_COMMAND_REG + SHORT_BASE + 
                      FORE_SHORT_START);
remote_status_reg = (char *)(REMOTE_STATUS_REG + SHORT_BASE +
                      FORE_SHORT_START);

printf("Fore Radar Local Status = %x \n",*local_status_reg);
printf("Fore Radar Remote Status = %x \n",*remote_status_reg);
*remote_command_reg = 0x40;

local_status_reg = (char *)(LOCAL_STATUS_REG + SHORT_BASE + AFT_SHORT_START);
remote_command_reg = (char *)(REMOTE_COMMAND_REG + SHORT_BASE + 
                      AFT_SHORT_START);
remote_status_reg = (char *)(REMOTE_STATUS_REG + SHORT_BASE + AFT_SHORT_START);

printf("Aft Radar Local Status = %x \n",*local_status_reg);
printf("Aft Radar Remote Status = %x \n",*remote_status_reg);
*remote_command_reg = 0x40;

}
