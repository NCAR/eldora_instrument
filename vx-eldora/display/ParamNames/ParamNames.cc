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
//
//
// description:
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define PARAMNAMES_SCOPE
#include "ParamNames.hh"

#include <string.h>

ParamNames::ParamNames(register char **n)
{
    if (n != 0)
      n = ParamTapeNames;

    register char **p = names;
    
    while (*n != 0)
      {
	  register int l = strlen(*n);
	  *p = new char[l + 1];
	  strcpy(*n,*p);
	  p++;
	  n++;
      }
}

int ParamNames::paramToNum(register int p)
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
      case ParamNames::NO_PARAM: return(19);
      default: return(-1);
    }
}

int ParamNames::numToParam(register int n)
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
      default: return(ParamNames::NO_PARAM);
    }
}

int ParamNames::nameToParam(register char *c)
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

int ParamNames::nameToNum(register char *c)
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
 
char *ParamNames::numToName(register int num)
{
    if (num < 0 || num > paramToNum(ParamNames::SPEC_WIDTH3))
      return(NULL);
    
    return(names[num]);
}

char *ParamNames::paramToName(register int num)
{
    if (paramToNum(num) == -1)
      return(NULL);
    
    return(names[num]);
}
