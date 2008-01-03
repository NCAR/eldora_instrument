#include "RR314.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ios>
#include <vector>

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

// save pointers to RR314 instances, in case we need to try
// to stop them during a signal.
/// All of the RR314 instances that we are manipulating.
std::vector<RR314*> rr314Instances;

/// Set this flag to true in order to request the main loop to
/// shut everything down.
bool _terminate = false;

//////////////////////////////////////////////////////////////////////
//
// Not all of these fields are filled by parseOptions()
/// The parameters that specify the configuration for each 
/// RR314 device.
struct runParams {
        int deviceNumber; ///< card number, starting at 0
        bool enabled; ///< true if the dma transfers are to be started
        int gates; ///< number of gates
        int nci; ///< number of coherent integrations. Also known as "samples"
        int startiq; ///< the starting gate number of iq capture.
        int numiq; ///< the number of gates for iq capture.
        int pulsewidth; ///< the pulse width in ns. Must map to oe of the values in DDCregisters.h
        int prf; ///< the pulse repetition frequency, hz.
        bool simulate; ///< set true to simulate an RR314 card, instead of accessing a real one.
        std::string xsvf; ///< path to a bit file to be loaded into the RR314 fpga.
        std::string kaiser; ///< path to a file containing coefficients for the kaiser filter.
        std::string gaussian; ///< path to a file containing coefficients for the gaussian filter.
        RR314* pRR314; ///< pointer to the instance of RR314 representing this card.
        std::ofstream* ofstreams[8]; ///< pointers to output streams for test or binary data capture.
        unsigned long sampleCounts[8]; ///< collect the number of DDS samples written for each dma channel.
        bool publish; ///< set ture if the rr314 data should be published to DDS.
        bool capture; ///< set true if the data should be captured to files in binary mode.
        bool textcapture; ///< set true if the data should be captured to files in text mode.
        std::string ORB; ///< path to the ORB configuration file.
        std::string DCPS; ///< path to the DCPS configuration file.
        unsigned long* droppedTS; ///< the number of TS samples that could not be published.
        unsigned long* droppedPulse; ///< the number of Pulse samples that could not be published.
        DDSPublisher* publisher; ///< the DDS publisher.
        PulseWriter* pulseWriter; ///< the DDS pulse writer.
        TSWriter* tsWriter; ///< the DDS TS writer.
};

//////////////////////////////////////////////////////////////////////
//
/// Parse the command line options, and also set some options 
/// that are not specified on the command line.
/// @return The runtime options that can be passed to the
/// threads that interact with the RR314.
struct runParams parseOptions(
        int argc,
            char** argv,
            int deviceNumber) {

    runParams params;

    // set the device number
    params.deviceNumber = deviceNumber;

    int pulseWidth;
    
    // get the option34
    po::options_description descripts("Options");
    descripts.add_options() ("help", "describe options") ("ORB", po::value<std::string>(&params.ORB), "ORB service configuration file (Corba ORBSvcConf arg)")
    ("DCPS", po::value<std::string>(&params.DCPS), "DCPS configuration file (OpenDDS DCPSConfigFile arg)")
    ("simulate", "run in simulation mode")
    ("start0","start RR314 device 0")
    ("start1", "start RR314 device 1")
    ("gates",po::value<int>(&params.gates)->default_value(500), "number of gates")
    ("nci", po::value<int>(&params.nci)->default_value(25), "number of coherent integrations")
    ("startiq", po::value<int>(&params.startiq)->default_value(100), "start gate for iq capture")
    ("numiq", po::value<int>(&params.numiq)->default_value(5),"number of gates for iq capture")
    ("pulsewidth", po::value<int>(&params.pulsewidth)->default_value(500), 
            "pulse width, nS (250, 500, 750, 1000, 1250, 1500, 1750, 2000)")
    ("prf", po::value<int>(&params.prf)->default_value(3400), "pulse repetition frequency")
    ("xsvf", po::value<std::string>(&params.xsvf)->default_value(""), "path to xsvf file")
    ("kaiser", po::value<std::string>(&params.kaiser)->default_value(""),"path to kaiser coefficient file")
    ("gaussian",po::value<std::string>(&params.gaussian)->default_value(""),"path to gaussian coefficient file")
    ("binary", "binary capture")
    ("text", "text capture") ("publish", "publish data");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, descripts), vm);
    po::notify(vm);

    params.publish = vm.count("publish") != 0;
    params.capture = (vm.count("binary") != 0) || (vm.count("text") != 0);
    params.textcapture = vm.count("text") != 0;
    params.simulate = vm.count("simulate") != 0;

    if (vm.count("help") || (vm.count("binary") && vm.count("text"))) {
        std::cout << "Initialize two rr314 cards. If --start0 ond/or --start1 is specified,\n";
        std::cout << "that card will be started for data collection.\n";
        std::cout << descripts << "\n";
        exit(1);
    }
    
    // verify that the decimation is one of the accepted values.
    switch(params.pulsewidth) {
        case 250:
        case 500:
        case 750:
        case 1000:
        case 1250:
        case 1500:
        case 1750:
        case 2000:
        break;
        default:
            std::cout << "pulse width must be one of: 250, 500, 750, 1000, 1250, 1500, 1750, 2000\n";
            exit(1);
        break;
    }

    // see if this device was selected to be active
    params.enabled = false;
    if ((deviceNumber == 0) && vm.count("start0"))
    params.enabled = true;
    if ((deviceNumber == 1) && vm.count("start1"))
    params.enabled = true;

    return params;
}

