/*  This looks like C, but it's really C++!!
 *      $Id$
 *
 *      Module:          ColorConverter.hh
 *      Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *      Date:            $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 2.0  1992/11/03  12:52:03  thor
 * *** empty log message ***
 *
 * Revision 1.14  1992/03/19  20:02:29  thor
 * Added additional parameter to SetBeamSize to allow for windows of a size
 * not matching the maximum beam length. Folded GetVertPoint into GetPoint &
 * changed conversion routines to use a unsigned char *.
 *
 * Revision 1.13  1992/02/11  19:55:16  thor
 * Added Raw method to do single point conversions.
 *
 * Revision 1.12  1992/02/05  18:24:25  thor
 * Added code to handle variable radii for Radials.
 *
 * Revision 1.11  1992/01/22  17:42:03  thor
 * Removed all the old stuff.
 *
 * Revision 1.10  1992/01/22  17:38:10  thor
 * Added new params to constructor to handle new conversion algorithm.
 *
 * Revision 1.9  1991/12/20  15:37:08  thor
 * Changed to new HorizPoint form.
 *
 * Revision 1.8  1991/12/18  16:11:57  thor
 * Added bounding code to prevent generation of negative counts.
 *
 * Revision 1.7  1991/12/17  21:25:35  thor
 * Converted to unsigned shorts!
 *
 * Revision 1.6  1991/12/06  16:37:18  thor
 * Added method for vertical displays.
 *
 * Revision 1.5  1991/10/14  19:16:08  thor
 * Fixed to use CELLSPACING structure from header.
 *
 * Revision 1.4  1991/06/28  19:06:55  thor
 * Changed SetBeamSize routine to use new precalcuated parameters.
 *
 * Revision 1.3  1991/06/25  14:06:08  thor
 * Rewrote beam sizing routine to handle variable gate sizes.
 *
 * Revision 1.2  1991/05/08  20:09:37  thor
 * Major change - dropped floating point, switched to table of levels.
 *
 * Revision 1.1  1991/04/09  19:16:57  thor
 * Initial revision
 *
 * Revision 1.1  1991/03/01  16:31:58  thor
 * Initial revision
 *
 *
 *
 * description:
 *        The ColorConverter class acts as a filter that converts 16
 * bit parameter values into 8 bit colors. Methods are provided for
 * horizontal, vertical and radial displays. For radial displays the
 * SetBeamSize method must be called to set up a index vector table.
 * This is used to map the nearest real gate onto the displayed gate.
 * All conversion methods use the same basic algorithm - we set up an
 * array that contains discrete levels in the constructor. When we
 * want the converted value we walk up the array. If the value is <
 * the array value the index equals the color (plus any offset into
 * the color table). If we reach the end of the array with no match
 * the color defaults to the maximum color value. Note that the values
 * are off by one - at 0 index the value is for step 1, etc. This is
 * done so that the colors fall into the correct bins.
 * 
 */
#ifndef INCColorConverterhh
#define INCColorConverterhh

#define OK_RPC

#include "vxWorks.h"
#include "stdioLib.h"
#include "memLib.h"
#include "CellSpacing.h"

#include "DataPoint.h"

static const int MAX_DATA_PLANES = 3;
static const int MAX_BINS = 31;
static const int CONV_TBL_SIZE = 65536;

class ColorConverter {
  protected:
    int valueOffset[MAX_DATA_PLANES]; // Offsets into the data array.

    int nbins;			// Number of colors in use.
    int numOfParams;		// Number of parameters in data array.
    int numOfValues;		// Number of parameters used.
    int numGates;		// Number of pixels in display.

    int gateIndex[DISPLAYED_GATES * MAX_DATA_PLANES]; // List of
						      // indexes into
						      // data array.

    unsigned char convertTbl[CONV_TBL_SIZE * MAX_DATA_PLANES];

  public:
    ColorConverter(int bins, float *max, float *min, float *scales,
		   float *biases, int *offsets, int nparams, int nvalues);

    void Reset(int bins, float *max, float *min, float *scales, float *biases,
	       int *offsets, int nparams, int nvalues);

    ColorConverter(void) {}

    // For radial case.
    void SetBeamSize(CELLSPACING &cs, int pgates, float realMax = -1.00);

    // Horizontal & Vertical case.
    void GetPoint(unsigned short *data, unsigned char *colors, int index);

    // Radial again.
    void GetBeam(unsigned short *data, unsigned char *colors);

    // Fast inline call.
    unsigned char Raw(FAST unsigned short datum, FAST int param)
      {
	  FAST unsigned char *lkup = convertTbl + datum;

	  if (param == 1)
	    lkup += 0x10000;
	  else
	    lkup += 0x20000;

	  return(*lkup);
      }
};


#endif INCColorConverterhh
