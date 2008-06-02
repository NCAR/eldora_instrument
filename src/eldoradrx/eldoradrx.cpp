// most importantly, our hardware:
#include "RR314.h"
#include "Bittware.h"
#include "EldoraRadarParams.h"

#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ios>
#include <vector>
#include <set>

#include <boost/program_options.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

// For configuration management
#include <QtConfig.h>
// Proxy argc/argv
#include <ArgvParams.h>
// DDS publisher interface
#include <DDSPublisher.h>
#include <DDSWriter.h>
// The types that go along with DDS
#include <RayTypeSupportC.h>
#include <RayTypeSupportImpl.h>
#include <TimeSeriesTypeSupportC.h>
#include <TimeSeriesTypeSupportImpl.h>
// The XML-RPC interface.
#include <DrxRPC.h>
// Timetag handling
#include <TimetagUtils.h>
// Class for merging housekeeping into incomplete RRBuffer-s
#include "HskpMerger.h"
// Housekeeping related
#include <DoradeASIB.h>
#include <DoradeFRAD.h>
#include <DoradeRYIB.h>

// Socket stuff for sending fake housekeeping
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace RedRapids;
using namespace boost::posix_time;
namespace po = boost::program_options;

// save pointers to RR314 instances, in case we need to try
// to stop them during a signal.
/// All of the RR314 instances that we are manipulating.
std::vector<RR314*> _rr314Instances;

/// The Bittware timer
Bittware* _bwtimer = 0;

/// Set this flag to true in order to request the main loop to
/// shut everything down.
bool _terminate = false;

// drop counters
unsigned long _droppedRay[2] =
    {
            0,
            0 };
unsigned long _droppedTS[2] =
    {
            0,
            0 };

/// Our DDS writers
static RayWriter* _rayWriter = 0;
static TSWriter* _tsWriter = 0;

static const unsigned int DEFAULT_HSKP_PORT = 2222;
unsigned int _hskpPort;

//////////////////////////////////////////////////////////////////////
//
// Not all of these fields are filled by parseOptions()
/// The parameters that specify the configuration for each RR314 device.
struct runParams
{
        // operating parameters
        bool         enabled;              ///< true if the dma transfers are to be started
        
        // signal processing parameters
        std::string  xsvf;                 ///< path to a bit file to be loaded into the RR314 fpga.
        std::string  kaiser;               ///< path to a file containing coefficients for the kaiser filter.
        std::string  gaussian;             ///< path to a file containing coefficients for the gaussian filter.

        // operating mode parameters
        bool          capture;             ///< set true if the data should be captured to files in binary mode.
        bool          textcapture;         ///< set true if the data should be captured to files in text mode.
        bool          simulateRR314;       ///< set true to simulate an RR314 card, instead of accessing a real one.
        bool          simulateHskp;        ///< set true to generate fake housekeeping data
        int           usleep;              ///< The usleep value for sleeps between frames
        bool          internaltimer;       ///< If true, use the rr314 builtin timer rather than an external

        // DDS parameters
        bool           publish;            ///< set ture if the rr314 data should be published to DDS.
        std::string    ORB;                ///< path to the ORB configuration file.
        std::string    DCPS;               ///< path to the DCPS configuration file.
        std::string    rayTopic;           ///< The published ray topic
        std::string    productsTopic;      /// The published product topic

        // RPC parameters
        int            rpcPort;            ///< the rpc port number

        // global objects
        int            deviceNumber;       ///< card number, starting at 0
        RR314*         pRR314;             ///< pointer to the instance of RR314 representing this card.
        std::ofstream* ofstreams[8];       ///< pointers to output streams for test or binary data capture.
        unsigned long  sampleCounts[8];    ///< collect the number of DDS samples written for each dma channel.
        unsigned long* droppedTS;          ///< the number of TS samples that could not be published.
        unsigned long* droppedRays;        ///< the number of Ray samples that could not be published.
        HskpMerger*    hskpMerger;         ///< the object which merges housekeeping with RRBuffer-s
};

