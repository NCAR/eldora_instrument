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
 *
 * Initial revision
 *
 * class. The constructor derives the slope and y intercept for all
 * needed parameters. The SetBeamSize method sets up the index vector
 * for radial displays. The GetPoint method returns the color
 * information for horizontal displays. The GetBeam method returns
 * color information for radial displays.
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
    --bins;			// Since slope is from 0 to y.

    for (FAST int i = 0; i < nvalues; i++)
    float fb = 0.0;		// Color offset.

    for (FAST int i = 0; i < nvalues; i++)
      {
	  valueOffset[i] = *offsets++;

	  slope[i] = (float)bins / (mx - mn); // dy is fixed.

	  yInt[i] = -(slope[i] * mn);
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
    FAST int bins = nbins;
    FAST int count = numOfValues;
    FAST int inc = 0;
    FAST int off = numOfParams * index;	// Offset to this gates' location.
    FAST int *offsets = valueOffset;
    FAST float *s = slope;
    FAST float *y = yInt;
    --count;
    for (FAST int i = 0; i < count; i++, inc += bins)
	  FAST unsigned short datum = *(data + offset);
	  FAST int offset = *offsets++ + off; // Now add in offset to param.

	  float datum = (float)*(data + offset);
    else
 	  FAST float m = *s++;
 	  FAST float b = *y++;
 	  FAST int color = (int)((m * datum) + b);

	  if (color < 0)
	    color = 0;
	  else if (color > bins)
	    {
		color = bins;
		--color;	// Because colors start at 0.
	    }

	  dp.colors[i] = (unsigned char)(color + inc);
void ColorConverter::GetBeam(FAST unsigned short *data,
			     FAST unsigned char *colors)
{
void ColorConverter::GetBeam(FAST short *data, FAST RadialData &rad)
    FAST int *ptr = gateIndex;
    FAST int bins = nbins;
    FAST int count = numOfValues;
    FAST int inc = 0;
    FAST unsigned char *colors = &rad.colors[0];

    for (FAST int i = 0; i < count; i++, inc += bins)
	    {
	  FAST int j = DISPLAYED_GATES;
	  FAST int *ptr = gateIndex;
	  FAST int offset = valueOffset[i];
	  FAST float m = slope[i];
	  FAST float b = yInt[i];

	  for (FAST int k = 0; k < j; k++)
	    {
		FAST int off = offset + *ptr++;	// Offset to param +
						// index to correct gate.

		float datum = (float)*(data + off);

		FAST int color = (int)((m * datum) + b);

		if (color < 0)
		  color = 0;
		else if (color > bins)
		  {
		      color = bins;
		      --color;	// Because colors start at 0.
		  }

		*colors++ = (unsigned char)(color + inc);
	    }
    
