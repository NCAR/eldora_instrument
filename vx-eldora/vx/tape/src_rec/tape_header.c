
/*
 *	$Id$
 *
 *	Module: tape_header
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:  Gets a pointer to a completely packed tape header
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <recordIncs.h>
#include <tapeDef.h>
#include <tapeGbl.h>

unsigned int tape_header()
{


if((hdrsz=GetRealHeader(Hdr,tapeHdr))==0)
  {
      printf("DID NOT RECEIVE A HEADER AFTER START!!\n");
      exit(0);
  }
return(hdrsz);
}










