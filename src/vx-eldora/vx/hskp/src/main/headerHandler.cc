/*
 *	$Id$
 *
 *	Module:		 headerHandler.c
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1991/09/11  19:36:04  thor
 * Initial revision
 *
 *
 * description:
 *        This module handles the RPC request to download a tape
 * header. It first deletes the old headerand then creates a new one.
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";


#include "global.hh"
extern "C"{
#include "memLib.h"
#include "stdioLib.h"
    HeaderPtr inHeader;
}
static char junk;		/* Something to return. */

void *sendheader_1(FAST TAPEHEADER *th, struct svc_req *x)
{
    if (Hdr != NULL)
      delete(Hdr);

    Hdr = new Header(th);

    inHeader = (HeaderPtr)Hdr;
    return((void *)&junk);
}




