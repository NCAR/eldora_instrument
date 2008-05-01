/*  This looks like C, but it's really C++!!
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
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "global.hh"

extern "C" {
#include "memLib.h"
};

static char junk;

void *sendheader_1(FAST TAPEHEADER *th, struct svc_req *x)
{
    if (Hdr == NULL)
      Hdr = new Header(th);
    else
      *Hdr = th;

    return((void *)&junk);
}
