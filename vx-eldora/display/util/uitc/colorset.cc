/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 colorset.cc
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1991/11/20  20:42:23  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "ColorSvr.hh"

extern "C" {
void colorset(void);
#include "stdioLib.h"
};

void colorset(void)
{
    GraphicController agc((void *)0xa0000000,1280,1024,2048,250);

    ColorSvr server(&agc);

    getchar();
}
