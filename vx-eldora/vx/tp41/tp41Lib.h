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
 * Revision 2.0  1992/11/02  20:42:08  thor
 * First offical ELDORA release!
 *
 * Revision 1.5  1992/08/24  18:16:50  thor
 * Added conditional around includes to avoid user dragging in obscure .h
 * files.
 *
 * Revision 1.4  1992/08/24  18:10:36  thor
 * Added new routines.
 *
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

#ifdef TP41LIB
#include "stdioLib.h"
#include "memLib.h"
#include "vme.h"
#include "tp41.h"
#include "sysLib.h"
#endif /* TP41LIB */

/* Local extensions. */

/* Dual porting. */
extern STATUS dport(char *localAddr, void *vmeAddr, int numSegs);
extern STATUS std_dport(char *localAddr, void *vmeAddr, int numSegs);
extern STATUS myDualPort(char *localAddr, char *vmeAddr, int addrSpace,
			 int mbox);

/* mailboxes */
extern STATUS createMailbox(void *localAddr, void *vmeAddr);
extern STATUS createStdMailbox(void *localAddr, void *vmeAddr);
extern STATUS connectMailbox(VOIDFUNCPTR function, int arg, int mailbox);

/* CIO manipulation */
extern int rcio(int, char *);
extern STATUS wcio(int, char *, unsigned char);

/* Diddle page bits. */
extern STATUS setTp41VMEPage(int page);
extern int getTp41VMEPage(void);

#define VME_EXT 0
#define VME_STD 1

#endif /* INCtp41Libh */

