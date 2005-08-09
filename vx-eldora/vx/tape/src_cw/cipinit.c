/*
 *	$Id$
 *
 *	Module:		 
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1994/01/06  21:31:28  craig
 * Initial revision
 *
 * Revision 1.3  1992/10/25  17:08:01  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/17  21:32:35  reif
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  17:50:09  reif
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>
/******************* SETUP DRIVE #0 *********************/

void cip_init()
{
unsigned short *reset;
unsigned short *status;
int stat;

reset = (unsigned short *)(CIP_BASE+CIP_BD_RST);
status = (unsigned short *)(CIP_BASE+CIP_BD_STAT);
printf("I'M IN CIP INIT!!!!\n");
*reset = 0; /* SET CONTROLLER TO KNOWN STATE */
printf("CIPRICO AND EXABYTE RESET\n");
while(*status!=0x202); /* CHECK FOR GOOD BOARD STATUS */
printf("STATUS OF %X RETURNED FROM %X\n",*status,status);
printf("CIPRICO BOARD IS READY\n");
}
