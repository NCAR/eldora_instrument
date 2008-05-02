// most importantly, our hardware:
#include "RR314.h"
#include "Bittware.h"

#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ios>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>

#include <boost/program_options.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

// For configuration management
#include "QtConfig.h"
// Proxy argc/argv
#include "ArgvParams.h"
// DDS publisher interface
#include "DDSPublisher.h"
#include "DDSWriter.h"
// The types that go along with DDS
#include "RayTypeSupportC.h"
#include "RayTypeSupportImpl.h"
#include "TimeSeriesTypeSupportC.h"
#include "TimeSeriesTypeSupportImpl.h"
// The XML-RPC interface.
#include "DrxRPC.h"
// Housekeeping related
#include <DoradeASIB.h>
#include <DoradeFRAD.h>
#include <DoradeRYIB.h>

using namespace RedRapids;
using namespace boost::posix_time;
namespace po = boost::program_options;

// save pointers to RR314 instances, in case we need to try
// to stop them during a signal.
/// All of the RR314 instances that we are manipulating.
std::vector<RR314*> rr314Instances;

/// The Bittware timer
Bittware* bwtimer = 0;

/// Set this flag to true in order to request the main loop to
/// shut everything down.
bool _terminate = false;

//////////////////////////////////////////////////////////////////////
//
// Not all of these fields are filled by parseOptions()
/// The parameters that specify the configuration for each RR314 device.
struct runParams {
        // radar parameters
        bool enabled; ///< true if the dma transfers are to be started
        int gates; ///< number of gates
        int nci; ///< number of coherent integrations. Also known as "samples"
        int startiq; ///< the starting gate number of iq capture.
        int numiq; ///< the number of gates for iq capture.
        int pulsewidth; ///< the pulse width in ns. Must map to oe of the values in DDCregisters.h
        int prf; ///< the pulse repetition frequency, hz.
        
        // operating mode parameters
        bool capture; ///< set true if the data should be captured to files in binary mode.
        bool textcapture; ///< set true if the data should be captured to files in text mode.
        bool simulate; ///< set true to simulate an RR314 card, instead of accessing a real one.
        int usleep; ///< The usleep value for sleeps between frames
        
        // signal processing parameters
        std::string xsvf; ///< path to a bit file to be loaded into the RR314 fpga.
        std::string kaiser; ///< path to a file containing coefficients for the kaiser filter.
        std::string gaussian; ///< path to a file containing coefficients for the gaussian filter.
        bool internaltimer; ///< If true, use the rr314 builtin timer rather than an external
        
        // DDS parameters
        bool publish; ///< set ture if the rr314 data should be published to DDS.
        std::string ORB; ///< path to the ORB configuration file.
        std::string DCPS; ///< path to the DCPS configuration file.
        std::string rayTopic; ///< The published ray topic
        std::string productsTopic; /// The published product topic
        
        // RPC parameters
        int rpcPort; ///< the rpc port number
        
        // global objects
        int deviceNumber; ///< card number, starting at 0
        RR314* pRR314; ///< pointer to the instance of RR314 representing this card.
        std::ofstream* ofstreams[8]; ///< pointers to output streams for test or binary data capture.
        unsigned long sampleCounts[8]; ///< collect the number of DDS samples written for each dma channel.
        unsigned long* droppedTS; ///< the number of TS samples that could not be published.
        unsigned long* droppedRay; ///< the number of Ray samples that could not be published.
        DDSPublisher* publisher; ///< the DDS publisher.
        RayWriter* rayWriter; ///< the DDS ray writer.
        TSWriter* tsWriter; ///< the DDS TS writer.
};

//
// prototypes
//
static void getConfigParams(runParams&);
static void parseOptions(runParams&, int argc, char** argv, int deviceNumber);
static void shutdownBoards();
static void setupSignalHandler();
static void* rrDataTask(void* threadArg);
static void* hskpReadTask(void* threadArg);
static void setupSignalHandler();
static void createFiles(runParams& params);
static void setupSignalHandler();
static EldoraDDS::Housekeeping* unpackHousekeeping(const unsigned char* buf, 
        int buflen);
