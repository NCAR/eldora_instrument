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
 * Revision 1.8  1992/04/14  15:40:09  thor
 * Added include of string.h, since it will be needed.
 *
 * Revision 1.7  1992/04/14  15:03:48  thor
 * Changes INLINE to work on all c++'s. Added routines to go from string to
 * param.
 *
 * Revision 1.6  1992/02/19  16:08:04  thor
 * Added items for 3 frequencies for reflectivity, velocity & spectral width.
 *
 * Revision 1.5  1991/10/14  19:50:46  thor
 * Fixed typo.
 *
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

#include <string.h>

static char *ParamTapeNames[] = {
"DBZ", "VR", "SNR", "SW", "POWER", "REAL_R1", "REAL_R2", "IMAG_R1",
"IMAG_R2", "NCP", "DBZ1", "DBZ2", "DBZ3", "VR1", "VR2", "VR3", "SW1", "SW2",
"SW3",
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
static const int REFLECT1     = 0x400;
static const int REFLECT2     = 0x800;
static const int REFLECT3     = 0x1000;
static const int VELOCITY1    = 0x2000;
static const int VELOCITY2    = 0x4000;
static const int VELOCITY3    = 0x8000;
static const int SPEC_WIDTH1  = 0x10000;
static const int SPEC_WIDTH2  = 0x20000;
static const int SPEC_WIDTH3  = 0x40000;
static const int NO_PARAM     = -1;

#ifndef __cplusplus
#define INLINE
#else
#define INLINE inline
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
      case REFLECT1: return(10);
      case REFLECT2: return(11);
      case REFLECT3: return(12);
      case VELOCITY1: return(13);
      case VELOCITY2: return(14);
      case VELOCITY3: return(15);
      case SPEC_WIDTH1: return(16);
      case SPEC_WIDTH2: return(17);
      case SPEC_WIDTH3: return(18);
      case NO_PARAM: return(19);
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
      case 10: return(REFLECT1);
      case 11: return(REFLECT2);
      case 12: return(REFLECT3);
      case 13: return(VELOCITY1);
      case 14: return(VELOCITY2);
      case 15: return(VELOCITY3);
      case 16: return(SPEC_WIDTH1);
      case 17: return(SPEC_WIDTH2);
      case 18: return(SPEC_WIDTH3);
      default: return(-1);
    }
}

static INLINE int StringToParam(char *c)
{
    char **names = ParamTapeNames;
    int param = 0;

    while (*names != 0)
      {
	  if (!strcmp(c,*names))
	    return(param);

	  names++;
	  param++;
      }
    return(NO_PARAM);
}
    
#undef INLINE

#endif /* INCParamNamesh */
