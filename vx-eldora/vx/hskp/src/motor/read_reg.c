/*
 *	$Id$
 *
 *	Module:	read_reg.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/01  20:42:15  craig
 * Initial revision
 *
 *
 * description:  Reads a specific address on the motion control card
 *             
 *             
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern

/* Include fifty million vx-works .h files */

#include "vxWorks.h"
#include "math.h"
#include "stdioLib.h"
#include "intLib.h"
#include "memLib.h"
#include "semLib.h"
#include "taskLib.h"
#include "tyLib.h"
#include "ioLib.h"
#include "in.h"
#include "systime.h"
#include "sysLib.h"

/* include the .h files that are housekeeper code specific */

#include "tp41vAdr.h"
#include "cntrlDef.h"
#include "cntrlGbl.h"
#include "cntrlFunc.h"

void read_reg()
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

