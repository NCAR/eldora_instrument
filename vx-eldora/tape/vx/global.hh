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
#ifndef INCglobalhh
#define INCglobalhh

#define OK_RPC
#define WRS_SIDE

extern "C" {
#include "vxWorks.h"
#include "TapeHeader.h"
};

#include "Header.hh"

#ifndef GBL_SCOPE
#define GBL_SCOPE extern
#endif

GBL_SCOPE Header *Hdr;

#endif // INCglobalhh
