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
extern void rcio(int, char *);
extern void wcio(int, char *, unsigned char);

/* WRS functions not otherwise defined. */

#endif /* INCtp41Libh */

