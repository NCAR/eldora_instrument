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
ColorConverter::ColorConverter(int bins, float *max, float *min, int *offsets,

ColorConverter::ColorConverter(int bins, float *max, float *min, float *scales,
    tbl[0] = NULL;		// Zero out pointers to be safe.
    tbl[1] = NULL;
    tbl[2] = NULL;

    Reset(bins,max,min,offsets,nparams,nvalues);
			       int nparams, int nvalues)
{
    Reset(bins,max,min,scales,biases,offsets,nparams,nvalues);

void ColorConverter::Reset(FAST int bins, float *max, float *min,
			   float *scales, float *biases,
			   FAST int *offsets, int nparams, FAST int nvalues)
{
    nbins = bins;
    FAST int *firsts = first;

    for (FAST int i = 0; i < nvalues; i++, firsts++)
    float fb = 0.0;		// Color offset.

    for (FAST int i = 0; i < nvalues; i++)
      {
	  if (tbl[i] != NULL)	// If already in use.
	    free((char *)tbl[i]);

	  FAST unsigned short *ptr = // Get new space.
	    (unsigned short *)malloc(sizeof(unsigned short) * bins);

	  tbl[i] = ptr;

	  float step = (mx - mn) / (float)(bins - 1);

	  *firsts = 0;
	  
	  // Note that the color array values start at the 1st real
	  // color, so the whole array may not get filled.
	  for (FAST int j = 1; j < bins; j++)
	    {
		float f = mn + ((float)j * step);

		if (f < 0.0)	// Negative counts cannot exist!
		  (*firsts)++;
		else
		  *ptr++ = (unsigned short)f; // Ignore roundoff err.
	    }
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

    free(fptr);
			      FAST unsigned char *colors,
			      FAST int index)
void ColorConverter::GetPoint(FAST unsigned short *data, FAST DataPoint &dp,
    FAST int off = numOfParams * index;	// Offset to this gates' location.
    FAST int *offsets = valueOffset;
    FAST unsigned char bins = nbins;
    FAST unsigned char tsize = bins - 1; // Loop count maximum.
    FAST unsigned char inc = 0;		 // Offset into color table.
    FAST int count = numOfValues;	 // How many to do.
    FAST int off = numOfParams * index;	 // Offset to this gates' location.
    FAST int offset = *offsets++ + off; // Now add in offset to param.
    FAST unsigned char *colors = &dp.colors[0];
    FAST unsigned short **tbls = tbl;

    for (FAST int i = 0; i < count; i++, inc += bins, tbls++)
      {
	  FAST int offset = *offsets++ + off; // Now add in offset to param.

	  FAST unsigned short datum = *(data + offset);

	  FAST unsigned short *lkup = *tbls;

	  FAST unsigned char color = first[i]; // Get 1st color.

	  do
	    {
		if (datum < *lkup++)
		  break;
	    } while (++color < tsize);

	  *colors++ = color + inc;
      }
}

void ColorConverter::GetVertPoint(FAST unsigned short *data,
				  FAST VertPoint &dp,
				  FAST int index)
{
    FAST unsigned char bins = nbins;
    FAST unsigned char tsize = bins - 1; // Loop count maximum.
    FAST unsigned char inc = 0;		 // Offset into color table.
    FAST int count = numOfValues;
    FAST int off = numOfParams * index;	// Offset to this gates' location.
    FAST int *offsets = valueOffset;
    FAST unsigned char *colors = &dp.colors[0];
    FAST unsigned short **tbls = tbl;
    --count;
    for (FAST int i = 0; i < count; i++, inc += bins, tbls++)
	  FAST unsigned short datum = *(data + offset);
	  FAST int offset = *offsets++ + off; // Now add in offset to param.

      }
    else
	  FAST unsigned short *lkup = *tbls;

	  FAST unsigned char color = first[i]; // Get 1st color.
	  *colors = *(lkup + datum);
	  do
	    {
		if (datum < *lkup++)
		  break;
	    } while (++color < tsize);
}
	  *colors++ = color + inc;
void ColorConverter::GetBeam(FAST unsigned short *data,
			     FAST unsigned char *colors)
{
void ColorConverter::GetBeam(FAST unsigned short *data, FAST RadialData &rad)
    FAST int *ptr = gateIndex;
    int count = numOfValues;	// How many.
    FAST unsigned char *colors = &rad.colors[0];
    FAST unsigned short **tbls = tbl;
    FAST int j = DISPLAYED_GATES;
    for (FAST int i = 0; i < j; i++)
    FAST unsigned short *table = *tbls;
	  FAST unsigned short datum = *(data + *ptr++);
    for (FAST int k = 0; k < j; k++)
	  *colors++ = *(lkup + datum);
	  FAST unsigned short datum = *(data + *ptr++);	// Get data.

	  FAST unsigned short *lkup = table; // Point to start of table.

	  FAST unsigned char color = first[0]; // Get 1st color.

	  switch(color)		// Go!
	    {
	      case 0:
		if (datum < *lkup++)
		  break;
	      case 1:
		if (datum < *lkup++)
		  {
		      color = 1;
		      break;
		  }
	      case 2:
		if (datum < *lkup++)
		  {
		      color = 2;
		      break;
		  }
	      case 3:
		if (datum < *lkup++)
		  {
		      color = 3;
		      break;
		  }
	      case 4:
		if (datum < *lkup++)
		  {
		      color = 4;
		      break;
		  }
	      case 5:
		if (datum < *lkup++)
		  {
		      color = 5;
		      break;
		  }
	      case 6:
		if (datum < *lkup++)
		  {
		      color = 6;
		      break;
		  }
	      case 7:
		if (datum < *lkup++)
		  {
		      color = 7;
		      break;
		  }
	      case 8:
		if (datum < *lkup++)
		  {
		      color = 8;
		      break;
		  }
	      case 9:
		if (datum < *lkup++)
		  {
		      color = 9;
		      break;
		  }
	      case 10:
		if (datum < *lkup++)
		  {
		      color = 10;
		      break;
		  }
	      case 11:
		if (datum < *lkup++)
		  {
		      color = 11;
		      break;
		  }
	      case 12:
		if (datum < *lkup++)
		  {
		      color = 12;
		      break;
		  }
	      case 13:
		if (datum < *lkup++)
		  {
		      color = 13;
		      break;
		  }
	      case 14:
		if (datum < *lkup++)
		  {
		      color = 14;
		      break;
		  }
	      case 15:
		if (datum < *lkup++)
		  {
		      color = 15;
		      break;
		  }
	      case 16:
		if (datum < *lkup++)
		  {
		      color = 16;
		      break;
		  }
	      case 17:
		if (datum < *lkup++)
		  {
		      color = 17;
		      break;
		  }
	      case 18:
		if (datum < *lkup++)
		  {
		      color = 18;
		      break;
		  }
	      case 19:
		if (datum < *lkup++)
		  {
		      color = 19;
		      break;
		  }
	      case 20:
		if (datum < *lkup++)
		  {
		      color = 20;
		      break;
		  }
	      case 21:
		if (datum < *lkup++)
		  {
		      color = 21;
		      break;
		  }
	      case 22:
		if (datum < *lkup++)
		  {
		      color = 22;
		      break;
		  }
	      case 23:
		if (datum < *lkup++)
		  {
		      color = 23;
		      break;
		  }
	      case 24:
		if (datum < *lkup++)
		  {
		      color = 24;
		      break;
		  }
	      case 25:
		if (datum < *lkup++)
		  {
		      color = 25;
		      break;
		  }
	      case 26:
		if (datum < *lkup++)
		  {
		      color = 26;
		      break;
		  }
	      case 27:
		if (datum < *lkup++)
		  {
		      color = 27;
		      break;
		  }
	      case 28:
		if (datum < *lkup++)
		  {
		      color = 28;
		      break;
		  }
	      case 29:
		if (datum < *lkup)
		  {
		      color = 29;
		      break;
		  }
	      case 30:
	      default:
		color = 30;
		break;
	    }

	  *colors++ = color;	// Note that these are the first bins
				// colors in the color table.

    if (count)
    if (!(--count))
      return;
	  lkup += 0x10000;
    table = *tbls++;
    FAST unsigned char inc = nbins;	// Offset into color table.

    for (k = 0; k < j; k++)
	    {
	  FAST unsigned short datum = *(data + *ptr++);

	  FAST unsigned short *lkup = table;

	  FAST unsigned char color = first[1];
		
	  switch(color)
	    {
	      case 0:
		if (datum < *lkup++)
		  break;
	      case 1:
		if (datum < *lkup++)
		  {
		      color = 1;
		      break;
		  }
	      case 2:
		if (datum < *lkup++)
		  {
		      color = 2;
		      break;
		  }
	      case 3:
		if (datum < *lkup++)
		  {
		      color = 3;
		      break;
		  }
	      case 4:
		if (datum < *lkup++)
		  {
		      color = 4;
		      break;
		  }
	      case 5:
		if (datum < *lkup++)
		  {
		      color = 5;
		      break;
		  }
	      case 6:
		if (datum < *lkup++)
		  {
		      color = 6;
		      break;
		  }
	      case 7:
		if (datum < *lkup++)
		  {
		      color = 7;
		      break;
		  }
	      case 8:
		if (datum < *lkup++)
		  {
		      color = 8;
		      break;
		  }
	      case 9:
		if (datum < *lkup++)
		  {
		      color = 9;
		      break;
		  }
	      case 10:
		if (datum < *lkup++)
		  {
		      color = 10;
		      break;
		  }
	      case 11:
		if (datum < *lkup++)
		  {
		      color = 11;
		      break;
		  }
	      case 12:
		if (datum < *lkup++)
		  {
		      color = 12;
		      break;
		  }
	      case 13:
		if (datum < *lkup++)
		  {
		      color = 13;
		      break;
		  }
	      case 14:
		if (datum < *lkup++)
		  {
		      color = 14;
		      break;
		  }
	      case 15:
		if (datum < *lkup++)
		  {
		      color = 15;
		      break;
		  }
	      case 16:
		if (datum < *lkup++)
		  {
		      color = 16;
		      break;
		  }
	      case 17:
		if (datum < *lkup++)
		  {
		      color = 17;
		      break;
		  }
	      case 18:
		if (datum < *lkup++)
		  {
		      color = 18;
		      break;
		  }
	      case 19:
		if (datum < *lkup++)
		  {
		      color = 19;
		      break;
		  }
	      case 20:
		if (datum < *lkup++)
		  {
		      color = 20;
		      break;
		  }
	      case 21:
		if (datum < *lkup++)
		  {
		      color = 21;
		      break;
		  }
	      case 22:
		if (datum < *lkup++)
		  {
		      color = 22;
		      break;
		  }
	      case 23:
		if (datum < *lkup++)
		  {
		      color = 23;
		      break;
		  }
	      case 24:
		if (datum < *lkup++)
		  {
		      color = 24;
		      break;
		  }
	      case 25:
		if (datum < *lkup++)
		  {
		      color = 25;
		      break;
		  }
	      case 26:
		if (datum < *lkup++)
		  {
		      color = 26;
		      break;
		  }
	      case 27:
		if (datum < *lkup++)
		  {
		      color = 27;
		      break;
		  }
	      case 28:
		if (datum < *lkup++)
		  {
		      color = 28;
		      break;
		  }
	      case 29:
		if (datum < *lkup)
		  {
		      color = 29;
		      break;
		  }
	      case 30:
	      default:
		color = 30;
		break;
      return;

	  *colors++ = color + inc; // Correct to proper color.
    

    if (!(--count))
    

    table = *tbls++;
    inc *= 2;


    for (k = 0; k < j; k++)
	    {
	  FAST unsigned short datum = *(data + *ptr++);

	  FAST unsigned short *lkup = table;

	  FAST unsigned char color = first[2];
		
	  switch(color)
	    {
	      case 0:
		if (datum < *lkup++)
		  break;
	      case 1:
		if (datum < *lkup++)
		  {
		      color = 1;
		      break;
		  }
	      case 2:
		if (datum < *lkup++)
		  {
		      color = 2;
		      break;
		  }
	      case 3:
		if (datum < *lkup++)
		  {
		      color = 3;
		      break;
		  }
	      case 4:
		if (datum < *lkup++)
		  {
		      color = 4;
		      break;
		  }
	      case 5:
		if (datum < *lkup++)
		  {
		      color = 5;
		      break;
		  }
	      case 6:
		if (datum < *lkup++)
		  {
		      color = 6;
		      break;
		  }
	      case 7:
		if (datum < *lkup++)
		  {
		      color = 7;
		      break;
		  }
	      case 8:
		if (datum < *lkup++)
		  {
		      color = 8;
		      break;
		  }
	      case 9:
		if (datum < *lkup++)
		  {
		      color = 9;
		      break;
		  }
	      case 10:
		if (datum < *lkup++)
		  {
		      color = 10;
		      break;
		  }
	      case 11:
		if (datum < *lkup++)
		  {
		      color = 11;
		      break;
		  }
	      case 12:
		if (datum < *lkup++)
		  {
		      color = 12;
		      break;
		  }
	      case 13:
		if (datum < *lkup++)
		  {
		      color = 13;
		      break;
		  }
	      case 14:
		if (datum < *lkup++)
		  {
		      color = 14;
		      break;
		  }
	      case 15:
		if (datum < *lkup++)
		  {
		      color = 15;
		      break;
		  }
	      case 16:
		if (datum < *lkup++)
		  {
		      color = 16;
		      break;
		  }
	      case 17:
		if (datum < *lkup++)
		  {
		      color = 17;
		      break;
		  }
	      case 18:
		if (datum < *lkup++)
		  {
		      color = 18;
		      break;
		  }
	      case 19:
		if (datum < *lkup++)
		  {
		      color = 19;
		      break;
		  }
	      case 20:
		if (datum < *lkup++)
		  {
		      color = 20;
		      break;
		  }
	      case 21:
		if (datum < *lkup++)
		  {
		      color = 21;
		      break;
		  }
	      case 22:
		if (datum < *lkup++)
		  {
		      color = 22;
		      break;
		  }
	      case 23:
		if (datum < *lkup++)
		  {
		      color = 23;
		      break;
		  }
	      case 24:
		if (datum < *lkup++)
		  {
		      color = 24;
		      break;
		  }
	      case 25:
		if (datum < *lkup++)
		  {
		      color = 25;
		      break;
		  }
	      case 26:
		if (datum < *lkup++)
		  {
		      color = 26;
		      break;
		  }
	      case 27:
		if (datum < *lkup++)
		  {
		      color = 27;
		      break;
		  }
	      case 28:
		if (datum < *lkup++)
		  {
		      color = 28;
		      break;
		  }
	      case 29:
		if (datum < *lkup)
		  {
		      color = 29;
		      break;
		  }
	      case 30:
	      default:
		color = 30;
		break;

	  *colors = color + inc;
d272 1
    
