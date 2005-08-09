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
 * Revision 1.1  1991/12/09  18:25:31  eric
 * Initial revision
 *
 *
 * description:  Contains global variables for use with the Multi Crate
 *               Pipeline (MCPL)
 *
 */

#ifndef INCmcpl_gbl
#define INCmcpl_gbl
#ifndef RP7_SCOPE
#define RP7_SCOPE extern
#endif /* RP7_SCOPE */
#include "vme_hndshk.h"
#include "TimeSeries.h"
#include "IndFreq.h"
#include "raw_data.h"

/* Reserve one space for entire MAD structure to transfer */
struct MAD *gmad;

/* Declare pointers to all transfer parameter blocks (TPB's) to be used */

struct TPB  *init;       /* Initializing Transfer Parameter Block (TPB) */
struct TPB  *tpb_to_proc; /* TPB to send TPB to any target processor */

struct TPB  *receive_gmad; /* TPB to receive a MAD structure */
struct TPB  *gmad_to_proc; /* TPB to send a MAD structure to any processor */
struct TPB  *receive_data; /* TPB to receive data on a remote node */
struct TPB  *bcast_tpb; /* TPB to broadcast data to all processors */
struct DATA_RAY  *ray;         /* A ray of data */
INDEP_FREQ *i_f;    /* Independent Frequency data */
TIME_SERIES *t_s;   /* Time Series data */
struct RAW_D *raw_d;    /* Raw data */
volatile short  *mailbox;

/* Pointers to other MCPL registers, storage for register values */

RP7_SCOPE volatile short *status_reg, *control_reg, *short_fifo, *start_reg;
RP7_SCOPE volatile long *long_fifo, *long_pntr,TS_gate, TS_freq, TS_off, ASCOPE_mode,FFT_pts;
RP7_SCOPE volatile float SCALE_fac;
/* RP7_SCOPE short status, control, start; */

/* Pointer for VME to VME handshaking in buffer memory */

struct vmevmehndshk *vme2_pntr;  /* general pointer to vme to vme structure */

/* Arrays for address storage to mailboxes and data */

RP7_SCOPE int number_of_mads[4]; /* Number of mads in each processor 0=> no processor */
RP7_SCOPE int current_mad[4];    /* Current mad (Either 0 or 1) in each processor */
RP7_SCOPE short proc_nodes[4];   /* Array of node numbers for each processor */
#define DIS1 1         /* All above arrays display processor #1 is entry 0 */
#define DIS2 2         /* All above arrays display processor #2 is entry 0 */
#define REC  0         /* All above arrays recording processor is entry 1 */
#define GRID 3         /* All above arrays gridding processor is entry 2 */

long madinfo[4][2][7];
/* First dimension is processor number dis1=0,dis2=1, record=2, gridding=3
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

void broadcast_data();

/* General purpose variables associated with the mcpl */

RP7_SCOPE int logical_length;
RP7_SCOPE short number_procs;

/* ASCOPE MODE Definitions */

#define TIMESERIES 0
#define VELOCITY 1
#define POWER 2
#define PWR_SPECTRUM 3

#endif /* INCmcpl_gbl */






