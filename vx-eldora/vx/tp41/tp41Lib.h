/*
 *	$Id$
 *
 *	Module:		 tp41Lib.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1992/03/04  17:11:20  thor
 * Added new code to dual port in standard i/o space.
 *
 * Revision 1.2  1992/01/28  15:53:47  thor
 * Added include of sysLib.h.
 *
 * Revision 1.1  1992/01/28  15:44:44  thor
 * Initial revision
 *
 *
 *
 * description:
 *        Additional function WRS should have included!
 *
 */
#ifndef INCtp41Libh
#define INCtp41Libh

#include "vxWorks.h"
#include "stdioLib.h"
#include "memLib.h"
#include "vme.h"
#include "tp41.h"
#include "sysLib.h"

/* Local extensions. */
extern STATUS dport(char *localAddr, void *vmeAddr, int numSegs);
extern STATUS std_dport(char *localAddr, void *vmeAddr, int numSegs);
extern STATUS myDualPort(char *localAddr, char *vmeAddr, int addrSpace,
			 int mbox);
extern STATUS createMailbox(void *localAddr, void *vmeAddr);
extern STATUS createStdMailbox(void *localAddr, void *vmeAddr);
extern STATUS connectMailbox(VOIDFUNCPTR function, int arg, int mailbox);
extern void rcio(int, char *);
extern void wcio(int, char *, unsigned char);

#define VME_EXT 0
#define VME_STD 1

/* WRS functions not otherwise defined. */

#endif /* INCtp41Libh */

