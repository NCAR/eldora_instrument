/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 DataGram.cc
 *	Original Author: Richard E. Neitzel
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

#include "DataGram.hh"

DataGram::DataGram(int port, char *host) : Socket(SOCK_DGRAM)
{
    if (Getsockd() < 0)
      return;

    if (host == NULL)
      Bind(port);
    else
      Bind(port,host);
}
