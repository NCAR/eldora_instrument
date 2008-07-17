#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cstdio>   // for popen()
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// headers for radd/archiver
#include <archiver/ArchiverListener.h>
#include <archiver/Resolver.h>
#include <archiver/EldoraWriter.h>
#include <tao/ORB_Core.h>

#include <dds/DCPS/Service_Participant.h>    // for TheServiceParticipant

#include <DoradeASIB.h>
#include <DoradeFRAD.h>
#include <DoradeRYIB.h>

#include <boost/date_time/posix_time/ptime.hpp>

#include "DoradeHeader.h"
#include "EldoraArchiver.h"

using std::string;

using namespace archiver;

typedef NameResolver<PortableServer::POA, InitResolver> POAResolver;

// Pointer to our singleton instance
EldoraArchiver* EldoraArchiver::_theArchiver = 0;

EldoraArchiver::EldoraArchiver(DDSSubscriber& subscriber, 
        std::string topicName, std::string dataDir) : 
ProductsReader(subscriber, topicName), 
_hdr(), 
_hdrValid(false),
_dataBuf(), 
_dataBufLen(0), 
_raysWritten(0) {
    std::cout << "Writing to data dir: " << dataDir << std::endl;
    
    // Create the archiver servant
    ArchiverService_impl* serviceImpl = 
        new ArchiverService_impl("archiver", TheServiceParticipant->get_ORB(),
                new archiver::EldoraWriter(), /*standalone*/ true);
    if (! serviceImpl) {
        std::cerr << __FUNCTION__ << ":" << __LINE__ << 
            ": Failed to create ArchiverService_impl";
        exit(1);
    }
    _archiverServant = serviceImpl->_this();

    // Configure the archiver servant and start it
    ArchiverConfig config = archiver::defaultConfig();
    config.fileName = "eldora";
    config.fileSizeLimit = 100 * 1024 * 1024; // 100 MB
    config.directoryName = dataDir.c_str();
    _status = _archiverServant->reconfig(config);
    _status = _archiverServant->start();
}

EldoraArchiver::~EldoraArchiver() {
    _archiverServant->quit();
    _theArchiver = 0;
}

// Load a new ELDORA header
bool
EldoraArchiver::loadHeader(std::string hdrFileName) {
    std::cout << "Requested to load header " << hdrFileName << std::endl;
    // Bail out quickly if the file does not exist.
    struct stat statbuf;
    if (stat(hdrFileName.c_str(), &statbuf)) {
        std::cerr << "Unable to find header file: " << hdrFileName << std::endl;
        _hdrValid = false;
        return _hdrValid;
    }
    // Get the checksum for the file.
    unsigned int checksum(0);
    std::string cksumCmd("/usr/bin/cksum ");
    cksumCmd += hdrFileName;
    FILE *cksumOutput = popen(cksumCmd.c_str(), "r");
    if (! cksumOutput) {
        std::cerr << "Failed to run command: '" << cksumCmd << "'" << std::endl;
        _hdrValid = false;
        return _hdrValid;
    }
    fscanf(cksumOutput, "%u", &checksum);
    pclose(cksumOutput);
    
    // If the checksum matches our current header, we're done here.
    if (_hdrValid && (checksum == _hdrChecksum)) {
        std::cout << "Checksum matches current header; header not changed." <<
            std::endl;
        return true;
    }
    
    // Now mark header as invalid until we successfully load the new header.
    // We don't invalidate it until now so that an existing header remains
    // valid through the checksum test above; this assures that data writing
    // will not stop if the same header is loaded again.
    _hdrValid = false;
    _hdrChecksum = 0;
    
    // Create a DoradeHeader from the header file
    try {
        _hdr = new DoradeHeader(hdrFileName);
    } catch (DoradeHeader::BadHeaderException ex) {
        std::cerr << "Header exception: " << ex << std::endl;
        return false;
    }

    // Keep the header checksum
    _hdrChecksum = checksum;
    
    // Set the volume time and file generation date to now
    boost::posix_time::ptime now = 
        boost::posix_time::second_clock::universal_time();
    _hdr->vold()->setVolumeDateTime(now);
    _hdr->vold()->setGenerationDate(now.date());

    // Build the ByteBlock containing the DORADE representation of our header
    _hdrBlock.length(_hdr->size());
    
    std::ostringstream os;
    os.rdbuf()->pubsetbuf((char*)_hdrBlock.get_buffer(), _hdrBlock.length());
    
    _hdr->streamTo(os, false);

    // Write the new header now (which will start a new file)
    _status = _archiverServant->sendBlock(_hdrBlock);
    if (_status->errorFlag) {
        std::cerr << _status->errorMessage << std::endl;
        _hdrValid = false;
        return false;
    }

    // OK, now mark our header as valid to re-enable data writing.
    _hdrValid = true;
    
    std::cout << "Successfully loaded header with checksum " << _hdrChecksum <<
        std::endl;
    
    return true;
}

