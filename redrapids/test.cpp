#include "RR314.h"
#include <iostream>
#include <iomanip>
#include <ios>
#include "boost/program_options.hpp"

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
};

//////////////////////////////////////////////////////////////////////
//

struct runParams
parseOptions(int argc, char** argv) {

  runParams params;
  params.simulate = false;

  // get the options
  po::options_description descripts ("Options");
  descripts.add_options()
    ("help", "describe options")
    ("simulate", "run in simulation mode")
    ("device",    po::value<int>(&params.device)->default_value(0),    "device number")
    ("gates",     po::value<int>(&params.gates)->default_value(1000),    "number of gates")
    ("nci",       po::value<int>(&params.nci)->default_value(100),       "number of coherent integrations")
    ("startiq",   po::value<int>(&params.startiq)->default_value(0),     "start gate for iq capture")
    ("numiq",     po::value<int>(&params.numiq)->default_value(100),     "number of gates for iq capture")
    ("decimation",po::value<int>(&params.decimation)->default_value(10), "decimation factor")
    ("xsvf",      po::value<std::string>(&params.xsvf)->default_value(""),"path to xsvf file")
    ("kaiser",    po::value<std::string>(&params.kaiser)->default_value(""),"path to kaiser coefficient file")
    ("gaussian",  po::value<std::string>(&params.gaussian)->default_value(""),"path to gaussian coefficient file")
    ;
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, descripts), vm);
  po::notify(vm);

  if (vm.count("gates")) {
    params.gates = vm["gates"].as<int>();
  }
  if (vm.count("simulate")) {
    params.simulate = true;
  }
  if (vm.count("help")) {
    std::cout << descripts << "\n";
    exit (1);
  }

  return params;
}

//////////////////////////////////////////////////////////////////////
//
// a task which just consumes the data from the
// rr314 as it becomes available
void *
dataTask(void* threadArg) {
  RR314* pRR314 = (RR314*) threadArg;
  
  while (1) {
    // loop while waiting for new buffers
    int* pBuf = pRR314->nextBuffer();
    pRR314->returnBuffer(pBuf);
  }
}

//////////////////////////////////////////////////////////////////////
//
// The main routine creates the card, starts it, and then just
// monitors the card activity.
int
main(int argc, char** argv) 
{

  runParams params = parseOptions(argc, argv);

  // create an RR314 card
  try {
    RR314 rr314(params.device,
		params.gates,
		params.nci,
		0,     // dual prt (0 or 1)
		params.startiq,
		params.numiq,
		params.decimation,
		params.xsvf,
		params.kaiser,
		params.gaussian,
	        params.simulate  // simulation mode?
		);
	      
    // create the data reading thread
    pthread_t dataThread;
    pthread_create(&dataThread, NULL, dataTask, (void*) &rr314);

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

