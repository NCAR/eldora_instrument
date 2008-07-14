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
        std::string topicName) : ProductsReader(subscriber, topicName),
        _dataBufLen(0) {
    // Create the archiver servant
    CORBA::ORB_ptr raddArchiverOrb = TheServiceParticipant->get_ORB();
    _servantImpl = new ArchiverService_impl("archiver", raddArchiverOrb, 
            new archiver::EldoraWriter(), /*standalone*/ true);
    if (! _servantImpl) {
        std::cerr << __FUNCTION__ << ":" << __LINE__ << 
            ": Failed to create ArchiverService_impl";
        exit(1);
    }
    ArchiverService_var servant = _servantImpl->_this();

    ArchiverStatus_var status;

//    // Activate the POA
//    PortableServer::POA_var poa = POAResolver::resolve(raddArchiverOrb, "RootPOA");
//    PortableServer::POAManager_var mgr = poa->the_POAManager();
//    mgr->activate();

    ArchiverConfig config = archiver::defaultConfig();
    config.fileName = "eldora";
    config.fileSizeLimit = 100 * 1024 * 1024; // 100 MB
    config.directoryName = "/tmp/EldoraArchiver";
    status = servant->reconfig(config);

    // Get our header
    boost::posix_time::ptime now = 
        boost::posix_time::second_clock::universal_time();
    try {
        _hdr = new DoradeHeader("/opt/eldora/conf/tparc_conv.hd");
        _hdr->vold()->setVolumeDateTime(now);
        _hdr->vold()->setGenerationDate(now.date());
    } catch (DoradeHeader::BadHeaderException ex) {
        std::cerr << "Header exception: " << ex << std::endl;
        abort();
    }

    // Build the ByteBlock containing the DORADE representation of our header
    std::ostringstream os;
    _hdrBlock.length(_hdr->size());
    os.rdbuf()->pubsetbuf((char*)_hdrBlock.get_buffer(), _hdrBlock.length());
    _hdr->streamTo(os, false);
    
    status = servant->start();
    status = servant->sendBlock(_hdrBlock);
//
//
//    string filename = status->currentFilename.in();
//    unsigned long total =(_hdrBlock.length() + block.length() + 2 * 8);
//
//    // Keep writing blocks until the file rolls over.
//    while(total <=(unsigned long)config.fileSizeLimit)
//    {
//        total += block.length() + 8;
//        status = servant->sendBlock(block);
//    }
//    // Header is appended to the end of the file.
//    total += hdrBlock.length() + 8;
//    std::cout << "total for " << filename << " should be " << total << std::endl;
//
//    // Need to write one more to actually start the next file.
//    total = hdrBlock.length() + 8;
//    total += block.length() + 8;
//    status = servant->sendBlock(block);
//    
//    // Tell the servant to quit
//    servant->quit();

//    // A final header should have been appended to the file
//    total += hdrBlock.length() + 8;
//    std::cout << "total for " << status->currentFilename.in() << 
//        " should be " << total << std::endl;    
}

EldoraArchiver::~EldoraArchiver() {
    _theArchiver = 0;
}

// This is the method which gets called when new data are available.
void
EldoraArchiver::notify() {
    std::cout << "in notify()" << std::endl;
    ByteBlock block;
    
    while (Products* pItem = getNextItem()) {
        EldoraDDS::Housekeeping* hskp = &(pItem->hskp);
        
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
        DoradeFRAD frad(hskp->dataSysStatus, hskp->radarName, 
                hskp->testPulsePower, hskp->testPulseStart, 
                hskp->testPulseWidth, hskp->testPulseFreq, hskp->testPulseAtten,
                hskp->testPulseFNum, hskp->noisePower, hskp->rayCount,
                hskp->firstRecGate, hskp->lastRecGate);

//        // Get the product list, parameter count, and ncells for this radar
//        DoradeRADD* radd = 0;
//        int nParams = 0;
//        int nCells = 0;
//        std::vector<EldoraDDS::Product> productList;
//
//        for (int r = 0; r < 2; r++) {
////            std::cout << "header " << _hdr << std::endl;
////            std::cout << "radar " << r << " " << _hdr->radd(r) << std::endl;
////            std::cout << _hdr->radd(r)->getRadarName() << std::endl;
////            // Trim trailing spaces from the radar names in the header
////            // before comparing
////            std::string hdrRadarName = _hdr->radd(r)->getRadarName();
////            hdrRadarName.erase(hdrRadarName.find_last_not_of(' ') + 1);
//            
////            if (hdrRadarName == hskp->radarName) {
//            if (1) {
//                radd = _hdr->radd(r);
//                nParams = radd->getNParams();
//                nCells = _hdr->cspd(r)->getTotalNCells();
//
//                for (int p = 0; p < nParams; p++) {
//                    // Trim trailing spaces from param names in the header, too
////                    std::string pName = _hdr->parm(r, p)->getParamName();
////                    pName.erase(pName.find_last_not_of(' ') + 1);
//                    std::string pName = "DBZ";
//                    
//                    if (pName == "VR")
//                        productList[p] = pItem->vr;
//                    else if (pName == "VS")
//                        productList[p] = pItem->vs;
//                    else if (pName == "VL")
//                        productList[p] = pItem->vl;
//                    else if (pName == "DM")
//                        productList[p] = pItem->dm;
//                    else if (pName == "DBZ")
//                        productList[p] = pItem->dbz;
//                    else if (pName == "SW")
//                        productList[p] = pItem->sw;
//                    else if (pName == "NCP")
//                        productList[p] = pItem->ncp;
//                    else {
//                        std::cerr << __FUNCTION__ << ":" << __LINE__ <<
//                            ": Unknown parameter '" << pName << 
//                            "' needed for " << radd->getRadarName() << 
//                            std::endl;
//                        exit(1);
//                    }
//                }
//                break;
//            }
//        }
//        if (! radd) {
//            std::cerr << __FUNCTION__ << ":" << __LINE__ << 
//                ": no match found for radar '" << hskp->radarName << "'" << 
//                std::endl;
//            exit(1);
//        }
        int nCells = 416;
        int nParams = 4;
//        std::vector<EldoraDDS::Product> productList;
//        productList[0] = pItem->sw;
//        productList[1] = pItem->vr;
//        productList[2] = pItem->ncp;
//        productList[3] = pItem->dbz;
            
        std::cout << "writing " << nCells << " cells and " << nParams <<
            " params" << std::endl;
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
            for (int p = 0; p < nParams; p++) {
//                _dataBuf[cell * nParams + p] = productList[p][cell];
                _dataBuf[cell * nParams + p] = cell;
            }
        }

        // Finally, put all of the data into the FRAD
        frad.putTwoByteData(_dataBuf, nData);

        // output stringstream
        std::ostringstream os;

        // Stuff the data ray into a ByteBlock
        block.length(ryib.getDescLen() + asib.getDescLen() + frad.getDescLen());
        os.rdbuf()->pubsetbuf((char*)block.get_buffer(), block.length());
        ryib.streamTo(os, false);
        asib.streamTo(os, false);
        frad.streamTo(os, false);


        ArchiverStatus_var status;
        
        status = _servantImpl->_this()->sendBlock(block);
        std::cout << "wrote a ray\n";
        //    std::cout << "servant status: \n" << *status.ptr() << std::endl;
    }
}

