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
#include <sys/select.h>

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

// operating parameters
bool _enabled[2]; ///< true if the dma transfers are to be started

// signal processing parameters
std::string _xsvf; ///< path to a bit file to be loaded into the RR314 fpga.
std::string _kaiser; ///< path to a file containing coefficients for the kaiser filter.
std::string _gaussian; ///< path to a file containing coefficients for the gaussian filter.

// operating mode parameters
bool _capture; ///< set true if the data should be captured to files in binary mode.
bool _textcapture; ///< set true if the data should be captured to files in text mode.
bool _simulateRR314; ///< set true to simulate an RR314 card, instead of accessing a real one.
bool _simulateHskp; ///< set true to generate fake housekeeping data
int _usleep; ///< The usleep value for sleeps between frames
bool _internaltimer; ///< If true, use the rr314 builtin timer rather than an external

// DDS parameters
bool _publish; ///< set ture if the rr314 data should be published to DDS.
std::string _ORB; ///< path to the ORB configuration file.
std::string _DCPS; ///< path to the DCPS configuration file.
std::string _rayTopic; ///< The published ray topic
std::string _productsTopic; /// The published product topic

/// The port number for RPC comms to eldoradrx.
int _rpcPort;

static const unsigned int DEFAULT_HSKP_PORT = 2222;
/// The port number for incoming housekeeper data
unsigned int _hskpPort;

// save pointers to RR314 instances, in case we need to try
// to stop them during a signal.
std::vector<RR314*> _rr314Instances;

/// Set this flag to true in order to request the main loop to
/// shut everything down.
bool _terminate = false;
/// Set this flag to ask the main loop to start everything
bool _sysStart = false;
/// set this flag to ask the main loop to stop everything
bool _sysStop = false;
/// true when the cards are running, false otherwise
bool _sysRunning = false;

/// Dropped ray counters, for and aft.
unsigned long _droppedRays[2];
/// Dropped time series counters, for and aft.
unsigned long _droppedTS[2];
/// 8 DMA channel sample counters, for and aft.
unsigned long _sampleCounts[2][8]; ///< collect the number of DDS samples written for each dma channel.

/// The Bittware timer
Bittware* _bwtimer = 0;

/// The rpc handler
DrxRPC* _rpcHandler;

/// Our DDS services
/// The publisher.
static DDSPublisher* _publisher = 0;
/// The ray writer.
static RayWriter* _rayWriter = 0;
/// The time series writer.
static TSWriter* _tsWriter = 0;

/// The two rr314 cards, for and aft.
RR314* _rr314[2] =
    {
            0,
            0 };

/// Radar operating parameters, for and aft.
static EldoraRadarParams _radarParams[2];

/// The housekeeper merger, for and aft.
HskpMerger* _hskpMerger[2] =
    {
            0,
            0 };

/// The two rr314 threads
pthread_t _rrThread[2] =
    {
            0,
            0 };

/// The housekeepr thread
pthread_t _hskpThread = 0;

// prototypes
//
/// create the DDS services
static void createDDSservices();
/// Create rr314, bittware and hskpMerger instances.
/// Start the rr314 and hskp data threads.
static void startAll();
/// Stop all of the threads and destroy rr314, bittware and hskpMerger instances.
static void stopAll();
/// Get items specified in the configuration file.
static void getConfigParams();
/// Set parameters specified on the command line
static void parseOptions(int argc,
                         char** argv);
/// Used for an emergency stop of the rr314 boards.
/// Must be done on a signal capture. If the rr314 boards
/// are left running after the program exits, they will crash 
/// Linux. Big Time.
static void shutdownBoards();
/// signal handler to capture those unexpected signals
static void setupSignalHandler();
/// The function that runs in the thread for reading rr314 data.
static void* rrReadTask(void* threadArg);
/// The function that runs in the thread for reading hskp data.
static void* hskpReadTask(void* threadArg);
/// Print status for a single rr card.
static void showStats(int devNumber,
                      int loopCount);
/// Print overall status
static void printStatus(int loopcount);
/// return the rr314 byte counts for the given boar.
//// return zeros if it is not running
std::vector<unsigned long> boardBytes(int boardNum);

