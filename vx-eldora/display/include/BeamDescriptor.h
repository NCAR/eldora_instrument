/*
 *	$Id$
 *
 *	Module:		 BeamDescriptor.h
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
 *        The BeamDescriptor structure is used to pass beam
 * configuration information.
 */
#ifndef INCBeamDescriptorh
#define INCBeamDescriptorh

struct Beam_Descriptor {
    int numOfGates;
    float gateLength;
    float gateWidth;
    float gateHeight;
    float offsetToFirstGate;
    float beamWidth;
};

typedef struct Beam_Descriptor BeamDescriptor;

#endif /* INCBeamDescriptorh */

