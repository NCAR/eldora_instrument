/*
 *	$Id$
 *
 *	Module:		 WdwGbls.h
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
 *        This a collection of useful constants common to all
 * displays, radial, vertial & horizontal.
 *
 */
#ifndef INCWdwGblsh
#define INCWdwGblsh

/* Constants */
/* Parameter sets. Maximum of three for now. */
static const int NULL_SET    = -1;
static const int A_SET       = 0;
static const int B_SET       = 1;
static const int C_SET       = 2;

/* Window names - note they start at 1 NOT 0. */
static const int WDW_ONE     = 0;
static const int WDW_TWO     = 1;
static const int WDW_THREE   = 2;
static const int WDW_FOUR    = 3;
static const int WDW_FIVE    = 4;
static const int WDW_SIX     = 5;
static const int WDW_SEVEN   = 6;
static const int WDW_EIGHT   = 7;
static const int WDW_NINE    = 8;

/* The currently available display parameters. */
static const int NULL_PARAMETER     = 0;
static const int REFLECTIVITY       = 1;
static const int VELOCITY           = 2;
static const int SPEC_WIDTH         = 3;
static const int LINEAR_POWER       = 4;
static const int REAL_R1            = 5;
static const int IMAG_R1            = 6;
static const int REAL_R2            = 7;
static const int IMAG_R2            = 8;

/* Radars */
static const int FORWARD = 0;
static const int AFT = 1;

#endif /* INCWdwGblsh */

