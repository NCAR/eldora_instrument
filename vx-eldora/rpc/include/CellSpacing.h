/*
 *	$Id$
 *
 *	Module:		 CellSpacing.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1991/08/30  18:39:19  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCCellSpacingh
#define INCCellSpacingh

#ifdef OK_RPC

#ifdef UNIX
#include <rpc/rpc.h>
#else
#include "rpc/rpc.h"
#endif /* UNIX */

#endif /* OK_RPC */

struct cell_spacing_d {
    char   cell_spacing_des[4]; /* Identifier for a cell spacing descriptor
				/* (ascii characters CSPD). */
    short  cell_spacing_des_len; /* Cell Spacing descriptor length in bytes. */
    short  num_segments;	/* Number of segments that contain cells of */
				/* equal widths. */
    short  distToFirst;		/* Distance to first gate in meters. */
    short  spacing[6];		/* Width of cells in each segment in m. */
    short  num_cells[6];	/* Number of cells in each segment. */
};				/* End of Structure */

#ifdef OK_RPC

typedef struct cell_spacing_d cell_spacing_d;
typedef struct cell_spacing_d CELLSPACING;

bool_t xdr_cell_spacing_d(XDR *, cell_spacing_d *);

#endif /* OK_RPC */

#endif /* INCCellSpacingh */

