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
 * Revision 1.3  1991/10/15  17:54:21  thor
 * Fixed to meet latest version of tape spec.
 *
 * Revision 1.2  1991/10/11  15:32:10  thor
 * Added variable for offset to first gate.
 *
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

struct cell_spacing_d {
    char   cell_spacing_des[4]; /* Identifier for a cell spacing descriptor */
				/* (ascii characters CSPD). */
    long   cell_spacing_des_len; /* Cell Spacing descriptor length in bytes. */
    short  num_segments;	/* Number of segments that contain cells of */
				/* equal widths. */
    short  distToFirst;		/* Distance to first gate in meters. */
    short  spacing[6];		/* Width of cells in each segment in m. */
    short  num_cells[6];	/* Number of cells in each segment. */
};				/* End of Structure */

typedef struct cell_spacing_d cell_spacing_d;
typedef struct cell_spacing_d CELLSPACING;

#endif /* INCCellSpacingh */