struct hskpThreadArg {
    HskpMerger*        hskpMerger;          ///< the object which merges housekeeping with RRBuffer-s    
    EldoraRadarParams* radarParams;         ///< radar parameters, one for FORE and one for AFT
};

struct rr314ThreadArg {
    EldoraRadarParams* radarParams;        ///< The radar operating paramters
    struct runParams*  drxParams;
};

//
// prototypes
//
static void getConfigParams(runParams&);
static void parseOptions(runParams&,
                         int argc,
                         char** argv,
                         bool& start0,
                         bool& start1);
static void shutdownBoards();
static void setupSignalHandler();
static void* rrDataTask(void* threadArg);
static void* hskpReadTask(void* threadArg);
static void setupSignalHandler();
static EldoraDDS::Housekeeping* newDDSHousekeeping(const unsigned char* buf,
                                                   int buflen);
static void showStats(runParams& drxParams,
                      RR314& rr314,
                      unsigned long& droppedRay,
                      unsigned long& droppedTS,
                      int loopCount);
static void publishAndCapture(RREntry* rentry,
                              Housekeeping* hskp);
static EldoraDDS::Housekeeping* newFakeDDSHousekeeping(ptime hskpTime,
                                                       bool isFore);
static void sendFakeHousekeeping(const EldoraDDS::Housekeeping* hskp);

