/*
 * Dump an ELDORA field format file
 */
#include <iostream>
#include <fstream>
#include <cassert>
#include <DoradeCSPD.h>
#include <DoradeFRAD.h>
#include <DoradePARM.h>
#include <DoradeRADD.h>
#include <DoradeRAWD.h>
#include <DoradeRYIB.h>
#include <DoradeVOLD.h>

int
main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <eldora_field_fmt_file>" <<
            std::endl;
        exit(1);
    }
    
    char block[65536];
    unsigned char* ublock = (unsigned char*)block;
    
    std::ifstream ifile(argv[1]);
    if (ifile.fail()) {
        std::cerr << "Failed to open " << argv[1] << std::endl;
        exit(1);
    }
    
    ifile.seekg(0, std::ios::end);
    int fileLen = ifile.tellg();
    ifile.seekg(0, std::ios::beg);

    try {
        while (! ifile.eof() && (ifile.tellg() < fileLen)) {
            // read the length of this block
            unsigned int blocksize;
            ifile.read(block, 4);
            blocksize = (ublock[0] << 24) | (ublock[1] << 16) | 
                (ublock[2] << 8) | ublock[3];
            assert(blocksize <= sizeof(block));

            // read the block
            ifile.read(block, blocksize);
            unsigned int blockpos = 0;
            
            int sensor = -1;

            while (blockpos < blocksize) {
                unsigned char* udata = (unsigned char*)block + blockpos;
                DoradeDescriptor desc(udata, blocksize - blockpos, false);
                std::string descName(desc.getDescName());
                unsigned int descLen(desc.getDescLen());

                if (descName == "VOLD") {
                    DoradeVOLD vold(udata, blocksize - blockpos, false);
                    vold.setVerbose(true);
                    // Historical KLUGE: VOLD "sensor descriptor count" for
                    // ELDORA is actually twice the number of sensors.  I'm 
                    // guessing this is because they are counting both RADD
                    // and FRIB descriptors as "sensor" descriptors, while
                    // the DORADE document shows a sensor descriptor as the
                    // whole block of descriptors describing a sensor and its
                    // parameters.
                    int nSensors = vold.getSensorCount() / 2;
                    std::cerr << "VOLD @ " << vold.getVolumeDateTime() << 
                        " with " << nSensors << " sensors" << std::endl;
                } else if (descName == "RADD") {
                    sensor++;
                    DoradeRADD radd(udata, blocksize - blockpos, false);
                    std::cerr << "\tRADD for " << radd.getRadarName() << 
                        std::endl;   
                } else if (desc.getDescName() == "PARM") {
                    DoradePARM parm(udata, blocksize - blockpos, false);
                    int paramSize = 0;
                    switch (parm.getParamType()) {
                    case 1:
                        paramSize = 1;
                        break;
                    case 2:
                        paramSize = 2;
                        break;
                    case 3:
                    case 4:
                        paramSize = 4;
                        break;
                    }
                    std::cerr << "\tPARM " << parm.getParamName() << 
                        " for sensor " << sensor << ", " << paramSize << 
                        " bytes" << std::endl;
                } else if (descName == "RYIB") {
                    DoradeRYIB ryib(udata, blocksize - blockpos, false);
                    std::cerr << "\tRYIB @ " << ryib.getDayOfYear() << "/" << 
                        ryib.getRayTime() << std::endl;   
                } else if (desc.getDescName() == "RAWD") {
                    DoradeRAWD rawd(udata, blocksize - blockpos, false);
                    // RAWD is a strange bird, and we don't get a good descriptor
                    // length until we have a DoradeRAWD rather than just
                    // a DoradeDescriptor.
                    descLen = rawd.getDescLen();
                    std::cerr << "\tRAWD (" << descLen << " bytes)" << std::endl;
                } else if (desc.getDescName() == "CSPD") {
                    DoradeCSPD cspd(udata, blocksize - blockpos, false);
                    short nsegs = cspd.getNSegments();
                    const short* ncells = cspd.getNCells();
                    int nGates = 0;
                    for (int s = 0; s < nsegs; s++)
                        nGates += ncells[s];
                    std::cerr << "\t" << "CSPD with " << nGates << 
                        " gates" << std::endl;
                } else if (desc.getDescName() == "FRAD") {
                    DoradeFRAD frad(udata, blocksize - blockpos, false);
                    const short* sdata = frad.getShortData();
                    std::cerr << "\t" << "FRAD [";
                    for (int i = 0; i < 10; i++)
                        std::cerr << sdata[i] << ", ";
                    std::cerr << "...]" << std::endl;
                } else {
                    std::cerr << "\t" << desc.getDescName() << " (" << 
                        desc.getDescLen() << " bytes)" << std::endl;
                }
                blockpos += descLen;
            }

            // trailing block length
            int endblocksize;
            ifile.read(block, 4);
            endblocksize = (ublock[0] << 24) | (ublock[1] << 16) | 
                (ublock[2] << 8) | ublock[3];
        }
    } 
    catch (DescriptorException dex) {
        std::cerr << dex.what() << std::endl;
        abort();
    }
}
