/*
 *	$Id$
 *
 *	Module: mcpl_gbl.h 		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date  $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:  Contains global variables for use with the Multi Crate
 *               Pipeline (MCPL)
 *
 */

#ifndef INCmcpl_gbl
#define INCmcpl_gbl

/* Reserve one space for entire MAD structure to transfer */
struct MAD gmad;

/* Declare all transfer parameter blocks (TPB's) to be used */

struct TPB  init;       /* Initializing Transfer Parameter Block (TPB) */
struct TPB  tpb_to_proc; /* TPB to send TPB to any target processor */
struct TPB  receive_gmad; /* TPB to receive a MAD structure */
struct TPB  gmad_to_proc; /* TPB to send a MAD structure to any processor */
struct TPB  receive_data; /* TPB to receive data on a remote node */
struct TPB  bcast_tpb; /* TPB to broadcast data to all processors */

/* Pointers to all defined TPB's */
long *init_pntr, *bcast_tpb_pntr, *receive_data_pntr;
long *tpb_to_proc_pntr,*gmad_to_proc_pntr,*receive_gmad_pntr;

/* Pointers to other MCPL registers, storage for register values */
short *status_reg, *control_reg, *short_fifo, *start_reg;
long *long_fifo, *long_pntr;
short status, control, start;

/* Arrays for address storage to mailboxes and data */

int number_of_mads[3]; /* Number of mads in each processor 0=> no processor */
int current_mad[3];    /* Current mad (Either 0 or 1) in each processor */
short proc_nodes[3];   /* Array of node numbers for each processor */
#define DIS 0          /* All above arrays display processor is entry 0 */
#define REC 1          /* All above arrays recording processor is entry 1 */
#define GRID 2         /* All above arrays gridding processor is entry 2 */

long madinfo[3][2][7];
/* First dimension is processor number display=0, record=1, gridding=2
   Second dimension is for each mad in a processor: 2 maximum, 1 minimum
   Third dimension is for various defined parameters:
   [P] [M] [0] base address of MAD M for processor P
   [P] [M] [1] Number of data areas in MAD M for processor P
   [P] [M] [2] Size of data areas in MAD M for processor P (in Bytes)
   [P] [M] [3] Completion status word for each different processor
   [P] [M] [4] Number of current data area in MAD M for processor P
   [P] [M] [6] Maximum Size of memory available for MAD M in processor P */

#define MAD_BASE     0
#define NUM_AREAS    1
#define SIZE_AREAS   2
#define COMPLETE_S   3
#define NUM_C_AREA   4
#define MAX_MEM      6

long *gmad_pntr;
void broadcast_data();

/* General purpose variables associated with the mcpl */
int logical_length;
short number_procs, mailbox;

#endif /* INCmcpl_gbl */
