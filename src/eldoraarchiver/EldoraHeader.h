#ifndef ELDORAHEADER_H_
#define ELDORAHEADER_H_

# include <vector>

# include <DoradeCSPD.h>
# include <DoradeFRIB.h>
# include <DoradeNDDS.h>
# include <DoradePARM.h>
# include <DoradeRADD.h>
# include <DoradeSITU.h>
# include <DoradeVOLD.h>
# include <DoradeWAVE.h>

class EldoraHeader {
public:
    class BadHeaderException : public std::string {
    public:
        BadHeaderException(std::string msg) : std::string(msg) {}
    };
    
    // Construct from a header file
    EldoraHeader(std::string fileName) throw(BadHeaderException);

//    // Construct from a character array containing raw header data
//    EldoraHeader(const char* rawData, int len) throw(BadHeaderException);
    
    ~EldoraHeader();
    
    // return a pointer to the VOLD (VOLume Descriptor)
    DoradeVOLD* vold() const { return _vold; }
    // return a pointer to the WAVE (WAVEform information block descriptor)
    DoradeWAVE* wave() const { return _wave; }
    // return a pointer to the SITU (in-SITU data descriptor)
    DoradeSITU* situ() const { return _situ; }
    // return a pointer to the NDDS (Navigational Data DeScriptor)
    DoradeNDDS* ndds() const { return _ndds; }
    // return a pointer to the RADD {RADar Descriptor) for the selected radar
    DoradeRADD* radd(int radar) const { return _radd[radar]; }
    // return a pointer to the FRIB (Field Radar Information Block descriptor) 
    // for the selected radar
    DoradeFRIB* frib(int radar) const { return _frib[radar]; }
    // return a pointer to the CSPD (Cell SPacing Descriptor) for the 
    // selected radar
    DoradeCSPD* cspd(int radar) const { return _cspd[radar]; }
    // return the number of PARM descriptors for the selected radar
    unsigned int nParams(int radar) const { return _radd[radar]->getNParams(); }
    // return a pointer to the PARM (PARaMeter) descriptor for the selected 
    // radar and parameter number
    DoradePARM* parm(int radar, int paramNum) const { return _parm[radar][paramNum]; }
    /**
     * Return the size of the external representation of this header, in bytes.
     * (This is the number of bytes that will be written by the streamTo()
     * method)
     * @return the size of the external representation of the header, in bytes
     */
    unsigned int size() const;
    /**
     * Put the binary representation of the header to the given stream.
     * @param os the stream to be written.
     * @param asLittleEndian true iff the binary representation should be 
     *      written in little-endian order.  Although the DORADE spec calls for
     *      big-endian, there is a long history of writing little-endian
     *      DORADE(-like) files.  This parameter defaults to false.
     * @return the std::ostream being written
     */
    std::ostream& streamTo(std::ostream& os, bool asLittleEndian) const;
    
private:
    DoradeVOLD* _vold;
    DoradeWAVE* _wave;
    DoradeSITU* _situ;
    DoradeNDDS* _ndds;
    DoradeRADD* _radd[2];
    DoradeFRIB* _frib[2];
    DoradeCSPD* _cspd[2];
    std::vector<DoradePARM*> _parm[2];
};

#endif /*ELDORAHEADER_H_*/
