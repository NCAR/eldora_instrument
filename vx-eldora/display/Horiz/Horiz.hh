/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Horiz.hh
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
// Revision 1.1  1994/04/08  20:29:55  thor
// Initial revision
//
 *
 *
 * description:
 *        
 */
#ifndef INCHoriz_hh
#define INCHoriz_hh

#pragma interface

class Horiz;

#include "Display.hh"

#ifndef _MPD_
static const float METERS_PER_DEGREE = 112251.29;
#define _MPD_
#endif

class Horiz : public Display {
  public:

    enum sizes { DATA_WIDTH = 936, DATA_HEIGHT = 936, TBL_WIDTH = 344,
		 TBL_HEIGHT = 1024, TITLE_WIDTH = 936, TITLE_HEIGHT = 88 };
    
    Horiz(GraphicController *gbd);

    void reset(Header *hdr, DispCommand *cmd);

    void displaySet(int set);

    void switchSets(void);
    
    void drawBeam(DataBeam *beam);
    
    void drawTitle(int set, int radar);

    void drawTable(int set, float max, float min, int param, int tblsize = 31);

    void undisplay(void);

    int wdwOk(int wdw);
    
    ~Horiz(void);

  private:
    // Local functions.
    void shift(int direction);

    int nearestGate(double dist);

    void makeCellVector(CELLSPACING &spacing);

    void updateTitle(void);

    void updateLocation(DataBeam *beam);

    void zeroCase(DataBeam *beam);

    void hashMarks(double meters);

    double DegreesToRadians(double d) { return(d * M_PI / 180.0); }

    int clip(Point p, int w, int h);

    // Local data members.
    double beamWidth;
    double latitude[2];
    double longitude[2];
    double altitude;
    double maxDistance;
    double degrees;
    double eighth;
    double pixelsPerMeter;	// Scale for drawing.

    int *cellVector;
    
    int maxGates;

    double isines[180];         // Inverted sine values.
    double itangents[180];      // Inverted tangent values.

    // Local enums.
    enum directions { NOUGHT, LEFT, RIGHT, UP, DOWN };
    enum misc { IMPOSSIBLE = 1000000 };
};

#endif // INCHoriz_hh
