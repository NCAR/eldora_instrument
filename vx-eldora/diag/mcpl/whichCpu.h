/*
 *	$Id$
 *
 *	Module:		 whichCpu.h
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
#ifndef INCwhichCpu_h
#define INCwhichCpu_h

#ifdef __cplusplus
extern "C" {
#endif

#include <vxWorks.h>
#include <sysLib.h>

static const int TP41_CPU     = 0x0;
static const int MVME133A_CPU = 0x1;
static const int UNKNOWN_CPU  = 0x2;

#ifdef __cplusplus
};
#endif

#endif /* INCwhichCpu_h */