//////////////////////////////////////////////////////////////////////
//
/// Create data capture files. The files will have the 
/// name "data<devNum>-<dmaChannel>.
/// @params runParams The configuration options for 
/// one RR314 device.
static void createFiles(
        runParams& params) {
    for (int i = 0; i < 8; i++) {
        std::stringstream s;
        s << "data";
        s.width(2);
        s.fill('0');
        s << params.deviceNumber;
        s << "-";
        s << i;
        params.ofstreams[i] = new std::ofstream(s.str().c_str(), std::ios::trunc);

    }

}

//////////////////////////////////////////////////////////////////////
//
/// terminate the data transfers from the RR314. This MUST be called
/// if RR314 instances are not going to be normally destroyed, e.g. in the
/// case of a signal leading to premature program termination, in order
/// to safely terminate the RR314 DMA transfers. Failure to do so will
/// lead to a Linux system crash as the RR314 DMA writes into random
/// memory.
static void shutdownRR314() {

    std::vector<RR314*>::iterator p;
    for (p = rr314Instances.begin(); p != rr314Instances.end(); p++) {
        std::cout << "stopping RR314 device " << (*p)->boardNumber() << std::endl;
        (*p)->RR314shutdown();
    }
}

//////////////////////////////////////////////////////////////////////
//
/// Signal handler. If it is a benign signal, set the terminate
/// flag and return. The polling loop will see the flag,
/// and terminate with proper object cleanup. 
/// For catastrophic signals, such as segv,
/// perform critical cleanup such as stopping the RR314 cards.
///
static void signalHandler(
        int signo) {

    std::cout << "caught signal " << signo << "\n";
    
    // try to terminate the RR314 boards
    shutdownRR314();
    
    // if it was a segv, produce a core dump
    if (signo == SIGSEGV) {
        // and abort with a dump.
        abort();
    }

    // set the terminate flag
    _terminate = true;

}

