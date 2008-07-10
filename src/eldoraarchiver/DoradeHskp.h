#ifndef DORADEHSKP_H_
#define DORADEHSKP_H_

#include "EldoraDdsC.h"

# include <DoradeASIB.h>
# include <DoradeFRAD.h>
# include <DoradeRYIB.h>

/**
 * Class to hold an ELDORA housekeeping block, consisting of the following
 * DORADE descriptors:
 *      RYIB
 *      ASIB
 *      (dataless) FRAD
 */
class DoradeHskp {
public:
    // Construct from EldoraDDS::Housekeeping
    DoradeHskp(const EldoraDDS::Housekeeping& ddsHskp);
    
    // Default constructor
    DoradeHskp() {};

    ~DoradeHskp();
    
    // return a pointer to our RYIB (RaY Information Block descriptor)
    DoradeRYIB* ryib() const { return _ryib; }
    // return a pointer to our ASIB (Aircraft/Ship Information Block descriptor)
    DoradeASIB* asib() const { return _asib; }
    // return a pointer to our FRAD (Field RAdar Data descriptor)
    DoradeFRAD* frad() const { return _frad; }
    /**
     * Return the size of the external representation of this housekeeping, 
     * in bytes.  (This is the number of bytes that will be written by the 
     * streamTo() method)
     * @return the size of the external representation of the housekeeping, 
     * in bytes
     */
    unsigned int size() const;
    /**
     * Put the binary representation of the housekeeping to the given stream.
     * @param os the stream to be written.
     * @param asLittleEndian true iff the binary representation should be 
     *      written in little-endian order.  Although the DORADE spec calls for
     *      big-endian, there is a long history of writing little-endian
     *      DORADE(-like) files.  This parameter defaults to false.
     * @return the std::ostream being written
     */
    std::ostream& streamTo(std::ostream& os, bool asLittleEndian) const;
    
private:
    DoradeRYIB* _ryib;
    DoradeASIB* _asib;
    DoradeFRAD* _frad;
};

#endif /*DORADEHSKP_H_*/