static EldoraDDS::Housekeeping* newDDSHousekeeping(const unsigned char* buf,
                                                   int buflen);
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

    // get the parameters specified in the configuration. These
    // may be overriden by command line options in the call to parseOptions.
    getConfigParams();

    // parse command line options. Initialize with values from
    // the configuration, and then allow them to be overriden
    // from the command line.
    parseOptions(argc, argv);

    // create DDS facilities if they are requested
    if (_publish) {
        createDDSservices();
    }

    // And set radar specific fields
    // set the radar names
    for (int i = 0; i < 4; i++) {
        _radarParams[0].radd_name[i] = "FORE"[i];
        _radarParams[1].radd_name[i] = "AFT "[i];
    }

    int loopCount = 0;

    // create our RPC handler. Tell it where the start, stop and terminate
    // flags are, and the function to call for board byte counts.
    _rpcHandler
            = new DrxRPC (_rpcPort, &_sysStart, &_sysStop, &_terminate, _radarParams, boardBytes);

    // This is the main loop
    while (1) {
        // poll once per second for commands to start or stop.
        // print the status every ten seconds
        for (int i = 0; i < 10; i++) {
            if (_sysStart) {
                if (!_sysRunning)
                    startAll();
                loopCount = 0;
                _sysStart = false;
                _sysRunning = true;
            }
            if (_sysStop) {
                _sysStop = false;
                // set _sysRunning to false immediately so
                // that boardBytes does not try to access 
                // rr314 instances while they are being destroyed in 
                // stopAll()
                /// @todo We need to mutex protect _sysRunning!
                _sysRunning = false;
                stopAll();
            }
            if (_terminate)
                break;
            sleep(1);
        }
        if (_terminate)
            break;
        if (_sysRunning)
            printStatus(loopCount);
        loopCount++;
    }

    std::cout << "Terminating " << argv[0] << "\n";
    if (_bwtimer) {
        delete _bwtimer;
    }
}

/////////////////////////////////////////////////////////////////////////
static void printStatus(int loopCount)
{
    // stats for card 0
    showStats(0, loopCount);
    // hskp merger info for 0
    _hskpMerger[0]->showStats(std::cout);
    // stats for card 1
    showStats(1, loopCount);
    // hskp merger info for 1
    _hskpMerger[1]->showStats(std::cout);
    // flush cout so that clientes reading from 
    // a pipe will see the output withou buffering
    std::cout.flush();
}

/////////////////////////////////////////////////////////////////////////
static void cancelThread(pthread_t* id)
{
    if (*id) {
        pthread_cancel(*id);
        pthread_join(*id, 0);
        *id = 0;
        sleep(1);
    }
}
/////////////////////////////////////////////////////////////////////////

static void stopAll()
{

    // stop the rr314 cards
    if (_rr314[0])
        _rr314[0]->shutdown();
    if (_rr314[1])
        _rr314[1]->shutdown();

    // kill the threads
    cancelThread(&_hskpThread);
    cancelThread(&_rrThread[0]);
    cancelThread(&_rrThread[1]);

    // AS a little extra insurance, zero the references to our 
    // objects before deleting them, because other routines(entered via other
    // threads) will often check to see if the pointer is zero before
    // trying to access the instance,
    for (int i = 0; i < 2; i++) {
        RR314* p314;
        p314 = _rr314[i];
        _rr314[i] = 0;
        delete p314;
    }
    for (int i = 0; i < 2; i++) {
        HskpMerger* pHskp;
        pHskp = _hskpMerger[i];
        _hskpMerger[i] = 0;
        delete pHskp;
    }

}