//////////////////////////////////////////////////////////////////////
//
// The main routine creates the cards, starts them, and then just
// monitors the card activity.
int main(int argc,
         char** argv)
{
    runParams drxParams[2];
    EldoraRadarParams radarParams[2];
    
    // get the parameters specified in the configuration. These
    // may be overriden by command line options in the call to parseOptions.
    runParams configParams;
    getConfigParams(configParams);

    // parse command line options. Initialize with values from
    // the configuration, and then allow them to be overriden
    // from the command line.
    drxParams[0] = configParams;
    bool start0;
    bool start1;
    parseOptions(drxParams[0], argc, argv, start0, start1);
    
    // now clone the run params for the second radar
    drxParams[1] = drxParams[0];
    
    // And set radar specific fields
    // set the radar names
    drxParams[0].deviceNumber = 0;
    drxParams[1].deviceNumber = 1;
    for (int i = 0; i < 4; i++) {
        radarParams[0].radd_name[i] = "FORE"[i];
        radarParams[1].radd_name[i] = "AFT "[i];        
    }
    
    if (start0)
        drxParams[0].enabled = true;
    if (start1)
        drxParams[1].enabled = true;

    // create timer
    if (!drxParams[0].simulateRR314 && !drxParams[0].internaltimer) {
        _bwtimer = new Bittware(0);
        _bwtimer->configure(radarParams[0]);
        if (!_bwtimer->isok()) {
            std::cerr << "Unable to create bittware timer\n";
            _bwtimer->shutdown();
            exit(1);
        }
    }

    for (int i = 0; i < 8; i++) {
        drxParams[0].sampleCounts[i] = 0;
        drxParams[1].sampleCounts[i] = 0;
    }
    // create an RR314 card
    try {
        RR314 rr314_0(drxParams[0].deviceNumber,
                radarParams[0],
                drxParams[0].gaussian,
                drxParams[0].kaiser,
                drxParams[0].xsvf,
                drxParams[0].internaltimer,
                drxParams[0].simulateRR314,
                drxParams[0].usleep,
                false // do not catch signals in RR314; we will do that ourselves.
        );

        RR314 rr314_1(drxParams[1].deviceNumber,
                radarParams[1],
                drxParams[1].gaussian,
                drxParams[1].kaiser,
                drxParams[1].xsvf,
                drxParams[1].internaltimer,
                drxParams[1].simulateRR314,
                drxParams[1].usleep,
                false // do not catch signals in RR314; we will do that ourselves
        );

        // save instances of RR314 for the shutdown handler
        _rr314Instances.push_back(&rr314_0);
        _rr314Instances.push_back(&rr314_1);

        // pass rr314 instance to data reading thread.
        drxParams[0].pRR314 = &rr314_0;
        drxParams[1].pRR314 = &rr314_1;

        drxParams[0].droppedRays = &_droppedRay[0];
        drxParams[1].droppedRays = &_droppedRay[1];

        drxParams[0].droppedTS = &_droppedTS[0];
        drxParams[1].droppedTS = &_droppedTS[1];

        // our housekeeping mergers, with 500 ms max retention time waiting to 
        // make matches
        HskpMerger hskpMerger[2] = {
            HskpMerger(&rr314_0, 500, publishAndCapture),
            HskpMerger(&rr314_1, 500, publishAndCapture)
        };

        if (drxParams[0].publish) {
            std::cout <<__FILE__ << " creating DDS services\n";

            ArgvParams argv("eldoradrx");
            argv["-ORBSvcConf"] = drxParams[0].ORB.c_str();
            argv["-DCPSConfigFile"] = drxParams[0].DCPS.c_str();

            // create our DDS publisher
            DDSPublisher* publisher = new DDSPublisher(argv.argc(), argv.argv());
            if (publisher->status()) {
                std::cout << "Unable to create a publisher, exiting.\n";
                exit(1);
            }

            // create the ray writer
            _rayWriter = new RayWriter(*publisher, "EldoraRays");

            // create the time series writer
            _tsWriter = new TSWriter(*publisher, "EldoraTS");

            // housekeeping mergers
            drxParams[0].hskpMerger = &(hskpMerger[0]);
            drxParams[1].hskpMerger = &(hskpMerger[1]);
        }

        // create the data reading threads
        pthread_t dataThread0;
        rr314ThreadArg rrArg0 = { &radarParams[0], &drxParams[0] };
        if (drxParams[0].enabled) {
            pthread_create(&dataThread0, NULL, rrDataTask, (void*) &rrArg0);
        }

        pthread_t dataThread1;
        rr314ThreadArg rrArg1 = { &radarParams[1], &drxParams[1] };
        if (drxParams[1].enabled) {
            pthread_create(&dataThread1, NULL, rrDataTask, (void*) &rrArg1);
        }

        // note if we're simulating housekeeping
        if (drxParams[0].simulateHskp)
        std::cout << "Housekeeping will be simulated" << std::endl;

        // Shift the default port for simulated housekeeping off of the 
        // normal default, so that we can simulate even if the real housekeeper
        // is running.
        _hskpPort = (drxParams[0].simulateHskp) ?
        (DEFAULT_HSKP_PORT + 1) : DEFAULT_HSKP_PORT;

        // and the housekeeping reader thread
        hskpThreadArg hArg;
        hArg.hskpMerger  = hskpMerger;
        hArg.radarParams = radarParams;

        pthread_t hskpThread;
        pthread_create(&hskpThread, NULL, hskpReadTask, (void*)&hArg);

        // setup the signal handlers before we run the cards.
        setupSignalHandler();

        // start the processing
        if (drxParams[0].enabled) {
            rr314_0.start();
        }
        if (drxParams[1].enabled) {
            rr314_1.start();
        }

        // start the timer, if we are using it.
        if (_bwtimer) {
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
            _bwtimer->start();
            // Tell the RedRapids cards what time xmit pulses start
            rr314_0.setXmitStartTime(xmitStartTime);
            rr314_1.setXmitStartTime(xmitStartTime);
        } else {
            if (!drxParams[0].simulateRR314) {
                // start internal timing for both cards 0 & 1
                rr314_0.startInternalTimer();
                rr314_1.startInternalTimer();
            }
        }
        int loopCount = 0;

        // create our RPC handler
        DrxRPC rpcHandler(drxParams[0].rpcPort, rr314_0, rr314_1);

        // periodically display the card activity.
        while(1)
        {
            // stats for card 0
            showStats(drxParams[0], rr314_0, _droppedRay[0], _droppedTS[0], loopCount);
            // hskp merger info for 0
            hskpMerger[0].showStats(std::cout);
            // stats for card 1
            showStats(drxParams[1], rr314_1, _droppedRay[1], _droppedTS[1], loopCount);
            // hskp merger info for 1
            hskpMerger[1].showStats(std::cout);
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
    if (_bwtimer) {
        delete _bwtimer;
    }
}

//////////////////////////////////////////////////////////////////////
///
/// get parameters that are spcified in the configuration file.
/// These can be overriden by command line specifications.
static void getConfigParams(runParams &drxParams)
{

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

    // operating mode parameters
    drxParams.enabled = config.getBool("Mode/Enabled", true);
    drxParams.simulateRR314 = config.getBool("Mode/SimulateRR314", false);
    drxParams.simulateHskp = config.getBool("Mode/SimulateHskp", false);
    drxParams.usleep = config.getInt("Mode/Usleep", 9000);
    drxParams.capture = config.getBool("Mode/BinaryCapture", false);
    drxParams.textcapture = config.getBool("Mode/TextCapture", false);
    drxParams.internaltimer = config.getBool("Mode/InternalTimer", false);

    // RR314 signal processing parameters
    drxParams.xsvf = config.getString("DSP/XsvfFile", "");
    drxParams.kaiser = config.getString("DSP/KaiserFile", "");
    drxParams.gaussian = config.getString("DSP/GaussianFile", "");

    // DDS parameters
    drxParams.publish = config.getBool("DDS/Publish", true);
    drxParams.ORB = config.getString("DDS/ORBConfigFile", orbFile);
    drxParams.DCPS = config.getString("DDS/DCPSConfigFile", dcpsFile);
    rayTopic = config.getString("DDS/TopicRay", "EldoraRays");
    productsTopic = config.getString("DDS/TopicProducts", "EldoraProducts");

    // RPC parameters
    drxParams.rpcPort = config.getInt("Rpc/RpcPort", 60000);
}
//////////////////////////////////////////////////////////////////////
//
/// Parse the command line options, and also set some options 
/// that are not specified on the command line.
/// @return The runtime options that can be passed to the
/// threads that interact with the RR314.
static void parseOptions(runParams& drxParams,
                         int argc,
                         char** argv,
                         bool& start0,
                         bool& start1)
{

    // get the option34
    po::options_description descripts("Options");
    descripts.add_options() ("help", "describe options") ("ORB", po::value<std::string>(&drxParams.ORB), "ORB service configuration file (Corba ORBSvcConf arg)")
    ("DCPS", po::value<std::string>(&drxParams.DCPS), "DCPS configuration file (OpenDDS DCPSConfigFile arg)")
    ("simRR314", "run RR314 in simulation mode")
    ("simHskp", "generate fake housekeeping data")
    ("usleep", po::value<int>(&drxParams.usleep), "usleep value for simulation")
    ("start0", "start RR314 device 0")
    ("start1", "start RR314 device 1")
    ("internaltimer", "use RR314 internal timer")
    ("xsvf", po::value<std::string>(&drxParams.xsvf), "path to xsvf file")
    ("kaiser", po::value<std::string>(&drxParams.kaiser), "path to kaiser coefficient file")
    ("gaussian", po::value<std::string>(&drxParams.gaussian),"path to gaussian coefficient file")
    ("binary", "binary capture")
    ("text", "text capture")
    ("publish", "publish data")
    ("rpcport", po::value<int>(&drxParams.rpcPort), "RPC port number");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, descripts), vm);
    po::notify(vm);

    drxParams.publish = vm.count("publish") != 0;
    drxParams.capture = (vm.count("binary") != 0) || (vm.count("text") != 0);
    drxParams.textcapture = vm.count("text") != 0;
    drxParams.simulateRR314 = vm.count("simRR314") != 0;
    drxParams.simulateHskp = vm.count("simHskp") != 0;
    drxParams.internaltimer = vm.count("internaltimer") != 0;

    if (vm.count("help") || (vm.count("binary") && vm.count("text"))) {
        std::cout << "Initialize two rr314 cards. If --start0 ond/or --start1 is specified,\n";
        std::cout << "that card will be started for data collection.\n";
        std::cout << descripts << "\n";
        exit(1);
    }

    // see if this device was selected to be active
    if (vm.count("start0"))
        start0 = true;
    if (vm.count("start1"))
        start1 = true;

}

