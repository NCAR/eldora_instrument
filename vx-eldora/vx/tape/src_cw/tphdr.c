
/*
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
 * Revision 1.1  1994/01/06  21:31:40  craig
 * Initial revision
 *
 * Revision 1.3  1992/10/25  17:08:18  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/17  21:34:03  reif
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  17:52:04  reif
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";


#include <cipincl.h>

unsigned int tape_header()
{


if((hdrsz=GetRealHeader(Hdr,tapeHdr))==0)
  {
      printf("DID NOT RECEIVE A HEADER AFTER START!!\n");
      exit(0);
  }
return(hdrsz);
}










