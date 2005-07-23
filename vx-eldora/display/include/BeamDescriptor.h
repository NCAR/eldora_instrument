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
 * Revision 1.1  1991/03/01  15:36:07  thor
 * Initial revision
 *
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

