/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 GraphicColors.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  90/12/11  13:22:58  thor
 * Changed default color map method to use fully specified
 * color table.
 * 
 * Revision 1.1  90/12/04  10:20:36  thor
 * Initial revision
 * 
 *
 *
 * description:
 *        These methods allow the colormap to be set to a user defined
 * map, reset to the default state and set for the overlay. The AGC's
 * RAMDAC is addressed by writing the LUT index number into the
 * address register and writing the color into the correct LUT
 * address. Note that for proper colors, longs must be set up as
 * follows:
 *        red in bits 31-24, blue in 23-16, green in 15-8
 *
 */
#define GRAPHIC_CTLR_PRIVATE
#include "GraphicController.hh"

#include "default_colors.h"

void GraphicController::setColorMap(FAST long *colors, FAST int mapSize)
{
    if (colors == NULL || mapSize == 0)
      return;

    FAST unsigned char *ramdacAddr = (baseAddr + RAMDAC_ADDR);
    FAST long *clut = (baseAddr + RAMDAC_CLUT);

    for (FAST int i = 0; i < mapSize; i++)
      {
	  *ramdacAddr = i;
	  *clut = *colors++;
      }
}

void GraphicController::setOverlayColorMap(FAST long color)
{
    FAST unsigned char *ramdacAddr = (baseAddr + RAMDAC_ADDR);
    FAST long *clut = (baseAddr + RAMDAC_OLUT);

    *ramdacAddr = 2;
    *clut = color;
}

void GraphicController::defaultColorMap(void)
{
    FAST int i = 0;
    FAST int j = MAX_CLUT_SIZE;
    FAST unsigned char *ramdacAddr = (baseAddr + RAMDAC_ADDR);
    FAST long *clut = (baseAddr + RAMDAC_CLUT);
    FAST long *colors = defaultColors;

    for (; i < j; i++)
      {
	  *ramdacAddr = i;
	  *clut = *colors++;
      }
}
