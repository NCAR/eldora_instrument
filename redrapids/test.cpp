#include "RR314.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ios>
#include "boost/program_options.hpp"

#include "EldoraPublisher.h"

using namespace RedRapids;
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
	params.simulate = false;

	// get the options
	po::options_description descripts("Options");
	descripts.add_options() ("help", "describe options") 
		("simulate",po::value<bool>(&params.simulate)->default_value(false),"run in simulation mode") 
		("device", po::value<int>(&params.device)->default_value(0), "device number") 
		("gates",po::value<int>(&params.gates)->default_value(1000), "number of gates") 
		("nci", po::value<int>(&params.nci)->default_value(100),"number of coherent integrations") 
		("startiq", po::value<int>(&params.startiq)->default_value(0), "start gate for iq capture")
		("numiq", po::value<int>(&params.numiq)->default_value(100),"number of gates for iq capture") 
		("decimation", po::value<int>(&params.decimation)->default_value(12), "decimation factor") 
		("xsvf", po::value<std::string>(&params.xsvf)->default_value(""), "path to xsvf file")
		("kaiser", po::value<std::string>(&params.kaiser)->default_value(""),"path to kaiser coefficient file") 
		("gaussian",po::value<std::string>(&params.gaussian)->default_value(""),"path to gaussian coefficient file") 
		("capture",po::value<bool>(&params.capture)->default_value(false),"capture data to files")
		("publish",po::value<bool>(&params.publish)->default_value(false),"publish data");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, descripts), vm);
	po::notify(vm);

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
	bool publish = pParams->publish;
	bool capture = pParams->capture;
	
	char* argv[] = { "testrr314", "-ORBSvcConf",
			"/home/martinc/workspace/Eldora/redrapids/conf/tcp.conf",
			"-DCPSConfigFile",
			"/home/martinc/workspace/Eldora/redrapids/conf/simpleConf.ini" };
	int argc = 5;

	// create the publisher
	ACE_Time_Value small(0, 100);
	EldoraPublisher publisher;

	if (publish) {
		int pubStatus = publisher.run(argc, argv);

		if (pubStatus) {
			std::cout << "Unable to run the publsher, return status from run is "
			<< pubStatus << std::endl;
		}
	}

	int pulses = 0;
	// loop while waiting for new buffers
	while (1) {

		RRbuffer* pBuf = pRR314->nextBuffer();

		// get the details of this buffer
		int channel = pBuf->channel;
		std::ofstream* pStream = pParams->ofstreams[channel];
		std::vector<int>& buf = pBuf->_data;

		// process buffer
		for (int i = 0; i < pBuf->nSamples; i++) {
			if ((channel %4)) {
				// ABP channel
				if (capture) {
					if ( (pParams->sampleCounts[channel] % (gates+2)) == 0) {
					// add a beam indicator for ABP channels
					*pStream << "beam " << (pParams->sampleCounts[channel]
							/(gates+2)) << std::endl;
					}
					// write the data to the channel file
					*pStream << buf[i] << std::endl;
				}

				if (publish) {
					if (pParams->sampleCounts[channel] == (gates+1)) {
					// send the plse to the publisher					
					EldoraDDS::Pulse* pPulse = publisher.getEmptyPulse();
					if (pPulse) {
						// set the timestamp
						//pPulse->timestamp = timestamp;

						// alternate the radar id between forward and aft
						pPulse->radarId = (pulses++ % 2) ? EldoraDDS::Forward
								: EldoraDDS::Aft;

						// send the pulse to the publisher
						publisher.publishPulse(pPulse);
					} else {
						std::cout << "can't get publisher pulse\n";
					}
					}
				}
			} else {
				// IQ channel
				if (capture) {
				// break into I and Q
				int iq = buf[i];
				short I = (buf[i] & 0xffff0000) >> 16;
				short Q = buf[i] & 0xffff;
				// write the data to the channel file
				*pStream << I << " " << Q << std::endl;
			}
			}

			// bump the sample count
			pParams->sampleCounts[channel]++;
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

		// peridically display the card activity.
		while(1) {
			std::vector<unsigned long> bytes = rr314.bytes();

			std::cout << std::setw(4);
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

