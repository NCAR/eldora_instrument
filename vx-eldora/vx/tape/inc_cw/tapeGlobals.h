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
 * Revision 1.1  1991/09/11  19:36:11  thor
 * Initial revision
 *
 *
 *
 * description:
 *        These are variables that are needed throughout the recording
 * system. 
 */
#ifndef INCtapeCtrlGblh
#define INCtapeCtrlGblh

#ifndef TAPE_CTRL_SCOPE
#define TAPE_CTRL_SCOPE extern
#endif /*  TAPE_CTRL_SCOPE */

#ifndef OK_RPC
#define OK_RPC
#endif /* OK_RPC */

#include "tapeControl.h"
#include "Header.h"

TAPE_CTRL_SCOPE int PP_FLAG; /* Pingpong drives? 1 = yes. */
TAPE_CTRL_SCOPE int UNLOAD_FLAG; /* UNLOAD DRIVE? 1 = yes. */
TAPE_CTRL_SCOPE int RUN_FLAG;
TAPE_CTRL_SCOPE int REC_FLAG;
TAPE_CTRL_SCOPE int REWIND_FLAG;
TAPE_CTRL_SCOPE int UNIT_NUM;
TAPE_CTRL_SCOPE int PARALLEL_REC;
TAPE_CTRL_SCOPE int CLIPPING;
TAPE_CTRL_SCOPE int SLICE;
TAPE_CTRL_SCOPE int THRSHLD;
TAPE_CTRL_SCOPE int WRITE_TAPE_STATUS;
TAPE_CTRL_SCOPE struct TapeStatus *tapeStatus; /* Global status block. */
TAPE_CTRL_SCOPE HeaderPtr inHeader; /* Global header. */
TAPE_CTRL_SCOPE int drives_to_use[2];   /* Current drive number(s) in use */
TAPE_CTRL_SCOPE int number_of_drives;   /* number of drives in use */
TAPE_CTRL_SCOPE int physical_unit[2][2]; /*SCSI bus unit numbers of drives */
TAPE_CTRL_SCOPE int record_sys[2][3]; /*Descrip. of two recording systems */
TAPE_CTRL_SCOPE int starting_amnt[2]; /* Starting amount of tape on each
					 recording system */
TAPE_CTRL_SCOPE VOLUME *vol;          /* Pointer to complete volume header */
TAPE_CTRL_SCOPE RADARDESC *rad_dscr;  /* Pointer to radar descriptor */
TAPE_CTRL_SCOPE CELLSPACING *cs;      /* Pointer to cell spacing descriptor */

#endif /* INCtapeCtrlGblh */