/////////////////////////////////////////////////////////////////////////
static void startAll()
{

    // initialize items that are reused on each restart:

    // hskp thread
    _hskpThread = 0;
    // bittware timer
    _bwtimer = 0;
    for (int i = 0; i < 2; i++) {
        // rr data read threads
        _rrThread[i] = 0;
        // rr314 cards
        _rr314[i] = 0;
        // hskp mergers
        _hskpMerger[i] = 0;
    }

    // initialize status counters
    for (int d = 0; d < 2; d++) {
        _droppedRays[d] = 0;
        _droppedTS[d] = 0;
        for (int i = 0; i < 8; i++) {
            _sampleCounts[d][i] = 0;
        }
    }

    // create timer
    if (!_simulateRR314 && !_internaltimer) {
        _bwtimer = new Bittware(0);
        _bwtimer->configure(_radarParams[0]);
        if (!_bwtimer->isok()) {
            std::cerr << "Unable to create bittware timer\n";
            _bwtimer->shutdown();
            exit(1);
        }
    }

    // create two RR314 cards
    try {
        _rr314[0] = new RR314(0,
                _radarParams[0],
                _gaussian,
                _kaiser,
                _xsvf,
                _internaltimer,
                _simulateRR314,
                _usleep,
                false // do not catch signals in RR314; we will do that ourselves.
        );

        _rr314[1] = new RR314(1,
                _radarParams[1],
                _gaussian,
                _kaiser,
                _xsvf,
                _internaltimer,
                _simulateRR314,
                _usleep,
                false // do not catch signals in RR314; we will do that ourselves
        );

        // save instances of RR314 for the shutdown handler
        _rr314Instances.push_back(_rr314[0]);
        _rr314Instances.push_back(_rr314[1]);

        // Create our housekeeping mergers, with 500 ms max retention time waiting to 
        // make matches
        _hskpMerger[0] = new HskpMerger(_rr314[0], 500, publishAndCapture);
        _hskpMerger[1] = new HskpMerger(_rr314[1], 500, publishAndCapture);

        // create the rr314 data reading threads
        if (_enabled[0]) {
            pthread_create(&_rrThread[0], NULL, rrReadTask, (void*) 0);
        }

        if (_enabled[1]) {
            pthread_create(&_rrThread[1], NULL, rrReadTask, (void*) 1);
        }

        // note if we're simulating housekeeping
        if (_simulateHskp)
        std::cout << "Housekeeping will be simulated" << std::endl;

        // Shift the default port for simulated housekeeping off of the 
        // normal default, so that we can simulate even if the real housekeeper
        // is running.
        _hskpPort = (_simulateHskp) ?
        (DEFAULT_HSKP_PORT + 1) : DEFAULT_HSKP_PORT;

        // and the housekeeping reader thread
        pthread_create(&_hskpThread, NULL, hskpReadTask, (void*)0);

        // setup the signal handlers before we run the cards.
        setupSignalHandler();

        // start the RR314 cards. They will not actually start 
        // sending data until the bittware timer is started.
        if (_enabled[0]) {
            _rr314[0]->start();
        }
        if (_enabled[1]) {
            _rr314[1]->start();
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
            _rr314[0]->setXmitStartTime(xmitStartTime);
            _rr314[1]->setXmitStartTime(xmitStartTime);
        } else {
            if (!_simulateRR314) {
                // start internal timing for both cards 0 & 1
                _rr314[0]->startInternalTimer();
                _rr314[1]->startInternalTimer();
            }
        }

    }
    catch (std::string e)
    {
        std::cout << e << std::endl;
        exit(1);
    }
}
//////////////////////////////////////////////////////////////////////
///
/// get parameters that are spcified in the configuration file.
/// These can be overriden by command line specifications.
static void getConfigParams()
{

    QtConfig config("NCAR", "EldoraDrx");

    // set up the default configuration directory path
    std::string EldoraDir("/conf/");
    char* e = getenv("ELDORADIR");
    if (e) {
        EldoraDir = e + EldoraDir;
    } else {
        std::cerr << "Environment variable ELDORADIR must be set.\n";
        exit(1);
    }

    // and create the default DDS configuration file paths, since these
    // depend upon ELDORADIR
    std::string orbFile = EldoraDir + "ORBSvc.conf";
    std::string dcpsFile = EldoraDir + "DDSClient.ini";
    std::string rayTopic;
    std::string productsTopic;

    // operating mode parameters
    _enabled[0] = config.getBool("Mode/Enabled", true);
    _enabled[1] = config.getBool("Mode/Enabled", true);
    _simulateRR314 = config.getBool("Mode/SimulateRR314", false);
    _simulateHskp = config.getBool("Mode/SimulateHskp", false);
    _usleep = config.getInt("Mode/Usleep", 9000);
    _capture = config.getBool("Mode/BinaryCapture", false);
    _textcapture = config.getBool("Mode/TextCapture", false);
    _internaltimer = config.getBool("Mode/InternalTimer", false);

    // RR314 signal processing parameters
    _xsvf = config.getString("DSP/XsvfFile", "");
    _kaiser = config.getString("DSP/KaiserFile", "");
    _gaussian = config.getString("DSP/GaussianFile", "");

    // DDS parameters
    _publish = config.getBool("DDS/Publish", true);
    _ORB = config.getString("DDS/ORBConfigFile", orbFile);
    _DCPS = config.getString("DDS/DCPSConfigFile", dcpsFile);
    _rayTopic = config.getString("DDS/TopicRay", "EldoraRays");
    _productsTopic = config.getString("DDS/TopicProducts", "EldoraProducts");

    // RPC parameters
    _rpcPort = config.getInt("Rpc/RpcPort", 60000);
}
//////////////////////////////////////////////////////////////////////
//
/// Parse the command line options, and also set some options 
/// that are not specified on the command line.
/// @return The runtime options that can be passed to the
/// threads that interact with the RR314.
static void parseOptions(int argc,
                         char** argv)
{

    // get the option34
    po::options_description descripts("Options");
    descripts.add_options() ("help", "describe options") ("ORB", po::value<std::string>(&_ORB), "ORB service configuration file (Corba ORBSvcConf arg)")
    ("DCPS", po::value<std::string>(&_DCPS), "DCPS configuration file (OpenDDS DCPSConfigFile arg)")
    ("simRR314", "run RR314 in simulation mode")
    ("simHskp", "generate fake housekeeping data")
    ("usleep", po::value<int>(&_usleep), "usleep value for simulation")
    ("start0", "start RR314 device 0")
    ("start1", "start RR314 device 1")
    ("internaltimer", "use RR314 internal timer")
    ("xsvf", po::value<std::string>(&_xsvf), "path to xsvf file")
    ("kaiser", po::value<std::string>(&_kaiser), "path to kaiser coefficient file")
    ("gaussian", po::value<std::string>(&_gaussian),"path to gaussian coefficient file")
    ("binary", "binary capture")
    ("text", "text capture")
    ("publish", "publish data")
    ("rpcport", po::value<int>(&_rpcPort), "RPC port number");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, descripts), vm);
    po::notify(vm);

    _publish = vm.count("publish") != 0;
    _capture = (vm.count("binary") != 0) || (vm.count("text") != 0);
    _textcapture = vm.count("text") != 0;
    _simulateRR314 = vm.count("simRR314") != 0;
    _simulateHskp = vm.count("simHskp") != 0;
    _internaltimer = vm.count("internaltimer") != 0;

    if (vm.count("help") || (vm.count("binary") && vm.count("text"))) {
        std::cout << "Initialize two rr314 cards. If --start0 ond/or --start1 is specified,\n";
        std::cout << "that card will be started for data collection.\n";
        std::cout << descripts << "\n";
        exit(1);
    }

    // see if this device was selected to be active
    if (vm.count("start0"))
    _enabled[0] = true;
    if (vm.count("start1"))
    _enabled[1] = true;

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
        (*p)->shutdown();
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
static void setRadarParams(EldoraRadarParams* radarParams,
                           EldoraDDS::Housekeeping* hskp)
{

    for (int i = 0; i < 4; i++)
        hskp->rxGain[i] = radarParams->frib_rxgain[i];
    
    for (int i = 0; i < 6; i++)
        hskp->cellWidth[i] = radarParams->cspd_width[i];

    hskp->freqs[0] = radarParams->radd_freq1;
    hskp->freqs[1] = radarParams->radd_freq2;
    hskp->freqs[2] = radarParams->radd_freq3;
    hskp->freqs[3] = radarParams->radd_freq4;

    hskp->xBandGain = radarParams->frib_xgain;
    hskp->lnaLoss = radarParams->frib_lnalos;
    hskp->noisePower = radarParams->radd_noipow;
    hskp->radarConstant = radarParams->radd_const;
    hskp->prt = radarParams->radd_ipp1;
    hskp->prtLong = radarParams->radd_ipp2;

    hskp->dbzScale = radarParams->parm_dbz_scale;
    hskp->dbzBias = radarParams->parm_dbz_bias;

    hskp->swScale = radarParams->parm_sw_scale;
    hskp->swBias = radarParams->parm_sw_bias;

    hskp->ncpScale = radarParams->parm_ncp_scale;
    hskp->ncpBias = radarParams->parm_ncp_bias;

    hskp->vsScale = radarParams->parm_vs_scale;
    hskp->vsBias = radarParams->parm_vs_bias;

    hskp->vlScale = radarParams->parm_vl_scale;
    hskp->vlBias = radarParams->parm_vl_bias;

    hskp->vrScale = radarParams->parm_vr_scale;
    hskp->vrBias = radarParams->parm_vr_bias;
}
//////////////////////////////////////////////////////////////////////
//
/// Arrange to catch signals. signalHandler() function
/// will be specifiec to called when signals are caught.
static void setupSignalHandler()
{

    // install signal handler for selected signals
    struct sigaction new_action, old_action;

    // Set up the structure to specify the new action.
    new_action.sa_handler = signalHandler;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;

    // signals of interest
    int sig_list[] =
        {
                SIGINT,
                SIGHUP,
                SIGTERM,
                SIGSEGV };
    int nsignals = sizeof(sig_list) / sizeof(*sig_list);

    // Change the handler for signals in the list that aren't being ignored.
    for (int s = 0; s < nsignals; s++) {
        sigaction(sig_list[s], NULL, &old_action);
        if (old_action.sa_handler != SIG_IGN)
            sigaction(sig_list[s], &new_action, NULL);
    }
}

