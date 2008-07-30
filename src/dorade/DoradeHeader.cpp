# include <iostream>
# include <fstream>
# include <stdlib.h>
# include <cstring>

# include "DoradeHeader.h"


DoradeHeader::DoradeHeader(std::string fileName, bool littleIn, bool fortranBlocking ) throw(BadHeaderException) {
    
    std::ifstream infile(fileName.c_str());
    if (infile.fail())
        throw BadHeaderException(std::string() + "Failed to open header '" + 
                                 fileName + "'");

    // allocate a char buffer to hold the entire file
    infile.seekg(0, std::ios::end);
    int length = infile.tellg();
    infile.seekg(0, std::ios::beg);

    unsigned char* buf = new unsigned char[length];
    infile.read((char*)buf, length);
    infile.close();
    
    unsigned char* data = buf;
    unsigned char* ublock = (unsigned char*)buf;
    unsigned int blocksize;
    

    int datalen = length;
    // if we're dealing with a Fortran blocked file - e.g. an Eldora
    // Archive, we need to skip the record size (4 byte) before and after
    // every record
    // however, it seems safe to assume that the entire DORADE HEADER is
    // contained in a single Fortran Blocked record, so we only have to
    // skip a single 4 byte block length
    
    
    int fortranBlockSize = 0;
    if (fortranBlocking) {
        fortranBlockSize = 4;
        // decode the big-endinan record size
        blocksize = (ublock[0] << 24) | (ublock[1] << 16) | 
                (ublock[2] << 8) | ublock[3];
        std::cerr << "blocksize = " << blocksize << std::endl;
        data += fortranBlockSize;
        datalen -= fortranBlockSize;
    }
    
    
    try {
        _vold = new DoradeVOLD(data, datalen, littleIn);
        data += _vold->getDescLen() ;
        datalen -= _vold->getDescLen();

        _wave = new DoradeWAVE(data, datalen, littleIn);
        data += _wave->getDescLen();
        datalen -= _wave->getDescLen();

        for (int r = 0; r < 2; r++) {
            _radd[r] = new DoradeRADD(data, datalen, littleIn);
            data += _radd[r]->getDescLen();
            datalen -= _radd[r]->getDescLen();

            _frib[r] = new DoradeFRIB(data, datalen, littleIn);
            data += _frib[r]->getDescLen();
            datalen -= _frib[r]->getDescLen();

            _cspd[r] = new DoradeCSPD(data, datalen, littleIn);
            data += _cspd[r]->getDescLen();
            datalen -= _cspd[r]->getDescLen();

            for (unsigned int p = 0; p < nParams(r); p++) {
                _parm[r].push_back(new DoradePARM(data, datalen, littleIn));
                data += _parm[r][p]->getDescLen();
                datalen -= _parm[r][p]->getDescLen();
            }
        }
        _ndds = new DoradeNDDS(data, datalen, littleIn);
        data += _ndds->getDescLen();
        datalen -= _ndds->getDescLen();
        
        _situ = new DoradeSITU(data, datalen, littleIn);
        data += _situ->getDescLen();
        datalen -= _situ->getDescLen();
    } catch (DescriptorException e) {
        throw BadHeaderException(std::string() + "descriptor exception: " + 
                                 e.what());
        exit(1);
    }
}

DoradeHeader::~DoradeHeader() {
    delete _vold;
    delete _wave;
    delete _ndds;
    delete _situ;
    for (unsigned int r = 0; r < 2; r++) {
        delete _radd[r];
        delete _frib[r];
        delete _cspd[r];
        for (unsigned int p = 0; p < _parm[r].size(); p++) {
            delete _parm[r][p];
        }
    }
}

std::ostream&
DoradeHeader::streamTo(std::ostream& os, bool asLittleEndian) const {
    _vold->streamTo(os, asLittleEndian);
    _wave->streamTo(os, asLittleEndian);
    for (unsigned int r = 0; r < 2; r++) {
        _radd[r]->streamTo(os, asLittleEndian);
        _frib[r]->streamTo(os, asLittleEndian);
        _cspd[r]->streamTo(os, asLittleEndian);
        for (unsigned int p = 0; p < nParams(r); p++)
            _parm[r][p]->streamTo(os, asLittleEndian);
    }
    _ndds->streamTo(os, asLittleEndian);
    _situ->streamTo(os, asLittleEndian);
    return os;
}

unsigned int
DoradeHeader::size() const {
    unsigned int nbytes = 0;
    nbytes += _vold->getDescLen();
    nbytes += _wave->getDescLen();
    for (unsigned int r = 0; r < 2; r++) {
        nbytes += _radd[r]->getDescLen();
        nbytes += _frib[r]->getDescLen();
        nbytes += _cspd[r]->getDescLen();
        for (unsigned int p = 0; p < nParams(r); p++)
            nbytes += _parm[r][p]->getDescLen();
    }
    nbytes += _ndds->getDescLen();
    nbytes += _situ->getDescLen();
    return nbytes;
}
