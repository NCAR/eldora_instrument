/*
 *	$Id$
 *
 *	Module:		 DataPoint.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.14  1992/03/16  17:44:14  thor
 * Changed color member in DualData to hold colors.
 *
 * Revision 1.13  1992/02/11  19:12:15  thor
 * Changed DualData to point to unsigned short array.
 *
 * Revision 1.12  1992/02/10  17:06:00  thor
 * Fixed mistake in DualData.
 *
 * Revision 1.11  1992/02/10  15:58:25  thor
 * Added DualData definition.
 *
 * Revision 1.10  1991/12/20  14:14:02  thor
 * Changed old DataPoint into HorizPoint with some changes as well.
 *
 * Revision 1.9  1991/12/12  20:50:34  thor
 * Added new height & width variables to VertPoint struct.
 *
 * Revision 1.8  1991/12/06  15:46:53  thor
 * Added structure for vertical display.
 *
 * Revision 1.7  1991/11/22  18:21:53  thor
 * Added time values to DataPoint.
 *
 * Revision 1.6  1991/11/01  19:39:58  thor
 * Added new Beam_time structure.
 *
 * Revision 1.5  1991/04/29  18:45:38  thor
 * Added direction member to RadialData & constants for direction.
 *
 * Revision 1.4  1991/04/08  20:37:01  thor
 * Added angle member to RadialData.
 *
 * Revision 1.3  1991/03/27  16:54:06  thor
 * Changed RadialData structure to eliminate redundant angle data.
 *
 * Revision 1.2  1991/03/01  20:30:24  thor
 * Fixed colors member of RadialData structure.
 *
 * Revision 1.1  1991/02/28  21:53:57  thor
 * Initial revision
 *
 *
 *
 * description:
 *        This header defines two structures for passing drawing and
 * command information for horizontal displays. The DataPoint
 * structure contains the aircraft track data, the plane region to
 * draw and up to four parameters of color data. The HorizMove
 * structure contains the new latitude and longitude, as well as the
 * direction to shift the display.
 * 
 *        This header also defines the structure used to pass drawing
 * information for radial displays. The RadialData structure contains
 * an angle component and three arrays to hold color data.
 *
 */
#ifndef INCDataPointh
#define INCDataPointh

#include "point.h"
#include "WdwGbls.h"

#ifndef VERTICES
#define VERTICES 4
#endif /* VERTICES */

struct Horiz_Move {
    int direction;		/* Where are we going. */
    float latitude;
    float longitude;
};

typedef struct Horiz_Move HorizMove;

struct Beam_Time {
    short hour;
    short minute;
    short second;
    short millisecond;
};

typedef struct Beam_Time Beam_Time;

struct Horiz_Point {
    HorizMove move;
    Beam_Time time;
    Point aircraft;		/* Location of aircraft. */
    Point ul;			/* Data's location. */
    unsigned short width;	/* Size of rectangle. */
    unsigned short height;
    unsigned char colors[4];
};

struct Vert_Point {
    Beam_Time time;
    Point ul;
    unsigned short width;
    unsigned short height;
    unsigned char colors[4];
};

struct Radial_Data {
    Beam_Time time;
    float angle;		/* Rotation angle. */
    int direction;
    unsigned char colors[DISPLAYED_GATES * 3];
};

struct Dual_Data {
    Beam_Time time;
    float angle;
    float altitude;
    int direction;
    unsigned short *data;
};

static const int POSITIVE = 1;
static const int NEGATIVE = 0;

static const int NO_MOVE     = -1;
static const int UPPER_LEFT  = 0;
static const int LEFT        = 1;
static const int LOWER_LEFT  = 2;
static const int UPPER_RIGHT = 3;
static const int RIGHT       = 4;
static const int LOWER_RIGHT = 5;
static const int UP          = 6;
static const int DOWN        = 7;
static const int INITIAL_LAT_LONG = 8; /* This means update only the */
				       /* location data. */

typedef struct Horiz_Point HorizPoint;
typedef struct Radial_Data RadialData;
typedef struct Vert_Point VertPoint;
typedef struct Dual_Data DualData;

static int HorizMoveSize  = sizeof(HorizMove);
static int HorizPointSize = sizeof(HorizPoint);
static int BeamTimeSize   = sizeof(Beam_Time);
static int VertPointSize  = sizeof(VertPoint);
static int RadialDataSize = sizeof(RadialData);
static int DualDataSize   = sizeof(DualData);

#endif INCDataPointh

