/*

 *	$Id$
 *
 *	Module:	motor.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/14  21:34:31  reif
 * Initial revision
 *
 *
 * description:
 *             
 *             
 *              
 *              
 *             
 *             
 *             
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <cntrlIncl.h>

void read_data()
{
unsigned char *full_addr;
unsigned int data;
int reg_addr;

puts("ENTER THE ADDRESS TO READ FROM");
scanf(" %x",&reg_addr);
getchar();
printf("REGISTER READING FROM IS %X\n",reg_addr);
full_addr=(unsigned char *)(MOTOR_BASE+reg_addr);
printf("FULL ADDRESS OF REGISTER IS %X\n",full_addr);
printf("DATA IN REGISTER %0X = %0X\n",full_addr,*full_addr);
return;
}

