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
 * Revision 2.6  1994/08/12  18:13:51  thor
 * Fixed a nit.
 *
 * Revision 2.5  1993/10/22  19:34:32  thor
 * Fixed overwrite problem.
 *
 * Revision 2.4  1993/10/21  14:52:45  thor
 * Complete rewrite of SetBeam to correctly handle all cases.
 *
 * Revision 2.3  1993/10/13  16:13:51  thor
 * Add an error message for debugging.
 *
 * Revision 2.2  1993/08/20  17:07:52  thor
 * Fixed bad count on lut generation, cleaned up code that generates
 * gateIndex and converted hardcoded table sizes to const.
 *
 * Revision 2.1  1992/11/09  14:13:57  thor
 * Fixed SetBeamSize to handle user specified sizes correctly.
 *
 * Revision 2.0  1992/11/03  12:52:00  thor
 * First offical ELDORA release!
 *
 * Revision 1.23  1992/03/19  20:03:49  thor
 * Added code to SetBeamSize to handle a window that is not the maximum
 * beam length. Removed GetVertPoint and folded into GetPoint. Conversion
 * methods not take an unsigned char * for flexibilty.
 *
 * Revision 1.22  1992/02/05  18:23:56  thor
 * Added code to handle variable radii for Radials.
 *
 * Revision 1.21  1992/01/28  19:01:11  thor
 * Changed firound to iround - 040 has no sp hardware!
 *
 * Revision 1.20  1992/01/28  14:57:13  thor
 * Added call to correctly round color value.
 *
 * Revision 1.19  1992/01/28  14:46:11  thor
 * Moved addition of color table offset to initialization & removed it from
 * runtime. Added correct table offset increases.
 *
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
#include "ColorConverter.hh"

#include <math.h>
#include <memLib.h>
#include <string.h>
#include <taskLib.h>
#include <iostream.h>

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

    FAST unsigned char *lkup = convertTbl;
    FAST int j = CONV_TBL_SIZE;

    float fbins = (float)bins;	// How many colors.
    float fb = 0.0;		// Color offset.

    for (FAST int i = 0; i < nvalues; i++)
      {
	  float mx = *max++;
	  float mn = *min++;

	  valueOffset[i] = *offsets++;

	  float bias = *biases++;
	  float scale = *scales++;
	  float maxColor = (float)(bins - 1);

	  float slope = (float)(bins - 1) / (mx - mn);
	  float b = -(slope * mn);

	  for (FAST int k = 0; k < j; k++)
	    {
		float t = ((float)k - bias) / scale;

		if (t < mn)
		  t = mn;

		float c = (slope * t) + b;

		if (c > maxColor)
		  c = maxColor;
		else if (c < 0.0)
		  c = 0.0;

		c += fb;	// Add in offset.

		unsigned char color = (unsigned char)iround(c);

		*lkup++ = color;
	    }
	  fb += fbins;		// Bump up in color lut.
      }
}

void ColorConverter::SetBeamSize(FAST CELLSPACING &cs, FAST int pgates,
				 float mpp)
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

    if (fptr == NULL)
      cout << "What the hell? " << ngates << "gates?" << endl;
    
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
	    }
      }

    float inc;

    if (mpp < 0.0)
      inc = (float)maxDist / (float)pgates; // Meters/pixel.
    else
      inc = mpp;

    numGates = pgates;

    FAST int *ptr = gateIndex;

    memset((char *)ptr,0xff,sizeof(gateIndex)); // Mark all as not found. 

    FAST int np = numOfParams;

    for (i = 0; i < pgates; i++, ptr++)
      {
	  float dist = inc * (float)i; // Target distance.

	  fp = fptr;
	  
	  float newDiff = *fp++ - dist;

	  float oldDiff = 1.0;

	  FAST int index = 0;

	  for (int k = 0; k < ngates; k++)
	    {
		if (newDiff >=  0.0)
		  {
		      newDiff = newDiff > 0.0 ? newDiff : -newDiff;

		      FAST int work_index = index;
		
		      if (oldDiff < newDiff)
			work_index--;
		      
		      if (work_index < 0)
			work_index = 0;

		      work_index *= np; // Correct for number of
					// parameters.
		      *ptr = work_index + valueOffset[0];
		      ptr[pgates] = work_index + valueOffset[1];
		      ptr[pgates * 2] = work_index + valueOffset[2];
		      break;
		  }
		oldDiff = newDiff;
		newDiff = *fp++ - dist;
		index++;
	    }
	  if (k == ngates)	// Hit end of real beam!
	    {
		--k;
		k *= np;
		*ptr = k + valueOffset[0];
		ptr[pgates] = k + valueOffset[1];
		ptr[pgates * 2] = k + valueOffset[2];
	    }
      }
	  
    free((char *)fptr);
}

void ColorConverter::GetPoint(FAST unsigned short *data,
			      FAST unsigned char *colors,
			      FAST int index)
{
    FAST int count = numOfValues;
    FAST int off = numOfParams * index;	// Offset to this gates' location.
    FAST int *offsets = valueOffset;
    FAST unsigned char *lkup = convertTbl;
 
    FAST int offset = *offsets++ + off; // Now add in offset to param.

    FAST unsigned short datum = *(data + offset);

    *colors++ = *(lkup + datum);

    --count;

    if (count)
      {
	  offset = *offsets++ + off;

	  FAST unsigned short datum = *(data + offset);

	  *colors++ = *(lkup + datum);
      }
    else
      return;

    --count;

    if (count)
      {
	  offset = *offsets + off;

	  FAST unsigned short datum = *(data + offset);

	  *colors = *(lkup + datum);
      }
}

void ColorConverter::GetBeam(FAST unsigned short *data,
			     FAST unsigned char *colors)
{
    FAST int count = numOfValues; // How many.
    FAST int j = numGates;
    FAST int *ptr = gateIndex;
    FAST unsigned char *lkup = convertTbl;

    for (FAST int i = 0; i < j; i++)
      {
	  FAST unsigned short datum = *(data + *ptr++);

	  *colors++ = *(lkup + datum);
      }

    --count;

    if (count)
      {
	  lkup += CONV_TBL_SIZE;
	  for (i = 0; i < j; i++)
	    {
		FAST unsigned short datum = *(data + *ptr++);
		
		*colors++ = *(lkup + datum);
	    }
      }
    else
      return;
    
    --count;

    if (count)
      {
	  lkup += CONV_TBL_SIZE;
	  for (i = 0; i < j; i++)
	    {
		FAST unsigned short datum = *(data + *ptr++);
		
		*colors++ = *(lkup + datum);
	    }
      }
}

    
