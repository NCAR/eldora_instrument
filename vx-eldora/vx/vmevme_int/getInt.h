/*
 *	$Id$
 *
 *	Module:		 getInt.h
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
#ifndef INCgetInt_h
#define INCgetInt_h

#ifdef __cplusplus
extern "C" {
#endif

#include "intGbls.h"

#include "logLib.h"
#include "msgQLib.h"
#include "intLib.h"

#include "Ray.h"
#include "Platform.h"
#include "FieldParam.h"

#ifndef scope
#define scope extern
#endif

scope MSG_Q_ID VMEQue;		/* Only this is visible. On an interrupt
				 * a handshake index is put in the queue.
				 */

void setupVMEIsr();		/* Call before using interrupts. */

#ifdef __cplusplus
};
#endif

#endif /* INCgetInt_h */

