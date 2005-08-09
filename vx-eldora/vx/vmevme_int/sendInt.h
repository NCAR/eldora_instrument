/*
 *	$Id$
 *
 *	Module:		 sendInt.h
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
 *        Send a PS interrupt over the VME-VME card.
 */
#ifndef INCsendInt_h
#define INCsendInt_h

#ifdef __cplusplus
extern "C" {
#endif

#include "intGbls.h"
#include "tp41vAdr.h"
#include "vmevmeAdr.h"

void triggerVMEInt(int hbox);	/* Parameter is current  */

#ifdef __cplusplus
};
#endif

#endif /* INCsendInt_h */

