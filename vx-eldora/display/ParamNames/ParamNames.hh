//
//	$Id$
//
//	Module:		 ParamNames.hh
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
//        This file defines the constants used to signify parameters.
// It also has two functions which return/take an index and convert.
//
#ifndef INCParamNameshh
#define INCParamNameshh

#include <string.h>


class ParamNames {
  public:

    enum params {
	REFLECT      = 0x1,
	VELOCITY     = 0x2,
	SNR          = 0x4,
	SPEC_WIDTH   = 0x8,
	LINEAR_POWER = 0x10,
	REAL_R1      = 0x20,
	REAL_R2      = 0x40,
	IMAG_R1      = 0x80,
	IMAG_R2      = 0x100,
	NCP          = 0x200,
	REFLECT1     = 0x400,
	REFLECT2     = 0x800,
	REFLECT3     = 0x1000,
	VELOCITY1    = 0x2000,
	VELOCITY2    = 0x4000,
	VELOCITY3    = 0x8000,
	SPEC_WIDTH1  = 0x10000,
	SPEC_WIDTH2  = 0x20000,
	SPEC_WIDTH3  = 0x40000,
	NO_PARAM     = -1
    };
    
    ParamNames(char **n = 0);

    char *numToName(int i);

    char *paramToName(int p);

    int numToParam(int i);

    int paramToNum(int i);

    int nameToNum(char *n);

    int nameToParam(char *n);

    ~ParamNames();

  private:
    char **names;

};

#ifdef PARAMNAMES_SCOPE
static char *ParamTapeNames[] = {
"DBZ", "VR", "SNR", "SW", "POWER", "REAL_R1", "REAL_R2", "IMAG_R1",
"IMAG_R2", "NCP", "DBZ1", "DBZ2", "DBZ3", "VR1", "VR2", "VR3", "SW1", "SW2",
"SW3",
0};
#endif // PARAMNAMES_SCOPE

#endif // INCParamNameshh
