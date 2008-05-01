
//******************************************************************************
//                                                                            
// 	Copyright (c) 1994,
//
//	National Center for Atmospheric Research                              *
//									      *
//*****************************************************************************
                                                                               
// $Header$


#ifndef INCDSP32C
#define INCDSP32C



class DSP32C {

    public:
	DSP32C(unsigned char * baseAddr);
	void pcrl(unsigned char val);	// write the register 
	void pcrh(unsigned char val);
	unsigned short pcr16(void);
	void setDMAaddr(unsigned long addr);
	void pdr16(unsigned short);
	void pdr216(unsigned short);
        void pir16(unsigned short);
	unsigned short pdr16();
	unsigned short pdr216();
        unsigned short pir16();
	

    private:
	unsigned char* pcrl_;
	unsigned char* pcrh_;
	unsigned short* pcr16_;
	unsigned short* pare16_;
	unsigned short* par16_;
	unsigned short* pdr16_;
	unsigned short* pdr216_;
        unsigned short* pir16_;
};

#ifndef NO_INLINE


inline void DSP32C::pcrl(unsigned char val) {
    *pcrl_ = val;
}
inline void DSP32C::pcrh(unsigned char val) {
    *pcrh_ = val;
}

inline void DSP32C::pdr16(unsigned short val) {
    *pdr16_ = val;
}
inline void DSP32C::pdr216(unsigned short val) {
    *pdr216_ = val;
}
inline void DSP32C::pir16(unsigned short val) {
    *pir16_ = val;
}

inline unsigned short DSP32C::pcr16(void ) {
    return *pcr16_;
}
inline unsigned short DSP32C::pdr16(void ) {
    return *pdr16_;
}
inline unsigned short DSP32C::pdr216(void ) {
    return *pdr216_;
}
inline unsigned short DSP32C::pir16(void ) {
    return *pir16_;
}



#endif




//******************************************************************************
//Revision history:                                                           *
// $Log$
// Revision 1.2  1995/02/03  21:33:08  vanandel
// : Committing in include
// : Modified Files:
// : 	Antenna.hh AntennaController.hh AntennaMonitor.hh Ascope.hh
// : 	CB_TaskParams.hh CollDSP32C.hh DSP32C.hh DataAcquisition.hh
// : 	DummyAnt_Ctl.hh EtherOut.hh FlagsP.SLList.hh Gdb.hh
// : 	HouseKeep.hh PMAC_Scan.hh PMAC_Serial_Ctl.hh PMAC_VME_Ctl.hh
// : 	QuadDSP32C.hh SignalProc.hh
//
// Revision 1.1  1994/10/24  23:38:41  vanandel
//  SPOL Radar Data System, lab version
//
//									      *
//*******************************END OF RCS INFO*******************************
#endif INCDSP32CHH
