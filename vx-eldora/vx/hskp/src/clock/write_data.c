/*
 *	$Id$
 *
 *	Module:	wrtdata.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1994/05/20  20:36:47  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  21:42:48  reif
 * Initial revision
 *
 *
 * description: The purpose of this module is to write data to any location
 *              on the time-of-day board;
 */


static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern
#include "hskpAll.h"

/***************** FUNCTION write_data() ********************/

void write_data() 
{
unsigned char *faddr;
unsigned char hsecond;
unsigned char data;
int baddr;

printf("ENTER THE ADDRESS TO WRITE TO\n");
scanf(" %x",&baddr);
getchar();

printf("Number just read in is %x\n",baddr);
faddr=(unsigned char *)(TOD_BASE + baddr);
printf("Address is %x\n",faddr);

printf("ENTER DATA\n");
scanf(" %x",&data);
getchar();

*faddr = data;
hsecond=*hsec_reg;
printf("Address= %x, data= %x\n",faddr,*faddr);

return;
}
