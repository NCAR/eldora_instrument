/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 ColorConverter.cc
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.8  1991/11/14  18:24:43  thor
 * Fixed a number of bugs that caused incorrect calculation of gate index.
 *
 * Revision 1.7  1991/10/22  17:17:37  thor
 * Do beam size calculations in ints.
 *
 * Revision 1.6  1991/10/18  19:25:21  thor
 * Switched to malloc/free due to weird interaction with VxWorks by delete.
 *
 * Revision 1.5  1991/10/14  19:16:33  thor
 * Fixed to use CELLSPACING structure from header.
 *
 * Revision 1.4  1991/06/28  19:05:57  thor
 * Changed SetBeamSize routine to use new precalcuated parameters.
 * Changed while to do loops.
 *
 * Revision 1.3  1991/06/25  14:04:27  thor
 * Fixed step calculation for bins. Rewrote beam sizing routine to handle
 * variable gate sizes.
 *
 * Revision 1.2  1991/05/08  20:08:41  thor
 * Major change - dropped floating point, switched to table of levels.
 *
 * Revision 1.1  1991/04/09  19:16:52  thor
 * Initial revision
 *
 *
 * description:
 *        This file contains all the methods for the ColorConverter
 * class. The SetBeamSize method sets up the index vector for radial
 * displays. The GetPoint method returns the color information for
 * horizontal displays. The GetBeam method returns color information
 * for radial displays.
 *
 */

#include "math.h"
#include "memLib.h"
#include "string.h"
#include "stdioLib.h"
ColorConverter::ColorConverter(FAST int bins, float *max, float *min,

void ColorConverter::Reset(FAST int bins, float *max, float *min,
			   float *scales, float *biases,
			   FAST int *offsets, int nparams, FAST int nvalues)
{
    nbins = bins;
    for (FAST int i = 0; i < nvalues; i++)
    float fb = 0.0;		// Color offset.

    for (FAST int i = 0; i < nvalues; i++)
      {
	  FAST short *ptr = (short *)malloc(sizeof(short) * bins);
	  tbl[i] = ptr;

	  float step = (mx - mn) / (float)(bins - 1);

	  for (FAST int j = 0; j < bins; j++)
	    *ptr++ = (short)(mn + ((float)j * step)); // Ignore roundoff err.

	  float mx = *max++;
		*lkup++ = color;
	    }
	  fb += fbins;		// Bump up in color lut.
void ColorConverter::SetBeamSize(FAST CELLSPACING &cs)

void ColorConverter::SetBeamSize(FAST CELLSPACING &cs, FAST int pgates,
				 float realMax = -1.0)
{
    FAST int seg = cs.num_segments;
    FAST short *widths = &cs.spacing[0];
    FAST short *ncells = &cs.num_cells[0];
    FAST int ngates = 0;

    FAST short *w = widths;
    FAST short *nc = ncells;
    FAST int maxDist = cs.distToFirst;;

    for (FAST int i = 0; i < seg; i++) // Calculate maximum distance & # gates.
      {
	  FAST int c = *nc++;
	  FAST int  width = *w++;

	  ngates += c;
    FAST float *fptr = (float *)malloc(sizeof(float) * ngates);; // Allocate space for
					  // temporary array to hold
					  // actual distances.
      (float *)malloc(sizeof(float) * ngates);; // Allocate space for
						// temporary array to hold
						// actual distances.

    FAST float *fp = fptr;

    nc = ncells;
    w = widths;

    float dist = (float)cs.distToFirst;;

    for (i = 0; i < seg; i++)	// Fill in actual distances.
      {
	  FAST int c = *nc++;
	  float width = (float)*w++;

	  for (FAST int j = 0; j < c; j++)
	    {
		*fp++ = dist;

		dist += width;
    float inc = (float)maxDist / (float)DISPLAYED_GATES; // Meters/pixel.

    FAST int dcells = pgates;

    FAST int *ptr = gateIndex;
    FAST int dcells = DISPLAYED_GATES;
    bfill((char *)ptr,ngates * sizeof(int),0xff); // Mark all as not found.

    FAST int np = numOfParams;
	    break;


	  FAST int index = 0;

	  for (FAST int k = 0; k < ngates; k++)
	    {
		newDiff = *fp++ - dist;

		if (newDiff >= oldDiff)	// If difference increased
					// this is our gate.
		  {
		if (newDiff >= oldDiff)
		      ptr[pgates] = index + valueOffset[1];
		      *ptr++ = index;
		  index++;
	    }

	  if (*ptr < 0)		// Ran off end of beam.
	    {

    free(fptr);
			      FAST unsigned char *colors,
			      FAST int index)
void ColorConverter::GetPoint(FAST short *data, FAST DataPoint &dp,
			    FAST int index)
{
    FAST unsigned char bins = nbins;
    FAST unsigned char tsize = bins - 1; // Loop count maximum.
    FAST unsigned char inc = 0;	   // Offset into color table.
    FAST int count = numOfValues;
    FAST int off = numOfParams * index;	// Offset to this gates' location.
    FAST int *offsets = valueOffset;
    FAST unsigned char *colors = &dp.colors[0];
    --count;
    for (FAST int i = 0; i < count; i++, inc += bins)
	  FAST unsigned short datum = *(data + offset);
	  FAST int offset = *offsets++ + off; // Now add in offset to param.

	  FAST short datum = *(data + offset);
    else
	  FAST short *lkup = tbl[i];
	  
	  for (FAST unsigned char color = 0; color < tsize; color++)
	    {
		if (datum <= *lkup++)
		  break;
	    }
}
	  *colors++ = color + inc;
void ColorConverter::GetBeam(FAST unsigned short *data,
			     FAST unsigned char *colors)
{
void ColorConverter::GetBeam(FAST short *data, FAST RadialData &rad)
    FAST int *ptr = gateIndex;
    FAST unsigned char bins = bins;
    FAST unsigned char tsize = bins - 1; // Loop count maximum.
    FAST unsigned char inc = 0;	   // Offset into color table.
    FAST int count = numOfValues;
    FAST int np = numOfParams;
    FAST unsigned char *colors = &rad.colors[0];
    FAST int *offs = valueOffset;

    for (FAST int i = 0; i < count; i++, inc += bins)
	    {
	  FAST int j = DISPLAYED_GATES;
	  FAST int *ptr = gateIndex;
	  FAST int offset = *offs++;

	  for (FAST int k = 0; k < j; k++)
	    {
		FAST int off = offset + (*ptr++ * np);	// Offset to param +
							// index to correct 
							// gate.
		FAST short datum = *(data + off);

		FAST short *lkup = tbl[i];

		for (FAST unsigned char color = 0; color < tsize; color++)
		  {
		      if (datum <= *lkup++)
			break;
		  }
		*colors++ = color + inc;
	    }
    
