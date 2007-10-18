#include <iostream>

#include "DDSPublisher.h"
#include "DDSWriter.h"
#include <boost/program_options.hpp>
#include "ArgvParams.h"
namespace po = boost::program_options;

using namespace EldoraDDS;

int main(int argc, char* argv[]) {

	int nWrites = 5;

	std::string topicName;
	std::string ORB;
	std::string DCPS;
	int delta;

	// get the options
	po::options_description descripts("Options");
	
	descripts.add_options() 
	("help", "describe options") 
	("delta", po::value<int>(&delta)->default_value(100), "publish delta (uSecs)")
	("topic", po::value<std::string>(&topicName), "DDS topic") 
	("ORB",   po::value<std::string>(&ORB),       "ORB service configuration file (Corba ORBSvcConf arg)") 
	("DCPS",  po::value<std::string>(&DCPS),      "DCPS configuration file (OpenDDS DCPSConfigFile arg)") 
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, descripts), vm);
	po::notify(vm);

	if (vm.count("help") || !vm.count("ORB") || !vm.count("DCPS")) {
		std::cout << descripts << "\n";
		exit(1);
	}
	
	// we have to do this bit of translation since the 
	// DDS routines want arguments starting with a single dash,
	// whereas boost::program_options uses double dashes.
	ArgvParams pubParams(argv[0]);
	pubParams["-ORBSvcConf"] = ORB;
	pubParams["-DCPSConfigFile"] = DCPS;

	ACE_Time_Value small(0, delta);
	DDSPublisher publisher(pubParams.argc(), pubParams.argv());

	int pubStatus = publisher.status();

	if (pubStatus) {
		std::cout << "Unable to run the publsher, status is "
				<< pubStatus << std::endl;
		return pubStatus;
	}

	DDSWriter<Pulse, PulseTypeSupportImpl, PulseTypeSupport_var, PulseDataWriter, PulseDataWriter_var>
			pulseWriter(publisher, topicName);

	short timestamp = 0;
	int numPulses = 0;
	int gates = 1500;

	while ( !pulseWriter.is_finished()) {

		while ((pulseWriter.itemsAvailable()) < nWrites) {
			ACE_OS::sleep(small);
		}
		for (int i = 0; i < nWrites; i++) {
			EldoraDDS::Pulse* pPulse;

			// get an available empty pulse from the publisher
			pPulse = pulseWriter.getEmptyItem();
			pPulse->abp.length(3*gates);

			// bump the timestamp on alternating pulses
			// so that forward and aft share a common timestamp
			timestamp += (numPulses % 2);

			numPulses++;

			for (int n = 0; n < 3*gates; n += 3) {
				pPulse->abp[n ] = timestamp + n;
				pPulse->abp[n+1] = timestamp + n+1;
				pPulse->abp[n+2] = timestamp + n+2;
			}

			// set the timestamp
			pPulse->timestamp = timestamp;

			// alternate the radar id between forward and aft
			pPulse->radarId = (numPulses % 2) ? EldoraDDS::Forward
					: EldoraDDS::Aft;

			// send the pulse to the publisher
			pulseWriter.publishItem(pPulse);

		}
		ACE_OS::sleep(small);
	}

	return 0;
}
