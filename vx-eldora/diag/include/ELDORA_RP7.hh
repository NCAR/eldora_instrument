
//******************************************************************************
//                                                                            
// 	Copyright (c) 1994,
//
//	National Center for Atmospheric Research                              *
//									      *
//*****************************************************************************
                                                                               
#ifndef INCELDORA_RP7
#define INCELDORA_RP7

#include <logLib.h>

#include "QuadDSP32C.hh"
#include "CollDSP32C.hh"
#include "DSP32C.hh"

class ELDORA_RP7 {

    public:
	ELDORA_RP7();
	~ELDORA_RP7();
        int ppp_tst(short ppp_num);
        int dp_tst(short dp_num);
        int coll_tst();
        int digif2ppp_tst(short ppp_num);
        int ppp2dp_tst(short ppp_num);
        int dp2coll_tst(short dp_num);
    private:
        short ppp_num, dp_num;
        QuadDSP32C *quad[7];
        CollDSP32C *coll;
    
};

#endif





