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
 * Revision 1.4  1991/10/14  19:40:36  thor
 * Added char strings for variables and NCP.
 *
 * Revision 1.3  1991/09/30  14:58:11  thor
 * Added no parameter value.
 *
 * Revision 1.2  1991/09/18  15:26:38  thor
 * Fixed backwards naming of inlines.
 *
 * Revision 1.1  1991/09/17  16:32:29  thor
 * Initial revision
 *
 *
 *
 * description:
 *        This file defines the constants used to signify parameters.
 * It also has two functions which return/take an index and convert.
 */
#ifndef INCParamNamesh
#define INCParamNamesh

static char *ParamTapeNames[] = {
"DBZ", "VR", "SNR", "SW", "POWER", "REAL_R1", "REAL_R2", "IMAG_R1",
"IMAG_R2", "NCP",
0};

static const int REFLECT      = 0x1;
static const int VELOCITY     = 0x2;
static const int SNR          = 0x4;
static const int SPEC_WIDTH   = 0x8;
static const int LINEAR_POWER = 0x10;
static const int REAL_R1      = 0x20;
static const int REAL_R2      = 0x40;
static const int IMAG_R1      = 0x80;
static const int IMAG_R2      = 0x100;
static const int NCP          = 0x200;
static const int NO_PARAM     = -1;

#ifndef _GNUC_
#define INLINE
#endif 

static INLINE int ParamToNum(register int p)
{
    switch(p) {
      case REFLECT: return(0);
      case VELOCITY: return(1);
      case SNR: return(2);
      case SPEC_WIDTH: return(3);
      case LINEAR_POWER: return(4);
      case REAL_R1: return(5);
      case REAL_R2: return(6);
      case IMAG_R1: return(7);
      case IMAG_R2: return(8);
      case NCP: return(9);
      case NO_PARAM: return(10);
      default: return(-1);
    }
}

static INLINE int NumToParam(register int n)
{
    switch(n) {
      case 0: return(REFLECT);
      case 1: return(VELOCITY);
      case 2: return(SNR);
      case 3: return(SPEC_WIDTH);
      case 4: return(LINEAR_POWER);
      case 5: return(REAL_R1);
      case 6: return(REAL_R2);
      case 7: return(IMAG_R1);
      case 8: return(IMAG_R2);
      case 9: return(NCP);
      default: return(-1);
    }
}

#undef INLINE

#endif /* INCParamNamesh */

