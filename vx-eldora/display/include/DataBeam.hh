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
 *
 *
 * description:
 *        
 */
#ifndef INCDataBeamhh
#define INCDataBeamhh

extern "C" {
#include "Ray.h"
#include "Aircraft.h"
#include "FieldParam.h"
};

struct data_beam {
    RAY ray;
    AIRCRAFT air;
    FIELDPARAMDATA data;
};

typedef struct data_beam DataBeam;

#endif // INCDataBeamhh

