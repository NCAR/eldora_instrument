#include <iostream>
#include <iomanip>
#include "EldoraSubscriber.h"
#include <boost/program_options.hpp>
#include "ArgvParams.h"
namespace po = boost::program_options;

int main(int argc, char* argv[]) {

	std::string topicName;
	std::string ORB;
	std::string DCPS;

	// get the options
	po::options_description descripts("Options");
	
	descripts.add_options() 
	("help", "describe options") 
	("topic", po::value<std::string>(&topicName), "DDS topic") 
	("ORB",   po::value<std::string>(&ORB),       "ORB service configuration file (Corba ORBSvcConf arg)") 
	("DCPS",  po::value<std::string>(&DCPS),      "DCPS configuration file (OpenDDS DCPSConfigFile arg)") 
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, descripts), vm);
	po::notify(vm);

	if (vm.count("help") || !vm.count("ORB") || !vm.count("DCPS") || !vm.count("topic")) {
		std::cout << descripts << "\n";
		exit(1);
	}
	
	// we have to do this bit of translation since the 
	// DDS routines want arguments starting with a single dash,
	// whereas boost::program_options uses double dashes.
	ArgvParams subParams(argv[0]);
	subParams["-ORBSvcConf"] = ORB;
	subParams["-DCPSConfigFile"] = DCPS;

	EldoraSubscriber subscriber(topicName);

  int subStatus = subscriber.run(subParams.argc(), subParams.argv());

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