//////////////////////////////////////////////////////////////////////
//
/// Arrange to catch signals. signalHandler() function
/// will be specifiec to called when signals are caught.
static void setupSignalHandler() {

    // install signal handler for abort signals
    struct sigaction new_action, old_action;

    /* Set up the structure to specify the new action. */
    new_action.sa_handler = signalHandler;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;

    sigaction(SIGINT, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction(SIGINT, &new_action, NULL);
    sigaction(SIGHUP, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction(SIGHUP, &new_action, NULL);
    sigaction(SIGTERM, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction(SIGTERM, &new_action, NULL);
    if (old_action.sa_handler != SIG_IGN)
        sigaction(SIGSEGV, &new_action, NULL);

}

//////////////////////////////////////////////////////////////////////
//
// a task which just consumes the data from the
// rr314 as it becomes available
static void * dataTask(
        void* threadArg) {
    runParams* pParams = (runParams*) threadArg;
    RR314* pRR314 = pParams->pRR314;
    int gates = pParams->gates;
    int numiq = pParams->numiq;
    bool publish = pParams->publish;
    bool capture = pParams->capture;
    bool textcapture = pParams->textcapture;
    PulseWriter* pulseWriter = pParams->pulseWriter;
    TSWriter* tsWriter = pParams->tsWriter;

    ACE_Time_Value small(0, 100000);

    std::cout <<__FILE__ << " gates:" << gates << " iqpairs:" << numiq << " nci:" << pParams->nci << "\n";

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
                    pPulse->nci = pABP->nci;
                    pPulse->chan = pABP->chanId;
                    for (unsigned int p = 0; p < pABP->_abp.size(); p++) {
                        float data = pABP->_abp[p];
                        pPulse->abp[p] = data;
                    }
                    // set the timestamp
                    pPulse->timestamp = pABP->pulseCount;
                    // device 0 is the aft radar, 1 is fore
                    pPulse->radarId =
                    (pParams->deviceNumber == 0) ?
                    EldoraDDS::Forward : EldoraDDS::Aft;

                    // send the pulse to the pulse publisher
                    pulseWriter->publishItem(pPulse);
                } else {
                    pParams->droppedPulse++;
                    //std::cout << "can't get publisher pulse\n";
                }
            } else {
                // a time series dmaChan
                EldoraDDS::TimeSeries* pTS = tsWriter->getEmptyItem();
                if (pTS) {
                    RRIQBuffer* pIQ = dynamic_cast<RRIQBuffer*>(pBuf);
                    // set the size
                    pTS->tsdata.length(pIQ->_iq.size());
                    pTS->nci = pIQ->nci;
                    pTS->chan = pIQ->chanId;
                    // std::cout << "channel " << (int)pTS->chan << "\n";
                    for (unsigned int p = 0; p < pIQ->_iq.size(); p++) {
                        pTS->tsdata[p] = pIQ->_iq[p];
                    }
                    // set the timestamp
                    pTS->timestamp = pIQ->pulseCount;
                    // device 0 is the aft radar, 1 is fore
                    pTS->radarId =
                    (pParams->deviceNumber == 0) ?
                    EldoraDDS::Forward : EldoraDDS::Aft;
                    // send the pulse to the pulde publisher
                    tsWriter->publishItem(pTS);
                } else {
                    pParams->droppedTS++;
                    //std::cout << "can't get publisher TS\n";
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
                if (textcapture) {
                    for (unsigned int i = 0; i < pABP->_abp.size(); i += 3) {
                        *pStream << pABP->_abp[i] << " "
                        << pABP->_abp[i+1] << " "
                        << pABP->_abp[i+2] << " "
                        << std::endl;
                    }
                } else {
                    pStream->write((char*)(&pABP->_abp[0]), pABP->_abp.size()*sizeof(pABP->_abp[0]));
                }
            } else {
                // IQ buffer
                RRIQBuffer* pIQ = dynamic_cast<RRIQBuffer*>(pBuf);
                assert(pIQ != 0);
                if (textcapture) {
                    for (unsigned int i = 0; i < pIQ->_iq.size(); i += 2) {
                        *pStream << pIQ->_iq[i] << " " << pIQ->_iq[i+1] << " "<< std::endl;
                    }
                } else {
                    pStream->write((char*)(&pIQ->_iq[0]), pIQ->_iq.size()*sizeof(pIQ->_iq[0]));
                }
            }
        }

        pRR314->returnBuffer(pBuf);
    }
}

//////////////////////////////////////////////////////////////////////

void showStats(
        runParams& params,
            RR314& rr314,
            unsigned long& droppedPulse,
            unsigned long& droppedTS,
            int loopCount) {
    // get the current temperature
    double temperature = rr314.temperature();

    // get the current byte count for each dmaChan
    // from rr314. This call causes the byte counters
    // in r314 to be reset to zero.
    std::vector<unsigned long> bytes = rr314.bytes();

    std::cout << "Device:" << params.deviceNumber << "  loop:" << loopCount++
            << "\n ";
    std::cout << std::setw(8);
    std::cout << std::setprecision(2);
    std::cout << "bytes processed ";
    // Print the number of free buffers in the rr314 buffer
    // pool. If 0, rr314 is being overrun
    unsigned long sum = 0;
    for (unsigned int c = 0; c < bytes.size(); c++) {
        std::cout << std::setw(6);
        std::cout << bytes[c] << " ";
        sum += bytes[c];
    }
    std::cout << sum << " ";
    std::cout << sum/10.0e6 << "MB/s";
    std::cout << "\n";

    std::cout << "samples      ";
    for (int i = 0; i < 8; i++) {
        std::cout << std::setw(8) << params.sampleCounts[i];
        params.sampleCounts[i] = 0;
    }
    std::cout << "\n";
    std::cout << "free IQ:" << rr314.numFreeIQBuffers() << "   free ABP:"
            << rr314.numFreeABPBuffers() << "   dropped pulses:"
            << droppedPulse << "   dropped TS:" << droppedTS
            << std::setprecision(4) << "   t:" << temperature << "C" << "\n\n";
    droppedPulse = 0;
    droppedTS = 0;
}

