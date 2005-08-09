/*
 *	$Id$
 *
 *	Module: cip_init		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1996/06/18  16:02:58  craig
 * Initial revision
 *
 *
 * description:  Completely resets the Ciprico Board
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <tapeDef.h>
#include <tapeGbl.h>

void cip_init()
{
unsigned short *reset;
unsigned short *status;
int stat;

reset = (unsigned short *)(CIP_BASE+SHORT_BASE+CIP_BD_RST);
status = (unsigned short *)(CIP_BASE+SHORT_BASE+CIP_BD_STAT);

printf("Reseting the Ciprico Board (which causes a SCSI reset also)\n");

*reset = 0; /* SET CONTROLLER TO KNOWN STATE */

while(*status!=0x202); /* CHECK FOR GOOD BOARD STATUS */

printf("STATUS OF %X RETURNED FROM %X\n",*status,status);
printf("CIPRICO BOARD IS READY\n");

}
