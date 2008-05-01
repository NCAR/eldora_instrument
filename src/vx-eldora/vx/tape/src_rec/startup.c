/*
 *	$Id$
 *
 *	Module:		 startup.c
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1996/06/24 22:59:53  craig
 * Initial revision
 *
 *
 * description:  This module is the entry point for the recording system. All
 *               hard/software initialization is done here.
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC

#include <recordIncs.h>
#include <tapeDef.h>
#include <tapeGbl.h>

extern void startRpc(void);
static TapeStatus stat;

void startup(void)
{
    /* Set global flags to default state. */
    REC_FLAG = 0;
    REWIND_FLAG = 0;
    RUN_FLAG=0;
    UNLOAD_FLAG=0;
    bzero((char *)&stat,sizeof(stat)); /* Set initial status. */

    tapeStatus = &stat;		/* Set up global pointers. */

    /* do whatever's needed for tape unit initialization here. */

    startRpc();
}












