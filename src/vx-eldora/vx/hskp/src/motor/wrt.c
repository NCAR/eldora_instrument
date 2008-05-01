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

void write_data()
{
unsigned char *full_addr;
unsigned int data;
int reg_addr;

puts("ENTER THE ADDRESS TO WRITE TO");
scanf(" %x",&reg_addr);
getchar();
printf("REGISTER WRITING TO IS %X\n",reg_addr);
full_addr=(unsigned char *)(MOTOR_BASE+reg_addr);
printf("FULL ADDRESS OF REGISTER IS %X\n",full_addr);
puts("ENTER DATA TO WRITE TO REGISTER");
scanf(" %x",&data);
getchar();
*full_addr=data;
taskDelay(1);
printf("DATA WRITTEN= %X  DATA IN REGISTER= %X\n",data,*full_addr); 
return;
}
