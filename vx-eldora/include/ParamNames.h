/*
 *	$Id$
 *
 *	Module:		 ParamNames.h
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
 *        This file defines the constants used to signify parameters.
 * It also has two functions which return/take an index and convert.
 */
#ifndef INCParamNamesh
#define INCParamNamesh

static const int REFLECT      = 0x1;
static const int VELOCITY     = 0x2;
static const int SNR          = 0x4;
static const int SPEC_WIDTH   = 0x8;
static const int LINEAR_POWER = 0x10;
static const int REAL_R1      = 0x20;
static const int REAL_R2      = 0x40;
static const int IMAG_R1      = 0x80;
static const int IMAG_R2      = 0x100;

#ifndef _GNUC_
#define INLINE
#endif 

static INLINE int NumToParam(int n)
{
    switch(n) {
      case REFLECT: return(0);
      case VELOCITY: return(1);
      case SNR: return(2);
      case SPEC_WIDTH: return(3);
      case LINEAR_POWER: return(4);
      case REAL_R1: return(5);
      case REAL_R2: return(6);
      case IMAG_R1: return(7);
      case IMAG_R2: return(8);
      default: return(-1);
    }
}

static INLINE int ParamToNum(int p)
{
    switch(p) {
      case 0: return(REFLECT);
      case 1: return(VELOCITY);
      case 2: return(SNR);
      case 3: return(SPEC_WIDTH);
      case 4: return(LINEAR_POWER);
      case 5: return(REAL_R1);
      case 6: return(REAL_R2);
      case 7: return(IMAG_R1);
      case 8: return(IMAG_R2);
      default: return(-1);
    }
}

#undef INLINE

#endif /* INCParamNamesh */

