/*
 *
 *	$id: go_motor.c,v 1.1 1992/09/01 20:42:05 craig Exp craig $
 *
 *	Module:	go_motor.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/01  20:42:05  craig
 * Initial revision
 *
 * description: This function will cause the motor to start up with all of
 *              its presently configured parameters;
 *             
 *             
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define scope extern

/* Include fifty million vx-works .h files */

#include "vxWorks.h"
#include "math.h"
#include "stdioLib.h"
#include "intLib.h"
#include "memLib.h"
#include "semLib.h"
#include "taskLib.h"
#include "tyLib.h"
#include "ioLib.h"
#include "in.h"
#include "systime.h"
#include "sysLib.h"


/* include the .h files that are housekeeper code specific */

#include "tp41vAdr.h"
#include "cntrlDef.h"
#include "cntrlGbl.h"
#include "cntrlFunc.h"

void go_motor()
{

*mot_prog_cntr=0x03;     /* GO */

}