static void showStats(runParams& params, RR314& rr314, 
        unsigned long& droppedRay, unsigned long& droppedTS, int loopCount);

//////////////////////////////////////////////////////////////////////
//
// The main routine creates the cards, starts them, and then just
// monitors the card activity.
int main(int argc,
         char** argv) {
	// get the parameters specified in the configuration. These
    // may be overriden by command line options in the call to parseOptions.
    runParams configParams;
    getConfigParams(configParams);
    
    // parse command line options. Initialize with values from
    // the configuration, and then allow them to be overriden
    // from the command line.
    runParams params0 = configParams;
    runParams params1 = configParams;

    parseOptions(params0, argc, argv, 0);
    parseOptions(params1, argc, argv, 1);

    // create timer
    if (!params0.simulate && !params0.internaltimer) {
        bwtimer = new Bittware(0);
        bwtimer->configure(params0.gates,
                params0.prf,
                params0.pulsewidth,
                params0.nci,
                false);
        if (!bwtimer->isok()) {
            std::cerr << "Unable to create bittware timer\n";
            bwtimer->shutdown();
            exit(1);
        }
    }

    if (params0.publish) {
        std::cout <<__FILE__ << " creating DDS services\n";

        ArgvParams argv("rr314dual");
        argv["-ORBSvcConf"] = params0.ORB.c_str();
        argv["-DCPSConfigFile"] = params0.DCPS.c_str();

        // create the publisher
        params0.publisher = new DDSPublisher(argv.argc(), argv.argv());
        params1.publisher = params0.publisher;

        // create the ray writer
        params0.rayWriter = new RayWriter(*params0.publisher, "EldoraRays");
        params1.rayWriter = params0.rayWriter;

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
                params0.internaltimer,
                params0.startiq,
                params0.numiq,
                params0.gaussian,
                params0.kaiser,
                params0.xsvf,
                params0.simulate,
                params0.usleep,
                false // do not catch signals in RR314; we will do that ourselves.
        );

        RR314 rr314_1(params1.deviceNumber,
                params1.gates,
                params1.prf,
                params1.pulsewidth,
                params1.nci,
                0, // dual prt (0 or 1)
                params1.internaltimer,
                params1.startiq,
                params1.numiq,
                params1.gaussian,
                params1.kaiser,
                params1.xsvf,
                params1.simulate,
                params1.usleep,
                false // do not catch signals in RR314; we will do that ourselves
        );

        // save instances of RR314 for the shutdown handler
        rr314Instances.push_back(&rr314_0);
        rr314Instances.push_back(&rr314_1);

        // pass rr314 instance to data reading thread.
        params0.pRR314 = &rr314_0;
        params1.pRR314 = &rr314_1;

        // also tell it where to put dropped dds counters
        unsigned long droppedRay[2];
        unsigned long droppedTS[2];

        params0.droppedRay = &droppedRay[0];
        params0.droppedRay = &droppedRay[1];

        params1.droppedTS = &droppedTS[0];
        params1.droppedTS = &droppedTS[1];

        // create the data reading threads
        pthread_t dataThread0;
        pthread_t dataThread1;
        if (params0.enabled) {
            pthread_create(&dataThread0, NULL, rrDataTask, (void*) &params0);
        }
        if (params1.enabled) {
            pthread_create(&dataThread1, NULL, rrDataTask, (void*) &params1);
        }

        // and the housekeeping reader thread
        pthread_t hskpThread;
        if (params0.enabled || params1.enabled) {
            pthread_create(&hskpThread, NULL, hskpReadTask, NULL);
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

        // start the timer, if we are using it.
        if (bwtimer) {
            // 
            // Send the timer start command ~0.2 seconds after the
            // top of a second.  Xmit pulses start at the top of the
            // next second after the start command is sent, so staying
            // away from the *immediate* vicinity of the top of a second
            // means we can determine the transmit start time with 
            // certainty (as long as our clock is reasonably accurate),
            // and we can still allow plenty of time for the timer 
            // card actually get things started before the top of the
            // next second.
            //
            int wake_uSec = 200000; // 0.2 seconds
            ptime now(microsec_clock::universal_time());
            // sleep until the next 0.2 second mark
            int usecNow = now.time_of_day().total_microseconds() % 1000000;
            int sleep_uSec = (1000000 + wake_uSec - usecNow) % 1000000;
            // xmit starts at the top of the next second after we wake
            ptime xmitStartTime = now + 
            microseconds(1000000 - wake_uSec + sleep_uSec);
            // Now sleep, and then start the timer card when we wake
            usleep(sleep_uSec);
            bwtimer->start();
            // Tell the RedRapids cards what time xmit pulses start
            rr314_0.setXmitStartTime(xmitStartTime);
            rr314_1.setXmitStartTime(xmitStartTime);
        }
        int loopCount = 0;

        // create our RPC handler
        DrxRPC rpcHandler(params0.rpcPort, rr314_0, rr314_1);

        // periodically display the card activity.
        while(1)
        {
            showStats(params0, rr314_0, droppedRay[0], droppedTS[0], loopCount);
            showStats(params1, rr314_1, droppedRay[1], droppedTS[1], loopCount);
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
    if (bwtimer) {
        delete bwtimer;
    }
}

//////////////////////////////////////////////////////////////////////
///
/// get parameters that are spcified in the configuration file.
/// These can be overriden by command line specifications.
static  void 
getConfigParams(runParams &params) {

    QtConfig config("NCAR", "EldoraDrx");
    
    // set up the default configuration directory path
    std::string EldoraDir("/conf/");
    char* e = getenv("ELDORADIR");
    if (e) {
        EldoraDir = e + EldoraDir;
    } else {
        std::cerr << "Environment varable ELDORADIR must be set.\n";
        exit(1);        
    }

    // and create the default DDS configuration file paths, since these
    // depend upon ELDORADIR
    std::string orbFile = EldoraDir + "ORBSvc.conf";
    std::string dcpsFile = EldoraDir + "DDSClient.ini";
    std::string rayTopic;
    std::string productsTopic;
    
    // radar parameters
    params.gates         = config.getInt   ("Radar/Gates",       500); 
    params.nci           = config.getInt   ("Radar/Samples",     25); 
    params.startiq       = config.getInt   ("Radar/StartIQ",     100); 
    params.numiq         = config.getInt   ("Radar/NumIQGates",  5); 
    params.pulsewidth    = config.getInt   ("Radar/PulseWidthNs",1000); 
    params.prf           = config.getInt   ("Radar/PrfHz",       1000);
    
    // operating mode parameters
    params.enabled       = config.getBool  ("Mode/Enabled",       true); 
    params.simulate      = config.getBool  ("Mode/Simulate",      false); 
    params.usleep        = config.getInt   ("Mode/Usleep",        9000);
    params.capture       = config.getBool  ("Mode/BinaryCapture", false); 
    params.textcapture   = config.getBool  ("Mode/TextCapture",   false);
    params.internaltimer = config.getBool  ("Mode/InernalTimer",  false);
    
    // RR314 signal processing parameters
    params.xsvf          = config.getString("DSP/XsvfFile",        "./"); 
    params.kaiser        = config.getString("DSP/KaiserFile",      "./"); 
    params.gaussian      = config.getString("DSP/GaussianFile",    "./"); 
    
    // DDS parameters
    params.publish       = config.getBool  ("DDS/Publish",         true); 
    params.ORB           = config.getString("DDS/ORBConfigFile",   orbFile);
    params.DCPS          = config.getString("DDS/DCPSConfigFile",  dcpsFile);
    rayTopic             = config.getString("DDS/TopicRay",        "EldoraRays");
    productsTopic        = config.getString("DDS/TopicProducts",   "EldoraProducts");
    
    // RPC parameters
    params.rpcPort       = config.getInt   ("Rpc/RpcPort",         6000);
}
//////////////////////////////////////////////////////////////////////
//
/// Parse the command line options, and also set some options 
/// that are not specified on the command line.
/// @return The runtime options that can be passed to the
/// threads that interact with the RR314.
static void
parseOptions(runParams& params, int argc, char** argv, int deviceNumber) {

    // set the device number
    params.deviceNumber = deviceNumber;

    // get the option34
    po::options_description descripts("Options");
    descripts.add_options() ("help", "describe options") 
    ("ORB", po::value<std::string>(&params.ORB), "ORB service configuration file (Corba ORBSvcConf arg)")
    ("DCPS", po::value<std::string>(&params.DCPS), "DCPS configuration file (OpenDDS DCPSConfigFile arg)")
    ("simulate", "run in simulation mode")
    ("usleep", po::value<int>(&params.usleep)->default_value(50000),"usleep value for simulation")
    ("start0", "start RR314 device 0")
    ("start1", "start RR314 device 1")
    ("gates",po::value<int>(&params.gates)->default_value(500), "number of gates")
    ("nci", po::value<int>(&params.nci)->default_value(25), "number of coherent integrations")
    ("startiq", po::value<int>(&params.startiq)->default_value(100), "start gate for iq capture")
    ("numiq", po::value<int>(&params.numiq)->default_value(5),"number of gates for iq capture")
    ("pulsewidth", po::value<int>(&params.pulsewidth)->default_value(500),"pulse width, nS (250, 500, 750, 1000, 1250, 1500, 1750, 2000)")
    ("prf", po::value<int>(&params.prf)->default_value(2500), "pulse repetition frequency")
    ("internaltimer",  "use RR314 internal timer")
    ("xsvf", po::value<std::string>(&params.xsvf)->default_value(""), "path to xsvf file")
    ("kaiser", po::value<std::string>(&params.kaiser)->default_value(""),"path to kaiser coefficient file")
    ("gaussian",po::value<std::string>(&params.gaussian)->default_value(""),"path to gaussian coefficient file")
    ("binary", "binary capture")
    ("text", "text capture") 
    ("publish", "publish data")
    ("rpcport", po::value<int>(&params.rpcPort)->default_value(60000), "RPC port number");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, descripts), vm);
    po::notify(vm);

    params.publish = vm.count("publish") != 0;
    params.capture = (vm.count("binary") != 0) || (vm.count("text") != 0);
    params.textcapture = vm.count("text") != 0;
    params.simulate = vm.count("simulate") != 0;
    params.internaltimer = vm.count("internaltimer") != 0;

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

}

