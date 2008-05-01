/*
 *	$Id$
 *
 *	Module:	tod2.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1994/05/20  20:36:41  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/30  20:12:54  craig
 * Initial revision
 *
 * description: The purpose of this module is to read data from any location
 *              on the time of day (tod) board.
 */


static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern
#include "hskpAll.h"

/**************** FUNCTION read_data() *********************/

void read_data() 
{
unsigned char *faddr;
unsigned char data;
unsigned char hsecond;
int baddr;

printf("ENTER ADDRESS TO READ FROM\n");
scanf(" %x",&baddr);
getchar();

faddr = (unsigned char *)(TOD_BASE + baddr);

hsecond=*hsec_reg;
printf("Address is %x, data is %x\n",faddr,*faddr);

return;
}
