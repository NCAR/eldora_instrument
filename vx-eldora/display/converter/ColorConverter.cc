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
    --bins;			// Since we don't need maximum value
				// in our table.

    for (FAST int i = 0; i < nvalues; i++)
    float fb = 0.0;		// Color offset.

    for (FAST int i = 0; i < nvalues; i++)
      {
	  FAST short *ptr = (short *)malloc(sizeof(short) * bins);
	  tbl[i] = ptr;

	  float step = mx - mn;

	  for (FAST int j = 0; j < bins; j++)
	    *ptr++ = (short)(mn + (j * step)); // Ignore roundoff errors.

	  float mx = *max++;
		*lkup++ = color;
	    }
	  fb += fbins;		// Bump up in color lut.
void ColorConverter::SetBeamSize(FAST int ngates)

    float skip = (float)ngates / (float)(DISPLAYED_GATES);
    

    FAST int np = numOfParams;
    // Index values are determined by picking the nearest gate to the
    // displayed gate.
    for (FAST int i = 0; i < ngates; i++)
      *ptr++ = (int)((skip * (float)i) + .5);
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

    for (FAST int i = 0; i < count; i++, inc += bins)
	    {
	  FAST int j = DISPLAYED_GATES;
	  FAST int *ptr = gateIndex;
	  FAST int offset = valueOffset[i];

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
    