//////////////////////////////////////////////////////////////////////
//
/// Terminate the data transfers from the RR314. This MUST be called
/// if RR314 instances are not going to be normally destroyed, e.g. in the
/// case of a signal leading to premature program termination, in order
/// to safely terminate the RR314 DMA transfers. Failure to do so will
/// lead to a Linux system crash as the RR314 DMA writes into random
/// memory.
static void shutdownBoards()
{

    std::vector<RR314*>::iterator p;
    for (p = _rr314Instances.begin(); p != _rr314Instances.end(); p++) {
        std::cout << "stopping RR314 device " << (*p)->boardNumber() << std::endl;
        (*p)->RR314shutdown();
    }

    if (_bwtimer) {
        std::cout << "stopping Bittware device\n";
        _bwtimer->shutdown();
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
static void signalHandler(int signo)
{

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
// Add radar parameters into the housekeeping
//
//////////////////////////////////////////////////////////////////////
static void setRadarParams(EldoraRadarParams* radarParams, EldoraDDS::Housekeeping* hskp) {

    for (int i = 0; i < 4; i++)
        hskp->rxGain[i] = radarParams->frib_rxgain[i];
    
    hskp->freqs[0]      = radarParams->radd_freq1;
    hskp->freqs[1]      = radarParams->radd_freq2;
    hskp->freqs[2]      = radarParams->radd_freq3;
    hskp->freqs[3]      = radarParams->radd_freq4;

    hskp->xBandGain     = radarParams->frib_xgain;
    hskp->lnaLoss       = radarParams->frib_lnalos;
    hskp->noisePower    = radarParams->radd_noipow;
    hskp->radarConstant = radarParams->radd_const;
    hskp->prt           = radarParams->radd_ipp1;
    hskp->prtLong       = radarParams->radd_ipp2;
    
}
//////////////////////////////////////////////////////////////////////
//
/// Arrange to catch signals. signalHandler() function
/// will be specifiec to called when signals are caught.
static void setupSignalHandler()
{

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
static void* rrDataTask(void* threadArg)
{
    runParams* pParams = ((rr314ThreadArg*) threadArg)->drxParams;
    EldoraRadarParams* radarParams = ((rr314ThreadArg*) threadArg)->radarParams;
 
    RR314* pRR314 = pParams->pRR314;
    HskpMerger* hskpMerger = pParams->hskpMerger;
    
    int gates = radarParams->wave_ngates[0];
    int numiq = radarParams->frib_frqgat;
    int samples = radarParams->wave_ngates[0];
    
    bool capture = pParams->capture;
    bool textcapture = pParams->textcapture;
    bool simulateHskp = pParams->simulateHskp;
    
    // When generating fake housekeeping, keep a set of the last few 
    // housekeeping times we've sent, so that we don't send out duplicate 
    // housekeeping.  The history only needs to be long enough to handle data 
    // times that arrive out of order.
    std::set<ptime> sentFakeHskp;
    const unsigned int FAKE_HSKP_HISTORY_LEN = 50;

    std::cout <<__FILE__ << " gates:" << gates << " iqpairs:" << numiq << " samples:" << samples << "\n";

    // loop while waiting for new buffers
    while (1) {
        // nextBuffer() will block until a new buffer is ready.
        RRBuffer* pBuf = pRR314->nextBuffer();

        // get the details of this buffer
        int channel = pBuf->dmaChan;
        // bump the sample count
        pParams->sampleCounts[channel]++;

        hskpMerger->newRRBuffer(pBuf, pParams->publish, pParams->capture, pParams->textcapture);

        // If we're doing fake housekeeping, deal with it now.
        if (simulateHskp) {
            // If we don't have this time in our history, generate and send
            // some fake housekeeping.
            if (sentFakeHskp.find(pBuf->rayTime) == sentFakeHskp.end()) {
                // Generate and send the housekeeping
                EldoraDDS::Housekeeping* hskp =
                newFakeDDSHousekeeping(pBuf->rayTime,
                        (pParams->deviceNumber == 1));
                hskpMerger->newHskp(hskp);
                
                // add in some radar paremters needed by products generator
                setRadarParams(radarParams, hskp);
                
               //// @todo When sendFakeHousekeeping() works, we'll use that to 
                //// send via the UDP port instead of going directly to 
                //// hskpMerger->newHskp()
                //                sendFakeHousekeeping(hskp);
                //                delete hskp;

                // add this time to our history of delivered fake housekeeping
                if (sentFakeHskp.size() == FAKE_HSKP_HISTORY_LEN)
                sentFakeHskp.erase(sentFakeHskp.begin());
                sentFakeHskp.insert(pBuf->rayTime);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////
// a task to read data from the housekeeper

static void* hskpReadTask(void* threadArg)
{
    HskpMerger* hskpMerger = ((hskpThreadArg*)threadArg)->hskpMerger;
    EldoraRadarParams* radarParams = ((hskpThreadArg*)threadArg)->radarParams;

    struct sockaddr_in inAddr;
    memset(&inAddr, 0, sizeof(inAddr));
    inAddr.sin_family = AF_INET;
    inAddr.sin_port = htons(_hskpPort); // normally 2222
    inAddr.sin_addr.s_addr = INADDR_ANY;

    // Create the UDP socket
    int inSocket;
    int on = 1;

    if ((inSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0
            || setsockopt(inSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))
                    < 0 || bind(inSocket,
                                (struct sockaddr*)&inAddr,
                                sizeof(inAddr)) < 0) {
        if (inSocket >= 0)
            close(inSocket);
        perror("creating incoming UDP socket");
        return NULL;
    }

    // Now just deal with incoming packets
    unsigned char buf[4096];

    // Just read incoming packets from the housekeeper and unpack them.
    while (1) {
        int nread;
        if ((nread = recv(inSocket, buf, sizeof(buf) - 1, 0)) < 0) {
            perror("reading from housekeeping socket");
            continue;
        }
        EldoraDDS::Housekeeping* hskp = newDDSHousekeeping(buf, nread);
        if (hskp == 0) {
            std::cerr << __FUNCTION__ << ": dropping bad housekeeping"
                    << std::endl;
            continue;
        }
        
        // add in some radar paremters needed by products generator
        setRadarParams(radarParams, hskp);
                
        // now merge housekeeping with ray data
        if (!strncmp(hskp->radarName, "FORE", 4))
            hskpMerger[1].newHskp(hskp);
        else
            hskpMerger[0].newHskp(hskp);
    }

    close(inSocket);
    return NULL;
}


//////////////////////////////////////////////////////////////////////
/// Return a new EldoraDDS::Housekeeping built from the given buf
/// sent by the housekeeper machine.  The returned struct should be deleted 
/// by the caller.  On error, NULL is returned.
static EldoraDDS::Housekeeping* newDDSHousekeeping(const unsigned char* hskprBuf,
                                                   int buflen)
{
    const unsigned char* data = hskprBuf;
    int datalen = buflen;
    // data from the housekeeper currently comes 176 bytes per ray.
    if (buflen != 176) {
        std::cerr << __FUNCTION__ << ": Got " << buflen
                << "bytes instead of the expected 176" << std::endl;
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
        std::cerr << "Descriptor exception: " << dex.what() <<
        std::endl;
        return 0;
    }

    // Get a new EldoraDDS::Housekeeping
    EldoraDDS::Housekeeping* hskp = new EldoraDDS::Housekeeping();

    // timetag is microseconds since 1970/1/1 00:00:00 UTC
    hskp->timetag = ptimeToTimetag(ryib->getRayDateTime());

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

static void showStats(runParams& drxParams,
                      RR314& rr314,
                      unsigned long& droppedRay,
                      unsigned long& droppedTS,
                      int loopCount)
{
    // get the current temperature
    double temperature = rr314.temperature();

    // get the current byte count for each dmaChan
    // from rr314. This call causes the byte counters
    // in r314 to be reset to zero.
    //std::vector<unsigned long> bytes = rr314.bytes();

    std::cout << "Device:" << drxParams.deviceNumber << "  loop:" << loopCount++
            << "\n ";
    //    std::cout << std::setw(8);
    //    std::cout << std::setprecision(2);
    //    std::cout << "bytes processed ";
    //    // Print the number of free buffers in the rr314 buffer
    //    // pool. If 0, rr314 is being overrun
    //    unsigned long sum = 0;
    //    for (unsigned int c = 0; c < bytes.size(); c++) {
    //        std::cout << std::setw(6);
    //        std::cout << bytes[c] << " ";
    //        sum += bytes[c];
    //    }
    //    std::cout << sum << " ";
    //    std::cout << sum/10.0e6 << "MB/s";
    //    std::cout << "\n";

    std::cout << "samples      ";
    for (int i = 0; i < 8; i++) {
        std::cout << std::setw(8) << drxParams.sampleCounts[i];
        drxParams.sampleCounts[i] = 0;
    }
    std::cout << "\n";
    std::cout << "free IQ:" << rr314.numFreeIQBuffers() << "   free ABP:"
            << rr314.numFreeABPBuffers() << "   dropped rays:" << droppedRay
            << "   dropped TS:" << droppedTS << std::setprecision(4) << "   t:"
            << temperature << "C" << "\n";
    droppedRay = 0;
    droppedTS = 0;

    std::cout.flush();
}

//////////////////////////////////////////////////////////////////////
void publishAndCapture(RREntry* rentry,
                       Housekeeping* hskp)
{
    static std::ofstream* outFiles[2][8]; // outFiles[board][channel]

    RRBuffer* rbuf = rentry->rrBuffer();
    int channel = rbuf->dmaChan;
    int boardNum = rbuf->boardNumber();

    // Set a few things in the housekeeping that are extracted from the RRBuffer
    // and its parent RR314
    hskp->samples = rbuf->parent()->samples();
    hskp->startIQ = rbuf->parent()->firstIQGate();
    hskp->numIQ = rbuf->parent()->numIQGates();

    hskp->chan = rbuf->chanId;
    hskp->rayNum = rbuf->rayNum;

    // Publish if requested to
    if (rentry->publish()) {
        // send the buffer to the appropriate writer
        if (rbuf->type == RRBuffer::ABPtype) {
            // an abp dmaChan
            EldoraDDS::Ray* pDdsRay = _rayWriter->getEmptyItem();
            if (pDdsRay) {
                pDdsRay->hskp = *hskp;
                // device 0 is the aft radar, 1 is fore
                pDdsRay->radarId = (boardNum == 0) ? EldoraDDS::Aft
                        : EldoraDDS::Forward;

                RRABPBuffer* pABP = dynamic_cast<RRABPBuffer*>(rbuf);

                // get the prt identifier
                pDdsRay->prtId = pABP->prtId;

                // set the size
                pDdsRay->abp.length(pABP->_abp.size());

                for (unsigned int p = 0; p < pABP->_abp.size(); p++) {
                    float data = pABP->_abp[p];
                    pDdsRay->abp[p] = data;
                }

                // send the ray to the ray publisher
                _rayWriter->publishItem(pDdsRay);
            } else {
                _droppedRay[boardNum]++;
                //std::cout << "can't get publisher ray\n";
            }
        } else {
            // a time series dmaChan
            EldoraDDS::TimeSeries* pDdsTS = _tsWriter->getEmptyItem();
            if (pDdsTS) {
                pDdsTS->hskp = *hskp;
                // device 0 is the aft radar, 1 is fore
                pDdsTS->radarId = (boardNum == 0) ? EldoraDDS::Aft
                        : EldoraDDS::Forward;

                RRIQBuffer* pIQ = dynamic_cast<RRIQBuffer*>(rbuf);

                // set the size
                pDdsTS->tsdata.length(pIQ->_iq.size());

                for (unsigned int p = 0; p < pIQ->_iq.size(); p++) {
                    pDdsTS->tsdata[p] = pIQ->_iq[p];
                }
                // send the ray to the ray publisher
                _tsWriter->publishItem(pDdsTS);
            } else {
                _droppedTS[boardNum]++;
                //std::cout << "can't get publisher TS\n";
            }
        }
    }

    // Capture if requested to
    if (rentry->capture()) {
        // get the file that this dmaChan is saved to.
        std::ofstream* oStream = outFiles[boardNum][channel];
        if (!oStream) {
            std::stringstream s;
            s.width(2);
            s.fill('0');
            s << "data" << boardNum << "-" << channel;
            oStream = outFiles[boardNum][channel] = new std::ofstream(s.str().c_str(), std::ios::trunc);
        }
        if (rbuf->type == RRBuffer::ABPtype) {
            // ABP buffer
            RRABPBuffer* pABP = dynamic_cast<RRABPBuffer*>(rbuf);
            assert (pABP != 0);
            if (rentry->textcapture()) {
                for (unsigned int i = 0; i < pABP->_abp.size(); i += 3) {
                    *oStream << pABP->_abp[i] << " " << pABP->_abp[i+1] << " "
                            << pABP->_abp[i+2] << " " << std::endl;
                }
            } else {
                oStream->write((char*)(&pABP->_abp[0]), pABP->_abp.size()
                        * sizeof(pABP->_abp[0]));
            }
        } else {
            // IQ buffer
            RRIQBuffer* pIQ = dynamic_cast<RRIQBuffer*>(rbuf);
            assert(pIQ != 0);
            if (rentry->textcapture()) {
                for (unsigned int i = 0; i < pIQ->_iq.size(); i += 2) {
                    *oStream << pIQ->_iq[i] << " " << pIQ->_iq[i+1] << " "
                            << std::endl;
                }
            } else {
                oStream->write((char*)(&pIQ->_iq[0]), pIQ->_iq.size()
                        * sizeof(pIQ->_iq[0]));
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////
/// Return a complete new EldoraDDS::Housekeeping for the given time and
/// radar.  The returned struct should be deleted by the caller.
EldoraDDS::Housekeeping* newFakeDDSHousekeeping(ptime hskpTime,
                                                bool isFore)
{
    EldoraDDS::Housekeeping* hskp = new EldoraDDS::Housekeeping();
    unsigned int msecsIntoDay = hskpTime.time_of_day().total_milliseconds();
    float secOfDay = (float)msecsIntoDay / 1000.0;

    hskp->timetag = ptimeToTimetag(hskpTime);

    // Fake rotation angle: Fore antenna points 0.0 deg at the start of a day,
    // and scans at rotRate deg/s clockwise forever.  Aft antenna points 180
    // degrees away from fore.
    float rotRate = 30.0; // deg/s
    float rotAngle = fmodf(secOfDay * rotRate + (isFore ? 0.0 : 180.0), 360.0);
    hskp->radarRotAngle = rotAngle;

    strncpy(hskp->radarName, isFore ? "FORE" : "AFT", sizeof(hskp->radarName));
    return hskp;
}
//////////////////////////////////////////////////////////////////////
void sendFakeHousekeeping(const EldoraDDS::Housekeeping* hskp)
{
    static int outSocket = -1;
    static struct sockaddr_in destAddr;

    // Create our socket and destination addr if we haven't already
    if (outSocket < 0) {
        // Open our outgoing socket
        if ((outSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
            perror("creating outgoing UDP socket for housekeeping");
            exit(1);
        }
        // Create the destination address
        memset(&destAddr, 0, sizeof(destAddr));
        destAddr.sin_family = AF_INET;
        destAddr.sin_port = htons(_hskpPort);
        std::cerr << "housekeeping port: " << _hskpPort << ", addr: "
                << inet_addr("localhost") << std::endl;
        if (!inet_aton("localhost", &(destAddr.sin_addr))) {
            std::cerr <<__FILE__ << ":" << __LINE__ <<
            ": bad address for outgoing housekeeping" << std::endl;
        }
    }

    // Now actually send the packet of housekeeping as if we were the 
                    // housekeeper machine
                    //    int nwrote;
                    //    if ((nwrote = sendto(outSocket, hskp, sizeof(*hskp), 0, 
                    //            (struct sockaddr*)&destAddr, sizeof(destAddr))) < 0)
                    //        std::cerr << __FILE__ << ":" << __LINE__ << ": error sending housekeeping: " <<
                    //            strerror(errno) << std::endl;
                }
