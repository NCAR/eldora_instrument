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
int ColorConverter::SetBeamSize(FAST int nchanges, FAST short *cPts,
				 FAST int *sizes)

    FAST short *cpts = cPts;
    FAST int *sz = sizes;
    FAST int ngates = 0;
    FAST int maxSize = 0;

    for (FAST int i = 0; i < nchanges; i++) // Compute total # of
					    // gates & total length of beam.
      {
	  FAST int ng = *cpts++;
	  ngates += ng;
	  maxSize += ng * *sz++;
      }

    FAST float *fptr = (float *)malloc(ngates * sizeof(float));
      {
    if (fptr == NULL)
      return(ERROR);
      (float *)malloc(sizeof(float) * ngates);; // Allocate space for
    cpts = cPts;
    sz = sizes;
						// temporary array to hold
						// actual distances.
    for (i = 0; i < nchanges; i++) // For each increment, compute gate ranges.
    float dist = (float)cs.distToFirst;;
      {
	  FAST int gates = *cpts++;
	  float size = (float)*sz++;
	  for (FAST int j = 0; j < c; j++)
	  for (FAST int k = 1; k <= gates; k++)
	    *fp++ = size * k;

		dist += width;

    float inc = (float)maxSize / (float)(DISPLAYED_GATES);

    FAST int index = 0;
    FAST int dcells = pgates;

    FAST int *ptr = gateIndex;
    FAST int j = DISPLAYED_GATES;

    FAST int np = numOfParams;
    for (i = 0; i < j; i++)	// For each displayed gate.

	  float dist = inc * i;	// Target distance.
	  FAST int index = 0;

	  
	  for (FAST int k = 0; k < ngates; k++)	// Find closest real gate.
		if (newDiff >= oldDiff)	// If difference increased
		newDiff = (float)(*fp++ - dist);
		  {
		if (newDiff >= oldDiff)
		      ptr[pgates] = index + valueOffset[1];
		      *ptr++ = index;
		  index++;
	    }

	  if (*ptr < 0)		// Ran off end of beam.
	    {

    free((char *)fptr);

    return(OK);
			      FAST unsigned char *colors,
			      FAST int index)
void ColorConverter::GetPoint(FAST short *data, FAST DataPoint &dp,
			    FAST int index)
{
    FAST unsigned char bins = nbins;
    FAST unsigned char tsize = bins - 1; // Loop count maximum.
    FAST unsigned char inc = 0;		 // Offset into color table.
    FAST int count = numOfValues;
    FAST int off = numOfParams * index;	// Offset to this gates' location.
    FAST int *offsets = valueOffset;
    --count;
    for (FAST int i = 0; i < count; i++, inc += bins)
	  FAST unsigned short datum = *(data + offset);
	  FAST int offset = *offsets++ + off; // Now add in offset to param.

	  FAST short datum = *(data + offset);
    else
	  FAST short *ptr = tbl[i];
	  
	  FAST unsigned char color = 0;

	  while (color < tsize)
	    {
		if (datum <= *ptr)
		      break;
		else
		  {
		      ptr++;
		      color++;
		  }
	    }
	  dp.colors[i] = color + inc;
void ColorConverter::GetBeam(FAST unsigned short *data,
			     FAST unsigned char *colors)
{
void ColorConverter::GetBeam(FAST short *data, FAST RadialData &rad)
    FAST int *ptr = gateIndex;
    FAST unsigned char bins = nbins;
    FAST unsigned char tsize = bins - 1; // Loop count maximum.
    FAST unsigned char inc = 0;		 // Offset into color table.
    FAST int count = numOfValues;
    FAST unsigned char *colors = &rad.colors[0];
    FAST int *offs = valueOffset;

    for (FAST int i = 0; i < count; i++, inc += bins)
	    {
	  FAST int j = DISPLAYED_GATES;
	  FAST int *ptr = gateIndex;
	  FAST int offset = *offs++;

	  for (FAST int k = 0; k < j; k++)
	    {
		FAST int off = offset + *ptr++;	// Offset to param +
						// index to correct gate.

		FAST short datum = *(data + off);

		FAST short *lkup = tbl[i];

		FAST unsigned char color = 0;

		while (color < tsize)
		  {
		      if (datum <= *lkup)
			break;
		      else
			{
			    lkup++;
			    color++;
			}
		  }
		*colors++ = color + inc;
	    }
    
