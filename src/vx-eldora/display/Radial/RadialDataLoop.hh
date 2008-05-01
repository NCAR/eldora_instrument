/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 RadialDataLoop.hh
 *	Original Author: Richard E. k. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  90/12/18  14:30:39  thor
 * Fixed constants for mouse.
 * 
 * Revision 1.2  90/12/14  10:47:06  thor
 * Fixed terminology.
 * 
Revision 1.1  90/12/11  13:26:01  thor
Initial revision

 *
 *
 * description:
 *        
 */
#ifndef INCRadialDataLoophh
#define INCRadialDataLoophh

#include "FixedPipe.h"
#include "Flags.hh"

static const unsigned int DISP_QUIT     = 0x1;
static const unsigned int MOUSE_QUIT    = 0x2;
static const unsigned int NEW_DATA      = 0x4;

static const unsigned int DATA_LOOP_MASK = DISP_QUIT | NEW_DATA;
static const unsigned int MOUSE_LOOP_MASK = MOUSE_QUIT;

extern void RadialDataLoop1(Flags *, FixedPipe *);
extern void RadialDataLoop2(Flags *, FixedPipe *);
extern void RadialDataLoop3(Flags *, FixedPipe *);

struct addrblock {
    float angle;
    unsigned char *colors;
};

typedef struct addrblock ADDRBLOCK;

struct datablock1 {
    float angle;
    unsigned char colors[512];
};

typedef struct datablock1 DATABLOCK1;

struct datablock2 {
    float angle;
    unsigned char colors[1024];
};

typedef struct datablock2 DATABLOCK2;

struct datablock3 {
    float angle;
    unsigned char colors[1536];
};

typedef struct datablock3 DATABLOCK3;

#endif INCRadialDataLoophh

