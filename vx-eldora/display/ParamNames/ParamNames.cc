//  This looks like C, but it's really C++!!
//	$Id$
//
//	Module:		 ParamNames.cc
//	Original Author: Richard E. Neitzel
//      Copywrited by the National Center for Atmospheric Research
//	Date:		 $Date$
//
// revision history
// ----------------
// $Log$
// Revision 1.3  1994/08/12  17:24:29  thor
// Added const to methods declarations.
//
// Revision 1.2  1993/12/14  17:21:05  thor
// Fixed things to use static data member.
//
// Revision 1.1  1993/12/09  20:19:24  thor
// Initial revision
//
//
//
// description:
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define PARAMNAMES_SCOPE
#include "ParamNames.hh"

#include <string.h>

int ParamNames::paramToNum(register int p) const
{
    switch(p) {
      case ParamNames::REFLECT: return(0);
      case ParamNames::VELOCITY: return(1);
      case ParamNames::SNR: return(2);
      case ParamNames::SPEC_WIDTH: return(3);
      case ParamNames::LINEAR_POWER: return(4);
      case ParamNames::REAL_R1: return(5);
      case ParamNames::REAL_R2: return(6);
      case ParamNames::IMAG_R1: return(7);
      case ParamNames::IMAG_R2: return(8);
      case ParamNames::NCP: return(9);
      case ParamNames::REFLECT1: return(10);
      case ParamNames::REFLECT2: return(11);
      case ParamNames::REFLECT3: return(12);
      case ParamNames::VELOCITY1: return(13);
      case ParamNames::VELOCITY2: return(14);
      case ParamNames::VELOCITY3: return(15);
      case ParamNames::SPEC_WIDTH1: return(16);
      case ParamNames::SPEC_WIDTH2: return(17);
      case ParamNames::SPEC_WIDTH3: return(18);
      case ParamNames::VEL_LONG: return(19);
      case ParamNames::VEL_SHORT: return(20);
      case ParamNames::VEL_UNFOLDED: return(21);
      case ParamNames::NO_PARAM: return(22);
      default: return(-1);
    }
}

int ParamNames::numToParam(register int n) const
{
    switch(n) {
      case 0: return(ParamNames::REFLECT);
      case 1: return(ParamNames::VELOCITY);
      case 2: return(ParamNames::SNR);
      case 3: return(ParamNames::SPEC_WIDTH);
      case 4: return(ParamNames::LINEAR_POWER);
      case 5: return(ParamNames::REAL_R1);
      case 6: return(ParamNames::REAL_R2);
      case 7: return(ParamNames::IMAG_R1);
      case 8: return(ParamNames::IMAG_R2);
      case 9: return(ParamNames::NCP);
      case 10: return(ParamNames::REFLECT1);
      case 11: return(ParamNames::REFLECT2);
      case 12: return(ParamNames::REFLECT3);
      case 13: return(ParamNames::VELOCITY1);
      case 14: return(ParamNames::VELOCITY2);
      case 15: return(ParamNames::VELOCITY3);
      case 16: return(ParamNames::SPEC_WIDTH1);
      case 17: return(ParamNames::SPEC_WIDTH2);
      case 18: return(ParamNames::SPEC_WIDTH3);
      case 19: return(ParamNames::VEL_LONG);
      case 20: return(ParamNames::VEL_SHORT);
      case 21: return(ParamNames::VEL_UNFOLDED);
      default: return(ParamNames::NO_PARAM);
    }
}

int ParamNames::nameToParam(register const char *c) const
{
    char **n = names;
    int param = 0;

    while (*n != 0)
      {
	  if (!strcmp(c,*names))
	    return(numToParam(param));

	  n++;
	  param++;
      }
    return(NO_PARAM);
}

int ParamNames::nameToNum(register const char *c) const
{
    char **n = names;
    int param = 0;

    while (*n != 0)
      {
	  if (!strcmp(c,*names))
	    return(param);

	  n++;
	  param++;
      }
    return(-1);
}
 
const char *ParamNames::numToName(register int num) const
{
    if (num < 0 || num > paramToNum(ParamNames::VEL_UNFOLDED))
      return(NULL);
    
    return(names[num]);
}

const char *ParamNames::paramToName(register int num) const
{
    register int index = paramToNum(num);
    if (index == -1)
      return(NULL);
    
    return(names[index]);
}