// This is the method which writes rays when new data are available.
void
EldoraArchiver::notify() {
    ByteBlock block;
    // If we don't have a valid header, just return immediately.
    if (! _hdrValid)
        return;
    
    while (Products* pItem = getNextItem()) {
        EldoraDDS::Housekeeping* hskp = &(pItem->hskp);
        
        // Get the radar name from the housekeeping, trimming trailing spaces
        std::string hskpRadarName(hskp->radarName);
        hskpRadarName.erase(hskpRadarName.find_last_not_of(' ') + 1);
        
        // Build the RYIB, ASIB, and FRAD from the incoming housekeeping
        DoradeRYIB ryib(hskp->sweepNum, hskp->julianDay, hskp->hour, 
                hskp->minute, hskp->second, hskp->millisecond, hskp->azimuth,
                hskp->elevation, hskp->peakXmitPower, hskp->trueScanRate,
                hskp->rayStatus);
        DoradeASIB asib(hskp->longitude, hskp->latitude, hskp->altitudeMSL,
                hskp->altitudeAGL, hskp->groundSpeedEW, hskp->groundSpeedNS,
                hskp->vertVelocity, hskp->heading, hskp->roll, hskp->pitch,
                hskp->yaw, hskp->radarRotAngle, hskp->radarTiltAngle, 
                hskp->windEW, hskp->windNS, hskp->windVert, 
                hskp->headingChangeRate, hskp->pitchChangeRate);
        DoradeFRAD frad(hskp->dataSysStatus, hskpRadarName, 
                hskp->testPulsePower, hskp->testPulseStart, 
                hskp->testPulseWidth, hskp->testPulseFreq, hskp->testPulseAtten,
                hskp->testPulseFNum, hskp->noisePower, hskp->rayCount,
                hskp->firstRecGate, hskp->lastRecGate);

        // Find the correct radar in the header and get the parameter count, 
        // ncells, and needed data pointers
        DoradeRADD* radd = 0;
        int nParams = 0;
        int nCells = 0;
        std::vector<short*> dataPtrs;

        for (int r = 0; r < 2; r++) {
            // Trim trailing spaces from the header radar name before comparing
            std::string hdrRadarName = _hdr->radd(r)->getRadarName();
            hdrRadarName.erase(hdrRadarName.find_last_not_of(' ') + 1);
                        
            if (hdrRadarName == hskpRadarName) {
                radd = _hdr->radd(r);
                nParams = radd->getNParams();
                nCells = _hdr->cspd(r)->getTotalNCells();

                for (int p = 0; p < nParams; p++) {
                    // Trim trailing spaces from param names in the header, too
                    std::string pName = _hdr->parm(r, p)->getParamName();
                    pName.erase(pName.find_last_not_of(' ') + 1);
                    
                    if (pName == "VR")
                        dataPtrs.push_back(pItem->vr.get_buffer());
                    else if (pName == "VS")
                        dataPtrs.push_back(pItem->vs.get_buffer());
                    else if (pName == "VL")
                        dataPtrs.push_back(pItem->vl.get_buffer());
                    else if (pName == "DM")
                        dataPtrs.push_back(pItem->dm.get_buffer());
                    else if (pName == "DBZ")
                        dataPtrs.push_back(pItem->dbz.get_buffer());
                    else if (pName == "SW")
                        dataPtrs.push_back(pItem->sw.get_buffer());
                    else if (pName == "NCP")
                        dataPtrs.push_back(pItem->ncp.get_buffer());
                    else {
                        std::cerr << __FUNCTION__ << ":" << __LINE__ <<
                            ": Unknown parameter '" << pName << 
                            "' needed for " << radd->getRadarName() << 
                            std::endl;
                        exit(1);
                    }
                }
                break;
            }
        }
        if (! radd) {
            std::cerr << __FUNCTION__ << ":" << __LINE__ << 
                ": no match found for radar '" << hskpRadarName << "'" << 
                std::endl;
            exit(1);
        }

        // Allocate or reallocate our _dataBuf if it isn't big enough
        int nData = nCells * nParams;
        if (_dataBufLen < nData) {
            delete[] _dataBuf;
            _dataBuf = new short[nData];
            _dataBufLen = nData;
        }
        
        // Now stuff all of the data into _dataBuf in the correct order for
        // the external DORADE representation
        for (int cell = 0; cell < nCells; cell++) {
            for (int p = 0; p < nParams; p++)
                _dataBuf[cell * nParams + p] = dataPtrs[p][cell];
        }

        // Finally, put all of the data into the FRAD
        frad.putTwoByteData(_dataBuf, nData);

        // Stuff the data ray (RYIB, ASIB, and FRAD) into a ByteBlock
        block.length(ryib.getDescLen() + asib.getDescLen() + frad.getDescLen());
        
        std::ostringstream os;
        os.rdbuf()->pubsetbuf((char*)block.get_buffer(), block.length());
        
        ryib.streamTo(os, false);
        asib.streamTo(os, false);
        frad.streamTo(os, false);

        // Finally, let the archiver servant actually write the block!
        _status = _archiverServant->sendBlock(block);       
        if (_status->errorFlag) {
            std::cerr << _status->errorMessage << std::endl;
        } else {
            _raysWritten++;
        }
        
        returnItem(pItem);
    }
}


