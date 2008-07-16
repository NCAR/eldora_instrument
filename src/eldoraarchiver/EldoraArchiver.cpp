#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sys/types.h>
#include <unistd.h>

#include <archiver/ArchiverListener.h>
#include <archiver/Resolver.h>
#include <archiver/EldoraWriter.h>
#include <tao/ORB_Core.h>

#include <DoradeASIB.h>
#include <DoradeFRAD.h>
#include <DoradeRYIB.h>

#include <ArgvParams.h>

#include <boost/date_time/posix_time/ptime.hpp>

#include "SvnVersion.h"
#include "QtConfig.h"

#include "DoradeHeader.h"

#include "EldoraArchiver.h"
#include <dds/DCPS/Service_Participant.h>    // for TheServiceParticipant

using std::string;

using namespace archiver;

typedef NameResolver<PortableServer::POA, InitResolver> POAResolver;

// Pointer to our singleton instance
EldoraArchiver* EldoraArchiver::_theArchiver = 0;

EldoraArchiver::EldoraArchiver(DDSSubscriber& subscriber, 
        std::string topicName, std::string hdrFileName, std::string dataDir) : 
ProductsReader(subscriber, topicName), 
_hdr(), 
_dataBuf(), 
_dataBufLen(0), 
_raysWritten(0) {
    std::cout << "Using header file: " << hdrFileName << std::endl;
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

    // Get our header
    boost::posix_time::ptime now = 
        boost::posix_time::second_clock::universal_time();
    try {
        _hdr = new DoradeHeader(hdrFileName);
        _hdr->vold()->setVolumeDateTime(now);
        _hdr->vold()->setGenerationDate(now.date());
    } catch (DoradeHeader::BadHeaderException ex) {
        std::cerr << "Header exception: " << ex << std::endl;
        abort();
    }

    // Build the ByteBlock containing the DORADE representation of our header
    _hdrBlock.length(_hdr->size());
    
    std::ostringstream os;
    os.rdbuf()->pubsetbuf((char*)_hdrBlock.get_buffer(), _hdrBlock.length());
    
    _hdr->streamTo(os, false);

    // Write the header to our file
    _status = _archiverServant->sendBlock(_hdrBlock);
    
    // OK, initialization is complete. We keep track of this because
    // our notify() method can get called before this constructor is
    // done, and it can't run until at least the _hdr is built.
    _initialized = true;
}

EldoraArchiver::~EldoraArchiver() {
    _archiverServant->quit();
    _theArchiver = 0;
}

// This is the method which gets called when new data are available.
void
EldoraArchiver::notify() {
    ByteBlock block;
    // If this get called before the constructor is done with its work, just 
    // return immediately.
    if (! _initialized)
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
        _raysWritten++;
        
        returnItem(pItem);
    }
}


int
main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <header_file_name> <data_dir>" << std::endl;
        std::cerr << "(The header file name should not include the path." << std::endl;
        std::cerr << "the path will be built automatically.)" << std::endl;
        exit(1);
    }

    std::cout << "My PID is " << getpid() << std::endl;

    // Configuration

    // get the configuration
    QtConfig eaConfig("NCAR", "EldoraArchiver");

    // set up the configuration directory path: $ELDORADIR/conf/
    char* e = getenv("ELDORADIR");
    if (! e) {
        // If ELDORADIR is not set, use a default
        e = "/opt/eldora";
        std::cerr << "Environment variable ELDORADIR is not set.  Using " <<
            e << "." << std::endl;
    }
    std::string eldoraConfigDir(e);
    eldoraConfigDir += "/conf/";
    
    std::string orbConfigFile = 
        eaConfig.getString("DDS/ORBConfigFile", eldoraConfigDir + "ORBSvc.conf");   
    std::string dcpsConfigFile = 
        eaConfig.getString("DDS/DCPSConfigFile", eldoraConfigDir + "DDSClient.ini");
    std::string dcpsInfoRepo = 
        eaConfig.getString("DDS/DCPSInfoRepo", "iiop://archiver:50000/DCPSInfoRepo");  
    std::string productsTopic = 
        eaConfig.getString("TopicProducts", "EldoraProducts");
    
    // create the subscriber
    ArgvParams subParams(argv[0]);
    subParams["-ORBSvcConf"] = orbConfigFile;
    subParams["-DCPSConfigFile"] = dcpsConfigFile;
    subParams["-DCPSInfoRepo"] = dcpsInfoRepo;

    DDSSubscriber subscriber(subParams.argc(), subParams.argv());
    int subStatus = subscriber.status();
    if (subStatus) {
        std::cerr << "Error " << subStatus << " creating subscriber" << std::endl;
        exit(subStatus);
    }

    std::string hdrFileName = eldoraConfigDir + argv[1];
    std::string dataDir(argv[2]);
    EldoraArchiver* theArchiver = 
        EldoraArchiver::TheArchiver(subscriber, productsTopic, hdrFileName, 
        		dataDir);
    
    int prevRaysWritten = theArchiver->raysWritten();
    while (1) {
        int sleepTime = 10;
        sleep(sleepTime);

        int raysWritten = theArchiver->raysWritten();
        int diff = raysWritten - prevRaysWritten;
        prevRaysWritten = raysWritten;
        
        std::cout << boost::posix_time::second_clock::universal_time() <<
            ": wrote " << diff << " rays in the last " << sleepTime <<
            " seconds" << std::endl;
    }

}
