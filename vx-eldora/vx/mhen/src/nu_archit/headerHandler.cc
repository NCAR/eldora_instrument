/*
 *	$Id$
 *
 *	Module:	headerHandler	 
 *	Original Author: Eric Loew
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/11/09  16:07:07  eric
 * Initial revision
 *
 * Revision 1.1  1991/12/09  22:13:15  eric
 * Initial revision
 *
 *
 * description:  This module handles RPC request to download a radar processor
 *               header. It first deletes the old headerand then creates a new
 *               one.
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#include "Header.hh"

extern "C"{
#include "vxWorks.h"
#include "stdioLib.h"
#include "RDPGlobals.h"
};
extern Header *Hdr;
static char junk;  /* something to return */
void *sendheader_1(FAST TAPEHEADER *tp,struct svc_req *req)
{
    if(Hdr == NULL)
      {
	  Hdr = new Header(tp);
	  inHeader = (HeaderPtr )Hdr;
      }
    else
      *Hdr = tp;
    return((void *)&junk);
}



