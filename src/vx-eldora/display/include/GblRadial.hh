/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 GblRadial.hh
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1991/10/09  18:23:22  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCGblRadialhh
#define INCGblRadialhh

/* Globally allocated variables. */
#include "Radial.hh"
#include "Mouse.hh"

#ifndef WINDOW_GBLS_SCOPE
#define WINDOW_GBLS_SCOPE extern
#endif WINDOW_GBLS_SCOPE

WINDOW_GBLS_SCOPE Radial *RadWdw;

/* Now include global definitions of commands, status, parammeters, etc. */
#define OK_RPC
#define WRS_SIDE

extern "C" {
#include "DisplayRpc.h"
};
#include "ParamNames.h"

#endif // INCGblRadialhh