//void
//EldoraArchiver::productSlot(std::vector<double> p, int radarId, float elDegrees,
//        int prodType, float gateSizeMeters, double dwellWidth, 
//        double airspdCorr) {
//    std::cout << "Got a product for " << radarId << std::endl;
//    // Data ray
//    boost::posix_time::ptime now = 
//        boost::posix_time::second_clock::universal_time();
//    DoradeRYIB ryib(now, 1, 0.0, 0.0, 500.0, 30.0, 0);
//    DoradeASIB asib(-105.0, 40.0, 2.6, 1.0, 50.0, 50.0, 2.0, 0.0, 0.0, 1.1, 
//            0.0, 180.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
//    DoradeFRAD frad(0, (radarId == 0) ? "FORE" : "AFT", 40.0, 20.0, 0.3, 3.2e9, 5, 0, -110.0, 0, 0, 399);
//    short shortData[2400];
//    frad.putTwoByteData(shortData, sizeof(shortData) / sizeof(*shortData));
//
//    // output stringstream
//    std::ostringstream os;
//
//    // Stuff the data ray into a ByteBlock
//    ByteBlock block;
//    block.length(ryib.getDescLen() + asib.getDescLen() + frad.getDescLen());
//    os.rdbuf()->pubsetbuf((char*)block.get_buffer(), block.length());
//    ryib.streamTo(os, false);
//    asib.streamTo(os, false);
//    frad.streamTo(os, false);
//    
//    ArchiverStatus_var status;
//    ArchiverService_var servant = _servantImpl->_this();
//    
//    status = servant->sendBlock(_hdrBlock);
//    status = servant->sendBlock(block);
//    std::cout << "servant status: \n" << *status.ptr() << std::endl;
//}
//
//int 
//EldoraArchiver::exec() {
//    // Get our header
//    boost::posix_time::ptime now = 
//        boost::posix_time::second_clock::universal_time();
//    try {
//        _hdr = new DoradeHeader("/opt/eldora/conf/tparc_conv.hd");
//        _hdr->vold()->setVolumeDateTime(now);
//        _hdr->vold()->setGenerationDate(now.date());
//    } catch (DoradeHeader::BadHeaderException ex) {
//        std::cerr << "Header exception: " << ex << std::endl;
//        abort();
//    }
//    
//    // Build the ByteBlock containing the DORADE representation of our header
//    std::ostringstream os;
//    _hdrBlock.length(_hdr->size());
//    os.rdbuf()->pubsetbuf((char*)_hdrBlock.get_buffer(), _hdrBlock.length());
//    _hdr->streamTo(os, false);
//
//    _servantImpl->_this()->start();
//    _foreSource->start();
//    _aftSource->start();
//    return QApplication::exec();
//}




int
main(int argc, char *argv[])
{
//    ACE_LOG_MSG->stop_tracing();
    if (argc != 1)
    {
        std::cerr << "Usage: " << string(argv[0]) << " [orb options]" << std::endl;
        exit(1);
    }

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

    EldoraArchiver* theArchiver = 
        EldoraArchiver::TheArchiver(subscriber, productsTopic);

    while (1) {
        usleep(200000); // 0.2 second
        std::cout << "*************** WAKE ***********" << std::endl;
    }

}
