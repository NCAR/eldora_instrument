/*
 *	$Id$
 *
 *	Module:		 tapeCtrlGbl.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1991/08/28  19:49:42  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCtapeCtrlGblh
#define INCtapeCtrlGblh

#ifndef TAPE_CTRL_SCOPE
#define TAPE_CTRL_SCOPE extern
#endif /*  TAPE_CTRL_SCOPE */

TAPE_CTRL_SCOPE int PingPongFlag;
TAPE_CTRL_SCOPE int RecordingFlag;
TAPE_CTRL_SCOPE int FixedUnitFlag;

TAPE_CTRL_SCOPE struct TapeStatus *tapeStatus;



#endif /* INCtapeCtrlGblh */

