/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 DataBeam.hh
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1991/10/09  20:27:41  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCDataBeamhh
#define INCDataBeamhh

extern "C" {
#include "Ray.h"
#include "Platform.h"
#include "FieldParam.h"
};

struct data_beam {
    RAY ray;
    PLATFORM air;
    FIELDPARAMDATA data;
};

typedef struct data_beam DataBeam;

#endif // INCDataBeamhh

