/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Vert.hh
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:
 *        
 */
#ifndef INCVert_hh
#define INCVert_hh

#pragma interface

class Vert;

#include "Display.hh"

class Vert : public Display {
  public:
    Vert(GraphicController *gbd);

    void reset(Header *hdr, DispCommand *cmd);

    void displaySet(int set);
    
    void drawBeam(DataBeam *beam);
    
    void drawTitle(int set, int radar);

    ~Vert(void);

  private:
    // Local functions.
    void shift(DataBeam *beam);

    int nearestGate(double dist);

    void makeCellVector(CELLSPACING &spacing);

    void zeroCase(DataBeam *beam);

    void hashMarks(double meters);

    double DegreesToRadians(double d) { return(d * M_PI / 180.0); }

    int clip(Point p, int w, int h);

    void calcX(double tilt);

    // Local data members.
    double beamWidth;
    double distance;
    double altitude;
    double maxDistance;
    double degrees;
    double pixelsPerMeter;	// Scale for drawing.
    double oldLat;
    double oldLong;

    int *cellVector;
    
    int maxGates;
    int whichSide;
    int lastMillsec;

    unsigned short aircraft_x;

    static const double METERS_PER_DEGREE;
    static const double TILT_ANGLE;
    // Local enums.
    enum directions { LEFT, RIGHT };
};

#endif // INCVert_hh