//////////////////////////////////////////////////////////////////////
//
// The main routine creates the card, starts it, and then just
// monitors the card activity.
int main(
        int argc,
            char** argv) {

    // parse command line options
    runParams params0 = parseOptions(argc, argv, 0);
    runParams params1 = parseOptions(argc, argv, 1);

    if (params0.publish) {
        std::cout <<__FILE__ << " creating DDS services\n";

        ArgvParams argv("rr314dual");
        argv["-ORBSvcConf"] = params0.ORB.c_str();
        argv["-DCPSConfigFile"] = params0.DCPS.c_str();

        // create the publisher
                params0.publisher = new DDSPublisher(argv.argc(), argv.argv());
                params1.publisher = params0.publisher;

                // create the pulse writer
                params0.pulseWriter = new PulseWriter(*params0.publisher, "EldoraPulses");
                params1.pulseWriter = params0.pulseWriter;

                // create the time series writer
                params0.tsWriter = new TSWriter(*params0.publisher, "EldoraTS");
                params1.tsWriter = params0.tsWriter;
            }

            // create data capture files
            if (params0.capture) {
                createFiles(params0);
                createFiles(params1);
            }

            for (int i = 0; i < 8; i++) {
                params0.sampleCounts[i] = 0;
                params1.sampleCounts[i] = 0;
            }
            // create an RR314 card
            try {
                RR314 rr314_0(params0.deviceNumber,
                        params0.gates,
                        params0.prf,
                        params0.pulsewidth,
                        params0.nci,
                        0, // dual prt (0 or 1)
                        params0.startiq,
                        params0.numiq,
                        params0.gaussian,
                        params0.kaiser,
                        params0.xsvf,
                        params0.simulate,
                        false              // do not catch signals in RR314; we will do that ourselves.
                );


                RR314 rr314_1(params1.deviceNumber,
                        params1.gates,
                        params1.prf,
                        params1.pulsewidth,
                        params1.nci,
                        0, // dual prt (0 or 1)
                        params1.startiq,
                        params1.numiq,
                        params1.gaussian,
                        params1.kaiser,
                        params1.xsvf,
                        params1.simulate,
                        false              // do not catch signals in RR314; we will do that ourselves
                );

                // save instances of RR314 for the shutdown handler
                rr314Instances.push_back(&rr314_0);
                rr314Instances.push_back(&rr314_1);

                // pass rr324 instance to data reading thread.
                params0.pRR314 = &rr314_0;
                params1.pRR314 = &rr314_1;

                // also tell it where to put dropped dds counters
                unsigned long droppedPulse[2];
                unsigned long droppedTS[2];

                params0.droppedPulse = &droppedPulse[0];
                params0.droppedPulse = &droppedPulse[1];

                params1.droppedTS = &droppedTS[0];
                params1.droppedTS = &droppedTS[1];

                // create the data reading threads
                pthread_t dataThread0;
                pthread_t dataThread1;
                if (params0.enabled) {
                    pthread_create(&dataThread0, NULL, dataTask, (void*) &params0);
                }
                if (params1.enabled) {
                    pthread_create(&dataThread1, NULL, dataTask, (void*) &params1);
                }

                // setup the signal handlers before we run the cards.
                setupSignalHandler();
                
                // start the processing
                if (params0.enabled) {
                    rr314_0.start();
                }
                if (params1.enabled) {
                    rr314_1.start();
                }

                int loopCount = 0;
                // periodically display the card activity.
                while(1)
                {
                    showStats(params0, rr314_0, droppedPulse[0], droppedTS[0], loopCount);
                    showStats(params1, rr314_1, droppedPulse[1], droppedTS[1], loopCount);
                    loopCount++;
                    for (int i = 0; i < 10; i++) {
                        if (_terminate)
                            break;
                        sleep(1);
                    }
                    if (_terminate)
                        break;
                }
            }
            catch (std::string e)
            {
                std::cout << e << std::endl;
                exit(1);
            }
            
            std::cout << "Terminating " << argv[0] << "\n";

        }

