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

struct Data_Point {
    Point aircraft;
    Point rect[VERTICES];
    unsigned char colors[4];
};

struct Horiz_Move {
    float latitude;
    float longitude;
    int direction;
};

struct Beam_Time {
    short hour;
    short minute;
    short second;
    short millisecond;
};

typedef struct Beam_Time Beam_Time;

struct Radial_Data {
    Beam_Time time;
    float angle;
    int direction;
    unsigned char colors[DISPLAYED_GATES * 3];
};

static const int POSITIVE = 1;
static const int NEGATIVE = 0;

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

typedef struct Data_Point DataPoint;
typedef struct Horiz_Move HorizMove;
typedef struct Radial_Data RadialData;

#endif INCDataPointh

