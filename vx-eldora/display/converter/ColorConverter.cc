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
 * Revision 1.18  1992/01/22  17:41:45  thor
 * Removed all the old stuff.
 *
 * Revision 1.17  1992/01/22  17:38:39  thor
 * Major rewrite to make code use a new algorithm.
 *
 * Revision 1.16  1992/01/17  20:26:26  thor
 * Added cast to call to free().
 *
 * Revision 1.15  1991/12/20  15:36:47  thor
 * Changed to new HorizPoint form.
 *
 * Revision 1.14  1991/12/18  20:33:36  thor
 * Fixed bad pointer incrementing in GetBeam.
 *
 * Revision 1.13  1991/12/18  16:12:35  thor
 * Added bounding code to prevent generation of negative counts.
 *
 * Revision 1.12  1991/12/17  21:25:17  thor
 * Converted to unsigned shorts!
 *
 * Revision 1.11  1991/12/06  16:37:01  thor
 * Added method for vertical displays.
 *
 * Revision 1.10  1991/11/15  16:46:18  thor
 * Fix another indexing bug, optimized calc loops.
 *
 * Revision 1.9  1991/11/14  20:02:13  thor
 * Fixed typo.
 *
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

extern "C" {
#include "math.h"
#include "memLib.h"
#include "string.h"
#include "stdioLib.h"
#include "taskLib.h"
};

ColorConverter::ColorConverter(int bins, float *max, float *min, float *scales,
			       float *biases, int *offsets,
			       int nparams, int nvalues)
{
    Reset(bins,max,min,scales,biases,offsets,nparams,nvalues);
}

void ColorConverter::Reset(FAST int bins, float *max, float *min,
			   float *scales, float *biases,
			   FAST int *offsets, int nparams, FAST int nvalues)
{
    nbins = bins;
    numOfParams = nparams;
    numOfValues = nvalues;

    float fbins = (float)bins;	// How many colors.
    float fb = 0.0;		// Color offset.

    for (FAST int i = 0; i < nvalues; i++)
      {
	  float mx = *max++;
	  float mn = *min++;
	  float slope = (float)(bins - 1) / (mx - mn);
	  float b = (float)(bins - 1) - (slope * mx);


	  valueOffset[i] = *offsets++;

	  float bias = *biases++;
	  for (FAST int k = 0; k < j; k++)
	  float b = -(slope * mn);
		float c = (slope * (((float)k - bias)/ scale)) + b;
		  t = mn;

		float c = (slope * t) + b;

		if (c > maxColor)
		  c = maxColor;
		unsigned char color = (unsigned char)c;
		c += fb;	// Add in offset.

		unsigned char color = (unsigned char)iround(c);
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
	  maxDist += c * width;
      }

    FAST float *fptr = 
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

    for (i = 0; i < dcells; i++)
      {
	  if (i == stopper)
	    break;


	  FAST int index = 0;

	  for (FAST int k = 0; k < ngates; k++)
	    {
		newDiff = *fp++ - dist;

		if (newDiff >= oldDiff)	// If difference increased
					// this is our gate.
		  {
		      index *= np; // Correct for number of parameters.
		      *ptr = index + valueOffset[0];
		      ptr[pgates] = index + valueOffset[1];
		      ptr[pgates * 2] = index + valueOffset[2];
		      break;
		      ptr[DISPLAYED_GATES] = index + valueOffset[1];
		      ptr[DISPLAYED_GATES * 2] = index + valueOffset[2];
		  index++;
	    }

	  if (*ptr < 0)		// Ran off end of beam.
	    {
		--index;
		index *= np;
		*ptr = index + valueOffset[0];
		ptr[pgates] = index + valueOffset[1];
		ptr[pgates * 2] = index + valueOffset[2];
	    }
		ptr[DISPLAYED_GATES] = index + valueOffset[1];
		ptr[DISPLAYED_GATES * 2] = index + valueOffset[2];
    free((char *)fptr);
}

void ColorConverter::GetPoint(FAST unsigned short *data,
			      FAST unsigned char *colors,
			      FAST int index)
void ColorConverter::GetPoint(FAST unsigned short *data, FAST HorizPoint &dp,
    FAST int off = numOfParams * index;	// Offset to this gates' location.
    FAST int *offsets = valueOffset;
    FAST unsigned char *lkup = convertTbl;
 
    FAST int offset = *offsets++ + off; // Now add in offset to param.
    FAST unsigned char *colors = &dp.colors[0];
    FAST unsigned char *lkup = convertTbl;
 
    FAST int offset = *offsets++ + off; // Now add in offset to param.

    FAST unsigned short datum = *(data + offset);

    *colors++ = *(lkup + datum);

    --count;

    FAST unsigned char bins;

    if (count)
      {
	  bins = nbins;

	  offset = *offsets++ + off;

	  FAST unsigned short datum = *(data + offset);

	  *colors++ = *(lkup + datum) + bins;
      }
    else
      return;

    --count;

    if (count)
      {
	  bins *= 2;

	  offset = *offsets + off;

	  FAST unsigned short datum = *(data + offset);

	  *colors = *(lkup + datum) + bins;
      }
}

void ColorConverter::GetVertPoint(FAST unsigned short *data,
				  FAST VertPoint &dp,
				  FAST int index)
{
    FAST int count = numOfValues;
    FAST int off = numOfParams * index;	// Offset to this gates' location.
    FAST int *offsets = valueOffset;
    FAST unsigned char *colors = &dp.colors[0];

    FAST unsigned short datum = *(data + offset);

    *colors++ = *(lkup + datum);

    --count;

    if (count)
      {
	  offset = *offsets++ + off;
    FAST unsigned char bins;


	  FAST unsigned short datum = *(data + offset);
	  bins = nbins;


	  *colors++ = *(lkup + datum);
      }
    else
	  *colors++ = *(lkup + datum) + bins;

    --count;

    if (count)
      {
	  offset = *offsets + off;

	  FAST unsigned short datum = *(data + offset);
	  bins *= 2;


	  *colors = *(lkup + datum);
      }
}
	  *colors = *(lkup + datum) + bins;
void ColorConverter::GetBeam(FAST unsigned short *data,
			     FAST unsigned char *colors)
{
void ColorConverter::GetBeam(FAST unsigned short *data, FAST RadialData &rad)
    FAST int *ptr = gateIndex;
    FAST unsigned char *lkup = convertTbl;
    FAST unsigned char *colors = &rad.colors[0];
    FAST int j = DISPLAYED_GATES;
    for (FAST int i = 0; i < j; i++)
      {
	  FAST unsigned short datum = *(data + *ptr++);

	  *colors++ = *(lkup + datum);
      }

    --count;

    if (count)
      {
	  lkup += 0x10000;
    FAST unsigned char bins;

	  for (i = 0; i < j; i++)
	    {
	  bins = nbins;

		
		*colors++ = *(lkup + datum);
	    }

		*colors++ = *(lkup + datum) + bins;
      return;
    
    --count;
    

      {

	  for (i = 0; i < j; i++)
	    {
	  bins *= 2;

		
		*colors++ = *(lkup + datum);
	    }

		*colors++ = *(lkup + datum) + bins;

    
