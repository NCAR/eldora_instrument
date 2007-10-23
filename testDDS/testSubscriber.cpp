#include <iostream>
#include <iomanip>
#include <boost/program_options.hpp>

#include "ArgvParams.h"
#include "DDSReader.h"
#include "TestReader.h"

namespace po = boost::program_options;

///////////////////////////////////////////////////////////////////////////////

void parseArgs(int argc, char** argv, std::string& topicName, std::string& ORB,
		std::string& DCPS) {

	// get the options
	po::options_description descripts("Options");

	descripts.add_options() ("help", "describe options") ("topic", po::value<std::string>(&topicName), "DDS topic")
	("ORB", po::value<std::string>(&ORB), "ORB service configuration file (Corba ORBSvcConf arg)")
	("DCPS", po::value<std::string>(&DCPS), "DCPS configuration file (OpenDDS DCPSConfigFile arg)")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, descripts), vm);
	po::notify(vm);

	if (vm.count("help") || !vm.count("ORB") || !vm.count("DCPS") || !vm.count("topic")) {
		std::cout << descripts << "\n";
		exit(1);
	}

}

///////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {

	std::string topicName;
	std::string ORB;
	std::string DCPS;

	parseArgs(argc, argv, topicName, ORB, DCPS);

	// we have to do this bit of translation since the 
	// DDS routines want arguments starting with a single dash,
	// whereas boost::program_options uses double dashes.
	ArgvParams subParams(argv[0]);
	subParams["-ORBSvcConf"] = ORB;
	subParams["-DCPSConfigFile"] = DCPS;

	// create the subscriber
	DDSSubscriber subscriber(subParams.argc(), subParams.argv());
	int subStatus = subscriber.status();
	if (subStatus)
		return subStatus;

	// create the reader
	TestReader reader(subscriber, topicName);

	// loop, and print status every 10 seconds
	int i = 0;
	ACE_Time_Value startTime = ACE_OS::gettimeofday();
	while (1) {
		// sleep for ten
		ACE_OS::sleep(10);
		// figure the elapsed time
		ACE_Time_Value stopTime = ACE_OS::gettimeofday();
		ACE_Time_Value deltaTime = stopTime - startTime;

		std::cout << setiosflags(std::ios_base::fixed);
		std::cout << std::setprecision(2);
		std::cout << std::setw(5);

		std::cout << i++ << "  " << deltaTime.sec() + (deltaTime.usec()/1.0e6)
				<< "  Samples:" << reader.numSamples()
				<< "  Dropped samples:" << reader.droppedSamples()
				<< "  Throughput:" << (reader.numBytes()/(deltaTime.sec()
				+ deltaTime.usec()/1.0e6))/1.0e6 << " MB/s\n";

		startTime = stopTime;
	}

	return 0;
}
