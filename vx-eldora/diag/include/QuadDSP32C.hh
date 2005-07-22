
//******************************************************************************
//                                                                            
// 	Copyright (c) 1992,
//
//	National Center for Atmospheric Research                              *
//									      *
//*****************************************************************************
                                                                               
// $Header$


#ifndef INCQuadDSP32C
#define INCQuadDSP32C

#include "DSP32C.hh"
extern "C" int nu_lddsp(int, int, int, int, char *);
extern "C" int dspgo(int, int, int, int);
extern "C" int dspstop(int, int, int, int);
extern "C" int quad_tst(int, int);
extern "C" int dsy(unsigned long, unsigned long, unsigned int);


class CollDSP32C;

class QuadDSP32C {

    public:
	QuadDSP32C(unsigned short freq, unsigned short board_num);
	QuadDSP32C &QuadDSP32C::operator=(const QuadDSP32C & q);
	~QuadDSP32C();
	int load(short proc,  char* filename);
	int go(short proc);
	int stop(short proc);
	int memtst(void);
	int hsdbb_wr(int proc, long seed );
        int hsdba_wr(int proc, long seed );
        int hsdba_rd(int proc, int digif_flag, long seed );
	int daisy(unsigned  short  value);
        int toggle_pdr(void);
    
    private:
	unsigned short frequency_;
	unsigned short board_num_;
	DSP32C *dsp[4];
};

#ifndef NO_INLINE

inline int QuadDSP32C::load(short proc, char *filename) {
    return nu_lddsp(1, frequency_, board_num_, proc, filename);
}

inline int QuadDSP32C::go(short proc ) {
    return dspgo(1, frequency_, board_num_, proc);
}

inline int QuadDSP32C::stop(short proc ) {
    return dspstop(1, frequency_, board_num_, proc);
}
inline int QuadDSP32C::daisy(unsigned short value) {
    dsy(frequency_, board_num_, value);
}

inline int QuadDSP32C::memtst() {
    return quad_tst( frequency_, board_num_);
}

#endif




//******************************************************************************
//Revision history:                                                           *
// $Log$
// Revision 1.2  1995/02/03  21:33:23  vanandel
// : Committing in include
// : Modified Files:
// : 	Antenna.hh AntennaController.hh AntennaMonitor.hh Ascope.hh
// : 	CB_TaskParams.hh CollDSP32C.hh DSP32C.hh DataAcquisition.hh
// : 	DummyAnt_Ctl.hh EtherOut.hh FlagsP.SLList.hh Gdb.hh
// : 	HouseKeep.hh PMAC_Scan.hh PMAC_Serial_Ctl.hh PMAC_VME_Ctl.hh
// : 	QuadDSP32C.hh SignalProc.hh
//
// Revision 1.1  1994/10/24  23:38:54  vanandel
// SPOL Radar Data System, lab version
// Revision 1.7  1994/03/07  16:46:51  vanandel
// : Committing in antenna
// :
// : Modified Files:
// : 	PMAC_Scan.cc angleSpec.cc
// : ----------------------------------------------------------------------
// Change to only send last angle twice for >1 fixed angle
//
//
// : Committing in dac
// :
// : Modified Files:
// : 	DataAcquisition.cc HouseKeep.cc
// : ----------------------------------------------------------------------
// Change to sample transition flag and fixed angle at midbeam time,
// along with antenna az,el
// : Committing in doc
// :
// : Modified Files:
// : 	AntDiag.html Roadmap.html Suntrack.html
// : ----------------------------------------------------------------------
// Updated documentation
// : Committing in exec
// :
// : Modified Files:
// : 	Imakefile exec.hh init.tcl
// : ----------------------------------------------------------------------
// Scheduling additions:
// 1) allow queuing scans
// 2) scheduler procedure to start scans on minute:second boundaries
//
// : Committing in include
// :
// : Modified Files:
// : 	AngleSpec.hh DataAcquisition.hh HouseKeep.hh QuadDSP32C.hh
// : ----------------------------------------------------------------------
//
// Revision 1.6  1993/10/18  15:54:36  vanandel
// time series support
//
// Revision 1.5  1993/01/13  18:32:32  vanandel
// added method to read average power
//
// Revision 1.4  1993/01/13  16:47:51  vanandel
// cleanup - setScale not a public method anymore
//
// Revision 1.3  1993/01/11  15:28:01  vanandel
// added DC offset removal
// prf support
//
// Revision 1.2  1992/12/22  18:42:35  vanandel
// added methods for setting gates, samples, spacing,
// loading code, and start/stop
//
// Revision 1.1  1992/12/21  16:28:49  vanandel
// Initial revision
//
//									      *
//*******************************END OF RCS INFO*******************************
#endif INCQuadDSP32CHH
