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
 * want the converted value we walk up the array. If the value is <=
 * the array value the index equals the color (plus any offset into
 * the color table). If we reach the end of the array with no match
 * the color defaults to the maximum color value.
 * 
 */
#ifndef INCColorConverterhh
#define INCColorConverterhh

#define OK_RPC

extern "C" {
#include "vxWorks.h"
#include "stdioLib.h"
#include "memLib.h"
#include "CellSpacing.h"
};

#include "DataPoint.h"

static const int MAX_DATA_PLANES = 3;

class ColorConverter {
  protected:
    int valueOffset[MAX_DATA_PLANES]; // Offsets into the data array.
    short *tbl[MAX_DATA_PLANES];      // The lookup tables.

    int nbins;			// Number of colors in use.
    int numOfParams;		// Number of parameters in data array.
    int numOfValues;		// Number of parameters used.

    int gateIndex[DISPLAYED_GATES]; // List of indexes into data array.

  public:
    ColorConverter(int bins, float *max, float *min, int *offsets, int nparams,
		int nvalues);

    ColorConverter(void) {}

    // For radial case.
    void SetBeamSize(CELLSPACING &cs);

    // Horizontal & vertical case.
    void GetPoint(short *data, DataPoint &dp, int index);

    // Radial again.
    void GetBeam(FAST short *data, FAST RadialData &rad);

    ~ColorConverter(void)
      {
	  FAST int j = numOfValues;

	  for (FAST int i = 0; i < j; i++)
	    {
		FAST short *ptr = tbl[i];

		free((char *)ptr);
	    }
      }
};


#endif INCColorConverterhh
