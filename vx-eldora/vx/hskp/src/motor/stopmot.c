/*
 *
 *	$Id$
 *
 *	Module:	stopmot.c	 
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

void stop_motor()
{
unsigned char *prog_cntr;

prog_cntr=(unsigned char *)PROGRAM_COUNTER;
*prog_cntr=0x01; /* HAULT MOTOR*/
}










