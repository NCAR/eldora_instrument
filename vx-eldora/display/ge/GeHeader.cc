/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 GeHeader.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1991/10/09  15:38:25  thor
 * Removed unused wavecount stuff.
 *
 * Revision 1.1  1991/10/01  16:15:13  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "GeGlobal.hh"

static char junk;

void *sendheader_1(FAST TAPEHEADER *th, struct svc_req *x)
{
    if (Hdr != NULL)
      delete(Hdr);

    Hdr = new Header(th);

    return((void *)&junk);
}


    
