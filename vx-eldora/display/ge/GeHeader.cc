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
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "GeGlobal.hh"

static char junk;

void *sendcounts_1(WAVECOUNTS *wc, struct svc_req *req)
{
    return((void *)&junk);
}

void *sendheader_1(FAST TAPEHEADER *th, struct svc_req *x)
{
    if (Hdr != NULL)
      delete(Hdr);

    Hdr = new Header(th);

    return((void *)&junk);
}


    
