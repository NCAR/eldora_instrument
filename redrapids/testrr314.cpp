#include "RR314.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ios>
#include "boost/program_options.hpp"
#include "ArgvParams.h"

#include "DDSPublisher.h"
#include "DDSWriter.h"

#include "PulseTypeSupportC.h"
#include "PulseTypeSupportImpl.h"
#include "TimeSeriesTypeSupportC.h"
#include "TimeSeriesTypeSupportImpl.h"

using namespace RedRapids;
using namespace EldoraDDS;
namespace po = boost::program_options;

//////////////////////////////////////////////////////////////////////
//

struct runParams {
        int device;
        int gates;
        int nci;
        int startiq;
        int numiq;
        int decimation;
        bool simulate;
        std::string xsvf;
        std::string kaiser;
        std::string gaussian;
        RR314* pRR314;
        std::ofstream* ofstreams[8];
        unsigned long sampleCounts[8];
        bool publish;
        bool capture;
};

//////////////////////////////////////////////////////////////////////
//

struct runParams parseOptions(int argc, char** argv) {

    runParams params;
    // get the options
    po::options_description descripts("Options");
    descripts.add_options() ("help", "describe options") ("simulate",
            "run in simulation mode") ("device",
            po::value<int>(&params.device)->default_value(0),
            "device number") ("gates",
            po::value<int>(&params.gates)->default_value(501),
            "number of gates") ("nci",
            po::value<int>(&params.nci)->default_value(50),
            "number of coherent integrations") ("startiq",
            po::value<int>(&params.startiq)->default_value(0),
            "start gate for iq capture") ("numiq",
            po::value<int>(&params.numiq)->default_value(11),
            "number of gates for iq capture") ("decimation",
            po::value<int>(&params.decimation)->default_value(6),
            "decimation factor") ("xsvf",
            po::value<std::string>(&params.xsvf)->default_value(""),
            "path to xsvf file") ("kaiser",
            po::value<std::string>(&params.kaiser)->default_value(""),
            "path to kaiser coefficient file") ("gaussian",
            po::value<std::string>(&params.gaussian)->default_value(""),
            "path to gaussian coefficient file") ("capture",
            "capture data to files") ("publish",
            "publish data");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, descripts),
            vm);
    po::notify(vm);

    params.publish = vm.count("publish") != 0;
    params.capture = vm.count("capture") != 0;
    params.simulate = vm.count("simulate") != 0;

    if (vm.count("help")) {
        std::cout << descripts << "\n";
        exit(1);
    }

    return params;
}

//////////////////////////////////////////////////////////////////////
//
void createFiles(runParams& params) {
    for (int i = 0; i < 8; i++) {
        std::stringstream s;
        s << "data";
        s.width(2);
        s.fill('0');
        s << i;
        params.ofstreams[i] = new std::ofstream(s.str().c_str(), std::ios::trunc);

    }

}