//////////////////////////////////////////////////////////////////////
//
// a task which just consumes the data from the
// rr314 as it becomes available
static void* rrReadTask(void* threadArg)
{
    int devNumber = (int)threadArg;

    EldoraRadarParams* radarParams = _radarParams + devNumber;

    RR314* pRR314;
    if (devNumber == 0)
        pRR314 = _rr314[0];
    else
        pRR314 = _rr314[1];

    HskpMerger* hskpMerger = _hskpMerger[devNumber];

    int gates = radarParams->wave_ngates[0];
    int numiq = radarParams->frib_frqgat;
    int samples = radarParams->wave_seqrep;

    bool simulateHskp = _simulateHskp;

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
        _sampleCounts[devNumber][channel]++;

        hskpMerger->newRRBuffer(pBuf, _publish, _capture, _textcapture);

        // If we're doing fake housekeeping, deal with it now.
        if (simulateHskp) {
            // If we don't have this time in our history, generate and send
            // some fake housekeeping.
            if (sentFakeHskp.find(pBuf->rayTime) == sentFakeHskp.end()) {
                // Generate and send the housekeeping
                EldoraDDS::Housekeeping* hskp =
                newFakeDDSHousekeeping(pBuf->rayTime, (devNumber == 1));

                // add in some radar paremters needed by products generator
                setRadarParams(radarParams, hskp);

                // Send the fake housekeeping on its way
                sendFakeHousekeeping(hskp);
                delete hskp;

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
        // Wait up to 0.1 second for data on our socket.  We don't want too
        // long a timeout, since we want to be able to respond to thread
        // cancellation pretty quickly.
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(inSocket, &fds)
;
                                                struct timeval timeout =
            {
                    0,
                    100000 }; // 0.1 second
        int nfds = select(inSocket + 1, &fds, NULL, NULL, &timeout);

        // If our parent is cancelling us, let it happen now.
        pthread_testcancel();

        // If the select timed out and we lived through the cancel test,
        // go back and wait again.
        if (nfds == 0)
            continue;

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

        // now merge housekeeping with ray data
        if (!strncmp(hskp->radarName, "FORE", 4)) {
            // add in some radar paremters needed by products generator
            setRadarParams(&_radarParams[1], hskp);
            _hskpMerger[1]->newHskp(hskp);
        } else {
            setRadarParams(&_radarParams[0], hskp);
            _hskpMerger[0]->newHskp(hskp);
        }
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
        
        // unpack the "header-only" FRAD descriptor from the housekeeper
        frad = new DoradeFRAD(data, datalen, false, true);
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

static void showStats(int devNumber,
                      int loopCount)
{
    // get the current temperature
    double temperature = _rr314[devNumber]->temperature();

    std::cout << "*** Device:" << devNumber << "  loop:" << loopCount++;

    std::cout << " samples:";
    for (int i = 0; i < 8; i++) {
        std::cout << std::setw(8) << _sampleCounts[devNumber][i];
        _sampleCounts[devNumber][i] = 0;
    }

    std::cout << "\n";
    std::cout << "free IQ:" << _rr314[devNumber]->numFreeIQBuffers()
            << "   free ABP:" << _rr314[devNumber]->numFreeABPBuffers()
            << "   dropped rays:" << _droppedRays[devNumber]
            << "   dropped TS:" << _droppedTS[devNumber]
            << std::setprecision(4) << "   t:" << temperature << "C" << "\n";

    _droppedRays[devNumber] = 0;
    _droppedTS[devNumber] = 0;

    std::cout.flush();
}

//////////////////////////////////////////////////////////////////////
void publishAndCapture(RREntry* rentry,
                       Housekeeping* hskp)
{
    static std::ofstream* outFiles[2][8]; // outFiles[board][channel]

    //if (!_sysRunning) {
    //    std::cout << __FUNCTION__ << ", not running, publish ignored\n";
    //    return;
    //}

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
                _droppedRays[boardNum]++;
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

/////////////////////////////////////////////////////////////////////
static void createDDSservices()
{
    std::cout <<__FILE__ << " creating DDS services\n";

    ArgvParams argv("eldoradrx");
    argv["-ORBSvcConf"] = _ORB;
    argv["-DCPSConfigFile"] = _DCPS;

    // create our DDS publisher
    _publisher = new DDSPublisher(argv.argc(), argv.argv());
    if (_publisher->status()) {
        std::cout << "Unable to create a publisher, exiting.\n";
        exit(1);
    }

    // create the ray writer
    _rayWriter = new RayWriter(*_publisher, "EldoraRays");

    // create the time series writer
    _tsWriter = new TSWriter(*_publisher, "EldoraTS");
}
/////////////////////////////////////////////////////////////////////
std::vector<unsigned long> boardBytes(int boardNum)
{
    if (_rr314[0] && _sysRunning)
        return _rr314[boardNum]->bytes();

    std::vector<unsigned long> bytes(8, 0);
    return bytes;

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
    float rotRate = 18.0; // deg/s
    float rotAngle = fmodf(secOfDay * rotRate + (isFore ? 0.0 : 180.0), 360.0);
    hskp->radarRotAngle = rotAngle;

    strncpy(hskp->radarName, isFore ? "FORE" : "AFT", sizeof(hskp->radarName));
    return hskp;
}
//////////////////////////////////////////////////////////////////////
// Send the given EldoraDDS::Housekeeping to _hskpPort as if it came
// from the VxWorks housekeeper
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

    // stringstream to hold the packet as we assemble it
    std::ostringstream ss;
    
    // assemble the RYIB from the housekeeping
    DoradeRYIB ryib(timetagToPtime(hskp->timetag), hskp->sweepNum, 
        hskp->azimuth, hskp->elevation, hskp->peakXmitPower, hskp->trueScanRate, 
        hskp->rayStatus);
    ryib.streamTo(ss, false);
    
    // assemble an ASIB from the housekeeping
    DoradeASIB asib(hskp->longitude, hskp->latitude, 
        hskp->altitudeMSL, hskp->altitudeAGL, 
        hskp->groundSpeedEW, hskp->groundSpeedNS, hskp->vertVelocity, 
        hskp->heading, hskp->roll, hskp->pitch, hskp->yaw, 
        hskp->radarRotAngle, hskp->radarTiltAngle, 
        hskp->windEW, hskp->windNS, hskp->windVert, 
        hskp->headingChangeRate, hskp->pitchChangeRate);
    asib.streamTo(ss, false);
    
    // assemble a FRAD from the housekeeping
    DoradeFRAD frad(hskp->dataSysStatus, hskp->radarName, 
        hskp->testPulsePower, hskp->testPulseStart, hskp->testPulseWidth, 
        hskp->testPulseFreq, hskp->testPulseAtten, hskp->testPulseFNum, 
        hskp->noisePower, hskp->rayCount, hskp->firstRecGate, 
        hskp->lastRecGate);
    frad.streamTo(ss, false);

    // Now actually send the packet of housekeeping as if we were the 
    // housekeeper machine
    int nwrote = sendto(outSocket, ss.str().data(), ss.str().size(), 0,
            (struct sockaddr*)&destAddr, sizeof(destAddr));
    if (nwrote < 0)
        std::cerr << __FILE__ << ":" << __LINE__ <<
            ": error sending housekeeping: " << strerror(errno) << std::endl;
}
