/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 rspStart.cc
 *	Original Author: Eric Loew
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * 
 * Initial revision
 *
 *
 *
 * description:
 *       This module starts both ELDORA Radar Signal Processors.
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "startup.h"

void hostStartup(void)
{
    // Add any other hosts.
    hostAdd("loki","128.117.85.7");
    // Uncomment for dual porting 68040
    // dport(0x40200000,0x6000000,4);
    // wcio(0,"a",0xc2);
    wcio(0,"a",0xc6); // configure Tadpole Card
    startup();
    go();
}
