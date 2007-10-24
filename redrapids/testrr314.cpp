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
	std::ofstream* ofstreams[16];
	unsigned long sampleCounts[16];
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
			"run in simulation mode") ("device", po::value<int>(&params.device)->default_value(0), "device number") ("gates",
			po::value<int>(&params.gates)->default_value(1000), "number of gates") (
			"nci", po::value<int>(&params.nci)->default_value(100),
			"number of coherent integrations") ("startiq", po::value<int>(&params.startiq)->default_value(0), "start gate for iq capture") (
			"numiq", po::value<int>(&params.numiq)->default_value(100),
			"number of gates for iq capture") ("decimation", po::value<int>(&params.decimation)->default_value(12), "decimation factor") (
			"xsvf", po::value<std::string>(&params.xsvf)->default_value(""), "path to xsvf file") (
			"kaiser", po::value<std::string>(&params.kaiser)->default_value(""),
			"path to kaiser coefficient file") ("gaussian",
			po::value<std::string>(&params.gaussian)->default_value(""),
			"path to gaussian coefficient file") ("capture",
			"capture data to files") ("publish", "publish data");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, descripts), vm);
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
	for (int i = 0; i < 16; i++) {
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
	argv["-ORBSvcConf"] = "/home/eldora/eldora/conf/tcp.conf";
	argv["-DCPSConfigFile"] = "/home/eldora/eldora/conf/simpleConf.ini";

	ACE_Time_Value small(0, 100000);

	DDSPublisher* publisher;
	DDSWriter<Pulse, PulseTypeSupportImpl, PulseTypeSupport_var, PulseDataWriter, PulseDataWriter_var>
			* pulseWriter;
	DDSWriter<TimeSeries, TimeSeriesTypeSupportImpl, TimeSeriesTypeSupport_var, TimeSeriesDataWriter, TimeSeriesDataWriter_var>
			* tsWriter;

	if (publish) {

		std::cout << "creating DDS services\n";
		// create the publisher
		publisher = new DDSPublisher(argv.argc(), argv.argv());

		// create the pulse writer
		pulseWriter = new PulseWriter(*publisher, "EldoraPulses");

		// create the time series writer
		tsWriter = new TSWriter(*publisher, "EldoraTS");
	}

	int buffers = 0;

	// loop while waiting for new buffers
	while (1) {
		// next buffer will block until a new buffer is ready.
		RRbuffer* pBuf = pRR314->nextBuffer();
		// get the details of this buffer
		int channel = pBuf->channel;

		// Are we publishing?
		if (publish) {
			// send the buffer to the appropriate writer
			if (channel %4) {
				// an abp channel
				EldoraDDS::Pulse* pPulse = pulseWriter->getEmptyItem();
				if (pPulse) {
					// set the size
					pPulse->abp.length(gates*3);
					// set the timestamp
					pPulse->timestamp = buffers++;
					// alternate the radar id between forward and aft
					pPulse->radarId = EldoraDDS::Aft;
					// send the pulse to the pulde publisher
					pulseWriter->publishItem(pPulse);
				} else {
					//std::cout << "can't get publisher pulse\n";
					ACE_OS::sleep(small);
				}
			} else {
				// a time series channel
				EldoraDDS::TimeSeries* pTS = tsWriter->getEmptyItem();
				if (pTS) {
					// set the size
					pTS->tsdata.length(numiq*2);
					// set the timestamp
					pTS->timestamp = buffers++;
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
			// get the file  that this channel is saved to.
			std::ofstream* pStream = pParams->ofstreams[channel];
			// and the pointer to the data. Note that each beam
			// and each gate collection is preceeded by 4 bytes
			// containing identifing information.
			std::vector<int>& buf = pBuf->_data;
			
			// save all of the samples
			for (int i = 0; i < pBuf->nSamples; i++) {
				if ((channel %4)) {
					// ABP channel
					if ( (pParams->sampleCounts[channel] % (gates+2)) == 0) {
						// add a beam counter for ABP channels
						*pStream << "beam " << (pParams->sampleCounts[channel]
								/(gates +2)) << std::endl;
					}
					// write the data to the channel file
					*pStream << buf[i] << std::endl;
				} else {
					// IQ channel
					// break into I and Q
					int iq = buf[i];
					short I = (buf[i] & 0xffff0000) >> 16;
					short Q = buf[i] & 0xffff;
					// write the data to the channel file
					*pStream << I << " " << Q << std::endl;
				}
			}
		}

		// bump the sample count
		pParams->sampleCounts[channel]++;

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

	for (int i = 0; i < 16; i++) {
		params.sampleCounts[i] = 0;
	}
	// create an RR314 card
	try {
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
		while(1) {
			// get the current byte count for each channel
			// from rr314. This call causes the byte counters
			// in r314 to be reset to zero.
			std::vector<unsigned long> bytes = rr314.bytes();

			std::cout << std::setw(4);
			// Print the number of free buffers in the rr314 buffer
			// pool. If 0, rr314 is being overrun
			std::cout << rr314.numFreeBuffers() << " ";
			unsigned long sum = 0;
			for (int c = 0; c < bytes.size(); c++) {
				std::cout << std::setw(6);
				std::cout << bytes[c] << " ";
				sum += bytes[c];
			}
			std::cout << sum;
			std::cout << "\n";

			sleep(10);
		}
	}
	catch (std::string e) {
		std::cout << e << std::endl;
		exit(1);
	}

}