//////////////////////////////////////////////////////////////////////
//
/// Create data capture files. The files will have the 
/// name "data<devNum>-<dmaChannel>.
/// @params runParams The configuration options for 
/// one RR314 device.
static void createFiles(runParams& params) {
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
/// Terminate the data transfers from the RR314. This MUST be called
/// if RR314 instances are not going to be normally destroyed, e.g. in the
/// case of a signal leading to premature program termination, in order
/// to safely terminate the RR314 DMA transfers. Failure to do so will
/// lead to a Linux system crash as the RR314 DMA writes into random
/// memory.
static void shutdownBoards() {

    std::vector<RR314*>::iterator p;
    for (p = rr314Instances.begin(); p != rr314Instances.end(); p++) {
        std::cout << "stopping RR314 device " << (*p)->boardNumber() << std::endl;
        (*p)->RR314shutdown();
    }
    
    if (bwtimer) {
    	std::cout << "stopping Bittware device\n";
        bwtimer->shutdown();
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
static void signalHandler(int signo) {

    std::cout << "caught signal " << signo << "\n";

    // try to terminate the RR314 & timer boards
    shutdownBoards();
    
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
static void* rrDataTask(void* threadArg) {
    runParams* pParams = (runParams*) threadArg;
    RR314* pRR314 = pParams->pRR314;
    int gates = pParams->gates;
    int numiq = pParams->numiq;
    bool publish = pParams->publish;
    bool capture = pParams->capture;
    bool textcapture = pParams->textcapture;
    RayWriter* rayWriter = pParams->rayWriter;
    TSWriter* tsWriter = pParams->tsWriter;

    float elevation[4] = 
        { 0.0,
                0.0,
                0.0,
                0.0 };
    // the aft radar is 180 degrees different from the forward.
    if (pParams->deviceNumber != 0) {
        for (int i = 0; i < 4; i++)
            elevation[i] = 180.0;
    }

    ACE_Time_Value small(0, 100000);

    std::cout <<__FILE__ << " gates:" << gates << " iqpairs:" << numiq << " nci:" << pParams->nci << "\n";

    // loop while waiting for new buffers
    while (1) {
        // next buffer will block until a new buffer is ready.
        RRBuffer* pBuf = pRR314->nextBuffer();
        //	std::cout << (pBuf->type == RRBuffer::IQtype ? "IQ  ":"ABP ") <<
        //	  pBuf->dmaChan << " " << pBuf->chanId << " " << pBuf->rayNum << "\n";
        // get the details of this buffer
        int channel = pBuf->dmaChan;
        // bump the sample count
        pParams->sampleCounts[channel]++;

        // Are we publishing?
        if (publish) {
            // send the buffer to the appropriate writer
            if (pBuf->type == RRBuffer::ABPtype) {
                // an abp dmaChan
                EldoraDDS::Ray* pRay = rayWriter->getEmptyItem();
                if (pRay) {
                    // device 0 is the aft radar, 1 is fore
                    pRay->radarId =
                    (pParams->deviceNumber == 0) ?
                    EldoraDDS::Forward : EldoraDDS::Aft;

                    RRABPBuffer* pABP = dynamic_cast<RRABPBuffer*>(pBuf);
                    // set the size
                    pRay->abp.length(pABP->_abp.size());

                    // simulate the elevation
                    elevation[channel/2] += 0.90;
                    if (elevation[channel/2] >= 360.0)
                    elevation[channel/2] -= 360.0;
                    pRay->hskp.elevation = elevation[channel/2];

                    pRay->hskp.samples = pABP->nci;
                    pRay->hskp.chan = pABP->chanId;
                    for (unsigned int p = 0; p < pABP->_abp.size(); p++) {
                        float data = pABP->_abp[p];
                        pRay->abp[p] = data;
                    }
                    // set the ray number
                    pRay->hskp.rayNum = pABP->rayNum;

                    // send the ray to the ray publisher
                    rayWriter->publishItem(pRay);
                } else {
                    pParams->droppedRay++;
                    //std::cout << "can't get publisher ray\n";
                }
            } else {
                // a time series dmaChan
                EldoraDDS::TimeSeries* pTS = tsWriter->getEmptyItem();
                if (pTS) {
                    RRIQBuffer* pIQ = dynamic_cast<RRIQBuffer*>(pBuf);
                    // set the size
                    pTS->tsdata.length(pIQ->_iq.size());
                    pTS->hskp.samples = pIQ->nci;
                    pTS->hskp.chan = pIQ->chanId;
                    pTS->hskp.startIQ = pParams->startiq;
                    pTS->hskp.numIQ = pParams->numiq;
                    for (unsigned int p = 0; p < pIQ->_iq.size(); p++) {
                        pTS->tsdata[p] = pIQ->_iq[p];
                    }
                    // set the timestamp
                    pTS->hskp.rayNum = pIQ->rayNum;
                    // device 0 is the aft radar, 1 is fore
                    pTS->radarId =
                    (pParams->deviceNumber == 0) ?
                    EldoraDDS::Forward : EldoraDDS::Aft;
                    // send the ray to the ray publisher
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

        pBuf->returnBuffer();
    }
}

//////////////////////////////////////////////////////////////////////
// a task to read data from the housekeeper

static void* hskpReadTask(void* threadArg) {
    // listen address for incoming data (port 2222, any sender)
    int portNum = 2222;
    struct sockaddr_in inAddr;
    memset(&inAddr, 0, sizeof(inAddr));
    inAddr.sin_family = AF_INET;
    inAddr.sin_port = htons(portNum);
    inAddr.sin_addr.s_addr = INADDR_ANY;

    // Create the UDP socket
    int inSocket;
    int on = 1;

    if ((inSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ||
            setsockopt(inSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0 ||
            bind(inSocket, (struct sockaddr*)&inAddr, sizeof(inAddr)) < 0)
    {
        if (inSocket >= 0)
            close(inSocket);
        perror("creating incoming UDP socket");
        return NULL;
    }

    // Now just deal with incoming packets
    unsigned char buf[4096];

    // Just read incoming packets from the housekeeper and unpack them.
    while (1)
    {
        int nread;
        if ((nread = recv(inSocket, buf, sizeof(buf) - 1, 0)) < 0)
        {
            perror("reading from housekeeping socket");
            continue;
        }
        EldoraDDS::Housekeeping* hskp = unpackHousekeeping(buf, nread);
    }
    
    close(inSocket);
    return NULL;
}

//////////////////////////////////////////////////////////////////////
/// Return a new EldoraDDS::Housekeeping built from the given buf.
/// The returned struct should be deleted by the caller.
/// On error, NULL is returned.
static EldoraDDS::Housekeeping*
unpackHousekeeping(const unsigned char* buf, int buflen) {
    const unsigned char* data = buf;
    int datalen = buflen;
    // data from the housekeeper currently comes 176 bytes per ray.
    if (buflen != 176) {
        std::cerr << __FUNCTION__ << ": Got " << buflen << 
            "bytes instead of the expected 176" << std::endl;
        return 0;
    }
    // the structure of what comes in is effectively DORADE:
    // one RYIB descriptor, one ASIB descriptor, and one (dataless)
    // FRAD descriptor
    DoradeRYIB *ryib = 0;
    DoradeASIB *asib = 0;
    DoradeFRAD *frad = 0;

    try {
        ryib = new DoradeRYIB(data, datalen, false);
        data += ryib->getDescLen();
        datalen -= ryib->getDescLen();
        
        asib = new DoradeASIB(data, datalen, false);
        data += asib->getDescLen();
        datalen -= asib->getDescLen();
        
        frad = new DoradeFRAD(data, datalen, false);
    } catch (DescriptorException dex) {
        std::cerr << std::string("Descriptor exception: ") + dex.what() << 
            std::endl;
        return 0;
    }
    
    // Get a new EldoraDDS::Housekeeping
    EldoraDDS::Housekeeping* hskp = new EldoraDDS::Housekeeping();
    
    // timetag is microseconds since 1970/1/1 00:00:00 UTC
    time_duration tDiff = ryib->getRayDateTime() - 
        ptime(boost::gregorian::date(1970, 1, 1), time_duration(0, 0, 0));
    long diff_secs = tDiff.total_seconds();
    long diff_usecs = (tDiff.fractional_seconds() * 1000000) / 
        time_duration::ticks_per_second();
    hskp->timetag = (long long)(diff_secs * 1000000) + diff_usecs;
    
    // Now fill the rest of the Housekeeping
    // RYIB contents
    hskp->sweepNum = ryib->getSweepNumber();
    hskp->julianDay = ryib->getDayOfYear();
    hskp->hour = ryib->getHour();
    hskp->minute = ryib->getMinute();
    hskp->second = ryib->getSecond();
    hskp->millisecond = ryib->getMillisecond();
    hskp->azimuth = ryib->getAzimuth();
    hskp->elevation = ryib->getElevation();
    hskp->peakXmitPower = ryib->getPeakXmitPower();
    hskp->trueScanRate = ryib->getTrueScanRate();
    hskp->rayStatus = ryib->getRayStatus();
    // ASIB contents
    hskp->longitude = asib->getLongitude();
    hskp->latitude = asib->getLatitude();
    hskp->altitudeMSL = asib->getAltitudeMSL();
    hskp->altitudeAGL = asib->getAltitudeAGL();
    hskp->groundSpeedEW = asib->getGroundSpeedEW();
    hskp->groundSpeedNS = asib->getGroundSpeedNS();
    hskp->vertVelocity = asib->getVerticalVelocity();
    hskp->heading = asib->getHeading();
    hskp->roll = asib->getRoll();
    hskp->pitch = asib->getPitch();
    hskp->yaw = asib->getYaw();
    hskp->radarRotAngle = asib->getAntennaScanAngle();
    hskp->radarTiltAngle = asib->getAntennaTiltAngle();
    hskp->windEW = asib->getUWind();
    hskp->windNS = asib->getVWind();
    hskp->windVert = asib->getWWind();
    hskp->headingChangeRate = asib->getHeadingChangeRate();
    hskp->pitchChangeRate = asib->getPitchChangeRate();
    // FRAD contents
    hskp->dataSysStatus = frad->getDataSystemStatus();
    strncpy(hskp->radarName, frad->getRadarName().c_str(), 8);
    hskp->testPulsePower = frad->getTestPulsePower();
    hskp->testPulseStart = frad->getTestPulseStart();
    hskp->testPulseWidth = frad->getTestPulseWidth();
    hskp->testPulseFreq = frad->getTestPulseFreq();
    hskp->testPulseAtten = frad->getTestPulseAttenuation();
    hskp->testPulseFNum = frad->getTestPulseFNum();
    hskp->noisePower = frad->getNoisePower();
    hskp->rayCount = frad->getRayCount();
    hskp->firstRecGate = frad->getFirstGate();
    hskp->lastRecGate = frad->getLastGate();

    return hskp;
}

//////////////////////////////////////////////////////////////////////

static void showStats(runParams& params,
               RR314& rr314,
               unsigned long& droppedRay,
               unsigned long& droppedTS,
               int loopCount) {
    // get the current temperature
    double temperature = rr314.temperature();

    // get the current byte count for each dmaChan
    // from rr314. This call causes the byte counters
    // in r314 to be reset to zero.
    //std::vector<unsigned long> bytes = rr314.bytes();

    std::cout << "Device:" << params.deviceNumber << "  loop:" << loopCount++
            << "\n ";
    std::cout << std::setw(8);
    std::cout << std::setprecision(2);
    //std::cout << "bytes processed ";
    // Print the number of free buffers in the rr314 buffer
    // pool. If 0, rr314 is being overrun
    //unsigned long sum = 0;
    //for (unsigned int c = 0; c < bytes.size(); c++) {
    //    std::cout << std::setw(6);
    //    std::cout << bytes[c] << " ";
    //    sum += bytes[c];
    //}
    //std::cout << sum << " ";
    //std::cout << sum/10.0e6 << "MB/s";
    //std::cout << "\n";

    std::cout << "samples      ";
    for (int i = 0; i < 8; i++) {
        std::cout << std::setw(8) << params.sampleCounts[i];
        params.sampleCounts[i] = 0;
    }
    std::cout << "\n";
    std::cout << "free IQ:" << rr314.numFreeIQBuffers() << "   free ABP:"
            << rr314.numFreeABPBuffers() << "   dropped rays:"
            << droppedRay << "   dropped TS:" << droppedTS
            << std::setprecision(4) << "   t:" << temperature << "C" << "\n";
    droppedRay = 0;
    droppedTS = 0;
    
    std::cout.flush();
}
