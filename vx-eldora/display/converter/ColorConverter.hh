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
 * All conversion methods use the same basic algorithm - using
 * precomputed values for slope and y intercept, the datum point is
 * used as the X value to find the color (Y). If the color is less
 * then zero, it is set to zero, conversely, if it is greater then the
 * maximum color it is set to the maximum.
 *
 */
#ifndef INCColorConverterhh
#define INCColorConverterhh

extern "C" {
#include "vxWorks.h"
#include "stdioLib.h"
#include "memLib.h"
};

#include "DataPoint.h"

static const int MAX_DATA_PLANES = 3;

class ColorConverter {
  protected:
    int valueOffset[MAX_DATA_PLANES];
    float slope[MAX_DATA_PLANES];
    float yInt[MAX_DATA_PLANES];

    int nbins;
    int numOfParams;
    int numOfValues;

    int gateIndex[DISPLAYED_GATES];

  public:
    ColorConverter(int bins, float *max, float *min, int *offsets, int nparams,
		int nvalues);

    ColorConverter(void) {}

    void SetBeamSize(FAST int ngates);

    // Horizontal & vertical case.
    void GetPoint(short *data, DataPoint &dp, int index);

    void GetBeam(FAST short *data, FAST RadialData &rad);
};


#endif INCColorConverterhh
