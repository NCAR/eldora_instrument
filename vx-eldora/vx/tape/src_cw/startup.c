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
 * Revision 1.3  1992/10/25  17:08:20  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/17  21:34:07  reif
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  17:52:04  reif
 * Initial revision
 *
 * Revision 1.1  1991/09/11  19:35:33  thor
 * Initial revision
 *
 *
 * description:
 *        This module is the entry point for the recording system. All
 * hard/software initialization is done here.
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC

#include "cipincl.h"

extern void startRpc(void);
static TapeStatus stat;

void startup(void)
{
    /* Set global flags to default state. */
    REC_FLAG = 0;
    REWIND_FLAG = 0;
    PP_FLAG=0;
    RUN_FLAG=0;
    UNLOAD_FLAG=0;
    PARALLEL_REC=1;
    bzero((char *)&stat,sizeof(stat)); /* Set initial status. */

    PACKSTATUS(0,&stat.status,0);
    PACKSTATUS(0,&stat.status,1);

    tapeStatus = &stat;		/* Set up global pointers. */

    /* do whatever's needed for tape unit initialization here. */

    startRpc();
}











