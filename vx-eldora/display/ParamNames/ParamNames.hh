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
// Revision 1.4  1994/09/12  15:56:13  thor
// Added new parameters per Craig.
//
// Revision 1.3  1994/08/12  17:24:29  thor
// Added const to methods declarations.
//
// Revision 1.2  1993/12/14  17:21:05  thor
// Fixed things to use static data member.
//
// Revision 1.1  1993/12/09  20:19:26  thor
// Initial revision
//
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
        VEL_LONG     = 0x80000,
        VEL_SHORT    = 0x100000,
        SPEC_WIDTHS  = 0x200000,
        SPEC_WIDTHL  = 0x400000,
        NCPL         = 0x800000,
        DBZL         = 0x1000000,
	NO_PARAM     = -1
    };
    
    ParamNames() {}

    const char *numToName(int i) const;

    const char *paramToName(int p) const;

    int numToParam(int i) const;

    int paramToNum(int i) const;

    int nameToNum(const char *n) const;

    int nameToParam(const char *n) const;

    ~ParamNames() {}

  private:
    static char *names[];

};

#ifdef PARAMNAMES_SCOPE
char *ParamNames::names[] = {
"DBZ", "VR", "SNR", "SW", "POWER", "REAL_R1", "REAL_R2", "IMAG_R1",
"IMAG_R2", "NCP", "DBZ1", "DBZ2", "DBZ3", "VR1", "VR2", "VR3", "SW1", "SW2",
"SW3", "VL", "VS", "SWS", "SWL", "NCPL", "DBZL",
0};
#endif // PARAMNAMES_SCOPE

#endif // INCParamNameshh
