#include <iostream>
#include <iomanip>
#include "DDSSubscriber.h"
#include <boost/program_options.hpp>
namespace po = boost::program_options;

int main(int argc, char* argv[]) {

	std::string topicName;

	// get the options
	po::options_description descripts("Options");
	descripts.add_options() ("help", "describe options") ("topic", po::value<std::string>(&topicName), "DDS topic") 
	("ORBSvcConf", "service configuration file") 
	("DCPSConfigFile", "DCPS configuration file") 
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, descripts), vm);
	po::notify(vm);

	if (vm.count("help")) {
		std::cout << descripts << "\n";
		exit(1);
	}

	EldoraSubscriber subscriber(topicName);

  int subStatus = subscriber.run(argc, argv);

  if (subStatus)
    return subStatus;

  std::cout << setiosflags(std::ios_base::fixed);
  std::cout << std::setprecision(2);
  std::cout << std::setw(5);
  std::cout << "\n";
	
  int i = 0;
  ACE_Time_Value startTime = ACE_OS::gettimeofday();
  while(1) {
    ACE_OS::sleep(10);

    long numBytes = subscriber.numBytes();

    std::map<EldoraDDS::RadarChoice, long> seqErrors;
    seqErrors = subscriber.sequenceErrors();

    ACE_Time_Value stopTime = ACE_OS::gettimeofday();

    ACE_Time_Value deltaTime = stopTime - startTime;

    std::cout << i++ << "  " << deltaTime.sec() + (deltaTime.usec()/1.0e6) << "  "
	 << numBytes/(deltaTime.sec() + deltaTime.usec()*1.0e-6)/1.0e6 << " MB/s " 
	 << "Forward sequence errors:" << seqErrors[EldoraDDS::Forward]  
	 << " Aft sequence errors:"    << seqErrors[EldoraDDS::Aft] 
	 << "\n";

    startTime = stopTime;
  }

  return 0;
}
