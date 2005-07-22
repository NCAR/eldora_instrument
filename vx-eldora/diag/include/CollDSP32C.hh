
//******************************************************************************
//                                                                            
// 	Copyright (c) 1994,
//
//	National Center for Atmospheric Research                              *
//									      *
//*****************************************************************************
                                                                               
// $Header$


#ifndef INCCollDSP32C
#define INCCollDSP32C

#include <logLib.h>

#include "QuadDSP32C.hh"

#include "DSP32C.hh"
extern "C" int nu_lddsp(int, int, int, int, char *);
extern "C" int dspgo(int, int, int, int);
extern "C" int dspstop(int, int, int, int);
extern "C" int coll_tst();


class CollDSP32C {

    public:
	CollDSP32C();
	CollDSP32C &CollDSP32C::operator=(const CollDSP32C & q);
	~CollDSP32C();
	int load(char* filename);
        int go();
	int stop();
	int memtst(void);
        int hsdbb_rd(long seed);
        int bimtst();
    
    private:
	DSP32C *dsp;
};

#ifndef NO_INLINE

inline int CollDSP32C::load( char *filename) {
    return nu_lddsp(1, 4, 0, 0, filename);
}

inline int CollDSP32C::go() {
    return dspgo(1, 4, 0, 0);
}

inline int CollDSP32C::stop() {
    return dspstop(1, 4, 0, 0);
}

inline int CollDSP32C::memtst() {
    return coll_tst();
}

#endif

#endif



//******************************************************************************
//Revision history:                                                           *
// $Log$
// Revision 1.2  1995/02/03  21:33:07  vanandel
// : Committing in include
// : Modified Files:
// : 	Antenna.hh AntennaController.hh AntennaMonitor.hh Ascope.hh
// : 	CB_TaskParams.hh CollDSP32C.hh DSP32C.hh DataAcquisition.hh
// : 	DummyAnt_Ctl.hh EtherOut.hh FlagsP.SLList.hh Gdb.hh
// : 	HouseKeep.hh PMAC_Scan.hh PMAC_Serial_Ctl.hh PMAC_VME_Ctl.hh
// : 	QuadDSP32C.hh SignalProc.hh
//
//  Revision 1.1  1994/10/24  23:38:40  vanandel
// SPOL Radar Data System, lab version
