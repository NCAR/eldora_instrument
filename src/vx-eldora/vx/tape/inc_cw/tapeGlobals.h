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
 * Revision 1.4  1996/02/09  17:45:31  craig
 * *** empty log message ***
 *
 * Revision 1.3  1995/01/26  16:22:35  craig
 * *** empty log message ***
 *
 * Revision 1.2  1994/11/14  17:58:23  craig
 * *** empty log message ***
 *
 * Revision 1.1  1994/01/06  21:39:16  craig
 * Initial revision
 *
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

TAPE_CTRL_SCOPE int UNLOAD_FLAG; /* UNLOAD DRIVE? 1 = yes. */
TAPE_CTRL_SCOPE int RUN_FLAG;
TAPE_CTRL_SCOPE int REC_FLAG;
TAPE_CTRL_SCOPE int REWIND_FLAG;
TAPE_CTRL_SCOPE int LOAD_FLAG;


TAPE_CTRL_SCOPE int WRITE_TAPE_STATUS;
TAPE_CTRL_SCOPE struct TapeStatus *tapeStatus; /* Global status block. */
TAPE_CTRL_SCOPE HeaderPtr inHeader; /* Global header. */
TAPE_CTRL_SCOPE int new_volume;        /* Flag to indicate a new volume has
					begun */
TAPE_CTRL_SCOPE int drives_to_use[2];   /* Current drive number(s) in use */
TAPE_CTRL_SCOPE int number_of_drives;   /* number of drives in use */
TAPE_CTRL_SCOPE int physical_unit[2][2]; /*SCSI bus unit numbers of drives */
TAPE_CTRL_SCOPE int record_sys[2][3]; /*Descrip. of two recording systems */
TAPE_CTRL_SCOPE int current_unit;     /* Recording system unit number in use*/
TAPE_CTRL_SCOPE int next_unit;        /* Next recording system unit number
					 to use*/
TAPE_CTRL_SCOPE long max_blk_sz;        /* maximum size of data records 
					   to write to tape */
TAPE_CTRL_SCOPE float amount_written[2]; /* Amount of data written to tape on
					  each recording system */
TAPE_CTRL_SCOPE int min_to_record;    /* minimum number of bytes to record on
				       each ray of data */
TAPE_CTRL_SCOPE VOLUME *vol;          /* Pointer to complete volume header */
TAPE_CTRL_SCOPE RADARDESC *rad_dscr;  /* Pointer to radar descriptor */
TAPE_CTRL_SCOPE CELLSPACING *cs;      /* Pointer to cell spacing descriptor */
TAPE_CTRL_SCOPE VOLUME *head;         /* Pointer to volume header */
TAPE_CTRL_SCOPE WAVEFORM *wave;      /* Pointer to waveform descriptor */
TAPE_CTRL_SCOPE char log_chars[90];   /* Array to hold characters to be sent
					 to the log file */
TAPE_CTRL_SCOPE int log_ints[10];     /* Array to hold integer values to be
					 sent to the log file, by definition
					 during run time, the first three
					 values are hour, minute, second */
TAPE_CTRL_SCOPE int calibrate;        /* Flag to switch over to the
					 calibrate mode */
TAPE_CTRL_SCOPE int reduce_gate[361]; /* number of gates to record given data
					 reduction */
TAPE_CTRL_SCOPE float alt_fact[361]; /* Altitude scale factor */

#endif /* INCtapeCtrlGblh */