//////////////////////////////////////////////////////////////////////
//
// a task which just consumes the data from the
// rr314 as it becomes available
void * dataTask(void* threadArg) {
    runParams* pParams = (runParams*) threadArg;
    RR314* pRR314 = pParams->pRR314;
    int gates = pParams->gates;
    int numiq = pParams->numiq;
    bool publish = pParams->publish;
    bool capture = pParams->capture;

    ArgvParams argv("testrr314");
    argv["-ORBSvcConf"] = ELDORA_ROOT "/conf/tcp.conf";
    argv["-DCPSConfigFile"] = ELDORA_ROOT "/conf/simpleConf.ini";

    ACE_Time_Value small(0, 100000);

    DDSPublisher* publisher;
    PulseWriter* pulseWriter;
    TSWriter* tsWriter;

    std::cout <<__FILE__ << " gates:" << gates << " iqpairs:" << numiq << " nci:" << pParams->nci << "\n";

    if (publish) {

        std::cout <<__FILE__ << " creating DDS services\n";
        // create the publisher
        publisher = new DDSPublisher(argv.argc(), argv.argv());

        // create the pulse writer
        pulseWriter = new PulseWriter(*publisher, "EldoraPulses");

        // create the time series writer
        tsWriter = new TSWriter(*publisher, "EldoraTS");
    }

    // loop while waiting for new buffers
    while (1) {
        // next buffer will block until a new buffer is ready.
        RRBuffer* pBuf = pRR314->nextBuffer();
//	std::cout << (pBuf->type == RRBuffer::IQtype ? "IQ  ":"ABP ") <<
//	  pBuf->dmaChan << " " << pBuf->chanId << " " << pBuf->pulseCount << "\n";
        // get the details of this buffer
        int channel = pBuf->dmaChan;
        // bump the sample count
        pParams->sampleCounts[channel]++;

        // Are we publishing?
        if (publish) {
            // send the buffer to the appropriate writer
            if (pBuf->type == RRBuffer::ABPtype) {
                // an abp dmaChan
                EldoraDDS::Pulse* pPulse = pulseWriter->getEmptyItem();
                if (pPulse) {
                    RRABPBuffer* pABP = dynamic_cast<RRABPBuffer*>(pBuf);
                    // set the size
                    pPulse->abp.length(pABP->_abp.size());
                    for (unsigned int p = 0; p < pABP->_abp.size(); p++) {
                        pPulse->abp[p] = pABP->_abp[p];
                    }
                    // set the timestamp
                    pPulse->timestamp = pABP->pulseCount;
                    // alternate the radar id between forward and aft
                    pPulse->radarId = EldoraDDS::Aft;
                    // send the pulse to the pulde publisher
                    pulseWriter->publishItem(pPulse);
                } else {
                    //std::cout << "can't get publisher pulse\n";
                    ACE_OS::sleep(small);
                }
            } else {
                // a time series dmaChan
                EldoraDDS::TimeSeries* pTS = tsWriter->getEmptyItem();
                if (pTS) {
                    RRIQBuffer* pIQ = dynamic_cast<RRIQBuffer*>(pBuf);
                    // set the size
                    pTS->tsdata.length(pIQ->_iq.size());
                    for (unsigned int p = 0; p < pIQ->_iq.size(); p++)
                    pTS->tsdata[p] = pIQ->_iq[p];
                    // set the timestamp
                    pTS->timestamp = pIQ->pulseCount;
                    // alternate the radar id between forward and aft
                    pTS->radarId = EldoraDDS::Aft;
                    // send the pulse to the pulde publisher
                    tsWriter->publishItem(pTS);
                } else {
                    //std::cout << "can't get publisher pulse\n";
                    ACE_OS::sleep(small);
                }
            }
        }

        // Are we capturing?
        if (capture) {
            // get the file  that this dmaChan is saved to.
            std::ofstream* pStream = pParams->ofstreams[channel];
            if (pBuf->type == RRBuffer::ABPtype) {
                // ABP buffer
                RRABPBuffer* pABP = dynamic_cast<RRABPBuffer*>(pBuf);
                assert (pABP != 0);
                for (unsigned int i = 0; i < pABP->_abp.size(); i += 3) {
                    *pStream
                    << pABP->_abp[i] << " "
                    << pABP->_abp[i+1] << " "
                    << pABP->_abp[i+2] << " "
                    << std::endl;
                }
            } else {
                // IQ buffer
                RRIQBuffer* pIQ = dynamic_cast<RRIQBuffer*>(pBuf);
                assert(pIQ != 0);
                for (unsigned int i = 0; i < pIQ->_iq.size(); i += 2) {
                    *pStream
                    << pIQ->_iq[i] << " "
                    << pIQ->_iq[i+1] << " "
                    << std::endl;
                }
            }
        }

        pRR314->returnBuffer(pBuf);
    }
}

//////////////////////////////////////////////////////////////////////
//
// The main routine creates the card, starts it, and then just
// monitors the card activity.
int main(int argc, char** argv) {

    // parse command line options
    runParams params = parseOptions(argc, argv);

    // create data capture files
    if (params.capture) {
        createFiles(params);
    }

    for (int i = 0; i < 8; i++) {
        params.sampleCounts[i] = 0;
    }
    // create an RR314 card
    try
    {
        RR314 rr314(params.device,
                params.gates,
                params.nci,
                0, // dual prt (0 or 1)
                params.startiq,
                params.numiq,
                params.decimation,
                params.xsvf,
                params.kaiser,
                params.gaussian,
                params.simulate // simulation mode?
        );

        // pass rr324 instance to data reading thread.
        params.pRR314 = &rr314;

        // create the data reading thread
        pthread_t dataThread;
        pthread_create(&dataThread, NULL, dataTask, (void*) &params);

        // start the processing
        rr314.start();

        // periodically display the card activity.
        while(1)
        {
            // get the current byte count for each dmaChan
            // from rr314. This call causes the byte counters
            // in r314 to be reset to zero.
            std::vector<unsigned long> bytes = rr314.bytes();

            std::cout << std::setw(8);
            std::cout << std::setprecision(2);
            // Print the number of free buffers in the rr314 buffer
            // pool. If 0, rr314 is being overrun
            std::cout << rr314.numFreeIQBuffers() << " "
            << rr314.numFreeABPBuffers() << " ";
            unsigned long sum = 0;
            for (unsigned int c = 0; c < bytes.size(); c++)
            {
                std::cout << std::setw(6);
                std::cout << bytes[c] << " ";
                sum += bytes[c];
            }
            std::cout << sum << " ";
            std::cout << sum/10.0e6 << "MB/s";
            std::cout << "\n";

            std::cout << "samples ";
            for (int i = 0; i < 8; i++) {
                std::cout << std::setw(8)
                << params.sampleCounts[i];
                params.sampleCounts[i] = 0;
            }
            std::cout << "\n";

            sleep(10);
        }
    }
    catch (std::string e)
    {
        std::cout << e << std::endl;
        exit(1);
    }

}

