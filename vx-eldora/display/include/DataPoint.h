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
 */
#ifndef INCDataPointh
#define INCDataPointh

#include "point.h"

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

#endif INCDataPointh

