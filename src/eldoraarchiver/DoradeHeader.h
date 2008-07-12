#ifndef DORADEHEADER_H_
#define DORADEHEADER_H_

# include <vector>

# include <DoradeCSPD.h>
# include <DoradeFRIB.h>
# include <DoradeNDDS.h>
# include <DoradePARM.h>
# include <DoradeRADD.h>
# include <DoradeSITU.h>
# include <DoradeVOLD.h>
# include <DoradeWAVE.h>

/**
 * Class to hold an ELDORA header, consisting of the following
 * DORADE descriptors:
 *      VOLD
 *      WAVE
 * 
 *      RADD     \
 *      FRIB     | * 2 (fore and aft)
 *      CSPD     |
 *      n * PARM /
 * 
 *      NDDS
 *      SITU
 */
class DoradeHeader {
public:
    /**
     * Class for exceptions from the DoradeHeader constructor.
     */
    class BadHeaderException : public std::string {
    public:
        BadHeaderException(std::string msg) : std::string(msg) {}
    };
    // Construct from a header file
    DoradeHeader(std::string fileName) throw(BadHeaderException);
    
    // Default constructor
    DoradeHeader() {};

    ~DoradeHeader();
    
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
    
    DoradeRADD* raddFore() const {
        if (_radd[0]->getRadarName() == "FORE")
            return _radd[0];
        else if (_radd[1]->getRadarName() == "FORE")
            return _radd[1];
        else
            return 0;
    }
    
    DoradeRADD* raddAft() const {
        if (_radd[0]->getRadarName() == "AFT")
            return _radd[0];
        else if (_radd[1]->getRadarName() == "AFT")
            return _radd[1];
        else
            return 0;
    }
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

#endif /*DORADEHEADER_H_*/
